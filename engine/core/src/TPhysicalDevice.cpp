#include "TPhysicalDevice.h"
#include "TAllocator.h"
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include "TVulkanLoader.h"

void Turbo::Core::TPhysicalDevice::CalculatePerformanceScore()
{
    switch (this->info.type)
    {
    case Turbo::Core::TPhysicalDeviceType::UNDEFINED:
        this->performanceScore += 0;
        break;
        /*case Turbo::Core::TPhysicalDeviceType::OTHER:
            this->performanceScore += 1;
            break;*/
    case Turbo::Core::TPhysicalDeviceType::INTEGRATED_GPU:
        this->performanceScore += 4;
        break;
    case Turbo::Core::TPhysicalDeviceType::DISCRETE_GPU:
        this->performanceScore += 5;
        break;
    case Turbo::Core::TPhysicalDeviceType::VIRTUAL_GPU:
        this->performanceScore += 6;
        break;
    case Turbo::Core::TPhysicalDeviceType::CPU:
        this->performanceScore += 1;
        break;
    default:
        break;
    }

    if (this->info.apiVersion.GetValidVulkanVersion() >= TVersion(1, 2, 0, 0))
    {
        this->performanceScore += 2;
    }
    else if (this->info.apiVersion.GetValidVulkanVersion() >= TVersion(1, 1, 0, 0))
    {
        this->performanceScore += 1;
    }
    else
    {
        this->performanceScore += 0;
    }

    if (this->info.queueFamilys.size() > 0)
    {
        size_t queue_family_count = this->info.queueFamilys.size();
        for (size_t queue_family_index = 0; queue_family_index < queue_family_count; queue_family_index++)
        {
            this->performanceScore += this->info.queueFamilys[queue_family_index].GetPerformanceScore();
        }
    }
}

void Turbo::Core::TPhysicalDevice::EnumerateProperties()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;

    Turbo::Core::vkGetPhysicalDeviceProperties(this->vkPhysicalDevice, &physicalDeviceProperties);

    // name
    this->info.name = physicalDeviceProperties.deviceName;

    // api Version
    this->info.apiVersion = Turbo::Core::TVersion(VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion), VK_VERSION_MINOR(physicalDeviceProperties.apiVersion), VK_VERSION_PATCH(physicalDeviceProperties.apiVersion), 0);

    // driver Version
    this->info.driverVersion = physicalDeviceProperties.driverVersion;

    // Vender
    TVendorType vendor_type = static_cast<TVendorType>(physicalDeviceProperties.vendorID);

    switch (vendor_type)
    {
    case Turbo::Core::TVendorType::UNDEFINED:
        vendor_type = TVendorType::UNDEFINED;
        break;
    case Turbo::Core::TVendorType::VIVANTE:
        vendor_type = TVendorType::VIVANTE;
        break;
    case Turbo::Core::TVendorType::VERISILICON:
        vendor_type = TVendorType::VERISILICON;
        break;
    case Turbo::Core::TVendorType::KAZAN:
        vendor_type = TVendorType::KAZAN;
        break;
    case Turbo::Core::TVendorType::CODEPLAY:
        vendor_type = TVendorType::CODEPLAY;
        break;
    case Turbo::Core::TVendorType::MESA:
        vendor_type = TVendorType::MESA;
        break;
    case Turbo::Core::TVendorType::NVIDIA:
        vendor_type = TVendorType::NVIDIA;
        break;
    case Turbo::Core::TVendorType::INTEL:
        vendor_type = TVendorType::INTEL;
        break;
    default:
        vendor_type = TVendorType::UNDEFINED;
        break;
    }
    this->info.vendor.vendorID = physicalDeviceProperties.vendorID;
    this->info.vendor.vendorType = vendor_type;

    // deviceID
    this->info.id = physicalDeviceProperties.deviceID;

    // Device Type
    switch (physicalDeviceProperties.deviceType)
    {
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
        this->info.type = TPhysicalDeviceType::OTHER;
        break;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        this->info.type = TPhysicalDeviceType::INTEGRATED_GPU;
        break;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        this->info.type = TPhysicalDeviceType::DISCRETE_GPU;
        break;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        this->info.type = TPhysicalDeviceType::VIRTUAL_GPU;
        break;
    case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
        this->info.type = TPhysicalDeviceType::CPU;
        break;
    default:
        this->info.type = TPhysicalDeviceType::UNDEFINED;
        break;
    }

    // UUID
    for (uint32_t uuid_index = 0; uuid_index < VK_UUID_SIZE; uuid_index++)
    {
        this->info.pipelineCacheUUID.uuid[uuid_index] = physicalDeviceProperties.pipelineCacheUUID[uuid_index];
    }

    // Limits
    this->info.limits = physicalDeviceProperties.limits;

    // sparse limits
    this->info.sparseProperties = physicalDeviceProperties.sparseProperties;

    // Feature
    Turbo::Core::vkGetPhysicalDeviceFeatures(this->vkPhysicalDevice, &(this->info.features));
}

