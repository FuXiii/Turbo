#include "TSurface.h"
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"

Turbo::Extension::TColorSpace::TColorSpace(TColorSpaceType colorSpaceType)
{
    this->colorSpaceType = colorSpaceType;
}

Turbo::Extension::TColorSpace::~TColorSpace()
{
}

Turbo::Extension::TColorSpaceType Turbo::Extension::TColorSpace::GetColorSpaceType()
{
    return this->colorSpaceType;
}

VkColorSpaceKHR Turbo::Extension::TColorSpace::GetVkColorSpaceKHR()
{
    return static_cast<VkColorSpaceKHR>(this->colorSpaceType);
}

std::string Turbo::Extension::TColorSpace::ToString()
{
    switch (this->colorSpaceType)
    {
    case Turbo::Extension::TColorSpaceType::UNDEFINED: {
        return std::string("UNDEFINED");
    }
    break;
    case Turbo::Extension::TColorSpaceType::SRGB_NONLINEAR: {
        return std::string("SRGB_NONLINEAR");
    }
    break;
    default: {
        return std::string("UNDEFINED");
    }
    break;
    }

    return std::string();
}

Turbo::Extension::TSurfaceFormat::TSurfaceFormat()
{
    this->format = Turbo::Core::TFormatInfo(Turbo::Core::TFormatType::UNDEFINED);
    this->colorSpace = TColorSpace(TColorSpaceType::UNDEFINED);
}

Turbo::Extension::TSurfaceFormat::TSurfaceFormat(Turbo::Core::TFormatInfo format, TColorSpace colorSpace)
{
    this->format = format;
    this->colorSpace = colorSpace;
}

Turbo::Extension::TSurfaceFormat::~TSurfaceFormat()
{
}

Turbo::Core::TFormatInfo Turbo::Extension::TSurfaceFormat::GetFormat()
{
    return this->format;
}

Turbo::Extension::TColorSpace Turbo::Extension::TSurfaceFormat::GetColorSpace()
{
    return this->colorSpace;
}

std::string Turbo::Extension::TSurfaceFormat::ToString()
{
    return std::string();
}

void Turbo::Extension::TSurface::InternalCreate()
{
    Turbo::Core::TInstance *instance = device->GetPhysicalDevice()->GetInstance();
    if (instance != nullptr && instance->GetVkInstance() != VK_NULL_HANDLE)
    {

#if defined(TURBO_PLATFORM_WINDOWS)
        if (!this->isExternalHandle)
        {
            if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE) && instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE))
            {
                VkWin32SurfaceCreateInfoKHR win32_surface_create_info = {};
                win32_surface_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
                win32_surface_create_info.pNext = nullptr;
                win32_surface_create_info.flags = 0;
                win32_surface_create_info.hinstance = this->hinstance;
                win32_surface_create_info.hwnd = this->hwnd;

                VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
                VkResult result = vkCreateWin32SurfaceKHR(instance->GetVkInstance(), &win32_surface_create_info, allocator, &this->vkSurfaceKHR);
                if (result != VK_SUCCESS)
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED);
                }
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
            }
        }
#elif defined(__APPLE__)
#elif defined(ANDROID) || defined(__ANDROID__)
#elif defined(__linux) || defined(__linux__)
#elif defined(__unix) || defined(__unix__)
#else
#endif
        this->GetSurfaceSupportQueueFamilys();
        this->GetSurfaceCapabilities();
        this->GetSurfaceSupportSurfaceFormats();
        this->GetSurfaceSupportPresentationMode();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
    }
}

void Turbo::Extension::TSurface::InternalDestroy()
{
    if (!this->isExternalHandle)
    {
        if (this->device != nullptr)
        {
            Turbo::Core::TInstance *instance = device->GetPhysicalDevice()->GetInstance();
            VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

            if (instance != nullptr && instance->GetVkInstance() != VK_NULL_HANDLE && this->vkSurfaceKHR != VK_NULL_HANDLE)
            {
                vkDestroySurfaceKHR(instance->GetVkInstance(), this->vkSurfaceKHR, allocator);
            }
        }
    }
}

#if defined(TURBO_PLATFORM_WINDOWS)
Turbo::Extension::TSurface::TSurface(Turbo::Core::TDevice *device, HINSTANCE hinstance, HWND hwnd)
{
    if (device != nullptr)
    {
        this->isExternalHandle = false;
        this->device = device;
        this->hinstance = hinstance;
        this->hwnd = hwnd;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
    }
}
#elif defined(__APPLE__)
Turbo::Extension::TSurface::TSurface(...)
{
}
#elif defined(ANDROID) || defined(__ANDROID__)
Turbo::Extension::TSurface::TSurface(Turbo::Core::TDevice *device, ANativeWindow *window)
{
    ...
}
#elif defined(__linux) || defined(__linux__)
Turbo::Extension::TSurface::TSurface(...)
{
}
#elif defined(__unix) || defined(__unix__)
Turbo::Extension::TSurface::TSurface(...)
{
}
#else
#endif

