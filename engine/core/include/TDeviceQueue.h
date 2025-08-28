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
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkQueue vkQueue = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TCommandBufferPool>> commandBufferPools;

    T_VULKAN_HANDLE_DATA TQueueFamilyInfo queueFamily;
    T_VULKAN_HANDLE_DATA uint32_t index;

    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;

  protected:
    virtual void AddChildHandle(TCommandBufferPool *commandBufferPool);
    virtual void RemoveChildHandle(TCommandBufferPool *commandBufferPool);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TDeviceQueue(TDevice *device, TQueueFamilyInfo &queueFamily, uint32_t index);

  protected:
    virtual ~TDeviceQueue();

  public:
    TQueueFamilyInfo GetQueueFamily() const;
    TDevice *GetDevice();

    VkQueue GetVkQueue();

    bool Submit(const std::vector<TSemaphore *> &waitSemaphores, const std::vector<TSemaphore *> &signalSemaphores, TCommandBuffer *commandBuffer, TFence *fence = nullptr);
    bool Submit(const std::vector<TRefPtr<TSemaphore>> &waitSemaphores, const std::vector<TRefPtr<TSemaphore>> &signalSemaphores, TCommandBuffer *commandBuffer, TFence *fence = nullptr);
    bool Submit(const std::initializer_list<TSemaphore *> &waitSemaphores, const std::initializer_list<TSemaphore *> &signalSemaphores, TCommandBuffer *commandBuffer, TFence *fence = nullptr);
    bool Submit(TCommandBuffer *commandBuffer, TFence *fence);

    void WaitIdle();

    bool IsSupportSurface(Turbo::Extension::TSurface *surface) const;

    TResult Present(Turbo::Extension::TSwapchain *swapchain, uint32_t imageIndex, const std::vector<TSemaphore *> &waitSemaphores = {});

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDEVICEQUEUE_H