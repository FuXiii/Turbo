#include "render/include/TBuffer.h"
#include "render/include/TDomain.h"
#include "render/include/TImage.h"
#include "render/include/TShader.h"
#include <chrono>
#include <framegraph/include/TFrameGraph.hpp>
#include <fstream>
#include <iostream>
#include <render/include/TContext.h>
#include <render/include/TPipeline.h>
#include <render/include/TRenderPass.h>
#include <render/include/TResourceAllocator.h>
#include <sstream>
#include <stdint.h>
#include <vector>

void WriteTextFile(const std::string &text, const std::string &filename)
{
    std::ofstream out_stream;
    out_stream.open(filename, std::ios::out | std::ios::trunc);

    if (out_stream.good())
    {
        out_stream << text;
    }

    out_stream.close();
}

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

void Test0()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    {
        // gpu only
        Turbo::Render::TColorImage2D::Descriptor gpu_only_image_2d_descriptor = {};
        gpu_only_image_2d_descriptor.flags = 0;
        gpu_only_image_2d_descriptor.width = 512;
        gpu_only_image_2d_descriptor.height = 512;
        gpu_only_image_2d_descriptor.layers = 1;
        gpu_only_image_2d_descriptor.mipLevels = 1;
        gpu_only_image_2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT;
        gpu_only_image_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TColorImage2D color_image_2d;
        color_image_2d.Create("color_image_2d", gpu_only_image_2d_descriptor, &resource_allocator);
        color_image_2d.Destroy(&resource_allocator);

        std::cout << "======================================== GPU only" << std::endl;

        // staging upload
        Turbo::Render::TColorImage2D::Descriptor staging_image_2d_descriptor = {};
        staging_image_2d_descriptor.flags = 0;
        staging_image_2d_descriptor.width = 512;
        staging_image_2d_descriptor.height = 512;
        staging_image_2d_descriptor.layers = 1;
        staging_image_2d_descriptor.mipLevels = 1;
        staging_image_2d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED | Turbo::Render::TImageUsageBits::TRANSFER_SRC;
        staging_image_2d_descriptor.domain = Turbo::Render::TDomainBits::CPU;

        Turbo::Render::TColorImage2D staging_image_2d;
        staging_image_2d.Create("staging_image_2d", staging_image_2d_descriptor, &resource_allocator);
        staging_image_2d.Destroy(&resource_allocator);

        std::cout << "======================================== staging upload" << std::endl;

        // read back
        Turbo::Render::TColorImage2D::Descriptor read_back_image_2d_descriptor = {};
        read_back_image_2d_descriptor.flags = 0;
        read_back_image_2d_descriptor.width = 512;
        read_back_image_2d_descriptor.height = 512;
        read_back_image_2d_descriptor.layers = 1;
        read_back_image_2d_descriptor.mipLevels = 1;
        read_back_image_2d_descriptor.usages = Turbo::Render::TImageUsageBits::TRANSFER_DST;
        read_back_image_2d_descriptor.domain = Turbo::Render::TDomainBits::CPU;

        Turbo::Render::TColorImage2D read_back_image_2d;
        read_back_image_2d.Create("read_back_image_2d", read_back_image_2d_descriptor, &resource_allocator);
        read_back_image_2d.Destroy(&resource_allocator);

        std::cout << "======================================== read back" << std::endl;

        // advanced upload
        Turbo::Render::TColorImage2D::Descriptor advanced_upload_image_2d_descriptor = {};
        advanced_upload_image_2d_descriptor.flags = 0;
        advanced_upload_image_2d_descriptor.width = 512;
        advanced_upload_image_2d_descriptor.height = 512;
        advanced_upload_image_2d_descriptor.layers = 1;
        advanced_upload_image_2d_descriptor.mipLevels = 1;
        advanced_upload_image_2d_descriptor.usages = Turbo::Render::TImageUsageBits::TRANSFER_DST;
        advanced_upload_image_2d_descriptor.domain = Turbo::Render::TDomainBits::BOTH /*Turbo::Render::TDomainBits::CPU|Turbo::Render::TDomainBits::GPU*/;

        Turbo::Render::TColorImage2D advanced_upload_image_2d;
        advanced_upload_image_2d.Create("read_back_image_2d", advanced_upload_image_2d_descriptor, &resource_allocator);
        advanced_upload_image_2d.Destroy(&resource_allocator);

        std::cout << "======================================== advanced upload" << std::endl;

        Turbo::Render::TDepthImage2D::Descriptor depth_image_2d_descriptor = {};
        depth_image_2d_descriptor.width = 512;
        depth_image_2d_descriptor.height = 512;
        depth_image_2d_descriptor.layers = 1;
        depth_image_2d_descriptor.mipLevels = 1;
        depth_image_2d_descriptor.usages = Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT | Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT;
        depth_image_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TDepthImage2D depth_image_2d;
        depth_image_2d.Create("depth_image_2d", depth_image_2d_descriptor, &resource_allocator);
        depth_image_2d.Destroy(&resource_allocator);

        std::cout << "======================================== depth image" << std::endl;

        Turbo::Render::TTexture2D::Descriptor texture_2d_descriptor = {};
        texture_2d_descriptor.width = 512;
        texture_2d_descriptor.height = 512;
        texture_2d_descriptor.mipLevels = 1;
        texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED;
        texture_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TTexture2D texture_2d;
        texture_2d.Create("texture_2d", texture_2d_descriptor, &resource_allocator);
        texture_2d.Destroy(&resource_allocator);

        std::cout << "======================================== texture2D" << std::endl;

        Turbo::Render::TTexture3D::Descriptor texture_3d_descriptor = {};
        texture_3d_descriptor.width = 512;
        texture_3d_descriptor.height = 512;
        texture_3d_descriptor.depth = 512;
        texture_3d_descriptor.mipLevels = 1;
        texture_3d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED;
        texture_3d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TTexture3D texture_3d;
        texture_3d.Create("texture_3d", texture_3d_descriptor, &resource_allocator);
        texture_3d.Destroy(&resource_allocator);

        std::cout << "======================================== texture3D" << std::endl;

        Turbo::Render::TDepthTexture2D::Descriptor depth_texture_2d_descriptor = {};
        depth_texture_2d_descriptor.width = 512;
        depth_texture_2d_descriptor.height = 512;
        depth_texture_2d_descriptor.mipLevels = 1;
        depth_texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT;
        depth_texture_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TDepthTexture2D depth_texture_2d;
        depth_texture_2d.Create("depth_texture_2d", depth_texture_2d_descriptor, &resource_allocator);
        depth_texture_2d.Destroy(&resource_allocator);

        std::cout << "======================================== depth texture2d" << std::endl;

        Turbo::Render::TBuffer::Descriptor vertex_buffer_cpu_descriptor;
        vertex_buffer_cpu_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_VERTEX_BUFFER;
        vertex_buffer_cpu_descriptor.size = 1024 * 1024;
        vertex_buffer_cpu_descriptor.domain = Turbo::Render::TDomainBits::CPU;

        Turbo::Render::TBuffer vertex_buffer_cpu;
        vertex_buffer_cpu.Create("vertex_buffer_cpu", vertex_buffer_cpu_descriptor, &resource_allocator);
        vertex_buffer_cpu.Destroy(&resource_allocator);

        std::cout << "======================================== vertex buffer cpu" << std::endl;

        Turbo::Render::TBuffer::Descriptor vertex_buffer_gpu_descriptor;
        vertex_buffer_gpu_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_VERTEX_BUFFER;
        vertex_buffer_gpu_descriptor.size = 1024 * 1024;
        vertex_buffer_gpu_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TBuffer vertex_buffer_gpu;
        vertex_buffer_gpu.Create("vertex_buffer_gpu", vertex_buffer_gpu_descriptor, &resource_allocator);
        vertex_buffer_gpu.Destroy(&resource_allocator);

        std::cout << "======================================== vertex buffer gpu" << std::endl;

        uint32_t create_count = 2048;
        std::vector<Turbo::Render::TTexture2D> texture2ds(create_count);
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
        for (uint32_t create_index = 0; create_index < create_count; create_index++)
        {
            Turbo::Render::TTexture2D::Descriptor temp_texture_2d_descriptor = {};
            temp_texture_2d_descriptor.width = 512;
            temp_texture_2d_descriptor.height = 512;
            temp_texture_2d_descriptor.mipLevels = 1;
            temp_texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED;
            temp_texture_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

            Turbo::Render::TTexture2D temp_texture_2d;
            temp_texture_2d.Create("temp_texture_2d", temp_texture_2d_descriptor, &resource_allocator);
            texture2ds[create_index] = temp_texture_2d;
        }
        std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
        std::chrono::duration<double> delta_time = end_time - start_time;
        std::cout << "create " << create_count << " Turbo::Render::TTexture2D use:" << delta_time.count() << "s" << std::endl;

        for (Turbo::Render::TTexture2D &texture2d_item : texture2ds)
        {
            texture2d_item.Destroy(&resource_allocator);
        }
    }

    {
        std::vector<uint32_t> index_datas{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        uint32_t create_buffer_size = index_datas.size() * sizeof(uint32_t);

        Turbo::Render::TBuffer::Descriptor copy_buffer_descriptor = {};
        copy_buffer_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_TRANSFER_DST | Turbo::Render::TBufferUsageBits::BUFFER_INDEX_BUFFER;
        copy_buffer_descriptor.size = create_buffer_size;
        copy_buffer_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TBuffer copy_buffer;
        copy_buffer.Create("copy_buffer", copy_buffer_descriptor, &resource_allocator);
        copy_buffer.Copy(index_datas.data(), create_buffer_size);
        copy_buffer.Destroy(&resource_allocator);

        //

        Turbo::Render::TBuffer::Descriptor copy_temp_buffer_descriptor = {};
        copy_temp_buffer_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_TRANSFER_SRC | Turbo::Render::TBufferUsageBits::BUFFER_INDEX_BUFFER;
        copy_temp_buffer_descriptor.size = create_buffer_size;
        copy_temp_buffer_descriptor.domain = Turbo::Render::TDomainBits::CPU;

        Turbo::Render::TBuffer copy_temp_buffer;
        copy_temp_buffer.Create("copy_temp_buffer", copy_temp_buffer_descriptor, &resource_allocator);
        copy_temp_buffer.Copy(index_datas.data(), create_buffer_size);

        Turbo::Render::TBuffer::Descriptor copy_gpu_buffer_descriptor = {};
        copy_gpu_buffer_descriptor.usages = Turbo::Render::TBufferUsageBits::BUFFER_TRANSFER_DST | Turbo::Render::TBufferUsageBits::BUFFER_INDEX_BUFFER;
        copy_gpu_buffer_descriptor.size = create_buffer_size;
        copy_gpu_buffer_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        Turbo::Render::TBuffer copy_gpu_buffer;
        copy_gpu_buffer.Create("copy_gpu_buffer", copy_gpu_buffer_descriptor, &resource_allocator);
        copy_gpu_buffer.Copy(&copy_temp_buffer, 0, copy_temp_buffer.GetSize());

        copy_temp_buffer.Destroy(&resource_allocator);
        copy_gpu_buffer.Destroy(&resource_allocator);
    }

    {
        Turbo::FrameGraph::TRenderPass fg_render_pass{};
        context.BeginRenderPass(fg_render_pass);
    }

    {
        Turbo::Render::TTexture2D color_texture2d;
        Turbo::Render::TTexture2D::Descriptor color_texture2d_descriptor{};
        color_texture2d_descriptor.width = 1920;
        color_texture2d_descriptor.height = 1080;
        color_texture2d_descriptor.mipLevels = 1;
        color_texture2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT;
        color_texture2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
        color_texture2d.Create("color_texture2d", color_texture2d_descriptor, &resource_allocator);

        Turbo::Render::TTexture2D normal_texture2d;
        Turbo::Render::TTexture2D::Descriptor normal_texture2d_descriptor{};
        normal_texture2d_descriptor.width = 1920;
        normal_texture2d_descriptor.height = 1080;
        normal_texture2d_descriptor.mipLevels = 1;
        normal_texture2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT;
        normal_texture2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
        normal_texture2d.Create("normal_texture2d", normal_texture2d_descriptor, &resource_allocator);

        Turbo::Render::TDepthTexture2D depth_texture2d;
        Turbo::Render::TDepthTexture2D::Descriptor depth_texture2d_descriptor{};
        depth_texture2d_descriptor.width = 1920;
        depth_texture2d_descriptor.height = 1080;
        depth_texture2d_descriptor.mipLevels = 1;
        depth_texture2d_descriptor.usages = Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT;
        depth_texture2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
        depth_texture2d.Create("depth_texture2d", depth_texture2d_descriptor, &resource_allocator);

        Turbo::Render::TSubpass subpass0;
        subpass0.AddColorAttachment(color_texture2d);
        subpass0.AddColorAttachment(normal_texture2d);
        subpass0.SetDepthStencilAttachment(depth_texture2d);

        Turbo::Render::TRenderPass render_pass;
        render_pass.AddSubpass(subpass0);

        context.BeginRenderPass(render_pass);

        depth_texture2d.Destroy(&resource_allocator);
        normal_texture2d.Destroy(&resource_allocator);
        color_texture2d.Destroy(&resource_allocator);
    }

    {
        std::string compute_shader_code = ReadTextFile("../../asset/shaders/perlin-worley.comp");
        Turbo::Render::TComputeShader *compute_shader = nullptr;
        {
            std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
            compute_shader = new Turbo::Render::TComputeShader(&context, Turbo::Render::TShader::TLanguage::GLSL, compute_shader_code);
            std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
            std::chrono::duration<double> delta_time = end_time - start_time;
            std::cout << "create Turbo::Render::TComputeShader compute_shader use:" << delta_time.count() << "s" << std::endl;
        }

        Turbo::Render::TComputePipeline compute_pipeline;
        compute_pipeline.SetComputeShader(compute_shader);

        context.BindPipeline(compute_pipeline);

        std::string vertex_shader_code = ReadTextFile("../../asset/shaders/shader_base.vert");
        Turbo::Render::TVertexShader *vertex_shader = nullptr;
        {
            std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
            vertex_shader = new Turbo::Render::TVertexShader(&context, Turbo::Render::TShader::TLanguage::GLSL, vertex_shader_code);
            std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
            std::chrono::duration<double> delta_time = end_time - start_time;
            std::cout << "create  Turbo::Render::TVertexShader vertex_shader use:" << delta_time.count() << "s" << std::endl;
        }

        std::string fragment_shader_code = ReadTextFile("../../asset/shaders/shader_base.frag");
        Turbo::Render::TFragmentShader *fragment_shader = nullptr;
        {
            std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
            fragment_shader = new Turbo::Render::TFragmentShader(&context, Turbo::Render::TShader::TLanguage::GLSL, fragment_shader_code);
            std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
            std::chrono::duration<double> delta_time = end_time - start_time;
            std::cout << "create  Turbo::Render::TFragmentShader fragment_shader use:" << delta_time.count() << "s" << std::endl;
        }

        Turbo::Render::TGraphicsPipeline graphics_pipeline;
        graphics_pipeline.SetVertexShader(vertex_shader);
        graphics_pipeline.SetFragmentShader(fragment_shader).SetPolygon(Turbo::Render::TGraphicsPipeline::TPolygon::FILL);

        context.BindPipeline(graphics_pipeline);

        delete compute_shader;
        delete vertex_shader;
        delete fragment_shader;
    }

    {
        // 这一部分只是示意性代码

        std::cout << "flush Commandbuffer to GPU" << std::endl;
        Turbo::Render::TGraphicsPipeline some_graphics_pipeline;
        Turbo::Render::TRenderPass some_render_pass;

        // 进行100万批次指令记录
        for (uint32_t some_command = 0; some_command < 1000000; some_command++)
        {
            context.BeginRenderPass(some_render_pass);
            context.BindPipeline(some_graphics_pipeline);
            // context.OtherCmd(....)//其他指令
            // context.Draw(...)//绘制指令
            // context.Dispatch(...)//调度指令
            // context.EndRenderPass(...)
            if (some_command % 2048 == 0)
            {
                context.Flush(); // 每记录2048条记录，将指令推到GPU进行执行
            }
        }

        context.Flush();     // 将剩余指令推到GPU进行执行
        if (context.Wait(1)) // 等待1纳秒，等待之前所有推送到GPU的指令执行完成，返回ture表示所有任务完成，返回false表示有任务在1纳米内没完成
        {
            std::cout << "wait GPU run all commandbuffer finished in 1 nanoseconds" << std::endl;
        }
        else
        {
            context.Wait(UINT64_MAX); // 一直等到所有任务完成
            std::cout << "wait GPU run all commandbuffer finished" << std::endl;
        }
    }
}

