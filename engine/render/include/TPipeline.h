#pragma once
#ifndef TURBO_RENDER_TPIPELINE_H
#define TURBO_RENDER_TPIPELINE_H

namespace Turbo
{
namespace Render
{
class TComputeShader;
class TVertexShader;
class TFragmentShader;

class TPipeline
{
  public:
    TPipeline() = default;
    ~TPipeline() = default;
};

class TComputePipeline : public TPipeline
{
  private:
    Turbo::Render::TComputeShader *computeShader = nullptr;

  public:
    TComputePipeline() = default;
    ~TComputePipeline() = default;

    TComputePipeline &SetComputeShader(Turbo::Render::TComputeShader *computeShader);
};

class TGraphicsPipeline : public TPipeline
{
  private:
    Turbo::Render::TVertexShader *vertexShader = nullptr;
    Turbo::Render::TFragmentShader *fragmentShader = nullptr;

  public:
    TGraphicsPipeline() = default;
    ~TGraphicsPipeline() = default;

    TGraphicsPipeline &SetVertexShader(Turbo::Render::TVertexShader *vertexShader);
    TGraphicsPipeline &SetFragmentShader(Turbo::Render::TFragmentShader *fragmentShader);
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H