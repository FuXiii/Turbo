#pragma once
#ifndef TPIPELINE_H
#define TPIPELINE_H
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

    std::vector<VkVertexInputBindingDescription> vkVertexInputBindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> vkVertexInputAttributeDescriptions;

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
    TPipeline(TRenderPass *renderPass, uint32_t subpass, TPipelineType type, VkPrimitiveTopology primitiveTopology, bool primitiveRestartEnable, std::vector<VkVertexInputBindingDescription> &vkVertexInputBindingDescriptions, std::vector<VkVertexInputAttributeDescription> vkVertexInputAttributeDescriptions, std::vector<TViewport> &viewports, std::vector<TScissor> &scissors, bool depthClampEnable, bool rasterizerDiscardEnable, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, bool depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, std::vector<TShader *> &shaders);
    ~TPipeline();

  public:
    VkPipelineLayout GetVkPipelineLayout();
    VkPipeline GetVkPipeline();

    TPipelineType GetType();

    VkPrimitiveTopology GetVkPrimitiveTopology();

    bool GetPrimitiveRestartEnable();

    std::vector<VkVertexInputBindingDescription> GetVkVertexInputBindingDescriptions();
    std::vector<VkVertexInputAttributeDescription> GetVkVertexInputAttributeDescriptions();

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