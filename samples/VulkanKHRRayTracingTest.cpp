#include "core/include/TDevice.h"
#include "core/include/TDeviceQueue.h"
#include "core/include/TEngine.h"
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

#include "core/include/TFence.h"
#include "core/include/TSemaphore.h"

#include <fstream>

#include <GLFW/glfw3.h>

#include "core/include/TSurface.h"
#include "core/include/TSwapchain.h"

#include <math.h>

#include "TPipelineDescriptorSet.h"
#include "TSampler.h"

#include "core/include/TVulkanLoader.h"

#include <memory>
#include <stdio.h>
#include <string.h>

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/ext.hpp>

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

const std::string VERT_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 0) uniform bufferVals {\n"
                                    "    float value;\n"
                                    "} myBufferVals;\n"
                                    "layout (location = 0) in vec3 pos;\n"
                                    "layout (location = 1) in vec3 color;"
                                    "layout (location = 2) out vec3 outColor;"
                                    "layout (location = 3) out float outValue;\n"
                                    "void main() {\n"
                                    "   gl_Position = vec4(pos,1);\n"
                                    "   outColor = color;\n"
                                    "   outValue = myBufferVals.value;\n"
                                    "}\n";

const std::string FRAG_SHADER_STR = "#version 450 core\n"
                                    "layout (location = 2) in vec3 inColor;\n"
                                    "layout (location = 3) in float inValue;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "void main() {\n"
                                    "   outColor = vec4(inColor,1);\n"
                                    "}\n";

const std::string RAY_GENERATION_SHADER_STR = ReadTextFile("../../asset/shaders/RayTraceTest.rgen");

typedef struct POSITION
{
    float x;
    float y;
    float z;
} POSITION;

typedef struct COLOR
{
    float r;
    float g;
    float b;
} COLOR;

typedef struct POSITION_AND_COLOR
{
    POSITION position;
    COLOR color;
} POSITION_AND_COLOR;

typedef struct NORMAL
{
    float x;
    float y;
    float z;
} NORMAL;

typedef struct TEXCOORD
{
    float u;
    float v;
} TEXCOORD;

int main()
{
    std::vector<POSITION_AND_COLOR> POSITION_AND_COLOR_DATA;
    POSITION_AND_COLOR_DATA.push_back(POSITION_AND_COLOR{{0.0f, -0.5f, 0.0f}, {1.f, 0.f, 0.f}});
    POSITION_AND_COLOR_DATA.push_back(POSITION_AND_COLOR{{0.5f, 0.5f, 0.0f}, {0.f, 1.f, 0.f}});
    POSITION_AND_COLOR_DATA.push_back(POSITION_AND_COLOR{{-0.5f, 0.5f, 0.0f}, {0.f, 0.f, 1.f}});

    float value = -10.0f;

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
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 0, 0, 0);
    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TPhysicalDevice *physical_device = instance->GetBestPhysicalDevice();

    Turbo::Core::TPhysicalDeviceFeatures physical_device_support_features = physical_device->GetDeviceFeatures();
    {
        if (physical_device_support_features.accelerationStructure)
        {
            std::cout << "Support acceleration structure feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure feature" << std::endl;
        }

        if (physical_device_support_features.accelerationStructureCaptureReplay)
        {
            std::cout << "Support acceleration structure capture replay feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure capture replay feature" << std::endl;
        }

        if (physical_device_support_features.accelerationStructureHostCommands)
        {
            std::cout << "Support acceleration structure host commands feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure host commands feature" << std::endl;
        }

        if (physical_device_support_features.accelerationStructureIndirectBuild)
        {
            std::cout << "Support acceleration structure indirect build feature" << std::endl;
        }
        else
        {
            std::cout << "Not support acceleration structure indirect build feature" << std::endl;
        }

        if (physical_device_support_features.bufferDeviceAddress)
        {
            std::cout << "Support bufferDeviceAddress feature" << std::endl;
        }
        else
        {
            std::cout << "Not support bufferDeviceAddress feature" << std::endl;
        }

        if (physical_device_support_features.bufferDeviceAddressCaptureReplay)
        {
            std::cout << "Support bufferDeviceAddressCaptureReplay feature" << std::endl;
        }
        else
        {
            std::cout << "Not support bufferDeviceAddressCaptureReplay feature" << std::endl;
        }

        if (physical_device_support_features.bufferDeviceAddressMultiDevice)
        {
            std::cout << "Support bufferDeviceAddressMultiDevice feature" << std::endl;
        }
        else
        {
            std::cout << "Not support bufferDeviceAddressMultiDevice feature" << std::endl;
        }
    }

    if (!physical_device_support_features.accelerationStructure)
    {
        delete instance;
        std::cout << "Please use a GPU which support hardware real-time ray tracing" << std::endl;
        return 0;
    }

    if (!glfwInit())
        return -1;
    GLFWwindow *window;
    int window_width = 500;
    int window_height = 500;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;
    physical_device_features.accelerationStructure = physical_device_support_features.accelerationStructure;
    physical_device_features.accelerationStructureCaptureReplay = physical_device_support_features.accelerationStructureCaptureReplay;
    physical_device_features.accelerationStructureHostCommands = physical_device_support_features.accelerationStructureHostCommands;
    physical_device_features.accelerationStructureIndirectBuild = physical_device_support_features.accelerationStructureIndirectBuild;
    physical_device_features.bufferDeviceAddress = physical_device_support_features.bufferDeviceAddress;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    physical_device->GetSupportExtensions();
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_ACCELERATION_STRUCTURE)
        {
            enable_device_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SPIRV_1_4)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::COMBINED_IMAGE_SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLED_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::INPUT_ATTACHMENT, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::ACCELERATION_STRUCTURE, 1000));

    Turbo::Core::TDescriptorPool *descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    const Turbo::Core::TDeviceDriver *device_driver = device->GetDeviceDriver();
    const Turbo::Core::TPhysicalDeviceDriver *physical_device_driver = physical_device->GetPhysicalDeviceDriver();

    {
        VkPhysicalDeviceAccelerationStructurePropertiesKHR vk_physical_device_acceleration_structure_properties_khr = {};
        vk_physical_device_acceleration_structure_properties_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR;
        vk_physical_device_acceleration_structure_properties_khr.pNext = nullptr;
        vk_physical_device_acceleration_structure_properties_khr.maxGeometryCount = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxInstanceCount = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxPrimitiveCount = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorUpdateAfterBindAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetUpdateAfterBindAccelerationStructures = 0;
        vk_physical_device_acceleration_structure_properties_khr.minAccelerationStructureScratchOffsetAlignment = 0;

        VkPhysicalDeviceProperties2 vk_physical_device_properties_2 = {};
        vk_physical_device_properties_2.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        vk_physical_device_properties_2.pNext = &vk_physical_device_acceleration_structure_properties_khr;
        vk_physical_device_properties_2.properties = {};

        if (physical_device_driver->vkGetPhysicalDeviceProperties2 != nullptr)
        {
            physical_device_driver->vkGetPhysicalDeviceProperties2(physical_device->GetVkPhysicalDevice(), &vk_physical_device_properties_2);
        }
        else if (physical_device_driver->vkGetPhysicalDeviceProperties2KHR != nullptr)
        {
            physical_device_driver->vkGetPhysicalDeviceProperties2KHR(physical_device->GetVkPhysicalDevice(), &vk_physical_device_properties_2);
        }

        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxGeometryCount = " << vk_physical_device_acceleration_structure_properties_khr.maxGeometryCount << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxInstanceCount = " << vk_physical_device_acceleration_structure_properties_khr.maxInstanceCount << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxPrimitiveCount = " << vk_physical_device_acceleration_structure_properties_khr.maxPrimitiveCount << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxPerStageDescriptorAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxPerStageDescriptorUpdateAfterBindAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxPerStageDescriptorUpdateAfterBindAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxDescriptorSetAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.maxDescriptorSetUpdateAfterBindAccelerationStructures = " << vk_physical_device_acceleration_structure_properties_khr.maxDescriptorSetUpdateAfterBindAccelerationStructures << std::endl;
        std::cout << "VkPhysicalDeviceAccelerationStructurePropertiesKHR.minAccelerationStructureScratchOffsetAlignment = " << vk_physical_device_acceleration_structure_properties_khr.minAccelerationStructureScratchOffsetAlignment << std::endl;

        // Turbo::Core::TBuffer *device_local_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
        Turbo::Core::TBuffer *device_local_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
        {
            Turbo::Core::TBuffer *staging_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
            memcpy(staging_vertex_buffer->Map(), POSITION_AND_COLOR_DATA.data(), sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
            staging_vertex_buffer->Unmap();

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            command_buffer->CmdCopyBuffer(staging_vertex_buffer, device_local_vertex_buffer, 0, 0, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            delete staging_vertex_buffer;
        }

        VkDeviceAddress device_local_vertex_buffer_device_address = 0;

        VkBufferDeviceAddressInfo device_local_vertex_buffer_device_address_info = {};
        device_local_vertex_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        device_local_vertex_buffer_device_address_info.pNext = nullptr;
        device_local_vertex_buffer_device_address_info.buffer = device_local_vertex_buffer->GetVkBuffer();

        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            device_local_vertex_buffer_device_address = device_driver->vkGetBufferDeviceAddress(device->GetVkDevice(), &device_local_vertex_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            device_local_vertex_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(device->GetVkDevice(), &device_local_vertex_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            device_local_vertex_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(device->GetVkDevice(), &device_local_vertex_buffer_device_address_info);
        }

        if (device_local_vertex_buffer_device_address != 0)
        {
            std::cout << "Successfully get device_local_vertex_buffer VkBuffer device local address " << std::endl;
        }

        VkDeviceOrHostAddressConstKHR vertex_data = {};
        vertex_data.deviceAddress = device_local_vertex_buffer_device_address;

        VkAccelerationStructureGeometryTrianglesDataKHR vk_acceleration_structure_geometry_triangles_data_khr = {};
        vk_acceleration_structure_geometry_triangles_data_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
        vk_acceleration_structure_geometry_triangles_data_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexFormat = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexData = vertex_data;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexStride = sizeof(POSITION_AND_COLOR);
        vk_acceleration_structure_geometry_triangles_data_khr.maxVertex = POSITION_AND_COLOR_DATA.size();
        vk_acceleration_structure_geometry_triangles_data_khr.indexType = VkIndexType::VK_INDEX_TYPE_NONE_KHR;
        vk_acceleration_structure_geometry_triangles_data_khr.indexData.deviceAddress = 0;
        vk_acceleration_structure_geometry_triangles_data_khr.transformData.deviceAddress = 0;

        VkAccelerationStructureGeometryDataKHR vk_acceleration_structure_geometry_data_khr = {};
        vk_acceleration_structure_geometry_data_khr.triangles = vk_acceleration_structure_geometry_triangles_data_khr;

        VkAccelerationStructureGeometryKHR vk_acceleration_structure_geometry_khr = {};
        vk_acceleration_structure_geometry_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        vk_acceleration_structure_geometry_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_khr.geometryType = VkGeometryTypeKHR::VK_GEOMETRY_TYPE_TRIANGLES_KHR;
        vk_acceleration_structure_geometry_khr.geometry = vk_acceleration_structure_geometry_data_khr;
        vk_acceleration_structure_geometry_khr.flags = VkGeometryFlagBitsKHR::VK_GEOMETRY_OPAQUE_BIT_KHR;

        VkAccelerationStructureBuildGeometryInfoKHR vk_acceleration_structure_build_geometry_info_khr = {};
        vk_acceleration_structure_build_geometry_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
        vk_acceleration_structure_build_geometry_info_khr.pNext = nullptr;
        vk_acceleration_structure_build_geometry_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
        vk_acceleration_structure_build_geometry_info_khr.flags = VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR | VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        vk_acceleration_structure_build_geometry_info_khr.mode = VkBuildAccelerationStructureModeKHR::VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        vk_acceleration_structure_build_geometry_info_khr.srcAccelerationStructure = VK_NULL_HANDLE;
        vk_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = VK_NULL_HANDLE;
        vk_acceleration_structure_build_geometry_info_khr.geometryCount = 1;
        vk_acceleration_structure_build_geometry_info_khr.pGeometries = &vk_acceleration_structure_geometry_khr;
        vk_acceleration_structure_build_geometry_info_khr.ppGeometries = nullptr;
        vk_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = 0;

        std::vector<uint32_t> max_primitive_counts(vk_acceleration_structure_build_geometry_info_khr.geometryCount);
        for (uint32_t index = 0; index < vk_acceleration_structure_build_geometry_info_khr.geometryCount; index++)
        {
            max_primitive_counts[index] = POSITION_AND_COLOR_DATA.size() / 3;
        }

        VkAccelerationStructureBuildSizesInfoKHR vk_acceleration_structure_build_sizes_info_khr = {};
        vk_acceleration_structure_build_sizes_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
        vk_acceleration_structure_build_sizes_info_khr.pNext = nullptr;
        vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize = 0;
        vk_acceleration_structure_build_sizes_info_khr.updateScratchSize = 0;
        vk_acceleration_structure_build_sizes_info_khr.buildScratchSize = 0;
        if (device_driver->vkGetAccelerationStructureBuildSizesKHR != nullptr)
        {
            device_driver->vkGetAccelerationStructureBuildSizesKHR(device->GetVkDevice(), VkAccelerationStructureBuildTypeKHR::VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &vk_acceleration_structure_build_geometry_info_khr, max_primitive_counts.data(), &vk_acceleration_structure_build_sizes_info_khr);
        }

        std::cout << "Bottom Level VkAccelerationStructureBuildSizesInfoKHR.accelerationStructureSize = " << vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize << std::endl;
        std::cout << "Bottom Level VkAccelerationStructureBuildSizesInfoKHR.updateScratchSize = " << vk_acceleration_structure_build_sizes_info_khr.updateScratchSize << std::endl;
        std::cout << "Bottom Level VkAccelerationStructureBuildSizesInfoKHR.buildScratchSize = " << vk_acceleration_structure_build_sizes_info_khr.buildScratchSize << std::endl;

        Turbo::Core::TBuffer *bottom_level_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize);

        // create acceleration structure
        VkDevice vk_device = device->GetVkDevice();
        VkAccelerationStructureCreateInfoKHR vk_acceleration_structure_create_info_khr = {};
        vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        vk_acceleration_structure_create_info_khr.pNext = nullptr;
        vk_acceleration_structure_create_info_khr.createFlags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        vk_acceleration_structure_create_info_khr.buffer = bottom_level_acceleration_structure_buffer->GetVkBuffer();                     // 将用于存储加速结构的缓存。大小一般可以为VkAccelerationStructureCreateInfoKHR::size，usage为VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR|VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT<由于之后创建顶层加速结构需要底层加速结构的地址，所以需要VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT>
        vk_acceleration_structure_create_info_khr.offset = 0;                                                                             // 单位比特，相对于buffer的偏移之后存储加速结构，需要是256的倍数。
        vk_acceleration_structure_create_info_khr.size = vk_acceleration_structure_build_sizes_info_khr.accelerationStructureSize;        // 该加速结构需要的大小。大小来源于vkGetAccelerationStructureBuildSizesKHR()函数中VkAccelerationStructureBuildSizesInfoKHR::accelerationStructureSize。
        vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR; // 加速结构的类型：TOP，BOTTOM，GENERIC
        vk_acceleration_structure_create_info_khr.deviceAddress = 0;                                                                      // 如果激活使用了accelerationStructureCaptureReplay特性，该地址为加速结构要求的那个设备地址。目前为VK_NULL_HANDLE

        VkAllocationCallbacks *vk_allocation_callbacks = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

        VkAccelerationStructureKHR bottom_level_acceleration_structure_khr = VK_NULL_HANDLE;
        VkResult result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &bottom_level_acceleration_structure_khr);
        if (result != VK_SUCCESS)
        {
            std::cout << "Create VkAccelerationStructureKHR Failed" << std::endl;
        }

        std::cout << "Create VkAccelerationStructureKHR Success" << std::endl;

        Turbo::Core::TBuffer *scratch_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, vk_acceleration_structure_build_sizes_info_khr.buildScratchSize);

        VkBufferDeviceAddressInfo scratch_buffer_device_address_info = {};
        scratch_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        scratch_buffer_device_address_info.pNext = nullptr;
        scratch_buffer_device_address_info.buffer = scratch_buffer->GetVkBuffer();

        VkDeviceAddress scratch_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddress(vk_device, &scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(vk_device, &scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(vk_device, &scratch_buffer_device_address_info);
        }

        if (scratch_buffer_device_address != 0)
        {
            std::cout << "Successfully get scratch_buffer VkBuffer device local address " << std::endl;
        }

        vk_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = bottom_level_acceleration_structure_khr;
        vk_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = scratch_buffer_device_address;

        VkAccelerationStructureBuildRangeInfoKHR vk_acceleration_structure_build_range_info_khr = {};
        vk_acceleration_structure_build_range_info_khr.primitiveCount = POSITION_AND_COLOR_DATA.size() / 3;
        vk_acceleration_structure_build_range_info_khr.primitiveOffset = 0;
        vk_acceleration_structure_build_range_info_khr.firstVertex = 0;
        vk_acceleration_structure_build_range_info_khr.transformOffset = 0;

        std::vector<VkAccelerationStructureBuildRangeInfoKHR> vk_acceleration_structure_build_range_info_khrs;
        vk_acceleration_structure_build_range_info_khrs.push_back(vk_acceleration_structure_build_range_info_khr);

        std::vector<VkAccelerationStructureBuildRangeInfoKHR * /*指向个数为 VkAccelerationStructureBuildGeometryInfoKHR::geometryCount 的 VkAccelerationStructureBuildRangeInfoKHR 数组*/> build_range_infos; // 该数组内元素个数应与 vkCmdBuildAccelerationStructuresKHR::infoCount 对应
        build_range_infos.push_back(vk_acceleration_structure_build_range_info_khrs.data());
        // TODO: compaction query pool for compact acceleration structure
        {
            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdBuildAccelerationStructuresKHR(command_buffer->GetVkCommandBuffer(), 1, &vk_acceleration_structure_build_geometry_info_khr, build_range_infos.data());
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
        }

        // TODO: compact acceleration structure
        {
            VkQueryPool query_pool = VK_NULL_HANDLE;

            VkQueryPoolCreateInfo vk_query_pool_create_info = {};
            vk_query_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
            vk_query_pool_create_info.pNext = nullptr;
            vk_query_pool_create_info.flags = 0;
            vk_query_pool_create_info.queryType = VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
            vk_query_pool_create_info.queryCount = 1;
            vk_query_pool_create_info.pipelineStatistics = 0;

            result = device->GetDeviceDriver()->vkCreateQueryPool(vk_device, &vk_query_pool_create_info, vk_allocation_callbacks, &query_pool);
            if (result != VK_SUCCESS)
            {
                std::cout << "Create VkQueryPool Failed" << std::endl;
            }

            std::cout << "Create VkQueryPool Success" << std::endl;

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdResetQueryPool(command_buffer->GetVkCommandBuffer(), query_pool, 0, 1);
            device->GetDeviceDriver()->vkCmdWriteAccelerationStructuresPropertiesKHR(command_buffer->GetVkCommandBuffer(), 1, &bottom_level_acceleration_structure_khr, VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, query_pool, 0);
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            VkDeviceSize compact_size = 0;
            device->GetDeviceDriver()->vkGetQueryPoolResults(vk_device, query_pool, 0, 1, sizeof(VkDeviceSize), &compact_size, sizeof(VkDeviceSize), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT);
            std::cout << "compact size:" << compact_size << std::endl;

            delete fence;
            command_pool->Free(command_buffer);

            VkAccelerationStructureKHR compact_bottom_level_acceleration_structure_khr = VK_NULL_HANDLE;

            Turbo::Core::TBuffer *compact_bottom_level_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, compact_size);

            VkAccelerationStructureCreateInfoKHR compact_vk_acceleration_structure_create_info_khr = {};
            compact_vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
            compact_vk_acceleration_structure_create_info_khr.pNext = nullptr;
            compact_vk_acceleration_structure_create_info_khr.createFlags = 0;
            compact_vk_acceleration_structure_create_info_khr.buffer = compact_bottom_level_acceleration_structure_buffer->GetVkBuffer();
            compact_vk_acceleration_structure_create_info_khr.offset = 0;
            compact_vk_acceleration_structure_create_info_khr.size = compact_size;
            compact_vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
            compact_vk_acceleration_structure_create_info_khr.deviceAddress = 0;

            result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &compact_vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &compact_bottom_level_acceleration_structure_khr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Failed" << std::endl;
            }

            std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Success" << std::endl;

            VkCopyAccelerationStructureInfoKHR vk_copy_acceleration_structure_info_khr = {};
            vk_copy_acceleration_structure_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
            vk_copy_acceleration_structure_info_khr.pNext = nullptr;
            vk_copy_acceleration_structure_info_khr.src = bottom_level_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.dst = compact_bottom_level_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.mode = VkCopyAccelerationStructureModeKHR::VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;

            command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdCopyAccelerationStructureKHR(command_buffer->GetVkCommandBuffer(), &vk_copy_acceleration_structure_info_khr);
            command_buffer->End();
            fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();
            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            device->GetDeviceDriver()->vkDestroyQueryPool(vk_device, query_pool, vk_allocation_callbacks);

            // destroy no compact acceleration structure
            device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(vk_device, bottom_level_acceleration_structure_khr, vk_allocation_callbacks);
            delete bottom_level_acceleration_structure_buffer;

            // set acceleration structure value to compact version
            bottom_level_acceleration_structure_khr = compact_bottom_level_acceleration_structure_khr;
            bottom_level_acceleration_structure_buffer = compact_bottom_level_acceleration_structure_buffer;
        }

        // Top Level Acceleration Structure
        glm::mat4 model = glm::mat4(1.0f);
        VkTransformMatrixKHR vk_transform_matrix_khr = {};
        memcpy(&vk_transform_matrix_khr, &model, sizeof(VkTransformMatrixKHR));

        VkAccelerationStructureDeviceAddressInfoKHR bottom_level_acceleration_structure_device_address_info_khr = {};
        bottom_level_acceleration_structure_device_address_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
        bottom_level_acceleration_structure_device_address_info_khr.pNext = nullptr;
        bottom_level_acceleration_structure_device_address_info_khr.accelerationStructure = bottom_level_acceleration_structure_khr;

        VkDeviceAddress bottom_level_acceleration_structure_device_address = device->GetDeviceDriver()->vkGetAccelerationStructureDeviceAddressKHR(vk_device, &bottom_level_acceleration_structure_device_address_info_khr);

        VkAccelerationStructureInstanceKHR vk_acceleration_structure_instance_khr = {};
        vk_acceleration_structure_instance_khr.transform = vk_transform_matrix_khr;
        vk_acceleration_structure_instance_khr.instanceCustomIndex = 0;
        vk_acceleration_structure_instance_khr.mask = 0xFF;
        vk_acceleration_structure_instance_khr.instanceShaderBindingTableRecordOffset = 0;
        vk_acceleration_structure_instance_khr.flags = VkGeometryInstanceFlagBitsKHR::VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
        vk_acceleration_structure_instance_khr.accelerationStructureReference = bottom_level_acceleration_structure_device_address;

        Turbo::Core::TBuffer *instance_buffer = nullptr;
        {
            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

            Turbo::Core::TBuffer *staging_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(VkAccelerationStructureInstanceKHR));
            void *staging_ptr = staging_buffer->Map();
            if (staging_ptr)
            {
                memcpy(staging_ptr, &vk_acceleration_structure_instance_khr, sizeof(VkAccelerationStructureInstanceKHR));
            }
            staging_buffer->Unmap();

            instance_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, sizeof(VkAccelerationStructureInstanceKHR));

            command_buffer->Begin();
            command_buffer->CmdCopyBuffer(staging_buffer, instance_buffer, 0, 0, sizeof(VkAccelerationStructureInstanceKHR));
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            delete staging_buffer;
            command_pool->Free(command_buffer);
            delete command_pool;
        }

        VkBufferDeviceAddressInfo instance_buffer_device_address_info = {};
        instance_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        instance_buffer_device_address_info.pNext = nullptr;
        instance_buffer_device_address_info.buffer = instance_buffer->GetVkBuffer();

        VkDeviceAddress instance_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            instance_buffer_device_address = device_driver->vkGetBufferDeviceAddress(vk_device, &instance_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            instance_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(vk_device, &instance_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            instance_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(vk_device, &instance_buffer_device_address_info);
        }

        if (instance_buffer_device_address != 0)
        {
            std::cout << "Successfully get instance_buffer VkBuffer device local address " << std::endl;
        }

        // Create Top Level Acceleration Structure
        VkAccelerationStructureGeometryInstancesDataKHR vk_acceleration_structure_geometry_instances_data_khr = {};
        vk_acceleration_structure_geometry_instances_data_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
        vk_acceleration_structure_geometry_instances_data_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_instances_data_khr.arrayOfPointers = VK_FALSE;
        vk_acceleration_structure_geometry_instances_data_khr.data.deviceAddress = instance_buffer_device_address;

        VkAccelerationStructureGeometryKHR top_level_acceleration_structure_geometry_khr = {};
        top_level_acceleration_structure_geometry_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        top_level_acceleration_structure_geometry_khr.pNext = nullptr;
        top_level_acceleration_structure_geometry_khr.geometryType = VkGeometryTypeKHR::VK_GEOMETRY_TYPE_INSTANCES_KHR;
        top_level_acceleration_structure_geometry_khr.geometry.instances = vk_acceleration_structure_geometry_instances_data_khr;
        top_level_acceleration_structure_geometry_khr.flags = 0;

        VkAccelerationStructureBuildGeometryInfoKHR top_level_acceleration_structure_build_geometry_info_khr = {};
        top_level_acceleration_structure_build_geometry_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.pNext = nullptr;
        top_level_acceleration_structure_build_geometry_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.flags = VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.mode = VkBuildAccelerationStructureModeKHR::VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        top_level_acceleration_structure_build_geometry_info_khr.srcAccelerationStructure = VK_NULL_HANDLE;
        top_level_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = VK_NULL_HANDLE;
        top_level_acceleration_structure_build_geometry_info_khr.geometryCount = 1;
        top_level_acceleration_structure_build_geometry_info_khr.pGeometries = &top_level_acceleration_structure_geometry_khr;
        top_level_acceleration_structure_build_geometry_info_khr.ppGeometries = nullptr;
        top_level_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = 0;
        top_level_acceleration_structure_build_geometry_info_khr.scratchData.hostAddress = 0;

        uint32_t instance_count = 1;
        VkAccelerationStructureBuildSizesInfoKHR top_level_acceleration_structure_build_sizes_info_khr = {};
        top_level_acceleration_structure_build_sizes_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
        top_level_acceleration_structure_build_sizes_info_khr.pNext = nullptr;
        top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize = 0;
        top_level_acceleration_structure_build_sizes_info_khr.updateScratchSize = 0;
        top_level_acceleration_structure_build_sizes_info_khr.buildScratchSize = 0;

        device->GetDeviceDriver()->vkGetAccelerationStructureBuildSizesKHR(vk_device, VkAccelerationStructureBuildTypeKHR::VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &top_level_acceleration_structure_build_geometry_info_khr, &instance_count, &top_level_acceleration_structure_build_sizes_info_khr);

        std::cout << "Top Level VkAccelerationStructureBuildSizesInfoKHR.accelerationStructureSize = " << top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize << std::endl;
        std::cout << "Top Level VkAccelerationStructureBuildSizesInfoKHR.updateScratchSize = " << top_level_acceleration_structure_build_sizes_info_khr.updateScratchSize << std::endl;
        std::cout << "Top Level VkAccelerationStructureBuildSizesInfoKHR.buildScratchSize = " << top_level_acceleration_structure_build_sizes_info_khr.buildScratchSize << std::endl;

        Turbo::Core::TBuffer *top_level_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize);

        VkAccelerationStructureCreateInfoKHR top_level_acceleration_structure_create_info_khr = {};
        top_level_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        top_level_acceleration_structure_create_info_khr.pNext = nullptr;
        top_level_acceleration_structure_create_info_khr.createFlags = 0;
        top_level_acceleration_structure_create_info_khr.buffer = top_level_acceleration_structure_buffer->GetVkBuffer();
        top_level_acceleration_structure_create_info_khr.offset = 0;
        top_level_acceleration_structure_create_info_khr.size = top_level_acceleration_structure_build_sizes_info_khr.accelerationStructureSize;
        top_level_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
        top_level_acceleration_structure_create_info_khr.deviceAddress = 0;

        VkAccelerationStructureKHR top_level_acceleration_structure_khr = VK_NULL_HANDLE;

        result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &top_level_acceleration_structure_create_info_khr, vk_allocation_callbacks, &top_level_acceleration_structure_khr);
        if (result != VkResult::VK_SUCCESS)
        {
            std::cout << "vkCreateAccelerationStructureKHR create Top Level VkAccelerationStructureKHR Failed" << std::endl;
        }

        std::cout << "vkCreateAccelerationStructureKHR create Top Level VkAccelerationStructureKHR Success" << std::endl;

        Turbo::Core::TBuffer *top_level_scratch_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_STORAGE_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, top_level_acceleration_structure_build_sizes_info_khr.buildScratchSize);

        VkBufferDeviceAddressInfo top_level_scratch_buffer_device_address_info = {};
        top_level_scratch_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        top_level_scratch_buffer_device_address_info.pNext = nullptr;
        top_level_scratch_buffer_device_address_info.buffer = top_level_scratch_buffer->GetVkBuffer();

        VkDeviceAddress top_level_scratch_buffer_device_address = 0;
        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            top_level_scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddress(vk_device, &top_level_scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            top_level_scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressKHR(vk_device, &top_level_scratch_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            top_level_scratch_buffer_device_address = device_driver->vkGetBufferDeviceAddressEXT(vk_device, &top_level_scratch_buffer_device_address_info);
        }

        if (top_level_scratch_buffer_device_address != 0)
        {
            std::cout << "Successfully get top_level_scratch_buffer VkBuffer device local address " << std::endl;
        }

        // Build Top Level Acceleration Structure
        top_level_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure = top_level_acceleration_structure_khr;
        top_level_acceleration_structure_build_geometry_info_khr.scratchData.deviceAddress = top_level_scratch_buffer_device_address;

        VkAccelerationStructureBuildRangeInfoKHR top_level_acceleration_structure_build_range_info_khr = {};
        top_level_acceleration_structure_build_range_info_khr.primitiveCount = 1;
        top_level_acceleration_structure_build_range_info_khr.primitiveOffset = 0;
        top_level_acceleration_structure_build_range_info_khr.firstVertex = 0;
        top_level_acceleration_structure_build_range_info_khr.transformOffset = 0;

        std::vector<VkAccelerationStructureBuildRangeInfoKHR> top_level_acceleration_structure_build_range_info_khrs;
        top_level_acceleration_structure_build_range_info_khrs.push_back(top_level_acceleration_structure_build_range_info_khr);

        std::vector<VkAccelerationStructureBuildRangeInfoKHR *> top_level_acceleration_structure_ppBuildRangeInfos;
        top_level_acceleration_structure_ppBuildRangeInfos.push_back(top_level_acceleration_structure_build_range_info_khrs.data());

        {
            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdBuildAccelerationStructuresKHR(command_buffer->GetVkCommandBuffer(), 1, &top_level_acceleration_structure_build_geometry_info_khr, top_level_acceleration_structure_ppBuildRangeInfos.data());
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
        }

        // Top Level Acceleration Structure Compact
        {
            VkQueryPool query_pool = VK_NULL_HANDLE;

            VkQueryPoolCreateInfo vk_query_pool_create_info = {};
            vk_query_pool_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
            vk_query_pool_create_info.pNext = nullptr;
            vk_query_pool_create_info.flags = 0;
            vk_query_pool_create_info.queryType = VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR;
            vk_query_pool_create_info.queryCount = 1;
            vk_query_pool_create_info.pipelineStatistics = 0;

            result = device->GetDeviceDriver()->vkCreateQueryPool(vk_device, &vk_query_pool_create_info, vk_allocation_callbacks, &query_pool);
            if (result != VK_SUCCESS)
            {
                std::cout << "Create VkQueryPool Failed" << std::endl;
            }

            std::cout << "Create VkQueryPool Success" << std::endl;

            Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
            Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdResetQueryPool(command_buffer->GetVkCommandBuffer(), query_pool, 0, 1);
            device->GetDeviceDriver()->vkCmdWriteAccelerationStructuresPropertiesKHR(command_buffer->GetVkCommandBuffer(), 1, &top_level_acceleration_structure_khr, VkQueryType::VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, query_pool, 0);
            command_buffer->End();
            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();

            VkDeviceSize compact_size = 0;
            device->GetDeviceDriver()->vkGetQueryPoolResults(vk_device, query_pool, 0, 1, sizeof(VkDeviceSize), &compact_size, sizeof(VkDeviceSize), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT);
            std::cout << "top level acceleration structure compact size:" << compact_size << std::endl;

            delete fence;
            command_pool->Free(command_buffer);

            VkAccelerationStructureKHR compact_acceleration_structure_khr = VK_NULL_HANDLE;

            Turbo::Core::TBuffer *compact_acceleration_structure_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_ACCELERATION_STRUCTURE_STORAGE | Turbo::Core::TBufferUsageBits::BUFFER_SHADER_DEVICE_ADDRESS, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, compact_size);

            VkAccelerationStructureCreateInfoKHR compact_vk_acceleration_structure_create_info_khr = {};
            compact_vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
            compact_vk_acceleration_structure_create_info_khr.pNext = nullptr;
            compact_vk_acceleration_structure_create_info_khr.createFlags = 0;
            compact_vk_acceleration_structure_create_info_khr.buffer = compact_acceleration_structure_buffer->GetVkBuffer();
            compact_vk_acceleration_structure_create_info_khr.offset = 0;
            compact_vk_acceleration_structure_create_info_khr.size = compact_size;
            compact_vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
            compact_vk_acceleration_structure_create_info_khr.deviceAddress = 0;

            result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &compact_vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &compact_acceleration_structure_khr);
            if (result != VkResult::VK_SUCCESS)
            {
                std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Failed" << std::endl;
            }

            std::cout << "vkCreateAccelerationStructureKHR create compact VkAccelerationStructureKHR Success" << std::endl;

            VkCopyAccelerationStructureInfoKHR vk_copy_acceleration_structure_info_khr = {};
            vk_copy_acceleration_structure_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
            vk_copy_acceleration_structure_info_khr.pNext = nullptr;
            vk_copy_acceleration_structure_info_khr.src = top_level_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.dst = compact_acceleration_structure_khr;
            vk_copy_acceleration_structure_info_khr.mode = VkCopyAccelerationStructureModeKHR::VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;

            command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            device->GetDeviceDriver()->vkCmdCopyAccelerationStructureKHR(command_buffer->GetVkCommandBuffer(), &vk_copy_acceleration_structure_info_khr);
            command_buffer->End();
            fence = new Turbo::Core::TFence(device);
            queue->Submit(nullptr, nullptr, command_buffer, fence);
            fence->WaitUntil();
            delete fence;
            command_pool->Free(command_buffer);
            delete command_pool;
            device->GetDeviceDriver()->vkDestroyQueryPool(vk_device, query_pool, vk_allocation_callbacks);

            // destroy no compact acceleration structure
            device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(vk_device, top_level_acceleration_structure_khr, vk_allocation_callbacks);
            delete top_level_acceleration_structure_buffer;

            // set acceleration structure value to compact version
            top_level_acceleration_structure_khr = compact_acceleration_structure_khr;
            top_level_acceleration_structure_buffer = compact_acceleration_structure_buffer;
        }

        Turbo::Core::TRayGenerationShader *ray_generation_shader_test = new Turbo::Core::TRayGenerationShader(device, Turbo::Core::TShaderLanguage::GLSL, RAY_GENERATION_SHADER_STR);

        delete ray_generation_shader_test;
        delete top_level_scratch_buffer;
        device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(vk_device, top_level_acceleration_structure_khr, vk_allocation_callbacks);
        delete top_level_acceleration_structure_buffer;
        delete instance_buffer;
        device->GetDeviceDriver()->vkDestroyAccelerationStructureKHR(vk_device, bottom_level_acceleration_structure_khr, vk_allocation_callbacks);
        delete scratch_buffer;
        delete bottom_level_acceleration_structure_buffer;
        delete device_local_vertex_buffer;
    }

    Turbo::Extension::TSurface *surface = new Turbo::Extension::TSurface(device, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();

    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Extension::TSwapchain *swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TImageView *> swapchain_image_views;
    for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

    Turbo::Core::TBuffer *value_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    void *value_ptr = value_buffer->Map();
    memcpy(value_ptr, &value, sizeof(value));
    value_buffer->Unmap();

    Turbo::Core::TBuffer *vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
    void *vertx_buffer_ptr = vertex_buffer->Map();
    memcpy(vertx_buffer_ptr, POSITION_AND_COLOR_DATA.data(), sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
    vertex_buffer->Unmap();
    POSITION_AND_COLOR_DATA.clear();

    Turbo::Core::TImage *depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TImageView *depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TVertexShader *vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TFragmentShader *fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    // std::cout << vertex_shader->ToString() << std::endl;
    // std::cout << fragment_shader->ToString() << std::endl;

    std::vector<Turbo::Core::TBuffer *> buffers;
    buffers.push_back(value_buffer);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // swapchain color image
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass); // subpass 0

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    Turbo::Core::TVertexBinding vertex_binding(0, sizeof(POSITION_AND_COLOR), Turbo::Core::TVertexRate::VERTEX);
    vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_AND_COLOR, position)); // position
    vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_AND_COLOR, color));    // color

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
    vertex_bindings.push_back(vertex_binding);

    Turbo::Core::TViewport viewport(0, 0, 500, 500, 0, 1);
    Turbo::Core::TScissor scissor(0, 0, 500, 500);

    Turbo::Core::TGraphicsPipeline *pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, vertex_shader, fragment_shader, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false);

    Turbo::Core::TPipelineDescriptorSet *pipeline_descriptor_set = descriptor_pool->Allocate(pipeline->GetPipelineLayout());
    pipeline_descriptor_set->BindData(0, 0, 0, buffers);

    std::vector<Turbo::Core::TBuffer *> vertex_buffers;
    vertex_buffers.push_back(vertex_buffer);

    std::vector<Turbo::Core::TFramebuffer *> swpachain_framebuffers;
    for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TImageView *> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(depth_image_view);

        Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    bool show_demo_window = true;

    float _time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        void *_ptr = value_buffer->Map();
        memcpy(_ptr, &value, sizeof(value));
        value_buffer->Unmap();

        //<Begin Rendering>
        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TSemaphore *wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

            // because we just have one command buffer, so we should reset the command buffer for each frame
            // If we create command buffer for each swapchain image, we don't need to reset it each frame

            Turbo::Core::TViewport frame_viewport(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor frame_scissor(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight());

            std::vector<Turbo::Core::TViewport> frame_viewports;
            frame_viewports.push_back(frame_viewport);

            std::vector<Turbo::Core::TScissor> frame_scissors;
            frame_scissors.push_back(frame_scissor);

            command_buffer->Begin();
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);

            // Triangle
            command_buffer->CmdBindPipeline(pipeline);
            command_buffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);
            command_buffer->CmdBindVertexBuffers(vertex_buffers);
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            command_buffer->CmdDraw(3, 1, 0, 0);

            command_buffer->CmdEndRenderPass();
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TSemaphore *> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(&wait_semaphores, nullptr, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

            delete fence;

            command_buffer->Reset(); // you can create an command buffer each for one swapchain image,for now just one command buffer

            Turbo::Core::TResult present_result = queue->Present(swapchain, current_image_index);
            switch (present_result)
            {
            case Turbo::Core::TResult::MISMATCH: {
                // the size of the window had changed you need to recreate swapchain

                // waite for idle
                device->WaitIdle();

                // destroy related resource
                // clear old swapchain image
                swapchain_images.clear();

                // destroy swapchain image views
                for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
                {
                    delete image_view_item;
                }

                swapchain_image_views.clear();

                // destroy depth image and view
                delete depth_image_view;
                delete depth_image;

                // destroy framebuffer
                for (Turbo::Core::TFramebuffer *frame_buffer_item : swpachain_framebuffers)
                {
                    delete frame_buffer_item;
                }
                swpachain_framebuffers.clear();

                // recreate swapchain
                Turbo::Extension::TSwapchain *old_swapchain = swapchain;
                Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
                delete old_swapchain;

                swapchain = new_swapchain;

                // recreate swapchain image views
                swapchain_images = swapchain->GetImages();
                for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
                {
                    Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TImageView *> swapchain_image_views;
                    swapchain_image_views.push_back(image_view_item);
                    swapchain_image_views.push_back(depth_image_view);

                    Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                    swpachain_framebuffers.push_back(swapchain_framebuffer);
                }
            }
            break;
            default: {
                //
            }
            break;
            }
        }
        break;
        case Turbo::Core::TResult::TIMEOUT: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::NOT_READY: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::MISMATCH: {
            // the size of the window had changed you need to recreate swapchain

            // waite for idle
            device->WaitIdle();

            // destroy related resource
            // clear old swapchain image
            swapchain_images.clear();

            // destroy swapchain image views
            for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
            {
                delete image_view_item;
            }

            swapchain_image_views.clear();

            // destroy depth image and view
            delete depth_image_view;
            delete depth_image;

            // destroy framebuffer
            for (Turbo::Core::TFramebuffer *frame_buffer_item : swpachain_framebuffers)
            {
                delete frame_buffer_item;
            }
            swpachain_framebuffers.clear();

            // recreate swapchain
            Turbo::Extension::TSwapchain *old_swapchain = swapchain;
            Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
            delete old_swapchain;

            swapchain = new_swapchain;

            // recreate swapchain image views
            swapchain_images = swapchain->GetImages();
            for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
            {
                Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

            // recreate framebuffer
            for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
            {
                std::vector<Turbo::Core::TImageView *> swapchain_image_views;
                swapchain_image_views.push_back(image_view_item);
                swapchain_image_views.push_back(depth_image_view);

                Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                swpachain_framebuffers.push_back(swapchain_framebuffer);
            }
        }
        break;
        default: {
            //
        }
        break;
        }

        delete wait_image_ready;
        //</End Rendering>
    }

    descriptor_pool->Free(pipeline_descriptor_set);
    delete pipeline;
    for (Turbo::Core::TFramebuffer *framebuffer_item : swpachain_framebuffers)
    {
        delete framebuffer_item;
    }

    delete render_pass;

    delete descriptor_pool;
    delete vertex_shader;
    delete fragment_shader;
    delete depth_image_view;
    delete depth_image;
    for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
    {
        delete image_view_item;
    }
    delete vertex_buffer;
    delete value_buffer;
    command_pool->Free(command_buffer);
    delete command_pool;
    delete swapchain;
    delete surface;
    PFN_vkDestroySurfaceKHR pfn_vk_destroy_surface_khr = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
    pfn_vk_destroy_surface_khr(instance->GetVkInstance(), vk_surface_khr, nullptr);
    glfwTerminate();
    delete device;
    delete instance;

    return 0;
}