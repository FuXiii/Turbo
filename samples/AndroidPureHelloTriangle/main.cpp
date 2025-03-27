#include <jni.h>

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>
#include <android/log.h>

extern "C"
{
#include <game-activity/native_app_glue/android_native_app_glue.c>
}

#include <vulkan/vulkan.h>
#include <dlfcn.h> //读取Vulkan动态库的头文件和Linux是一样的

#if defined(ANDROID) || defined(__ANDROID__)
#define TURBO_ANDROID_PLATFORM
#endif

#if defined(TURBO_ANDROID_PLATFORM)
#define ANDROID_IS_ANDROID_STR "Is ANDROID"
#else
#define ANDROID_IS_ANDROID_STR "Not ANDROID"
#endif

#include "./core/include/TEngine.h"
#include "./core/include/TVulkanLoader.h"

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

#include "core/include/TSurface.h"
#include "core/include/TSwapchain.h"

#include <math.h>

#include "core/include/TPipelineDescriptorSet.h"
#include "core/include/TSampler.h"

#include "core/include/TVulkanLoader.h"

#include <chrono>

const std::string VERT_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 0) uniform bufferVals {\n"
                                    "    float value;\n"
                                    "} myBufferVals;\n"
                                    "layout(push_constant) uniform my_push_constants_vertex\n"
                                    "{\n"
                                    "   float value;"
                                    "}my_push_constants_data;\n"
                                    "layout (location = 0) in vec3 pos;\n"
                                    "layout (location = 1) in vec3 color;"
                                    "layout (location = 2) out vec3 outColor;"
                                    "layout (location = 3) out float outValue;\n"
                                    "void main() {\n"
                                    "   gl_Position = vec4(pos * my_push_constants_data.value,1);\n"
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

float value = -10.0f;
std::vector<POSITION_AND_COLOR> POSITION_AND_COLOR_DATA;

Turbo::Core::TInstance *instance = nullptr;
Turbo::Core::TPhysicalDevice *physical_device = nullptr;
Turbo::Core::TDevice *device = nullptr;
Turbo::Core::TDeviceQueue *queue = nullptr;
Turbo::Extension::TSurface *surface = nullptr;
Turbo::Extension::TSwapchain *swapchain = nullptr;
std::vector<Turbo::Core::TImage *> swapchain_images;
std::vector<Turbo::Core::TImageView *> swapchain_image_views;
Turbo::Core::TCommandBufferPool *command_pool = nullptr;
Turbo::Core::TCommandBuffer *command_buffer = nullptr;

Turbo::Core::TBuffer *value_buffer = nullptr;
Turbo::Core::TBuffer *vertex_buffer = nullptr;

Turbo::Core::TImage *depth_image = nullptr;
Turbo::Core::TImageView *depth_image_view = nullptr;

Turbo::Core::TVertexShader *vertex_shader = nullptr;
Turbo::Core::TFragmentShader *fragment_shader = nullptr;

Turbo::Core::TDescriptorPool *descriptor_pool = nullptr;

Turbo::Core::TRenderPass *render_pass = nullptr;

Turbo::Core::TGraphicsPipeline *pipeline = nullptr;

std::vector<Turbo::Core::TBuffer *> vertex_buffers;

Turbo::Core::TPipelineDescriptorSet *pipeline_descriptor_set = nullptr;

std::vector<Turbo::Core::TFramebuffer *> swpachain_framebuffers;

/*!
 * Handles commands sent to this Android application
 * @param pApp the app the commands are coming from
 * @param cmd the command to handle
 */
void handle_cmd(android_app *pApp, int32_t cmd)
{
    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW: //该窗体被打开
    {
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
        // std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions = Turbo::Core::TExtensionInfo::GetInstanceExtensions(nullptr);
        for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
        {
            if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
            {
                enable_instance_extensions.push_back(extension);
            }
            else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_ANDROID_SURFACE)
            {
                enable_instance_extensions.push_back(extension);
            }
        }

        Turbo::Core::TVersion instance_version(1, 0, 0, 0);
        instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);

        physical_device = instance->GetBestPhysicalDevice();
        LOGI("Physical Device: %s:", physical_device->GetDeviceName().c_str());

        Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
        physical_device_features.sampleRateShading = true;

        std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
        physical_device->GetSupportExtensions();
        std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
        for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
        {
            if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
            {
                enable_device_extensions.push_back(extension);
            }
        }

        device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
        queue = device->GetBestGraphicsQueue();

        surface = new Turbo::Extension::TSurface(device, pApp->window);

        Turbo::Extension::TCompositeAlphas surface_support_composite_alphas = surface->GetSupportedCompositeAlpha();
        LOGI("Surface--SupportedCompositeAlpha: %x:", surface_support_composite_alphas);

        uint32_t max_image_count = surface->GetMaxImageCount();
        uint32_t min_image_count = surface->GetMinImageCount();

        uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;
        swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::/*B8G8R8A8_SRGB*/ R8G8B8A8_UNORM, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

        swapchain_images = swapchain->GetImages();

        for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
        {
            Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::/*B8G8R8A8_SRGB*/ R8G8B8A8_UNORM, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            swapchain_image_views.push_back(swapchain_view);
        }

        command_pool = new Turbo::Core::TCommandBufferPool(queue);
        command_buffer = command_pool->Allocate();

        value_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
        void *value_ptr = value_buffer->Map();
        memcpy(value_ptr, &value, sizeof(value));
        value_buffer->Unmap();

        vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
        void *vertx_buffer_ptr = vertex_buffer->Map();
        memcpy(vertx_buffer_ptr, POSITION_AND_COLOR_DATA.data(), sizeof(POSITION_AND_COLOR) * POSITION_AND_COLOR_DATA.size());
        vertex_buffer->Unmap();
        POSITION_AND_COLOR_DATA.clear();

        depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
        depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

        vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
        fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

        LOGI("Vertex Shader Code: %s:", vertex_shader->ToString().c_str());
        LOGI("Fragment Shader Code: %s:", fragment_shader->ToString().c_str());

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

        descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

        std::vector<Turbo::Core::TBuffer *> buffers;
        buffers.push_back(value_buffer);

        Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
        subpass.AddColorAttachmentReference(0,
                                            Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL); // swapchain color image
        subpass.SetDepthStencilAttachmentReference(1,
                                                   Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

        std::vector<Turbo::Core::TSubpass> subpasses;
        subpasses.push_back(subpass); // subpass 0

        Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
        Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        std::vector<Turbo::Core::TAttachment> attachemts;
        attachemts.push_back(swapchain_color_attachment);
        attachemts.push_back(depth_attachment);

        render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

        Turbo::Core::TVertexBinding vertex_binding(0, sizeof(POSITION_AND_COLOR), Turbo::Core::TVertexRate::VERTEX);
        vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_AND_COLOR, position)); // position
        vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32_SFLOAT, offsetof(POSITION_AND_COLOR, color));    // color

        std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
        vertex_bindings.push_back(vertex_binding);

        Turbo::Core::TViewport viewport(0, 0, 500, 500, 0, 1);
        Turbo::Core::TScissor scissor(0, 0, 500, 500);

        pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, vertex_shader, fragment_shader, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false);

        pipeline_descriptor_set = descriptor_pool->Allocate(pipeline->GetPipelineLayout());
        pipeline_descriptor_set->BindData(0, 0, 0, buffers);

        vertex_buffers.push_back(vertex_buffer);

        for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
        {
            std::vector<Turbo::Core::TImageView *> image_views;
            image_views.push_back(swapchain_image_view_item);
            image_views.push_back(depth_image_view);

            Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
            swpachain_framebuffers.push_back(swapchain_framebuffer);
        }
    }
    break;
    case APP_CMD_TERM_WINDOW: //窗体被隐藏或是关闭了
    {
        LOGE("handle_cmd::APP_CMD_TERM_WINDOW");
    }
    break;
    default:
        break;
    }
}

