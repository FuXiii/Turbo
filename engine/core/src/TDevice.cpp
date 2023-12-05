#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TException.h"
#include "TLayerInfo.h"
#include "TPhysicalDevice.h"
#include "TQueueFamilyInfo.h"
#include "TVmaAllocator.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TDevice::AddChildHandle(const TRefPtr<TDeviceQueue> &deviceQueue)
{
    if (deviceQueue.Valid())
    {
        this->deviceQueues.push_back(deviceQueue);
    }
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TDevice::RemoveChildHandle(const TRefPtr<TDeviceQueue> &deviceQueue)
{
    // Turbo::Core::TDeviceQueue *result = nullptr;
    // uint32_t index = 0;
    // bool is_found = false;
    // uint32_t device_queues_count = this->deviceQueues.size();

    // for (uint32_t device_queue_index = 0; device_queue_index < device_queues_count; device_queue_index++)
    // {
    //     if (this->deviceQueues[device_queue_index] == deviceQueue)
    //     {
    //         result = this->deviceQueues[device_queue_index];
    //         index = device_queue_index;
    //         is_found = true;
    //         break;
    //     }
    // }

    // if (result != nullptr && is_found)
    // {
    //     this->deviceQueues.erase(this->deviceQueues.begin() + index);
    // }

    // device_queues_count = this->deviceQueues.size();

    // // refresh DeviceQueue::index
    // std::vector<Turbo::Core::TQueueFamilyInfo> device_queue_familys = this->GetDeviceQueueFamilyInfos();
    // for (Turbo::Core::TQueueFamilyInfo &queue_family_item : device_queue_familys)
    // {
    //     uint32_t index = 0;
    //     for (uint32_t device_queue_index = 0; device_queue_index < device_queues_count; device_queue_index++)
    //     {
    //         if (this->deviceQueues[device_queue_index]->queueFamily.index == queue_family_item.index)
    //         {
    //             this->deviceQueues[device_queue_index]->index = index;
    //             index = index + 1;
    //         }
    //     }
    // }
    // return result;

    // We don't need to remove it,because queue created by device.we just delete is at ~TDevice()
    return nullptr;
}

void Turbo::Core::TDevice::InspectExtensionAndVersionDependencies(TExtensionType extensionType)
{
    switch (extensionType)
    {
    case TExtensionType::UNDEFINED: {
    }
    break;
    case TExtensionType::VK_KHR_16BIT_STORAGE: {
    }
    break;
    case TExtensionType::VK_KHR_8BIT_STORAGE: {
    }
    break;
    case TExtensionType::VK_KHR_ANDROID_SURFACE: {
    }
    break;
    case TExtensionType::VK_KHR_BIND_MEMORY2: {
    }
    break;
    case TExtensionType::VK_KHR_BUFFER_DEVICE_ADDRESS: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_DEVICE_GROUP);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_BUFFER_DEVICE_ADDRESS))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_BUFFER_DEVICE_ADDRESS));
        }
    }
    break;
    case TExtensionType::VK_KHR_COPY_COMMANDS2: {
    }
    break;
    case TExtensionType::VK_KHR_CREATE_RENDERPASS2: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_MULTIVIEW);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_MAINTENANCE2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_CREATE_RENDERPASS2))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_CREATE_RENDERPASS2));
        }
    }
    break;
    case TExtensionType::VK_KHR_DEDICATED_ALLOCATION: {
    }
    break;
    case TExtensionType::VK_KHR_DEFERRED_HOST_OPERATIONS: {
        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_DEFERRED_HOST_OPERATIONS))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_DEFERRED_HOST_OPERATIONS));
        }
    }
    break;
    case TExtensionType::VK_KHR_DEPTH_STENCIL_RESOLVE: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_CREATE_RENDERPASS2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_DEPTH_STENCIL_RESOLVE))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_DEPTH_STENCIL_RESOLVE));
        }
    }
    break;
    case TExtensionType::VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE: {
    }
    break;
    case TExtensionType::VK_KHR_DEVICE_GROUP: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_DEVICE_GROUP_CREATION);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_DEVICE_GROUP))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_DEVICE_GROUP));
        }
    }
    break;
    case TExtensionType::VK_KHR_DEVICE_GROUP_CREATION: {
        // VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2 is instance extension, so we will do nothing in device extension.
    }
    break;
    case TExtensionType::VK_KHR_DISPLAY: {
    }
    break;
    case TExtensionType::VK_KHR_DISPLAY_SWAPCHAIN: {
    }
    break;
    case TExtensionType::VK_KHR_DRAW_INDIRECT_COUNT: {
    }
    break;
    case TExtensionType::VK_KHR_DRIVER_PROPERTIES: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_FENCE: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_FENCE_FD: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_FENCE_WIN32: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_MEMORY: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_MEMORY_CAPABILITIES: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_MEMORY_FD: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_MEMORY_WIN32: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_FD: {
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_WIN32: {
    }
    break;
    case TExtensionType::VK_KHR_GET_DISPLAY_PROPERTIES2: {
    }
    break;
    case TExtensionType::VK_KHR_GET_MEMORY_REQUIREMENTS2: {
    }
    break;
    case TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2: {
        // VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2 is instance extension, so we will do nothing in device extension.
    }
    break;
    case TExtensionType::VK_KHR_GET_SURFACE_CAPABILITIES2: {
    }
    break;
    case TExtensionType::VK_KHR_IMAGE_FORMAT_LIST: {
    }
    break;
    case TExtensionType::VK_KHR_IMAGELESS_FRAMEBUFFER: {
    }
    break;
    case TExtensionType::VK_KHR_INCREMENTAL_PRESENT: {
    }
    break;
    case TExtensionType::VK_KHR_MAINTENANCE1: {
    }
    break;
    case TExtensionType::VK_KHR_MAINTENANCE2: {
        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_MAINTENANCE2))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_MAINTENANCE2));
        }
    }
    break;
    case TExtensionType::VK_KHR_MAINTENANCE3: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_MAINTENANCE3))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_MAINTENANCE3));
        }
    }
    break;
    case TExtensionType::VK_KHR_MULTIVIEW: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_MULTIVIEW))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_MULTIVIEW));
        }
    }
    break;
    case TExtensionType::VK_KHR_PERFORMANCE_QUERY: {
    }
    break;
    case TExtensionType::VK_KHR_PIPELINE_EXECUTABLE_PROPERTIES: {
    }
    break;
    case TExtensionType::VK_KHR_PIPELINE_LIBRARY: {
    }
    break;
    case TExtensionType::VK_KHR_PORTABILITY_SUBSET: {
    }
    break;
    case TExtensionType::VK_KHR_PUSH_DESCRIPTOR: {
    }
    break;
    case TExtensionType::VK_KHR_RAY_TRACING: {
    }
    break;
    case TExtensionType::VK_KHR_RELAXED_BLOCK_LAYOUT: {
    }
    break;
    case TExtensionType::VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE: {
    }
    break;
    case TExtensionType::VK_KHR_SAMPLER_YCBCR_CONVERSION: {
    }
    break;
    case TExtensionType::VK_KHR_SEPARATE_DEPTH_STENCIL_LAYOUTS: {
    }
    break;
    case TExtensionType::VK_KHR_SHADER_ATOMIC_INT64: {
    }
    break;
    case TExtensionType::VK_KHR_SHADER_CLOCK: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_SHADER_CLOCK))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_SHADER_CLOCK));
        }
    }
    break;
    case TExtensionType::VK_KHR_SHADER_DRAW_PARAMETERS: {
    }
    break;
    case TExtensionType::VK_KHR_SHADER_FLOAT16_INT8: {
    }
    break;
    case TExtensionType::VK_KHR_SHADER_FLOAT_CONTROLS: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_SHADER_FLOAT_CONTROLS))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_SHADER_FLOAT_CONTROLS));
        }
    }
    break;
    case TExtensionType::VK_KHR_SHADER_NON_SEMANTIC_INFO: {
    }
    break;
    case TExtensionType::VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES: {
    }
    break;
    case TExtensionType::VK_KHR_SHARED_PRESENTABLE_IMAGE: {
    }
    break;
    case TExtensionType::VK_KHR_SPIRV_1_4: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_SHADER_FLOAT_CONTROLS);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_SPIRV_1_4))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_SPIRV_1_4));
        }
    }
    break;
    case TExtensionType::VK_KHR_STORAGE_BUFFER_STORAGE_CLASS: {
    }
    break;
    case TExtensionType::VK_KHR_SURFACE: {
    }
    break;
    case TExtensionType::VK_KHR_SURFACE_PROTECTED_CAPABILITIES: {
    }
    break;
    case TExtensionType::VK_KHR_SWAPCHAIN: {
    }
    break;
    case TExtensionType::VK_KHR_SWAPCHAIN_MUTABLE_FORMAT: {
    }
    break;
    case TExtensionType::VK_KHR_TIMELINE_SEMAPHORE: {
    }
    break;
    case TExtensionType::VK_KHR_UNIFORM_BUFFER_STANDARD_LAYOUT: {
    }
    break;
    case TExtensionType::VK_KHR_VARIABLE_POINTERS: {
    }
    break;
    case TExtensionType::VK_KHR_VULKAN_MEMORY_MODEL: {
    }
    break;
    case TExtensionType::VK_KHR_WAYLAND_SURFACE: {
    }
    break;
    case TExtensionType::VK_KHR_WIN32_KEYED_MUTEX: {
    }
    break;
    case TExtensionType::VK_KHR_WIN32_SURFACE: {
    }
    break;
    case TExtensionType::VK_KHR_XCB_SURFACE: {
    }
    break;
    case TExtensionType::VK_KHR_XLIB_SURFACE: {
    }
    break;
    case TExtensionType::VK_EXT_4444_FORMATS: {
    }
    break;
    case TExtensionType::VK_EXT_ACQUIRE_XLIB_DISPLAY: {
    }
    break;
    case TExtensionType::VK_EXT_ASTC_DECODE_MODE: {
    }
    break;
    case TExtensionType::VK_EXT_BLEND_OPERATION_ADVANCED: {
    }
    break;
    case TExtensionType::VK_EXT_BUFFER_DEVICE_ADDRESS: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_EXT_DESCRIPTOR_INDEXING))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_EXT_DESCRIPTOR_INDEXING));
        }
    }
    break;
    case TExtensionType::VK_EXT_CALIBRATED_TIMESTAMPS: {
    }
    break;
    case TExtensionType::VK_EXT_CONDITIONAL_RENDERING: {
    }
    break;
    case TExtensionType::VK_EXT_CONSERVATIVE_RASTERIZATION: {
    }
    break;
    case TExtensionType::VK_EXT_CUSTOM_BORDER_COLOR: {
    }
    break;
    case TExtensionType::VK_EXT_DEBUG_MARKER: {
    }
    break;
    case TExtensionType::VK_EXT_DEBUG_REPORT: {
    }
    break;
    case TExtensionType::VK_EXT_DEBUG_UTILS: {
    }
    break;
    case TExtensionType::VK_EXT_DEPTH_CLIP_ENABLE: {
    }
    break;
    case TExtensionType::VK_EXT_DEPTH_RANGE_UNRESTRICTED: {
    }
    break;
    case TExtensionType::VK_EXT_DESCRIPTOR_INDEXING: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_MAINTENANCE3);

        if (!this->IsEnabledExtension(TExtensionType::VK_EXT_DESCRIPTOR_INDEXING))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_EXT_DESCRIPTOR_INDEXING));
        }
    }
    break;
    case TExtensionType::VK_EXT_DEVICE_MEMORY_REPORT: {
    }
    break;
    case TExtensionType::VK_EXT_DIRECT_MODE_DISPLAY: {
    }
    break;
    case TExtensionType::VK_EXT_DIRECTFB_SURFACE: {
    }
    break;
    case TExtensionType::VK_EXT_DISCARD_RECTANGLES: {
    }
    break;
    case TExtensionType::VK_EXT_DISPLAY_CONTROL: {
    }
    break;
    case TExtensionType::VK_EXT_DISPLAY_SURFACE_COUNTER: {
    }
    break;
    case TExtensionType::VK_EXT_EXTENDED_DYNAMIC_STATE: {
    }
    break;
    case TExtensionType::VK_EXT_EXTERNAL_MEMORY_DMA_BUF: {
    }
    break;
    case TExtensionType::VK_EXT_EXTERNAL_MEMORY_HOST: {
    }
    break;
    case TExtensionType::VK_EXT_FILTER_CUBIC: {
    }
    break;
    case TExtensionType::VK_EXT_FRAGMENT_DENSITY_MAP: {
    }
    break;
    case TExtensionType::VK_EXT_FRAGMENT_DENSITY_MAP2: {
    }
    break;
    case TExtensionType::VK_EXT_FRAGMENT_SHADER_INTERLOCK: {
    }
    break;
    case TExtensionType::VK_EXT_FULL_SCREEN_EXCLUSIVE: {
    }
    break;
    case TExtensionType::VK_EXT_GLOBAL_PRIORITY: {
    }
    break;
    case TExtensionType::VK_EXT_HDR_METADATA: {
    }
    break;
    case TExtensionType::VK_EXT_HEADLESS_SURFACE: {
    }
    break;
    case TExtensionType::VK_EXT_HOST_QUERY_RESET: {
    }
    break;
    case TExtensionType::VK_EXT_IMAGE_DRM_FORMAT_MODIFIER: {
    }
    break;
    case TExtensionType::VK_EXT_IMAGE_ROBUSTNESS: {
    }
    break;
    case TExtensionType::VK_EXT_INDEX_TYPE_UINT8: {
    }
    break;
    case TExtensionType::VK_EXT_INLINE_UNIFORM_BLOCK: {
    }
    break;
    case TExtensionType::VK_EXT_LINE_RASTERIZATION: {
    }
    break;
    case TExtensionType::VK_EXT_MEMORY_BUDGET: {
    }
    break;
    case TExtensionType::VK_EXT_MEMORY_PRIORITY: {
    }
    break;
    case TExtensionType::VK_EXT_METAL_SURFACE: {
    }
    break;
    case TExtensionType::VK_EXT_PCI_BUS_INFO: {
    }
    break;
    case TExtensionType::VK_EXT_PIPELINE_CREATION_CACHE_CONTROL: {
    }
    break;
    case TExtensionType::VK_EXT_PIPELINE_CREATION_FEEDBACK: {
    }
    break;
    case TExtensionType::VK_EXT_POST_DEPTH_COVERAGE: {
    }
    break;
    case TExtensionType::VK_EXT_PRIVATE_DATA: {
    }
    break;
    case TExtensionType::VK_EXT_QUEUE_FAMILY_FOREIGN: {
    }
    break;
    case TExtensionType::VK_EXT_ROBUSTNESS2: {
    }
    break;
    case TExtensionType::VK_EXT_SAMPLE_LOCATIONS: {
    }
    break;
    case TExtensionType::VK_EXT_SAMPLER_FILTER_MINMAX: {
    }
    break;
    case TExtensionType::VK_EXT_SCALAR_BLOCK_LAYOUT: {
    }
    break;
    case TExtensionType::VK_EXT_SEPARATE_STENCIL_USAGE: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_ATOMIC_FLOAT: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_IMAGE_ATOMIC_INT64: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_STENCIL_EXPORT: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_SUBGROUP_BALLOT: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_SUBGROUP_VOTE: {
    }
    break;
    case TExtensionType::VK_EXT_SHADER_VIEWPORT_INDEX_LAYER: {
    }
    break;
    case TExtensionType::VK_EXT_SUBGROUP_SIZE_CONTROL: {
    }
    break;
    case TExtensionType::VK_EXT_SWAPCHAIN_COLORSPACE: {
    }
    break;
    case TExtensionType::VK_EXT_TEXEL_BUFFER_ALIGNMENT: {
    }
    break;
    case TExtensionType::VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR: {
    }
    break;
    case TExtensionType::VK_EXT_TOOLING_INFO: {
    }
    break;
    case TExtensionType::VK_EXT_TRANSFORM_FEEDBACK: {
    }
    break;
    case TExtensionType::VK_EXT_VALIDATION_CACHE: {
    }
    break;
    case TExtensionType::VK_EXT_VALIDATION_FEATURES: {
    }
    break;
    case TExtensionType::VK_EXT_VALIDATION_FLAGS: {
    }
    break;
    case TExtensionType::VK_EXT_VERTEX_ATTRIBUTE_DIVISOR: {
    }
    break;
    case TExtensionType::VK_EXT_YCBCR_IMAGE_ARRAYS: {
    }
    break;
    case TExtensionType::VK_AMD_BUFFER_MARKER: {
    }
    break;
    case TExtensionType::VK_AMD_DEVICE_COHERENT_MEMORY: {
    }
    break;
    case TExtensionType::VK_AMD_DISPLAY_NATIVE_HDR: {
    }
    break;
    case TExtensionType::VK_AMD_DRAW_INDIRECT_COUNT: {
    }
    break;
    case TExtensionType::VK_AMD_GCN_SHADER: {
    }
    break;
    case TExtensionType::VK_AMD_GPU_SHADER_HALF_FLOAT: {
    }
    break;
    case TExtensionType::VK_AMD_GPU_SHADER_INT16: {
    }
    break;
    case TExtensionType::VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR: {
    }
    break;
    case TExtensionType::VK_AMD_MIXED_ATTACHMENT_SAMPLES: {
    }
    break;
    case TExtensionType::VK_AMD_NEGATIVE_VIEWPORT_HEIGHT: {
    }
    break;
    case TExtensionType::VK_AMD_PIPELINE_COMPILER_CONTROL: {
    }
    break;
    case TExtensionType::VK_AMD_RASTERIZATION_ORDER: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_BALLOT: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_CORE_PROPERTIES: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_CORE_PROPERTIES2: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_EXPLICIT_VERTEX_PARAMETER: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_FRAGMENT_MASK: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_IMAGE_LOAD_STORE_LOD: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_INFO: {
    }
    break;
    case TExtensionType::VK_AMD_SHADER_TRINARY_MINMAX: {
    }
    break;
    case TExtensionType::VK_AMD_TEXTURE_GATHER_BIAS_LOD: {
    }
    break;
    case TExtensionType::VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER: {
    }
    break;
    case TExtensionType::VK_FUCHSIA_IMAGEPIPE_SURFACE: {
    }
    break;
    case TExtensionType::VK_GGP_FRAME_TOKEN: {
    }
    break;
    case TExtensionType::VK_GGP_STREAM_DESCRIPTOR_SURFACE: {
    }
    break;
    case TExtensionType::VK_GOOGLE_DECORATE_STRING: {
    }
    break;
    case TExtensionType::VK_GOOGLE_DISPLAY_TIMING: {
    }
    break;
    case TExtensionType::VK_GOOGLE_HLSL_FUNCTIONALITY1: {
    }
    break;
    case TExtensionType::VK_GOOGLE_USER_TYPE: {
    }
    break;
    case TExtensionType::VK_IMG_FILTER_CUBIC: {
    }
    break;
    case TExtensionType::VK_IMG_FORMAT_PVRTC: {
    }
    break;
    case TExtensionType::VK_INTEL_PERFORMANCE_QUERY: {
    }
    break;
    case TExtensionType::VK_INTEL_SHADER_INTEGER_FUNCTIONS2: {
    }
    break;
    case TExtensionType::VK_MVK_IOS_SURFACE: {
    }
    break;
    case TExtensionType::VK_MVK_MACOS_SURFACE: {
    }
    break;
    case TExtensionType::VK_NN_VI_SURFACE: {
    }
    break;
    case TExtensionType::VK_NV_CLIP_SPACE_W_SCALING: {
    }
    break;
    case TExtensionType::VK_NV_COMPUTE_SHADER_DERIVATIVES: {
    }
    break;
    case TExtensionType::VK_NV_COOPERATIVE_MATRIX: {
    }
    break;
    case TExtensionType::VK_NV_CORNER_SAMPLED_IMAGE: {
    }
    break;
    case TExtensionType::VK_NV_COVERAGE_REDUCTION_MODE: {
    }
    break;
    case TExtensionType::VK_NV_DEDICATED_ALLOCATION: {
    }
    break;
    case TExtensionType::VK_NV_DEDICATED_ALLOCATION_IMAGE_ALIASING: {
    }
    break;
    case TExtensionType::VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS: {
    }
    break;
    case TExtensionType::VK_NV_DEVICE_DIAGNOSTICS_CONFIG: {
    }
    break;
    case TExtensionType::VK_NV_DEVICE_GENERATED_COMMANDS: {
    }
    break;
    case TExtensionType::VK_NV_EXTERNAL_MEMORY: {
    }
    break;
    case TExtensionType::VK_NV_EXTERNAL_MEMORY_CAPABILITIES: {
    }
    break;
    case TExtensionType::VK_NV_EXTERNAL_MEMORY_WIN32: {
    }
    break;
    case TExtensionType::VK_NV_FILL_RECTANGLE: {
    }
    break;
    case TExtensionType::VK_NV_FRAGMENT_COVERAGE_TO_COLOR: {
    }
    break;
    case TExtensionType::VK_NV_FRAGMENT_SHADER_BARYCENTRIC: {
    }
    break;
    case TExtensionType::VK_NV_FRAMEBUFFER_MIXED_SAMPLES: {
    }
    break;
    case TExtensionType::VK_NV_GEOMETRY_SHADER_PASSTHROUGH: {
    }
    break;
    case TExtensionType::VK_NV_GLSL_SHADER: {
    }
    break;
    case TExtensionType::VK_NV_MESH_SHADER: {
    }
    break;
    case TExtensionType::VK_NV_RAY_TRACING: {
    }
    break;
    case TExtensionType::VK_NV_REPRESENTATIVE_FRAGMENT_TEST: {
    }
    break;
    case TExtensionType::VK_NV_SAMPLE_MASK_OVERRIDE_COVERAGE: {
    }
    break;
    case TExtensionType::VK_NV_SCISSOR_EXCLUSIVE: {
    }
    break;
    case TExtensionType::VK_NV_SHADER_IMAGE_FOOTPRINT: {
    }
    break;
    case TExtensionType::VK_NV_SHADER_SM_BUILTINS: {
    }
    break;
    case TExtensionType::VK_NV_SHADER_SUBGROUP_PARTITIONED: {
    }
    break;
    case TExtensionType::VK_NV_SHADING_RATE_IMAGE: {
    }
    break;
    case TExtensionType::VK_NV_VIEWPORT_ARRAY2: {
    }
    break;
    case TExtensionType::VK_NV_VIEWPORT_SWIZZLE: {
    }
    break;
    case TExtensionType::VK_NV_WIN32_KEYED_MUTEX: {
    }
    break;
    case TExtensionType::VK_NVX_IMAGE_VIEW_HANDLE: {
    }
    break;
    case TExtensionType::VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES: {
    }
    break;
    case TExtensionType::VK_QCOM_RENDER_PASS_SHADER_RESOLVE: {
    }
    break;
    case TExtensionType::VK_QCOM_RENDER_PASS_STORE_OPS: {
    }
    break;
    case TExtensionType::VK_QCOM_RENDER_PASS_TRANSFORM: {
    }
    break;
    case TExtensionType::VK_KHR_DYNAMIC_RENDERING: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_DEPTH_STENCIL_RESOLVE);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_DYNAMIC_RENDERING))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_DYNAMIC_RENDERING));
        }
    }
    break;
    case TExtensionType::VK_EXT_MESH_SHADER: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_SPIRV_1_4);

        if (!this->IsEnabledExtension(TExtensionType::VK_EXT_MESH_SHADER))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_EXT_MESH_SHADER));
        }
    }
    break;
    case TExtensionType::VK_KHR_ACCELERATION_STRUCTURE: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_EXT_DESCRIPTOR_INDEXING);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_BUFFER_DEVICE_ADDRESS);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_DEFERRED_HOST_OPERATIONS);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_ACCELERATION_STRUCTURE))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_ACCELERATION_STRUCTURE));
        }
    }
    break;
    case TExtensionType::VK_KHR_RAY_TRACING_PIPELINE: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_SPIRV_1_4);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_ACCELERATION_STRUCTURE);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_RAY_TRACING_PIPELINE))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_RAY_TRACING_PIPELINE));
        }
    }
    case TExtensionType::VK_KHR_RAY_QUERY: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_SPIRV_1_4);
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_ACCELERATION_STRUCTURE);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_RAY_QUERY))
        {
            this->enabledExtensions.push_back(this->physicalDevice->GetExtensionByType(TExtensionType::VK_KHR_RAY_QUERY));
        }
    }
    break;
    }
}

