#include "TPhysicalDevice.h"
#include "TAllocator.h"
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include "TVulkanLoader.h"

std::string Turbo::Core::TPhysicalDeviceFeatures::ToString() const
{
    return std::string();
}

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

    this->physicalDeviceDriver->vkGetPhysicalDeviceProperties(this->vkPhysicalDevice, &physicalDeviceProperties);

    // name
    this->info.name = physicalDeviceProperties.deviceName;

    // api Version
    this->info.apiVersion = Turbo::Core::TVersion(VK_API_VERSION_MAJOR(physicalDeviceProperties.apiVersion), VK_API_VERSION_MINOR(physicalDeviceProperties.apiVersion), VK_API_VERSION_PATCH(physicalDeviceProperties.apiVersion), 0);

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

    // TODO: we need storage extension properties [in Turbo::Core::TPhysicalDeviceInfo]

    // Feature
    // If the pNext chain includes a VkPhysicalDeviceVulkan13Features structure, then it must not include a VkPhysicalDeviceDynamicRenderingFeatures
    //  // For VK_KHR_dynamic_rendering
    //  VkPhysicalDeviceDynamicRenderingFeaturesKHR vk_physical_device_dynamic_rendering_features_khr = {};
    //  vk_physical_device_dynamic_rendering_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    //  vk_physical_device_dynamic_rendering_features_khr.pNext = nullptr;
    //  vk_physical_device_dynamic_rendering_features_khr.dynamicRendering = VK_FALSE;

    // For Vulkan1.0
    this->physicalDeviceDriver->vkGetPhysicalDeviceFeatures(this->vkPhysicalDevice, &(this->info.features));

    // NOTE: add new feature in here(don't forgot refresh pNext in feature chain)

    // For VK_KHR_shader_clock
    VkPhysicalDeviceShaderClockFeaturesKHR vk_physical_device_shader_clock_features_khr = {};
    vk_physical_device_shader_clock_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR;
    vk_physical_device_shader_clock_features_khr.pNext = nullptr;
    vk_physical_device_shader_clock_features_khr.shaderSubgroupClock = VK_FALSE;
    vk_physical_device_shader_clock_features_khr.shaderDeviceClock = VK_FALSE;

    // For VK_KHR_ray_query
    VkPhysicalDeviceRayQueryFeaturesKHR vk_physical_device_ray_query_features_khr = {};
    vk_physical_device_ray_query_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
    vk_physical_device_ray_query_features_khr.pNext = &vk_physical_device_shader_clock_features_khr;
    vk_physical_device_ray_query_features_khr.rayQuery = VK_FALSE;

    // For VK_KHR_ray_tracing_pipeline
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR vk_physical_device_ray_tracing_pipeline_features_khr = {};
    vk_physical_device_ray_tracing_pipeline_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    vk_physical_device_ray_tracing_pipeline_features_khr.pNext = &vk_physical_device_ray_query_features_khr;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipeline = VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipelineShaderGroupHandleCaptureReplay = VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipelineTraceRaysIndirect = VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTraversalPrimitiveCulling = VK_FALSE;

    // For VK_KHR_buffer_device_address
    VkPhysicalDeviceBufferDeviceAddressFeaturesKHR vk_physical_device_buffer_device_address_features_khr = {};
    vk_physical_device_buffer_device_address_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR;
    vk_physical_device_buffer_device_address_features_khr.pNext = &vk_physical_device_ray_tracing_pipeline_features_khr;
    vk_physical_device_buffer_device_address_features_khr.bufferDeviceAddress = VK_FALSE;
    vk_physical_device_buffer_device_address_features_khr.bufferDeviceAddressCaptureReplay = VK_FALSE;
    vk_physical_device_buffer_device_address_features_khr.bufferDeviceAddressMultiDevice = VK_FALSE;

    // For VK_KHR_acceleration_structure
    VkPhysicalDeviceAccelerationStructureFeaturesKHR vk_physical_device_acceleration_structure_features_khr = {};
    vk_physical_device_acceleration_structure_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    vk_physical_device_acceleration_structure_features_khr.pNext = &vk_physical_device_buffer_device_address_features_khr;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructure = VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructureCaptureReplay = VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructureIndirectBuild = VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructureHostCommands = VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.descriptorBindingAccelerationStructureUpdateAfterBind = VK_FALSE;

    // For VK_EXT_mesh_shader
    VkPhysicalDeviceMeshShaderFeaturesEXT vk_physical_device_mesh_shader_features_ext = {};
    vk_physical_device_mesh_shader_features_ext.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
    vk_physical_device_mesh_shader_features_ext.pNext = &vk_physical_device_acceleration_structure_features_khr;
    vk_physical_device_mesh_shader_features_ext.taskShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.meshShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.multiviewMeshShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.primitiveFragmentShadingRateMeshShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.meshShaderQueries = VK_FALSE;
    // For VK_NV_mesh_shader
    VkPhysicalDeviceMeshShaderFeaturesNV vk_physical_device_mesh_shader_features_nv = {};
    vk_physical_device_mesh_shader_features_nv.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
    vk_physical_device_mesh_shader_features_nv.pNext = &vk_physical_device_mesh_shader_features_ext;
    vk_physical_device_mesh_shader_features_nv.taskShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_nv.meshShader = VK_FALSE;
    //  For Vulkan1.1
    VkPhysicalDeviceVulkan11Features vk_physical_device_vulkan_1_1_features = {};
    vk_physical_device_vulkan_1_1_features.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    vk_physical_device_vulkan_1_1_features.pNext = &vk_physical_device_mesh_shader_features_nv;
    // For Vulkan1.2
    VkPhysicalDeviceVulkan12Features vk_physical_device_vulkan_1_2_features = {};
    vk_physical_device_vulkan_1_2_features.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    vk_physical_device_vulkan_1_2_features.pNext = &vk_physical_device_vulkan_1_1_features;
    // For Vulkan1.3
    VkPhysicalDeviceVulkan13Features vk_physical_device_vulkan_1_3_features = {};
    vk_physical_device_vulkan_1_3_features.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    vk_physical_device_vulkan_1_3_features.pNext = &vk_physical_device_vulkan_1_2_features;

    VkPhysicalDeviceFeatures2 vk_physical_device_features2 = {};
    vk_physical_device_features2.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    vk_physical_device_features2.pNext = &vk_physical_device_vulkan_1_3_features;
    vk_physical_device_features2.features = {};

    if (this->physicalDeviceDriver->vkGetPhysicalDeviceFeatures2 != nullptr)
    {
        this->physicalDeviceDriver->vkGetPhysicalDeviceFeatures2(this->vkPhysicalDevice, &vk_physical_device_features2);
    }
    else if (this->physicalDeviceDriver->vkGetPhysicalDeviceFeatures2KHR != nullptr)
    {
        this->physicalDeviceDriver->vkGetPhysicalDeviceFeatures2KHR(this->vkPhysicalDevice, &vk_physical_device_features2);
    }

    this->info.vulkan11Feature = vk_physical_device_vulkan_1_1_features;
    this->info.vulkan11Feature.pNext = nullptr;
    this->info.vulkan12Feature = vk_physical_device_vulkan_1_2_features;
    this->info.vulkan12Feature.pNext = nullptr;
    this->info.vulkan13Feature = vk_physical_device_vulkan_1_3_features;
    this->info.vulkan13Feature.pNext = nullptr;
    this->info.meshShaderFeaturesEXT = vk_physical_device_mesh_shader_features_ext;
    this->info.meshShaderFeaturesEXT.pNext = nullptr;
    this->info.meshShaderFeaturesNV = vk_physical_device_mesh_shader_features_nv;
    this->info.meshShaderFeaturesNV.pNext = nullptr;
    this->info.accelerationStructureFeaturesKHR = vk_physical_device_acceleration_structure_features_khr;
    this->info.accelerationStructureFeaturesKHR.pNext = nullptr;
    this->info.physicalDeviceBufferDeviceAddressFeaturesKHR = vk_physical_device_buffer_device_address_features_khr;
    this->info.physicalDeviceBufferDeviceAddressFeaturesKHR.pNext = nullptr;
    this->info.physicalDeviceRayTracingPipelineFeaturesKHR = vk_physical_device_ray_tracing_pipeline_features_khr;
    this->info.physicalDeviceRayTracingPipelineFeaturesKHR.pNext = nullptr;
    this->info.physicalDeviceRayQueryFeaturesKHR = vk_physical_device_ray_query_features_khr;
    this->info.physicalDeviceRayQueryFeaturesKHR.pNext = nullptr;
    this->info.physicalDeviceShaderClockFeaturesKHR = vk_physical_device_shader_clock_features_khr;
    this->info.physicalDeviceShaderClockFeaturesKHR.pNext = nullptr;
}

