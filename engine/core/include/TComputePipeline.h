#pragma once
#ifndef TURBO_CORE_TCOMPUTEPIPELINE_H
#define TURBO_CORE_TCOMPUTEPIPELINE_H
#include "TPipeline.h"

namespace Turbo
{
namespace Core
{
class TRenderPass;
class TComputeShader;

class TComputePipeline : public Turbo::Core::TPipeline
{
  private:
    T_VULKAN_HANDLE_HANDLE

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    //[[deprecated]] TComputePipeline(TComputeShader *computeShader);
    //[[deprecated]] TComputePipeline(TPipelineCache *pipelineCache, TComputeShader *computeShader);

    TComputePipeline(const TPipelineLayout::TLayout &layout, TComputeShader *computeShader, const TPipeline::TSpecializationConstants &specializationConstants = {}, TPipelineCache *pipelineCache = nullptr); // NOTE: new!

  protected:
    virtual ~TComputePipeline();

  public:
    virtual std::string ToString() const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TCOMPUTEPIPELINE_H