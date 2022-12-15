#include "TImage.h"
#include "TResourceAllocator.h"
#include <core/include/TException.h>

void Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TImageCreateFlags image_create_flags = descriptor.flags;
    TFormat format = descriptor.format;
    uint32_t width = descriptor.width;
    uint32_t height = descriptor.height;
    uint32_t depth = descriptor.depth;
    uint32_t layers = descriptor.layers;
    uint32_t mipLevels = descriptor.mipLevels;
    TImageUsages usages = descriptor.usages;
    TDomain domain = descriptor.domain;

    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        this->image = resource_allocator->CreateImage(descriptor);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)", "Please use an available allocator");
    }

    // TODO:create Turbo::Core::ImageView
}

void Turbo::Render::TImage::Destroy(void *allocator)
{
    // TODO:destroy Turbo::Core::ImageView

    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        resource_allocator->DestroyImage(this->image);
    }
}

void Turbo::Render::TColorImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TImage::Descriptor image_descriptor{};
    image_descriptor.flags = descriptor.flags;
    image_descriptor.format = Turbo::Render::TFormat::R8G8B8A8_UNORM;
    image_descriptor.width = descriptor.width;
    image_descriptor.height = descriptor.height;
    image_descriptor.depth = descriptor.depth;
    image_descriptor.layers = descriptor.layers;
    image_descriptor.mipLevels = descriptor.mipLevels;
    image_descriptor.usages = descriptor.usages;
    image_descriptor.domain = descriptor.domain;

    TImage::Create(name, image_descriptor, allocator);
}

void Turbo::Render::TColorImage2D::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TColorImage::Descriptor color_image_descriptor{};
    color_image_descriptor.flags = descriptor.flags;
    color_image_descriptor.width = descriptor.width;
    color_image_descriptor.height = descriptor.height;
    color_image_descriptor.depth = 1;
    color_image_descriptor.layers = descriptor.layers;
    color_image_descriptor.mipLevels = descriptor.mipLevels;
    color_image_descriptor.usages = descriptor.usages;
    color_image_descriptor.domain = descriptor.domain;

    TColorImage::Create(name, color_image_descriptor, allocator);
}