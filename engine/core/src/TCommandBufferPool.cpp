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
    for (TCommandBufferBase *secondary_command_buffer_item : this->secondaryCommandBuffers)
    {
        secondary_command_buffer_item->InternalDestroy(); // FIXME:Normally, it is automatically destroyed when the reference count reaches zero.
                                                          // FIXME:For now, if this->secondaryCommandBuffers not empty, it means there has some TSecondaryCommandBuffer
                                                          // FIXME:had not been Free, so here we need force destroy it.
    }

    for (TCommandBufferBase *command_buffer_item : this->commandBuffers)
    {
        command_buffer_item->InternalDestroy(); // FIXME:Normally, it is automatically destroyed when the reference count reaches zero.
                                                // FIXME:For now, if this->commandBuffers not empty, it means there has some TCommandBuffer
                                                // FIXME:had not been Free, so here we need force destroy it.
    }

    this->secondaryCommandBuffers.clear(); // Just clear it
    this->commandBuffers.clear();          // Just clear it

    TDevice *device = this->deviceQueue->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

    device->GetDeviceDriver()->vkDestroyCommandPool(vk_device, this->vkCommandPool, allocator);
}

bool Turbo::Core::TCommandBufferPool::Free(TRefPtr<TCommandBufferBase> &commandBufferBase)
{
    if (commandBufferBase.Valid())
    {
        switch (commandBufferBase->GetLevel())
        {
        case TCommandBufferLevel::PRIMARY: {
            for (uint32_t command_buffer_index = 0; command_buffer_index < this->commandBuffers.size(); command_buffer_index++)
            {
                if (this->commandBuffers[command_buffer_index] == commandBufferBase)
                {
                    this->commandBuffers.erase(this->commandBuffers.begin() + command_buffer_index);
                    // commandBufferBase.Release(); // NOTE: It will force release memory
                    commandBufferBase = nullptr;
                    return true;
                }
            }
        }
        break;
        case TCommandBufferLevel::SECONDARY: {
            for (uint32_t secondary_command_buffer_index = 0; secondary_command_buffer_index < this->secondaryCommandBuffers.size(); secondary_command_buffer_index++)
            {
                if (this->secondaryCommandBuffers[secondary_command_buffer_index] == commandBufferBase)
                {
                    this->secondaryCommandBuffers.erase(this->secondaryCommandBuffers.begin() + secondary_command_buffer_index);
                    // commandBufferBase.Release(); // NOTE: It will force release memory
                    commandBufferBase = nullptr;
                    return true;
                }
            }
        }
        break;
        };
    }

    return false;
}

Turbo::Core::TCommandBufferPool::TCommandBufferPool(const TRefPtr<TDeviceQueue> &deviceQueue) : Turbo::Core::TVulkanHandle()
{
    // if (deviceQueue != nullptr && deviceQueue->GetVkQueue() != VK_NULL_HANDLE)
    if (deviceQueue.Valid())
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

    // OLD:for (; this->commandBuffers.size() > 0;)
    // OLD:{
    // OLD:    // TCommandBufferBase *command_buffer_base = this->commandBuffers[0];
    // OLD:    // this->Free(command_buffer_base);
    // OLD:    this->Free(this->commandBuffers[0]);
    // OLD:}

    this->InternalDestroy();
}

Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> &Turbo::Core::TCommandBufferPool::Allocate()
{
    this->commandBuffers.push_back(new TCommandBuffer(this));
    return this->commandBuffers.back();
}

void Turbo::Core::TCommandBufferPool::Free(TRefPtr<TCommandBuffer> &commandBuffer)
{
    // OLD: this->Free(static_cast<TCommandBufferBase *>(commandBuffer));
    TRefPtr<TCommandBufferBase> command_buffer_base = commandBuffer;
    if (this->Free(command_buffer_base))
    {
        // NOTE: Now reference count should be 1
        if (commandBuffer.ReferenceCount() != 1)
        {
            // FIXME: maybe need throw a exception?
        }
        commandBuffer = nullptr;
    }
}

Turbo::Core::TRefPtr<Turbo::Core::TSecondaryCommandBuffer> &Turbo::Core::TCommandBufferPool::AllocateSecondary()
{
    this->secondaryCommandBuffers.push_back(new TSecondaryCommandBuffer(this));
    return this->secondaryCommandBuffers.back();
}

void Turbo::Core::TCommandBufferPool::Free(TRefPtr<TSecondaryCommandBuffer> &secondaryCommandBuffer)
{
    // OLD: this->Free(static_cast<TCommandBufferBase *>(secondaryCommandBuffer));
    TRefPtr<TCommandBufferBase> command_buffer_base = secondaryCommandBuffer;
    if (this->Free(command_buffer_base))
    {
        // NOTE: Now reference count should be 1
        if (secondaryCommandBuffer.ReferenceCount() != 1)
        {
            // FIXME: maybe need throw a exception?
        }
        secondaryCommandBuffer = nullptr;
    }
}

const Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> &Turbo::Core::TCommandBufferPool::GetDeviceQueue()
{
    return this->deviceQueue;
}

VkCommandPool Turbo::Core::TCommandBufferPool::GetVkCommandPool()
{
    return this->vkCommandPool;
}

std::string Turbo::Core::TCommandBufferPool::ToString() const
{
    return std::string();
}
