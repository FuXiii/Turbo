#include "TBuffer.h"
#include "TDevice.h"
#include "TException.h"
#include "TPhysicalDevice.h"
#include "TVmaAllocator.h"

// #define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

void Turbo::Core::TBuffer::InternalCreate()
{
    VkBufferCreateInfo vk_buffer_create_info = {};
    vk_buffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vk_buffer_create_info.pNext = nullptr;
    vk_buffer_create_info.flags = this->bufferFlags;
    vk_buffer_create_info.size = this->size;
    vk_buffer_create_info.usage = this->usages;
    // vk_buffer_create_info.sharingMode=???;//By Vma?
    // vk_buffer_create_info.queueFamilyIndexCount=???;//By Vma?
    // vk_buffer_create_info.pQueueFamilyIndices=???;//By Vma?

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_AUTO;
    alloc_info.flags = this->memoryFlags;

    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    VmaAllocation *vma_allocation = (VmaAllocation *)this->vmaAllocation;
    VkResult result = vmaCreateBuffer(*vma_allocator, &vk_buffer_create_info, &alloc_info, &this->vkBuffer, vma_allocation, nullptr);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TBuffer::InternalCreate::vmaCreateBuffer");
    }

    VmaAllocationInfo *vma_allocation_info = (VmaAllocationInfo *)this->vmaAllocationInfo;
    vmaGetAllocationInfo(*vma_allocator, *vma_allocation, vma_allocation_info);
}

void Turbo::Core::TBuffer::InternalDestroy()
{
    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    VmaAllocation *vma_allocation = (VmaAllocation *)this->vmaAllocation;
    vmaDestroyBuffer(*vma_allocator, this->vkBuffer, *vma_allocation);
}

Turbo::Core::TBuffer::TBuffer(TDevice *device, VkBufferCreateFlags bufferFlags, TBufferUsages usages, TMemoryFlags memoryFlags, TDeviceSize size)
{
    if (device != nullptr)
    {
        this->device = device;
        this->bufferFlags = bufferFlags;
        this->memoryFlags = memoryFlags;
        this->usages = usages;
        this->size = size;
        this->vmaAllocation = malloc(sizeof(VmaAllocation));
        this->vmaAllocationInfo = malloc(sizeof(VmaAllocationInfo));
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TBuffer::TBuffer");
    }
}

Turbo::Core::TBuffer::~TBuffer()
{
    this->InternalDestroy();
    free(this->vmaAllocation);
    this->vmaAllocation = nullptr;
    free(this->vmaAllocationInfo);
    this->vmaAllocationInfo = nullptr;
}

Turbo::Core::TBufferUsageFlags Turbo::Core::TBuffer::GetBufferUsageFlags()
{
    return this->usages;
}

bool Turbo::Core::TBuffer::IsTransferSource()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsTransferDestination()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsUniformTexelBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsStorageTexelBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsUniformBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsStorageBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsIndexBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsVertexBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TBuffer::IsIndirectBuffer()
{
    if (this->usages & VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
    {
        return true;
    }

    return false;
}

VkBuffer Turbo::Core::TBuffer::GetVkBuffer()
{
    return this->vkBuffer;
}

void *Turbo::Core::TBuffer::Map()
{
    void *result = nullptr;
    if (((this->memoryFlags & TMemoryFlagsBits::HOST_ACCESS_RANDOM) == TMemoryFlagsBits::HOST_ACCESS_RANDOM) || ((this->memoryFlags & TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE) == TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE))
    {
        VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
        vmaMapMemory(*vma_allocator, *((VmaAllocation *)this->vmaAllocation), &result);
    }

    return result;
}

void Turbo::Core::TBuffer::Unmap()
{
    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    vmaUnmapMemory(*vma_allocator, *((VmaAllocation *)this->vmaAllocation));
}

void Turbo::Core::TBuffer::Flush(TDeviceSize offset, TDeviceSize size)
{
    VmaAllocator *vma_allocator = (VmaAllocator *)(this->device->GetVmaAllocator()->GetVmaAllocator());
    vmaFlushAllocation(*vma_allocator, *((VmaAllocation *)this->vmaAllocation), offset, size);
}

Turbo::Core::TMemoryTypeInfo Turbo::Core::TBuffer::GetMemoryTypeInfo()
{
    uint32_t memory_type_index = ((VmaAllocationInfo *)this->vmaAllocationInfo)->memoryType;
    return this->device->GetPhysicalDevice()->GetMemoryTypeByIndex(memory_type_index);
}

std::string Turbo::Core::TBuffer::ToString()
{
    return std::string();
}
