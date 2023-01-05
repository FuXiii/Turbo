#pragma once
#ifndef TURBO_RENDER_TRENDERPASS_H
#define TURBO_RENDER_TRENDERPASS_H
#include "TImage.h"
#include <stdint.h>
#include <vector>

namespace Turbo
{
namespace Render
{

class TSubpass
{
  private:
    std::vector<Turbo::Render::TColorImage> colors;
    std::vector<Turbo::Render::TImage> inputs;
    Turbo::Render::TDepthStencilImage depthStencil;

  public:
    TSubpass() = default;
    ~TSubpass() = default;

    TSubpass &AddColorAttachment(const Turbo::Render::TColorImage &colorImage);
    TSubpass &AddInputAttachment(const Turbo::Render::TImage &image);
    TSubpass &SetDepthStencilAttachment(const Turbo::Render::TDepthStencilImage &depthStencil);

    const std::vector<Turbo::Render::TColorImage> &GetColorAttachments();
    const std::vector<Turbo::Render::TImage> &GetInputAttachments();
    Turbo::Render::TDepthStencilImage GetDepthStencilAttachment();
};

class TRenderPass
{
  private:
    std::vector<TSubpass> subpasses;

  public:
    TRenderPass() = default;
    ~TRenderPass() = default;

    TRenderPass &AddSubpass(const Turbo::Render::TSubpass &subpass);
    const std::vector<Turbo::Render::TSubpass> &GetSubpasses();
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TRENDERPASS_H