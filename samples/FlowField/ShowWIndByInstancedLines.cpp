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
#include <GlobalWind.h>
#include <InfiniteCoordinateAxisPass.h>

#include <glm/ext.hpp>

std::string asset_root(TURBO_ASSET_ROOT);
class GlobalWindInstancedLinesPass : public Turbo::Core::TObject
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline;

    Turbo::Core::TRefPtr<Turbo::Core::TImage> particalImage;
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> particalImageView;

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipelineDescriptorSet;

    uint32_t particalImageWidth = 1024;
    uint32_t particalImageHeight = 1024;

    struct MyPushConstants
    {
        float time = 0;
        float deltaTime = 0;
        float speed = 1;
        bool isEarth = false;
        float radius = 1000.0;
    };
    MyPushConstants myPushConstants;

    bool isDraw = true;

  private:
    void Init(Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TDescriptorPool *descriptorPool, Camera *camera, Turbo::Core::TImageView *flowFieldImageView, Turbo::Core::TSampler *sampler)
    {
        if (Turbo::Core::TReferenced::Valid(commandBuffer) && Turbo::Core::TReferenced::Valid(descriptorPool) && Turbo::Core::TReferenced::Valid(camera) && Turbo::Core::TReferenced::Valid(flowFieldImageView) && Turbo::Core::TReferenced::Valid(sampler))
        {
            auto command_pool = commandBuffer->GetCommandBufferPool();
            auto queue = command_pool->GetDeviceQueue();
            auto device = queue->GetDevice();
            auto render_pass = commandBuffer->GetCurrentRenderPass();

            auto vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/ShowWindByInstancedPoints.vert"));
            auto geometry_shader = new Turbo::Core::TGeometryShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/ShowWIndByInstancedLines.geom"));
            auto fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/ShowWindByInstancedPoints.frag"));
            this->pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, {}, vertex_shader, geometry_shader, fragment_shader, Turbo::Core::TTopologyType::POINT_LIST, false, false, false, Turbo::Core::TPolygonMode::POINT, Turbo::Core::TCullModeBits::MODE_NONE, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

            {
                this->particalImage = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, this->particalImageWidth, this->particalImageHeight, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_STORAGE, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
                this->particalImageView = new Turbo::Core::TImageView(this->particalImage, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, this->particalImage->GetFormat().GetFormatType(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT /*ASPECT_METADATA_BIT*/, 0, 1, 0, 1);
                {
                    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();
                    command_buffer->Begin();
                    command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, this->particalImageView);
                    command_buffer->CmdClearColorImage(this->particalImageView, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, 0, 0, 0);
                    command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::BOTTOM_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::GENERAL, this->particalImageView);
                    command_buffer->End();

                    Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
                    queue->Submit(command_buffer, fence);
                    fence->WaitUntil();
                    command_pool->Free(command_buffer);
                }
            }

            this->pipelineDescriptorSet = descriptorPool->Allocate(this->pipeline->GetPipelineLayout());
            this->pipelineDescriptorSet->BindData(0, 0, camera->GetViewProjectionBuffer());
            this->pipelineDescriptorSet->BindData(0, 1, std::make_pair(flowFieldImageView, sampler));
            this->pipelineDescriptorSet->BindData(0, 2, this->particalImageView);
        }
    }

  public:
    void Draw(Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TDescriptorPool *descriptorPool, Camera *camera, Turbo::Core::TImageView *flowFieldImageView, Turbo::Core::TSampler *sampler)
    {
        if (!this->pipeline.Valid())
        {
            this->Init(commandBuffer, descriptorPool, camera, flowFieldImageView, sampler);
        }

        if (this->isDraw)
        {
            if (ImGui::GetCurrentContext() != nullptr)
            {
                float delta_time = ImGui::GetIO().DeltaTime;
                this->myPushConstants.time += delta_time;
                this->myPushConstants.deltaTime = delta_time;
            }

            commandBuffer->CmdBindPipeline(this->pipeline);
            commandBuffer->CmdPushConstants(0, sizeof(this->myPushConstants), &(this->myPushConstants));
            commandBuffer->CmdBindPipelineDescriptorSet(this->pipelineDescriptorSet);
            commandBuffer->CmdDraw(1, this->particalImageWidth * this->particalImageHeight, 0, 0);
        }
    }

    void OnGUI()
    {
        ImGui::Begin(this->ToString().c_str());
        ImGui::SliderFloat("speed", &(this->myPushConstants.speed), 0, 1);
        ImGui::Checkbox("Is earth", &(this->myPushConstants.isEarth));
        if (this->myPushConstants.isEarth)
        {
            ImGui::SliderFloat("radius", &(this->myPushConstants.radius), 0, 1000.0 /*6378137.0*/);
        }
        ImGui::End();
    }

    void SetIsDraw(bool isDraw)
    {
        this->isDraw = isDraw;
    }

  public:
    virtual std::string ToString() const
    {
        return "GlobalWindInstancedPointsPass";
    }
};

