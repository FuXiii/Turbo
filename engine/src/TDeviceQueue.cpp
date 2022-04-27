#include "TDeviceQueue.h"
#include "TCommandBuffer.h"
#include "TCommandBufferPool.h"
#include "TDevice.h"
#include "TException.h"
#include "TFence.h"
#include "TPhysicalDevice.h"
#include "TQueueFamilyInfo.h"
#include "TSemaphore.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TDeviceQueue::AddChildHandle(TCommandBufferPool *commandBufferPool)
{
    if (commandBufferPool != nullptr)
    {
        this->commandBufferPools.push_back(commandBufferPool);
    }
}

Turbo::Core::TCommandBufferPool *Turbo::Core::TDeviceQueue::RemoveChildHandle(TCommandBufferPool *commandBufferPool)
{
    Turbo::Core::TCommandBufferPool *result = nullptr;
    uint32_t index = 0;
    bool is_found = false;
    uint32_t command_buffer_pools_count = this->commandBufferPools.size();

    for (uint32_t command_buffer_pool_index = 0; command_buffer_pool_index < command_buffer_pools_count; command_buffer_pool_index++)
    {
        if (this->commandBufferPools[command_buffer_pool_index] == commandBufferPool)
        {
            result = this->commandBufferPools[command_buffer_pool_index];
            index = command_buffer_pool_index;
            is_found = true;
            break;
        }
    }

    if (result != nullptr && is_found)
    {
        this->commandBufferPools.erase(this->commandBufferPools.begin() + index);
    }

    return result;
}

void Turbo::Core::TDeviceQueue::InternalCreate()
{
    VkDevice vk_device = this->device->GetVkDevice();
    uint32_t queue_family_index = this->queueFamily.GetIndex();

    vkGetDeviceQueue(vk_device, queue_family_index, this->index, &this->vkQueue);

    for (TCommandBufferPool *command_buffer_pool_item : this->commandBufferPools)
    {
        command_buffer_pool_item->InternalCreate();
    }
}

void Turbo::Core::TDeviceQueue::InternalDestroy()
{
    for (TCommandBufferPool *command_buffer_pool_item : this->commandBufferPools)
    {
        command_buffer_pool_item->InternalDestroy();
    }

    // VkQueue 没有销毁函数，置空
    this->vkQueue = VK_NULL_HANDLE;
}

Turbo::Core::TDeviceQueue::TDeviceQueue(TDevice *device, TQueueFamilyInfo &queueFamily, uint32_t index) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr && queueFamily.GetIndex() != UINT32_MAX)
    {
        uint32_t available_count = device->GetPhysicalDevice()->GetAvailableQueueCount(queueFamily);
        if (available_count > 0)
        {
            this->device = device;
            this->queueFamily = queueFamily;
            this->index = index;

            this->device->AddChildHandle(this);

            this->device->InternalRebuild();

            //更新TPhysicalDevice下的可用queue的数量
            device->physicalDevice->AvailableQueueCountMinusOneByQueueFamilyIndex(this->queueFamily.GetIndex());
        }
        else
        {
            throw Turbo::Core::TException(TResult::RESOURCE_STARVATION);
        }
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TDeviceQueue::~TDeviceQueue()
{
    this->device->RemoveChildHandle(this);

    device->physicalDevice->AvailableQueueCountPlussOneByQueueFamilyIndex(this->queueFamily.GetIndex());

    // this->device->InternalRebuild();

    // for (; this->commandBufferPools.size() > 0;)
    // {
    //     delete this->commandBufferPools[0];
    // }

    this->commandBufferPools.clear();

    this->InternalDestroy();
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TDeviceQueue::GetQueueFamily()
{
    return this->queueFamily;
}

VkQueue Turbo::Core::TDeviceQueue::GetVkQueue()
{
    return this->vkQueue;
}

Turbo::Core::TDevice *Turbo::Core::TDeviceQueue::GetDevice()
{
    return this->device;
}

bool Turbo::Core::TDeviceQueue::Submit(std::vector<TSemaphore *> &waitSemaphores, std::vector<TSemaphore *> &signalSemaphores, TCommandBuffer *commandBuffer, TFence *fence)
{
    std::vector<VkSemaphore> wait_semaphores;
    std::vector<VkPipelineStageFlags> wait_dst_stage_masks;
    for (TSemaphore *semaphore_item : waitSemaphores)
    {
        wait_semaphores.push_back(semaphore_item->GetVkSemaphore());
        wait_dst_stage_masks.push_back(semaphore_item->GetWaitDstStageMask());
    }

    std::vector<VkSemaphore> signal_semaphores;
    for (TSemaphore *semaphore_item : signalSemaphores)
    {
        signal_semaphores.push_back(semaphore_item->GetVkSemaphore());
    }

    VkCommandBuffer vk_commandbuffer = commandBuffer->GetVkCommandBuffer();

    VkSubmitInfo vk_submit_info;
    vk_submit_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vk_submit_info.pNext = nullptr;
    vk_submit_info.waitSemaphoreCount = wait_semaphores.size();
    vk_submit_info.pWaitSemaphores = wait_semaphores.data();
    vk_submit_info.pWaitDstStageMask = wait_dst_stage_masks.data();
    vk_submit_info.commandBufferCount = 1;
    vk_submit_info.pCommandBuffers = &vk_commandbuffer;
    vk_submit_info.signalSemaphoreCount = signal_semaphores.size();
    vk_submit_info.pSignalSemaphores = signal_semaphores.data();

    VkFence vk_fence = fence->GetVkFence();
    VkResult result = vkQueueSubmit(this->vkQueue, 1, &vk_submit_info, vk_fence);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::FAIL);
    }

    return true;
}

std::string Turbo::Core::TDeviceQueue::ToString()
{
    return std::string();
}
