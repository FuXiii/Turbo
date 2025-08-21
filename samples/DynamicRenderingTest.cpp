#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TEngine.h>
#include <TPhysicalDevice.h>
#include <TVulkanAllocator.h>

#include <TBuffer.h>
#include <TCommandBuffer.h>
#include <TCommandBufferPool.h>
#include <TImage.h>
#include <TImageView.h>

#include <TShader.h>

#include <TAttachment.h>
#include <TGraphicsPipeline.h>
#include <TRenderPass.h>
#include <TRenderingPipeline.h>
#include <TSubpass.h>

#include <TDescriptorPool.h>
#include <TDescriptorSet.h>
#include <TDescriptorSetLayout.h>
#include <TFramebuffer.h>

#include <TFence.h>
#include <TSemaphore.h>

#include <GLFW/glfw3.h>

#include <TSurface.h>
#include <TSwapchain.h>

#include <TPipelineDescriptorSet.h>

#include <TVulkanLoader.h>

#include <stdio.h>
#include <string.h>

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

    Turbo::Core::TVersion support_instance_version = Turbo::Core::TInstance::GetVulkanInstanceVersion();
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &support_instance_version);
    Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> physical_device = instance->GetBestPhysicalDevice();

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

    VkPhysicalDeviceFeatures vk_physical_device_features = {};
    vk_physical_device_features.sampleRateShading = VK_TRUE;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    if (instance->GetVulkanVersion() < Turbo::Core::TVersion(1, 3, 0, 0))
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Not Support Vulkan Dynamic Rendering");
    }

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.dynamicRendering = true;

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

    Turbo::Core::TRefPtr<Turbo::Extension::TSurface> surface = new Turbo::Extension::TSurface(device, nullptr, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();

    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> value_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    void *value_ptr = value_buffer->Map();
    memcpy(value_ptr, &value, sizeof(value));
    value_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
    void *vertx_buffer_ptr = vertex_buffer->Map();
    memcpy(vertx_buffer_ptr, POSITION_AND_COLOR_DATA.data(), sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
    vertex_buffer->Unmap();
    POSITION_AND_COLOR_DATA.clear();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    std::cout << vertex_shader->ToString() << std::endl;
    std::cout << fragment_shader->ToString() << std::endl;

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

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> buffers;
    buffers.push_back(value_buffer);

    Turbo::Core::TVertexBinding vertex_binding(0, sizeof(POSITION_AND_COLOR), Turbo::Core::TVertexRate::VERTEX);
    vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_AND_COLOR, position)); // position
    vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_AND_COLOR, color));    // color

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
    vertex_bindings.push_back(vertex_binding);

    Turbo::Core::TViewport viewport(0, 0, 500, 500, 0, 1);
    Turbo::Core::TScissor scissor(0, 0, 500, 500);

    // TODO: we need to create Dynamic Rendering Pipeline
    Turbo::Core::TAttachmentsFormat rendering_attachments = {};
    rendering_attachments.AddColorAttachmentFormat(swapchain_image_views[0]->GetFormat().GetFormatType());
    rendering_attachments.SetDepthAttachmentFormat(depth_image_view->GetFormat().GetFormatType());

    Turbo::Core::TPipelineLayout::TLayout rendering_pipeline_layout;
    rendering_pipeline_layout << *vertex_shader << *fragment_shader;
    Turbo::Core::TRefPtr<Turbo::Core::TVertexShaderStage> vertex_shader_stage = new Turbo::Core::TVertexShaderStage(vertex_shader);
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShaderStage> fragment_shader_stage = new Turbo::Core::TFragmentShaderStage(fragment_shader);

    Turbo::Core::TRefPtr<Turbo::Core::TRenderingPipeline> rendering_pipeline = new Turbo::Core::TRenderingPipeline(rendering_pipeline_layout, rendering_attachments, vertex_bindings, vertex_shader_stage, fragment_shader_stage, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false);

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipeline_descriptor_set = descriptor_pool->Allocate(rendering_pipeline->GetPipelineLayout());
    pipeline_descriptor_set->BindData(0, 0, 0, buffers);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> vertex_buffers;
    vertex_buffers.push_back(vertex_buffer);

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
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

            // because we just have one command buffer, so we should reset the command buffer for each frame
            // If we create command buffer for each swapchain image, we don't need to reset it each frame

            Turbo::Core::TRefPtr<Turbo::Core::TImageView> current_image_view = swapchain_image_views[current_image_index];

            Turbo::Core::TViewport frame_viewport(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor frame_scissor(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight());

            std::vector<Turbo::Core::TViewport> frame_viewports;
            frame_viewports.push_back(frame_viewport);

            std::vector<Turbo::Core::TScissor> frame_scissors;
            frame_scissors.push_back(frame_scissor);

            command_buffer->Begin();

            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, current_image_view);
            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_ATTACHMENT_OPTIMAL, depth_image_view);

            Turbo::Core::TRenderingAttachments rendering_attachments;
            rendering_attachments.AddColorAttachment(current_image_view, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE);
            rendering_attachments.SetDepthAttachment(depth_image_view, Turbo::Core::TImageLayout::DEPTH_ATTACHMENT_OPTIMAL, Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE);

            command_buffer->CmdBeginRendering(rendering_attachments);

            // Triangle
            command_buffer->CmdBindPipeline(rendering_pipeline);
            command_buffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);
            command_buffer->CmdBindVertexBuffers(vertex_buffers);
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            command_buffer->CmdDraw(3, 1, 0, 0);

            command_buffer->CmdEndRendering();

            command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL, Turbo::Core::TImageLayout::PRESENT_SRC_KHR, current_image_view);

            command_buffer->End();

            Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSemaphore>> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(wait_semaphores, {}, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

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
                swapchain_image_views.clear();

                // destroy depth image and view
                // recreate swapchain
                Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(swapchain);
                swapchain = new_swapchain;

                // recreate swapchain image views
                swapchain_images = swapchain->GetImages();
                for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
                {
                    Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);
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
            swapchain_image_views.clear();

            // destroy depth image and view
            // recreate swapchain
            Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(swapchain);
            swapchain = new_swapchain;

            // recreate swapchain image views
            swapchain_images = swapchain->GetImages();
            for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
            {
                Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);
        }
        break;
        default: {
            //
        }
        break;
        }
        //</End Rendering>
    }

    descriptor_pool->Free(pipeline_descriptor_set);

    command_pool->Free(command_buffer);

    glfwTerminate();

    return 0;
}