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
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkSemaphore vkSemaphore = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN VkPipelineStageFlags waitDstStageMask;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TSemaphore(const TRefPtr<TDevice> &device, TPipelineStages waitDstStageMask);

  protected:
    virtual ~TSemaphore();

  public:
    VkSemaphore GetVkSemaphore();
    TPipelineStages GetWaitDstStageMask() const;

  public:
    virtual std::string ToString()const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSEMAPHORE_H