void Turbo::Core::TPhysicalDevice::EnumerateQueueFamily()
{
    uint32_t queue_family_count = 0;
    this->physicalDeviceDriver->vkGetPhysicalDeviceQueueFamilyProperties(this->vkPhysicalDevice, &queue_family_count, nullptr);
    if (queue_family_count > 0)
    {
        std::vector<VkQueueFamilyProperties> queue_family_properties;
        queue_family_properties.resize(queue_family_count);
        this->info.queueFamilys.resize(queue_family_count);

        this->physicalDeviceDriver->vkGetPhysicalDeviceQueueFamilyProperties(this->vkPhysicalDevice, &queue_family_count, queue_family_properties.data());

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
    this->physicalDeviceDriver->vkGetPhysicalDeviceMemoryProperties(this->vkPhysicalDevice, &physical_device_memory_properties);

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
    // TRefPtr<const TPhysicalDevice> physical_device = this;
    // std::vector<TFormatInfo> support_formats = TFormatInfo::GetSupportFormats(physical_device);
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

    // OLD:this->availableQueueCountMap.clear();

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

        // OLD:this->availableQueueCountMap[this->info.queueFamilys[queue_index]] = this->info.queueFamilys[queue_index].GetQueueCount();
    }
}

void Turbo::Core::TPhysicalDevice::AddChildHandle(TDevice *device)
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        // FIXME: find it, prevent duplicate additions

        this->devices.push_back(device);
    }
}

