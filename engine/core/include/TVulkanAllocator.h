#pragma once
#ifndef TURBO_CORE_TVULKANALLOCATOR_H
#define TURBO_CORE_TVULKANALLOCATOR_H
#include "TAllocator.h"

namespace Turbo
{
namespace Core
{
class TVulkanAllocator : public TAllocator
{
  private:
    static TVulkanAllocator *instance;
    static VkAllocationCallbacks vkAllocationCallbacks;

  private:
    explicit TVulkanAllocator();
    ~TVulkanAllocator();

  public:
    static TVulkanAllocator *Instance();
    static void Destory();

  public:
    static void *VKAPI_PTR Allocate(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    static void *VKAPI_PTR Reallocate(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    static void VKAPI_PTR Free(void *pUserData, void *pMemory);

    static void VKAPI_PTR InternalAllocationNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);
    static void VKAPI_PTR InternalFreeNotification(void *pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);

    VkAllocationCallbacks *GetVkAllocationCallbacks();

    virtual std::string ToString() override;
};


} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TVULKANALLOCATOR_H