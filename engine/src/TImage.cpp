#include "TImage.h"
#include "TDevice.h"
#include "TException.h"
#include "TVmaAllocator.h"

#include "vk_mem_alloc.h"

void Turbo::Core::TImage::InternalCreate()
{
    VkImageCreateInfo vk_image_create_info = {};
    vk_image_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vk_image_create_info.pNext = nullptr;
    vk_image_create_info.flags = this->imageFlags;
    vk_image_create_info.imageType = this->type;
    vk_image_create_info.format = this->format.GetVkFormat();
    vk_image_create_info.extent = this->extent;
    vk_image_create_info.mipLevels = this->mipLevels;
    vk_image_create_info.arrayLayers = this->arrayLayers;
    vk_image_create_info.samples = this->samples;
    vk_image_create_info.tiling = this->tiling;
    vk_image_create_info.usage = this->usage;
    // vk_image_create_info.sharingMode;//By Vma?
    // vk_image_create_info.queueFamilyIndexCount;//By Vma?
    // vk_image_create_info.pQueueFamilyIndices;//By Vma?
    vk_image_create_info.initialLayout = this->layout;

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

Turbo::Core::TImage::TImage(TDevice *device, VkImageCreateFlags imageFlags, VkImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, TMemoryFlags memoryFlags, VkImageLayout layout) : Turbo::Core::TVulkanHandle()
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
        this->usage = usage;
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
