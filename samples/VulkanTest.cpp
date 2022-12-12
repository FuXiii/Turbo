#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TEngine.h"
#include "TFormatInfo.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"

#include "TBuffer.h"
#include "TCommandBuffer.h"
#include "TCommandBufferPool.h"
#include "TImage.h"
#include "TImageView.h"

#include "TShader.h"

#include "TAttachment.h"
#include "TGraphicsPipeline.h"
#include "TRenderPass.h"
#include "TSubpass.h"

#include "TDescriptorPool.h"
#include "TDescriptorSet.h"
#include "TDescriptorSetLayout.h"
#include "TFramebuffer.h"

#include "TFence.h"
#include "TSemaphore.h"

#include <cstddef>
#include <fstream>

#include <GLFW/glfw3.h>

#include "TSurface.h"
#include "TSwapchain.h"

#include <math.h>

#include "TPipelineDescriptorSet.h"
#include "TSampler.h"

#include "TVulkanLoader.h"
#include "vulkan/vulkan_core.h"

#include <memory>
#include <stdio.h>
#include <string.h>

int main()
{
    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    Turbo::Core::TEngine engine;

    Turbo::Core::TLayerInfo khronos_validation;
    std::vector<Turbo::Core::TLayerInfo> support_layers = engine.GetInstance().GetSupportLayers();
    for (Turbo::Core::TLayerInfo &layer : support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_layer;
    if (khronos_validation.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_layer.push_back(khronos_validation);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions = engine.GetInstance().GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 0, 0, 0);
    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TPhysicalDevice *physical_device = instance->GetBestPhysicalDevice();
    std::cout << "Physical Device:" << physical_device->GetDeviceName() << std::endl;

    auto physical_device_support_extensions = physical_device->GetSupportExtensions();
    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    VkPhysicalDeviceFeatures vk_physical_device_features = {};
    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &vk_physical_device_features);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    VkImageCreateInfo vk_image_create_info = {};
    vk_image_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vk_image_create_info.pNext = nullptr;
    vk_image_create_info.flags = 0;
    vk_image_create_info.imageType = VkImageType::VK_IMAGE_TYPE_2D;
    vk_image_create_info.format = VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
    vk_image_create_info.extent.width = 512;
    vk_image_create_info.extent.height = 512;
    vk_image_create_info.extent.depth = 1;
    vk_image_create_info.mipLevels = 1;
    vk_image_create_info.arrayLayers = 1;
    vk_image_create_info.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vk_image_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
    vk_image_create_info.queueFamilyIndexCount = 0;
    vk_image_create_info.pQueueFamilyIndices = nullptr;
    vk_image_create_info.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

    VkImage vk_image = VK_NULL_HANDLE;
    VkResult result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);

    Turbo::Core::TImage *temp_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_UNORM, 512, 512, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::LINEAR, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE);
    delete temp_image;

    delete device;
    delete instance;

    return 0;
}