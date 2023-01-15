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
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

    size_t size;
    void *data;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipelineCache(TDevice *device);
    TPipelineCache(TDevice *device, size_t size, void *data);
    ~TPipelineCache();

    VkPipelineCache GetVkPipelineCache();

    size_t GetSize();
    TResult GetData(size_t size, void *dst);

    uint32_t GetHeaderSize();                       // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    TPipelineCacheHeaderVersion GetHeaderVersion(); // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    TVendorInfo GetVendor();                        // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    uint32_t GetDeviceID();                         // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    std::vector<uint8_t> GetUUID();                 // FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif