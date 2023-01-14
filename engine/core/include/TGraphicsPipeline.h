#pragma once
#ifndef TURBO_CORE_TGRAPHICSPIPELINE_H
#define TURBO_CORE_TGRAPHICSPIPELINE_H
#include "TImage.h"
#include "TPipeline.h"

namespace Turbo
{
namespace Core
{
class TRenderPass;
class TShader;
class TVertexShader;
class TFragmentShader;

class TVertexAttribute : public Turbo::Core::TInfo
{
  private:
    uint32_t location;
    TFormatType formatType;
    uint32_t offset;

  public:
    TVertexAttribute(uint32_t location, TFormatType formatType, uint32_t offset);
    ~TVertexAttribute();

    uint32_t GetLocation();
    TFormatType GetFormatType();
    uint32_t GetOffset();

  public:
    virtual std::string ToString() override;
};

typedef enum class TVertexRate
{
    VERTEX = 0,
    INSTANCE = 1
} TVertexRate;

class TVertexBinding : public Turbo::Core::TInfo
{
  private:
    uint32_t binding;
    uint32_t stride;
    TVertexRate rate;
    std::vector<TVertexAttribute> vertexAttributes;

  public:
    TVertexBinding(uint32_t binding, uint32_t stride, TVertexRate rate);
    ~TVertexBinding();

    [[deprecated]] void AddAttribute(uint32_t location, TFormatInfo format, uint32_t offset);
    void AddAttribute(uint32_t location, TFormatType formatType, uint32_t offset);
    void AddAttribute(TVertexAttribute &vertexAttribute);

    uint32_t GetBinding();
    uint32_t GetStride();
    TVertexRate GetVertexRate();
    const std::vector<TVertexAttribute> &GetVertexAttributes();

  public:
    virtual std::string ToString() override;
};

typedef enum class TTopologyType
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
} TTopologyType;

typedef enum class TPolygonMode
{
    FILL = 0,
    LINE = 1,
    POINT = 2,
} TPolygonMode;

typedef enum TCullModeBits
{
    MODE_NONE = 0,
    MODE_FRONT_BIT = 0x00000001,
    MODE_BACK_BIT = 0x00000002,
    MODE_FRONT_AND_BACK = 0x00000003,
} TCullModeBits;
typedef VkFlags TCullModes;

typedef enum class TFrontFace
{
    COUNTER_CLOCKWISE = 0,
    CLOCKWISE = 1,
} TFrontFace;

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

class TGraphicsPipeline : public Turbo::Core::TPipeline
{
  private:
    T_VULKAN_HANDLE_PARENT TRenderPass *renderPass = nullptr;

    TTopologyType topology;
    bool primitiveRestartEnable;

    std::vector<TVertexBinding> vertexBindings;

    bool depthClampEnable;
    bool rasterizerDiscardEnable;
    TPolygonMode polygonMode;
    TCullModes cullMode;
    TFrontFace frontFace;
    bool depthBiasEnable;
    float depthBiasConstantFactor;
    float depthBiasClamp;
    float depthBiasSlopeFactor;
    float lineWidth;

    bool multisampleEnable;
    TSampleCountBits sample;

    //<DepthStencilState>
    bool depthTestEnable;
    bool depthWriteEnable;
    TCompareOp depthCompareOp;
    bool depthBoundsTestEnable;
    bool stencilTestEnable;
    TStencilOp frontFailOp;
    TStencilOp frontPassOp;
    TStencilOp frontDepthFailOp;
    TCompareOp frontCompareOp;
    uint32_t frontCompareMask;
    uint32_t frontWriteMask;
    uint32_t frontReference;
    TStencilOp backFailOp;
    TStencilOp backPassOp;
    TStencilOp backDepthFailOp;
    TCompareOp backCompareOp;
    uint32_t backCompareMask;
    uint32_t backWriteMask;
    uint32_t backReference;
    float minDepthBounds;
    float maxDepthBounds;
    //</DepthStencilState>

    //<Blend>
    bool logicOpEnable;
    TLogicOp logicOp;
    bool blendEnable;
    TBlendFactor srcColorBlendFactor;
    TBlendFactor dstColorBlendFactor;
    TBlendOp colorBlendOp;
    TBlendFactor srcAlphaBlendFactor;
    TBlendFactor dstAlphaBlendFactor;
    TBlendOp alphaBlendOp;
    float constantR;
    float constantG;
    float constantB;
    float constantA;
    //</Blend>

    uint32_t subpass;

