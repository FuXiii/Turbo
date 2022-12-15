#pragma once
#ifndef TURBO_CORE_TBUFFER_H
#define TURBO_CORE_TBUFFER_H
#include "TVulkanHandle.h"
#include "TMemoryTypeInfo.h"

namespace Turbo
{
namespace Core
{
class TBufferCreateInfo;
class TDevice;
class TQueueFamilyInfo;
class TDeviceMemory;

typedef enum class TIndexType
{
    UINT16 = 0,
    UINT32 = 1
} TIndexType;

typedef enum TBufferUsageBits
{
    BUFFER_TRANSFER_SRC = 0x00000001,
    BUFFER_TRANSFER_DST = 0x00000002,
    BUFFER_UNIFORM_TEXEL = 0x00000004,
    BUFFER_STORAGE_TEXEL = 0x00000008,
    BUFFER_UNIFORM_BUFFER = 0x00000010,
    BUFFER_STORAGE_BUFFER = 0x00000020,
    BUFFER_INDEX_BUFFER = 0x00000040,
    BUFFER_VERTEX_BUFFER = 0x00000080,
    BUFFER_INDIRECT_BUFFER = 0x00000100,
} TBufferUsageBits;
typedef VkFlags TBufferUsages;

class TBuffer : public TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkBuffer vkBuffer = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocation = nullptr;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocationInfo = nullptr;
    T_VULKAN_HANDLE_CHILDREN;

    T_VULKAN_HANDLE_DATA TDeviceSize size = 0;
    T_VULKAN_HANDLE_DATA VkBufferCreateFlags bufferFlags = 0;
    T_VULKAN_HANDLE_DATA TMemoryFlags memoryFlags = 0;
    T_VULKAN_HANDLE_DATA TBufferUsages usages = 0;

  protected:
    // virtual void AddChildHandle(TCommandBufferPool *commandBufferPool);
    // virtual TCommandBufferPool *RemoveChildHandle(TCommandBufferPool *commandBufferPool);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TBuffer(TDevice *device, VkBufferCreateFlags bufferFlags, TBufferUsages usages, TMemoryFlags memoryFlags, TDeviceSize size);
    ~TBuffer();

    void *Map();
    void Unmap();

    void Flush(TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE);

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

    TMemoryTypeInfo GetMemoryTypeInfo();

    VkBuffer GetVkBuffer();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TBUFFER_H