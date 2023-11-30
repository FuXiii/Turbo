#include "TSwapchain.h"
#include "TDevice.h"
#include "TException.h"
#include "TFence.h"
#include "TPhysicalDevice.h"
#include "TSemaphore.h"
#include "TSurface.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Extension::TSwapchain::InternalCreate()
{
    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = this->surface->GetDevice();
    if (device.Valid() && device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
    {
        VkDevice vk_device = device->GetVkDevice();

        // imageCount
        if ((this->surface->GetMinImageCount() > this->minImageCount) || (this->surface->GetMaxImageCount() < this->minImageCount))
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "this minImage count out the range of [surface.minImageCount , surface.maxImageCount]");
        }

        // surfaceFormat
        std::vector<Turbo::Extension::TSurfaceFormat> surface_formats = this->surface->GetSurfaceFormats();
        bool is_support_surface_format = false;
        for (Turbo::Extension::TSurfaceFormat &surface_format_item : surface_formats)
        {
            if (this->format.GetVkFormat() == surface_format_item.GetFormat().GetVkFormat())
            {
                is_support_surface_format = true;
                break;
            }
        }

        if (!is_support_surface_format)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "this swapchain.format cant not compatible with surface");
        }

        // width
        if ((this->surface->GetMinWidth() > this->width) || (this->surface->GetMaxWidth() < this->width))
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the extent of swapchain out range of [surface.minExtent surface.maxExtent]");
        }

        // height
        if ((this->surface->GetMinHeight() > this->height) || (this->surface->GetMaxHeight() < this->height))
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the extent of swapchain out range of [surface.minExtent surface.maxExtent]");
        }

        // arrayLayers
        if (this->surface->GetMaxImageArrayLayers() < this->imageArrayLayers)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the arrary of swapchain out range of [surface.minArrary surface.maxArrary]");
        }

        // usages
        Turbo::Core::TImageUsages surface_support_images_usages = this->surface->GetSupportedUsages();
        if ((surface_support_images_usages & this->usages) != this->usages)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the usage of swapchain can not compatible with surface");
        }

        // transform
        TSurfaceTransforms surface_support_transforms = this->surface->GetSupportedTransforms();
        if ((surface_support_transforms & this->transform) != this->transform)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the transform of swapchain can not compatible with surface");
        }

        // compositeAlpha
        TCompositeAlphas surface_support_composite_alphas = this->surface->GetSupportedCompositeAlpha();
        if ((surface_support_composite_alphas & this->compositeAlpha) != this->compositeAlpha)
        {
            if ((this->compositeAlpha & TCompositeAlphaBits::ALPHA_OPAQUE_BIT) != TCompositeAlphaBits::ALPHA_OPAQUE_BIT)
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the composite alpha of swapchain can not compatible with surface");
            }
        }

        // presentMode
        std::vector<TPresentMode> surface_support_present_modes = this->surface->GetPresentModes();
        bool is_support_present_mode = false;
        for (TPresentMode &present_mode_item : surface_support_present_modes)
        {
            if (this->presentMode == present_mode_item)
            {
                is_support_present_mode = true;
                break;
            }
        }

        if (!is_support_present_mode)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::InternalCreate", "the present mode of swapchain can not compatible with surface");
        }
        this->minImageCount = 3;
        VkSwapchainCreateInfoKHR vk_swapchain_create_info_khr = {};
        vk_swapchain_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        vk_swapchain_create_info_khr.pNext = nullptr;
        vk_swapchain_create_info_khr.flags = 0;
        vk_swapchain_create_info_khr.surface = this->surface->GetVkSurfaceKHR();
        vk_swapchain_create_info_khr.minImageCount = this->minImageCount;
        vk_swapchain_create_info_khr.imageFormat = this->format.GetVkFormat();
        vk_swapchain_create_info_khr.imageColorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        vk_swapchain_create_info_khr.imageExtent.width = this->width;
        vk_swapchain_create_info_khr.imageExtent.height = this->height;
        vk_swapchain_create_info_khr.imageArrayLayers = this->imageArrayLayers;
        vk_swapchain_create_info_khr.imageUsage = this->usages;
        vk_swapchain_create_info_khr.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
        vk_swapchain_create_info_khr.queueFamilyIndexCount = 0;
        vk_swapchain_create_info_khr.pQueueFamilyIndices = nullptr;
        vk_swapchain_create_info_khr.preTransform = (VkSurfaceTransformFlagBitsKHR)this->transform;
        vk_swapchain_create_info_khr.compositeAlpha = (VkCompositeAlphaFlagBitsKHR)this->compositeAlpha;
        vk_swapchain_create_info_khr.presentMode = (VkPresentModeKHR)this->presentMode;
        vk_swapchain_create_info_khr.clipped = VK_FALSE;
        if (this->isClipped)
        {
            vk_swapchain_create_info_khr.clipped = VK_TRUE;
        }
        vk_swapchain_create_info_khr.oldSwapchain = VK_NULL_HANDLE;
        if (this->oldSwapchain.Valid())
        {
            vk_swapchain_create_info_khr.oldSwapchain = this->oldSwapchain->GetVkSwapchainKHR();
        }

        VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

        VkResult result = this->vkCreateSwapchainKHR(vk_device, &vk_swapchain_create_info_khr, allocator, &this->vkSwapchainKHR);
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSwapchain::InternalCreate::vkCreateSwapchainKHR");
        }

        // Get VkImages
        uint32_t image_count;
        result = this->vkGetSwapchainImagesKHR(vk_device, this->vkSwapchainKHR, &image_count, nullptr);
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSwapchain::InternalCreate::vkGetSwapchainImagesKHR");
        }

        std::vector<VkImage> vk_images;
        vk_images.resize(image_count);
        result = this->vkGetSwapchainImagesKHR(vk_device, this->vkSwapchainKHR, &image_count, vk_images.data());
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSwapchain::InternalCreate::vkGetSwapchainImagesKHR");
        }

        for (VkImage vk_image_item : vk_images)
        {
            Turbo::Core::TImage *image = new Turbo::Core::TImage(this->surface->GetDevice(), vk_image_item, 0, Core::TImageType::DIMENSION_2D, this->format, this->width, this->height, 1, 1, this->imageArrayLayers, Core::TSampleCountBits::SAMPLE_1_BIT, Core::TImageTiling::OPTIMAL, this->usages, Core::TImageLayout::UNDEFINED);
            this->images.push_back(image);
        }
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::InternalCreate");
    }
}

