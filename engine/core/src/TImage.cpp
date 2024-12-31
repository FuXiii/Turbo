#include "TImage.h"
#include "TDevice.h"
#include "TException.h"
#include "TPhysicalDevice.h"
#include "TVmaAllocator.h"

#include "vk_mem_alloc.h"
#include <stdint.h>

void Turbo::Core::TImage::InternalCreate()
{
    bool is_format_support_image = this->device->GetPhysicalDevice()->IsFormatSupportImage(this->format, this->type, this->tiling, this->usages, this->imageFlags);
    if (!is_format_support_image)
    {
        throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TImage::InternalCreate()", "Unsupport format");
    }

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
    case TImageLayout::UNDEFINED: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    }
    break;
    case TImageLayout::GENERAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;
    }
    break;
    case TImageLayout::COLOR_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    break;
    case TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }
    break;
    case TImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    }
    break;
    case TImageLayout::SHADER_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
    }
    break;
    case TImageLayout::TRANSFER_SRC_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    }
    break;
    case TImageLayout::TRANSFER_DST_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    }
    break;
    case TImageLayout::PREINITIALIZED: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED;
    }
    break;
    case TImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
    }
    break;
    case TImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
    }
    break;
    case TImageLayout::DEPTH_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    }
    break;
    case TImageLayout::DEPTH_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
    }
    break;
    case TImageLayout::STENCIL_ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
    }
    break;
    case TImageLayout::STENCIL_READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    break;
    case TImageLayout::READ_ONLY_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    }
    break;
    case TImageLayout::ATTACHMENT_OPTIMAL: {
        vk_image_layout = VkImageLayout::VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    }
    break;
    case TImageLayout::PRESENT_SRC_KHR: {
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
    vk_image_create_info.initialLayout = vk_image_layout; // NOTE: initialLayout must be VK_IMAGE_LAYOUT_UNDEFINED or VK_IMAGE_LAYOUT_PREINITIALIZED <https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap12.html#VUID-VkImageCreateInfo-initialLayout-00993>

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = this->memoryFlags;

    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    VmaAllocation *vma_allocation = (VmaAllocation *)this->vmaAllocation;
    VkResult result = vmaCreateImage(*vma_allocator, &vk_image_create_info, &alloc_info, &this->vkImage, vma_allocation, nullptr);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TImage::InternalCreate::vmaCreateImage");
    }

    VmaAllocationInfo *vma_allocation_info = (VmaAllocationInfo *)this->vmaAllocationInfo;
    vmaGetAllocationInfo(*vma_allocator, *vma_allocation, vma_allocation_info);
}

void Turbo::Core::TImage::InternalDestroy()
{
    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    VmaAllocation *vma_allocation = (VmaAllocation *)this->vmaAllocation;
    if (vma_allocation != nullptr)
    {
        vmaDestroyImage(*vma_allocator, this->vkImage, *vma_allocation);
    }
}

Turbo::Core::TImage::TImage(TDevice *device, VkImage vkImage, VkImageCreateFlags imageFlags, TImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TImageLayout layout) : Turbo::Core::TVulkanHandle()
{
    this->device = device;
    this->vkImage = vkImage;
    this->imageFlags = imageFlags;
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
}

Turbo::Core::TImage::TImage(const TRefPtr<TDevice> &device, VkImageCreateFlags imageFlags, TImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout) : Turbo::Core::TVulkanHandle()
{
    if (device.Valid())
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
        this->vmaAllocationInfo = malloc(sizeof(VmaAllocationInfo));
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImage::TImage");
    }
}

Turbo::Core::TImage::TImage(TDevice *device, VkImageCreateFlags imageFlags, TImageType type, TFormatType formatType, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout)
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        TPhysicalDevice *physical_device = device->GetPhysicalDevice();
        if (physical_device->IsSupportFormat(formatType))
        {
            this->format = physical_device->GetFormatInfo(formatType);
        }
        else
        {
            throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TImage::TImage", "Unsupport format");
        }

        this->device = device;
        this->imageFlags = imageFlags;
        this->memoryFlags = memoryFlags;
        this->type = type;
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
        this->vmaAllocationInfo = malloc(sizeof(VmaAllocationInfo));
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImage::TImage");
    }
}

Turbo::Core::TImage::~TImage()
{
    this->InternalDestroy();
    free(this->vmaAllocation);
    this->vmaAllocation = nullptr;
    free(this->vmaAllocationInfo);
    this->vmaAllocationInfo = nullptr;
}

VkImage Turbo::Core::TImage::GetVkImage()
{
    return this->vkImage;
}

Turbo::Core::TFormatInfo Turbo::Core::TImage::GetFormat() const
{
    return this->format;
}

Turbo::Core::TDevice *Turbo::Core::TImage::GetDevice()
{
    return this->device;
}

uint32_t Turbo::Core::TImage::GetWidth() const
{
    return this->extent.width;
}

uint32_t Turbo::Core::TImage::GetHeight() const
{
    return this->extent.height;
}

uint32_t Turbo::Core::TImage::GetDepth() const
{
    return this->extent.depth;
}

Turbo::Core::TSampleCountBits Turbo::Core::TImage::GetSampleCountBits() const
{
    return this->samples;
}

Turbo::Core::TImageUsages Turbo::Core::TImage::GetUsages() const
{
    return this->usages;
}

uint32_t Turbo::Core::TImage::GetMipLevels() const
{
    return this->mipLevels;
}

uint32_t Turbo::Core::TImage::GetArrayLayers() const
{
    return this->arrayLayers;
}

Turbo::Core::TMemoryTypeInfo Turbo::Core::TImage::GetMemoryTypeInfo() const
{
    uint32_t memory_type_index = ((VmaAllocationInfo *)this->vmaAllocationInfo)->memoryType;
    return this->device->GetPhysicalDevice()->GetMemoryTypeByIndex(memory_type_index);
}

bool Turbo::Core::TImage::IsMappable() const
{
    if (((this->memoryFlags & TMemoryFlagsBits::HOST_ACCESS_RANDOM) == TMemoryFlagsBits::HOST_ACCESS_RANDOM) || ((this->memoryFlags & TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE) == TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE))
    {
        return true;
    }

    return false;
}

void *Turbo::Core::TImage::Map()
{
    void *result = nullptr;
    if (this->vmaAllocation != nullptr)
    {
        if (this->IsMappable())
        {
            VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
            vmaMapMemory(*vma_allocator, *((VmaAllocation *)this->vmaAllocation), &result);
        }
    }

    return result;
}

void Turbo::Core::TImage::Unmap()
{
    if (this->vmaAllocation != nullptr)
    {
        VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
        vmaUnmapMemory(*vma_allocator, *((VmaAllocation *)this->vmaAllocation));
    }
}

std::string Turbo::Core::TImage::ToString() const
{
    return std::string();
}

bool Turbo::Core::TImage::Valid() const
{
    if (this->vkImage != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}