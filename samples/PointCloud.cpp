#include <iostream>
#include <ply.h>

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
#include "core/include/TComputePipeline.h"
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

#include "core/include/TPipelineDescriptorSet.h"
#include "core/include/TSampler.h"

#include <glm/ext.hpp>

#include "core/include/TVulkanLoader.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <imgui.h>

typedef struct POSITION
{
    float x;
    float y;
    float z;
    float w;
} POSITION;

typedef struct COLOR
{
    float r;
    float g;
    float b;
    float a;
} COLOR;

typedef struct PlyPointData
{
    POSITION position;
    COLOR color;
} PlyPointData;

std::vector<PlyPointData> LoadPly(const std::string &url)
{
    std::vector<PlyPointData> result;

    int elements_count = 0;
    char **elements;
    int file_type;
    float version;

    PlyFile *ply = ply_open_for_reading(const_cast<char *>(std::string("../../asset/models/points.ply").c_str()), &elements_count, &elements, &file_type, &version);
    if (ply != nullptr)
    {
        // std::cout << "ply != nullptr" << std::endl;

        for (size_t elem_index = 0; elem_index < elements_count; elem_index++)
        {
            auto elem_name = elements[elem_index];
            if (equal_strings("vertex", elem_name))
            {
                int num_elems = 0;
                int num_props = 0;

                auto plist = ply_get_element_description(ply, elem_name, &num_elems, &num_props);
                // std::cout << "elem_name:" << elem_name << std::endl;

                struct PlyVectex
                {
                    float x;
                    float y;
                    float z;
                    unsigned char r;
                    unsigned char g;
                    unsigned char b;
                    unsigned char a;
                };

                PlyProperty vert_props[] = {
                    {"x", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVectex, x), 0, 0, 0, 0},
                    {"y", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVectex, y), 0, 0, 0, 0},
                    {"z", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVectex, z), 0, 0, 0, 0},
                    {"red", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, r), 0, 0, 0, 0},
                    {"green", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, g), 0, 0, 0, 0},
                    {"blue", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, b), 0, 0, 0, 0},
                    {"alpha", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, a), 0, 0, 0, 0},
                };

                ply_get_property(ply, elem_name, &vert_props[0]);
                ply_get_property(ply, elem_name, &vert_props[1]);
                ply_get_property(ply, elem_name, &vert_props[2]);
                ply_get_property(ply, elem_name, &vert_props[3]);
                ply_get_property(ply, elem_name, &vert_props[4]);
                ply_get_property(ply, elem_name, &vert_props[5]);
                ply_get_property(ply, elem_name, &vert_props[6]);

                for (size_t i = 0; i < num_elems; i++)
                {
                    PlyVectex vertex;
                    ply_get_element(ply, &vertex);

                    {
                        PlyPointData ply_point_data;
                        ply_point_data.position.x = vertex.x;
                        ply_point_data.position.y = vertex.y;
                        ply_point_data.position.z = vertex.z;
                        ply_point_data.position.w = 0;
                        ply_point_data.color.r = vertex.r / 255.0f;
                        ply_point_data.color.g = vertex.g / 255.0f;
                        ply_point_data.color.b = vertex.b / 255.0f;
                        ply_point_data.color.a = vertex.a / 255.0f;

                        result.push_back(ply_point_data);
                    }

                    {
                        // std::cout << "i:" << i << std::endl;
                        // std::cout << "\tx: " << vertex.x << std::endl;
                        // std::cout << "\ty: " << vertex.y << std::endl;
                        // std::cout << "\tz: " << vertex.z << std::endl;
                        // std::cout << "\tr: " << (int)vertex.r << std::endl;
                        // std::cout << "\tg: " << (int)vertex.g << std::endl;
                        // std::cout << "\tb: " << (int)vertex.b << std::endl;
                        // std::cout << "\ta: " << (int)vertex.a << std::endl;
                        //
                        // std::cout << std::endl;
                    }
                }
            }
        }

        ply_close(ply);
    }
    else
    {
        std::cout << "ply == nullptr" << std::endl;
    }

    return result;
}

