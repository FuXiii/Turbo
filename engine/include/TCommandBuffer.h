#pragma once
#ifndef TCOMMANDBUFFER_H
#define TCOMMANDBUFFER_H
#include "TBarrier.h"
#include "TBuffer.h"
#include "TPipeline.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{

class TCommandBufferPool;
class TRenderPass;
class TFramebuffer;
class TBuffer;
class TViewport;
class TScissor;
class TDescriptorSet;
class TPipelineDescriptorSet;

class TCommandBuffer : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TCommandBufferPool;

  private:
    T_VULKAN_HANDLE_PARENT TCommandBufferPool *commandBufferPool = nullptr;
    T_VULKAN_HANDLE_HANDLE VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN; //???

    TRenderPass *currentRenderPass = nullptr;
    TPipeline *currentPipeline = nullptr;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TCommandBuffer(TCommandBufferPool *commandBufferPool);
    ~TCommandBuffer();

    VkCommandBuffer GetVkCommandBuffer();

  public:
    bool Begin();
    void CmdBeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t offsetX = 0, uint32_t offsetY = 0, uint32_t width = TURBO_WHOLE_EXTENT, uint32_t height = TURBO_WHOLE_EXTENT);
    void CmdBindPipeline(TPipeline *pipeline);
    void CmdBindDescriptorSets(uint32_t firstSet, std::vector<TDescriptorSet *> &descriptorSets);
    void CmdBindPipelineDescriptorSet(uint32_t firstSet, TPipelineDescriptorSet *pipelineDescriptorSet);
    void CmdBindVertexBuffers(std::vector<TBuffer *> &vertexBuffers);
    void CmdSetViewport(std::vector<TViewport> &viewports);
    void CmdSetScissor(std::vector<TScissor> &scissors);
    void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void CmdNextSubpass();
    void CmdEndRenderPass();
    bool End();
    bool Reset();

    void CmdPipelineBarrier(TPipelineStages srcStages, TPipelineStages dstStages, std::vector<TMemoryBarrier> &memoryBarriers, std::vector<TBufferMemoryBarrier> &bufferBarriers, std::vector<TImageMemoryBarrier> &imageBarriers);
    void CmdPipelineMemoryBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TMemoryBarrier &memoryBarrier);
    void CmdPipelineBufferBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TBufferMemoryBarrier &bufferBarrier);
    void CmdPipelineImageBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TImageMemoryBarrier &imageBarrier);

    void CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, TImage *image, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, TImageView *imageView);

    void CmdFillBuffer(TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, uint32_t data = 0);
    void CmdFillBuffer(TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, float data = 0.0f);

    // The special value VK_WHOLE_SIZE is not accepted for the size parameter to
    // vkCmdUpdateBuffer() because it is also used as the size of the host memory region that is the
    // source of the data. The maximum size of data that can be placed in a buffer with
    // vkCmdUpdateBuffer() is 65,536 bytes.
    void CmdUpdateBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, const void *data);
    void CmdCopyBuffer(TBuffer *srcBuffer, TBuffer *dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size);

    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects);
    void CmdClearColorImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0);

    void CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects);
    void CmdClearDepthStencilImage(TImageView *imageView, TImageLayout layout, float depth = 0, uint32_t stencil = 0);

    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects);
    void CmdClearImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0, float depth = 0, uint32_t stencil = 0);

    void CmdCopyBufferToImage(TBuffer *srcBuffer, TImage *dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    void CmdCopyImageToBuffer(TImage *srcImage, TImageLayout layout, TBuffer *dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    void CmdCopyImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth);

    void CmdBindIndexBuffer(TBuffer *buffer, TDeviceSize offset = 0, TIndexType indexType = TIndexType::UINT32);
    void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

    void BlitImage();

    void TransferDeviceQueue();

    void DrawIndexedIndirect();
    void DrawIndirect();

    void SetLineWidth();
    void ExecuteCommands();

    void ClearAttachments();

    void Dispatch();
    void DispatchIndirect();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif // !TCOMMANDBUFFER_H