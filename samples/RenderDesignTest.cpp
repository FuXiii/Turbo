#include <iostream>
#include <render/include/TContext.h>
#include <render/include/TResourceAllocator.h>

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

    return 0;
}