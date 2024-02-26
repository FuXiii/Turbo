#pragma once
#ifndef TURBO_CORE_TREFERENCED_H
#define TURBO_CORE_TREFERENCED_H
#include <cstdint>

namespace Turbo
{
namespace Core
{
class TReferenced
{
  private:
    mutable uint32_t referenceCount = 0;

  private:
    void Release() const; // NOTE: It will force delete the memory it occupied. If you really know what you are doing now, otherwise never call it yourself!

  public:
    TReferenced();

    uint32_t Reference() const;
    uint32_t UnReference() const;
    uint32_t UnReferenceWithoutDelete() const;

    uint32_t GetReferenceCount() const;

    virtual bool Valid() const;

  protected:
    virtual ~TReferenced();
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TPHYSICALDEVICEINFO_H