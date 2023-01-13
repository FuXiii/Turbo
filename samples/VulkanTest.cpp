#include "TInstance.h"
#include "core/include/TDevice.h"
#include "core/include/TDeviceQueue.h"
#include "core/include/TEngine.h"
#include "core/include/TFormatInfo.h"
#include "core/include/TPhysicalDevice.h"
#include "core/include/TVulkanAllocator.h"

#include "core/include/TBuffer.h"
#include "core/include/TCommandBuffer.h"
#include "core/include/TCommandBufferPool.h"
#include "core/include/TImage.h"
#include "core/include/TImageView.h"

#include "core/include/TShader.h"

#include "core/include/TAttachment.h"
#include "core/include/TGraphicsPipeline.h"
#include "core/include/TRenderPass.h"
#include "core/include/TSubpass.h"

#include "core/include/TDescriptorPool.h"
#include "core/include/TDescriptorSet.h"
#include "core/include/TDescriptorSetLayout.h"
#include "core/include/TFramebuffer.h"

#include "TFence.h"
#include "TSemaphore.h"

#include <cstddef>
#include <fstream>

#include <GLFW/glfw3.h>

#include "core/include/TSurface.h"
#include "core/include/TSwapchain.h"

#include <math.h>

#include "core/include/TPipelineDescriptorSet.h"
#include "core/include/TSampler.h"

#include "core/include/TVulkanLoader.h"

#include <memory>
#include <stdio.h>
#include <string.h>

std::string ReadTextFile(const std::string &filename)
{
    std::vector<std::string> data;

    std::ifstream file;

    file.open(filename, std::ios::in);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return std::string{(std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())};
}

void Test0(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test0>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();

    VkImageCreateInfo vk_image_create_info = {};
    vk_image_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    vk_image_create_info.pNext = nullptr;
    vk_image_create_info.flags = 0;
    vk_image_create_info.imageType = VkImageType::VK_IMAGE_TYPE_2D;
    vk_image_create_info.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
    vk_image_create_info.extent.width = 512;
    vk_image_create_info.extent.height = 512;
    vk_image_create_info.extent.depth = 1;
    vk_image_create_info.mipLevels = 1;
    vk_image_create_info.arrayLayers = 1;
    vk_image_create_info.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    vk_image_create_info.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
    vk_image_create_info.queueFamilyIndexCount = 0;
    vk_image_create_info.pQueueFamilyIndices = nullptr;
    vk_image_create_info.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

    // GPU only
    VkImage vk_image = VK_NULL_HANDLE;
    VkResult result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== GPU only" << std::endl;
    // staging upload
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== staging upload" << std::endl;

    // read back
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== read back" << std::endl;

    // advanced upload
    vk_image_create_info.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vk_image_create_info.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
    result = Turbo::Core::vkCreateImage(device->GetVkDevice(), &vk_image_create_info, nullptr, &vk_image);
    Turbo::Core::vkDestroyImage(device->GetVkDevice(), vk_image, nullptr);
    std::cout << "======================================== advanced upload" << std::endl;

    Turbo::Core::TImage *temp_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::B8G8R8A8_UNORM, 512, 512, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::LINEAR, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE);
    delete temp_image;

    std::vector<Turbo::Core::TFormatInfo> support_format = physical_device->GetSupportFormats();

    for (Turbo::Core::TFormatInfo &format_info_item : support_format)
    {
        VkFormat vk_format = format_info_item.GetVkFormat();
        std::cout << "vk_format::" << vk_format << std::endl;
    }

    const std::string vert_shader_str = ReadTextFile("../../asset/shaders/shader_base.vert");
    const std::string fragment_shader_str = ReadTextFile("../../asset/shaders/shader_base.frag");

    Turbo::Core::TVertexShader *vs = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, vert_shader_str);
    Turbo::Core::TFragmentShader *fs = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, fragment_shader_str);

    std::cout << vs->ToString() << std::endl;
    std::cout << fs->ToString() << std::endl;

    delete vs;
    delete fs;
    std::cout << "</Test0>" << std::endl;
}

void Test1(Turbo::Core::TDeviceQueue *deviceQueue)
{
    std::cout << "<Test1>" << std::endl;
    Turbo::Core::TDevice *device = deviceQueue->GetDevice();
    Turbo::Core::TPhysicalDevice *physical_device = device->GetPhysicalDevice();
    Turbo::Core::TInstance *instance = physical_device->GetInstance();

    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physical_device->GetVkPhysicalDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkQueue vk_queue = deviceQueue->GetVkQueue();

    VkPipelineCacheCreateInfo vk_pipeline_cache_create_info = {};
    vk_pipeline_cache_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vk_pipeline_cache_create_info.pNext = nullptr;
    vk_pipeline_cache_create_info.flags = 0;
    vk_pipeline_cache_create_info.initialDataSize = 0;
    vk_pipeline_cache_create_info.pInitialData = nullptr;

    VkPipelineCache vk_pipeline_cache = VK_NULL_HANDLE;
    VkResult result = Turbo::Core::vkCreatePipelineCache(vk_device, &vk_pipeline_cache_create_info, nullptr, &vk_pipeline_cache);
    if (result != VkResult::VK_SUCCESS)
    {
        std::cout << "Error::Turbo::Core::vkCreatePipelineCache(...)" << std::endl;
        std::cout << "</Test1>" << std::endl;
        return;
    }
    std::cout << "Success::Turbo::Core::vkCreatePipelineCache(...)" << std::endl;

    Turbo::Core::vkDestroyPipelineCache(vk_device, vk_pipeline_cache, nullptr);
    std::cout << "</Test1>" << std::endl;
}

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

    Test0(queue);
    Test1(queue);

    delete device;
    delete instance;

    return 0;
}