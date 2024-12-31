#pragma once
#ifndef TURBO_CORE_TINSTANCE_H
#define TURBO_CORE_TINSTANCE_H
#include "TLayerInfo.h"
#include "TVulkanHandle.h"
#include <vector>

namespace Turbo
{
namespace Core
{
class TDevice;
class TPhysicalDevice;

struct TInstanceFunctionTable;
using TInstanceDriver = TInstanceFunctionTable;

class TInstance : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TEngine;
    friend class TPhysicalDevice;

  private:
    T_VULKAN_HANDLE_PARENT
    T_VULKAN_HANDLE_HANDLE VkInstance vkInstance = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TPhysicalDevice>> physicalDevices;

    T_VULKAN_HANDLE_DATA std::vector<TLayerInfo> enabledLayers;
    T_VULKAN_HANDLE_DATA std::vector<TExtensionInfo> enabledExtensions;
    T_VULKAN_HANDLE_DATA TVersion vulkanVersion;

    T_VULKAN_HANDLE_REFRESH_DATA std::vector<TLayerInfo> supportLayers;
    T_VULKAN_HANDLE_REFRESH_DATA std::vector<TExtensionInfo> supportExtensions;

    TInstanceDriver *instanceDriver = nullptr;

  protected:
    bool IsHaveHandle(TPhysicalDevice *physicalDevice);
    virtual void AddChildHandle(TPhysicalDevice *physicalDevice);
    virtual void RemoveChildHandle(TPhysicalDevice *physicalDevice);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;
    virtual void InspectExtensionAndVersionDependencies(TExtensionType extensionType);

  public:
    static bool IsSupportVulkan();
    static TVersion GetVulkanInstanceVersion();

  private:
    VkResult CreateVkInstance(std::vector<TLayerInfo> *enabledLayers, std::vector<TExtensionInfo> *enabledExtensions, TVersion *vulkanVersion);

  public:
    explicit TInstance(std::vector<TLayerInfo> *enabledLayers = nullptr, std::vector<TExtensionInfo> *enabledExtensions = nullptr, TVersion *vulkanVersion = nullptr);
    // explicit TInstance(const TInstance &instance) = delete;
  protected:
    virtual ~TInstance();

  public:
    VkInstance GetVkInstance();
    TVersion GetVulkanVersion() const;

    size_t GetSupportLayerCount() const;
    std::vector<TLayerInfo> GetSupportLayers() const;
    bool IsSupportLayer(std::string layerName) const;
    bool IsSupportLayer(TLayerType layerType) const;

    size_t GetSupportExtensionCount() const;
    std::vector<TExtensionInfo> GetSupportExtensions() const;
    bool IsSupportExtension(std::string extensionName) const;
    bool IsSupportExtension(TExtensionType extensionType) const;

    size_t GetEnabledLayerCount() const;
    std::vector<TLayerInfo> GetEnabledLayers() const;
    bool IsEnabledLayer(std::string layerName) const;
    bool IsEnabledLayer(TLayerType layerType) const;

    size_t GetEnabledExtensionCount() const;
    std::vector<TExtensionInfo> GetEnabledExtensions() const;
    bool IsEnabledExtension(std::string extensionName) const;
    bool IsEnabledExtension(TExtensionType extensionType) const;

    uint32_t GetPhysicalDeviceCount() const;
    TPhysicalDevice *GetPhysicalDevice(uint32_t index);
    const std::vector<TPhysicalDevice *> &GetPhysicalDevices();
    TPhysicalDevice *GetBestPhysicalDevice();

    const TInstanceDriver *GetInstanceDriver();

    TExtensionInfo GetExtensionByType(TExtensionType extensionType) const;

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TINSTANCE_H