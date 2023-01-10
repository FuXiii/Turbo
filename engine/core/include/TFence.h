#pragma once
#include <vector>
#ifndef TURBO_CORE_TFENCE_H
#define TURBO_CORE_TFENCE_H
#include "TVulkanHandle.h"
#include <map>

namespace Turbo
{
namespace Core
{
class TDevice;

class TFence : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkFence vkFence = VK_NULL_HANDLE;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TFence(TDevice *device);
    ~TFence();

    TDevice *GetDevice();

    VkFence GetVkFence();

    TResult Wait(uint64_t timeout);

    void WaitUntil();

  public:
    virtual std::string ToString() override;
};

class TFences : public Turbo::Core::TObject
{
  private:
    std::map<TDevice *, std::vector<TFence *>> fenceMap;

  public:
    TFences() = default;
    ~TFences() = default;

    void Add(TFence *fence);

    TResult Wait(uint64_t timeout);

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TFENCE_H