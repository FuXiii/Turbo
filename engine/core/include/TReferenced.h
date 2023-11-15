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
  protected:
    mutable uint32_t referenceCount = 0;

  public:
    TReferenced();

    uint32_t Reference() const;
    uint32_t UnReference() const;

  protected:
    virtual ~TReferenced();
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TPHYSICALDEVICEINFO_H