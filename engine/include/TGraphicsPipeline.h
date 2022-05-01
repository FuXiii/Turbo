#pragma once
#ifndef TGRAPHICSPIPELINE_H
#define TGRAPHICSPIPELINE_H
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

class TGraphicsPipeline : public Turbo::Core::TPipeline
{
  private:
    T_VULKAN_HANDLE_PARENT TRenderPass *renderPass = nullptr;

    TTopologyType topology;
    bool primitiveRestartEnable;

    std::vector<TVertexBinding> vertexBindings;

    std::vector<TViewport> viewports;
    std::vector<TScissor> scissors;

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

    uint32_t subpass;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, TTopologyType topology, bool primitiveRestartEnable, std::vector<TVertexBinding> &vertexBindings, std::vector<TViewport> &viewports, std::vector<TScissor> &scissors, bool depthClampEnable, bool rasterizerDiscardEnable, TPolygonMode polygonMode, TCullModes cullMode, TFrontFace frontFace, bool depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, std::vector<TShader *> &shaders);
    ~TGraphicsPipeline();

    TTopologyType GetTopologyType();

    bool GetPrimitiveRestartEnable();

    const std::vector<TVertexBinding> &GetVertexBindings();

    std::vector<TViewport> GetViewports();
    std::vector<TScissor> GetScissors();

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