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
#include <render/include/TSampler.h>
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

        // uint32_t create_count = 2048;
        // std::vector<Turbo::Render::TTexture2D> texture2ds(create_count);
        // std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
        // for (uint32_t create_index = 0; create_index < create_count; create_index++)
        // {
        //     Turbo::Render::TTexture2D::Descriptor temp_texture_2d_descriptor = {};
        //     temp_texture_2d_descriptor.width = 512;
        //     temp_texture_2d_descriptor.height = 512;
        //     temp_texture_2d_descriptor.mipLevels = 1;
        //     temp_texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED;
        //     temp_texture_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

        //     Turbo::Render::TTexture2D temp_texture_2d;
        //     temp_texture_2d.Create("temp_texture_2d", temp_texture_2d_descriptor, &resource_allocator);
        //     texture2ds[create_index] = temp_texture_2d;
        // }
        // std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
        // std::chrono::duration<double> delta_time = end_time - start_time;
        // std::cout << "create " << create_count << " Turbo::Render::TTexture2D use:" << delta_time.count() << "s" << std::endl;

        // for (Turbo::Render::TTexture2D &texture2d_item : texture2ds)
        // {
        //     texture2d_item.Destroy(&resource_allocator);
        // }
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
        // context.BeginRenderPass(fg_render_pass);
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
    std::cout << "Test2()::Begin......................................................." << std::endl;

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

    Turbo::Render::TRenderPassPool render_pass_pool(&context);
    bool render_pass_proxy_0 = render_pass_pool.Allocate(render_pass);
    render_pass_proxy_0 ? std::cout << "Allocate RenderPass0 Success" << std::endl : std::cout << "Allocate RenderPass0 Faild" << std::endl;

    bool render_pass_proxy_1 = render_pass_pool.Allocate(render_pass);
    render_pass_proxy_1 ? std::cout << "Allocate RenderPass1 Success" << std::endl : std::cout << "Allocate RenderPass1 Faild" << std::endl;

    bool render_pass_proxy_2 = render_pass_pool.Allocate(render_pass);
    render_pass_proxy_2 ? std::cout << "Allocate RenderPass2 Success" << std::endl : std::cout << "Allocate RenderPass2 Faild" << std::endl;

    if (render_pass.IsValid())
    {
        std::cout << "render_pass is valid" << std::endl;
    }
    else
    {
        std::cout << "render_pass not valid" << std::endl;
    }

    color_texture_2d.Destroy(&resource_allocator);
    depth_texture_2d.Destroy(&resource_allocator);

    std::cout << "Test2()::End......................................................." << std::endl;
}

void Test3()
{
    std::cout << "Test3()::Begin......................................................." << std::endl;

    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    Turbo::Render::TTexture2D color_texture_2d;
    Turbo::Render::TTexture2D::Descriptor color_texture_2d_descriptor = {};
    color_texture_2d_descriptor.width = 512;
    color_texture_2d_descriptor.height = 512;
    color_texture_2d_descriptor.mipLevels = 1;
    color_texture_2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT | Turbo::Render::TImageUsageBits::TRANSFER_DST;
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

    if (context.BeginRenderPass(render_pass))
    {
        // context.BindPipeline(const Turbo::Render::TComputePipeline &computePipeline);
        context.EndRenderPass();

        context.ClearTexture(color_texture_2d, 1, 1, 1, 1);

        context.Flush();
        context.Wait(UINT64_MAX);
    }

    color_texture_2d.Destroy(&resource_allocator);
    depth_texture_2d.Destroy(&resource_allocator);

    std::cout << "Test3()::End......................................................." << std::endl;
}

#include <core/include/TCommandBuffer.h>
#include <core/include/TDevice.h>
#include <core/include/TDeviceQueue.h>
#include <core/include/TFence.h>
#include <core/include/TImage.h>
#include <core/include/TImageView.h>
#include <core/include/TInstance.h>
#include <core/include/TPipeline.h>
#include <core/include/TSurface.h>
#include <core/include/TSwapchain.h>
#include <core/include/TVulkanLoader.h>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

