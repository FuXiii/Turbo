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
    bool IsHaveHandle(const TRefPtr<TPhysicalDevice> &physicalDevice);
    virtual void AddChildHandle(const TRefPtr<TPhysicalDevice> &physicalDevice);
    virtual TRefPtr<TPhysicalDevice> RemoveChildHandle(const TRefPtr<TPhysicalDevice> &physicalDevice);
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
    TVersion GetVulkanVersion();

    size_t GetSupportLayerCount();
    std::vector<TLayerInfo> GetSupportLayers();
    bool IsSupportLayer(std::string layerName);
    bool IsSupportLayer(TLayerType layerType);

    size_t GetSupportExtensionCount();
    std::vector<TExtensionInfo> GetSupportExtensions();
    bool IsSupportExtension(std::string extensionName);
    bool IsSupportExtension(TExtensionType extensionType);

    size_t GetEnabledLayerCount();
    std::vector<TLayerInfo> GetEnabledLayers();
    bool IsEnabledLayer(std::string layerName);
    bool IsEnabledLayer(TLayerType layerType);

    size_t GetEnabledExtensionCount();
    std::vector<TExtensionInfo> GetEnabledExtensions();
    bool IsEnabledExtension(std::string extensionName);
    bool IsEnabledExtension(TExtensionType extensionType);

    uint32_t GetPhysicalDeviceCount();
    TRefPtr<TPhysicalDevice> GetPhysicalDevice(uint32_t index);
    const std::vector<TRefPtr<TPhysicalDevice>> &GetPhysicalDevices();
    TRefPtr<TPhysicalDevice> GetBestPhysicalDevice();

    const TInstanceDriver *GetInstanceDriver();

    TExtensionInfo GetExtensionByType(TExtensionType extensionType);

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TINSTANCE_H