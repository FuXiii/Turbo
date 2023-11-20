#include "TDeviceQueue.h"
#include "TCommandBuffer.h"
#include "TCommandBufferPool.h"
#include "TDevice.h"
#include "TException.h"
#include "TFence.h"
#include "TPhysicalDevice.h"
#include "TQueueFamilyInfo.h"
#include "TSemaphore.h"
#include "TSurface.h"
#include "TSwapchain.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TDeviceQueue::AddChildHandle(const TRefPtr<TCommandBufferPool> &commandBufferPool)
{
    if (commandBufferPool != nullptr)
    {
        this->commandBufferPools.push_back(commandBufferPool);
    }
}

Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> Turbo::Core::TDeviceQueue::RemoveChildHandle(const TRefPtr<TCommandBufferPool> &commandBufferPool)
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

    this->device->GetDeviceDriver()->vkGetDeviceQueue(vk_device, queue_family_index, this->index, &this->vkQueue);

    // TODO: load vkQueuePresentKHR function
    // FIXME: If device not enable VK_KHR_swapchain extension we can not load:
    // FIXME: vkQueuePresentKHR
    if (this->device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN))
    {
        this->vkQueuePresentKHR = TVulkanLoader::Instance()->LoadDeviceFunction<PFN_vkQueuePresentKHR>(this->device, "vkQueuePresentKHR");
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Core::TDeviceQueue::InternalCreate()", "Please enable the VK_KHR_swapchain extension");
    }

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

Turbo::Core::TDeviceQueue::TDeviceQueue(const TRefPtr<TDevice> &device, TQueueFamilyInfo &queueFamily, uint32_t index) : Turbo::Core::TVulkanHandle()
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

            // 更新TPhysicalDevice下的可用queue的数量
            device->physicalDevice->AvailableQueueCountMinusOneByQueueFamilyIndex(this->queueFamily.GetIndex());
        }
        else
        {
            throw Turbo::Core::TException(TResult::RESOURCE_STARVATION, "Turbo::Core::TDeviceQueue::TDeviceQueue");
        }
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDeviceQueue::TDeviceQueue");
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

Turbo::Core::TRefPtr<Turbo::Core::TDevice> Turbo::Core::TDeviceQueue::GetDevice()
{
    return this->device;
}

bool Turbo::Core::TDeviceQueue::Submit(std::vector<TRefPtr<TSemaphore>> *waitSemaphores, std::vector<TRefPtr<TSemaphore>> *signalSemaphores, const TRefPtr<TCommandBuffer> &commandBuffer, const TRefPtr<TFence> &fence)
{
    std::vector<VkSemaphore> wait_semaphores;
    std::vector<VkPipelineStageFlags> wait_dst_stage_masks;
    if (waitSemaphores != nullptr)
    {
        for (TSemaphore *semaphore_item : *waitSemaphores)
        {
            wait_semaphores.push_back(semaphore_item->GetVkSemaphore());
            wait_dst_stage_masks.push_back(semaphore_item->GetWaitDstStageMask());
        }
    }

    std::vector<VkSemaphore> signal_semaphores;
    if (signalSemaphores != nullptr)
    {
        for (TSemaphore *semaphore_item : *signalSemaphores)
        {
            signal_semaphores.push_back(semaphore_item->GetVkSemaphore());
        }
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

    VkFence vk_fence = VK_NULL_HANDLE;
    if (fence != nullptr)
    {
        vk_fence = fence->GetVkFence();
    }

    VkResult result = this->device->GetDeviceDriver()->vkQueueSubmit(this->vkQueue, 1, &vk_submit_info, vk_fence);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::FAIL, "Turbo::Core::TDeviceQueue::Submit::vkQueueSubmit");
    }

    return true;
}

void Turbo::Core::TDeviceQueue::WaitIdle()
{
    VkResult result = this->device->GetDeviceDriver()->vkQueueWaitIdle(this->vkQueue);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::FAIL, "Turbo::Core::TDeviceQueue::WaitIdle");
    }
}

bool Turbo::Core::TDeviceQueue::IsSupportSurface(const TRefPtr<Turbo::Extension::TSurface> &surface)
{
    uint32_t queue_family_index = this->queueFamily.GetIndex();
    std::vector<Turbo::Core::TQueueFamilyInfo> support_queue_familys = surface->GetSupportQueueFamilys();
    for (Turbo::Core::TQueueFamilyInfo &queue_family_item : support_queue_familys)
    {
        if (queue_family_index == queue_family_item.GetIndex())
        {
            return true;
        }
    }

    return false;
}

Turbo::Core::TResult Turbo::Core::TDeviceQueue::Present(const TRefPtr<Turbo::Extension::TSwapchain> &swapchain, uint32_t imageIndex)
{
    if (swapchain != nullptr)
    {
        VkSwapchainKHR vk_swapchain_khr = swapchain->GetVkSwapchainKHR();
        uint32_t image_index = imageIndex;
        VkPresentInfoKHR vk_present_info_khr = {};
        vk_present_info_khr.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        vk_present_info_khr.pNext = nullptr;
        vk_present_info_khr.waitSemaphoreCount = 0;
        vk_present_info_khr.pWaitSemaphores = nullptr;
        vk_present_info_khr.swapchainCount = 1;
        vk_present_info_khr.pSwapchains = &vk_swapchain_khr;
        vk_present_info_khr.pImageIndices = &image_index;
        vk_present_info_khr.pResults = nullptr;

        // TODO: load vkQueuePresentKHR(...) by TVulkanLoader
        VkResult result = this->vkQueuePresentKHR(this->vkQueue, &vk_present_info_khr);
        switch (result)
        {
        case VkResult::VK_SUCCESS: {
            return Turbo::Core::TResult::SUCCESS;
        }
        break;
            break;
        case VkResult::VK_ERROR_OUT_OF_DATE_KHR:
        case VkResult::VK_SUBOPTIMAL_KHR: {
            return Turbo::Core::TResult::MISMATCH;
        }
        break;
        case VkResult::VK_ERROR_DEVICE_LOST: {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Core::TDeviceQueue::Present", "Vulkan implementation is unable to make that guarantee, please recreate swapchain");
        }
        break;
        default: {
            throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "Turbo::Core::TDeviceQueue::Present");
        }
        break;
        }
    }

    return TResult::INVALID_PARAMETER;
}

std::string Turbo::Core::TDeviceQueue::ToString()
{
    return std::string();
}