void Turbo::Core::TDevice::InternalCreate()
{
    std::vector<Turbo::Core::TQueueFamilyInfo> device_queue_family_infos = this->GetDeviceQueueFamilyInfos();
    uint32_t device_queue_family_info_count = device_queue_family_infos.size();

    if (device_queue_family_info_count <= 0)
    {
        return; // Vulkan Specification:device queue create count must greater than zero.
    }

    std::vector<VkDeviceQueueCreateInfo> vk_device_queue_create_infos;
    vk_device_queue_create_infos.resize(device_queue_family_info_count);
    this->deviceQueuePriorities.resize(device_queue_family_info_count);
    for (uint32_t device_queue_index = 0; device_queue_index < device_queue_family_info_count; device_queue_index++)
    {
        TQueueFamilyInfo &queue_family_info = device_queue_family_infos[device_queue_index];
        uint32_t queue_family_index = queue_family_info.GetIndex();
        uint32_t queue_family_count = this->GetDeviceQueueCountByQueueFamily(queue_family_info);
        this->deviceQueuePriorities[queue_family_index].resize(queue_family_count, 0.0f);

        vk_device_queue_create_infos[device_queue_index].sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        vk_device_queue_create_infos[device_queue_index].pNext = nullptr;
        vk_device_queue_create_infos[device_queue_index].flags = 0;
        vk_device_queue_create_infos[device_queue_index].queueFamilyIndex = queue_family_index;
        vk_device_queue_create_infos[device_queue_index].queueCount = queue_family_count;
        vk_device_queue_create_infos[device_queue_index].pQueuePriorities = this->deviceQueuePriorities[queue_family_index].data();
    }

    size_t enable_layer_count = this->enabledLayers.size();
    std::vector<const char *> enable_layer_names(enable_layer_count);
    for (uint32_t enable_layer_index = 0; enable_layer_index < enable_layer_count; enable_layer_index++)
    {
        enable_layer_names[enable_layer_index] = this->enabledLayers[enable_layer_index].GetName().c_str();
    }

    for (uint32_t enabled_extensions_index = 0; enabled_extensions_index < this->enabledExtensions.size(); enabled_extensions_index++)
    {
        this->InspectExtensionAndVersionDependencies(this->enabledExtensions[enabled_extensions_index].GetExtensionType());
    }

    size_t enable_extension_count = this->enabledExtensions.size();
    std::vector<const char *> enable_extension_names(enable_extension_count);
    for (uint32_t enable_extension_index = 0; enable_extension_index < enable_extension_count; enable_extension_index++)
    {
        enable_extension_names[enable_extension_index] = this->enabledExtensions[enable_extension_index].GetName().c_str();
    }

    void *vk_device_create_info_p_next = nullptr;

    // feature
    VkPhysicalDeviceFeatures vk_physical_device_features = {};
    vk_physical_device_features.geometryShader = this->enabledFeatures.geometryShader ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.tessellationShader = this->enabledFeatures.tessellationShader ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.sampleRateShading = this->enabledFeatures.sampleRateShading ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.depthClamp = this->enabledFeatures.depthClamp ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.depthBiasClamp = this->enabledFeatures.depthBiasClamp ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.wideLines = this->enabledFeatures.wideLines ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.fillModeNonSolid = this->enabledFeatures.fillModeNonSolid ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.samplerAnisotropy = this->enabledFeatures.samplerAnisotropy ? VK_TRUE : VK_FALSE;
    vk_physical_device_features.logicOp = this->enabledFeatures.logicOp ? VK_TRUE : VK_FALSE;

    // We need to compare current Vulkan Instance Version
    Turbo::Core::TVersion vulkan_version = this->GetPhysicalDevice()->GetInstance()->GetVulkanVersion();

    VkPhysicalDeviceVulkan11Features vk_physical_device_vulkan11_features = {}; // Vulkan1.1
    vk_physical_device_vulkan11_features.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    vk_physical_device_vulkan11_features.pNext = nullptr;

    VkPhysicalDeviceVulkan12Features vk_physical_device_vulkan12_features = {}; // Vulkan1.2
    vk_physical_device_vulkan12_features.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    vk_physical_device_vulkan12_features.pNext = nullptr;
    vk_physical_device_vulkan12_features.timelineSemaphore = this->enabledFeatures.timelineSemaphore ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceVulkan13Features vk_physical_device_vulkan13_features = {}; // Vulkan1.3
    vk_physical_device_vulkan13_features.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    vk_physical_device_vulkan13_features.pNext = nullptr;
    vk_physical_device_vulkan13_features.dynamicRendering = this->enabledFeatures.dynamicRendering ? VK_TRUE : VK_FALSE;

    // NOTE: for Extensions feature...
    VkPhysicalDeviceShaderClockFeaturesKHR vk_physical_device_shader_clock_features_khr = {};
    vk_physical_device_shader_clock_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR;
    vk_physical_device_shader_clock_features_khr.pNext = nullptr;
    vk_physical_device_shader_clock_features_khr.shaderSubgroupClock = this->enabledFeatures.shaderSubgroupClock ? VK_TRUE : VK_FALSE;
    vk_physical_device_shader_clock_features_khr.shaderDeviceClock = this->enabledFeatures.shaderDeviceClock ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceRayQueryFeaturesKHR vk_physical_device_ray_query_features_khr = {};
    vk_physical_device_ray_query_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
    vk_physical_device_ray_query_features_khr.pNext = &vk_physical_device_shader_clock_features_khr;
    vk_physical_device_ray_query_features_khr.rayQuery = this->enabledFeatures.rayQuery ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceRayTracingPipelineFeaturesKHR vk_physical_device_ray_tracing_pipeline_features_khr = {};
    vk_physical_device_ray_tracing_pipeline_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    vk_physical_device_ray_tracing_pipeline_features_khr.pNext = &vk_physical_device_ray_query_features_khr;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipeline = this->enabledFeatures.rayTracingPipeline ? VK_TRUE : VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipelineShaderGroupHandleCaptureReplay = this->enabledFeatures.rayTracingPipelineShaderGroupHandleCaptureReplay ? VK_TRUE : VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = this->enabledFeatures.rayTracingPipelineShaderGroupHandleCaptureReplayMixed ? VK_TRUE : VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTracingPipelineTraceRaysIndirect = this->enabledFeatures.rayTracingPipelineTraceRaysIndirect ? VK_TRUE : VK_FALSE;
    vk_physical_device_ray_tracing_pipeline_features_khr.rayTraversalPrimitiveCulling = this->enabledFeatures.rayTraversalPrimitiveCulling ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceBufferDeviceAddressFeaturesKHR vk_physical_device_buffer_device_address_features_khr = {};
    vk_physical_device_buffer_device_address_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR;
    vk_physical_device_buffer_device_address_features_khr.pNext = &vk_physical_device_ray_tracing_pipeline_features_khr;
    vk_physical_device_buffer_device_address_features_khr.bufferDeviceAddress = this->enabledFeatures.bufferDeviceAddress ? VK_TRUE : VK_FALSE;
    vk_physical_device_buffer_device_address_features_khr.bufferDeviceAddressCaptureReplay = this->enabledFeatures.bufferDeviceAddressCaptureReplay ? VK_TRUE : VK_FALSE;
    vk_physical_device_buffer_device_address_features_khr.bufferDeviceAddressMultiDevice = this->enabledFeatures.bufferDeviceAddressMultiDevice ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceAccelerationStructureFeaturesKHR vk_physical_device_acceleration_structure_features_khr = {};
    vk_physical_device_acceleration_structure_features_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    vk_physical_device_acceleration_structure_features_khr.pNext = &vk_physical_device_buffer_device_address_features_khr;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructure = this->enabledFeatures.accelerationStructure ? VK_TRUE : VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructureCaptureReplay = this->enabledFeatures.accelerationStructureCaptureReplay ? VK_TRUE : VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructureIndirectBuild = this->enabledFeatures.accelerationStructureIndirectBuild ? VK_TRUE : VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.accelerationStructureHostCommands = this->enabledFeatures.accelerationStructureHostCommands ? VK_TRUE : VK_FALSE;
    vk_physical_device_acceleration_structure_features_khr.descriptorBindingAccelerationStructureUpdateAfterBind = this->enabledFeatures.descriptorBindingAccelerationStructureUpdateAfterBind ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceMeshShaderFeaturesEXT vk_physical_device_mesh_shader_features_ext = {};
    vk_physical_device_mesh_shader_features_ext.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
    vk_physical_device_mesh_shader_features_ext.pNext = &vk_physical_device_acceleration_structure_features_khr;
    vk_physical_device_mesh_shader_features_ext.taskShader = this->enabledFeatures.taskShaderEXT ? VK_TRUE : VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.meshShader = this->enabledFeatures.meshShaderEXT ? VK_TRUE : VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.multiviewMeshShader = this->enabledFeatures.multiviewMeshShaderEXT ? VK_TRUE : VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.primitiveFragmentShadingRateMeshShader = this->enabledFeatures.primitiveFragmentShadingRateMeshShaderEXT ? VK_TRUE : VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.meshShaderQueries = this->enabledFeatures.meshShaderQueriesEXT ? VK_TRUE : VK_FALSE;

    VkPhysicalDeviceMeshShaderFeaturesNV vk_physical_device_mesh_shader_features_nv = {};
    vk_physical_device_mesh_shader_features_nv.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
    vk_physical_device_mesh_shader_features_nv.pNext = &vk_physical_device_mesh_shader_features_ext;
    vk_physical_device_mesh_shader_features_nv.taskShader = this->enabledFeatures.taskShaderNV ? VK_TRUE : VK_FALSE;
    vk_physical_device_mesh_shader_features_nv.meshShader = this->enabledFeatures.meshShaderNV ? VK_TRUE : VK_FALSE;

    vk_device_create_info_p_next = &vk_physical_device_mesh_shader_features_nv;

    if (vulkan_version >= Turbo::Core::TVersion(1, 1, 0, 0))
    {
        vk_physical_device_vulkan11_features.pNext = &vk_physical_device_mesh_shader_features_nv;
        vk_device_create_info_p_next = &vk_physical_device_vulkan11_features;
    }
    else
    {
        // TODO: Not support Vulkan 1.1 feature
    }

    if (vulkan_version >= Turbo::Core::TVersion(1, 2, 0, 0))
    {
        vk_physical_device_vulkan12_features.pNext = &vk_physical_device_vulkan11_features;
        vk_device_create_info_p_next = &vk_physical_device_vulkan12_features;
    }
    else
    {
        // TODO: Not support Vulkan 1.2 feature
        this->enabledFeatures.timelineSemaphore = false;
    }

    if (vulkan_version >= Turbo::Core::TVersion(1, 3, 0, 0))
    {
        vk_physical_device_vulkan13_features.pNext = &vk_physical_device_vulkan12_features;
        vk_device_create_info_p_next = &vk_physical_device_vulkan13_features;
    }
    else
    {
        // TODO: Not support Vulkan 1.3 feature
        this->enabledFeatures.dynamicRendering = false;
    }

    VkDeviceCreateInfo vk_device_create_info = {};
    vk_device_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    vk_device_create_info.pNext = vk_device_create_info_p_next;
    vk_device_create_info.flags = 0;
    vk_device_create_info.queueCreateInfoCount = vk_device_queue_create_infos.size();
    vk_device_create_info.pQueueCreateInfos = vk_device_queue_create_infos.data();
    vk_device_create_info.enabledLayerCount = enable_layer_count;
    vk_device_create_info.ppEnabledLayerNames = enable_layer_names.data();
    vk_device_create_info.enabledExtensionCount = enable_extension_count;
    vk_device_create_info.ppEnabledExtensionNames = enable_extension_names.data();
    vk_device_create_info.pEnabledFeatures = &vk_physical_device_features;

    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = this->GetPhysicalDevice()->GetPhysicalDeviceDriver()->vkCreateDevice(this->physicalDevice->GetVkPhysicalDevice(), &vk_device_create_info, allocator, &this->vkDevice);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TDevice::InternalCreate::vkCreateDevice");
    }

    // TODO: use TVulkanLoader load all device-specific function(return device-specific function table)
    this->deviceDriver = new TDeviceDriver();
    *this->deviceDriver = TVulkanLoader::Instance()->LoadDeviceDriver(this); // TODO:load dynamic rendering function

    if (this->vmaAllocator.Valid())
    {
        this->vmaAllocator->InternalCreate();
    }

    for (TDeviceQueue *device_queue_item : this->deviceQueues)
    {
        device_queue_item->InternalCreate();
    }
}

