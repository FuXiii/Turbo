#include "TSwapchain.h"
#include "TDevice.h"
#include "TException.h"
#include "TFence.h"
#include "TPhysicalDevice.h"
#include "TSemaphore.h"
#include "TSurface.h"
#include "TVulkanAllocator.h"

void Turbo::Extension::TSwapchain::InternalCreate()
{
    Turbo::Core::TDevice *device = this->surface->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    if (device != nullptr && vk_device != VK_NULL_HANDLE && device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
    {
        // imageCount
        if ((this->surface->GetMinImageCount() > this->minImageCount) || (this->surface->GetMaxImageCount() < this->minImageCount))
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
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
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

        // width
        if ((this->surface->GetMinWidth() > this->width) || (this->surface->GetMaxWidth() < this->width))
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

        // height
        if ((this->surface->GetMinHeight() > this->height) || (this->surface->GetMaxHeight() < this->height))
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

        // arrayLayers
        if (this->surface->GetMaxImageArrayLayers() > this->imageArrayLayers)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

        // usages
        Turbo::Core::TImageUsages surface_support_images_usages = this->surface->GetSupportedUsages();
        if ((surface_support_images_usages & this->usages) != this->usages)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

        // transform
        TSurfaceTransforms surface_support_transforms = this->surface->GetSupportedTransforms();
        if ((surface_support_transforms & this->transform) != this->transform)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

        // compositeAlpha
        TCompositeAlphas surface_support_composite_alphas = this->surface->GetSupportedCompositeAlpha();
        if ((surface_support_composite_alphas & this->compositeAlpha) != this->compositeAlpha)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
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
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }

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
        vk_swapchain_create_info_khr.oldSwapchain = nullptr;

        VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

        VkResult result = vkCreateSwapchainKHR(vk_device, &vk_swapchain_create_info_khr, allocator, &this->vkSwapchainKHR);
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED);
        }

        // Get VkImages
        uint32_t image_count;
        result = vkGetSwapchainImagesKHR(vk_device, this->vkSwapchainKHR, &image_count, nullptr);
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED);
        }

        std::vector<VkImage> vk_images;
        vk_images.resize(image_count);
        result = vkGetSwapchainImagesKHR(vk_device, this->vkSwapchainKHR, &image_count, vk_images.data());
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED);
        }

        for (VkImage vk_image_item : vk_images)
        {
            Turbo::Core::TImage *image = new Turbo::Core::TImage(this->surface->GetDevice(), vk_image_item, 0, Core::TImageType::DIMENSION_2D, this->format, this->width, this->height, 1, 1, this->imageArrayLayers, Core::TSampleCountBits::SAMPLE_1_BIT, Core::TImageTiling::OPTIMAL, this->usages, Core::TImageLayout::UNDEFINED);
            this->images.push_back(image);
        }
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
    }
}

void Turbo::Extension::TSwapchain::InternalDestroy()
{
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vkDestroySwapchainKHR(this->surface->GetDevice()->GetVkDevice(), this->vkSwapchainKHR, allocator);
}

Turbo::Extension::TSwapchain::TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped)
{
    if (surface != nullptr)
    {
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

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
    }
}

Turbo::Extension::TSwapchain::TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped)
{
    if (surface != nullptr)
    {
        this->surface = surface;
        this->minImageCount = minImageCount;
        this->format = format;
        this->width = surface->GetCurrentWidth();
        this->height = surface->GetCurrentHeight();
        this->imageArrayLayers = 1;
        this->usages = usages;
        this->transform = surface->GetCurrentTransform();

        TCompositeAlphas support_composite_alphas = this->surface->GetSupportedCompositeAlpha();
        if (!this->surface->IsSupportCompositeAlphaOpaque())
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED);
        }
        this->compositeAlpha = TCompositeAlphaBits::ALPHA_OPAQUE_BIT;

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

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER);
    }
}

Turbo::Extension::TSwapchain::~TSwapchain()
{
    this->InternalDestroy();
}

const std::vector<Turbo::Core::TImage *> &Turbo::Extension::TSwapchain::GetImages()
{
    return this->images;
}

Turbo::Core::TResult Turbo::Extension::TSwapchain::AcquireNextImage(uint64_t timeout, Turbo::Core::TSemaphore *signalSemphore, Turbo::Core::TFence *signalFence, uint32_t *index)
{
    if (index != nullptr)
    {
        VkSemaphore signal_semaphore = VK_NULL_HANDLE;
        VkFence signal_fence = VK_NULL_HANDLE;

        if (signalSemphore != nullptr)
        {
            signal_semaphore = signalSemphore->GetVkSemaphore();
        }

        if (signalFence != nullptr)
        {
            signal_fence = signalFence->GetVkFence();
        }

        Turbo::Core::TDevice *device = this->surface->GetDevice();
        VkDevice vk_device = device->GetVkDevice();
        VkResult result = vkAcquireNextImageKHR(vk_device, this->vkSwapchainKHR, timeout, signal_semaphore, signal_fence, index);
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
        case VkResult::VK_SUBOPTIMAL_KHR: {
            return Turbo::Core::TResult::SUBOPTIMAL;
        }
        break;
        default: {
            throw Turbo::Core::TException(Turbo::Core::TResult::FAIL);
        }
        break;
        }
    }

    return Turbo::Core::TResult::INVALID_PARAMETER;
}

Turbo::Core::TResult Turbo::Extension::TSwapchain::AcquireNextImageUntil(Turbo::Core::TSemaphore *signalSemphore, Turbo::Core::TFence *signalFence, uint32_t *index)
{
    return this->AcquireNextImage(UINT64_MAX, signalSemphore, signalFence, index);
}

std::string Turbo::Extension::TSwapchain::ToString()
{
    return std::string();
}
