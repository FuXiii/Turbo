#pragma once
#ifndef TPHYSICALDEVICE_H
#define TPHYSICALDEVICE_H
#include "TImage.h"
#include "TPhysicalDeviceInfo.h"
#include "TVulkanHandle.h"
#include <map>
#include <vector>

namespace Turbo
{
namespace Core
{
//<未实现，该功能为Vulkan 1.2版本功能>
typedef enum class TDriver
{
    UNDEFINED = 0,
    /*...*/
} TDriverEnum;
//</未实现，该功能为Vulkan 1.2版本功能>

class TInstance;
class TPhysicalDeviceInfo;
class TQueueFamilyInfo;
class TDevice;

class TPhysicalDevice : public TVulkanHandle
{
  public:
    friend class TInstance;
    // friend class TPhysicalDeviceProxy;
    friend class TDevice;
    friend class TDeviceMemory;
    friend class TDeviceQueue;

  private:
    T_VULKAN_HANDLE_PARENT TInstance *instance = nullptr;
    T_VULKAN_HANDLE_HANDLE VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TDevice *> devices;

    T_VULKAN_HANDLE_DATA uint32_t index = 0;

    T_VULKAN_HANDLE_REFRESH_DATA uint32_t performanceScore = 0;

    T_VULKAN_HANDLE_REFRESH_DATA TPhysicalDeviceInfo info;

    T_VULKAN_HANDLE_REFRESH_DATA bool isSupportGraphics = false;
    T_VULKAN_HANDLE_REFRESH_DATA bool isSupportCompute = false;
    T_VULKAN_HANDLE_REFRESH_DATA bool isSupportTransfer = false;
    T_VULKAN_HANDLE_REFRESH_DATA bool isSupportSparse = false;
    T_VULKAN_HANDLE_REFRESH_DATA bool isSupportProtected = false;

    T_VULKAN_HANDLE_REFRESH_DATA uint32_t bestGraphysicsQueueFamilyIndex = UINT32_MAX;
    T_VULKAN_HANDLE_REFRESH_DATA uint32_t bestComputeQueueFamilyIndex = UINT32_MAX;
    T_VULKAN_HANDLE_REFRESH_DATA uint32_t bestTransferQueueFamilyIndex = UINT32_MAX;
    T_VULKAN_HANDLE_REFRESH_DATA uint32_t bestSparseQueueFamilyIndex = UINT32_MAX;
    T_VULKAN_HANDLE_REFRESH_DATA uint32_t bestProtectedQueueFamilyIndex = UINT32_MAX;

    T_VULKAN_HANDLE_REFRESH_DATA std::map<TQueueFamilyInfo, uint32_t> availableQueueCountMap;

  private:
    void CalculatePerformanceScore();
    void InitDeviceQueueParameter();

    void EnumerateProperties();
    void EnumerateQueueFamily();
    void EnumerateSupportLayerAndExtension();
    void EnumerateMemoryType();
    void EnumerateFromat();

  protected:
    virtual void AddChildHandle(TDevice *device);
    virtual TDevice *RemoveChildHandle(TDevice *device);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TPhysicalDevice(TInstance *instance, uint32_t index);
    ~TPhysicalDevice();

  private:
    void AvailableQueueCountMinusOneByQueueFamilyIndex(uint32_t queueFamilyIndex);
    void AvailableQueueCountPlussOneByQueueFamilyIndex(uint32_t queueFamilyIndex);
    void ResetQueueCountMap();

  public:
    VkPhysicalDevice GetVkPhysicalDevice();
    std::string GetDeviceName();
    TVersion GetDeviceApiVersion();
    uint32_t GetDriverVersion();
    TVendorInfo GetVendor();
    uint32_t GetPhysicalDeviceID();
    TPhysicalDeviceType GetDeviceType();
    TPipelineCacheUUID GetDevicePiplineCacheUUID();
    TPhysicalDeviceLimits GetDeviceLimits();
    // template <typename T>
    // T GetDeviceLimit(/*enume key*/);//<<--��ȡ�ض�������,����ʵ�֡�����ʹ�ã���-ֵ�Ի�ö������ѯ
    // VkPhysicalDeviceSparseProperties
    // GetDeviceSparseProperties();//<<--����VkPhysicalDeviceSparseProperties�д洢��5��boolֵ�������Ƿ�ֱ���г���
    TPhysicalDeviceFeatures GetDeviceFeatures(); //ֱ�ӷ������е�����
    // bool IsDeviceSupportFeature(enume key);//<<--����ʹ�ã���-ֵ����ѯ

    size_t GetSupportExtensionCount();
    std::vector<TExtensionInfo> GetSupportExtensions();
    bool IsSupportExtension(std::string extensionName);
    bool IsSupportExtension(TExtensionType extensionType);

    size_t GetSupportLayerCount();
    std::vector<TLayerInfo> GetSupportLayers();
    bool IsSupportLayer(std::string layerName);
    bool IsSupportLayer(TLayerType layerType);

    std::vector<TFormatInfo> GetSupportFormats();
    bool IsSupportFormat(TFormatType formatType);
    bool IsSupportFormat(TFormatInfo format);

    size_t GetQueueFamilyCount();
    std::vector<TQueueFamilyInfo> GetQueueFamilys();
    bool IsHasQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex);
    TQueueFamilyInfo GetQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex);

    size_t GetMemoryTypeCount();
    std::vector<TMemoryTypeInfo> GetMemoryTypes();
    bool IsHasMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex);
    TMemoryTypeInfo GetMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex);

    bool IsSupportGraphics();
    TQueueFamilyInfo GetBestGraphicsQueueFamily();

    bool IsSupportCompute();
    TQueueFamilyInfo GetBestComputeQueueFamily();

    bool IsSupportTransfer();
    TQueueFamilyInfo GetBestTransferQueueFamily();

    bool IsSupportSparse();
    TQueueFamilyInfo GetBestSparseQueueFamily();

    bool IsSupportProtected();
    TQueueFamilyInfo GetBestProtectedQueueFamily();

    uint32_t GetPerformanceScore();

    TInstance *GetInstance();

    uint32_t GetAvailableQueueCount(TQueueFamilyInfo &queueFamily);

    uint32_t GetAvailableQueueCount(uint32_t queueFamilyIndex);

    TExtent3D GetMaxImageExtent(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    TExtent3D GetMaxImageExtent(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    uint32_t GetMaxImageMipLevels(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    uint32_t GetMaxImageMipLevels(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    uint32_t GetMaxImageArrayLayers(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    uint32_t GetMaxImageArrayLayers(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    TSampleCounts GetSupportImageSampleCounts(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    TSampleCounts GetSupportImageSampleCounts(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    TDeviceSize GetMaxImageResourceSize(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
    TDeviceSize GetMaxImageResourceSize(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);

    TFormatFeatures GetLinearFeatures(TFormatType formatType);
    TFormatFeatures GetLinearFeatures(TFormatInfo &format);
    TFormatFeatures GetOptimalFeatures(TFormatType formatType);
    TFormatFeatures GetOptimalFeatures(TFormatInfo &format);
    TFormatFeatures GetlBufferFeatures(TFormatType formatType);
    TFormatFeatures GetlBufferFeatures(TFormatInfo &format);

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TPHYSICALDEVICE_H