void Turbo::Core::TDevice::InternalDestroy()
{
    for (TDeviceQueue *device_queue_item : this->deviceQueues)
    {
        device_queue_item->InternalDestroy();
    }

    if (this->vmaAllocator.Valid())
    {
        this->vmaAllocator->InternalDestroy();
    }

    this->deviceQueuePriorities.clear();

    // this->physicalDevice->ResetQueueCountMap();

    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    if (this->vkDevice != VK_NULL_HANDLE)
    {
        this->deviceDriver->vkDestroyDevice(this->vkDevice, allocator);
        this->vkDevice = VK_NULL_HANDLE;

        delete this->deviceDriver;
    }
}

Turbo::Core::TDevice::TDevice(const TRefPtr<TPhysicalDevice> &physicalDevice, std::vector<TLayerInfo> *enabledLayers, std::vector<TExtensionInfo> *enabledExtensions, TPhysicalDeviceFeatures *enabledFeatures) : Turbo::Core::TVulkanHandle()
{
    /*
    Vulkan Spec::1.2.156
    [VkDeviceQueueCreateInfo]If the protected memory feature is not enabled, the VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT
    bit of flags must not be set
    */

    if (physicalDevice.Valid())
    {
        this->physicalDevice = physicalDevice;

        if (enabledLayers != nullptr)
        {
            this->enabledLayers = *enabledLayers;
        }

        if (enabledExtensions != nullptr)
        {
            this->enabledExtensions = *enabledExtensions;
            // TODO: we need to check and add extension dependencies(missing extensions required)
        }

        if (enabledFeatures != nullptr)
        {
            this->enabledFeatures = *enabledFeatures;
        }

        this->InternalCreate();

        this->physicalDevice->AddChildHandle(this);

        // Create TDeviceQueues
        std::vector<TQueueFamilyInfo> queue_family_infos = this->GetPhysicalDevice()->GetQueueFamilys();
        for (TQueueFamilyInfo queue_family_info_item : queue_family_infos)
        {
            new TDeviceQueue(this, queue_family_info_item, 0);
        }

        this->vmaAllocator = new TVmaAllocator(this);
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDevice::TDevice");
    }
}

