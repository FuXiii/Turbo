#include "TSemaphore.h"
#include "TDevice.h"
#include "TException.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TSemaphore::InternalCreate()
{
    VkSemaphoreCreateInfo vk_semaphore_create_info;
    vk_semaphore_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vk_semaphore_create_info.pNext = nullptr;
    vk_semaphore_create_info.flags = 0;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateSemaphore(vk_device, &vk_semaphore_create_info, allocator, &this->vkSemaphore);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
    }
}

void Turbo::Core::TSemaphore::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vkDestroySemaphore(vk_device, this->vkSemaphore, allocator);
}

Turbo::Core::TSemaphore::TSemaphore(TDevice *device, VkPipelineStageFlags waitDstStageMask)
{
    if (device != nullptr)
    {
        this->device = device;
        this->waitDstStageMask = waitDstStageMask;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TSemaphore::~TSemaphore()
{
    this->InternalDestroy();
}

Turbo::Core::TPipelineStages Turbo::Core::TSemaphore::GetWaitDstStageMask()
{
    return this->waitDstStageMask;
}

VkSemaphore Turbo::Core::TSemaphore::GetVkSemaphore()
{
    return this->vkSemaphore;
}

std::string Turbo::Core::TSemaphore::ToString()
{
    return std::string();
}
