#pragma once
#ifndef TURBO_CORE_TPIPELINECACHE_H
#define TURBO_CORE_TPIPELINECACHE_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

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

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif