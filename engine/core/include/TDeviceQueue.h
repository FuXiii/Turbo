#pragma once
#ifndef TURBO_CORE_TDEVICEQUEUE_H
#define TURBO_CORE_TDEVICEQUEUE_H
#include "TQueueFamilyInfo.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Extension
{
class TSurface;
class TSwapchain;
} // namespace Extension
} // namespace Turbo

namespace Turbo
{
namespace Core
{
class TDevice;
class TDeviceQueueCreateInfo;
class TCommandBufferPool;
class TSemaphore;
class TCommandBuffer;
class TFence;

class TDeviceQueue : public TVulkanHandle
{
  public:
    friend class TDevice;
    friend class TCommandBufferPool;

  private:
    T_VULKAN_HANDLE_PARENT TDevice *device;
    T_VULKAN_HANDLE_HANDLE VkQueue vkQueue = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TCommandBufferPool *> commandBufferPools;

    T_VULKAN_HANDLE_DATA TQueueFamilyInfo queueFamily;
    T_VULKAN_HANDLE_DATA uint32_t index;

    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;

  protected:
    virtual void AddChildHandle(TCommandBufferPool *commandBufferPool);
    virtual TCommandBufferPool *RemoveChildHandle(TCommandBufferPool *commandBufferPool);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TDeviceQueue(TDevice *device, TQueueFamilyInfo &queueFamily, uint32_t index);
    ~TDeviceQueue();

  public:
    TQueueFamilyInfo GetQueueFamily();
    TDevice *GetDevice();

    VkQueue GetVkQueue();

    bool Submit(std::vector<TSemaphore *> *waitSemaphores, std::vector<TSemaphore *> *signalSemaphores, TCommandBuffer *commandBuffer, TFence *fence);

    void WaitIdle();

    bool IsSupportSurface(Turbo::Extension::TSurface *surface);

    TResult Present(Turbo::Extension::TSwapchain *swapchain, uint32_t imageIndex);

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDEVICEQUEUE_H