void Turbo::Core::TPhysicalDevice::EnumerateQueueFamily()
{
    uint32_t queue_family_count = 0;
    Turbo::Core::vkGetPhysicalDeviceQueueFamilyProperties(this->vkPhysicalDevice, &queue_family_count, nullptr);
    if (queue_family_count > 0)
    {
        std::vector<VkQueueFamilyProperties> queue_family_properties;
        queue_family_properties.resize(queue_family_count);
        this->info.queueFamilys.resize(queue_family_count);

        Turbo::Core::vkGetPhysicalDeviceQueueFamilyProperties(this->vkPhysicalDevice, &queue_family_count, queue_family_properties.data());

        for (uint32_t queue_family_index = 0; queue_family_index < queue_family_count; queue_family_index++)
        {
            this->info.queueFamilys[queue_family_index].queueFlags = queue_family_properties[queue_family_index].queueFlags;
            this->info.queueFamilys[queue_family_index].queueCount = queue_family_properties[queue_family_index].queueCount;
            this->info.queueFamilys[queue_family_index].timestampValidBits = queue_family_properties[queue_family_index].timestampValidBits;
            this->info.queueFamilys[queue_family_index].minImageTransferGranularity.width = queue_family_properties[queue_family_index].minImageTransferGranularity.width;
            this->info.queueFamilys[queue_family_index].minImageTransferGranularity.height = queue_family_properties[queue_family_index].minImageTransferGranularity.height;
            this->info.queueFamilys[queue_family_index].minImageTransferGranularity.depth = queue_family_properties[queue_family_index].minImageTransferGranularity.depth;

            this->info.queueFamilys[queue_family_index].index = queue_family_index;

            this->info.queueFamilys[queue_family_index].CalculatePerformanceScore();
        }
    }
}

void Turbo::Core::TPhysicalDevice::EnumerateSupportLayerAndExtension()
{
    this->info.supportLayers = TLayerInfo::GetPhysicalDeviceLayers(this);
    this->info.supportExtensions = TExtensionInfo::GetPhysicalDeviceExtensions(this);
}

void Turbo::Core::TPhysicalDevice::EnumerateMemoryType()
{
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
    Turbo::Core::vkGetPhysicalDeviceMemoryProperties(this->vkPhysicalDevice, &physical_device_memory_properties);

    if (physical_device_memory_properties.memoryTypeCount > 0)
    {
        this->info.memoryTypes.resize(physical_device_memory_properties.memoryTypeCount);

        for (uint32_t memory_type_index = 0; memory_type_index < physical_device_memory_properties.memoryTypeCount; memory_type_index++)
        {
            this->info.memoryTypes[memory_type_index].memoryPropertyFlags = physical_device_memory_properties.memoryTypes[memory_type_index].propertyFlags;
            uint32_t heap_index = physical_device_memory_properties.memoryTypes[memory_type_index].heapIndex;
            this->info.memoryTypes[memory_type_index].memoryHeap.memoryHeapFlags = physical_device_memory_properties.memoryHeaps[heap_index].flags;
            this->info.memoryTypes[memory_type_index].memoryHeap.size = physical_device_memory_properties.memoryHeaps[heap_index].size;
            this->info.memoryTypes[memory_type_index].index = memory_type_index;
        }
    }
}

void Turbo::Core::TPhysicalDevice::EnumerateFromat()
{
    std::vector<TFormatInfo> support_formats = TFormatInfo::GetSupportFormats(this);
    for (TFormatInfo &format_info_item : support_formats)
    {
        this->info.supportFormats[format_info_item.GetFormatType()] = format_info_item;
    }
}

