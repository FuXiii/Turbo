#include "TCommandBufferPool.h"
#include "TCommandBuffer.h"
#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TException.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include <stdint.h>

void Turbo::Core::TCommandBufferPool::AddChildHandle(TCommandBuffer *commandBuffer)
{
    // Nothing to do, this is duty of TPool
}

Turbo::Core::TCommandBuffer *Turbo::Core::TCommandBufferPool::RemoveChildHandle(TCommandBuffer *commandBuffer)
{
    // Nothing to do, this is duty of TPool
    return nullptr;
}

void Turbo::Core::TCommandBufferPool::InternalCreate()
{
    TDevice *device = this->deviceQueue->GetDevice();
    VkDevice vk_device = device->GetVkDevice();

    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = nullptr;
    command_pool_create_info.flags = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = deviceQueue->GetQueueFamily().GetIndex();

    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

    VkResult result = device->GetDeviceDriver()->vkCreateCommandPool(vk_device, &command_pool_create_info, allocator, &this->vkCommandPool);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TCommandBufferPool::InternalCreate::vkCreateCommandPool");
    }

    std::vector<TCommandBuffer *> command_buffers = this->GetPool();
    for (TCommandBuffer *command_buffer_item : command_buffers)
    {
        command_buffer_item->InternalCreate();
    }
}

void Turbo::Core::TCommandBufferPool::InternalDestroy()
{
    std::vector<TCommandBuffer *> command_buffers = this->GetPool();
    for (TCommandBuffer *command_buffer_item : command_buffers)
    {
        command_buffer_item->InternalDestroy();
    }

    TDevice *device = this->deviceQueue->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

    device->GetDeviceDriver()->vkDestroyCommandPool(vk_device, this->vkCommandPool, allocator);
}

Turbo::Core::TCommandBufferPool::TCommandBufferPool(TDeviceQueue *deviceQueue) : Turbo::Core::TPool<TCommandBuffer>(UINT32_MAX), Turbo::Core::TVulkanHandle()
{
    if (deviceQueue != nullptr && deviceQueue->GetVkQueue() != VK_NULL_HANDLE)
    {
        this->deviceQueue = deviceQueue;

        this->InternalCreate();

        this->deviceQueue->AddChildHandle(this);
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TCommandBufferPool::TCommandBufferPool");
    }
}

Turbo::Core::TCommandBufferPool::~TCommandBufferPool()
{
    this->deviceQueue->RemoveChildHandle(this);

    const std::vector<TCommandBuffer *> &command_buffers = this->GetPool();

    for (; command_buffers.size() > 0;)
    {
        this->Free(command_buffers[0]);
    }

    this->InternalDestroy();
}

Turbo::Core::TCommandBuffer *Turbo::Core::TCommandBufferPool::Allocate()
{
    return TPool<TCommandBuffer>::Allocate(this);
}

Turbo::Core::TDeviceQueue *Turbo::Core::TCommandBufferPool::GetDeviceQueue()
{
    return this->deviceQueue;
}

VkCommandPool Turbo::Core::TCommandBufferPool::GetVkCommandPool()
{
    return this->vkCommandPool;
}

std::string Turbo::Core::TCommandBufferPool::ToString()
{
    return std::string();
}
