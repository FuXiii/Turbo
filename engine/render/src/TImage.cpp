#include "render/include/TImage.h"
#include "render/include/TResourceAllocator.h"
#include "vulkan/vulkan_core.h"
#include <core/include/TCommandBuffer.h>
#include <core/include/TException.h>
#include <core/include/TImageView.h>
#include <core/include/TPhysicalDevice.h>

Turbo::Core::TRefPtr<Turbo::Core::TImageView> Turbo::Render::TImage::CreateImageView(Turbo::Core::TRefPtr<Turbo::Core::TImage> &image)
{
    // TODO: nothing
    return nullptr;
}

void Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        this->image = resource_allocator->CreateImage(descriptor);
        this->descriptor = descriptor;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)", "Please use an available allocator");
    }

    // TODO:create Turbo::Core::ImageView
    this->imageView = this->CreateImageView(this->image);

    {
        // FIXME: Test Code:
        if (this->IsValid())
        {
            Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
            Turbo::Render::TCommandBuffer command_buffer = resource_allocator->GetContext()->GetCommandBuffer();

            command_buffer.commandBuffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TPipelineStageBits::TOP_OF_PIPE_BIT, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TAccessBits::ACCESS_NONE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::GENERAL, this->imageView);
        }
    }
}

void Turbo::Render::TImage::Destroy(void *allocator)
{
    // TODO:destroy Turbo::Core::ImageView
    // OLD:delete this->imageView;
    this->imageView = nullptr;

    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        resource_allocator->DestroyImage(this->image);
    }

    memset(&this->descriptor, 0, sizeof(this->descriptor));
}

Turbo::Render::TFormat Turbo::Render::TImage::GetFormat() const
{
    return this->descriptor.format;
}

uint32_t Turbo::Render::TImage::GetWidth() const
{
    return this->descriptor.width;
}

uint32_t Turbo::Render::TImage::GetHeight() const
{
    return this->descriptor.height;
}

uint32_t Turbo::Render::TImage::GetDepth() const
{
    return this->descriptor.depth;
}

uint32_t Turbo::Render::TImage::GetLayers() const
{
    return this->descriptor.layers;
}

uint32_t Turbo::Render::TImage::GetMipLevels() const
{
    return this->descriptor.mipLevels;
}

Turbo::Render::TImageUsages Turbo::Render::TImage::GetUsages() const
{
    return this->descriptor.usages;
}

Turbo::Render::TDomain Turbo::Render::TImage::GetDomain() const
{
    return this->descriptor.domain;
}

Turbo::Render::TSampleCountBits Turbo::Render::TImage::GetSampleCountBits() const
{
    return Turbo::Render::TSampleCountBits::SAMPLE_1_BIT;
}

bool Turbo::Render::TImage::IsValid() const
{
    if (this->image != nullptr && this->imageView != nullptr)
    {
        if (this->image->GetVkImage() != VK_NULL_HANDLE && this->imageView->GetVkImageView() != VK_NULL_HANDLE)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Render::TImage::operator==(const TImage &image)
{
    if (this->image == image.image)
    {
        return true;
    }

    return false;
}

bool Turbo::Render::TImage::operator!=(const TImage &image)
{
    if (this->image != image.image)
    {
        return true;
    }

    return false;
}

void Turbo::Render::TColorImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TImage::Descriptor image_descriptor{};
    image_descriptor.flags = descriptor.flags;
    image_descriptor.format = Turbo::Render::TFormat::UNDEFINED;
    image_descriptor.width = descriptor.width;
    image_descriptor.height = descriptor.height;
    image_descriptor.depth = descriptor.depth;
    image_descriptor.layers = descriptor.layers;
    image_descriptor.mipLevels = descriptor.mipLevels;
    image_descriptor.usages = descriptor.usages;
    image_descriptor.domain = descriptor.domain;

    Turbo::Render::TResourceAllocator *resource_allocator = nullptr;
    if (allocator != nullptr)
    {
        resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)", "Please use an available allocator");
    }

    Turbo::Core::TPhysicalDevice *physical_device = resource_allocator->GetContext()->GetPhysicalDevice();
    static std::vector<Turbo::Render::TFormat> pending_formats{Turbo::Render::TFormat::R8G8B8A8_SRGB, Turbo::Render::TFormat::B8G8R8A8_SRGB, Turbo::Render::TFormat::R8G8B8_SRGB, Turbo::Render::TFormat::B8G8R8_SRGB, Turbo::Render::TFormat::R8G8B8A8_UNORM, Turbo::Render::TFormat::B8G8R8A8_UNORM, Turbo::Render::TFormat::R8G8B8_UNORM, Turbo::Render::TFormat::B8G8R8_UNORM};

    std::vector<Turbo::Render::TFormat>::iterator begin = pending_formats.begin();
    std::vector<Turbo::Render::TFormat>::iterator end = pending_formats.end();
    // choose format
    if (((image_descriptor.usages & TImageUsageBits::COLOR_ATTACHMENT) == TImageUsageBits::COLOR_ATTACHMENT) || ((image_descriptor.usages & TImageUsageBits::INPUT_ATTACHMENT) == TImageUsageBits::INPUT_ATTACHMENT))
    {
        begin = pending_formats.begin();
    }
    else if (((image_descriptor.usages & TImageUsageBits::SAMPLED) == TImageUsageBits::SAMPLED) || ((image_descriptor.usages & TImageUsageBits::STORAGE) == TImageUsageBits::STORAGE))
    {
        begin = pending_formats.begin() + 4;
    }
    else
    {
        // TODO: do nothing
    }

    // TODO: choose support format
    for (; begin != end; begin++)
    {
        Turbo::Render::TFormat candidate_format = *begin;
        Turbo::Core::TFormatInfo format_info = physical_device->GetFormatInfo(static_cast<Turbo::Core::TFormatType>(candidate_format));

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportTransferSrc())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportTransferSrc())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportTransferDst())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportTransferDst())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::SAMPLED) == Turbo::Render::TImageUsageBits::SAMPLED)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportSampledImage())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportSampledImage())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::STORAGE) == Turbo::Render::TImageUsageBits::STORAGE)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportStorageImage())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportStorageImage())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT) == Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportColorAttachment())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportColorAttachment())
                {
                    continue;
                }
            }
        }

        // Normally, depth_stencil usage should not be set, but we still try to deal with it,
        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT) == Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportDepthStencilAttachment())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportDepthStencilAttachment())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::TRANSIENT_ATTACHMENT) == Turbo::Render::TImageUsageBits::TRANSIENT_ATTACHMENT)
        {
            // In Vulkan spec can not find some VkFormatFeatureFlagBits standard about TRANSIENT_ATTACHMENT
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT) == Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT)
        {
            // In Vulkan spec can not find some VkFormatFeatureFlagBits standard about INPUT_ATTACHMENT
        }

        image_descriptor.format = static_cast<Turbo::Render::TFormat>(format_info.GetFormatType());
        break;
    }

    if (image_descriptor.format == Turbo::Render::TFormat::UNDEFINED)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)", "Can not found suitable format for this case");
    }

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

