#include "TVulkanAllocator.h"

VkAllocationCallbacks Turbo::Core::TVulkanAllocator::vkAllocationCallbacks;
Turbo::Core::TRefPtr<Turbo::Core::TVulkanAllocator> Turbo::Core::TVulkanAllocator::instance = nullptr;

Turbo::Core::TVulkanAllocator::TVulkanAllocator()
{
    TVulkanAllocator::vkAllocationCallbacks.pUserData = nullptr;
    TVulkanAllocator::vkAllocationCallbacks.pfnAllocation = &TVulkanAllocator::Allocate;
    TVulkanAllocator::vkAllocationCallbacks.pfnReallocation = &TVulkanAllocator::Reallocate;
    TVulkanAllocator::vkAllocationCallbacks.pfnFree = &TVulkanAllocator::Free;
    TVulkanAllocator::vkAllocationCallbacks.pfnInternalAllocation = &TVulkanAllocator::InternalAllocationNotification;
    TVulkanAllocator::vkAllocationCallbacks.pfnInternalFree = &TVulkanAllocator::InternalFreeNotification;
}

Turbo::Core::TVulkanAllocator::~TVulkanAllocator()
{
}

Turbo::Core::TRefPtr<Turbo::Core::TVulkanAllocator> Turbo::Core::TVulkanAllocator::Instance()
{
    if (!TVulkanAllocator::instance.Valid())
    {
        TVulkanAllocator::instance = new TVulkanAllocator();
    }
    return TVulkanAllocator::instance;
}

void Turbo::Core::TVulkanAllocator::Destory()
{
    if (TVulkanAllocator::instance.Valid())
    {
        // delete TVulkanAllocator::instance;
        TVulkanAllocator::instance = nullptr;
    }
}

void *VKAPI_PTR Turbo::Core::TVulkanAllocator::Allocate(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    // std::cout << "TAllocator::Allocation"
    //           << " Alignment::" << alignment << " Size::" << (size / 1024.0f) << " KB" << std::endl;

    return Turbo::Core::TAllocator::Allocate(size, alignment);
}

void *VKAPI_PTR Turbo::Core::TVulkanAllocator::Reallocate(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    // std::cout << "TAllocator::Reallocation" << std::endl;

    return Turbo::Core::TAllocator::Reallocate(pOriginal, size, alignment);
}

void VKAPI_PTR Turbo::Core::TVulkanAllocator::Free(void *pUserData, void *pMemory)
{
    // std::cout << "TAllocator::Free" << std::endl;

    return Turbo::Core::TAllocator::Free(pMemory);
}

void VKAPI_PTR Turbo::Core::TVulkanAllocator::InternalAllocationNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
    // std::cout << "TAllocator::InternalAllocationNotification" << std::endl;
}

void VKAPI_PTR Turbo::Core::TVulkanAllocator::InternalFreeNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
{
    // std::cout << "TAllocator::InternalFreeNotification" << std::endl;
}

VkAllocationCallbacks *Turbo::Core::TVulkanAllocator::GetVkAllocationCallbacks()
{
    return &TVulkanAllocator::vkAllocationCallbacks;
}

std::string Turbo::Core::TVulkanAllocator::ToString() const
{
    return std::string();
}
