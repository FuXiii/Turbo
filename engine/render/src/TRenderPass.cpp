#include "render/include/TRenderPass.h"
#include "render/include/TImage.h"
#include <core/include/TCore.h>
#include <core/include/TException.h>
#include <core/include/TFramebuffer.h>
#include <core/include/TRenderPass.h>

Turbo::Render::TSubpass &Turbo::Render::TSubpass::AddColorAttachment(const Turbo::Render::TColorImage &colorImage)
{
    if (colorImage.IsValid())
    {
        this->colors.push_back(colorImage);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_EXTERNAL_HANDLE, "Turbo::Render::TSubpass::AddColorAttachment", "ColorImage which input is invalid");
    }
    return *this;
}

Turbo::Render::TSubpass &Turbo::Render::TSubpass::AddInputAttachment(const Turbo::Render::TImage &image)
{
    if (image.IsValid())
    {
        this->inputs.push_back(image);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_EXTERNAL_HANDLE, "Turbo::Render::TSubpass::AddColorAttachment", "ColorImage which input is invalid");
    }
    return *this;
}

Turbo::Render::TSubpass &Turbo::Render::TSubpass::SetDepthStencilAttachment(const Turbo::Render::TDepthStencilImage &depthStencil)
{
    if (depthStencil.IsValid())
    {
        this->depthStencil = depthStencil;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_EXTERNAL_HANDLE, "Turbo::Render::TSubpass::AddColorAttachment", "ColorImage which input is invalid");
    }
    return *this;
}

const std::vector<Turbo::Render::TColorImage> &Turbo::Render::TSubpass::GetColorAttachments()
{
    return this->colors;
}

const std::vector<Turbo::Render::TImage> &Turbo::Render::TSubpass::GetInputAttachments()
{
    return this->inputs;
}

Turbo::Render::TDepthStencilImage Turbo::Render::TSubpass::GetDepthStencilAttachment()
{
    return this->depthStencil;
}

Turbo::Render::TRenderPass &Turbo::Render::TRenderPass::AddSubpass(const Turbo::Render::TSubpass &subpass)
{
    this->subpasses.push_back(subpass);
    return *this;
}

const std::vector<Turbo::Render::TSubpass> &Turbo::Render::TRenderPass::GetSubpasses()
{
    return this->subpasses;
}

std::vector<Turbo::Render::TImage> Turbo::Render::TRenderPass::GetAttachments()
{
    std::vector<Turbo::Render::TImage> result;

    for (Turbo::Render::TSubpass &subpass_item : this->subpasses)
    {
        std::vector<Turbo::Render::TColorImage> color_images = subpass_item.GetColorAttachments();
        std::vector<Turbo::Render::TImage> input_images = subpass_item.GetInputAttachments();
        Turbo::Render::TDepthStencilImage depth_stencil_image = subpass_item.GetDepthStencilAttachment();

        for (Turbo::Render::TColorImage &color_image_item : color_images)
        {
            bool was_in_result = false;

            for (Turbo::Render::TImage &result_image_item : result)
            {
                if (color_image_item == result_image_item)
                {
                    was_in_result = true;
                    break;
                }
            }

            if (!was_in_result)
            {
                result.push_back(color_image_item);
            }
        }

        for (Turbo::Render::TImage &input_image_item : input_images)
        {
            bool was_in_result = false;

            for (Turbo::Render::TImage &result_image_item : result)
            {
                if (input_image_item == result_image_item)
                {
                    was_in_result = true;
                    break;
                }
            }

            if (!was_in_result)
            {
                result.push_back(input_image_item);
            }
        }

        {
            bool was_in_result = false;

            for (Turbo::Render::TImage &result_image_item : result)
            {
                if (depth_stencil_image == result_image_item)
                {
                    was_in_result = true;
                    break;
                }
            }

            if (!was_in_result)
            {
                result.push_back(depth_stencil_image);
            }
        }
    }

    return result;
}

bool Turbo::Render::TRenderPass::IsValid() const
{
    if (this->renderPass != nullptr && this->renderPass->GetVkRenderPass() != VK_NULL_HANDLE && this->framebuffer != nullptr && this->framebuffer->GetVkFramebuffer() != VK_NULL_HANDLE)
    {
        return true;
    }

    return false;
}