void Turbo::Core::TPhysicalDevice::RemoveChildHandle(TDevice *device)
{
    uint32_t index = 0;
    bool is_found = false;
    uint32_t devices_count = this->devices.size();

    for (uint32_t device_index = 0; device_index < devices_count; device_index++)
    {
        if (this->devices[device_index] == device)
        {
            index = device_index;
            is_found = true;
            break;
        }
    }

    if (is_found)
    {
        this->devices.erase(this->devices.begin() + index);
    }
}

void Turbo::Core::TPhysicalDevice::InternalCreate()
{
    // this->vkPhysicalDevice=xxx 获取物理设备句柄
    uint32_t physical_device_count = 0;

    PFN_vkEnumeratePhysicalDevices vk_enumerate_physical_devices = this->GetInstance()->GetInstanceDriver()->vkEnumeratePhysicalDevices;
    vk_enumerate_physical_devices(this->instance->GetVkInstance(), &physical_device_count, nullptr);

    std::vector<VkPhysicalDevice> vk_physical_devices;
    vk_physical_devices.resize(physical_device_count);
    vk_enumerate_physical_devices(this->instance->GetVkInstance(), &physical_device_count, vk_physical_devices.data());

    this->vkPhysicalDevice = vk_physical_devices[this->index];

    TRefPtr<TPhysicalDevice> temp_ref_physical_device = this;

    // 创建 TPhysicalDeviceDriver
    this->physicalDeviceDriver = new TPhysicalDeviceDriver();
    *this->physicalDeviceDriver = TVulkanLoader::Instance()->LoadPhysicalDeviceDriver(this);

    // 枚举各种属性：
    this->EnumerateProperties();
    this->EnumerateSupportLayerAndExtension();
    this->EnumerateQueueFamily();
    this->EnumerateMemoryType();
    this->EnumerateFromat();

    // Performance Score
    this->CalculatePerformanceScore();
    this->InitDeviceQueueParameter();

    // for (TDevice *device_item : this->devices)
    //{
    //     device_item->InternalCreate();
    // }

    temp_ref_physical_device.Unbind();
}