void Test4()
{
    std::cout << "Test4()::Begin......................................................." << std::endl;
    /*
       [Clear Pass] → Color Texture → [Present Pass]
       1. 在ClearPass创建一个Texture并使用Clear刷新颜色
       2. 在PresentPass中将之前刷新的Texture结果拷贝至要显示的纹理中
       3. 显示
       4. 重复1-3
    */
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    if (!glfwInit())
        return;

    int window_width = 512;
    int window_height = 512;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = context.GetInstance()->GetVkInstance();
    VkResult surface_result = glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    if (vk_surface_khr == VK_NULL_HANDLE)
    {
        std::cout << "vk_surface_khr is VK_NULL_HANDLE::" << surface_result << std::endl;
        return;
    }

    Turbo::Extension::TSurface *surface = new Turbo::Extension::TSurface(context.GetDevice(), vk_surface_khr);

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

    int current_width = window_width;
    int current_height = window_height;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        Turbo::FrameGraph::TFrameGraph fg;

        struct ClearPassData
        {
            Turbo::FrameGraph::TResource colorTexture;
        };

        fg.AddPass<ClearPassData>(
            "ClearPass",
            [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, ClearPassData &data) {
                data.colorTexture = builder.Create<Turbo::Render::TTexture2D>("ColorTexture", {(uint32_t)current_width, (uint32_t)current_height, 1, Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT | Turbo::Render::TImageUsageBits::TRANSFER_SRC | Turbo::Render::TImageUsageBits::TRANSFER_DST, Turbo::Render::TDomainBits::GPU});

                auto subpass0 = builder.CreateSubpass();
                data.colorTexture = subpass0.Write(data.colorTexture);

                fg.GetBlackboard()["ColorTexture"] = data.colorTexture;
            },
            [&](const ClearPassData &data, const Turbo::FrameGraph::TResources &resources, void *_context) {
                // TODO:Clear Color Texture

                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(data.colorTexture);

                if (_context != nullptr)
                {
                    Turbo::Render::TContext *temp_context = (Turbo::Render::TContext *)_context;

                    auto now_time = glfwGetTime();

                    float r = (std::sin(now_time) + 1) * 0.5;
                    float g = (std::cos(now_time) + 1) * 0.5;
                    float b = (std::cos(now_time + 3.1415926 / 3) + 1) * 0.5;

                    temp_context->ClearTexture(color_texture, r, g, b, 1);
                }
            });

        struct PresentPassData
        {
            Turbo::FrameGraph::TResource colorTexture;
        };

        fg.AddPass<PresentPassData>(
            "PresentPass",
            [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, PresentPassData &data) {
                data.colorTexture = fg.GetBlackboard()["ColorTexture"];

                auto subpass0 = builder.CreateSubpass();
                data.colorTexture = subpass0.Read(data.colorTexture);

                builder.SideEffect();
            },
            [&](const PresentPassData &data, const Turbo::FrameGraph::TResources &resources, void *_context) {
                // TODO:Present Color Texture

                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(data.colorTexture);

                if (_context != nullptr)
                {
                    Turbo::Render::TContext *temp_context = (Turbo::Render::TContext *)_context;

                    temp_context->Flush();
                    temp_context->Wait(UINT64_MAX);
                    // TODO: present
                    uint32_t index = UINT32_MAX;
                    Turbo::Core::TFence *acquire_next_image_fence = new Turbo::Core::TFence(temp_context->GetDevice());
                    Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(nullptr, acquire_next_image_fence, &index);
                    acquire_next_image_fence->WaitUntil();
                    delete acquire_next_image_fence;
                    if (result == Turbo::Core::TResult::SUCCESS)
                    {
                        // TODO:Blit Image

                        auto show_target = swapchain_image_views[index];

                        Turbo::Core::TFence *fence = new Turbo::Core::TFence(temp_context->GetDevice());
                        Turbo::Core::TCommandBuffer *cb = temp_context->AllocateCommandBuffer();
                        cb->Begin();
                        cb->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, show_target);
                        // cb->CmdClearColorImage(show_target, Turbo::Core::TImageLayout::GENERAL, 1, 0, 0, 1);
                        cb->CmdBlitImage(temp_context->GetTextureImage(color_texture), Turbo::Core::TImageLayout::GENERAL, show_target->GetImage(), Turbo::Core::TImageLayout::GENERAL, 0, 0, 0, current_width, current_height, 1, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, current_width, current_height, 1, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1);
                        cb->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::GENERAL, Turbo::Core::TImageLayout::PRESENT_SRC_KHR, show_target);
                        cb->End();
                        temp_context->GetDeviceQueue()->Submit(nullptr, nullptr, cb, fence);

                        fence->WaitUntil();

                        delete fence;
                        temp_context->FreeCommandBuffer(cb);
                        Turbo::Core::TResult result = temp_context->GetDeviceQueue()->Present(swapchain, index);
                        switch (result)
                        {
                        case Turbo::Core::TResult::MISMATCH: {
                            Turbo::Core::TDevice *device = temp_context->GetDevice();
                            device->WaitIdle();

                            swapchain_images.clear();

                            for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
                            {
                                delete swapchain_image_view_item;
                            }

                            swapchain_image_views.clear();

                            Turbo::Extension::TSwapchain *old_swapchain = swapchain;
                            Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
                            delete old_swapchain;

                            swapchain = new_swapchain;

                            swapchain_images = swapchain->GetImages();
                            for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
                            {
                                Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                                swapchain_image_views.push_back(swapchain_view);
                            }

                            glfwGetWindowSize(window, &current_width, &current_height);
                        }
                        break;
                        default: {
                        }
                        break;
                        }
                        // std::cout << "Present" << std::endl;
                    }
                }
            });

        fg.Compile();
        fg.Execute(&context, &resource_allocator);
    }

    context.GetDevice()->WaitIdle();

    for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
    {
        delete swapchain_image_view_item;
    }

    delete swapchain;
    delete surface;
    PFN_vkDestroySurfaceKHR pfn_vk_destroy_surface_khr = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(context.GetInstance()->GetVkInstance(), "vkDestroySurfaceKHR");
    pfn_vk_destroy_surface_khr(context.GetInstance()->GetVkInstance(), vk_surface_khr, nullptr);
    glfwTerminate();
    std::cout << "Test4()::End......................................................." << std::endl;
}

