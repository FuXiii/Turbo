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

  public:
    TReferenced();

    uint32_t Reference() const;
    uint32_t UnReference() const;

    uint32_t GetReferenceCount() const;

    // NOTE: It will force delete the memory it occupied. If you really know what are you doing now, otherwise never call it yourself!
    // FIXME: It best to be private
    void Release() const;

    virtual bool Valid() const;

  protected:
    virtual ~TReferenced();
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TPHYSICALDEVICEINFO_H