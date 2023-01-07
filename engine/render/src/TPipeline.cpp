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

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetVertexShader(Turbo::Render::TVertexShader *vertexShader)
{
    if (vertexShader != nullptr)
    {
        this->vertexShader = vertexShader;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TGraphicsPipeline::SetVertexShader", "please ensure VertexShader is valid");
    }

    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetFragmentShader(Turbo::Render::TFragmentShader *fragmentShader)
{
    if (fragmentShader != nullptr)
    {
        this->fragmentShader = fragmentShader;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TGraphicsPipeline::SetFragmentShader", "please ensure FragmentShader is valid");
    }

    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetTopology(TTopology topology)
{
    this->topology = topology;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetPrimitiveRestartEnable(bool primitiveRestartEnable)
{
    this->primitiveRestartEnable = primitiveRestartEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthClampEnable(bool depthClampEnable)
{
    this->depthClampEnable = depthClampEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetRasterizerDiscardEnable(bool rasterizerDiscardEnable)
{
    this->rasterizerDiscardEnable = rasterizerDiscardEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetPolygon(TPolygon polygon)
{
    this->polygon = polygon;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetCull(TCull cull)
{
    this->cull = cull;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetFront(TFront front)
{
    this->front = front;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthBiasEnable(bool depthBiasEnable)
{
    this->depthBiasEnable = depthBiasEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthBiasConstantFactor(float depthBiasConstantFactor)
{
    this->depthBiasConstantFactor = depthBiasConstantFactor;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthBiasClamp(float depthBiasClamp)
{
    this->depthBiasClamp = depthBiasClamp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthBiasSlopeFactor(float depthBiasSlopeFactor)
{
    this->depthBiasSlopeFactor = depthBiasSlopeFactor;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetLineWidth(float lineWidth)
{
    this->lineWidth = lineWidth;
    return *this;
}