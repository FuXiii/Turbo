#include "TImageView.h"
#include "TDevice.h"
#include "TException.h"
#include "TImage.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TImageView::InternalCreate()
{
    VkComponentMapping vk_component_mapping = {};
    vk_component_mapping.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R;
    vk_component_mapping.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G;
    vk_component_mapping.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B;
    vk_component_mapping.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A;

    VkImageSubresourceRange vk_image_subresource_range = {};
    vk_image_subresource_range.aspectMask = this->aspectMask;
    vk_image_subresource_range.baseMipLevel = this->baseMipLevel;
    vk_image_subresource_range.levelCount = this->levelCount;
    vk_image_subresource_range.baseArrayLayer = this->baseArrayLayer;
    vk_image_subresource_range.layerCount = this->layerCount;

    VkImageViewCreateInfo vk_image_view_create_info = {};
    vk_image_view_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vk_image_view_create_info.pNext = nullptr;
    vk_image_view_create_info.flags = 0;
    vk_image_view_create_info.image = this->image->GetVkImage();
    vk_image_view_create_info.viewType = this->viewType;
    vk_image_view_create_info.format = this->format.GetVkFormat();
    vk_image_view_create_info.components = vk_component_mapping;
    vk_image_view_create_info.subresourceRange = vk_image_subresource_range;

    VkDevice vk_device = this->image->GetDevice()->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateImageView(vk_device, &vk_image_view_create_info, allocator, &this->vkImageView);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
    }
}

void Turbo::Core::TImageView::InternalDestroy()
{
    VkDevice vk_device = this->image->GetDevice()->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vkDestroyImageView(vk_device, this->vkImageView, allocator);
    this->vkImageView = VK_NULL_HANDLE;
}

Turbo::Core::TImageView::TImageView(TImage *image, VkImageViewType viewType, TFormatInfo format, VkImageAspectFlags aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (image != nullptr && image->GetVkImage() != VK_NULL_HANDLE)
    {
        this->image = image;
        this->viewType = viewType;
        this->format = format;
        this->aspectMask = aspectMask;
        this->baseMipLevel = baseMipLevel;
        this->levelCount = levelCount;
        this->baseArrayLayer = baseArrayLayer;
        this->layerCount = layerCount;

        this->InternalCreate();
    }
    else
    {
        throw TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TImageView::~TImageView()
{
    this->InternalDestroy();
}

Turbo::Core::TImage *Turbo::Core::TImageView::GetImage()
{
    return this->image;
}

VkImageView Turbo::Core::TImageView::GetVkImageView()
{
    return this->vkImageView;
}

std::string Turbo::Core::TImageView::ToString()
{
    return std::string();
}
