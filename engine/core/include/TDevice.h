#pragma once
#ifndef TURBO_CORE_TDEVICE_H
#define TURBO_CORE_TDEVICE_H
#include "TPhysicalDevice.h"
#include "TVulkanHandle.h"

#include "TDescriptorSetLayout.h"
#include "TPipelineLayout.h"
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

  public:
    class TLayoutManager
    {
      private:
        TRefPtr<TDevice> device;

        std::unordered_map<std::size_t, TRefPtr<TDescriptorSetLayout>> descriptorSetLayoutMap;
        std::unordered_map<std::size_t, TRefPtr<TPipelineLayout>> pipelineLayoutMap;

      public:
        TLayoutManager(TDevice *device);

        TDescriptorSetLayout *GetOrCreateLayout(const TDescriptorSetLayout::TLayout &layout);
        TPipelineLayout *GetOrCreateLayout(const TPipelineLayout::TLayout &layout);
    };

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TPhysicalDevice> physicalDevice;
    T_VULKAN_HANDLE_HANDLE VkDevice vkDevice = VK_NULL_HANDLE;
    // OLD:T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TDeviceQueue>> deviceQueues;
    T_VULKAN_HANDLE_CHILDREN std::map<TQueueFamilyIndex, std::vector<TRefPtr<TDeviceQueue>>> deviceQueues;
    T_VULKAN_HANDLE_CHILDREN TRefPtr<TVmaAllocator> vmaAllocator;

    T_VULKAN_HANDLE_DATA std::vector<std::vector<float>> deviceQueuePriorities;

    T_VULKAN_HANDLE_DATA std::vector<TLayerInfo> enabledLayers;
    T_VULKAN_HANDLE_DATA std::vector<TExtensionInfo> enabledExtensions;
    T_VULKAN_HANDLE_DATA TPhysicalDeviceFeatures enabledFeatures;

    TDeviceDriver *deviceDriver = nullptr;
    TLayoutManager *layoutManager = nullptr;

  protected:
    virtual void AddChildHandle(TDeviceQueue *deviceQueue);
    virtual void RemoveChildHandle(TDeviceQueue *deviceQueue);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;
    virtual void InspectExtensionAndVersionDependencies(TExtensionType extensionType);

    // OLD:uint32_t GetDeviceQueueCountByQueueFamily(TQueueFamilyInfo queueFamily);
    // OLD:std::vector<TQueueFamilyInfo> GetDeviceQueueFamilyInfos();

  public:
    TDevice(TPhysicalDevice *physicalDevice, std::vector<TLayerInfo> *enabledLayers = nullptr, std::vector<TExtensionInfo> *enabledExtensions = nullptr, TPhysicalDeviceFeatures *enableFeatures = nullptr);
    TDevice(TPhysicalDevice *physicalDevice, const std::vector<TLayerInfo> &enabledLayers, const std::vector<TExtensionInfo> &enabledExtensions, const TPhysicalDeviceFeatures &enableFeatures);

  protected:
    virtual ~TDevice();

  public:
    VkDevice GetVkDevice();

    TPhysicalDevice *GetPhysicalDevice();

    size_t GetEnabledLayersCount() const;
    std::vector<TLayerInfo> GetEnabledLayers() const;

    size_t GetEnabledExtensionCount() const;
    std::vector<TExtensionInfo> GetEnabledExtensions();
    bool IsEnabledExtension(std::string extensionName) const;
    bool IsEnabledExtension(TExtensionType extensionType) const;

    TPhysicalDeviceFeatures GetEnableDeviceFeatures() const;

    TVmaAllocator *GetVmaAllocator();

    TDeviceQueue *GetBestGraphicsQueue();
    TDeviceQueue *GetBestComputeQueue();
    TDeviceQueue *GetBestTransferQueue();
    TDeviceQueue *GetBestSparseBindingQueue();
    TDeviceQueue *GetBestProtectedQueue();

    void WaitIdle();

    const TDeviceDriver *GetDeviceDriver();

    TLayoutManager &GetLayoutManager();

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDEVICE_H