#define TUEBO_COMPILE 1

void Test1()
{
#if TUEBO_COMPILE
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);
    /*
    [Color Pass] → Color Texture → [Post Pass] → RenderTarget Texture → [Present Pass]
                 ↘ Depthh Texture ↗
    */
    Turbo::FrameGraph::TFrameGraph fg;

    struct ColorPassData
    {
        Turbo::FrameGraph::TResource colorTexture;
        Turbo::FrameGraph::TResource depthTexture;
    };

    fg.AddPass<ColorPassData>(
        "Color Pass",
        [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, ColorPassData &data) {
            data.colorTexture = builder.Create<Turbo::Render::TTexture2D>("Color Texture2D", {512, 512, 1, Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT | Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT, Turbo::Render::TDomainBits::GPU});
            data.depthTexture = builder.Create<Turbo::Render::TDepthTexture2D>("Depth Texture2D", {512, 512, 1, Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT | Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT, Turbo::Render::TDomainBits::GPU});

            Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass subpass0 = builder.CreateSubpass();
            data.colorTexture = subpass0.Write(data.colorTexture);
            data.depthTexture = subpass0.Write(data.depthTexture);

            fg.GetBlackboard()["ColorTexture"] = data.colorTexture;
            fg.GetBlackboard()["DepthTexture"] = data.depthTexture;
        },
        [=](const ColorPassData &data, const Turbo::FrameGraph::TResources &resources, void *context) {
            // TODO:
            Turbo::FrameGraph::TRenderPass render_pass = resources.GetRenderPass();
            std::vector<Turbo::FrameGraph::TSubpass> subpasses = render_pass.GetSubpasses();

            // 这样使用，略显笨拙，并且容易出现问题，考虑是否有更简便的方法?
            {
                Turbo::FrameGraph::TSubpass subpass0 = subpasses[0];
                std::vector<Turbo::FrameGraph::TResource> input_resources = subpass0.GetInputs();
                std::vector<Turbo::FrameGraph::TResource> read_resources = subpass0.GetReads();
                std::vector<Turbo::FrameGraph::TResource> write_resources = subpass0.GetWrites();

                bool inputs_is_empty = input_resources.size() == 0 ? true : false; // input_resources is empty
                bool reads_is_empty = read_resources.size() == 0 ? true : false;   // read_resources is empty
                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(write_resources[0]);
                Turbo::Render::TDepthTexture2D depth_texture = resources.Get<Turbo::Render::TDepthTexture2D>(write_resources[1]);
            }

            // 简化的宗旨就是尽量不要遍历，而是使用Turbo提供的接口（尽可能的简化）。可能的使用方式：
            {
                Turbo::FrameGraph::TResource color_texture_resource = render_pass.GetSubpass(0).GetWrite(0); // 当用户不小心发生了数组越界，Turbo将会返回一个非法资源句柄
                if (color_texture_resource.IsValid())                                                        // 判断句柄是否合法（有效）
                {
                    Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(color_texture_resource);
                    std::cout << u"color_texture::Width×Height::" << color_texture.GetWidth() << u"×" << color_texture.GetHeight() << std::endl;
                }
            }
        });

    struct PostPassData
    {
        Turbo::FrameGraph::TResource colorTexture;
        Turbo::FrameGraph::TResource depthTexture;
        Turbo::FrameGraph::TResource renderTargetTexture;
    };

    fg.AddPass<PostPassData>(
        "Post Pass",
        [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, PostPassData &data) {
            data.colorTexture = fg.GetBlackboard()["ColorTexture"];
            data.depthTexture = fg.GetBlackboard()["DepthTexture"];
            data.renderTargetTexture = builder.Create<Turbo::Render::TTexture2D>("RenderTarget Texture2D", {512, 512, 1, Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT, Turbo::Render::TDomainBits::GPU});

            Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass subpass0 = builder.CreateSubpass();
            data.colorTexture = subpass0.Read(data.colorTexture);
            data.depthTexture = subpass0.Read(data.depthTexture);
            data.renderTargetTexture = subpass0.Write(data.renderTargetTexture);

            fg.GetBlackboard()["RenderTargetTexture"] = data.renderTargetTexture;
        },
        [=](const PostPassData &data, const Turbo::FrameGraph::TResources &resources, void *context) {
            // TODO:
        });

    struct PresentPassData
    {
        Turbo::FrameGraph::TResource renderTargetTexture;
    };

    fg.AddPass<PresentPassData>(
        "Present Pass",
        [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, PresentPassData &data) {
            data.renderTargetTexture = fg.GetBlackboard()["RenderTargetTexture"];

            Turbo::FrameGraph::TFrameGraph::TBuilder::TSubpass subpass0 = builder.CreateSubpass();
            data.renderTargetTexture = subpass0.Read(data.renderTargetTexture);
        },
        [=](const PresentPassData &data, const Turbo::FrameGraph::TResources &resources, void *context) {
            // TODO:
        });

    fg.Compile();
    std::string mermaid = fg.ToMermaid();

    std::stringstream ss_to_markdown;
    ss_to_markdown << "```mermaid" << std::endl << mermaid << "```" << std::endl;

    WriteTextFile(ss_to_markdown.str(), "./RenderFg.md");

    std::stringstream ss_to_html;
    ss_to_html << "<html>" << std::endl;
    ss_to_html << "<body>" << std::endl;
    ss_to_html << "<script src=\"https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js\"></script>" << std::endl;
    ss_to_html << "<script>mermaid.initialize({startOnLoad:true});</script>" << std::endl;
    ss_to_html << "<div class=\"mermaid\">" << std::endl;
    ss_to_html << mermaid << std::endl;
    ss_to_html << "</div>" << std::endl;
    ss_to_html << "</body>" << std::endl;
    ss_to_html << "</html>" << std::endl;

    WriteTextFile(ss_to_html.str(), "./RenderFg.html");

    fg.Execute(&context, &resource_allocator);
