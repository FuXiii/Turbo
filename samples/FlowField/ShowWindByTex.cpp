#include <iostream>
#include <TInstance.h>
// #include <TExtensionInfo.h>
#include <GLFW/glfw3.h>
#include <TPhysicalDevice.h>
#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TSurface.h>
#include <TSwapchain.h>
#include <TImageView.h>
#include <TCommandBufferPool.h>
#include <TCommandBuffer.h>
#include <GlobalWind.h>
#include <TFence.h>

template <typename T>
T Max(const T &a, const T &b)
{
    return a > b ? a : b;
}

std::string asset_root(TURBO_ASSET_ROOT);

int main()
{
    auto vulkan_verssion = Turbo::Core::TInstance::GetVulkanInstanceVersion();

    std::vector<Turbo::Core::TLayerInfo> enable_layers;
    {
        auto instance_support_layers = Turbo::Core::TLayerInfo::GetInstanceLayers();
        for (auto &layer : instance_support_layers)
        {
            switch (layer.GetLayerType())
            {
            case Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION: {
                enable_layers.push_back(layer);
            }
            break;
            default: {
            }
            break;
            }
        }
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_extensions;
    {
        auto instance_support_extensions = Turbo::Core::TExtensionInfo::GetInstanceExtensions();
        for (auto &extension : instance_support_extensions)
        {
            switch (extension.GetExtensionType())
            {
            case Turbo::Core::TExtensionType::VK_KHR_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            case Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE: {
                enable_extensions.push_back(extension);
            }
            break;
            default: {
            }
            break;
            }
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(enable_layers, enable_extensions, vulkan_verssion);

    auto physical_device = instance->GetBestPhysicalDevice();

    if (!glfwInit())
        return -1;

    GLFWwindow *window;
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    {
        auto get_ratio = []() {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            if (monitor)
            {
                int xpos, ypos, width, height;
                glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);
                float widthf = width;
                return height / widthf;
            }
            return 1.0f;
        };

        int window_width = 500;
        int window_height = window_width * get_ratio();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(window_width, window_height, TURBO_PROJECT_NAME, NULL, NULL);
        VkInstance vk_instance = instance->GetVkInstance();
        glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    {
        std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
        for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
        {
            switch (extension.GetExtensionType())
            {
            case Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN: {
                enable_device_extensions.push_back(extension);
            }
            break;
            default: {
            }
            break;
            }
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, {}, enable_device_extensions, {});
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

    Turbo::Core::TRefPtr<Turbo::Extension::TSurface> surface = new Turbo::Extension::TSurface(device, nullptr, vk_surface_khr);

    // Turbo::Core::TFormatType swapchain_image_format = surface->GetSupportFormats()[0].GetFormat().GetFormatType();
    Turbo::Core::TFormatType swapchain_image_format = Turbo::Core::TFormatType::UNDEFINED;
    {
        bool is_support_B8G8R8A8_SRGB = false;
        auto surface_support_formats = surface->GetSupportFormats();
        for (auto &item : surface_support_formats)
        {
            auto format = item.GetFormat();
            if (format.GetFormatType() == Turbo::Core::TFormatType::B8G8R8A8_SRGB)
            {
                is_support_B8G8R8A8_SRGB = true;
                break;
            }
        }

        if (is_support_B8G8R8A8_SRGB)
        {
            swapchain_image_format = Turbo::Core::TFormatType::B8G8R8A8_SRGB;
        }
        else
        {
            swapchain_image_format = surface_support_formats[0].GetFormat().GetFormatType();
        }
    }

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, Max(surface->GetMinImageCount(), surface->GetMaxImageCount()), swapchain_image_format, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (auto &item : swapchain->GetImages())
    {
        Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, swapchain_image_format, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    auto load_flow_field_to_image = [&]() -> Turbo::Core::TImage * {
        GlobalWind gw(asset_root + "/global_wind.bin");
        auto width = gw.Width();
        auto height = gw.Height();

        size_t flow_field_size = 0;
        gw.Load(flow_field_size, nullptr);

        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> flow_field_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, flow_field_size);
        {
            gw.Load(flow_field_size, flow_field_buffer->Map());
            flow_field_buffer->Unmap();
        }

        Turbo::Core::TImage *result = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, width, height, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
        {
            Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> copy_command_buffer = command_pool->Allocate();
            copy_command_buffer->Begin();
            copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, result, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            copy_command_buffer->CmdCopyBufferToImage(flow_field_buffer, result, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, width, height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, width, height, 1);
            copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, result, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            copy_command_buffer->End();

            Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
            queue->Submit(copy_command_buffer, fence);
            fence->WaitUntil();
            command_pool->Free(copy_command_buffer);
        }

        return result;
    };

    Turbo::Core::TRefPtr<Turbo::Core::TImage> flow_field = load_flow_field_to_image();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    command_pool->Free(command_buffer);

    glfwTerminate();
    return 0;
}