    //<Under development>
    VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;
    //</Under development>

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    [[deprecated]] TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, std::vector<TShader *> &shaders, TTopologyType topology = TTopologyType::TRIANGLE_LIST, bool primitiveRestartEnable = false, bool depthClampEnable = false, bool rasterizerDiscardEnable = false, TPolygonMode polygonMode = TPolygonMode::FILL, TCullModes cullMode = TCullModeBits::MODE_BACK_BIT, TFrontFace frontFace = TFrontFace::CLOCKWISE, bool depthBiasEnable = false, float depthBiasConstantFactor = 0, float depthBiasClamp = 0, float depthBiasSlopeFactor = 0, float lineWidth = 1, bool multisampleEnable = false, TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT, bool depthTestEnable = true, bool depthWriteEnable = true, TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, TStencilOp frontFailOp = TStencilOp::KEEP, TStencilOp frontPassOp = TStencilOp::KEEP, TStencilOp frontDepthFailOp = TStencilOp::KEEP, TCompareOp frontCompareOp = TCompareOp::ALWAYS, uint32_t frontCompareMask = 0, uint32_t frontWriteMask = 0, uint32_t frontReference = 0, TStencilOp backFailOp = TStencilOp::KEEP, TStencilOp backPassOp = TStencilOp::KEEP, TStencilOp backDepthFailOp = TStencilOp::KEEP, TCompareOp backCompareOp = TCompareOp::ALWAYS, uint32_t backCompareMask = 0, uint32_t backWriteMask = 0, uint32_t backReference = 0, float minDepthBounds = 0, float maxDepthBounds = 0, bool logicOpEnable = false, TLogicOp logicOp = TLogicOp::NO_OP, bool blendEnable = false, TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO, TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO, TBlendOp colorBlendOp = TBlendOp::ADD, TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO, TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO, TBlendOp alphaBlendOp = TBlendOp::ADD, float constantR = 1, float constantG = 1, float constantB = 1, float constantA = 1);
    TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TTopologyType topology = TTopologyType::TRIANGLE_LIST, bool primitiveRestartEnable = false, bool depthClampEnable = false, bool rasterizerDiscardEnable = false, TPolygonMode polygonMode = TPolygonMode::FILL, TCullModes cullMode = TCullModeBits::MODE_BACK_BIT, TFrontFace frontFace = TFrontFace::CLOCKWISE, bool depthBiasEnable = false, float depthBiasConstantFactor = 0, float depthBiasClamp = 0, float depthBiasSlopeFactor = 0, float lineWidth = 1, bool multisampleEnable = false, TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT, bool depthTestEnable = true, bool depthWriteEnable = true, TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, TStencilOp frontFailOp = TStencilOp::KEEP, TStencilOp frontPassOp = TStencilOp::KEEP, TStencilOp frontDepthFailOp = TStencilOp::KEEP, TCompareOp frontCompareOp = TCompareOp::ALWAYS, uint32_t frontCompareMask = 0, uint32_t frontWriteMask = 0, uint32_t frontReference = 0, TStencilOp backFailOp = TStencilOp::KEEP, TStencilOp backPassOp = TStencilOp::KEEP, TStencilOp backDepthFailOp = TStencilOp::KEEP, TCompareOp backCompareOp = TCompareOp::ALWAYS, uint32_t backCompareMask = 0, uint32_t backWriteMask = 0, uint32_t backReference = 0, float minDepthBounds = 0, float maxDepthBounds = 0, bool logicOpEnable = false, TLogicOp logicOp = TLogicOp::NO_OP, bool blendEnable = false, TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO, TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO, TBlendOp colorBlendOp = TBlendOp::ADD, TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO, TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO, TBlendOp alphaBlendOp = TBlendOp::ADD, float constantR = 1, float constantG = 1, float constantB = 1, float constantA = 1);
    //<Under development>
    TGraphicsPipeline(VkPipelineCache vkPipelineCache, TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TTopologyType topology = TTopologyType::TRIANGLE_LIST, bool primitiveRestartEnable = false, bool depthClampEnable = false, bool rasterizerDiscardEnable = false, TPolygonMode polygonMode = TPolygonMode::FILL, TCullModes cullMode = TCullModeBits::MODE_BACK_BIT, TFrontFace frontFace = TFrontFace::CLOCKWISE, bool depthBiasEnable = false, float depthBiasConstantFactor = 0, float depthBiasClamp = 0, float depthBiasSlopeFactor = 0, float lineWidth = 1, bool multisampleEnable = false, TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT, bool depthTestEnable = true, bool depthWriteEnable = true, TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, TStencilOp frontFailOp = TStencilOp::KEEP, TStencilOp frontPassOp = TStencilOp::KEEP, TStencilOp frontDepthFailOp = TStencilOp::KEEP, TCompareOp frontCompareOp = TCompareOp::ALWAYS, uint32_t frontCompareMask = 0, uint32_t frontWriteMask = 0, uint32_t frontReference = 0, TStencilOp backFailOp = TStencilOp::KEEP, TStencilOp backPassOp = TStencilOp::KEEP, TStencilOp backDepthFailOp = TStencilOp::KEEP, TCompareOp backCompareOp = TCompareOp::ALWAYS, uint32_t backCompareMask = 0, uint32_t backWriteMask = 0, uint32_t backReference = 0, float minDepthBounds = 0, float maxDepthBounds = 0, bool logicOpEnable = false, TLogicOp logicOp = TLogicOp::NO_OP, bool blendEnable = false, TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO, TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO, TBlendOp colorBlendOp = TBlendOp::ADD, TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO, TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO, TBlendOp alphaBlendOp = TBlendOp::ADD, float constantR = 1, float constantG = 1, float constantB = 1, float constantA = 1);
    //</Under development>
    ~TGraphicsPipeline();

    TTopologyType GetTopologyType();

    bool GetPrimitiveRestartEnable();

    const std::vector<TVertexBinding> &GetVertexBindings();

    bool GetDepthClampEnable();
    bool GetRasterizerDiscardEnable();
    TPolygonMode GetPolygonMode();
    TCullModes GetCullMode();
    TFrontFace GetFrontFace();
    bool GetDepthBiasEnable();
    float GetDepthBiasConstantFactor();
    float GetDepthBiasClamp();
    float GetDepthBiasSlopeFactor();
    float GetLineWidth();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TGRAPHICSPIPELINE_H