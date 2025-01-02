#pragma once
#ifndef TURBO_CORE_TCOMMANDBUFFERPOOL_H
#define TURBO_CORE_TCOMMANDBUFFERPOOL_H
// #include "TPool.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{

class TDeviceQueue;
class TCommandBufferBase;
class TCommandBuffer;
class TSecondaryCommandBuffer;

class TCommandBufferPool : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TDeviceQueue;
    friend class TCommandBuffer;

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDeviceQueue> deviceQueue;
    T_VULKAN_HANDLE_HANDLE VkCommandPool vkCommandPool = VK_NULL_HANDLE;
    // OLD: T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TCommandBufferBase>> commandBuffers;

    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TCommandBuffer>> commandBuffers;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TSecondaryCommandBuffer>> secondaryCommandBuffers;

  protected:
    // virtual void AddChildHandle(const TRefPtr<TCommandBuffer> &commandBuffer);
    virtual void AddChildHandle(TCommandBuffer *commandBuffer);
    virtual TCommandBuffer *RemoveChildHandle(TCommandBuffer *commandBuffer);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

    bool Free(const TCommandBufferBase *commandBufferBase);

  public:
    // TCommandBufferPool(const TRefPtr<TDeviceQueue> &deviceQueue);
    TCommandBufferPool(TDeviceQueue *deviceQueue);

  protected:
    virtual ~TCommandBufferPool();

  public:
    TCommandBuffer *Allocate();
    void Free(TCommandBuffer *commandBuffer);

    TSecondaryCommandBuffer *AllocateSecondary();
    void Free(TSecondaryCommandBuffer *secondaryCommandBuffer);

    TDeviceQueue *GetDeviceQueue();
    VkCommandPool GetVkCommandPool();

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TCOMMANDBUFFERPOOL_H