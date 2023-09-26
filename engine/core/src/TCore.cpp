#include "TCore.h"

Turbo::Core::TResult Turbo::Core::VkResultToTResult(VkResult vkResult)
{
    TResult result = TResult::UNDEFINED;
    switch (vkResult)
    {
    case VkResult::VK_SUCCESS:
        result = TResult::SUCCESS;
        break;
    case VkResult::VK_NOT_READY:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_TIMEOUT:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_EVENT_SET:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_EVENT_RESET:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_INCOMPLETE:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_OUT_OF_HOST_MEMORY:
        result = TResult::OUT_OF_HOST_MEMORY;
        break;
    case VkResult::VK_ERROR_OUT_OF_DEVICE_MEMORY:
        result = TResult::OUT_OF_DEVICE_MEMORY;
        break;
    case VkResult::VK_ERROR_INITIALIZATION_FAILED:
        result = TResult::INITIALIZATION_FAILED;
        break;
    case VkResult::VK_ERROR_DEVICE_LOST:
        result = TResult::DEVICE_LOST;
        break;
    case VkResult::VK_ERROR_MEMORY_MAP_FAILED:
        result = TResult::MEMORY_MAP_FAILED; //=
        break;
    case VkResult::VK_ERROR_LAYER_NOT_PRESENT:
        result = TResult::LAYER_NOT_PRESENT;
        break;
    case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT:
        result = TResult::EXTENSION_NOT_PRESENT;
        break;
    case VkResult::VK_ERROR_FEATURE_NOT_PRESENT:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_INCOMPATIBLE_DRIVER:
        result = TResult::UNSUPPORTED;
        break;
    case VkResult::VK_ERROR_TOO_MANY_OBJECTS:
        result = TResult::TOO_MANY_OBJECTS;
        break;
    case VkResult::VK_ERROR_FORMAT_NOT_SUPPORTED:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_FRAGMENTED_POOL:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_UNKNOWN:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_OUT_OF_POOL_MEMORY:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_INVALID_EXTERNAL_HANDLE:
        result = TResult::INVALID_EXTERNAL_HANDLE;
        break;
    case VkResult::VK_ERROR_FRAGMENTATION:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_SURFACE_LOST_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_SUBOPTIMAL_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_OUT_OF_DATE_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_VALIDATION_FAILED_EXT:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_INVALID_SHADER_NV:
        result = TResult::UNDEFINED; //=
        break;
    // case VkResult::VK_ERROR_INCOMPATIBLE_VERSION_KHR:
    // 	result = TResult::UNDEFINED;//=
    // 	break;
    case VkResult::VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_NOT_PERMITTED_EXT:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_THREAD_IDLE_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_THREAD_DONE_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_OPERATION_DEFERRED_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_OPERATION_NOT_DEFERRED_KHR:
        result = TResult::UNDEFINED; //=
        break;
    case VkResult::VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT:
        result = TResult::UNDEFINED; //=
        break;
    default:
        result = TResult::UNDEFINED;
        break;
    }

    return result;
}

std::string Turbo::Core::TResultToString(Turbo::Core::TResult result)
{
    switch (result)
    {
    case Turbo::Core::TResult::UNIMPLEMENTED: {
        return "UNIMPLEMENTED";
    }
    break;
    case Turbo::Core::TResult::SHADER_LINK_FAILED: {
        return "SHADER_LINK_FAILED";
    }
    break;
    case Turbo::Core::TResult::SHADER_PARSE_FAILED: {
        return "SHADER_PARSE_FAILED";
    }
    break;
    case Turbo::Core::TResult::RESOURCE_STARVATION: {
        return "RESOURCE_STARVATION";
    }
    break;
    case Turbo::Core::TResult::MEMORY_MAP_FAILED: {
        return "MEMORY_MAP_FAILED";
    }
    break;
    case Turbo::Core::TResult::OUT_OF_RANGE: {
        return "OUT_OF_RANGE";
    }
    break;
    case Turbo::Core::TResult::INVALID_PARAMETER: {
        return "INVALID_PARAMETER";
    }
    break;
    case Turbo::Core::TResult::INVALID_EXTERNAL_HANDLE: {
        return "INVALID_EXTERNAL_HANDLE";
    }
    break;
    case Turbo::Core::TResult::DEVICE_LOST: {
        return "DEVICE_LOST";
    }
    break;
    case Turbo::Core::TResult::TOO_MANY_OBJECTS: {
        return "TOO_MANY_OBJECTS";
    }
    break;
    case Turbo::Core::TResult::EXTENSION_NOT_PRESENT: {
        return "EXTENSION_NOT_PRESENT";
    }
    break;
    case Turbo::Core::TResult::LAYER_NOT_PRESENT: {
        return "LAYER_NOT_PRESENT";
    }
    break;
    case Turbo::Core::TResult::INITIALIZATION_FAILED: {
        return "INITIALIZATION_FAILED";
    }
    break;
    case Turbo::Core::TResult::OUT_OF_DEVICE_MEMORY: {
        return "OUT_OF_DEVICE_MEMORY";
    }
    break;
    case Turbo::Core::TResult::OUT_OF_HOST_MEMORY: {
        return "OUT_OF_HOST_MEMORY";
    }
    break;
    case Turbo::Core::TResult::UNSUPPORTED: {
        return "UNSUPPORTED";
    }
    break;
    case Turbo::Core::TResult::FAIL: {
        return "FAIL";
    }
    break;
    case Turbo::Core::TResult::UNDEFINED: {
        return "UNDEFINED ";
    }
    break;
    case Turbo::Core::TResult::SUCCESS: {
        return "SUCCESS ";
    }
    break;
    case Turbo::Core::TResult::TIMEOUT: {
        return "TIMEOUT ";
    }
    break;
    case Turbo::Core::TResult::NOT_READY: {
        return "NOT_READY ";
    }
    break;
    case Turbo::Core::TResult::MISMATCH: {
        return "MISMATCH ";
    }
    break;
    default:
        return "Undefined";
        break;
    }

    return "Undefined";
}
