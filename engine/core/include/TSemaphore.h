#pragma once
#ifndef TURBO_CORE_TSEMAPHORE_H
#define TURBO_CORE_TSEMAPHORE_H
#include "TPipeline.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

class TSemaphore : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkSemaphore vkSemaphore = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN VkPipelineStageFlags waitDstStageMask;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TSemaphore(TDevice *device, TPipelineStages waitDstStageMask);
    ~TSemaphore();

    VkSemaphore GetVkSemaphore();

    TPipelineStages GetWaitDstStageMask();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSEMAPHORE_H