#endif
}

void Test2()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    Turbo::Render::TTexture2D color_texture_2d;
    Turbo::Render::TTexture2D::Descriptor color_texture_2d_descriptor = {};
    color_texture_2d_descriptor.width = 512;
    color_texture_2d_descriptor.height = 512;
    color_texture_2d_descriptor.mipLevels = 1;
    color_texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT;
    color_texture_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    color_texture_2d.Create("color_texture_2d", color_texture_2d_descriptor, &resource_allocator);

    Turbo::Render::TDepthTexture2D depth_texture_2d;
    Turbo::Render::TDepthTexture2D::Descriptor depth_texture2d_descriptor = {};
    depth_texture2d_descriptor.width = 512;
    depth_texture2d_descriptor.height = 512;
    depth_texture2d_descriptor.mipLevels = 1;
    depth_texture2d_descriptor.usages = Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT;
    depth_texture2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    depth_texture_2d.Create("depth_texture_2d", depth_texture2d_descriptor, &resource_allocator);

    Turbo::Render::TSubpass subpass0;
    subpass0.AddColorAttachment(color_texture_2d);
    subpass0.SetDepthStencilAttachment(depth_texture_2d);

    Turbo::Render::TRenderPass render_pass;
    render_pass.AddSubpass(subpass0);

    {
        Turbo::Render::TRenderPassPool render_pass_pool(&context);
        Turbo::Render::TRenderPassPool::TRenderPassProxy render_pass_proxy_0 = render_pass_pool.Allocate(render_pass);
        render_pass_proxy_0.IsValid() ? std::cout << "Allocate RenderPass0 Success" << std::endl : std::cout << "Allocate RenderPass0 Faild" << std::endl;

        Turbo::Render::TRenderPassPool::TRenderPassProxy render_pass_proxy_1 = render_pass_pool.Allocate(render_pass);
        render_pass_proxy_1.IsValid() ? std::cout << "Allocate RenderPass1 Success" << std::endl : std::cout << "Allocate RenderPass1 Faild" << std::endl;

        Turbo::Render::TRenderPassPool::TRenderPassProxy render_pass_proxy_2 = render_pass_pool.Allocate(render_pass);
        render_pass_proxy_2.IsValid() ? std::cout << "Allocate RenderPass2 Success" << std::endl : std::cout << "Allocate RenderPass2 Faild" << std::endl;
    }

    color_texture_2d.Destroy(&resource_allocator);
    depth_texture_2d.Destroy(&resource_allocator);
}

