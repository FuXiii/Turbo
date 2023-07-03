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
void ImageSaveToPPM(Turbo::Core::TImage *image, Turbo::Core::TCommandBufferPool *commandBufferPool, Turbo::Core::TDeviceQueue *deviceQueue, std::string name)
{
    std::string save_file_path = "./";
    std::string save_file_name = name;

    Turbo::Core::TImage *source_image = image;

    Turbo::Core::TImage *temp_image = new Turbo::Core::TImage(image->GetDevice(), 0, Turbo::Core::TImageType::DIMENSION_2D, source_image->GetFormat(), source_image->GetWidth(), source_image->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::LINEAR, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE);

    Turbo::Core::TCommandBuffer *temp_command_buffer = commandBufferPool->Allocate();

    temp_command_buffer->Begin();
    temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, 0, 0, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, temp_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, 0, 0, Turbo::Core::TImageLayout::PRESENT_SRC_KHR, Turbo::Core::TImageLayout::TRANSFER_SRC_OPTIMAL, source_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    temp_command_buffer->CmdCopyImage(source_image, Turbo::Core::TImageLayout::TRANSFER_SRC_OPTIMAL, temp_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, source_image->GetWidth(), source_image->GetHeight(), 1);
    temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::HOST_BIT, 0, 0, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::GENERAL, temp_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    temp_command_buffer->End();

    Turbo::Core::TFence *gpu_copy_to_cpu_fence = new Turbo::Core::TFence(image->GetDevice());

    deviceQueue->Submit(nullptr, nullptr, temp_command_buffer, gpu_copy_to_cpu_fence);

    gpu_copy_to_cpu_fence->WaitUntil();

    delete gpu_copy_to_cpu_fence;

    std::string filename;
    filename.append(save_file_path);
    filename.append(save_file_name);
    filename.append(".ppm");

    VkImageSubresource subres = {};
    subres.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subres.mipLevel = 0;
    subres.arrayLayer = 0;
    VkSubresourceLayout sr_layout;
    image->GetDevice()->GetDeviceDriver()->vkGetImageSubresourceLayout(image->GetDevice()->GetVkDevice(), temp_image->GetVkImage(), &subres, &sr_layout);

    char *ptr = (char *)temp_image->Map();

    ptr += sr_layout.offset;
    std::ofstream file(filename.c_str(), std::ios::binary);

    file << "P6\n";
    file << source_image->GetWidth() << " ";
    file << source_image->GetHeight() << "\n";
    file << 255 << "\n";

    int x = 0;
    int y = 0;

    for (y = 0; y < source_image->GetHeight(); y++)
    {
        const int *row = (const int *)ptr;
        int swapped;

        if (source_image->GetFormat().GetVkFormat() == VK_FORMAT_B8G8R8A8_UNORM || source_image->GetFormat().GetVkFormat() == VK_FORMAT_B8G8R8A8_SRGB)
        {
            for (x = 0; x < source_image->GetWidth(); x++)
            {
                swapped = (*row & 0xff00ff00) | (*row & 0x000000ff) << 16 | (*row & 0x00ff0000) >> 16;
                file.write((char *)&swapped, 3);
                row++;
            }
        }
        else if (source_image->GetFormat().GetVkFormat() == VK_FORMAT_R8G8B8A8_UNORM)
        {
            for (x = 0; x < source_image->GetWidth(); x++)
            {
                file.write((char *)row, 3);
                row++;
            }
        }
        else
        {
            printf("Unrecognized image format - will not write image files");
            break;
        }

        ptr += sr_layout.rowPitch;
    }

    file.close();
    temp_image->Unmap();
    delete temp_image;
    commandBufferPool->Free(temp_command_buffer);
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
    }

    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    const Turbo::Core::TDeviceDriver *device_driver = device->GetDeviceDriver();

    {
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

        VkDeviceAddress device_address = 0;

        VkBufferDeviceAddressInfo vk_buffer_device_address_info = {};
        vk_buffer_device_address_info.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        vk_buffer_device_address_info.pNext = nullptr;
        vk_buffer_device_address_info.buffer = device_local_vertex_buffer->GetVkBuffer();

        if (device_driver->vkGetBufferDeviceAddress != nullptr)
        {
            device_address = device_driver->vkGetBufferDeviceAddress(device->GetVkDevice(), &vk_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressKHR != nullptr)
        {
            device_address = device_driver->vkGetBufferDeviceAddressKHR(device->GetVkDevice(), &vk_buffer_device_address_info);
        }
        else if (device_driver->vkGetBufferDeviceAddressEXT != nullptr)
        {
            device_address = device_driver->vkGetBufferDeviceAddressEXT(device->GetVkDevice(), &vk_buffer_device_address_info);
        }

        if (device_address != 0)
        {
            std::cout << "Successfully get VkBuffer device local address " << std::endl;
        }

        VkDeviceOrHostAddressConstKHR vertex_data = {};

        VkAccelerationStructureGeometryTrianglesDataKHR vk_acceleration_structure_geometry_triangles_data_khr = {};
        vk_acceleration_structure_geometry_triangles_data_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
        vk_acceleration_structure_geometry_triangles_data_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexFormat = VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexData;
        vk_acceleration_structure_geometry_triangles_data_khr.vertexStride;
        vk_acceleration_structure_geometry_triangles_data_khr.maxVertex;
        vk_acceleration_structure_geometry_triangles_data_khr.indexType;
        vk_acceleration_structure_geometry_triangles_data_khr.indexData;
        vk_acceleration_structure_geometry_triangles_data_khr.transformData;

        VkAccelerationStructureGeometryDataKHR vk_acceleration_structure_geometry_data_khr = {};
        vk_acceleration_structure_geometry_data_khr.triangles = vk_acceleration_structure_geometry_triangles_data_khr;

        VkAccelerationStructureGeometryKHR vk_acceleration_structure_geometry_khr = {};
        vk_acceleration_structure_geometry_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
        vk_acceleration_structure_geometry_khr.pNext = nullptr;
        vk_acceleration_structure_geometry_khr.geometryType = VkGeometryTypeKHR ::VK_GEOMETRY_TYPE_TRIANGLES_KHR;
        vk_acceleration_structure_geometry_khr.geometry = vk_acceleration_structure_geometry_data_khr;
        vk_acceleration_structure_geometry_khr.flags = VkGeometryFlagBitsKHR::VK_GEOMETRY_OPAQUE_BIT_KHR;

        VkAccelerationStructureBuildGeometryInfoKHR vk_acceleration_structure_build_geometry_info_khr = {};
        vk_acceleration_structure_build_geometry_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
        vk_acceleration_structure_build_geometry_info_khr.pNext = nullptr;
        vk_acceleration_structure_build_geometry_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
        vk_acceleration_structure_build_geometry_info_khr.flags = VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR | VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        vk_acceleration_structure_build_geometry_info_khr.mode = VkBuildAccelerationStructureModeKHR::VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
        vk_acceleration_structure_build_geometry_info_khr.srcAccelerationStructure = VK_NULL_HANDLE;
        vk_acceleration_structure_build_geometry_info_khr.dstAccelerationStructure;
        vk_acceleration_structure_build_geometry_info_khr.geometryCount;
        vk_acceleration_structure_build_geometry_info_khr.pGeometries;
        vk_acceleration_structure_build_geometry_info_khr.ppGeometries;
        vk_acceleration_structure_build_geometry_info_khr.scratchData;

        // create acceleration structure
        VkDevice vk_device = device->GetVkDevice();
        VkAccelerationStructureCreateInfoKHR vk_acceleration_structure_create_info_khr = {};
        vk_acceleration_structure_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
        vk_acceleration_structure_create_info_khr.pNext = nullptr;
        vk_acceleration_structure_create_info_khr.createFlags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
        vk_acceleration_structure_create_info_khr.buffer = VK_NULL_HANDLE;                                                                // 将用于存储加速结构的缓存。大小一般可以为VkAccelerationStructureCreateInfoKHR::size，usage为VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR|VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT<由于之后创建顶层加速结构需要底层加速结构的地址，所以需要VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT>
        vk_acceleration_structure_create_info_khr.offset = 0;                                                                             // 单位比特，相对于buffer的偏移之后存储加速结构，需要是256的倍数。
        vk_acceleration_structure_create_info_khr.size = 0;                                                                               // 该加速结构需要的大小。大小来源于vkGetAccelerationStructureBuildSizesKHR()函数中VkAccelerationStructureBuildSizesInfoKHR::accelerationStructureSize。
        vk_acceleration_structure_create_info_khr.type = VkAccelerationStructureTypeKHR::VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR; // 加速结构的类型：TOP，BOTTOM，GENERIC
        vk_acceleration_structure_create_info_khr.deviceAddress = 0;                                                                      // 如果激活使用了accelerationStructureCaptureReplay特性，该地址为加速结构要求的那个设备地址。目前为VK_NULL_HANDLE

        VkAllocationCallbacks *vk_allocation_callbacks = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

        VkAccelerationStructureKHR vk_acceleration_structure_khr = VK_NULL_HANDLE;
        // VkResult result = device->GetDeviceDriver()->vkCreateAccelerationStructureKHR(vk_device, &vk_acceleration_structure_create_info_khr, vk_allocation_callbacks, &vk_acceleration_structure_khr);
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

    Turbo::Core::TDescriptorPool *descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

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

    ImageSaveToPPM(swapchain_images[0], command_pool, queue, "PureHelloTriangle");

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