Turbo::Core::TDevice::~TDevice()
{
    if (this->vkDevice != VK_NULL_HANDLE)
    {
        this->deviceDriver->vkDeviceWaitIdle(this->vkDevice);

        // delete this->vmaAllocator;
        this->vmaAllocator = nullptr;

        this->physicalDevice->RemoveChildHandle(this);

        for (TRefPtr<TDeviceQueue> &device_queue_item : this->deviceQueues)
        {
            // delete device_queue_item;
            device_queue_item = nullptr;
        }

        this->deviceQueues.clear();

        this->InternalDestroy();
    }
}

VkDevice Turbo::Core::TDevice::GetVkDevice()
{
    return this->vkDevice;
}

const Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> &Turbo::Core::TDevice::GetPhysicalDevice()
{
    return this->physicalDevice;
}

size_t Turbo::Core::TDevice::GetEnabledLayersCount() const
{
    return this->enabledLayers.size();
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TDevice::GetEnabledLayers() const
{
    return this->enabledLayers;
}

size_t Turbo::Core::TDevice::GetEnabledExtensionCount() const
{
    return this->enabledExtensions.size();
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TDevice::GetEnabledExtensions()
{
    return this->enabledExtensions;
}

bool Turbo::Core::TDevice::IsEnabledExtension(std::string extensionName) const
{
    if (!extensionName.empty())
    {
        size_t enabled_extension_count = this->enabledExtensions.size();
        for (size_t extension_index = 0; extension_index < enabled_extension_count; extension_index++)
        {
            if (this->enabledExtensions[extension_index].GetName() == extensionName)
            {
                return true;
            }
        }
    }

    return false;
}

bool Turbo::Core::TDevice::IsEnabledExtension(TExtensionType extensionType) const
{
    return this->IsEnabledExtension(TExtensionInfo::GetExtensionNameByExtensionType(extensionType));
}

Turbo::Core::TPhysicalDeviceFeatures Turbo::Core::TDevice::GetEnableDeviceFeatures() const
{
    return this->enabledFeatures;
}

uint32_t Turbo::Core::TDevice::GetDeviceQueueCountByQueueFamily(TQueueFamilyInfo queueFamily)
{
    uint32_t count = 0;

    for (TRefPtr<TDeviceQueue> &item : this->deviceQueues)
    {
        if (item->GetQueueFamily().GetIndex() == queueFamily.GetIndex())
        {
            count = count + 1;
        }
    }

    return count;
}

std::vector<Turbo::Core::TQueueFamilyInfo> Turbo::Core::TDevice::GetDeviceQueueFamilyInfos()
{
    std::vector<Turbo::Core::TQueueFamilyInfo> result;
    for (TDeviceQueue *device_queue_item : this->deviceQueues)
    {
        bool is_found = false;
        for (TQueueFamilyInfo &item : result)
        {
            if (item.GetIndex() == device_queue_item->GetQueueFamily().GetIndex())
            {
                is_found = true;
                break;
            }
        }

        if (!is_found)
        {
            result.push_back(device_queue_item->GetQueueFamily());
        }
    }

    return result;
}

const Turbo::Core::TRefPtr<Turbo::Core::TVmaAllocator> &Turbo::Core::TDevice::GetVmaAllocator()
{
    return this->vmaAllocator;
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TDevice::GetBestGraphicsQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportGraphics() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = queue_family_info.score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TDevice::GetBestComputeQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportCompute() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = queue_family_info.score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TDevice::GetBestTransferQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportTransfer() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = queue_family_info.score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TDevice::GetBestSparseBindingQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportSparse() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = queue_family_info.score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> Turbo::Core::TDevice::GetBestProtectedQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportProtected() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = queue_family_info.score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

void Turbo::Core::TDevice::WaitIdle()
{
    VkResult result = this->deviceDriver->vkDeviceWaitIdle(this->vkDevice);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::FAIL, "Turbo::Core::TDevice::WaitIdle");
    }
}

const Turbo::Core::TDeviceDriver *Turbo::Core::TDevice::GetDeviceDriver()
{
    return this->deviceDriver;
}

std::string Turbo::Core::TDevice::ToString() const
{
    return std::string();
}

bool Turbo::Core::TDevice::Valid() const
{
    if (this->vkDevice != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}