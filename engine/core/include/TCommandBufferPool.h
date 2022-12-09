#pragma once
#ifndef TURBO_CORE_TCOMMANDBUFFERPOOL_H
#define TURBO_CORE_TCOMMANDBUFFERPOOL_H
//#include "TPool.h"
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
    T_VULKAN_HANDLE_PARENT TDeviceQueue *deviceQueue = nullptr;
    T_VULKAN_HANDLE_HANDLE VkCommandPool vkCommandPool = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TCommandBufferBase *> commandBuffers; // std::vector<T *>  Turbo::Core::TPool::pool

  protected:
    virtual void AddChildHandle(TCommandBuffer *commandBuffer);
    virtual TCommandBuffer *RemoveChildHandle(TCommandBuffer *commandBuffer);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

    void Free(TCommandBufferBase *commandBufferBase);

  public:
    TCommandBufferPool(TDeviceQueue *deviceQueue);
    ~TCommandBufferPool();

    TCommandBuffer *Allocate();
    void Free(TCommandBuffer *commandBuffer);

    TSecondaryCommandBuffer *AllocateSecondary();
    void Free(TSecondaryCommandBuffer *secondaryCommandBuffer);

    TDeviceQueue *GetDeviceQueue();
    VkCommandPool GetVkCommandPool();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TCOMMANDBUFFERPOOL_H