void Turbo::Core::TPhysicalDevice::InternalDestroy()
{
    for (TDevice *device_item : this->devices)
    {
        device_item->InternalDestroy();
    }

    delete this->physicalDeviceDriver;

    // VkPhysicalDevice 没有销毁函数，置空
    this->vkPhysicalDevice = VK_NULL_HANDLE;
}

Turbo::Core::TPhysicalDevice::TPhysicalDevice(TInstance *instance, uint32_t index) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(instance))
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

size_t Turbo::Core::TPhysicalDevice::GetSupportLayerCount() const
{
    return this->info.supportLayers.size();
}

const std::vector<Turbo::Core::TLayerInfo> &Turbo::Core::TPhysicalDevice::GetSupportLayers() const
{
    return this->info.supportLayers;
}

bool Turbo::Core::TPhysicalDevice::IsSupportLayer(std::string layerName) const
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

bool Turbo::Core::TPhysicalDevice::IsSupportLayer(TLayerType layerType) const
{
    return this->IsSupportLayer(TLayerInfo::GetLayerNameByLayerType(layerType));
}

std::vector<Turbo::Core::TFormatInfo> Turbo::Core::TPhysicalDevice::GetSupportFormats() const
{
    return TFormatInfo::GetSupportFormats((Turbo::Core::TPhysicalDevice *)(this));
}

bool Turbo::Core::TPhysicalDevice::IsSupportFormat(TFormatType formatType) const
{
    if (this->info.supportFormats.find(formatType) != this->info.supportFormats.end())
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TPhysicalDevice::IsSupportFormat(TFormatInfo format) const
{
    return this->IsSupportFormat(format.GetFormatType());
}

Turbo::Core::TFormatInfo Turbo::Core::TPhysicalDevice::GetFormatInfo(TFormatType formatType) const
{
    Turbo::Core::TFormatInfo result;
    if (this->IsSupportFormat(formatType))
    {
        result = this->info.supportFormats.at(formatType);
    }
    return result;
}

size_t Turbo::Core::TPhysicalDevice::GetQueueFamilyCount() const
{
    return this->info.queueFamilys.size();
}

const std::vector<Turbo::Core::TQueueFamilyInfo> &Turbo::Core::TPhysicalDevice::GetQueueFamilys() const
{
    return this->info.queueFamilys;
}

bool Turbo::Core::TPhysicalDevice::IsHasQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex) const
{
    if (this->info.queueFamilys.size() > 0 && queueFamilyIndex < this->info.queueFamilys.size())
    {
        return true;
    }

    return false;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetQueueFamilyByIndex(TQueueFamilyIndex queueFamilyIndex) const
{
    if (queueFamilyIndex > this->info.queueFamilys.size() - 1)
    {
        throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TPhysicalDevice::GetQueueFamilyByIndex");
    }

    return this->info.queueFamilys[queueFamilyIndex];
}

size_t Turbo::Core::TPhysicalDevice::GetMemoryTypeCount() const
{
    return this->info.memoryTypes.size();
}

const std::vector<Turbo::Core::TMemoryTypeInfo> &Turbo::Core::TPhysicalDevice::GetMemoryTypes() const
{
    return this->info.memoryTypes;
}

bool Turbo::Core::TPhysicalDevice::IsHasMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex) const
{
    if (this->info.memoryTypes.size() > 0 && memoryTypeIndex < this->info.memoryTypes.size())
    {
        return true;
    }

    return false;
}

Turbo::Core::TMemoryTypeInfo Turbo::Core::TPhysicalDevice::GetMemoryTypeByIndex(TMemoryTypeIndex memoryTypeIndex) const
{
    if (memoryTypeIndex > this->info.memoryTypes.size() - 1)
    {
        throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TPhysicalDevice::GetMemoryTypeByIndex");
    }

    return this->info.memoryTypes[memoryTypeIndex];
}

