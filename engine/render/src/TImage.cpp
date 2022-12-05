#include "TImage.h"

void Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor/*TODO: we need a allocator/context*/)
{
    uint32_t width = descriptor.width;
    uint32_t height = descriptor.height;
    uint32_t depth = descriptor.depth;
    uint32_t layers = descriptor.layers;
    // create Turbo::Core::Image
    // create Turbo::Core::ImageView
}

void Turbo::Render::TImage::Destroy(/*TODO: we need a allocator/context*/)
{
    // destroy Turbo::Core::ImageView
    // destroy Turbo::Core::Image
}

void Turbo::Render::TColorImage::Create(const std::string &name, const Descriptor &descriptor/*TODO: we need a allocator/context*/)
{
    TImage::Descriptor image_descriptor{};
    image_descriptor.width = descriptor.width;
    image_descriptor.height = descriptor.height;
    image_descriptor.depth = descriptor.depth;
    image_descriptor.layers = descriptor.layers;

    TImage::Create(name, image_descriptor);
}

void Turbo::Render::TColorImage2D::Create(const std::string &name, const Descriptor &descriptor/*TODO: we need a allocator/context*/)
{
    TColorImage::Descriptor color_image_descriptor{};
    color_image_descriptor.width = descriptor.width;
    color_image_descriptor.height = descriptor.height;
    color_image_descriptor.depth = 1;
    color_image_descriptor.layers = descriptor.layers;

    TColorImage::Create(name, color_image_descriptor);
}

void Turbo::Render::Test()
{
    TColorImage2D color_image_2d;
    color_image_2d.Create("ColorImage", {512, 512, 1}/*TODO: we need a allocator/context*/);
    color_image_2d.Destroy(/*TODO: we need a allocator/context*/);
}