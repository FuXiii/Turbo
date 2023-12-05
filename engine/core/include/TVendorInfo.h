#pragma once
#ifndef TURBO_CORE_TVENDORINFO_H
#define TURBO_CORE_TVENDORINFO_H
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
typedef enum class TVendorType
{
    UNDEFINED = 0x0,
    VIVANTE = VkVendorId::VK_VENDOR_ID_VIV,
    VERISILICON = VkVendorId::VK_VENDOR_ID_VSI,
    KAZAN = VkVendorId::VK_VENDOR_ID_KAZAN,
    CODEPLAY = VkVendorId::VK_VENDOR_ID_CODEPLAY,
    MESA = 0x10005, // VkVendorId::VK_VENDOR_ID_MESA
    NVIDIA = 0x10DE,
    INTEL = 0x8086
} TVendorEnum;

class TVendorInfo : public Turbo::Core::TInfo
{
  public:
    friend class TPhysicalDevice;

  private:
    TVendorType vendorType;
    uint32_t vendorID;

  public:
    explicit TVendorInfo();
    explicit TVendorInfo(TVendorType vendorType, uint32_t vendorID);
    ~TVendorInfo();

  public:
    TVendorType GetVendorType() const;
    uint32_t GetVendorID() const;

    virtual std::string ToString() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TVENDORINFO_H