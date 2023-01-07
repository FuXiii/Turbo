#include "render/include/TPipeline.h"
#include <core/include/TException.h>

Turbo::Render::TComputePipeline &Turbo::Render::TComputePipeline::SetComputeShader(Turbo::Render::TComputeShader *computeShader)
{
    if (computeShader != nullptr)
    {
        this->computeShader = computeShader;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TComputePipeline::TComputePipeline", "please ensure ComputeShader is valid");
    }

    return *this;
}
