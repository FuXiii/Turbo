#include "TBuffer.h"
#include "TImage.h"
#include <chrono>
#include <framegraph/include/TFrameGraph.hpp>
#include <iostream>
#include <render/include/TContext.h>
#include <render/include/TResourceAllocator.h>
#include <vector>

int main()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

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

    //========================================================================

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

    //========================================================================
    
    Turbo::FrameGraph::TRenderPass fg_render_pass{};
    fg_render_pass.testValue = 123;

    context.BeginRenderPass(fg_render_pass);

    return 0;
}