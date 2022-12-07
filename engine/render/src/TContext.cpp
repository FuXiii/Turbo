#include "TContext.h"
#include <core/include/TCore.h>
#include <core/include/TException.h>
#include <core/include/TInstance.h>
#include <core/include/TPhysicalDevice.h>
#include <core/include/TVersion.h>
#include <core/include/TVulkanLoader.h>

Turbo::Render::TContext::TContext()
{
    // TODO:
    // check Vulkan support
    if (!Turbo::Core::TInstance::IsSupportVulkan())
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Render::TContext::TContext()", "Unsupport Vulkan, please try to update your GPU drive or install Vulkan Runtime");
    }

    Turbo::Core::TVersion support_vulkan_version = Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion();

    // temp instance for collect information
    Turbo::Core::TInstance *temp_instance = new Turbo::Core::TInstance();
    Turbo::Core::TVersion temp_vulkan_version = temp_instance->GetVulkanVersion();
    Turbo::Core::TPhysicalDevice *temp_best_physical_device = temp_instance->GetBestPhysicalDevice();

    if (temp_best_physical_device->GetVkPhysicalDevice() != VK_NULL_HANDLE)
    {
        if (temp_best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion() >= support_vulkan_version.GetValidVulkanVersion())
        {
            Turbo::Core::TVersion instance_support_vulkan_version = support_vulkan_version.GetValidVulkanVersion();
            delete temp_instance;
            temp_instance = new Turbo::Core::TInstance(nullptr, nullptr, &instance_support_vulkan_version);
        }
        else
        {
            Turbo::Core::TVersion device_support_vulkan_version = temp_best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion();
            delete temp_instance;
            temp_instance = new Turbo::Core::TInstance(nullptr, nullptr, &device_support_vulkan_version);
        }
    }

    Turbo::Core::TVersion instance_support_version = temp_instance->GetVulkanVersion();
    std::vector<Turbo::Core::TLayerInfo> instance_support_layers = temp_instance->GetSupportLayers();
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions = temp_instance->GetSupportExtensions();

    delete temp_instance;

    Turbo::Core::TLayerInfo khronos_validation_layer;
    for (Turbo::Core::TLayerInfo &layer : instance_support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation_layer = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_instance_layer;
    if (khronos_validation_layer.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_instance_layer.push_back(khronos_validation_layer);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;

    Turbo::Core::TExtensionInfo vk_khr_surface_extension;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            vk_khr_surface_extension = extension;
            break;
        }
    }

    if (vk_khr_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_surface_extension);
    }

#if defined(TURBO_PLATFORM_WINDOWS)
    Turbo::Core::TExtensionInfo vk_khr_win32_surface_extension;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            vk_khr_win32_surface_extension = extension;
            break;
        }
    }

    if (vk_khr_win32_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_win32_surface_extension);
    }
#elif defined(TURBO_PLATFORM_LINUX)
    Turbo::Core::TExtensionInfo vk_khr_wayland_surface_extension;
    Turbo::Core::TExtensionInfo vk_khr_xcb_surface_extension;
    Turbo::Core::TExtensionInfo vk_khr_xlib_surface_extension;

    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (vk_khr_wayland_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            vk_khr_wayland_surface_extension = extension;
        }
        else if (vk_khr_xcb_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            vk_khr_xcb_surface_extension = extension;
        }
        else if (vk_khr_xlib_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            vk_khr_xlib_surface_extension = extension;
        }
    }

    if (vk_khr_wayland_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_wayland_surface_extension);
    }

    if (vk_khr_xcb_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_xcb_surface_extension);
    }

    if (vk_khr_xlib_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_xlib_surface_extension);
    }
#else
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Render::TContext::TContext()", "Turbo had not implemented this platform");
#endif

    // create instance
    this->instance = new Turbo::Core::TInstance(&enable_instance_layer, &enable_instance_extensions, &instance_support_version);
    std::cout << "Vulkan Version:" << this->instance->GetVulkanVersion().ToString() << std::endl;
}

Turbo::Render::TContext::~TContext()
{
    // TODO:
    // destroy device queue
    // destroy device
    // destroy physical device
    // destroy instance
    delete this->instance;
}
