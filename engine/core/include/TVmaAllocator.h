#pragma once
#ifndef TURBO_CORE_TVMAALLOCATOR_H
#define TURBO_CORE_TVMAALLOCATOR_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

class TVmaAllocator : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TDevice;

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr;
    T_VULKAN_HANDLE_HANDLE void *vmaAllocator;
    T_VULKAN_HANDLE_CHILDREN; // In CPP: VmaAllocator vmaAllocator;

  private:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TVmaAllocator(const TRefPtr<TDevice> &device);

  protected:
    virtual ~TVmaAllocator();

  public:
    void *GetVmaAllocator();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TVMAALLOCATOR_H