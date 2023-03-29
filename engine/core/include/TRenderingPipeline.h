#pragma once
#ifndef TURBO_CORE_TRENDERINGPIPELINE_H
#define TURBO_CORE_TRENDERINGPIPELINE_H
#include "TAttachment.h"
#include "TGraphicsPipeline.h"

namespace Turbo
{
namespace Core
{

class TImageView;

typedef enum TResolveModeBits
{
    NONE = 0,
    SAMPLE_ZERO_BIT = 0x00000001,
    AVERAGE_BIT = 0x00000002,
    MIN_BIT = 0x00000004,
    MAX_BIT = 0x00000008,
} TResolveModeBits;
// using TResolveMode = VkFlags;

class TRenderingAttachments
{
  private:
    struct TRenderingAttachment
    {
        TImageView *imageView = nullptr;
        TImageLayout layout = TImageLayout::UNDEFINED;
        TImageView *resolveImageView = nullptr;
        TImageLayout resolveLayout = TImageLayout::UNDEFINED;
        TResolveModeBits resolveModeBits = TResolveModeBits::NONE;
        TLoadOp loadOp = TLoadOp::DONT_CARE;
        TStoreOp storeOp = TStoreOp::DONT_CARE;
    };

    std::vector<TRenderingAttachment> colorAttachments;
    TRenderingAttachment depthAttachment;
    TRenderingAttachment stencilAttachment;

  public:
    TRenderingAttachments() = default;
    ~TRenderingAttachments() = default;

    void AddColorAttachment(TImageView *imageView, TImageLayout layout, TImageView *resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp);
    void AddColorAttachment(TImageView *imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp);
    void SetDepthAttachment(TImageView *imageView, TImageLayout layout, TImageView *resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp);
    void SetDepthAttachment(TImageView *imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp);
    void SetStencilAttachment(TImageView *imageView, TImageLayout layout, TImageView *resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp);
    void SetStencilAttachment(TImageView *imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp);
};

class TAttachmentsFormat
{
  private:
    std::vector<TFormatType> colorAttachmentFormats;
    TFormatType depthAttachmentFormat = TFormatType::UNDEFINED;
    TFormatType stencilAttachmentFormat = TFormatType::UNDEFINED;

  public:
    TAttachmentsFormat() = default;
    ~TAttachmentsFormat() = default;

    void AddColorAttachmentFormat(TFormatType formatType);
    void SetDepthAttachmentFormat(TFormatType formatType);
    void SetStencilAttachmentFormat(TFormatType formatType);

    const std::vector<TFormatType> &GetColorAttachmentFormats() const;
    const TFormatType &GetDepthAttachmentFormat() const;
    const TFormatType &GetStencilAttachmentFormat() const;
};

class TRenderingPipeline : public Turbo::Core::TPipeline
{
  private:
    TAttachmentsFormat renderingAttachments;

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

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TRenderingPipeline(const TAttachmentsFormat &renderingAttachments, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TTopologyType topology = TTopologyType::TRIANGLE_LIST, bool primitiveRestartEnable = false, bool depthClampEnable = false, bool rasterizerDiscardEnable = false, TPolygonMode polygonMode = TPolygonMode::FILL, TCullModes cullMode = TCullModeBits::MODE_BACK_BIT, TFrontFace frontFace = TFrontFace::CLOCKWISE, bool depthBiasEnable = false, float depthBiasConstantFactor = 0, float depthBiasClamp = 0, float depthBiasSlopeFactor = 0, float lineWidth = 1, bool multisampleEnable = false, TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT, bool depthTestEnable = true, bool depthWriteEnable = true, TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, TStencilOp frontFailOp = TStencilOp::KEEP, TStencilOp frontPassOp = TStencilOp::KEEP, TStencilOp frontDepthFailOp = TStencilOp::KEEP, TCompareOp frontCompareOp = TCompareOp::ALWAYS, uint32_t frontCompareMask = 0, uint32_t frontWriteMask = 0, uint32_t frontReference = 0, TStencilOp backFailOp = TStencilOp::KEEP, TStencilOp backPassOp = TStencilOp::KEEP, TStencilOp backDepthFailOp = TStencilOp::KEEP, TCompareOp backCompareOp = TCompareOp::ALWAYS, uint32_t backCompareMask = 0, uint32_t backWriteMask = 0, uint32_t backReference = 0, float minDepthBounds = 0, float maxDepthBounds = 0, bool logicOpEnable = false, TLogicOp logicOp = TLogicOp::NO_OP, bool blendEnable = false, TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO, TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO, TBlendOp colorBlendOp = TBlendOp::ADD, TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO, TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO, TBlendOp alphaBlendOp = TBlendOp::ADD, float constantR = 1, float constantG = 1, float constantB = 1, float constantA = 1);
    // Pipeline Cache version
    // TRenderingPipeline(TPipelineCache* pipelineCache, const TRenderingAttachments &renderingAttachments, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TTopologyType topology = TTopologyType::TRIANGLE_LIST, bool primitiveRestartEnable = false, bool depthClampEnable = false, bool rasterizerDiscardEnable = false, TPolygonMode polygonMode = TPolygonMode::FILL, TCullModes cullMode = TCullModeBits::MODE_BACK_BIT, TFrontFace frontFace = TFrontFace::CLOCKWISE, bool depthBiasEnable = false, float depthBiasConstantFactor = 0, float depthBiasClamp = 0, float depthBiasSlopeFactor = 0, float lineWidth = 1, bool multisampleEnable = false, TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT, bool depthTestEnable = true, bool depthWriteEnable = true, TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL, bool depthBoundsTestEnable = false, bool stencilTestEnable = false, TStencilOp frontFailOp = TStencilOp::KEEP, TStencilOp frontPassOp = TStencilOp::KEEP, TStencilOp frontDepthFailOp = TStencilOp::KEEP, TCompareOp frontCompareOp = TCompareOp::ALWAYS, uint32_t frontCompareMask = 0, uint32_t frontWriteMask = 0, uint32_t frontReference = 0, TStencilOp backFailOp = TStencilOp::KEEP, TStencilOp backPassOp = TStencilOp::KEEP, TStencilOp backDepthFailOp = TStencilOp::KEEP, TCompareOp backCompareOp = TCompareOp::ALWAYS, uint32_t backCompareMask = 0, uint32_t backWriteMask = 0, uint32_t backReference = 0, float minDepthBounds = 0, float maxDepthBounds = 0, bool logicOpEnable = false, TLogicOp logicOp = TLogicOp::NO_OP, bool blendEnable = false, TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO, TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO, TBlendOp colorBlendOp = TBlendOp::ADD, TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO, TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO, TBlendOp alphaBlendOp = TBlendOp::ADD, float constantR = 1, float constantG = 1, float constantB = 1, float constantA = 1);
    ~TRenderingPipeline();
};
} // namespace Core
} // namespace Turbo

#endif