int main()
{
    auto points_data = LoadPly("../../asset/models/points.ply");
    std::cout << "points_data::size::" << points_data.size() << std::endl;

    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    std::vector<Turbo::Core::TLayerInfo> support_layers;
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions;
    {
        Turbo::Core::TRefPtr<Turbo::Core::TInstance> temp_instance = new Turbo::Core::TInstance();
        support_layers = temp_instance->GetSupportLayers();
        instance_support_extensions = temp_instance->GetSupportExtensions();
    }

    Turbo::Core::TLayerInfo khronos_validation;
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

    Turbo::Core::TVersion instance_version(1, 2, 0, 0);
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> physical_device = instance->GetBestPhysicalDevice();

    if (!glfwInit())
        return -1;
    GLFWwindow *window;
    int window_width = 1920 / 2.0;
    int window_height = 1080 / 2.0;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

    Turbo::Core::TRefPtr<Turbo::Extension::TSurface> surface = new Turbo::Extension::TSurface(device, nullptr, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();

    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImage>> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> points_position_image = nullptr;
    {
        uint32_t tex_size = 512;

        std::vector<POSITION> points_position;
        for (auto &point : points_data)
        {
            points_position.push_back(point.position);
        }

        if (points_position.size() > (512 * 512))
        {
            throw std::runtime_error("points count exceed 512 * 512 = 262,144");
        }

        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> points_position_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, points_position.size() * sizeof(POSITION));
        void *volumn_ptr = points_position_buffer->Map();
        memcpy(volumn_ptr, points_position.data(), points_position.size() * sizeof(POSITION));
        points_position_buffer->Unmap();

        points_position_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, tex_size, tex_size, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> temp_command_buffer = command_pool->Allocate();
        temp_command_buffer->Begin();
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, points_position_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        {
            uint32_t row = points_position.size() / tex_size;
            uint32_t residue = points_position.size() - row * tex_size;

            {
                uint32_t copy_buffer_offset = 0;
                uint32_t copy_image_offset_x = 0;
                uint32_t copy_image_offset_y = 0;
                uint32_t copy_image_offset_z = 0;
                uint32_t copy_width = tex_size;
                uint32_t copy_height = row;
                uint32_t copy_depth = 1;
                uint32_t copy_mip_level = 0;
                temp_command_buffer->CmdCopyBufferToImage(points_position_buffer, points_position_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, 0, 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
            }

            if (residue > 0)
            {
                uint32_t copy_buffer_offset = row * tex_size * sizeof(POSITION);
                uint32_t copy_image_offset_x = 0;
                uint32_t copy_image_offset_y = row;
                uint32_t copy_image_offset_z = 0;
                uint32_t copy_width = residue;
                uint32_t copy_height = 1;
                uint32_t copy_depth = 1;
                uint32_t copy_mip_level = 0;
                temp_command_buffer->CmdCopyBufferToImage(points_position_buffer, points_position_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, /*copy_width*/ 0, /*copy_height*/ 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
            }
        }
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, points_position_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        temp_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);

        queue->Submit(temp_command_buffer, fence);

        fence->WaitUntil();

        command_pool->Free(temp_command_buffer);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TImage> points_color_image = nullptr;
    {
        uint32_t tex_size = 512;

        std::vector<COLOR> points_color;
        for (auto &point : points_data)
        {
            points_color.push_back(point.color);
        }

        if (points_color.size() > (512 * 512))
        {
            throw std::runtime_error("points count exceed 512 * 512 = 262,144");
        }

        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> points_color_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, points_color.size() * sizeof(COLOR));
        void *volumn_ptr = points_color_buffer->Map();
        memcpy(volumn_ptr, points_color.data(), points_color.size() * sizeof(COLOR));
        points_color_buffer->Unmap();

        points_color_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, tex_size, tex_size, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> temp_command_buffer = command_pool->Allocate();
        temp_command_buffer->Begin();
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, points_color_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        {
            uint32_t row = points_color.size() / tex_size;
            uint32_t residue = points_color.size() - row * tex_size;

            {
                uint32_t copy_buffer_offset = 0;
                uint32_t copy_image_offset_x = 0;
                uint32_t copy_image_offset_y = 0;
                uint32_t copy_image_offset_z = 0;
                uint32_t copy_width = tex_size;
                uint32_t copy_height = row;
                uint32_t copy_depth = 1;
                uint32_t copy_mip_level = 0;
                temp_command_buffer->CmdCopyBufferToImage(points_color_buffer, points_color_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, 0, 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
            }

            if (residue > 0)
            {
                uint32_t copy_buffer_offset = row * tex_size * sizeof(COLOR);
                uint32_t copy_image_offset_x = 0;
                uint32_t copy_image_offset_y = row;
                uint32_t copy_image_offset_z = 0;
                uint32_t copy_width = residue;
                uint32_t copy_height = 1;
                uint32_t copy_depth = 1;
                uint32_t copy_mip_level = 0;
                temp_command_buffer->CmdCopyBufferToImage(points_color_buffer, points_color_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, /*copy_width*/ 0, /*copy_height*/ 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
            }
        }
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, points_color_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        temp_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);

        queue->Submit(temp_command_buffer, fence);

        fence->WaitUntil();

        command_pool->Free(temp_command_buffer);
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}