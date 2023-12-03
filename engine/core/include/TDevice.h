#pragma once
#ifndef TURBO_CORE_TDEVICE_H
#define TURBO_CORE_TDEVICE_H
#include "TPhysicalDevice.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDeviceCreateInfo;
class TQueueFamilyInfo;
class TDeviceQueue;
class TLayerInfo;
class TExtensionInfo;
class TVmaAllocator;
enum class TExtensionType;

struct TDeviceFunctionTable;
using TDeviceDriver = TDeviceFunctionTable;

class TDevice : public Turbo::Core::TVulkanHandle
{
  private:
    friend class TDeviceQueueCreateInfo;
    friend class TDeviceQueue;
    friend class TPhysicalDevice;

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TPhysicalDevice> physicalDevice = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDevice vkDevice = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TDeviceQueue>> deviceQueues;
    T_VULKAN_HANDLE_CHILDREN TRefPtr<TVmaAllocator> vmaAllocator = nullptr;

    T_VULKAN_HANDLE_DATA std::vector<std::vector<float>> deviceQueuePriorities;

    T_VULKAN_HANDLE_DATA std::vector<TLayerInfo> enabledLayers;
    T_VULKAN_HANDLE_DATA std::vector<TExtensionInfo> enabledExtensions;
    T_VULKAN_HANDLE_DATA TPhysicalDeviceFeatures enabledFeatures;

    TDeviceDriver *deviceDriver = nullptr;

  protected:
    virtual void AddChildHandle(const TRefPtr<TDeviceQueue> &deviceQueue);
    virtual TRefPtr<TDeviceQueue> RemoveChildHandle(const TRefPtr<TDeviceQueue> &deviceQueue);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;
    virtual void InspectExtensionAndVersionDependencies(TExtensionType extensionType);

    uint32_t GetDeviceQueueCountByQueueFamily(TQueueFamilyInfo queueFamily);
    std::vector<TQueueFamilyInfo> GetDeviceQueueFamilyInfos();

  public:
    explicit TDevice(const TRefPtr<TPhysicalDevice> &physicalDevice, std::vector<TLayerInfo> *enabledLayers = nullptr, std::vector<TExtensionInfo> *enabledExtensions = nullptr, TPhysicalDeviceFeatures *enableFeatures = nullptr);

  protected:
    virtual ~TDevice();

  public:
    VkDevice GetVkDevice();

    TRefPtr<TPhysicalDevice> GetPhysicalDevice();

    size_t GetEnabledLayersCount() const;
    std::vector<TLayerInfo> GetEnabledLayers() const;

    size_t GetEnabledExtensionCount() const;
    std::vector<TExtensionInfo> GetEnabledExtensions();
    bool IsEnabledExtension(std::string extensionName) const;
    bool IsEnabledExtension(TExtensionType extensionType) const;

    TPhysicalDeviceFeatures GetEnableDeviceFeatures() const;

    TRefPtr<TVmaAllocator> GetVmaAllocator();

    TRefPtr<TDeviceQueue> GetBestGraphicsQueue();
    TRefPtr<TDeviceQueue> GetBestComputeQueue();
    TRefPtr<TDeviceQueue> GetBestTransferQueue();
    TRefPtr<TDeviceQueue> GetBestSparseBindingQueue();
    TRefPtr<TDeviceQueue> GetBestProtectedQueue();

    void WaitIdle();

    const TDeviceDriver *GetDeviceDriver();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDEVICE_H
