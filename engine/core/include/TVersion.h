#pragma once
#ifndef TURBO_CORE_TVERSION_H
#define TURBO_CORE_TVERSION_H
#include "TInfo.h"
#include <stdint.h>

namespace Turbo
{
namespace Core
{
class TVersion : public TInfo
{
  private:
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
    uint32_t develop;

  public:
    static TVersion TurboVersion;

  public:
    explicit TVersion();
    explicit TVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t develop = 0);
    TVersion(const TVersion &version);
    ~TVersion();

  public:
    uint32_t GetVulkanVersion() const;
    TVersion GetValidVulkanVersion() const;

    uint32_t GetMajor() const;
    uint32_t GetMinor() const;
    uint32_t GetPatch() const;
    uint32_t GetDevelop() const;

    TVersion &operator=(const TVersion &version);
    bool operator==(const TVersion &version) const;
    bool operator!=(const TVersion &version) const;
    bool operator>(const TVersion &version) const;
    bool operator<(const TVersion &version) const;
    bool operator<=(const TVersion &version) const;
    bool operator>=(const TVersion &version) const;

    virtual std::string ToString() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TVERSION_H