class GlobalWindPatchPass : public Turbo::Core::TObject
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> pipeline;

    Turbo::Core::TRefPtr<Turbo::Core::TImage> flowFieldImage;
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> flowFieldImageView;
    Turbo::Core::TRefPtr<Turbo::Core::TSampler> sampler;

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipelineDescriptorSet;

    bool isDraw = true;

  private:
    void Init(Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TDescriptorPool *descriptorPool, Camera *camera)
    {
        if (Turbo::Core::TReferenced::Valid(commandBuffer) && Turbo::Core::TReferenced::Valid(descriptorPool) && Turbo::Core::TReferenced::Valid(camera))
        {
            auto command_pool = commandBuffer->GetCommandBufferPool();
            auto queue = command_pool->GetDeviceQueue();
            auto device = queue->GetDevice();
            auto render_pass = commandBuffer->GetCurrentRenderPass();

            Turbo::Core::TVertexShader *vertexShader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/Patch.vert"));
            Turbo::Core::TFragmentShader *fragmentShader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, ReadTextFile(std::string(TURBO_ASSET_ROOT) + "/shaders/Patch.frag"));
            this->pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, {}, vertexShader, fragmentShader, Turbo::Core::TTopologyType::TRIANGLE_STRIP, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_NONE, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

            auto load_flow_field_to_image = [&]() -> Turbo::Core::TImage * {
                GlobalWind gw(asset_root + "/global_wind.bin");
                auto width = gw.Width();
                auto height = gw.Height();

                std::cout << "height=" << height << std::endl;
                std::cout << "width=" << width << std::endl;

                size_t flow_field_size = gw.Load();
                Turbo::Core::TRefPtr<Turbo::Core::TBuffer> flow_field_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, flow_field_size);
                {
                    gw.Load(flow_field_buffer->Map());
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

            this->flowFieldImage = load_flow_field_to_image();
            this->flowFieldImageView = new Turbo::Core::TImageView(this->flowFieldImage, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, this->flowFieldImage->GetFormat().GetFormatType(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            this->sampler = new Turbo::Core::TSampler(device, Turbo::Core::TFilter::LINEAR, Turbo::Core::TFilter::LINEAR, Turbo::Core::TMipmapMode::LINEAR, Turbo::Core::TAddressMode::MIRRORED_REPEAT, Turbo::Core::TAddressMode::MIRRORED_REPEAT, Turbo::Core::TAddressMode::MIRRORED_REPEAT, Turbo::Core::TBorderColor::FLOAT_OPAQUE_WHITE, 0.0f, 0.0f, 1.0f);

            this->pipelineDescriptorSet = descriptorPool->Allocate(this->pipeline->GetPipelineLayout());
            this->pipelineDescriptorSet->BindData(0, 0, camera->GetViewProjectionBuffer());
            this->pipelineDescriptorSet->BindData(0, 1, this->flowFieldImageView);
            this->pipelineDescriptorSet->BindData(0, 2, sampler);
        }
    }

  public:
    void Draw(Turbo::Core::TCommandBuffer *commandBuffer, Turbo::Core::TDescriptorPool *descriptorPool, Camera *camera)
    {
        if (!this->pipeline.Valid())
        {
            this->Init(commandBuffer, descriptorPool, camera);
        }

        if (this->isDraw)
        {
            commandBuffer->CmdBindPipeline(this->pipeline);
            commandBuffer->CmdBindPipelineDescriptorSet(this->pipelineDescriptorSet);
            commandBuffer->CmdDraw(4, 1, 0, 0);
        }
    }

    Turbo::Core::TImageView *GetFlowFieldImageView()
    {
        return this->flowFieldImageView;
    }

    Turbo::Core::TSampler *GetSampler()
    {
        return this->sampler;
    }

    void SetIsDraw(bool isDraw)
    {
        this->isDraw = isDraw;
    }

  public:
    virtual std::string ToString() const
    {
        return "GlobalWindPatchPass";
    }
};

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
    Turbo::Core::TRefPtr<GlobalWindPatchPass> global_wind_patch_pass = new GlobalWindPatchPass();
    Turbo::Core::TRefPtr<GlobalWindInstancedLinesPass> global_wind_instanced_lines_pass = new GlobalWindInstancedLinesPass();

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
            global_wind_patch_pass->Draw(command_buffer, descriptor_pool, camera);
            global_wind_instanced_lines_pass->Draw(command_buffer, descriptor_pool, camera, global_wind_patch_pass->GetFlowFieldImageView(), global_wind_patch_pass->GetSampler());

            command_buffer->CmdEndRenderPass();

            imgui_pass->Draw(delta_time, command_buffer, current_swapchain_image_view, [&]() {
                static float view_speed = 1.0f;

                static bool is_draw_global_wind_patch_pass = true;
                static bool is_draw_global_wind_instanced_points_pass = true;

                ImGui::Begin(TURBO_PROJECT_NAME);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                if (ImGui::SliderFloat("view speed", &view_speed, 1.0f, 100.0f))
                {
                    camera->SetSpeed(view_speed);
                }

                ImGui::Text("- W,A,S,D to move.");
                ImGui::Text("- Push down and drag mouse right button to rotate view.");
                ImGui::Text("- Mouse wheel forward and backward view.");

                if (ImGui::Checkbox("Is draw GlobalWindPatchPass", &is_draw_global_wind_patch_pass))
                {
                    global_wind_patch_pass->SetIsDraw(is_draw_global_wind_patch_pass);
                }

                if (ImGui::Checkbox("Is draw GlobalWindInstancedPointsPass", &is_draw_global_wind_instanced_points_pass))
                {
                    global_wind_instanced_lines_pass->SetIsDraw(is_draw_global_wind_instanced_points_pass);
                }

                ImGui::End();

                if (is_draw_global_wind_instanced_points_pass)
                {
                    global_wind_instanced_lines_pass->OnGUI();
                }
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