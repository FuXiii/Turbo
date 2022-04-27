#pragma once
#ifndef TBUFFER_H
#define TBUFFER_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TBufferCreateInfo;
class TDevice;
class TQueueFamilyInfo;
class TDeviceMemory;

class TBuffer : public TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkBuffer vkBuffer = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocation = nullptr;
    T_VULKAN_HANDLE_CHILDREN;

    T_VULKAN_HANDLE_DATA VkDeviceSize size = 0;
    T_VULKAN_HANDLE_DATA VkBufferCreateFlags bufferFlags = 0;
    T_VULKAN_HANDLE_DATA TMemoryFlags memoryFlags = 0;
    T_VULKAN_HANDLE_DATA VkBufferUsageFlags usage = 0;

  protected:
    // virtual void AddChildHandle(TCommandBufferPool *commandBufferPool);
    // virtual TCommandBufferPool *RemoveChildHandle(TCommandBufferPool *commandBufferPool);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TBuffer(TDevice *device, VkBufferCreateFlags bufferFlags, VkBufferUsageFlags usage, TMemoryFlags memoryFlags, VkDeviceSize size);
    ~TBuffer();

    void *Map();
    void Unmap();

  public:
    TBufferUsageFlags GetBufferUsageFlags();
    bool IsTransferSource();
    bool IsTransferDestination();
    bool IsUniformTexelBuffer();
    bool IsStorageTexelBuffer();
    bool IsUniformBuffer();
    bool IsStorageBuffer();
    bool IsIndexBuffer();
    bool IsVertexBuffer();
    bool IsIndirectBuffer();

    VkBuffer GetVkBuffer();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TBUFFER_H