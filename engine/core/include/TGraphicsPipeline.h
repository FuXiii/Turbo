#pragma once
#ifndef TGRAPHICSPIPELINE_H
#define TGRAPHICSPIPELINE_H
#include "TImage.h"
#include "TPipeline.h"

namespace Turbo
{
namespace Core
{
class TRenderPass;
class TShader;

class TVertexAttribute : public Turbo::Core::TInfo
{
  private:
    uint32_t location;
    TFormatInfo format;
    uint32_t offset;

  public:
    TVertexAttribute(uint32_t location, TFormatInfo format, uint32_t offset);
    ~TVertexAttribute();

    uint32_t GetLocation();
    TFormatInfo GetFormat();
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

    void AddAttribute(uint32_t location, TFormatInfo format, uint32_t offset);
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

    uint32_t subpass;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, std::vector<TShader *> &shaders, TTopologyType topology = TTopologyType::TRIANGLE_LIST, bool primitiveRestartEnable = false, bool depthClampEnable = false, bool rasterizerDiscardEnable = false, TPolygonMode polygonMode = TPolygonMode::FILL, TCullModes cullMode = TCullModeBits::MODE_BACK_BIT, TFrontFace frontFace = TFrontFace::CLOCKWISE, bool depthBiasEnable = false, float depthBiasConstantFactor = 0, float depthBiasClamp = 0, float depthBiasSlopeFactor = 0, float lineWidth = 1, bool multisampleEnable = false, TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT, bool depthTestEnable = true, bool depthWriteEnable = true, TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, TStencilOp frontFailOp = TStencilOp::KEEP, TStencilOp frontPassOp = TStencilOp::KEEP, TStencilOp frontDepthFailOp = TStencilOp::KEEP, TCompareOp frontCompareOp = TCompareOp::ALWAYS, uint32_t frontCompareMask = 0, uint32_t frontWriteMask = 0, uint32_t frontReference = 0, TStencilOp backFailOp = TStencilOp::KEEP, TStencilOp backPassOp = TStencilOp::KEEP, TStencilOp backDepthFailOp = TStencilOp::KEEP, TCompareOp backCompareOp = TCompareOp::ALWAYS, uint32_t backCompareMask = 0, uint32_t backWriteMask = 0, uint32_t backReference = 0, float minDepthBounds = 0, float maxDepthBounds = 0);
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
#endif // !TGRAPHICSPIPELINE_H