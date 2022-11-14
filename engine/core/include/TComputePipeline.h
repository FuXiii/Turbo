#pragma once
#ifndef TCOMPUTEPIPELINE_H
#define TCOMPUTEPIPELINE_H
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
    TComputePipeline(TComputeShader *computeShader);
    ~TComputePipeline();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TCOMPUTEPIPELINE_H