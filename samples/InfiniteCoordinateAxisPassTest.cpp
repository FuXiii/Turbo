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
#include <TViewport.h>
#include <TScissor.h>

#include <ImGuiPass.h>
#include <Camera.h>
#include <InfiniteCoordinateAxisPass.h>

#include <glm/ext.hpp>

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

    Turbo::Core::TRefPtr<Camera> camera = new Camera(device);
    Turbo::Core::TRefPtr<InfiniteCoordinateAxisPass> infinite_coordinate_axis_pass = new InfiniteCoordinateAxisPass();

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

    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    // Init--------------------------------------------------------------------------------
    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TAttachment color_attachment(swapchain_image_views.front()->GetFormat(), Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_ATTACHMENT_OPTIMAL);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::ATTACHMENT_OPTIMAL);
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachments;
    {
        attachments.push_back(color_attachment);
        attachments.push_back(depth_attachment);
    }

    std::vector<Turbo::Core::TSubpass> subpasses;
    {
        subpasses.push_back(subpass);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> render_pass = new Turbo::Core::TRenderPass(device, attachments, subpasses);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer>> framebuffers;
    for (auto image_view : swapchain_image_views)
    {
        std::vector<Turbo::Core::TImageView *> attachments;
        attachments.push_back(image_view);
        attachments.push_back(depth_image_view);

        Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> framebuffer = new Turbo::Core::TFramebuffer(render_pass, attachments);
        framebuffers.push_back(framebuffer);
    }

    float previous_time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float current_time = glfwGetTime();
        float delta_time = current_time - previous_time;
        previous_time = current_time;

        auto Update = [&]() {
            {
                camera->Update();
            }
        };
        Update();

        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        switch (swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index))
        {
        case Turbo::Core::TResult::SUCCESS: {

            Turbo::Core::TViewport viewport(0, 0, swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor scissor(0, 0, viewport.GetWidth(), viewport.GetHeight());

            Turbo::Core::TImageView *current_swapchain_image_view = swapchain_image_views[current_image_index];
            Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();
            command_buffer->Begin();
            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, current_swapchain_image_view);
            command_buffer->CmdClearColorImage(current_swapchain_image_view, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, 0, 0, 1);
            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, current_swapchain_image_view);

            command_buffer->CmdBeginRenderPass(render_pass, framebuffers[current_image_index]);
            command_buffer->CmdSetViewport(viewport);
            command_buffer->CmdSetScissor(scissor);

            infinite_coordinate_axis_pass->Draw(command_buffer, camera);

            command_buffer->CmdEndRenderPass();

            imgui_pass->Draw(delta_time, command_buffer, current_swapchain_image_view, [&]() {
                static bool is_show_demo_window = true;

                if (is_show_demo_window)
                    ImGui::ShowDemoWindow(&is_show_demo_window);

                static float f = 0.0f;
                static int counter = 0;
                static float value = 1.0f;
                static float view_speed = 1.0f;

                ImGui::Begin(TURBO_PROJECT_NAME);

                ImGui::Text("This is some useful text.");

                ImGui::SliderFloat("value", &value, 0.0f, 1.0f);
                if (ImGui::SliderFloat("view speed", &view_speed, 1.0f, 100.0f))
                {
                    camera->SetSpeed(view_speed);
                }

                if (ImGui::Button("Button"))
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Checkbox("show demo window", &is_show_demo_window);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::Text("- W,A,S,D to move.");
                ImGui::Text("- Push down and drag mouse right button to rotate view.");
                ImGui::Text("- Mouse wheel forward and backward view.");

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

            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);

            framebuffers.clear();
            for (auto image_view : swapchain_image_views)
            {
                std::vector<Turbo::Core::TImageView *> attachments;
                attachments.push_back(image_view);
                attachments.push_back(depth_image_view);

                Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> framebuffer = new Turbo::Core::TFramebuffer(render_pass, attachments);
                framebuffers.push_back(framebuffer);
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