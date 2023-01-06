#pragma once
#ifndef TURBO_RENDER_TPIPELINE_H
#define TURBO_RENDER_TPIPELINE_H

namespace Turbo
{
namespace Render
{
class TPipeline
{
  public:
    TPipeline() = default;
    ~TPipeline() = default;
};

class TComputePipeline : public TPipeline
{
  private:
    // TODO: TComputeShader* computeShader=nullptr;
  public:
    TComputePipeline() = default;
    // TODO: TComputePipeline(TComputeShader* computeShader);
    ~TComputePipeline() = default;
};

class TGraphicsPipeline : public TPipeline
{
  public:
    TGraphicsPipeline() = default;
    ~TGraphicsPipeline() = default;
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H