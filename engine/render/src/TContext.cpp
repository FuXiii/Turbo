#include "TContext.h"
#include "TImage.h"
#include <core/include/TCore.h>
#include <core/include/TException.h>
#include <core/include/TImage.h>
#include <core/include/TInstance.h>
#include <core/include/TPhysicalDevice.h>
#include <core/include/TVersion.h>
#include <core/include/TVulkanLoader.h>

Turbo::Render::TContext::TContext()
{
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
    std::cout << "Support Vulkan Version:" << support_vulkan_version.ToString() << std::endl;
    std::cout << "Turbo Vulkan Version:" << this->instance->GetVulkanVersion().ToString() << std::endl;

    this->physicalDevice = this->instance->GetBestPhysicalDevice();

    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = this->physicalDevice->GetSupportExtensions();

    Turbo::Core::TExtensionInfo vk_khr_swapchain_extension;
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            vk_khr_swapchain_extension = extension;
            break;
        }
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    if (vk_khr_swapchain_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_device_extensions.push_back(vk_khr_swapchain_extension);
    }

    VkPhysicalDeviceFeatures vk_physical_device_features = {VK_FALSE};
    vk_physical_device_features.sampleRateShading = VK_TRUE;
    this->device = new Turbo::Core::TDevice(this->physicalDevice, nullptr, &enable_device_extensions, &vk_physical_device_features);
    this->graphicsQueue = this->device->GetBestGraphicsQueue();
}

Turbo::Render::TContext::~TContext()
{
    this->graphicsQueue = nullptr;
    delete this->device;
    this->physicalDevice = nullptr;
    delete this->instance;
}

Turbo::Core::TImage *Turbo::Render::TContext::CreateImage(const TImage::Descriptor &descriptor)
{
    TImageCreateFlags image_create_flags = descriptor.flags;
    TFormat format = descriptor.format;
    uint32_t width = descriptor.width;
    uint32_t height = descriptor.height;
    uint32_t depth = descriptor.depth;
    uint32_t layers = descriptor.layers;
    uint32_t mip_levels = descriptor.mipLevels;
    TImageUsages usages = descriptor.usages;
    TDomain domain = descriptor.domain;

    if (width == 0 && height == 0 && depth == 0)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Core::TImage *Turbo::Render::TContext::CreateImage(const TImage::Descriptor &descriptor)", "Try to create image, but all three dimensions are zero");
    }

    Turbo::Core::TDevice *device = this->device;

    VkImageCreateFlags vk_image_create_flags = 0;
    if ((image_create_flags & Turbo::Render::TImageCreateFlagBits::CUBE) == Turbo::Render::TImageCreateFlagBits::CUBE)
    {
        vk_image_create_flags |= VkImageCreateFlagBits::VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }

    Turbo::Core::TImageType type = Turbo::Core::TImageType::DIMENSION_1D;
    if (width >= 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_1D;
    }
    if (height >= 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_2D;
    }
    if (depth >= 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_3D;
    }

    Turbo::Core::TFormatType format_type = Turbo::Core::TFormatType::UNDEFINED;
    switch (format)
    {
    case TFormat::B8G8R8A8_SRGB: {
        format_type = Turbo::Core::TFormatType::B8G8R8A8_SRGB;
    }
    break;
    case TFormat::D32_SFLOAT: {
        format_type = Turbo::Core::TFormatType::D32_SFLOAT;
    }
    break;
    case TFormat::R8G8B8A8_UNORM: {
        format_type = Turbo::Core::TFormatType::R8G8B8A8_UNORM;
    }
    break;
    }

    Turbo::Core::TSampleCountBits sample_count_bits = Turbo::Core::TSampleCountBits::SAMPLE_1_BIT;

    Turbo::Core::TImageTiling image_tiling = Turbo::Core::TImageTiling::OPTIMAL;

    Turbo::Core::TImageUsages image_usages = usages;

    Turbo::Core::TMemoryFlags memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;

    if ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
    {
        image_tiling = Turbo::Core::TImageTiling::LINEAR;
    }
    else
    {
        image_tiling = Turbo::Core::TImageTiling::OPTIMAL;
    }

    // GPU only
    if (((domain & Turbo::Render::TDomainBits::CPU) != Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;
    }
    // Staging copy for upload
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE;
    }
    // Readback
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_RANDOM;
    }
    // Advanced data uploading(Both CPU and GPU domain can access)
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE | Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD;
    }

    Turbo::Core::TImageLayout layout = Turbo::Core::TImageLayout::UNDEFINED;

    return new Turbo::Core::TImage(device, vk_image_create_flags, type, format_type, width, height, depth, mip_levels, layers, sample_count_bits, image_tiling, image_usages, memory_flags, layout);
}

void Turbo::Render::TContext::DestroyImage(Turbo::Core::TImage *image)
{
    if (image != nullptr && image->GetVkImage() != VK_NULL_HANDLE)
    {
        delete image;
    }
}
