#pragma once
#ifndef TURBO_CORE_TBUFFER_H
#define TURBO_CORE_TBUFFER_H
#include "TMemoryTypeInfo.h"
#include "TVulkanHandle.h"

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
    BUFFER_SHADER_DEVICE_ADDRESS = 0x00020000,
    BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY = 0x00080000,
    BUFFER_ACCELERATION_STRUCTURE_STORAGE = 0x00100000,
    BUFFER_SHADER_BINDING_TABLE = 0x00000400
} TBufferUsageBits;
typedef VkFlags TBufferUsages;

class TBuffer : public TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr; // TDevice* device = nullptr
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
    explicit TBuffer(const TRefPtr<TDevice> &device, VkBufferCreateFlags bufferFlags, TBufferUsages usages, TMemoryFlags memoryFlags, TDeviceSize size);

  protected:
    virtual ~TBuffer();

  public:
    bool IsMappable() const;
    void *Map();
    void Unmap();

    void Flush(TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE);

  public:
    TBufferUsageFlags GetBufferUsageFlags() const;
    bool IsTransferSource() const;
    bool IsTransferDestination() const;
    bool IsUniformTexelBuffer() const;
    bool IsStorageTexelBuffer() const;
    bool IsUniformBuffer() const;
    bool IsStorageBuffer() const;
    bool IsIndexBuffer() const;
    bool IsVertexBuffer() const;
    bool IsIndirectBuffer() const;

    TMemoryTypeInfo GetMemoryTypeInfo() const;

    VkBuffer GetVkBuffer();

    virtual std::string ToString() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TBUFFER_H