void Test5()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    struct Position
    {
        float x;
        float y;
        float z;
    };

    struct UV
    {
        float x;
        float y;
    };

    struct Normal
    {
        float x;
        float y;
        float z;
    };

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    };

    struct VertexData
    {
        Position position;
        UV uv;
        Normal normal;
        Color color;
    };

    float l = 1;
    float h = l * std::sin(3.1415926 / 6);
    float w = l * std::cos(3.1415926 / 6);

    std::vector<VertexData> vertexs{{{-w, -h, 0}, {0, 0}, {0, 0, 1}, {1, 0, 0, 1}}, {{w, -h, 0}, {1, 0}, {0, 0, 1}, {0, 1, 0, 1}}, {{0, l, 0}, {0.5, 1}, {0, 0, 1}, {0, 0, 1, 1}}};

    Turbo::Render::TVertexBuffer vertex_buffer;
    Turbo::Render::TVertexBuffer::Descriptor vertex_buffer_descriptor;
    vertex_buffer_descriptor.size = sizeof(VertexData) * vertexs.size();
    vertex_buffer_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    vertex_buffer_descriptor.stride = sizeof(VertexData);
    vertex_buffer_descriptor.rate = Turbo::Render::TVertexBuffer::TRate::VERTEX;

    Turbo::Render::TAttributeID position_id = vertex_buffer.AddAttribute(Turbo::Render::TFormat::R32G32B32_SFLOAT, offsetof(VertexData, position));
    Turbo::Render::TAttributeID uv_id = vertex_buffer.AddAttribute(Turbo::Render::TFormat::R32G32_SFLOAT, offsetof(VertexData, uv));
    Turbo::Render::TAttributeID normal_id = vertex_buffer.AddAttribute(Turbo::Render::TFormat::R32G32B32_SFLOAT, offsetof(VertexData, normal));
    Turbo::Render::TAttributeID color_id = vertex_buffer.AddAttribute(Turbo::Render::TFormat::R32G32B32A32_SFLOAT, offsetof(VertexData, color));

    vertex_buffer.Create("vertex_buffer", vertex_buffer_descriptor, &resource_allocator);
    vertex_buffer.Copy(vertexs.data(), vertex_buffer_descriptor.size);
    vertex_buffer.Destroy(&resource_allocator);
}

void Test6()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    std::vector<uint32_t> indexs;
    indexs.push_back(0);
    indexs.push_back(1);
    indexs.push_back(2);

    Turbo::Render::TIndexBuffer index_buffer;

    index_buffer.Create("index_buffer", {indexs.size() * sizeof(uint32_t), Turbo::Render::TDomainBits::GPU}, &resource_allocator);
    index_buffer.Copy(indexs);
    index_buffer.Destroy(&resource_allocator);
}

