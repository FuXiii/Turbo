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
    typedef enum TTopology
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

    typedef enum TPolygon
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

    typedef enum TFront
    {
        COUNTER_CLOCKWISE = 0,
        CLOCKWISE = 1,
    } TFront;

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

    bool depthBiasEnable = false;
    float depthBiasConstantFactor = 0;
    float depthBiasClamp = 0;
    float depthBiasSlopeFactor = 0;
    float lineWidth = 1;

  public:
    TGraphicsPipeline() = default;
    ~TGraphicsPipeline() = default;

    TGraphicsPipeline &SetVertexShader(Turbo::Render::TVertexShader *vertexShader);
    TGraphicsPipeline &SetFragmentShader(Turbo::Render::TFragmentShader *fragmentShader);

    TGraphicsPipeline &SetTopology(TTopology topology);
    TGraphicsPipeline &SetPrimitiveRestartEnable(bool primitiveRestartEnable);
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
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H