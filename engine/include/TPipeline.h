#pragma once
#ifndef TPIPELINE_H
#define TPIPELINE_H
#include "TFormatInfo.h"
#include "TScissor.h"
#include "TViewport.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TRenderPass;
class TShader;

typedef enum class TPipelineType
{
    Graphics,
    Compute
} TPipelineType;

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

class TPipeline : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRenderPass *renderPass = nullptr;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_HANDLE VkPipeline vkPipeline = VK_NULL_HANDLE;
    // VkPipeline will create in RenderPass and exist in Subpass
    // T_VULKAN_HANDLE_HANDLE VkPipeline vkPipeline = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TShader *> shaders;

    TPipelineType type;
    VkPrimitiveTopology primitiveTopology;
    bool primitiveRestartEnable;

    std::vector<TVertexBinding> vertexBindings;

    std::vector<TViewport> viewports;
    std::vector<TScissor> scissors;

    bool depthClampEnable;
    bool rasterizerDiscardEnable;
    VkPolygonMode polygonMode;
    VkCullModeFlags cullMode;
    VkFrontFace frontFace;
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
    TPipeline(TRenderPass *renderPass, uint32_t subpass, TPipelineType type, VkPrimitiveTopology primitiveTopology, bool primitiveRestartEnable, std::vector<TVertexBinding> &vertexBindings, std::vector<TViewport> &viewports, std::vector<TScissor> &scissors, bool depthClampEnable, bool rasterizerDiscardEnable, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, bool depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, std::vector<TShader *> &shaders);
    ~TPipeline();

  public:
    VkPipelineLayout GetVkPipelineLayout();
    VkPipeline GetVkPipeline();

    TPipelineType GetType();

    VkPrimitiveTopology GetVkPrimitiveTopology();

    bool GetPrimitiveRestartEnable();

    const std::vector<TVertexBinding> &GetVertexBindings();

    std::vector<TShader *> GetShaders();

    std::vector<TViewport> GetViewports();
    std::vector<TScissor> GetScissors();

    bool GetDepthClampEnable();
    bool GetRasterizerDiscardEnable();
    VkPolygonMode GetPolygonMode();
    VkCullModeFlags GetCullMode();
    VkFrontFace GetFrontFace();
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
#endif // !TPIPELINE_H