Turbo::Extension::TSurface::TSurface(Turbo::Core::TDevice *device, VkSurfaceKHR vkSurfaceKHR)
{
    if (device != nullptr && vkSurfaceKHR != VK_NULL_HANDLE)
    {
        this->isExternalHandle = true;
        this->device = device;
        this->vkSurfaceKHR = vkSurfaceKHR;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
    }
}

Turbo::Extension::TSurface::~TSurface()
{
    this->InternalDestroy();
}

void Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys()
{
    Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();
    size_t queue_family_count = physical_device->GetQueueFamilyCount();
    for (size_t queue_family_index = 0; queue_family_index < queue_family_count; queue_family_index++)
    {
        Turbo::Core::TQueueFamilyInfo queue_family = physical_device->GetQueueFamilyByIndex(static_cast<uint32_t>(queue_family_index));

        VkBool32 is_support_surface = VK_FALSE;

        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex(), this->vkSurfaceKHR, &is_support_surface);
        if (result == VK_SUCCESS)
        {
            if (is_support_surface == VK_TRUE)
            {
#if defined(TURBO_PLATFORM_WINDOWS)
                VkBool32 is_support_win32_presentation = vkGetPhysicalDeviceWin32PresentationSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex());
                if (is_support_win32_presentation == VK_TRUE)
                {
                    this->supportQueueFamilys.push_back(queue_family);
                }
                else
                {
                    throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result));
                }
#elif defined(__APPLE__)
#elif defined(ANDROID) || defined(__ANDROID__)
#elif defined(__linux) || defined(__linux__)
#elif defined(__unix) || defined(__unix__)
#else
#endif
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result));
        }
    }

    if (this->supportQueueFamilys.size() == 0)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
    }
}

void Turbo::Extension::TSurface::GetSurfaceCapabilities()
{
    Turbo::Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

    VkSurfaceCapabilitiesKHR surface_capanilities;
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_capanilities);
    if (result == VK_SUCCESS)
    {
        this->minImageCount = surface_capanilities.minImageCount;
        this->maxImageCount = surface_capanilities.maxImageCount;
        this->currentExtent = surface_capanilities.currentExtent;
        this->minImageExtent = surface_capanilities.minImageExtent;
        this->maxImageExtent = surface_capanilities.maxImageExtent;
        this->maxImageArrayLayers = surface_capanilities.maxImageArrayLayers;

        this->supportedTransforms = surface_capanilities.supportedTransforms;
        this->currentTransform = (TSurfaceTransformBits)surface_capanilities.currentTransform;
        this->supportedCompositeAlpha = surface_capanilities.supportedCompositeAlpha;
        this->supportedUsageFlags = surface_capanilities.supportedUsageFlags;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result));
    }
}

void Turbo::Extension::TSurface::GetSurfaceSupportSurfaceFormats()
{
    Turbo::Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

    uint32_t surface_format_count = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_format_count, nullptr);
    if (result == VK_SUCCESS && surface_format_count > 0)
    {
        std::vector<VkSurfaceFormatKHR> surface_formats;
        surface_formats.resize(surface_format_count);

        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_format_count, surface_formats.data());
        if (result == VK_SUCCESS)
        {
            std::vector<Turbo::Core::TFormatInfo> support_formats = physical_device->GetSupportFormats();
            size_t support_formats_size = support_formats.size();

            for (size_t surface_format_index = 0; surface_format_index < surface_format_count; surface_format_index++)
            {
                if (support_formats_size > 0)
                {
                    for (size_t support_format_index = 0; support_format_index < support_formats_size; support_format_index++)
                    {
                        if (support_formats[support_format_index].GetVkFormat() == surface_formats[surface_format_index].format)
                        {
                            Turbo::Extension::TSurfaceFormat surface_format;
                            surface_format.format = support_formats[support_format_index];
                            surface_format.colorSpace = Turbo::Extension::TColorSpace(static_cast<Turbo::Extension::TColorSpaceType>(surface_formats[surface_format_index].colorSpace));

                            this->surfaceFormats.push_back(surface_format);
                            break;
                        }
                    }
                }
                else
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
                }
            }

            if (this->surfaceFormats.size() == 0)
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result));
        }
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result));
    }
}

