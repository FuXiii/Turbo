#include "TImage.h"
#include "TDevice.h"
#include "TException.h"
#include "TVmaAllocator.h"

#include "vk_mem_alloc.h"

void Turbo::Core::TImage::InternalCreate()
{
    VkImageType vk_image_type = VkImageType::VK_IMAGE_TYPE_1D;
    switch (this->type)
    {
    case DIMENSION_1D: {
        vk_image_type = VkImageType::VK_IMAGE_TYPE_1D;
    }
    break;
    case DIMENSION_2D: {
        vk_image_type = VkImageType::VK_IMAGE_TYPE_2D;
    }
    break;
    case DIMENSION_3D: {
        vk_image_type = VkImageType::VK_IMAGE_TYPE_3D;
    }
    break;
    }

    VkImageTiling vk_image_tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
    switch (this->tiling)
    {
    case OPTIMAL: {
        vk_image_tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
    }
    break;
    case LINEAR: {
        vk_image_tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    }
    break;
    }

    VkImageLayout vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

    switch (this->layout)
    {
    case LAYOUT_UNDEFINED: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    }
    break;
    case LAYOUT_GENERAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;
    }
    break;
    case LAYOUT_COLOR_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    break;
    case LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }
    break;
    case LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    }
    break;
    case LAYOUT_SHADER_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
    }
    break;
    case LAYOUT_TRANSFER_SRC_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    }
    break;
    case LAYOUT_TRANSFER_DST_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    }
    break;
    case LAYOUT_PREINITIALIZED: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED;
    }
    break;
    case LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
    }
    break;
    case LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
    }
    break;
    case LAYOUT_DEPTH_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    }
    break;
    case LAYOUT_DEPTH_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
    }
    break;
    case LAYOUT_STENCIL_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
    }
    break;
    case LAYOUT_STENCIL_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    break;
    case LAYOUT_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    }
    break;
    case LAYOUT_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    }
    break;
    case LAYOUT_PRESENT_SRC_KHR: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    break;
    }

    VkImageCreateInfo vk_image_create_info = {};
    vk_image_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vk_image_create_info.pNext = nullptr;
    vk_image_create_info.flags = this->imageFlags;
    vk_image_create_info.imageType = vk_image_type;
    vk_image_create_info.format = this->format.GetVkFormat();
    vk_image_create_info.extent = this->extent;
    vk_image_create_info.mipLevels = this->mipLevels;
    vk_image_create_info.arrayLayers = this->arrayLayers;
    vk_image_create_info.samples = (VkSampleCountFlagBits)this->samples;
    vk_image_create_info.tiling = vk_image_tiling;
    vk_image_create_info.usage = this->usages;
    // vk_image_create_info.sharingMode;//By Vma?
    // vk_image_create_info.queueFamilyIndexCount;//By Vma?
    // vk_image_create_info.pQueueFamilyIndices;//By Vma?
    vk_image_create_info.initialLayout = vk_image_layout;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = this->memoryFlags;

    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());

    VkResult result = vmaCreateImage(*vma_allocator, &vk_image_create_info, &alloc_info, &this->vkImage, (VmaAllocation *)this->vmaAllocation, nullptr);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
    }
}

void Turbo::Core::TImage::InternalDestroy()
{
    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    VmaAllocation *vma_allocation = (VmaAllocation *)this->vmaAllocation;
    vmaDestroyImage(*vma_allocator, this->vkImage, *vma_allocation);
}

Turbo::Core::TImage::TImage(TDevice *device, VkImageCreateFlags imageFlags, TImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr)
    {
        this->device = device;
        this->imageFlags = imageFlags;
        this->memoryFlags = memoryFlags;
        this->type = type;
        this->format = format;
        this->extent.width = width;
        this->extent.height = height;
        this->extent.depth = depth;
        this->mipLevels = mipLevels;
        this->arrayLayers = arrayLayers;
        this->samples = samples;
        this->tiling = tiling;
        this->usages = usages;
        this->layout = layout;
        this->vmaAllocation = malloc(sizeof(VmaAllocation));
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TImage::~TImage()
{
    this->InternalDestroy();
    free(this->vmaAllocation);
    this->vmaAllocation = nullptr;
}

VkImage Turbo::Core::TImage::GetVkImage()
{
    return this->vkImage;
}

Turbo::Core::TFormatInfo Turbo::Core::TImage::GetFormat()
{
    return this->format;
}

Turbo::Core::TDevice *Turbo::Core::TImage::GetDevice()
{
    return this->device;
}

uint32_t Turbo::Core::TImage::GetWidth()
{
    return this->extent.width;
}

uint32_t Turbo::Core::TImage::GetHeight()
{
    return this->extent.height;
}

uint32_t Turbo::Core::TImage::GetDepth()
{
    return this->extent.depth;
}

std::string Turbo::Core::TImage::ToString()
{
    return std::string();
}
