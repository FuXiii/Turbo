#pragma once
#ifndef TURBO_RENDER_TPIPELINE_H
#define TURBO_RENDER_TPIPELINE_H
#include <stdint.h>

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
  public:
    typedef enum class TTopology
    {
        POINT_LIST = 0,
        LINE_LIST = 1,
        LINE_STRIP = 2,
        TRIANGLE_LIST = 3,
        TRIANGLE_STRIP = 4,
        TRIANGLE_FAN = 5,
        LINE_LIST_WITH_ADJACENCY = 6,
        LINE_STRIP_WITH_ADJACENCY = 7,
        TRIANGLE_LIST_WITH_ADJACENCY = 8,
        TRIANGLE_STRIP_WITH_ADJACENCY = 9,
        PATCH_LIST = 10,
    } TTopology;

    typedef enum class TPolygon
    {
        FILL = 0,
        LINE = 1,
        POINT = 2,
    } TPolygon;

    typedef enum TCullBits
    {
        NONE = 0,
        FRONT_BIT = 0x00000001,
        BACK_BIT = 0x00000002,
        FRONT_AND_BACK = 0x00000003,
    } TCullBits;
    using TCull = uint32_t;

    typedef enum class TFront
    {
        COUNTER_CLOCKWISE = 0,
        CLOCKWISE = 1,
    } TFront;

    typedef enum class TCompareOp
    {
        NEVER = 0,
        LESS = 1,
        EQUAL = 2,
        LESS_OR_EQUAL = 3,
        GREATER = 4,
        NOT_EQUAL = 5,
        GREATER_OR_EQUAL = 6,
        ALWAYS = 7
    } TCompareOp;

    typedef enum class TStencilOp
    {
        KEEP = 0,
        ZERO = 1,
        REPLACE = 2,
        INCREMENT_AND_CLAMP = 3,
        DECREMENT_AND_CLAMP = 4,
        INVERT = 5,
        INCREMENT_AND_WRAP = 6,
        DECREMENT_AND_WRAP = 7
    } TStencilOp;

    typedef enum class TLogicOp
    {
        CLEAR = 0,
        AND = 1,
        AND_REVERSE = 2,
        COPY = 3,
        AND_INVERTED = 4,
        NO_OP = 5,
        XOR = 6,
        OR = 7,
        NOR = 8,
        EQUIVALENT = 9,
        INVERT = 10,
        OR_REVERSE = 11,
        COPY_INVERTED = 12,
        OR_INVERTED = 13,
        NAND = 14,
        SET = 15,
    } TLogicOp;

    typedef enum class TBlendFactor
    {
        ZERO = 0,
        ONE = 1,
        SRC_COLOR = 2,
        ONE_MINUS_SRC_COLOR = 3,
        DST_COLOR = 4,
        ONE_MINUS_DST_COLOR = 5,
        SRC_ALPHA = 6,
        ONE_MINUS_SRC_ALPHA = 7,
        DST_ALPHA = 8,
        ONE_MINUS_DST_ALPHA = 9,
        CONSTANT_COLOR = 10,
        ONE_MINUS_CONSTANT_COLOR = 11,
        CONSTANT_ALPHA = 12,
        ONE_MINUS_CONSTANT_ALPHA = 13,
        SRC_ALPHA_SATURATE = 14,
        SRC1_COLOR = 15,
        ONE_MINUS_SRC1_COLOR = 16,
        SRC1_ALPHA = 17,
        ONE_MINUS_SRC1_ALPHA = 18,
    } TBlendFactor;

    typedef enum class TBlendOp
    {
        ADD = 0,
        SUBTRACT = 1,
        REVERSE_SUBTRACT = 2,
        MIN = 3,
        MAX = 4,
    } TBlendOp;

  private:
    Turbo::Render::TVertexShader *vertexShader = nullptr;
    Turbo::Render::TFragmentShader *fragmentShader = nullptr;

    TTopology topology = TTopology::TRIANGLE_LIST;
    bool primitiveRestartEnable = false;
    bool depthClampEnable = false;
    bool rasterizerDiscardEnable = false;
    TPolygon polygon = TPolygon::FILL;
    TCull cull = TCullBits::BACK_BIT;
    TFront front = TFront::CLOCKWISE;

    uint32_t patchControlPoints;

    bool depthBiasEnable = false;
    float depthBiasConstantFactor = 0;
    float depthBiasClamp = 0;
    float depthBiasSlopeFactor = 0;
    float lineWidth = 1;

    // bool multisampleEnable //maintenance by Turbo
    // TSampleCountBits sample //maintenance by Turbo

    bool depthTestEnable = true;
    bool depthWriteEnable = true;
    TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL;
    bool depthBoundsTestEnable = false;
    bool stencilTestEnable = false;
    // VkPipelineDepthStencilStateCreateInfo::VkStencilOpState front;
    TStencilOp frontFailOp = TStencilOp::KEEP;
    TStencilOp frontPassOp = TStencilOp::KEEP;
    TStencilOp frontDepthFailOp = TStencilOp::KEEP;
    TCompareOp frontCompareOp = TCompareOp::ALWAYS;
    uint32_t frontCompareMask = 0;
    uint32_t frontWriteMask = 0;
    uint32_t frontReference = 0;
    // VkPipelineDepthStencilStateCreateInfo::VkStencilOpState back;
    TStencilOp backFailOp = TStencilOp::KEEP;
    TStencilOp backPassOp = TStencilOp::KEEP;
    TStencilOp backDepthFailOp = TStencilOp::KEEP;
    TCompareOp backCompareOp = TCompareOp::ALWAYS;
    uint32_t backCompareMask = 0;
    uint32_t backWriteMask = 0;
    uint32_t backReference = 0;

    float minDepthBounds = 0;
    float maxDepthBounds = 0;

    // VkPipelineColorBlendStateCreateInfo
    bool logicOpEnable = false;
    TLogicOp logicOp = TLogicOp::NO_OP;

    // VkPipelineColorBlendStateCreateInfo::pAttachments;
    bool blendEnable = false;
    TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO;
    TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO;
    TBlendOp colorBlendOp = TBlendOp::ADD;
    TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO;
    TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO;
    TBlendOp alphaBlendOp = TBlendOp::ADD;

  public:
    TGraphicsPipeline() = default;
    ~TGraphicsPipeline() = default;

    // VkPipelineShaderStageCreateInfo
    TGraphicsPipeline &SetVertexShader(Turbo::Render::TVertexShader *vertexShader);
    TGraphicsPipeline &SetFragmentShader(Turbo::Render::TFragmentShader *fragmentShader);

    // VkPipelineInputAssemblyStateCreateInfo
    TGraphicsPipeline &SetTopology(TTopology topology);
    TGraphicsPipeline &SetPrimitiveRestartEnable(bool primitiveRestartEnable);

    // VkPipelineTessellationStateCreateInfo
    // TODO:注：Turbo::Core目前不支持该属性，需要Turbo::Core提供相关实现
    TGraphicsPipeline &SetPatchControlPoints(uint32_t patchControlPoints);

    // VkPipelineRasterizationStateCreateInfo
    TGraphicsPipeline &SetDepthClampEnable(bool depthClampEnable);
    TGraphicsPipeline &SetRasterizerDiscardEnable(bool rasterizerDiscardEnable);
    TGraphicsPipeline &SetPolygon(TPolygon polygon);
    TGraphicsPipeline &SetCull(TCull cull);
    TGraphicsPipeline &SetFront(TFront front);
    TGraphicsPipeline &SetDepthBiasEnable(bool depthBiasEnable);
    TGraphicsPipeline &SetDepthBiasConstantFactor(float depthBiasConstantFactor);
    TGraphicsPipeline &SetDepthBiasClamp(float depthBiasClamp);
    TGraphicsPipeline &SetDepthBiasSlopeFactor(float depthBiasSlopeFactor);
    TGraphicsPipeline &SetLineWidth(float lineWidth);

    // VkPipelineDepthStencilStateCreateInfo
    TGraphicsPipeline &SetDepthTestEnable(bool depthTestEnable);
    TGraphicsPipeline &SetDepthWriteEnable(bool depthWriteEnable);
    TGraphicsPipeline &SetDepthCompareOp(TCompareOp depthCompareOp);
    TGraphicsPipeline &SetDepthBoundsTestEnable(bool depthBoundsTestEnable);
    TGraphicsPipeline &SetStencilTestEnable(bool stencilTestEnable);

    TGraphicsPipeline &SetStencilFrontFailOp(TStencilOp frontFailOp);
    TGraphicsPipeline &SetStencilFrontPassOp(TStencilOp frontPassOp);
    TGraphicsPipeline &SetStencilFrontDepthFailOp(TStencilOp frontDepthFailOp);
    TGraphicsPipeline &SetStencilFrontCompareOp(TCompareOp frontCompareOp);
    TGraphicsPipeline &SetStencilFrontCompareMask(uint32_t frontCompareMask);
    TGraphicsPipeline &SetStencilFrontWriteMask(uint32_t frontWriteMask);
    TGraphicsPipeline &SetStencilFrontReference(uint32_t frontReference);

    TGraphicsPipeline &SetStencilBackFailOp(TStencilOp backFailOp);
    TGraphicsPipeline &SetStencilBackPassOp(TStencilOp backPassOp);
    TGraphicsPipeline &SetStencilBackDepthFailOp(TStencilOp backDepthFailOp);
    TGraphicsPipeline &SetStencilBackCompareOp(TCompareOp backCompareOp);
    TGraphicsPipeline &SetStencilBackCompareMask(uint32_t backCompareMask);
    TGraphicsPipeline &SetStencilBackWriteMask(uint32_t backWriteMask);
    TGraphicsPipeline &SetStencilBackReference(uint32_t backReference);

    TGraphicsPipeline &SetMinDepthBounds(float minDepthBounds);
    TGraphicsPipeline &SetMaxDepthBounds(float maxDepthBounds);

    // VkPipelineColorBlendStateCreateInfo
    TGraphicsPipeline &SetLogicOpEnable(bool logicOpEnable);
    TGraphicsPipeline &SetLogicOp(TLogicOp logicOp);
    TGraphicsPipeline &SetBlendEnable(bool blendEnable);
    TGraphicsPipeline &SetSrcColorBlendFactor(TBlendFactor srcColorBlendFactor);
    TGraphicsPipeline &SetDstColorBlendFactor(TBlendFactor dstColorBlendFactor);
    TGraphicsPipeline &SetColorBlendOp(TBlendOp colorBlendOp);
    TGraphicsPipeline &SetSrcAlphaBlendFactor(TBlendFactor srcAlphaBlendFactor);
    TGraphicsPipeline &SetDstAlphaBlendFactor(TBlendFactor dstAlphaBlendFactor);
    TGraphicsPipeline &SetAlphaBlendOp(TBlendOp alphaBlendOp);
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H