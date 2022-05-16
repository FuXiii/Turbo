#pragma once
#ifndef TCOMMANDBUFFERPOOL_H
#define TCOMMANDBUFFERPOOL_H
#include "TPool.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{

class TDeviceQueue;
class TCommandBuffer;

class TCommandBufferPool : public Turbo::Core::TPool<TCommandBuffer>, public Turbo::Core::TVulkanHandle
{
  public:
    friend class TDeviceQueue;
    friend class TCommandBuffer;

  private:
    T_VULKAN_HANDLE_PARENT TDeviceQueue *deviceQueue = nullptr;
    T_VULKAN_HANDLE_HANDLE VkCommandPool vkCommandPool = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN; // std::vector<T *>  Turbo::Core::TPool::pool

  protected:
    virtual void AddChildHandle(TCommandBuffer *commandBuffer);
    virtual TCommandBuffer *RemoveChildHandle(TCommandBuffer *commandBuffer);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TCommandBufferPool(TDeviceQueue *deviceQueue);
    ~TCommandBufferPool();

    TCommandBuffer *Allocate();

    TDeviceQueue *GetDeviceQueue();
    VkCommandPool GetVkCommandPool();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif // !TCOMMANDBUFFERPOOL_H