void Turbo::Extension::TSwapchain::InternalDestroy()
{
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->vkDestroySwapchainKHR(this->surface->GetDevice()->GetVkDevice(), this->vkSwapchainKHR, allocator);
}

Turbo::Extension::TSwapchain::TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped)
{
    if (surface.Valid())
    {
        Turbo::Core::TDevice *device = surface->GetDevice();
        if (device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
        {
            this->vkCreateSwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkCreateSwapchainKHR>(device, "vkCreateSwapchainKHR");
            this->vkGetSwapchainImagesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkGetSwapchainImagesKHR>(device, "vkGetSwapchainImagesKHR");
            this->vkDestroySwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkDestroySwapchainKHR>(device, "vkDestroySwapchainKHR");
            this->vkAcquireNextImageKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkAcquireNextImageKHR>(device, "vkAcquireNextImageKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSwapchain::TSwapchain", "Please enable the VK_KHR_swapchain extension");
        }

        this->surface = surface;
        this->minImageCount = minImageCount;
        this->format = format;
        this->width = width;
        this->height = height;
        this->imageArrayLayers = imageArrayLayers;
        this->usages = usages;
        this->transform = transform;
        this->compositeAlpha = compositeAlpha;
        this->presentMode = presentMode;
        this->isClipped = isClipped;
        this->oldSwapchain = nullptr;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain");
    }
}

Turbo::Extension::TSwapchain::TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped)
{
    if (surface.Valid())
    {
        Turbo::Core::TDevice *device = surface->GetDevice();
        if (device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
        {
            this->vkCreateSwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkCreateSwapchainKHR>(device, "vkCreateSwapchainKHR");
            this->vkGetSwapchainImagesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkGetSwapchainImagesKHR>(device, "vkGetSwapchainImagesKHR");
            this->vkDestroySwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkDestroySwapchainKHR>(device, "vkDestroySwapchainKHR");
            this->vkAcquireNextImageKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkAcquireNextImageKHR>(device, "vkAcquireNextImageKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSwapchain::TSwapchain", "Please enable the VK_KHR_swapchain extension");
        }

        this->surface = surface;
        this->minImageCount = minImageCount;
        this->format = format;
        this->width = surface->GetCurrentWidth();
        this->height = surface->GetCurrentHeight();
        this->imageArrayLayers = 1;
        this->usages = usages;
        this->transform = surface->GetCurrentTransform();

        if (this->surface->IsSupportCompositeAlphaInherit())
        {
            this->compositeAlpha = TCompositeAlphaBits::ALPHA_INHERIT_BIT;
        }

        if (this->surface->IsSupportCompositeAlphaOpaque())
        {
            this->compositeAlpha = TCompositeAlphaBits::ALPHA_OPAQUE_BIT;
        }

        if (this->compositeAlpha == 0)
        {
            // throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::TSwapchain", "this surface unsupport CompositeAlphaOpaque");
            this->compositeAlpha = TCompositeAlphaBits::ALPHA_OPAQUE_BIT;
        }

        if (this->surface->IsSupportPresentModeFifo())
        {
            this->presentMode = TPresentMode::FIFO;
        }
        else if (this->surface->IsSupportPresentModeFifoRelaxed())
        {
            this->presentMode = TPresentMode::FIFO_RELAXED;
        }
        else if (this->surface->IsSupportPresentModeMailbox())
        {
            this->presentMode = TPresentMode::MAILBOX;
        }
        else
        {
            this->presentMode = TPresentMode::IMMEDIATE;
        }

        this->isClipped = isClipped;
        this->oldSwapchain = nullptr;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain");
    }
}

Turbo::Extension::TSwapchain::TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatType formatType, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped)
{
    if (surface.Valid())
    {
        Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = surface->GetDevice();
        if (device.Valid())
        {
            if (device->GetPhysicalDevice()->IsSupportFormat(formatType))
            {
                this->format = device->GetPhysicalDevice()->GetFormatInfo(formatType);
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::TSwapchain", "Unsupport format");
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain", "device is invalid");
        }

        if (device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
        {
            this->vkCreateSwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkCreateSwapchainKHR>(device, "vkCreateSwapchainKHR");
            this->vkGetSwapchainImagesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkGetSwapchainImagesKHR>(device, "vkGetSwapchainImagesKHR");
            this->vkDestroySwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkDestroySwapchainKHR>(device, "vkDestroySwapchainKHR");
            this->vkAcquireNextImageKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkAcquireNextImageKHR>(device, "vkAcquireNextImageKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSwapchain::TSwapchain", "Please enable the VK_KHR_swapchain extension");
        }

        this->surface = surface;
        this->minImageCount = minImageCount;
        this->width = width;
        this->height = height;
        this->imageArrayLayers = imageArrayLayers;
        this->usages = usages;
        this->transform = transform;
        this->compositeAlpha = compositeAlpha;
        this->presentMode = presentMode;
        this->isClipped = isClipped;
        this->oldSwapchain = nullptr;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain");
    }
}

Turbo::Extension::TSwapchain::TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatType formatType, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped)
{
    if (surface.Valid())
    {
        Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = surface->GetDevice();
        if (device.Valid())
        {
            if (device->GetPhysicalDevice()->IsSupportFormat(formatType))
            {
                this->format = device->GetPhysicalDevice()->GetFormatInfo(formatType);
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::TSwapchain", "Unsupport format");
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain", "device is invalid");
        }

        if (device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
        {
            this->vkCreateSwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkCreateSwapchainKHR>(device, "vkCreateSwapchainKHR");
            this->vkGetSwapchainImagesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkGetSwapchainImagesKHR>(device, "vkGetSwapchainImagesKHR");
            this->vkDestroySwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkDestroySwapchainKHR>(device, "vkDestroySwapchainKHR");
            this->vkAcquireNextImageKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkAcquireNextImageKHR>(device, "vkAcquireNextImageKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSwapchain::TSwapchain", "Please enable the VK_KHR_swapchain extension");
        }

        this->surface = surface;
        this->minImageCount = minImageCount;
        this->width = surface->GetCurrentWidth();
        this->height = surface->GetCurrentHeight();
        this->imageArrayLayers = 1;
        this->usages = usages;
        this->transform = surface->GetCurrentTransform();

        if (this->surface->IsSupportCompositeAlphaInherit())
        {
            this->compositeAlpha = TCompositeAlphaBits::ALPHA_INHERIT_BIT;
        }

        if (this->surface->IsSupportCompositeAlphaOpaque())
        {
            this->compositeAlpha = TCompositeAlphaBits::ALPHA_OPAQUE_BIT;
        }

        if (this->compositeAlpha == 0)
        {
            // throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSwapchain::TSwapchain", "this surface unsupport CompositeAlphaOpaque");
            this->compositeAlpha = TCompositeAlphaBits::ALPHA_OPAQUE_BIT;
        }

        if (this->surface->IsSupportPresentModeFifo())
        {
            this->presentMode = TPresentMode::FIFO;
        }
        else if (this->surface->IsSupportPresentModeFifoRelaxed())
        {
            this->presentMode = TPresentMode::FIFO_RELAXED;
        }
        else if (this->surface->IsSupportPresentModeMailbox())
        {
            this->presentMode = TPresentMode::MAILBOX;
        }
        else
        {
            this->presentMode = TPresentMode::IMMEDIATE;
        }

        this->isClipped = isClipped;
        this->oldSwapchain = nullptr;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain");
    }
}

Turbo::Extension::TSwapchain::TSwapchain(const Turbo::Core::TRefPtr<TSwapchain> &oldSwapchain)
{
    if (oldSwapchain.Valid())
    {
        Turbo::Core::TDevice *device = oldSwapchain->GetSurface()->GetDevice();
        if (device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
        {
            this->vkCreateSwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkCreateSwapchainKHR>(device, "vkCreateSwapchainKHR");
            this->vkGetSwapchainImagesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkGetSwapchainImagesKHR>(device, "vkGetSwapchainImagesKHR");
            this->vkDestroySwapchainKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkDestroySwapchainKHR>(device, "vkDestroySwapchainKHR");
            this->vkAcquireNextImageKHR = Turbo::Core::TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkAcquireNextImageKHR>(device, "vkAcquireNextImageKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSwapchain::TSwapchain", "Please enable the VK_KHR_swapchain extension");
        }

        this->surface = oldSwapchain->GetSurface();
        this->minImageCount = oldSwapchain->GetMinImageCount();
        this->format = oldSwapchain->GetFormat();
        this->width = surface->GetCurrentWidth();
        this->height = surface->GetCurrentHeight();
        this->imageArrayLayers = oldSwapchain->GetImageArrayLayers();
        this->usages = oldSwapchain->GetUsages();
        this->transform = oldSwapchain->GetTransform();
        this->compositeAlpha = oldSwapchain->GetCompositeAlpha();
        this->presentMode = oldSwapchain->GetPresentMode();
        this->isClipped = oldSwapchain->GetIsClipped();
        this->oldSwapchain = oldSwapchain;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSwapchain::TSwapchain");
    }
}

Turbo::Extension::TSwapchain::~TSwapchain()
{
    this->InternalDestroy();
}

Turbo::Core::TRefPtr<Turbo::Extension::TSurface> Turbo::Extension::TSwapchain::GetSurface()
{
    return this->surface;
}

const std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImage>> &Turbo::Extension::TSwapchain::GetImages()
{
    return this->images;
}

Turbo::Core::TResult Turbo::Extension::TSwapchain::AcquireNextImage(uint64_t timeout, const Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> &signalSemphore, const Turbo::Core::TRefPtr<Turbo::Core::TFence> &signalFence, uint32_t *index)
{
    if (index != nullptr)
    {
        VkSemaphore signal_semaphore = VK_NULL_HANDLE;
        VkFence signal_fence = VK_NULL_HANDLE;

        if (signalSemphore.Valid())
        {
            signal_semaphore = signalSemphore->GetVkSemaphore();
        }

        if (signalFence.Valid())
        {
            signal_fence = signalFence->GetVkFence();
        }

        Turbo::Core::TDevice *device = this->surface->GetDevice();
        VkDevice vk_device = device->GetVkDevice();
        VkResult result = this->vkAcquireNextImageKHR(vk_device, this->vkSwapchainKHR, timeout, signal_semaphore, signal_fence, index);
        switch (result)
        {
        case VkResult::VK_SUCCESS: {
            return Turbo::Core::TResult::SUCCESS;
        }
        break;
        case VkResult::VK_TIMEOUT: {
            return Turbo::Core::TResult::TIMEOUT;
        }
        break;
        case VkResult::VK_NOT_READY: {
            return Turbo::Core::TResult::NOT_READY;
        }
        break;
        case VkResult::VK_ERROR_OUT_OF_DATE_KHR:
        case VkResult::VK_SUBOPTIMAL_KHR: {
            return Turbo::Core::TResult::MISMATCH;
        }
        break;
        default: {
            throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "Turbo::Extension::TSwapchain::AcquireNextImage::vkAcquireNextImageKHR");
        }
        break;
        }
    }

    return Turbo::Core::TResult::INVALID_PARAMETER;
}

Turbo::Core::TResult Turbo::Extension::TSwapchain::AcquireNextImageUntil(const Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> &signalSemphore, const Turbo::Core::TRefPtr<Turbo::Core::TFence> &signalFence, uint32_t *index)
{
    return this->AcquireNextImage(UINT64_MAX, signalSemphore, signalFence, index);
}

VkSwapchainKHR Turbo::Extension::TSwapchain::GetVkSwapchainKHR()
{
    return this->vkSwapchainKHR;
}

uint32_t Turbo::Extension::TSwapchain::GetMinImageCount()
{
    return this->minImageCount;
}

Turbo::Core::TFormatInfo Turbo::Extension::TSwapchain::GetFormat()
{
    return this->format;
}

uint32_t Turbo::Extension::TSwapchain::GetWidth()
{
    return this->width;
}

uint32_t Turbo::Extension::TSwapchain::GetHeight()
{
    return this->height;
}

uint32_t Turbo::Extension::TSwapchain::GetImageArrayLayers()
{
    return this->imageArrayLayers;
}

Turbo::Core::TImageUsages Turbo::Extension::TSwapchain::GetUsages()
{
    return this->usages;
}

Turbo::Extension::TSurfaceTransformBits Turbo::Extension::TSwapchain::GetTransform()
{
    return this->transform;
}

Turbo::Extension::TCompositeAlphaBits Turbo::Extension::TSwapchain::GetCompositeAlpha()
{
    return this->compositeAlpha;
}

Turbo::Extension::TPresentMode Turbo::Extension::TSwapchain::GetPresentMode()
{
    return this->presentMode;
}

bool Turbo::Extension::TSwapchain::GetIsClipped()
{
    return this->isClipped;
}

std::string Turbo::Extension::TSwapchain::ToString()
{
    return std::string();
}
