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

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetPatchControlPoints(uint32_t patchControlPoints)
{
    this->patchControlPoints = patchControlPoints;
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

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthTestEnable(bool depthTestEnable)
{
    this->depthTestEnable = depthTestEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthWriteEnable(bool depthWriteEnable)
{
    this->depthWriteEnable = depthWriteEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthCompareOp(TCompareOp depthCompareOp)
{
    this->depthCompareOp = depthCompareOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDepthBoundsTestEnable(bool depthBoundsTestEnable)
{
    this->depthBoundsTestEnable = depthBoundsTestEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilTestEnable(bool stencilTestEnable)
{
    this->stencilTestEnable = stencilTestEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontFailOp(TStencilOp frontFailOp)
{
    this->frontFailOp = frontFailOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontPassOp(TStencilOp frontPassOp)
{
    this->frontPassOp = frontPassOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontDepthFailOp(TStencilOp frontDepthFailOp)
{
    this->frontDepthFailOp = frontDepthFailOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontCompareOp(TCompareOp frontCompareOp)
{
    this->frontCompareOp = frontCompareOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontCompareMask(uint32_t frontCompareMask)
{
    this->frontCompareMask = frontCompareMask;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontWriteMask(uint32_t frontWriteMask)
{
    this->frontWriteMask = frontWriteMask;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilFrontReference(uint32_t frontReference)
{
    this->frontReference = frontReference;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackFailOp(TStencilOp backFailOp)
{
    this->backFailOp = backFailOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackPassOp(TStencilOp backPassOp)
{
    this->backPassOp = backPassOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackDepthFailOp(TStencilOp backDepthFailOp)
{
    this->backDepthFailOp = backDepthFailOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackCompareOp(TCompareOp backCompareOp)
{
    this->backCompareOp = backCompareOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackCompareMask(uint32_t backCompareMask)
{
    this->backCompareMask = backCompareMask;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackWriteMask(uint32_t backWriteMask)
{
    this->backWriteMask = backWriteMask;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetStencilBackReference(uint32_t backReference)
{
    this->backReference = backReference;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetMinDepthBounds(float minDepthBounds)
{
    this->minDepthBounds = minDepthBounds;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetMaxDepthBounds(float maxDepthBounds)
{
    this->maxDepthBounds = maxDepthBounds;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetLogicOpEnable(bool logicOpEnable)
{
    this->logicOpEnable = logicOpEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetLogicOp(TLogicOp logicOp)
{
    this->logicOp = logicOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetBlendEnable(bool blendEnable)
{
    this->blendEnable = blendEnable;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetSrcColorBlendFactor(TBlendFactor srcColorBlendFactor)
{
    this->srcColorBlendFactor = srcColorBlendFactor;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDstColorBlendFactor(TBlendFactor dstColorBlendFactor)
{
    this->dstColorBlendFactor = dstColorBlendFactor;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetColorBlendOp(TBlendOp colorBlendOp)
{
    this->colorBlendOp = colorBlendOp;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetSrcAlphaBlendFactor(TBlendFactor srcAlphaBlendFactor)
{
    this->srcAlphaBlendFactor = srcAlphaBlendFactor;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetDstAlphaBlendFactor(TBlendFactor dstAlphaBlendFactor)
{
    this->dstAlphaBlendFactor = dstAlphaBlendFactor;
    return *this;
}

Turbo::Render::TGraphicsPipeline &Turbo::Render::TGraphicsPipeline::SetAlphaBlendOp(TBlendOp alphaBlendOp)
{
    this->alphaBlendOp = alphaBlendOp;
    return *this;
}
