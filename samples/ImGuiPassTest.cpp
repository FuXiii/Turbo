#include <iostream>
#include <TInstance.h>
#include <GLFW/glfw3.h>
#include <TPhysicalDevice.h>
#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TSurface.h>
#include <TSwapchain.h>
#include <TImageView.h>
#include <TCommandBufferPool.h>
#include <TCommandBuffer.h>
#include <TFence.h>
#include <TSemaphore.h>
#include <TAttachment.h>
#include <TRenderPass.h>

#include <imgui.h>
#include <ReadFile.h>
#include <TGraphicsPipeline.h>
#include <TPipelineDescriptorSet.h>
#include <TFramebuffer.h>
#include <TBarrier.h>

#include <ImGuiPass.h>

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

    Turbo::Core::TRefPtr<ImGuiPass> imgui_pass = new ImGuiPass(window);

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

    Turbo::Core::TFormatType swapchain_image_format = surface->GetSupportFormats().front().GetFormat().GetFormatType();

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, surface->GetMaxImageCount(), swapchain_image_format, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (auto &item : swapchain->GetImages())
    {
        Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, swapchain_image_format, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);

    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::COMBINED_IMAGE_SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::SAMPLED_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::STORAGE_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::UNIFORM_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::STORAGE_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::STORAGE_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptor::TType::INPUT_ATTACHMENT, 1000));

    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    float previous_time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        switch (swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index))
        {
        case Turbo::Core::TResult::SUCCESS: {

            Turbo::Core::TViewport viewport(0, 0, swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor scissor(0, 0, swapchain->GetWidth(), swapchain->GetHeight());

            Turbo::Core::TImageView *current_swapchain_image_view = swapchain_image_views[current_image_index];
            Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, current_swapchain_image_view);
            command_buffer->CmdClearColorImage(current_swapchain_image_view, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, (std::sin(current_time) + 1) * 0.5, (std::cos(current_time) + 1) * 0.5, (std::cos(0.5 * current_time) + 1) * 0.5, 1);
            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, current_swapchain_image_view);

            imgui_pass->Draw(delta_time, command_buffer, current_swapchain_image_view, [&]() {
                static bool is_show_demo_window = true;

                if (is_show_demo_window)
                    ImGui::ShowDemoWindow(&is_show_demo_window);

                static float f = 0.0f;
                static int counter = 0;
                static float value = 1.0f;

                ImGui::Begin(TURBO_PROJECT_NAME);

                ImGui::Text("This is some useful text.");

                ImGui::SliderFloat("value", &value, 0.0f, 1.0f);

                if (ImGui::Button("Button"))
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Checkbox("show demo window", &is_show_demo_window);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            });

            command_buffer->End();
            {
                Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
                queue->Submit(command_buffer, fence);
                fence->WaitUntil();
                command_pool->Free(command_buffer);
            }
            command_pool->Free(command_buffer);
            Turbo::Core::TResult present_result = queue->Present(swapchain, current_image_index);
        }
        break;
        case Turbo::Core::TResult::TIMEOUT: {
        }
        break;
        case Turbo::Core::TResult::NOT_READY: {
        }
        break;
        case Turbo::Core::TResult::MISMATCH: {
            device->WaitIdle();
            swapchain = new Turbo::Extension::TSwapchain(swapchain);
            swapchain_image_views.clear();
            for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain->GetImages())
            {
                Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, swapchain_image_format, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }
        }
        break;
        default: {
            //
        }
        break;
        }
    }

    glfwTerminate();
    return 0;
}