void Turbo::Render::TColorImage3D::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TColorImage::Descriptor color_image_descriptor{};
    color_image_descriptor.flags = 0;
    color_image_descriptor.width = descriptor.width;
    color_image_descriptor.height = descriptor.height;
    color_image_descriptor.depth = descriptor.depth;
    color_image_descriptor.layers = 1;
    color_image_descriptor.mipLevels = descriptor.mipLevels;
    color_image_descriptor.usages = descriptor.usages;
    color_image_descriptor.domain = descriptor.domain;

    TColorImage::Create(name, color_image_descriptor, allocator);
}

void Turbo::Render::TDepthStencilImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TImage::Descriptor image_descriptor{};
    image_descriptor.flags = 0;
    image_descriptor.format = Turbo::Render::TFormat::UNDEFINED;
    image_descriptor.width = descriptor.width;
    image_descriptor.height = descriptor.height;
    image_descriptor.depth = descriptor.depth;
    image_descriptor.layers = descriptor.layers;
    image_descriptor.mipLevels = descriptor.mipLevels;
    image_descriptor.usages = descriptor.usages | Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT;
    image_descriptor.domain = descriptor.domain;

    Turbo::Render::TResourceAllocator *resource_allocator = nullptr;
    if (allocator != nullptr)
    {
        resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)", "Please use an available allocator");
    }

    Turbo::Core::TPhysicalDevice *physical_device = resource_allocator->GetContext()->GetPhysicalDevice();
    static std::vector<Turbo::Render::TFormat> pending_formats{Turbo::Render::TFormat::D32_SFLOAT, Turbo::Render::TFormat::D16_UNORM};

    std::vector<Turbo::Render::TFormat>::iterator begin = pending_formats.begin();
    std::vector<Turbo::Render::TFormat>::iterator end = pending_formats.end();
    // choose format
    for (; begin != end; begin++)
    {
        Turbo::Render::TFormat candidate_format = *begin;
        Turbo::Core::TFormatInfo format_info = physical_device->GetFormatInfo(static_cast<Turbo::Core::TFormatType>(candidate_format));

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportTransferSrc())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportTransferSrc())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportTransferDst())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportTransferDst())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::SAMPLED) == Turbo::Render::TImageUsageBits::SAMPLED)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportSampledImage())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportSampledImage())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::STORAGE) == Turbo::Render::TImageUsageBits::STORAGE)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportStorageImage())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportStorageImage())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT) == Turbo::Render::TImageUsageBits::COLOR_ATTACHMENT)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportColorAttachment())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportColorAttachment())
                {
                    continue;
                }
            }
        }

        // Normally, depth_stencil usage should not be set, but we still try to deal with it,
        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT) == Turbo::Render::TImageUsageBits::DEPTH_STENCIL_ATTACHMENT)
        {
            if ((image_descriptor.domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
            {
                if (!format_info.IsLinearTilingSupportDepthStencilAttachment())
                {
                    continue;
                }
            }
            else
            {
                if (!format_info.IsOptimalTilingSupportDepthStencilAttachment())
                {
                    continue;
                }
            }
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::TRANSIENT_ATTACHMENT) == Turbo::Render::TImageUsageBits::TRANSIENT_ATTACHMENT)
        {
            // In Vulkan spec can not find some VkFormatFeatureFlagBits standard about TRANSIENT_ATTACHMENT
        }

        if ((image_descriptor.usages & Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT) == Turbo::Render::TImageUsageBits::INPUT_ATTACHMENT)
        {
            // In Vulkan spec can not find some VkFormatFeatureFlagBits standard about INPUT_ATTACHMENT
        }

        image_descriptor.format = static_cast<Turbo::Render::TFormat>(format_info.GetFormatType());
        break;
    }

    if (image_descriptor.format == Turbo::Render::TFormat::UNDEFINED)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Render::TImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)", "Can not found suitable format for this case");
    }

    TImage::Create(name, image_descriptor, allocator);
}

