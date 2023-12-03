#pragma once
#ifndef TURBO_CORE_TMEMORYTYPE_H
#define TURBO_CORE_TMEMORYTYPE_H
#include "TInfo.h"
#include "TMemoryHeapInfo.h"

namespace Turbo
{
namespace Core
{
typedef VkMemoryPropertyFlags TMemoryPropertyFlags;

class TMemoryTypeInfo : public TInfo
{
  public:
    friend class TPhysicalDevice;

  private:
    TMemoryPropertyFlags memoryPropertyFlags;
    TMemoryHeapInfo memoryHeap;

    uint32_t index;

  public:
    explicit TMemoryTypeInfo();
    ~TMemoryTypeInfo();

  public:
    TMemoryHeapInfo GetMemoryHeap() const;

    uint32_t GetIndex() const;

    bool IsDeviceLocal() const;
    bool IsHostVisible() const;
    bool IsHostCoherent() const;
    bool IsHostCached() const;
    bool IsLazilyAllocated() const;
    bool IsProtected() const;

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TMEMORYTYPE_H