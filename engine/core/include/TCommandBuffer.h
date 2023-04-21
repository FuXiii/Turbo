#pragma once
#ifndef TURBO_CORE_TCOMMANDBUFFER_H
#define TURBO_CORE_TCOMMANDBUFFER_H
#include "TBarrier.h"
#include "TBuffer.h"
#include "TPipeline.h"
#include "TSampler.h"
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
class TRenderingAttachments;

typedef enum class TCommandBufferLevel
{
    PRIMARY = 0,
    SECONDARY = 1
} TCommandBufferLevelEnum;

typedef enum class TSubpassContents
{
    INLINE = 0,
    SECONDARY_COMMAND_BUFFERS = 1,
} TSubpassContentsEnum;

class TCommandBufferBase : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TCommandBufferPool;

  private:
    T_VULKAN_HANDLE_PARENT TCommandBufferPool *commandBufferPool = nullptr;
    T_VULKAN_HANDLE_HANDLE VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN; //???

    TCommandBufferLevel level;
    VkCommandBufferInheritanceInfo *vkCommandBufferInheritanceInfo = nullptr;

  protected:
    TRenderPass *currentRenderPass = nullptr;
    TPipeline *currentPipeline = nullptr;
    uint32_t currentSubpass = 0;
    TFramebuffer *currentFramebuffer = nullptr;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TCommandBufferBase(TCommandBufferPool *commandBufferPool, TCommandBufferLevel level);
    ~TCommandBufferBase();

    VkCommandBuffer GetVkCommandBuffer();

    TCommandBufferLevel GetLevel();

    TCommandBufferPool *GetCommandBufferPool();

  public:
    bool Begin();
    void CmdBeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, TSubpassContents subpassContents = TSubpassContents::INLINE, uint32_t offsetX = 0, uint32_t offsetY = 0, uint32_t width = TURBO_WHOLE_EXTENT, uint32_t height = TURBO_WHOLE_EXTENT);
    void CmdBindPipeline(TPipeline *pipeline);
    void CmdBindDescriptorSets(uint32_t firstSet, std::vector<TDescriptorSet *> &descriptorSets);
    void CmdBindPipelineDescriptorSet(TPipelineDescriptorSet *pipelineDescriptorSet);
    void CmdBindVertexBuffers(std::vector<TBuffer *> &vertexBuffers);
    void CmdSetViewport(std::vector<TViewport> &viewports);
    void CmdSetScissor(std::vector<TScissor> &scissors);
    void CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    void CmdNextSubpass();
    void CmdEndRenderPass();

    //<Dynamic Rendering>
    void CmdBeginRendering(const TRenderingAttachments &renderingAttachment, uint32_t offsetX = 0, uint32_t offsetY = 0, uint32_t width = TURBO_WHOLE_EXTENT, uint32_t height = TURBO_WHOLE_EXTENT);
    void CmdEndRendering();
    //</Dynamic Rendering>

    //<VK_EXT_mesh_shader>
    void CmdDrawMeshTasksEXT(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
    //</VK_EXT_mesh_shader>

    //<VK_NV_mesh_shader>
    void CmdDrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask);
    //</VK_NV_mesh_shader>

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

    // - The VkImageSubresourceRange::aspectMask members of the elements of the pRanges array must each only include VK_IMAGE_ASPECT_COLOR_BIT
    // - imageLayout specifies the current layout of the image subresource ranges to be cleared, and must be VK_IMAGE_LAYOUT_GENERAL or VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL.
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects);
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a);
    void CmdClearColorImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0);

    // imageLayout specifies the current layout of the image subresource ranges to be cleared, and must be VK_IMAGE_LAYOUT_GENERAL or VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL.
    void CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects);
    void CmdClearDepthStencilImage(TImageView *imageView, TImageLayout layout, float depth = 0, uint32_t stencil = 0);

    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects);
    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil);
    void CmdClearImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0, float depth = 0, uint32_t stencil = 0);

    void CmdCopyBufferToImage(TBuffer *srcBuffer, TImage *dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    void CmdCopyImageToBuffer(TImage *srcImage, TImageLayout layout, TBuffer *dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    void CmdCopyImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth);

    void CmdBindIndexBuffer(TBuffer *buffer, TDeviceSize offset = 0, TIndexType indexType = TIndexType::UINT32);
    void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

    void CmdBlitImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, int32_t srcStartOffsetX, int32_t srcStartOffsetY, int32_t srcStartOffsetZ, int32_t srcEndOffsetX, int32_t srcEndOffsetY, int32_t srcEndOffsetZ, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t dstStartOffsetX, int32_t dstStartOffsetY, int32_t dstStartOffsetZ, int32_t dstEndOffsetX, int32_t dstEndOffsetY, int32_t dstEndOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, TFilter filter = TFilter::LINEAR);

    void CmdResolveImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcOffsetX, int32_t srcOffsety, int32_t srcOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstOffsetX, int32_t dstOffsety, int32_t dstOffsetZ, uint32_t width, uint32_t height, uint32_t depth);

    void CmdSetLineWidth(float lineWidth);

    void CmdPushConstants(TPipelineLayout *pipelineLayout, uint32_t offset, uint32_t size, const void *values);
    void CmdPushConstants(uint32_t offset, uint32_t size, const void *values);

    void CmdDispatch(uint32_t workGroupsX, uint32_t workGroupsY, uint32_t workGroupsZ);

    void CmdTransferDeviceQueue();

    void CmdDrawIndexedIndirect();
    void CmdDrawIndirect();

    void CmdClearAttachments();

    void CmdDispatchIndirect();
};

class TSecondaryCommandBuffer : public Turbo::Core::TCommandBufferBase
{
  public:
    friend class TCommandBufferPool;

  public:
    TSecondaryCommandBuffer(TCommandBufferPool *commandBufferPool);
    ~TSecondaryCommandBuffer();

  public:
    void Begin() = delete;
    void Begin(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t subpass);

  public:
    virtual std::string ToString() override;
};

class TCommandBuffer : public Turbo::Core::TCommandBufferBase
{
  public:
    friend class TCommandBufferPool;

  public:
    TCommandBuffer(TCommandBufferPool *commandBufferPool);
    ~TCommandBuffer();

  public:
    void CmdExecuteCommand(TSecondaryCommandBuffer *secondaryCommandBuffer);

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TCOMMANDBUFFER_H