void Turbo::Render::TDepthImage::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TDepthStencilImage::Descriptor depth_stencil_image_descriptor = {};
    depth_stencil_image_descriptor.width = descriptor.width;
    depth_stencil_image_descriptor.height = descriptor.height;
    depth_stencil_image_descriptor.depth = descriptor.depth;
    depth_stencil_image_descriptor.layers = descriptor.layers;
    depth_stencil_image_descriptor.mipLevels = descriptor.mipLevels;
    depth_stencil_image_descriptor.usages = descriptor.usages;
    depth_stencil_image_descriptor.domain = descriptor.domain;

    Turbo::Render::TDepthStencilImage::Create(name, depth_stencil_image_descriptor, allocator);
}

void Turbo::Render::TDepthImage2D::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TDepthImage::Descriptor depth_image_descriptor = {};
    depth_image_descriptor.width = descriptor.width;
    depth_image_descriptor.height = descriptor.height;
    depth_image_descriptor.depth = 1;
    depth_image_descriptor.layers = descriptor.layers;
    depth_image_descriptor.mipLevels = descriptor.mipLevels;
    depth_image_descriptor.usages = descriptor.usages;
    depth_image_descriptor.domain = descriptor.domain;

    Turbo::Render::TDepthImage::Create(name, depth_image_descriptor, allocator);
}

Turbo::Core::TRefPtr<Turbo::Core::TImageView> Turbo::Render::TTexture2D::CreateImageView(Turbo::Core::TRefPtr<Turbo::Core::TImage> &image)
{
    if (image.Valid())
    {
        return new Turbo::Core::TImageView(image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, image->GetFormat().GetFormatType(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, image->GetMipLevels(), 0, 1);
    }

    return nullptr;
}

void Turbo::Render::TTexture2D::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TColorImage2D::Descriptor color_image2d_descriptor{};
    color_image2d_descriptor.flags = 0;
    color_image2d_descriptor.width = descriptor.width;
    color_image2d_descriptor.height = descriptor.height;
    color_image2d_descriptor.layers = 1;
    color_image2d_descriptor.mipLevels = descriptor.mipLevels;
    color_image2d_descriptor.usages = descriptor.usages;
    color_image2d_descriptor.domain = descriptor.domain;

    TColorImage2D::Create(name, color_image2d_descriptor, allocator);
}

Turbo::Core::TRefPtr<Turbo::Core::TImageView> Turbo::Render::TTexture3D::CreateImageView(Turbo::Core::TRefPtr<Turbo::Core::TImage> &image)
{
    if (image.Valid())
    {
        return new Turbo::Core::TImageView(image, Turbo::Core::TImageViewType::IMAGE_VIEW_3D, image->GetFormat().GetFormatType(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, image->GetMipLevels(), 0, 1);
    }

    return nullptr;
}

void Turbo::Render::TTexture3D::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TColorImage3D::Descriptor color_image3d_descriptor{};
    color_image3d_descriptor.width = descriptor.width;
    color_image3d_descriptor.height = descriptor.height;
    color_image3d_descriptor.depth = descriptor.depth;
    color_image3d_descriptor.mipLevels = descriptor.mipLevels;
    color_image3d_descriptor.usages = descriptor.usages;
    color_image3d_descriptor.domain = descriptor.domain;

    TColorImage3D::Create(name, color_image3d_descriptor, allocator);
}

Turbo::Core::TRefPtr<Turbo::Core::TImageView> Turbo::Render::TDepthTexture2D::CreateImageView(Turbo::Core::TRefPtr<Turbo::Core::TImage> &image)
{
    if (image.Valid())
    {
        return new Turbo::Core::TImageView(image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, image->GetFormat().GetFormatType(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, image->GetMipLevels(), 0, 1);
    }

    return nullptr;
}

void Turbo::Render::TDepthTexture2D::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    TDepthImage2D::Descriptor depth_image2d_descriptor = {};
    depth_image2d_descriptor.width = descriptor.width;
    depth_image2d_descriptor.height = descriptor.height;
    depth_image2d_descriptor.layers = 1;
    depth_image2d_descriptor.mipLevels = descriptor.mipLevels;
    depth_image2d_descriptor.usages = descriptor.usages;
    depth_image2d_descriptor.domain = descriptor.domain;

    TDepthImage2D::Create(name, depth_image2d_descriptor, allocator);
}
