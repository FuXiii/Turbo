#pragma once
#ifndef TURBO_CORE_TPHYSICALDEVICEINFO_H
#define TURBO_CORE_TPHYSICALDEVICEINFO_H

#include "TExtensionInfo.h"
#include "TFormatInfo.h"
#include "TInfo.h"
#include "TLayerInfo.h"
#include "TMemoryTypeInfo.h"
#include "TQueueFamilyInfo.h"
#include "TVendorInfo.h"
#include "TVersion.h"
#include <map>

namespace Turbo
{
namespace Core
{
typedef enum class TPhysicalDeviceType
{
    UNDEFINED = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER,
    OTHER = UNDEFINED,
    INTEGRATED_GPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
    DISCRETE_GPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
    VIRTUAL_GPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
    CPU = VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU
} TPhysicalDeviceTypeEnum;

typedef struct TPipelineCacheUUID
{
    uint8_t uuid[TURBO_UUID_SIZE];
} TPipelineCacheUUID;

class TPhysicalDeviceInfo : public Turbo::Core::TInfo
{
  public:
    std::string name;
    TVersion apiVersion;
    uint32_t driverVersion;
    TVendorInfo vendor;
    uint32_t id;
    TPhysicalDeviceType type;
    TPipelineCacheUUID pipelineCacheUUID;
    VkPhysicalDeviceLimits limits;
    VkPhysicalDeviceSparseProperties sparseProperties;
    VkPhysicalDeviceFeatures features;

    std::vector<TLayerInfo> supportLayers;
    std::vector<TExtensionInfo> supportExtensions;

    std::vector<TQueueFamilyInfo> queueFamilys;

    std::vector<TMemoryTypeInfo> memoryTypes;

    std::map<TFormatType, TFormatInfo> supportFormats;

  public:
    TPhysicalDeviceInfo();
    ~TPhysicalDeviceInfo();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TPHYSICALDEVICEINFO_H