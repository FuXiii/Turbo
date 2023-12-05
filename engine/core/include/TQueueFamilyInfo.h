#pragma once
#ifndef TURBO_CORE_TQUEUEFAMILY_H
#define TURBO_CORE_TQUEUEFAMILY_H
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
class TQueueFamilyInfo : public TInfo
{
  public:
    friend class TPhysicalDevice;
    friend class TDevice;

  private:
    TQueueFlags queueFlags;
    uint32_t queueCount;
    uint32_t timestampValidBits;
    TExtent3D minImageTransferGranularity;

    uint32_t index;

    uint32_t score;

  private:
    void CalculatePerformanceScore();

  public:
    explicit TQueueFamilyInfo();
    TQueueFamilyInfo(const TQueueFamilyInfo &obj);
    ~TQueueFamilyInfo();

  public:
    bool IsSupportGraphics() const;
    bool IsSupportCompute() const;
    bool IsSupportTransfer() const;
    bool IsSupportSparse() const;
    bool IsSupportProtected() const;

    bool IsSupportPresent() const;

    uint32_t GetQueueCount() const;
    uint32_t GetTimestampValidBits() const;
    VkExtent3D GetMinImageTransferGranularity() const;
    uint32_t GetIndex() const;

    uint32_t GetPerformanceScore() const;

    bool operator==(const TQueueFamilyInfo &queueFamily) const;
    bool operator!=(const TQueueFamilyInfo &queueFamily) const;
    bool operator<(const TQueueFamilyInfo &queueFamily) const;

    virtual std::string ToString() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TQUEUEFAMILY_H
