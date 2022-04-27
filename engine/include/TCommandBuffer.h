#pragma once
#ifndef TCOMMANDBUFFER_H
#define TCOMMANDBUFFER_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{

class TCommandBufferPool;
class TRenderPass;
class TFramebuffer;
class TPipeline;
class TBuffer;
class TViewport;
class TScissor;
class TDescriptorSet;

class TCommandBuffer : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TCommandBufferPool;

  private:
    T_VULKAN_HANDLE_PARENT TCommandBufferPool *commandBufferPool = nullptr;
    T_VULKAN_HANDLE_HANDLE VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN; //??? 

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TCommandBuffer(TCommandBufferPool *commandBufferPool);
    ~TCommandBuffer();

    VkCommandBuffer GetVkCommandBuffer();

  public:
    bool Begin();
    void BeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t offsetX = 0, uint32_t offsetY = 0, uint32_t width = TURBO_WHOLE_EXTENT, uint32_t height = TURBO_WHOLE_EXTENT);
    void BindPipeline(TPipeline *pipeline);
    void BindDescriptorSets(TPipeline *pipeline, uint32_t firstSet, std::vector<TDescriptorSet *>& descriptorSets);
    void BindVertexBuffers(std::vector<TBuffer *> &vertexBuffers);
    void SetViewport(std::vector<TViewport> &viewports);
    void SetScissor(std::vector<TScissor> &scissors);
    void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void NextSubpass();
    void EndRenderPass();
    bool End();
    bool Reset();

    void PipelineBarrier();
    void BindIndexBuffer();
    void DrawIndexed();
    void DrawIndexedIndirect();
    void DrawIndirect();

    void SetLineWidth();
    void ExecuteCommands();

    void FillBuffer();

    void BlitImage();
    void ClearAttachments();
    void ClearColorImage();
    void ClearDepthStencilImage();
    void CopyBuffer();
    void CopyBufferToImage();
    void CopyImage();
    void CopyImageToBuffer();

    void Dispatch();
    void DispatchIndirect();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif // !TCOMMANDBUFFER_H