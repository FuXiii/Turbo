#include "TImageView.h"
#include "TDevice.h"
#include "TException.h"
#include "TImage.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TImageView::InternalCreate()
{
    VkComponentMapping vk_component_mapping = {};
    vk_component_mapping.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R;
    vk_component_mapping.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G;
    vk_component_mapping.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B;
    vk_component_mapping.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A;

    VkImageSubresourceRange vk_image_subresource_range = {};
    vk_image_subresource_range.aspectMask = this->aspects;
    vk_image_subresource_range.baseMipLevel = this->baseMipLevel;
    vk_image_subresource_range.levelCount = this->levelCount;
    vk_image_subresource_range.baseArrayLayer = this->baseArrayLayer;
    vk_image_subresource_range.layerCount = this->layerCount;

    VkImageViewCreateInfo vk_image_view_create_info = {};
    vk_image_view_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vk_image_view_create_info.pNext = nullptr;
    vk_image_view_create_info.flags = 0;
    vk_image_view_create_info.image = this->image->GetVkImage();
    vk_image_view_create_info.viewType = (VkImageViewType)this->viewType;
    vk_image_view_create_info.format = this->format.GetVkFormat();
    vk_image_view_create_info.components = vk_component_mapping;
    vk_image_view_create_info.subresourceRange = vk_image_subresource_range;

    TDevice *device = this->image->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = device->GetDeviceDriver()->vkCreateImageView(vk_device, &vk_image_view_create_info, allocator, &this->vkImageView);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TImageView::InternalCreate::vkCreateImageView");
    }
}

void Turbo::Core::TImageView::InternalDestroy()
{
    TDevice *device = this->image->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    device->GetDeviceDriver()->vkDestroyImageView(vk_device, this->vkImageView, allocator);
    this->vkImageView = VK_NULL_HANDLE;
}

Turbo::Core::TImageView::TImageView(const TRefPtr<TImage> &image, TImageViewType viewType, TFormatInfo format, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (image.Valid())
    {
        this->image = image;
        this->viewType = viewType;
        this->format = format;
        this->aspects = aspects;
        this->baseMipLevel = baseMipLevel;
        this->levelCount = levelCount;
        this->baseArrayLayer = baseArrayLayer;
        this->layerCount = layerCount;

        this->InternalCreate();
    }
    else
    {
        throw TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImageView::TImageView");
    }
}

Turbo::Core::TImageView::TImageView(const TRefPtr<TImage> &image, TImageViewType viewType, TFormatType formatType, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (image.Valid())
    {
        TPhysicalDevice *physical_device = image->GetDevice()->GetPhysicalDevice();
        if (physical_device->IsSupportFormat(formatType))
        {
            this->format = physical_device->GetFormatInfo(formatType);
        }
        else
        {
            throw TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImageView::TImageView", "Unsupport format");
        }

        this->image = image;
        this->viewType = viewType;
        this->aspects = aspects;
        this->baseMipLevel = baseMipLevel;
        this->levelCount = levelCount;
        this->baseArrayLayer = baseArrayLayer;
        this->layerCount = layerCount;

        this->InternalCreate();
    }
    else
    {
        throw TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImageView::TImageView");
    }
}

Turbo::Core::TImageView::~TImageView()
{
    this->InternalDestroy();
}

const Turbo::Core::TRefPtr<Turbo::Core::TImage> &Turbo::Core::TImageView::GetImage()
{
    return this->image;
}

VkImageView Turbo::Core::TImageView::GetVkImageView()
{
    return this->vkImageView;
}

Turbo::Core::TImageViewType Turbo::Core::TImageView::GetViewType() const
{
    return this->viewType;
}

Turbo::Core::TFormatInfo Turbo::Core::TImageView::GetFormat() const
{
    return this->format;
}

Turbo::Core::TImageAspects Turbo::Core::TImageView::GetAspects() const
{
    return this->aspects;
}

uint32_t Turbo::Core::TImageView::GetBaseMipLevel() const
{
    return this->baseMipLevel;
}

uint32_t Turbo::Core::TImageView::GetLevelCount() const
{
    return this->levelCount;
}

uint32_t Turbo::Core::TImageView::GetBaseArrayLayer() const
{
    return this->baseArrayLayer;
}

uint32_t Turbo::Core::TImageView::GetLayerCount() const
{
    return this->layerCount;
}

std::string Turbo::Core::TImageView::ToString()
{
    return std::string();
}