void Turbo::Extension::TSurface::GetSurfaceSupportPresentationMode()
{
    Turbo::Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

    uint32_t present_mode_count = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &present_mode_count, nullptr);
    if (result == VK_SUCCESS)
    {
        if (present_mode_count > 0)
        {
            std::vector<VkPresentModeKHR> vk_present_mode_khrs;
            vk_present_mode_khrs.resize(present_mode_count);
            result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &present_mode_count, vk_present_mode_khrs.data());
            if (result == VK_SUCCESS)
            {
                for (size_t present_mode_index = 0; present_mode_index < present_mode_count; present_mode_index++)
                {
                    switch (vk_present_mode_khrs[present_mode_index])
                    {
                    case VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR: {
                        this->presentModes.push_back(TPresentMode::IMMEDIATE);
                    }
                    break;
                    case VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR: {
                        this->presentModes.push_back(TPresentMode::MAILBOX);
                    }
                    break;
                    case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR: {
                        this->presentModes.push_back(TPresentMode::FIFO);
                    }
                    break;
                    case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR: {
                        this->presentModes.push_back(TPresentMode::FIFO_RELAXED);
                    }
                    break;
                    default: {
                    }
                    break;
                    }
                }
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result));
    }
}

VkSurfaceKHR Turbo::Extension::TSurface::GetVkSurfaceKHR()
{
    return this->vkSurfaceKHR;
}

uint32_t Turbo::Extension::TSurface::GetMinImageCount()
{
    return this->minImageCount;
}

uint32_t Turbo::Extension::TSurface::GetMaxImageCount()
{
    return this->maxImageCount;
}

Turbo::Core::TExtent2D Turbo::Extension::TSurface::GetCurrentExtent()
{
    return this->currentExtent;
}

uint32_t Turbo::Extension::TSurface::GetCurrentWidth()
{
    return this->currentExtent.width;
}

uint32_t Turbo::Extension::TSurface::GetCurrentHeight()
{
    return this->currentExtent.height;
}

Turbo::Core::TExtent2D Turbo::Extension::TSurface::GetMinImageExtent()
{
    return this->minImageExtent;
}

uint32_t Turbo::Extension::TSurface::GetMinWidth()
{
    return this->minImageExtent.width;
}

uint32_t Turbo::Extension::TSurface::GetMinHeight()
{
    return this->minImageExtent.height;
}

Turbo::Core::TExtent2D Turbo::Extension::TSurface::GetMaxImageExtent()
{
    return this->maxImageExtent;
}

uint32_t Turbo::Extension::TSurface::GetMaxWidth()
{
    return this->maxImageExtent.width;
}

uint32_t Turbo::Extension::TSurface::GetMaxHeight()
{
    return this->maxImageExtent.height;
}

uint32_t Turbo::Extension::TSurface::GetMaxImageArrayLayers()
{
    return this->maxImageArrayLayers;
}

bool Turbo::Extension::TSurface::IsSupportIdentityTransform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportRotate90Transform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportRotate180Transform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportRotate270Transform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorTransform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorRotate90Transform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorRotate180Transform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorRotate270Transform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportInheritTransform()
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaOpaque()
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaPreMultiplied()
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaPostMultiplied()
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaInherit()
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeImmediate()
{
    for (TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::IMMEDIATE)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeMailbox()
{
    for (TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::MAILBOX)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeFifo()
{
    for (TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::FIFO)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeFifoRelaxed()
{
    for (TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::FIFO_RELAXED)
        {
            return true;
        }
    }

    return false;
}

std::vector<Turbo::Extension::TSurfaceFormat> Turbo::Extension::TSurface::GetSurfaceFormats()
{
    return this->surfaceFormats;
}

std::vector<Turbo::Extension::TPresentMode> Turbo::Extension::TSurface::GetPresentModes()
{
    return this->presentModes;
}

Turbo::Extension::TSurfaceTransforms Turbo::Extension::TSurface::GetSupportedTransforms()
{
    return this->supportedTransforms;
}

Turbo::Extension::TSurfaceTransformBits Turbo::Extension::TSurface::GetCurrentTransform()
{
    return this->currentTransform;
}

Turbo::Extension::TCompositeAlphas Turbo::Extension::TSurface::GetSupportedCompositeAlpha()
{
    return this->supportedCompositeAlpha;
}

Turbo::Core::TImageUsages Turbo::Extension::TSurface::GetSupportedUsages()
{
    return this->supportedUsageFlags;
}

Turbo::Core::TDevice *Turbo::Extension::TSurface::GetDevice()
{
    return this->device;
}

std::string Turbo::Extension::TSurface::ToString()
{
    return std::string();
}