/*!
 * Enable the motion events you want to handle; not handled events are
 * passed back to OS for further processing. For this example case,
 * only pointer and joystick devices are enabled.
 *
 * @param motionEvent the newly arrived GameActivityMotionEvent.
 * @return true if the event is from a pointer or joystick device,
 *         false for all other input devices.
 */
bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent)
{
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER || sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

/*!
 * This the main entry point for a native activity
 */
void android_main(struct android_app *pApp)
{
    // Can be removed, useful to ensure your code is running
    LOGI("Welcome to android_main");
    LOGI(ANDROID_IS_ANDROID_STR);
    // LOGI("Vulkan Version: %s:",Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString().c_str());

    // Register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    // Set input event filters (set it to NULL if the app wants to process all inputs).
    // Note that for key inputs, this example uses the default default_key_filter()
    // implemented in android_native_app_glue.c.
    android_app_set_motion_event_filter(pApp, motion_event_filter_func);

    POSITION_AND_COLOR_DATA.push_back(POSITION_AND_COLOR{{0.0f, -0.5f, 0.0f}, {1.f, 0.f, 0.f}});
    POSITION_AND_COLOR_DATA.push_back(POSITION_AND_COLOR{{0.5f, 0.5f, 0.0f}, {0.f, 1.f, 0.f}});
    POSITION_AND_COLOR_DATA.push_back(POSITION_AND_COLOR{{-0.5f, 0.5f, 0.0f}, {0.f, 0.f, 1.f}});

    // This sets up a typical game/event loop. It will run until the app is destroyed.
    int events;
    android_poll_source *pSource;
    do
    {
        // Process all pending events before running game logic.
        if (ALooper_pollAll(0, nullptr, &events, (void **)&pSource) >= 0)
        {
            if (pSource)
            {
                pSource->process(pApp, pSource);
            }
        }

        // Check if any user data is associated. This is assigned in handle_cmd
        if (surface != nullptr)
        {

            {
                static float __value = 0;
                __value += 0.01;
                value = (sin(__value) + 1) * 0.5;
                // value = 1;
            }
            LOGI("value:%f", value);

            void *_ptr = value_buffer->Map();
            memcpy(_ptr, &value, sizeof(value));
            value_buffer->Unmap();

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
                command_buffer->CmdPushConstants(0, sizeof(float), &value);
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
                        Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::/*B8G8R8A8_SRGB*/ R8G8B8A8_UNORM, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                        swapchain_image_views.push_back(swapchain_view);
                    }

                    // recreate depth image and view
                    depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                    depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                    // recreate framebuffer
                    for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
                    {
                        std::vector<Turbo::Core::TImageView *> image_views;
                        image_views.push_back(image_view_item);
                        image_views.push_back(depth_image_view);

                        Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
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
                    Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::/*B8G8R8A8_SRGB*/ R8G8B8A8_UNORM, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TImageView *> image_views;
                    image_views.push_back(image_view_item);
                    image_views.push_back(depth_image_view);

                    Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
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
        }
    } while (!pApp->destroyRequested);

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
    delete device;
    delete instance;
}
