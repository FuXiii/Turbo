#pragma once
#ifndef TURBO_CORE_TPHYSICALDEVICE_H
#define TURBO_CORE_TPHYSICALDEVICE_H
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

class TPhysicalDeviceFeatures : public Turbo::Core::TInfo
{
  public:
    bool geometryShader = false;
    bool tessellationShader = false;
    bool sampleRateShading = false;
    bool depthClamp = false;
    bool depthBiasClamp = false;
    bool wideLines = false;
    bool fillModeNonSolid = false;
    bool samplerAnisotropy = false;
    bool timelineSemaphore = false;
    bool dynamicRendering = false;

    bool taskShaderNV = false;
    bool meshShaderNV = false;
    bool taskShaderEXT = false;
    bool meshShaderEXT = false;
    bool multiviewMeshShaderEXT = false;
    bool primitiveFragmentShadingRateMeshShaderEXT = false;
    bool meshShaderQueriesEXT = false;

    bool accelerationStructure = false;
    bool accelerationStructureCaptureReplay = false;
    bool accelerationStructureIndirectBuild = false;
    bool accelerationStructureHostCommands = false;
    bool descriptorBindingAccelerationStructureUpdateAfterBind = false;

    bool logicOp = false;

    bool bufferDeviceAddress = false;
    bool bufferDeviceAddressCaptureReplay = false;
    bool bufferDeviceAddressMultiDevice = false;

    bool rayTracingPipeline = false;
    bool rayTracingPipelineShaderGroupHandleCaptureReplay = false;
    bool rayTracingPipelineShaderGroupHandleCaptureReplayMixed = false;
    bool rayTracingPipelineTraceRaysIndirect = false;
    bool rayTraversalPrimitiveCulling = false;

    bool rayQuery = false;

    bool shaderSubgroupClock = false;
    bool shaderDeviceClock = false;

  public:
    TPhysicalDeviceFeatures() = default;
    ~TPhysicalDeviceFeatures() = default;

  public:
    virtual std::string ToString() const override;
};

class TInstance;
class TPhysicalDeviceInfo;
class TQueueFamilyInfo;
class TDevice;

struct TPhysicalDeviceFunctionTable;
using TPhysicalDeviceDriver = TPhysicalDeviceFunctionTable;

class TPhysicalDevice : public TVulkanHandle
{
  public:
    friend class TInstance;
    // friend class TPhysicalDeviceProxy;
    friend class TDevice;
    friend class TDeviceMemory;
    friend class TDeviceQueue;

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TInstance> instance;
    T_VULKAN_HANDLE_HANDLE VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TDevice>> devices;

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

    // OLD:T_VULKAN_HANDLE_REFRESH_DATA std::map<TQueueFamilyInfo, uint32_t> availableQueueCountMap;

    TPhysicalDeviceDriver *physicalDeviceDriver = nullptr;

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
    virtual void RemoveChildHandle(TDevice *device);
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPhysicalDevice(TInstance *instance, uint32_t index);

  protected:
    virtual ~TPhysicalDevice();

  private:
    // OLD:void AvailableQueueCountMinusOneByQueueFamilyIndex(uint32_t queueFamilyIndex);
    // OLD:void AvailableQueueCountPlussOneByQueueFamilyIndex(uint32_t queueFamilyIndex);
    // OLD:void ResetQueueCountMap();

  public:
    VkPhysicalDevice GetVkPhysicalDevice();
    std::string GetDeviceName() const;
    TVersion GetDeviceApiVersion() const;
    uint32_t GetDriverVersion() const;
    TVendorInfo GetVendor() const;
    uint32_t GetPhysicalDeviceID() const;
    TPhysicalDeviceType GetDeviceType() const;
    TPipelineCacheUUID GetDevicePiplineCacheUUID() const;
    TPhysicalDeviceLimits GetDeviceLimits() const;
    // template <typename T>
    // T GetDeviceLimit(/*enume key*/);
    // VkPhysicalDeviceSparseProperties
    // GetDeviceSparseProperties();

    TPhysicalDeviceFeatures GetDeviceFeatures() const;
    // bool IsDeviceSupportFeature(enume key);

    size_t GetSupportExtensionCount() const;
    const std::vector<TExtensionInfo> &GetSupportExtensions() const;
    TExtensionInfo GetExtensionByType(TExtensionType extensionType) const;
    bool IsSupportExtension(std::string extensionName) const;
    bool IsSupportExtension(TExtensionType extensionType) const;

    size_t GetSupportLayerCount() const;
    const std::vector<TLayerInfo> &GetSupportLayers() const;
    bool IsSupportLayer(std::string layerName) const;
    bool IsSupportLayer(TLayerType layerType) const;

    std::vector<TFormatInfo> GetSupportFormats() const;
    bool IsSupportFormat(TFormatType formatType) const;
    bool IsSupportFormat(TFormatInfo format) const;
    TFormatInfo GetFormatInfo(TFormatType formatType) const;

    size_t GetQueueFamilyCount() const;
    const std::vector<TQueueFamilyInfo> &GetQueueFamilys() const;
    bool IsHasQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex) const;
    TQueueFamilyInfo GetQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex) const;

    size_t GetMemoryTypeCount() const;
    const std::vector<TMemoryTypeInfo> &GetMemoryTypes() const;
    bool IsHasMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex) const;
    TMemoryTypeInfo GetMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex) const;

    bool IsSupportGraphics() const;
    TQueueFamilyInfo GetBestGraphicsQueueFamily() const;

    bool IsSupportCompute() const;
    TQueueFamilyInfo GetBestComputeQueueFamily() const;

    bool IsSupportTransfer() const;
    TQueueFamilyInfo GetBestTransferQueueFamily() const;

    bool IsSupportSparse() const;
    TQueueFamilyInfo GetBestSparseQueueFamily() const;

    bool IsSupportProtected() const;
    TQueueFamilyInfo GetBestProtectedQueueFamily() const;

    uint32_t GetPerformanceScore() const;

    TInstance *GetInstance();

    uint32_t GetAvailableQueueCount(TQueueFamilyInfo &queueFamily) const;

    uint32_t GetAvailableQueueCount(uint32_t queueFamilyIndex) const;

    bool IsFormatSupportImage(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    bool IsFormatSupportImage(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;

    TExtent3D GetMaxImageExtent(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    TExtent3D GetMaxImageExtent(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    uint32_t GetMaxImageMipLevels(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    uint32_t GetMaxImageMipLevels(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    uint32_t GetMaxImageArrayLayers(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    uint32_t GetMaxImageArrayLayers(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    TSampleCounts GetSupportImageSampleCounts(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    TSampleCounts GetSupportImageSampleCounts(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    TDeviceSize GetMaxImageResourceSize(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;
    TDeviceSize GetMaxImageResourceSize(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const;

    TFormatFeatures GetLinearFeatures(TFormatType formatType) const;
    TFormatFeatures GetLinearFeatures(TFormatInfo &format) const;
    TFormatFeatures GetOptimalFeatures(TFormatType formatType) const;
    TFormatFeatures GetOptimalFeatures(TFormatInfo &format) const;
    TFormatFeatures GetBufferFeatures(TFormatType formatType) const;
    TFormatFeatures GetBufferFeatures(TFormatInfo &format) const;

    const TPhysicalDeviceDriver *GetPhysicalDeviceDriver();

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TPHYSICALDEVICE_H