#pragma once
#ifndef TURBO_CORE_TPIPELINECACHE_H
#define TURBO_CORE_TPIPELINECACHE_H
#include "TVendorInfo.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

enum class TPipelineCacheHeaderVersion
{
    ONE = 1
};

class TPipelineCache : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

    size_t size;
    void *data;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipelineCache(const TRefPtr<TDevice> &device);
    TPipelineCache(const TRefPtr<TDevice> &device, size_t size, void *data);

  protected:
    virtual ~TPipelineCache();

  public:
    VkPipelineCache GetVkPipelineCache();

    size_t GetSize() const;
    TResult GetData(size_t size, void *dst) const;

    uint32_t GetHeaderSize() const;                       // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    TPipelineCacheHeaderVersion GetHeaderVersion() const; // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    TVendorInfo GetVendor() const;                        // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    uint32_t GetDeviceID() const;                         // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    std::vector<uint8_t> GetUUID() const;                 // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo

#endif