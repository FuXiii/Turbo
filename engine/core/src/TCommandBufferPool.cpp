#include "TCommandBufferPool.h"
#include "TCommandBuffer.h"
#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TException.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include <stdint.h>

void Turbo::Core::TCommandBufferPool::AddChildHandle(const TRefPtr<TCommandBuffer> &commandBuffer)
{
    // Nothing to do, this is duty to Allocate()
}

Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> Turbo::Core::TCommandBufferPool::RemoveChildHandle(const TRefPtr<TCommandBuffer> &commandBuffer)
{
    // Nothing to do, this is duty to TPool
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

    for (TCommandBufferBase *command_buffer_item : this->commandBuffers)
    {
        command_buffer_item->InternalCreate();
    }
}

void Turbo::Core::TCommandBufferPool::InternalDestroy()
{
    for (TCommandBufferBase *command_buffer_item : this->commandBuffers)
    {
        command_buffer_item->InternalDestroy();
    }

    TDevice *device = this->deviceQueue->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

    device->GetDeviceDriver()->vkDestroyCommandPool(vk_device, this->vkCommandPool, allocator);
}

void Turbo::Core::TCommandBufferPool::Free(const TRefPtr<TCommandBufferBase> &commandBufferBase)
{
    uint32_t index = 0;
    bool is_found = false;
    for (TCommandBufferBase *command_buffer_item : this->commandBuffers)
    {
        if (command_buffer_item == commandBufferBase)
        {
            is_found = true;
            break;
        }
        index = index + 1;
    }

    if (is_found)
    {
        delete this->commandBuffers[index];
        this->commandBuffers.erase(this->commandBuffers.begin() + index);
    }
}

Turbo::Core::TCommandBufferPool::TCommandBufferPool(const TRefPtr<TDeviceQueue> &deviceQueue) : Turbo::Core::TVulkanHandle()
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

    for (; this->commandBuffers.size() > 0;)
    {
        TCommandBufferBase *command_buffer_base = this->commandBuffers[0];
        this->Free(command_buffer_base);
    }

    this->InternalDestroy();
}

Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> Turbo::Core::TCommandBufferPool::Allocate()
{
    Turbo::Core::TCommandBuffer *command_buffer = new TCommandBuffer(this);
    this->commandBuffers.push_back(command_buffer);
    return command_buffer;
}

void Turbo::Core::TCommandBufferPool::Free(const TRefPtr<TCommandBuffer> &commandBuffer)
{
    this->Free(static_cast<TCommandBufferBase *>(commandBuffer));
}

Turbo::Core::TRefPtr<Turbo::Core::TSecondaryCommandBuffer> Turbo::Core::TCommandBufferPool::AllocateSecondary()
{
    Turbo::Core::TSecondaryCommandBuffer *secondary_command_buffer = new TSecondaryCommandBuffer(this);
    this->commandBuffers.push_back(secondary_command_buffer);
    return secondary_command_buffer;
}

void Turbo::Core::TCommandBufferPool::Free(const TRefPtr<TSecondaryCommandBuffer> &secondaryCommandBuffer)
{
    this->Free(static_cast<TCommandBufferBase *>(secondaryCommandBuffer));
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TCommandBufferPool::GetDeviceQueue()
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
