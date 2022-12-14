#include <iostream>
#include <render/include/TContext.h>
#include <render/include/TResourceAllocator.h>

int main()
{
    Turbo::Render::TContext context;
    Turbo::Render::TResourceAllocator resource_allocator(&context);

    Turbo::Render::TColorImage2D::Descriptor color_image_2d_descriptor = {};
    color_image_2d_descriptor.flags = 0;
    color_image_2d_descriptor.width = 512;
    color_image_2d_descriptor.height = 512;
    color_image_2d_descriptor.layers = 1;
    color_image_2d_descriptor.mipLevels = 1;
    color_image_2d_descriptor.usages = Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT;
    color_image_2d_descriptor.domain = Turbo::Render::TDomainBits::GPU;

    Turbo::Render::TColorImage2D color_image_2d;
    color_image_2d.Create("color_image_2d", color_image_2d_descriptor, &resource_allocator);
    color_image_2d.Destroy(&resource_allocator);

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

    return 0;
}