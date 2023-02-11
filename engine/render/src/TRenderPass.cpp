#include "render/include/TRenderPass.h"
#include "render/include/TImage.h"
#include <core/include/TCore.h>
#include <core/include/TException.h>
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

bool Turbo::Render::TRenderPass::IsValid()
{
    if (this->renderPass != nullptr && this->renderPass->GetVkRenderPass() != VK_NULL_HANDLE)
    {
        return true;
    }

    return false;
}
