#include "TRenderPass.h"
#include "TImage.h"

Turbo::Render::TSubpass &Turbo::Render::TSubpass::AddColorAttachment(const Turbo::Render::TColorImage &colorImage)
{
    this->colors.push_back(colorImage);
    return *this;
}

Turbo::Render::TSubpass &Turbo::Render::TSubpass::AddInputAttachment(const Turbo::Render::TImage &image)
{
    this->inputs.push_back(image);
    return *this;
}

Turbo::Render::TSubpass &Turbo::Render::TSubpass::SetDepthStencilAttachment(const Turbo::Render::TDepthStencilImage &depthStencil)
{
    this->depthStencil = depthStencil;
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
