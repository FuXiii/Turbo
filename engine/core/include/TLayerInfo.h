#pragma once
#ifndef TURBO_CORE_TLAYER_H
#define TURBO_CORE_TLAYER_H
#include "TExtensionInfo.h"
#include "TInfo.h"
#include "TVersion.h"
#include <vector>

namespace Turbo
{
namespace Core
{
typedef enum class TLayerType
{
    UNDEFINED = 0,
    VK_LAYER_KHRONOS_VALIDATION,
    VK_LAYER_LUNARG_API_DUMP,
    VK_LAYER_LUNARG_DEVICE_SIMULATION,
    VK_LAYER_LUNARG_MONITOR,
    VK_LAYER_LUNARG_SCREENSHOT,
    VK_LAYER_LUNARG_VKTRACE,
    VK_LAYER_NV_OPTIMUS,
    VK_LAYER_VALVE_STEAM_FOSSILIZE,
    VK_LAYER_VALVE_STEAM_OVERLAY
} TLayerTypeEnum;

class TPhysicalDevice;

class TLayerInfo : public TInfo
{
  public:
    friend class TInstanceCreateInfo;
    friend class TDeviceCreateInfo;

  private:
    TLayerType layerType;
    std::string name;
    TVersion specificationVersion;
    uint32_t implementationVersion;
    std::string description;

    std::vector<TExtensionInfo> extensions;

  public:
    static size_t GetInstanceLayerCount();
    static std::vector<TLayerInfo> GetInstanceLayers();

    static size_t GetPhysicalDeviceLayerCount(const TRefPtr<TPhysicalDevice> &physicalDevice);
    static std::vector<TLayerInfo> GetPhysicalDeviceLayers(const TRefPtr<TPhysicalDevice> &physicalDevice);

    static TLayerType GetLayerTypeByLayerName(std::string layerName);
    static std::string GetLayerNameByLayerType(TLayerType layerType);

    static bool IsSupportInstanceLayer(TLayerType layerType);

  public:
    explicit TLayerInfo();
    ~TLayerInfo();

  public:
    TLayerType GetLayerType() const;
    const std::string &GetName() const;
    TVersion GetSpecificationVersion() const;
    uint32_t GetImplementationVersion() const;
    std::string GetDescription() const;

    size_t GetExtensionCount() const;
    TExtensionInfo GetExtension(uint32_t index) const;
    std::vector<TExtensionInfo> GetExtensions() const;

    bool operator==(const TLayerInfo &layer) const;
    bool operator!=(const TLayerInfo &layer) const;

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TLAYER_H