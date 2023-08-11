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

    template <typename T>
    static constexpr T AlignUp(T meta, size_t alignment) noexcept;

    virtual std::string ToString() override;
};

template <typename T /*TODO: T is integral [std::is_integral]*/>
constexpr T Turbo::Core::TVulkanAllocator::AlignUp(T meta, size_t alignment) noexcept
{
    return T((meta + (T(meta) - 1)) & ~T(alignment - 1));
}

} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TVULKANALLOCATOR_H