void Test7()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    struct UniformStruct
    {
        float a;
        int b;
        double c;
        uint32_t d;
    };

    UniformStruct us;
    us.a = 1;
    us.b = 2;
    us.c = 3;
    us.d = 4;

    Turbo::Render::TUniformBuffer<UniformStruct> uniform_buffer;

    uniform_buffer.Create("uniform_buffer", {Turbo::Render::TDomainBits::BOTH}, &resource_allocator);
    uniform_buffer.Copy(us);
    uniform_buffer.Destroy(&resource_allocator);
}

void Test8()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    Turbo::Render::TTexture2D texture0;
    Turbo::Render::TTexture2D::Descriptor texture0_descriptor = {};
    texture0_descriptor.width = 512;
    texture0_descriptor.height = 512;
    texture0_descriptor.mipLevels = 1;
    texture0_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED | Turbo::Render::TImageUsageBits::TRANSFER_DST;
    texture0_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    texture0.Create("texture0", texture0_descriptor, &resource_allocator);

    Turbo::Render::TTexture2D texture1;
    Turbo::Render::TTexture2D::Descriptor texture1_descriptor = {};
    texture1_descriptor.width = 512;
    texture1_descriptor.height = 512;
    texture1_descriptor.mipLevels = 1;
    texture1_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED | Turbo::Render::TImageUsageBits::TRANSFER_DST;
    texture1_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    texture1.Create("texture1", texture1_descriptor, &resource_allocator);

    std::vector<Turbo::Render::TTexture2D> textures;
    textures.push_back(texture0);
    textures.push_back(texture1);

    Turbo::Render::TTexture3D texture_3d_0;
    Turbo::Render::TTexture3D::Descriptor texture_3d_descriptor = {};
    texture_3d_descriptor.width = 512;
    texture_3d_descriptor.height = 512;
    texture_3d_descriptor.depth = 512;
    texture_3d_descriptor.mipLevels = 1;
    texture_3d_descriptor.usages = Turbo::Render::TImageUsageBits::SAMPLED | Turbo::Render::TImageUsageBits::TRANSFER_DST;
    texture_3d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

    texture_3d_0.Create("texture_3d_0", texture_3d_descriptor, &resource_allocator);

    struct UniformStruct
    {
        float a;
        int b;
        double c;
        uint32_t d;
    };

    UniformStruct us;
    us.a = 1;
    us.b = 2;
    us.c = 3;
    us.d = 4;

    Turbo::Render::TUniformBuffer<UniformStruct> uniform_buffer;

    uniform_buffer.Create("uniform_buffer", {Turbo::Render::TDomainBits::BOTH}, &resource_allocator);
    uniform_buffer.Copy(us);

    Turbo::Render::TSampler sampler;
    sampler.Create("sampler", {}, &resource_allocator);

    context.BindDescriptor(0, 0, textures);
    context.BindDescriptor(0, 1, texture_3d_0);
    context.BindDescriptor(0, 2, uniform_buffer);

    sampler.Destroy(&resource_allocator);
    uniform_buffer.Destroy(&resource_allocator);
    texture_3d_0.Destroy(&resource_allocator);
    texture1.Destroy(&resource_allocator);
    texture0.Destroy(&resource_allocator);
}

