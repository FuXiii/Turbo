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
    TComputePipeline(Turbo::Render::TComputeShader *computeShader);
    ~TComputePipeline() = default;
};

class TGraphicsPipeline : public TPipeline
{
  public:
    TGraphicsPipeline() = default;
    // TODO::TGraphicsPipeline(vertexShader, fragmentShader)
    ~TGraphicsPipeline() = default;
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H