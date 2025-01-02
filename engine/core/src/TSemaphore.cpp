#include "TSemaphore.h"
#include "TDevice.h"
#include "TException.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TSemaphore::InternalCreate()
{
    VkSemaphoreCreateInfo vk_semaphore_create_info;
    vk_semaphore_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vk_semaphore_create_info.pNext = nullptr;
    vk_semaphore_create_info.flags = 0;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = this->device->GetDeviceDriver()->vkCreateSemaphore(vk_device, &vk_semaphore_create_info, allocator, &this->vkSemaphore);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TSemaphore::InternalCreate::vkCreateSemaphore");
    }
}

void Turbo::Core::TSemaphore::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->device->GetDeviceDriver()->vkDestroySemaphore(vk_device, this->vkSemaphore, allocator);
}

Turbo::Core::TSemaphore::TSemaphore(TDevice *device, VkPipelineStageFlags waitDstStageMask)
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->waitDstStageMask = waitDstStageMask;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TSemaphore::TSemaphore");
    }
}

Turbo::Core::TSemaphore::~TSemaphore()
{
    this->InternalDestroy();
}

Turbo::Core::TPipelineStages Turbo::Core::TSemaphore::GetWaitDstStageMask() const
{
    return this->waitDstStageMask;
}

VkSemaphore Turbo::Core::TSemaphore::GetVkSemaphore()
{
    return this->vkSemaphore;
}

std::string Turbo::Core::TSemaphore::ToString() const
{
    return std::string();
}

bool Turbo::Core::TSemaphore::Valid() const
{
    if (this->vkSemaphore != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}
