#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TEngine.h"
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

#include <fstream>

#include <SDL.h>
#include <SDL_vulkan.h>

#include "TSurface.h"
#include "TSwapchain.h"

#include <math.h>

#undef main

struct Vertex
{
    float posX, posY, posZ, posW; // Position data
    float r, g, b, a;             // Color
};

#define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.f
#define RGB(_x_, _y_, _z_) (_x_), (_y_), (_z_)

static const Vertex VERTEXS_DATA[] = {
    {XYZ1(0.5f, 0.5f, 0.0f), RGB(1.f, 0.f, 0.f)},
    {XYZ1(-0.5f, 0.5f, 0.0f), RGB(0.f, 1.f, 0.f)},
    {XYZ1(-0.5f, -0.5f, 0.0f), RGB(0.f, 0.f, 1.f)},
    {XYZ1(0.5f, -0.50f, 0.0f), RGB(1.f, 1.f, 0.f)},
};

std::vector<uint32_t> INDICES_DATA = {0, 1, 2, 2, 3, 0};

const std::string VERT_SHADER_STR = "#version 450 core\n"
                                    "layout (set = 0, binding = 0) uniform bufferVals {\n"
                                    "    float scale;\n"
                                    "} myBufferVals;\n"
                                    "layout (location = 0) in vec4 pos;\n"
                                    "layout (location = 1) in vec4 inColor;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "void main() {\n"
                                    "   outColor = inColor;\n"
                                    "   gl_Position =vec4( myBufferVals.scale * pos.x,myBufferVals.scale * pos.y,myBufferVals.scale * pos.z,1);\n"
                                    "}\n";

const std::string FRAG_SHADER_STR = "#version 450 core\n"
                                    "layout (location = 0) in vec4 color;\n"
                                    "layout (location = 0) out vec4 outColor;\n"
                                    "void main() {\n"
                                    "	outColor = color;\n"
                                    "}\n";

int main()
{
    float scale = 1.0f;

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
    enable_layer.push_back(khronos_validation);

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
    }

    Turbo::Core::TInstance *instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions);
    Turbo::Core::TPhysicalDevice *physical_device = instance->GetBestPhysicalDevice();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window *window = NULL;
    int window_width = 500;
    int window_height = 500;
    window = SDL_CreateWindow("Turbo", 100, 100, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    SDL_Vulkan_CreateSurface(window, vk_instance, &vk_surface_khr);

    VkPhysicalDeviceFeatures vk_physical_device_features = {};
    vk_physical_device_features.sampleRateShading = VK_TRUE;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    physical_device->GetSupportExtensions();
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
            break;
        }
    }

    Turbo::Core::TDevice *device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &vk_physical_device_features);
    Turbo::Core::TDeviceQueue *queue = device->GetBestGraphicsQueue();

    Turbo::Extension::TSurface *surface = new Turbo::Extension::TSurface(device, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();

    Turbo::Extension::TSwapchain *swapchain = new Turbo::Extension::TSwapchain(surface, max_image_count - 1, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TImageView *> swapchain_image_views;
    for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TCommandBufferPool *command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TCommandBuffer *command_buffer = command_pool->Allocate();

    Turbo::Core::TBuffer *scale_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    void *scale_ptr = scale_buffer->Map();
    memcpy(scale_ptr, &scale, sizeof(scale));
    scale_buffer->Unmap();

    scale = 0.5f;

    Turbo::Core::TBuffer *dynamic_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(float));
    scale_ptr = dynamic_buffer->Map();
    memcpy(scale_ptr, &scale, sizeof(scale));
    dynamic_buffer->Unmap();

    Turbo::Core::TBuffer *vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(VERTEXS_DATA));
    void *vertex_buffer_ptr = vertex_buffer->Map();
    memcpy(vertex_buffer_ptr, VERTEXS_DATA, sizeof(VERTEXS_DATA));
    vertex_buffer->Unmap();

    Turbo::Core::TBuffer *index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(INDICES_DATA));
    void *index_buffer_ptr = index_buffer->Map();
    memcpy(index_buffer_ptr, INDICES_DATA.data(), sizeof(INDICES_DATA));
    index_buffer->Unmap();

    Turbo::Core::TImage *depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, 500, 500, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TImageView *depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TShader *vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, VERT_SHADER_STR);
    Turbo::Core::TShader *fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, FRAG_SHADER_STR);

    std::cout << vertex_shader->ToString() << std::endl;
    std::cout << fragment_shader->ToString() << std::endl;

    Turbo::Core::TDescriptorSize uniform_buffer_descriptor_size(Turbo::Core::TDescriptorType::UNIFORM_BUFFER, 100);
    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(uniform_buffer_descriptor_size);

    Turbo::Core::TDescriptorPool *descriptor_pool = new Turbo::Core::TDescriptorPool(device, 100, descriptor_sizes);

    Turbo::Core::TDescriptorSetLayout *descriptor_set_layout = vertex_shader->GetDescriptorSetLayouts()[0];

    Turbo::Core::TDescriptorSet *descriptor_set = descriptor_pool->Allocate(descriptor_set_layout);
    std::vector<Turbo::Core::TBuffer *> buffers;
    buffers.push_back(scale_buffer);
    descriptor_set->BindData(0, 0, buffers);

    Turbo::Core::TDescriptorSet *dynamic_descriptor_set = descriptor_pool->Allocate(descriptor_set_layout);
    std::vector<Turbo::Core::TBuffer *> buffers2;
    buffers2.push_back(dynamic_buffer);
    dynamic_descriptor_set->BindData(0, 0, buffers2);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass); // subpass 1
    subpasses.push_back(subpass); // subpass 2

    Turbo::Core::TAttachment color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRenderPass *render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    Turbo::Core::TVertexBinding vertex_binding(0, sizeof(VERTEXS_DATA[0]), Turbo::Core::TVertexRate::VERTEX);
    vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, 0);  // pos
    vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, 16); // color

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings{vertex_binding};

    Turbo::Core::TViewport viewport(0, 0, 500, 500, 0, 1);
    Turbo::Core::TScissor scissor(0, 0, 500, 500);

    std::vector<Turbo::Core::TViewport> viewports;
    viewports.push_back(viewport);

    std::vector<Turbo::Core::TScissor> scissors;
    scissors.push_back(scissor);

    std::vector<Turbo::Core::TShader *> shaders{vertex_shader, fragment_shader};
    Turbo::Core::TGraphicsPipeline *pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, vertex_bindings, viewports, scissors, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, shaders);
    Turbo::Core::TGraphicsPipeline *pipeline2 = new Turbo::Core::TGraphicsPipeline(render_pass, 1, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, vertex_bindings, viewports, scissors, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, shaders);

    std::vector<Turbo::Core::TDescriptorSet *> descriptor_sets;
    descriptor_sets.push_back(descriptor_set);

    std::vector<Turbo::Core::TDescriptorSet *> descriptor_sets2;
    descriptor_sets2.push_back(dynamic_descriptor_set);

    std::vector<Turbo::Core::TBuffer *> vertex_buffers;
    vertex_buffers.push_back(vertex_buffer);

    std::vector<Turbo::Core::TFramebuffer *> swpachain_framebuffers;
    for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TImageView *> swapchain_image_views;
        swapchain_image_views.push_back(image_view_item);
        swapchain_image_views.push_back(depth_image_view);

        Turbo::Core::TFramebuffer *swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    float _time = 0;
    bool is_window_quit = false;
    SDL_Event e;
    while (!is_window_quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                is_window_quit = true;
            }
        }

        float scale_data = (sin(_time) + 1) / 2.0f;

        void *_ptr = dynamic_buffer->Map();
        memcpy(_ptr, &scale_data, sizeof(scale_data));
        dynamic_buffer->Unmap();

        _time = _time + 0.1f;

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
            command_buffer->Begin();
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);
            command_buffer->CmdBindPipeline(pipeline);
            command_buffer->CmdBindDescriptorSets(0, descriptor_sets);
            command_buffer->CmdBindVertexBuffers(vertex_buffers);
            command_buffer->CmdSetViewport(viewports);
            command_buffer->CmdSetScissor(scissors);
            command_buffer->CmdBindIndexBuffer(index_buffer);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);
            command_buffer->CmdNextSubpass();
            command_buffer->CmdBindPipeline(pipeline2);
            command_buffer->CmdBindDescriptorSets(0, descriptor_sets2);
            command_buffer->CmdDrawIndexed(INDICES_DATA.size(), 1, 0, 0, 0);
            command_buffer->CmdEndRenderPass();
            command_buffer->End();

            Turbo::Core::TFence *fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TSemaphore *> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);
            std::vector<Turbo::Core::TSemaphore *> signal_semaphores;

            queue->Submit(wait_semaphores, signal_semaphores, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

            delete fence;

            command_buffer->Reset(); // you can create an command buffer each for one swapchain image,for now just one command buffer

            queue->Present(swapchain, current_image_index);
        }
        break;
        case Turbo::Core::TResult::TIMEOUT: {
            // you need to wait
        }
        break;
        case Turbo::Core::TResult::NOT_READY: {
            // you need to wait
        }
        break;
        case Turbo::Core::TResult::SUBOPTIMAL: {
            // the size of the window had changed you need to recreate swapchain
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

    {
        std::string save_file_path = "E:/Turbo/";
        std::string save_file_name("VulkanImage");

        Turbo::Core::TImage *source_image = swapchain_images[0];

        Turbo::Core::TImage *temp_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, source_image->GetFormat(), source_image->GetWidth(), source_image->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::LINEAR, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE);

        Turbo::Core::TCommandBuffer *temp_command_buffer = command_pool->Allocate();

        temp_command_buffer->Begin();
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, 0, 0, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, temp_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, 0, 0, Turbo::Core::TImageLayout::PRESENT_SRC_KHR, Turbo::Core::TImageLayout::TRANSFER_SRC_OPTIMAL, source_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        temp_command_buffer->CmdCopyImage(source_image, Turbo::Core::TImageLayout::TRANSFER_SRC_OPTIMAL, temp_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, source_image->GetWidth(), source_image->GetHeight(), 1);
        temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::HOST_BIT, 0, 0, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::GENERAL, temp_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        temp_command_buffer->End();

        Turbo::Core::TFence *gpu_copy_to_cpu_fence = new Turbo::Core::TFence(device);
        std::vector<Turbo::Core::TSemaphore *> gpu_copy_to_cpu_wait_semaphores;
        std::vector<Turbo::Core::TSemaphore *> gpu_copy_to_cpu_signal_semaphores;

        queue->Submit(gpu_copy_to_cpu_wait_semaphores, gpu_copy_to_cpu_signal_semaphores, temp_command_buffer, gpu_copy_to_cpu_fence);

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
        vkGetImageSubresourceLayout(device->GetVkDevice(), temp_image->GetVkImage(), &subres, &sr_layout);

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
        command_pool->Free(temp_command_buffer);
    }

    delete pipeline;
    delete pipeline2;
    for (Turbo::Core::TFramebuffer *framebuffer_item : swpachain_framebuffers)
    {
        delete framebuffer_item;
    }
    delete render_pass;
    descriptor_pool->Free(descriptor_set);
    descriptor_pool->Free(dynamic_descriptor_set);
    delete descriptor_pool;
    delete vertex_shader;
    delete fragment_shader;
    delete depth_image_view;
    delete depth_image;
    for (Turbo::Core::TImageView *image_view_item : swapchain_image_views)
    {
        delete image_view_item;
    }
    delete index_buffer;
    delete vertex_buffer;
    delete scale_buffer;
    delete dynamic_buffer;
    command_pool->Free(command_buffer);
    delete command_pool;
    delete swapchain;
    delete surface;
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete device;
    delete instance;

    return 0;
}
