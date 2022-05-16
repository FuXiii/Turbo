#pragma once
#ifndef TFENCE_H
#define TFENCE_H
#include "TVulkanHandle.h"

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

    VkFence GetVkFence();

    TResult Wait(uint64_t timeout);

    void WaitUntil();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TFENCE_H