bool Turbo::Core::TPhysicalDevice::IsSupportGraphics() const
{
    return this->isSupportGraphics;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestGraphicsQueueFamily() const
{
    if (this->bestGraphysicsQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestGraphysicsQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportCompute() const
{
    return this->isSupportCompute;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestComputeQueueFamily() const
{
    if (this->bestComputeQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestComputeQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportTransfer() const
{
    return this->isSupportTransfer;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestTransferQueueFamily() const
{
    if (this->bestTransferQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestTransferQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportSparse() const
{
    return this->isSupportSparse;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestSparseQueueFamily() const
{
    if (this->bestSparseQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestSparseQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

bool Turbo::Core::TPhysicalDevice::IsSupportProtected() const
{
    return this->isSupportProtected;
}

Turbo::Core::TQueueFamilyInfo Turbo::Core::TPhysicalDevice::GetBestProtectedQueueFamily() const
{
    if (this->bestProtectedQueueFamilyIndex != UINT32_MAX)
    {
        return this->info.queueFamilys[this->bestProtectedQueueFamilyIndex];
    }
    return TQueueFamilyInfo();
}

uint32_t Turbo::Core::TPhysicalDevice::GetPerformanceScore() const
{
    return this->performanceScore;
}

Turbo::Core::TInstance *Turbo::Core::TPhysicalDevice::GetInstance()
{
    return this->instance;
}

const Turbo::Core::TPhysicalDeviceDriver *Turbo::Core::TPhysicalDevice::GetPhysicalDeviceDriver()
{
    return this->physicalDeviceDriver;
}

std::string Turbo::Core::TPhysicalDevice::ToString() const
{
    return std::string();
}

bool Turbo::Core::TPhysicalDevice::Valid() const
{
    if (this->vkPhysicalDevice != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}

VkPhysicalDevice Turbo::Core::TPhysicalDevice::GetVkPhysicalDevice()
{
    return this->vkPhysicalDevice;
}

std::string Turbo::Core::TPhysicalDevice::GetDeviceName() const
{
    return this->info.name;
}

Turbo::Core::TVersion Turbo::Core::TPhysicalDevice::GetDeviceApiVersion() const
{
    return this->info.apiVersion;
}

uint32_t Turbo::Core::TPhysicalDevice::GetDriverVersion() const
{
    return this->info.driverVersion;
}

uint32_t Turbo::Core::TPhysicalDevice::GetPhysicalDeviceID() const
{
    return this->info.id;
}

Turbo::Core::TPhysicalDeviceType Turbo::Core::TPhysicalDevice::GetDeviceType() const
{
    return this->info.type;
}

Turbo::Core::TPipelineCacheUUID Turbo::Core::TPhysicalDevice::GetDevicePiplineCacheUUID() const
{
    return this->info.pipelineCacheUUID;
}

VkPhysicalDeviceLimits Turbo::Core::TPhysicalDevice::GetDeviceLimits() const
{
    return this->info.limits;
}

Turbo::Core::TPhysicalDeviceFeatures Turbo::Core::TPhysicalDevice::GetDeviceFeatures() const
{
    TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.geometryShader = this->info.features.geometryShader == VK_TRUE ? true : false;
    physical_device_features.tessellationShader = this->info.features.tessellationShader == VK_TRUE ? true : false;
    physical_device_features.sampleRateShading = this->info.features.sampleRateShading == VK_TRUE ? true : false;
    physical_device_features.depthClamp = this->info.features.depthClamp == VK_TRUE ? true : false;
    physical_device_features.depthBiasClamp = this->info.features.depthBiasClamp == VK_TRUE ? true : false;
    physical_device_features.wideLines = this->info.features.wideLines == VK_TRUE ? true : false;
    physical_device_features.fillModeNonSolid = this->info.features.fillModeNonSolid == VK_TRUE ? true : false;
    physical_device_features.samplerAnisotropy = this->info.features.samplerAnisotropy == VK_TRUE ? true : false;
    physical_device_features.logicOp = this->info.features.logicOp == VK_TRUE ? true : false;

    physical_device_features.timelineSemaphore = this->info.vulkan12Feature.timelineSemaphore == VK_TRUE ? true : false;

    physical_device_features.dynamicRendering = this->info.vulkan13Feature.dynamicRendering == VK_TRUE ? true : false;

    physical_device_features.taskShaderNV = this->info.meshShaderFeaturesNV.taskShader == VK_TRUE ? true : false;
    physical_device_features.meshShaderNV = this->info.meshShaderFeaturesNV.meshShader == VK_TRUE ? true : false;
    physical_device_features.taskShaderEXT = this->info.meshShaderFeaturesEXT.taskShader == VK_TRUE ? true : false;
    physical_device_features.meshShaderEXT = this->info.meshShaderFeaturesEXT.meshShader == VK_TRUE ? true : false;
    physical_device_features.multiviewMeshShaderEXT = this->info.meshShaderFeaturesEXT.multiviewMeshShader == VK_TRUE ? true : false;
    physical_device_features.primitiveFragmentShadingRateMeshShaderEXT = this->info.meshShaderFeaturesEXT.primitiveFragmentShadingRateMeshShader == VK_TRUE ? true : false;
    physical_device_features.meshShaderQueriesEXT = this->info.meshShaderFeaturesEXT.meshShaderQueries == VK_TRUE ? true : false;

    physical_device_features.accelerationStructure = this->info.accelerationStructureFeaturesKHR.accelerationStructure == VK_TRUE ? true : false;
    physical_device_features.accelerationStructureCaptureReplay = this->info.accelerationStructureFeaturesKHR.accelerationStructureCaptureReplay == VK_TRUE ? true : false;
    physical_device_features.accelerationStructureIndirectBuild = this->info.accelerationStructureFeaturesKHR.accelerationStructureIndirectBuild == VK_TRUE ? true : false;
    physical_device_features.accelerationStructureHostCommands = this->info.accelerationStructureFeaturesKHR.accelerationStructureHostCommands == VK_TRUE ? true : false;
    physical_device_features.descriptorBindingAccelerationStructureUpdateAfterBind = this->info.accelerationStructureFeaturesKHR.descriptorBindingAccelerationStructureUpdateAfterBind == VK_TRUE ? true : false;

    physical_device_features.bufferDeviceAddress = this->info.physicalDeviceBufferDeviceAddressFeaturesKHR.bufferDeviceAddress == VK_TRUE ? true : false;
    physical_device_features.bufferDeviceAddressCaptureReplay = this->info.physicalDeviceBufferDeviceAddressFeaturesKHR.bufferDeviceAddressCaptureReplay == VK_TRUE ? true : false;
    physical_device_features.bufferDeviceAddressMultiDevice = this->info.physicalDeviceBufferDeviceAddressFeaturesKHR.bufferDeviceAddressMultiDevice == VK_TRUE ? true : false;

    physical_device_features.rayTracingPipeline = this->info.physicalDeviceRayTracingPipelineFeaturesKHR.rayTracingPipeline == VK_TRUE ? true : false;
    physical_device_features.rayTracingPipelineShaderGroupHandleCaptureReplay = this->info.physicalDeviceRayTracingPipelineFeaturesKHR.rayTracingPipelineShaderGroupHandleCaptureReplay == VK_TRUE ? true : false;
    physical_device_features.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = this->info.physicalDeviceRayTracingPipelineFeaturesKHR.rayTracingPipelineShaderGroupHandleCaptureReplayMixed == VK_TRUE ? true : false;
    physical_device_features.rayTracingPipelineTraceRaysIndirect = this->info.physicalDeviceRayTracingPipelineFeaturesKHR.rayTracingPipelineTraceRaysIndirect == VK_TRUE ? true : false;
    physical_device_features.rayTraversalPrimitiveCulling = this->info.physicalDeviceRayTracingPipelineFeaturesKHR.rayTraversalPrimitiveCulling == VK_TRUE ? true : false;

    physical_device_features.rayQuery = this->info.physicalDeviceRayQueryFeaturesKHR.rayQuery == VK_TRUE ? true : false;

    physical_device_features.shaderSubgroupClock = this->info.physicalDeviceShaderClockFeaturesKHR.shaderSubgroupClock == VK_TRUE ? true : false;
    physical_device_features.shaderDeviceClock = this->info.physicalDeviceShaderClockFeaturesKHR.shaderDeviceClock == VK_TRUE ? true : false;

    return physical_device_features;
}

Turbo::Core::TVendorInfo Turbo::Core::TPhysicalDevice::GetVendor() const
{
    return this->info.vendor;
}

size_t Turbo::Core::TPhysicalDevice::GetSupportExtensionCount() const
{
    return this->info.supportExtensions.size();
}

const std::vector<Turbo::Core::TExtensionInfo> &Turbo::Core::TPhysicalDevice::GetSupportExtensions() const
{
    return this->info.supportExtensions;
}

Turbo::Core::TExtensionInfo Turbo::Core::TPhysicalDevice::GetExtensionByType(TExtensionType extensionType) const
{
    TExtensionInfo result;
    for (const TExtensionInfo &type_item : this->info.supportExtensions)
    {
        if (type_item.GetExtensionType() == extensionType)
        {
            result = type_item;
            break;
        }
    }

    return result;
}

bool Turbo::Core::TPhysicalDevice::IsSupportExtension(std::string extensionName) const
{
    if (!extensionName.empty())
    {
        size_t support_extension_count = this->info.supportExtensions.size();
        for (size_t extension_index = 0; extension_index < support_extension_count; extension_index++)
        {
            if (this->info.supportExtensions[extension_index].GetName() == extensionName)
            {
                return true;
            }
        }
    }

    return false;
}

bool Turbo::Core::TPhysicalDevice::IsSupportExtension(TExtensionType extensionType) const
{
    return this->IsSupportExtension(TExtensionInfo::GetExtensionNameByExtensionType(extensionType));
}

uint32_t Turbo::Core::TPhysicalDevice::GetAvailableQueueCount(Turbo::Core::TQueueFamilyInfo &queueFamily) const
{
    return this->GetAvailableQueueCount(queueFamily.GetIndex());
}

uint32_t Turbo::Core::TPhysicalDevice::GetAvailableQueueCount(uint32_t queueFamilyIndex) const
{
    if (queueFamilyIndex < this->info.queueFamilys.size())
    {
        return this->info.queueFamilys[queueFamilyIndex].GetQueueCount();
    }

    return 0;
}

// OLD:void Turbo::Core::TPhysicalDevice::AvailableQueueCountMinusOneByQueueFamilyIndex(uint32_t queueFamilyIndex)
// OLD:{
// OLD:    for (auto &item : this->availableQueueCountMap)
// OLD:    {
// OLD:        TQueueFamilyInfo famuly_queue = item.first;
// OLD:        if (famuly_queue.GetIndex() == queueFamilyIndex)
// OLD:        {
// OLD:            item.second = item.second - 1;
// OLD:            break;
// OLD:        }
// OLD:    }
// OLD:}

// OLD:void Turbo::Core::TPhysicalDevice::AvailableQueueCountPlussOneByQueueFamilyIndex(uint32_t queueFamilyIndex)
// OLD:{
// OLD:    for (auto &item : this->availableQueueCountMap)
// OLD:    {
// OLD:        TQueueFamilyInfo famuly_queue = item.first;
// OLD:        if (famuly_queue.GetIndex() == queueFamilyIndex)
// OLD:        {
// OLD:            item.second = item.second + 1;
// OLD:            break;
// OLD:        }
// OLD:    }
// OLD:}

// OLD:void Turbo::Core::TPhysicalDevice::ResetQueueCountMap()
// OLD:{
// OLD:    for (auto &queue_family_item : this->info.queueFamilys)
// OLD:    {
// OLD:        uint32_t queue_index = queue_family_item.GetIndex();
// OLD:        uint32_t queue_count = queue_family_item.GetQueueCount();
// OLD:
// OLD:        for (auto &available_queue_count_map_item : this->availableQueueCountMap)
// OLD:        {
// OLD:            TQueueFamilyInfo famuly_queue = available_queue_count_map_item.first;
// OLD:            if (famuly_queue.GetIndex() == queue_index)
// OLD:            {
// OLD:                available_queue_count_map_item.second = queue_count;
// OLD:                break;
// OLD:            }
// OLD:        }
// OLD:    }
// OLD:}

bool Turbo::Core::TPhysicalDevice::IsFormatSupportImage(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    VkImageFormatProperties vk_image_format_properties = {};
    VkResult result = this->physicalDeviceDriver->vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TPhysicalDevice::IsFormatSupportImage(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    return this->IsFormatSupportImage(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TExtent3D Turbo::Core::TPhysicalDevice::GetMaxImageExtent(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    VkImageFormatProperties vk_image_format_properties = {};
    VkResult result = this->physicalDeviceDriver->vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    if (result == VkResult::VK_SUCCESS)
    {
        return vk_image_format_properties.maxExtent;
    }

    vk_image_format_properties.maxExtent.width = 0;
    vk_image_format_properties.maxExtent.height = 0;
    vk_image_format_properties.maxExtent.depth = 0;

    return vk_image_format_properties.maxExtent;
}

Turbo::Core::TExtent3D Turbo::Core::TPhysicalDevice::GetMaxImageExtent(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    return this->GetMaxImageExtent(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageMipLevels(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    VkImageFormatProperties vk_image_format_properties = {};
    VkResult result = this->physicalDeviceDriver->vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    if (result == VkResult::VK_SUCCESS)
    {
        return vk_image_format_properties.maxMipLevels;
    }

    return 0;
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageMipLevels(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    return this->GetMaxImageMipLevels(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageArrayLayers(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    VkImageFormatProperties vk_image_format_properties = {};
    VkResult result = this->physicalDeviceDriver->vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    if (result == VkResult::VK_SUCCESS)
    {
        return vk_image_format_properties.maxArrayLayers;
    }

    return 0;
}

uint32_t Turbo::Core::TPhysicalDevice::GetMaxImageArrayLayers(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    return this->GetMaxImageArrayLayers(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TSampleCounts Turbo::Core::TPhysicalDevice::GetSupportImageSampleCounts(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    VkImageFormatProperties vk_image_format_properties = {};
    VkResult result = this->physicalDeviceDriver->vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    if (result == VkResult::VK_SUCCESS)
    {
        return vk_image_format_properties.sampleCounts;
    }

    return 0;
}

Turbo::Core::TSampleCounts Turbo::Core::TPhysicalDevice::GetSupportImageSampleCounts(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    return this->GetSupportImageSampleCounts(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TDeviceSize Turbo::Core::TPhysicalDevice::GetMaxImageResourceSize(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    VkImageFormatProperties vk_image_format_properties = {};
    VkResult result = this->physicalDeviceDriver->vkGetPhysicalDeviceImageFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, (VkImageType)imageType, (VkImageTiling)tiling, usages, imageFlags, &vk_image_format_properties);
    if (result == VkResult::VK_SUCCESS)
    {
        return vk_image_format_properties.maxResourceSize;
    }

    return 0;
}

Turbo::Core::TDeviceSize Turbo::Core::TPhysicalDevice::GetMaxImageResourceSize(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags) const
{
    return this->GetMaxImageResourceSize(format.GetFormatType(), imageType, tiling, usages, imageFlags);
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetLinearFeatures(TFormatType formatType) const
{
    VkFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    this->physicalDeviceDriver->vkGetPhysicalDeviceFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, &format_properties);

    return format_properties.linearTilingFeatures;
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetLinearFeatures(TFormatInfo &format) const
{
    return this->GetLinearFeatures(format.GetFormatType());
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetOptimalFeatures(TFormatType formatType) const
{
    VkFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    this->physicalDeviceDriver->vkGetPhysicalDeviceFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, &format_properties);

    return format_properties.optimalTilingFeatures;
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetOptimalFeatures(TFormatInfo &format) const
{
    return this->GetOptimalFeatures(format.GetFormatType());
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetBufferFeatures(TFormatType formatType) const
{
    VkFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    this->physicalDeviceDriver->vkGetPhysicalDeviceFormatProperties(this->vkPhysicalDevice, (VkFormat)formatType, &format_properties);

    return format_properties.bufferFeatures;
}

Turbo::Core::TFormatFeatures Turbo::Core::TPhysicalDevice::GetBufferFeatures(TFormatInfo &format) const
{
    return this->GetBufferFeatures(format.GetFormatType());
}