void Turbo::Core::TPhysicalDevice::InitDeviceQueueParameter()
{
    this->isSupportGraphics = false;
    this->isSupportCompute = false;
    this->isSupportTransfer = false;
    this->isSupportSparse = false;
    this->isSupportProtected = false;

    uint32_t best_graphyics_queue_score = 0;
    uint32_t best_compute_queue_score = 0;
    uint32_t best_transfer_queue_score = 0;
    uint32_t best_sparse_queue_score = 0;
    uint32_t best_protected_queue_score = 0;

    size_t queue_family_count = this->info.queueFamilys.size();

    this->availableQueueCountMap.clear();

    auto ptr = &this->info.queueFamilys;

    for (size_t queue_index = 0; queue_index < queue_family_count; queue_index++)
    {
        if (this->info.queueFamilys[queue_index].IsSupportGraphics())
        {
            this->isSupportGraphics = true;

            if (best_graphyics_queue_score < this->info.queueFamilys[queue_index].GetPerformanceScore())
            {
                best_graphyics_queue_score = this->info.queueFamilys[queue_index].GetPerformanceScore();
                this->bestGraphysicsQueueFamilyIndex = queue_index;
            }
        }

        if (this->info.queueFamilys[queue_index].IsSupportCompute())
        {
            this->isSupportCompute = true;

            if (best_compute_queue_score < this->info.queueFamilys[queue_index].GetPerformanceScore())
            {
                best_compute_queue_score = this->info.queueFamilys[queue_index].GetPerformanceScore();
                this->bestComputeQueueFamilyIndex = queue_index;
            }
        }

        if (this->info.queueFamilys[queue_index].IsSupportTransfer())
        {
            this->isSupportTransfer = true;

            if (best_transfer_queue_score < this->info.queueFamilys[queue_index].GetPerformanceScore())
            {
                best_transfer_queue_score = this->info.queueFamilys[queue_index].GetPerformanceScore();
                this->bestTransferQueueFamilyIndex = queue_index;
            }
        }

        if (this->info.queueFamilys[queue_index].IsSupportSparse())
        {
            this->isSupportSparse = true;

            if (best_sparse_queue_score < this->info.queueFamilys[queue_index].GetPerformanceScore())
            {
                best_sparse_queue_score = this->info.queueFamilys[queue_index].GetPerformanceScore();
                this->bestSparseQueueFamilyIndex = queue_index;
            }
        }

        if (this->info.queueFamilys[queue_index].IsSupportProtected())
        {
            this->isSupportProtected = true;
            if (best_protected_queue_score < this->info.queueFamilys[queue_index].GetPerformanceScore())
            {
                best_protected_queue_score = this->info.queueFamilys[queue_index].GetPerformanceScore();
                this->bestProtectedQueueFamilyIndex = queue_index;
            }
        }

        this->availableQueueCountMap[this->info.queueFamilys[queue_index]] = this->info.queueFamilys[queue_index].GetQueueCount();
    }
}

void Turbo::Core::TPhysicalDevice::AddChildHandle(TDevice *device)
{
    if (device != nullptr)
    {
        this->devices.push_back(device);
    }
}

Turbo::Core::TDevice *Turbo::Core::TPhysicalDevice::RemoveChildHandle(TDevice *device)
{
    Turbo::Core::TDevice *result = nullptr;
    uint32_t index = 0;
    bool is_found = false;
    uint32_t devices_count = this->devices.size();

    for (uint32_t device_index = 0; device_index < devices_count; device_index++)
    {
        if (this->devices[device_index] == device)
        {
            result = this->devices[device_index];
            index = device_index;
            is_found = true;
            break;
        }
    }

    if (result != nullptr && is_found)
    {
        this->devices.erase(this->devices.begin() + index);
    }

    return result;
}

void Turbo::Core::TPhysicalDevice::InternalCreate()
{
    // this->vkPhysicalDevice=xxx 获取物理设备句柄
    uint32_t physical_device_count = 0;
    Turbo::Core::vkEnumeratePhysicalDevices(this->instance->GetVkInstance(), &physical_device_count, nullptr);

    std::vector<VkPhysicalDevice> vk_physical_devices;
    vk_physical_devices.resize(physical_device_count);
    Turbo::Core::vkEnumeratePhysicalDevices(this->instance->GetVkInstance(), &physical_device_count, vk_physical_devices.data());

    this->vkPhysicalDevice = vk_physical_devices[this->index];

    // 枚举各种属性：
    this->EnumerateProperties();
    this->EnumerateSupportLayerAndExtension();
    this->EnumerateQueueFamily();
    this->EnumerateMemoryType();
    this->EnumerateFromat();

    // Performance Score
    this->CalculatePerformanceScore();
    this->InitDeviceQueueParameter();

    for (TDevice *device_item : this->devices)
    {
        device_item->InternalCreate();
    }
}