void Test9()
{
    std::cout << "Test9()::Begin......................................................." << std::endl;
    /*
       [Draw Pass(进行一个简单的三角形绘制)] → Color Texture → [Present Pass]
       1. 在DrawPass创建一个Texture并使用DrawCall进行绘制
       2. 在PresentPass中将之前绘制的Texture结果拷贝至要显示的纹理中
       3. 显示
       4. 重复1-3
    */
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    if (!glfwInit())
        return;

    int window_width = 512;
    int window_height = 512;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = context.GetInstance()->GetVkInstance();
    VkResult surface_result = glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    if (vk_surface_khr == VK_NULL_HANDLE)
    {
        std::cout << "vk_surface_khr is VK_NULL_HANDLE::" << surface_result << std::endl;
        return;
    }

    Turbo::Extension::TSurface *surface = new Turbo::Extension::TSurface(context.GetDevice(), vk_surface_khr);

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

    int current_width = window_width;
    int current_height = window_height;

    struct vec3
    {
        float x;
        float y;
        float z;
    };

    struct col3
    {
        float r;
        float g;
        float b;
    };

    struct VertexData
    {
        vec3 position;
        col3 color;
    };

    std::vector<VertexData> vertex_data;
    vertex_data.push_back({{0.0f, -0.5f, 0.0f}, {1, 0, 0}});
    vertex_data.push_back({{0.5f, 0.5f, 0.0f}, {0, 1, 0}});
    vertex_data.push_back({{-0.5f, 0.5f, 0.0f}, {0, 0, 1}});

    Turbo::Render::TVertexBuffer vertex_buffer;
    Turbo::Render::TVertexBuffer::Descriptor vertex_buffer_descriptor = {};
    vertex_buffer_descriptor.size = sizeof(VertexData) * vertex_data.size();
    vertex_buffer_descriptor.domain = Turbo::Render::TDomainBits::GPU;
    vertex_buffer_descriptor.rate = Turbo::Render::TVertexBuffer::TRate::VERTEX;
    vertex_buffer_descriptor.stride = sizeof(VertexData);

    vertex_buffer.Create("VertexBuffer", vertex_buffer_descriptor, &resource_allocator);
    Turbo::Render::TAttributeID position_attribute_id = vertex_buffer.AddAttribute(Turbo::Render::TFormat::R32G32B32_SFLOAT, offsetof(VertexData, position));
    Turbo::Render::TAttributeID color_attribute_id = vertex_buffer.AddAttribute(Turbo::Render::TFormat::R32G32B32_SFLOAT, offsetof(VertexData, color));
    vertex_buffer.Copy(vertex_data.data(), sizeof(VertexData) * vertex_data.size());

    Turbo::Render::TVertexShader *vertex_shader = new Turbo::Render::TVertexShader(&context, Turbo::Render::TShader::TLanguage::GLSL, ReadTextFile("../../asset/shaders/pure_triangle.vert"));
    Turbo::Render::TFragmentShader *fragment_shader = new Turbo::Render::TFragmentShader(&context, Turbo::Render::TShader::TLanguage::GLSL, ReadTextFile("../../asset/shaders/pure_triangle.frag"));

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        Turbo::FrameGraph::TFrameGraph fg;

        struct DrawPassData
        {
            Turbo::FrameGraph::TResource colorTexture;
        };

        fg.AddPass<DrawPassData>(
            "DrawPass",
            [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, DrawPassData &data) {
                data.colorTexture = builder.Create<Turbo::Render::TTexture2D>("ColorTexture", {(uint32_t)current_width, (uint32_t)current_height, 1, Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT | Turbo::Render::TImageUsageBits::TRANSFER_SRC | Turbo::Render::TImageUsageBits::TRANSFER_DST, Turbo::Render::TDomainBits::GPU});

                auto subpass0 = builder.CreateSubpass();
                data.colorTexture = subpass0.Write(data.colorTexture);

                fg.GetBlackboard()["ColorTexture"] = data.colorTexture;
            },
            [&](const DrawPassData &data, const Turbo::FrameGraph::TResources &resources, void *_context) {
                // TODO:Clear Color Texture

                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(data.colorTexture);

                resources.GetRenderPass().GetSubpass(0).GetWrite(0).IsValid();

                Turbo::Render::TSubpass subpass;
                subpass.AddColorAttachment(color_texture);

                Turbo::Render::TRenderPass triangle_render_pass;
                triangle_render_pass.AddSubpass(subpass);

                Turbo::Render::TGraphicsPipeline graphics_pipeline;
                graphics_pipeline.SetVertexShader(vertex_shader);
                graphics_pipeline.SetFragmentShader(fragment_shader);

                context.BeginRenderPass(triangle_render_pass);
                context.BindVeretxAttribute(vertex_buffer, position_attribute_id, 0);
                context.BindVeretxAttribute(vertex_buffer, color_attribute_id, 1);
                context.BindPipeline(graphics_pipeline);
                context.Draw(3, 1, 0, 0); //FIXME: <<<---此时发生了内存泄漏
                context.EndRenderPass();

                context.Flush();
                context.Wait(UINT64_MAX);
            });

        struct PresentPassData
        {
            Turbo::FrameGraph::TResource colorTexture;
        };

        fg.AddPass<PresentPassData>(
            "PresentPass",
            [&](Turbo::FrameGraph::TFrameGraph::TBuilder &builder, PresentPassData &data) {
                data.colorTexture = fg.GetBlackboard()["ColorTexture"];

                auto subpass0 = builder.CreateSubpass();
                data.colorTexture = subpass0.Read(data.colorTexture);

                builder.SideEffect();
            },
            [&](const PresentPassData &data, const Turbo::FrameGraph::TResources &resources, void *_context) {
                // TODO:Present Color Texture

                Turbo::Render::TTexture2D color_texture = resources.Get<Turbo::Render::TTexture2D>(data.colorTexture);

                if (_context != nullptr)
                {
                    Turbo::Render::TContext *temp_context = (Turbo::Render::TContext *)_context;

                    temp_context->Flush();
                    temp_context->Wait(UINT64_MAX);
                    // TODO: present
                    uint32_t index = UINT32_MAX;
                    Turbo::Core::TFence *acquire_next_image_fence = new Turbo::Core::TFence(temp_context->GetDevice());
                    Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(nullptr, acquire_next_image_fence, &index);
                    acquire_next_image_fence->WaitUntil();
                    delete acquire_next_image_fence;
                    if (result == Turbo::Core::TResult::SUCCESS)
                    {
                        // TODO:Blit Image
                        auto show_target = swapchain_image_views[index];

                        Turbo::Core::TFence *fence = new Turbo::Core::TFence(temp_context->GetDevice());
                        Turbo::Core::TCommandBuffer *cb = temp_context->AllocateCommandBuffer();
                        cb->Begin();
                        cb->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, show_target);
                        // cb->CmdClearColorImage(show_target, Turbo::Core::TImageLayout::GENERAL, 1, 0, 0, 1);
                        cb->CmdBlitImage(temp_context->GetTextureImage(color_texture), Turbo::Core::TImageLayout::GENERAL, show_target->GetImage(), Turbo::Core::TImageLayout::GENERAL, 0, 0, 0, current_width, current_height, 1, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, current_width, current_height, 1, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1);
                        cb->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::GENERAL, Turbo::Core::TImageLayout::PRESENT_SRC_KHR, show_target);
                        cb->End();
                        temp_context->GetDeviceQueue()->Submit(nullptr, nullptr, cb, fence);

                        fence->WaitUntil();

                        delete fence;
                        temp_context->FreeCommandBuffer(cb);
                        Turbo::Core::TResult result = temp_context->GetDeviceQueue()->Present(swapchain, index);
                        switch (result)
                        {
                        case Turbo::Core::TResult::MISMATCH: {
                            context.GC();
                            Turbo::Core::TDevice *device = temp_context->GetDevice();
                            device->WaitIdle();

                            swapchain_images.clear();

                            for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
                            {
                                delete swapchain_image_view_item;
                            }

                            swapchain_image_views.clear();

                            Turbo::Extension::TSwapchain *old_swapchain = swapchain;
                            Turbo::Extension::TSwapchain *new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);
                            delete old_swapchain;

                            swapchain = new_swapchain;

                            swapchain_images = swapchain->GetImages();
                            for (Turbo::Core::TImage *swapchain_image_item : swapchain_images)
                            {
                                Turbo::Core::TImageView *swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                                swapchain_image_views.push_back(swapchain_view);
                            }

                            glfwGetWindowSize(window, &current_width, &current_height);
                        }
                        break;
                        default: {
                        }
                        break;
                        }
                        // std::cout << "Present" << std::endl;
                    }
                }
            });

        fg.Compile();
        fg.Execute(&context, &resource_allocator);

        context.GC();
    }

    context.GetDevice()->WaitIdle();

    delete fragment_shader;
    delete vertex_shader;

    vertex_buffer.Destroy(&resource_allocator);

    for (Turbo::Core::TImageView *swapchain_image_view_item : swapchain_image_views)
    {
        delete swapchain_image_view_item;
    }

    delete swapchain;
    delete surface;
    PFN_vkDestroySurfaceKHR pfn_vk_destroy_surface_khr = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(context.GetInstance()->GetVkInstance(), "vkDestroySurfaceKHR");
    pfn_vk_destroy_surface_khr(context.GetInstance()->GetVkInstance(), vk_surface_khr, nullptr);
    glfwTerminate();
    std::cout << "Test9()::End......................................................." << std::endl;
}

int main()
{
    // Test0();
    // Test1();
    // Test2();
    // Test3();
    // Test4();
    // Test5();
    // Test6();
    // Test7();
    // Test8();
    Test9();
    return 0;
}