void Test3()
{
    std::cout<<"Test3()::Begin......................................................."<<std::endl;

    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    Turbo::Render::TTexture2D color_texture_2d;
    Turbo::Render::TTexture2D::Descriptor color_texture_2d_descriptor = {};
    color_texture_2d_descriptor.width = 512;
    color_texture_2d_descriptor.height = 512;
    color_texture_2d_descriptor.mipLevels = 1;
    color_texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT;
    color_texture_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    color_texture_2d.Create("color_texture_2d", color_texture_2d_descriptor, &resource_allocator);

    Turbo::Render::TTexture3D color_texture_3d;
    Turbo::Render::TTexture3D::Descriptor color_texture_3d_descriptor = {};
    color_texture_3d_descriptor.width = 512;
    color_texture_3d_descriptor.height = 512;
    color_texture_3d_descriptor.depth = 512;
    color_texture_3d_descriptor.mipLevels = 1;
    color_texture_3d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED;
    color_texture_3d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    color_texture_3d.Create("color_texture_3d", color_texture_3d_descriptor, &resource_allocator);

    Turbo::Render::TDepthTexture2D depth_texture_2d;
    Turbo::Render::TDepthTexture2D::Descriptor depth_texture2d_descriptor = {};
    depth_texture2d_descriptor.width = 512;
    depth_texture2d_descriptor.height = 512;
    depth_texture2d_descriptor.mipLevels = 1;
    depth_texture2d_descriptor.usages = Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT;
    depth_texture2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    depth_texture_2d.Create("depth_texture_2d", depth_texture2d_descriptor, &resource_allocator);

    color_texture_2d.Destroy(&resource_allocator);
    color_texture_3d.Destroy(&resource_allocator);
    depth_texture_2d.Destroy(&resource_allocator);

    std::cout<<"Test3()::End......................................................."<<std::endl;
}

int main()
{
    Test0();
    Test1();
    Test2();
    Test3();
    return 0;
}