void Turbo::Core::TPhysicalDevice::InternalDestroy()
{
    for (TDevice *device_item : this->devices)
    {
        device_item->InternalDestroy();
    }

    // VkPhysicalDevice 没有销毁函数，置空
    this->vkPhysicalDevice = VK_NULL_HANDLE;
}

Turbo::Core::TPhysicalDevice::TPhysicalDevice(TInstance *instance, uint32_t index) : Turbo::Core::TVulkanHandle()
{
    if (instance != nullptr)
    {
        this->instance = instance;
        this->index = index;
        this->performanceScore = 0;
        this->InternalCreate();
        this->instance->AddChildHandle(this);
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPhysicalDevice::TPhysicalDevice");
    }
}

Turbo::Core::TPhysicalDevice::~TPhysicalDevice()
{
    this->instance->RemoveChildHandle(this);

    // for (; this->devices.size() > 0;)
    // {
    //     delete this->devices[0];
    // }

    this->devices.clear();

    this->InternalDestroy();
}

size_t Turbo::Core::TPhysicalDevice::GetSupportLayerCount()
{
    return this->info.supportLayers.size();
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TPhysicalDevice::GetSupportLayers()
{
    return this->info.supportLayers;
}

bool Turbo::Core::TPhysicalDevice::IsSupportLayer(std::string layerName)
{
    if (!layerName.empty())
    {
        TLayerType layer_type = TLayerInfo::GetLayerTypeByLayerName(layerName);
        size_t support_layers_count = this->info.supportLayers.size();
        if (layer_type != TLayerType::UNDEFINED)
        {
            for (size_t layer_index = 0; layer_index < support_layers_count; layer_index++)
            {
                if (this->info.supportLayers[layer_index].GetLayerType() == layer_type)
                {
                    return true;
                }
            }
        }
        else
        {
            for (size_t layer_index = 0; layer_index < support_layers_count; layer_index++)
            {
                if (this->info.supportLayers[layer_index].GetName() == layerName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TPhysicalDevice::IsSupportLayer(TLayerType layerType)
{
    return this->IsSupportLayer(TLayerInfo::GetLayerNameByLayerType(layerType));
}

std::vector<Turbo::Core::TFormatInfo> Turbo::Core::TPhysicalDevice::GetSupportFormats()
{
    return TFormatInfo::GetSupportFormats(this);
}

bool Turbo::Core::TPhysicalDevice::IsSupportFormat(TFormatType formatType)
{
    if (this->info.supportFormats.find(formatType) != this->info.supportFormats.end())
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TPhysicalDevice::IsSupportFormat(TFormatInfo format)
{
    return this->IsSupportFormat(format.GetFormatType());
}

size_t Turbo::Core::TPhysicalDevice::GetQueueFamilyCount()
{
    return this->info.queueFamilys.size();
}

std::vector<Turbo::Core::TQueueFamilyInfo> Turbo::Core::TPhysicalDevice::GetQueueFamilys()
{
    return this->info.queueFamilys;
}

bool Turbo::Core::TPhysicalDevice::IsHasQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex)
{
    if (this->info.queueFamilys.size() > 0 && queueFamilyIndex < this->info.queueFamilys.size())
    {
        return true;
    }

    return false;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex)
{
    if (queueFamilyIndex > this->info.queueFamilys.size() - 1)
    {
        throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TPhysicalDevice::GetQueueFamilyByIndex");
    }

    return this->info.queueFamilys[queueFamilyIndex];
}

size_t Turbo::Core::TPhysicalDevice::GetMemoryTypeCount()
{
    return this->info.memoryTypes.size();
}

std::vector<Turbo::Core::TMemoryTypeInfo> Turbo::Core::TPhysicalDevice::GetMemoryTypes()
{
    return this->info.memoryTypes;
}

bool Turbo::Core::TPhysicalDevice::IsHasMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex)
{
    if (this->info.memoryTypes.size() > 0 && memoryTypeIndex < this->info.memoryTypes.size())
    {
        return true;
    }

    return false;
}

Turbo::Core::TMemoryTypeInfo Turbo::Core::TPhysicalDevice::GetMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex)
{
    if (memoryTypeIndex > this->info.memoryTypes.size() - 1)
    {
        throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TPhysicalDevice::GetMemoryTypeByIndex");
    }

    return this->info.memoryTypes[memoryTypeIndex];
}

bool Turbo::Core::TPhysicalDevice::IsSupportGraphics()
{
    return this->isSupportGraphics;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestGraphicsQueueFamily()
{
    if (this->bestGraphysicsQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestGraphysicsQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportCompute()
{
    return this->isSupportCompute;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestComputeQueueFamily()
{
    if (this->bestComputeQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestComputeQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportTransfer()
{
    return this->isSupportTransfer;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestTransferQueueFamily()
{
    if (this->bestTransferQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestTransferQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportSparse()
{
    return this->isSupportSparse;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestSparseQueueFamily()
{
    if (this->bestSparseQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestSparseQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportProtected()
{
    return this->isSupportProtected;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestProtectedQueueFamily()
{
    if (this->bestProtectedQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestProtectedQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

uint32_t Turbo::Core::TPhysicalDevice::GetPerformanceScore()
{
    return this->performanceScore;
}

Turbo::Core::TInstance *Turbo::Core::TPhysicalDevice::GetInstance()
{
    return this->instance;
}

std::string Turbo::Core::TPhysicalDevice::ToString()
{
    return std::string();
}

VkPhysicalDevice Turbo::Core::TPhysicalDevice::GetVkPhysicalDevice()
{
    return this->vkPhysicalDevice;
}

std::string Turbo::Core::TPhysicalDevice::GetDeviceName()
{
    return this->info.name;
}

Turbo::Core::TVersion Turbo::Core::TPhysicalDevice::GetDeviceApiVersion()
{
    return this->info.apiVersion;
}

uint32_t Turbo::Core::TPhysicalDevice::GetDriverVersion()
{
    return this->info.driverVersion;
}

uint32_t Turbo::Core::TPhysicalDevice::GetPhysicalDeviceID()
{
    return this->info.id;
}

Turbo::Core::TPhysicalDeviceType Turbo::Core::TPhysicalDevice::GetDeviceType()
{
    return this->info.type;
}

Turbo::Core::TPipelineCacheUUID Turbo::Core::TPhysicalDevice::GetDevicePiplineCacheUUID()
{
    return this->info.pipelineCacheUUID;
}

VkPhysicalDeviceLimits Turbo::Core::TPhysicalDevice::GetDeviceLimits()
{
    return this->info.limits;
}

VkPhysicalDeviceFeatures Turbo::Core::TPhysicalDevice::GetDeviceFeatures()
{
    return this->info.features;
}

Turbo::Core::TVendorInfo Turbo::Core::TPhysicalDevice::GetVendor()
{
    return this->info.vendor;
}

size_t Turbo::Core::TPhysicalDevice::GetSupportExtensionCount()
{
    return this->info.supportExtensions.size();
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TPhysicalDevice::GetSupportExtensions()
{
    return this->info.supportExtensions;
}

bool Turbo::Core::TPhysicalDevice::IsSupportExtension(std::string extensionName)
{
    if (!extensionName.empty())
    {
        TExtensionType extension_type = TExtensionInfo::GetExtensionTypeByExtensionName(extensionName);
        size_t support_extension_count = this->info.supportExtensions.size();
        if (extension_type != TExtensionType::UNDEFINED)
        {
            for (size_t extension_index = 0; extension_index < support_extension_count; extension_index++)
            {
                if (this->info.supportExtensions[extension_index].GetExtensionType() == extension_type)
                {
                    return true;
                }
            }
        }
        else
        {
            for (size_t extension_index = 0; extension_index < support_extension_count; extension_index++)
            {
                if (this->info.supportExtensions[extension_index].GetName() == extensionName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TPhysicalDevice::IsSupportExtension(TExtensionType extensionType)
{
    return this->IsSupportExtension(TExtensionInfo::GetExtensionNameByExtensionType(extensionType));
}

uint32_t Turbo::Core::TPhysicalDevice::GetAvailableQueueCount(Turbo::Core::TQueueFamilyInfo &queueFamily)
{
    return this->GetAvailableQueueCount(queueFamily.GetIndex());
}

uint32_t Turbo::Core::TPhysicalDevice::GetAvailableQueueCount(uint32_t queueFamilyIndex)
{
    for (auto &item : this->availableQueueCountMap)
    {
        TQueueFamilyInfo famuly_queue = item.first;
        if (famuly_queue.GetIndex() == queueFamilyIndex)
        {
            return item.second;
        }
    }

    return 0;
}

void Turbo::Core::TPhysicalDevice::AvailableQueueCountMinusOneByQueueFamilyIndex(uint32_t queueFamilyIndex)
{
    for (auto &item : this->availableQueueCountMap)
    {
        TQueueFamilyInfo famuly_queue = item.first;
        if (famuly_queue.GetIndex() == queueFamilyIndex)
        {
            item.second = item.second - 1;
            break;
        }
    }
}

void Turbo::Core::TPhysicalDevice::AvailableQueueCountPlussOneByQueueFamilyIndex(uint32_t queueFamilyIndex)
{
    for (auto &item : this->availableQueueCountMap)
    {
        TQueueFamilyInfo famuly_queue = item.first;
        if (famuly_queue.GetIndex() == queueFamilyIndex)
        {
            item.second = item.second + 1;
            break;
        }
    }
}

void Turbo::Core::TPhysicalDevice::ResetQueueCountMap()
{
    for (auto &queue_family_item : this->info.queueFamilys)
    {
        uint32_t queue_index = queue_family_item.GetIndex();
        uint32_t queue_count = queue_family_item.GetQueueCount();

        for (auto &available_queue_count_map_item : this->availableQueueCountMap)
        {
            TQueueFamilyInfo famuly_queue = available_queue_count_map_item.first;
            if (famuly_queue.GetIndex() == queue_index)
            {
                available_queue_count_map_item.second = queue_count;
                break;
            }
        }
    }
}

Turbo::Core::TExtent3D Turbo::Core::TPhysicalDevice::GetMaxImageExtent(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    VkImageFormatProperties vk_image_format_properties = {};
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    return vk_image_format_properties.maxExtent;
}

Turbo::Core::TExtent3D Turbo::Core::TPhysicalDevice::GetMaxImageExtent(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    return this->GetMaxImageExtent(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageMipLevels(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    VkImageFormatProperties vk_image_format_properties = {};
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    return vk_image_format_properties.maxMipLevels;
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageMipLevels(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    return this->GetMaxImageMipLevels(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageArrayLayers(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    VkImageFormatProperties vk_image_format_properties = {};
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    return vk_image_format_properties.maxArrayLayers;
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageArrayLayers(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    return this->GetMaxImageArrayLayers(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TSampleCounts Turbo::Core::TPhysicalDevice::GetSupportImageSampleCounts(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    VkImageFormatProperties vk_image_format_properties = {};
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    return vk_image_format_properties.sampleCounts;
}

Turbo::Core::TSampleCounts Turbo::Core::TPhysicalDevice::GetSupportImageSampleCounts(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    return this->GetSupportImageSampleCounts(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TDeviceSize Turbo::Core::TPhysicalDevice::GetMaxImageResourceSize(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    VkImageFormatProperties vk_image_format_properties = {};
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    return vk_image_format_properties.maxResourceSize;
}

Turbo::Core::TDeviceSize Turbo::Core::TPhysicalDevice::GetMaxImageResourceSize(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags)
{
    return this->GetMaxImageResourceSize(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetLinearFeatures(TFormatType formatType)
{
    VkFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    Turbo::Core::vkGetPhysicalDeviceFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, &format_properties);

    return format_properties.linearTilingFeatures;
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetLinearFeatures(TFormatInfo &format)
{
    return this->GetLinearFeatures(format.GetFormatType());
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetOptimalFeatures(TFormatType formatType)
{
    VkFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    Turbo::Core::vkGetPhysicalDeviceFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, &format_properties);

    return format_properties.optimalTilingFeatures;
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetOptimalFeatures(TFormatInfo &format)
{
    return this->GetOptimalFeatures(format.GetFormatType());
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetBufferFeatures(TFormatType formatType)
{
    VkFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    Turbo::Core::vkGetPhysicalDeviceFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, &format_properties);

    return format_properties.bufferFeatures;
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetBufferFeatures(TFormatInfo &format)
{
    return this->GetBufferFeatures(format.GetFormatType());
}
