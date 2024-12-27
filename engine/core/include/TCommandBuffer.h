#pragma once
#ifndef TURBO_CORE_TCOMMANDBUFFER_H
#define TURBO_CORE_TCOMMANDBUFFER_H
#include "TBarrier.h"
#include "TBuffer.h"
#include "TPipeline.h"
#include "TSampler.h"
#include "TVulkanHandle.h"

#include "TDeviceQueue.h"
#include "TCommandBufferPool.h"
#include "TDescriptorSet.h"
#include "TDevice.h"
#include "TVulkanLoader.h"
#include "TPipelineLayout.h"

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
    T_VULKAN_HANDLE_PARENT TRefPtr<TCommandBufferPool> commandBufferPool; // TCommandBufferPool *commandBufferPool = nullptr;
    T_VULKAN_HANDLE_HANDLE VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN; //???

    TCommandBufferLevel level;
    VkCommandBufferInheritanceInfo *vkCommandBufferInheritanceInfo = nullptr;

  protected:
    TRefPtr<TRenderPass> currentRenderPass;
    TRefPtr<TPipeline> currentPipeline;
    uint32_t currentSubpass = 0;
    TRefPtr<TFramebuffer> currentFramebuffer;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    //[[deprecated]] TCommandBufferBase(const TRefPtr<TCommandBufferPool> &commandBufferPool, TCommandBufferLevel level);
    TCommandBufferBase(TCommandBufferPool *commandBufferPool, TCommandBufferLevel level);

  protected:
    virtual ~TCommandBufferBase();

  public:
    VkCommandBuffer GetVkCommandBuffer();

    TCommandBufferLevel GetLevel() const;

    // [[deprecated]] const TRefPtr<TCommandBufferPool> &GetCommandBufferPool();
    TCommandBufferPool *GetCommandBufferPool();

  public:
    bool Begin();
    //[[deprecated]] void CmdBeginRenderPass(const TRefPtr<TRenderPass> &renderPass, const TRefPtr<TFramebuffer> &framebuffer, TSubpassContents subpassContents = TSubpassContents::INLINE, uint32_t offsetX = 0, uint32_t offsetY = 0, uint32_t width = TURBO_WHOLE_EXTENT, uint32_t height = TURBO_WHOLE_EXTENT);
    void CmdBeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, TSubpassContents subpassContents = TSubpassContents::INLINE, uint32_t offsetX = 0, uint32_t offsetY = 0, uint32_t width = TURBO_WHOLE_EXTENT, uint32_t height = TURBO_WHOLE_EXTENT);
    //[[deprecated]] void CmdBindPipeline(const TRefPtr<TPipeline> &pipeline);
    void CmdBindPipeline(TPipeline *pipeline);
    // void CmdBindDescriptorSets(uint32_t firstSet, const std::vector<TRefPtr<TDescriptorSet>> &descriptorSets = {});
    // void CmdBindDescriptorSets(uint32_t firstSet, const std::vector<TDescriptorSet *> &descriptorSets = {});

    template <typename T>
    void CmdBindDescriptorSets(uint32_t firstSet, const T &descriptorSets = {})
    {
        VkPipelineBindPoint vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;

        if (this->currentPipeline.Valid() && !descriptorSets.empty())
        {
            switch (this->currentPipeline->GetType())
            {
            case TPipelineType::Compute: {
                vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;
            }
            break;
            case TPipelineType::Graphics: {
                vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
            }
            break;
            }

            std::vector<VkDescriptorSet> vk_descriptor_sets;
            // for (TDescriptorSet *descriptor_set_item : descriptorSets)
            for (auto &descriptor_set_item : descriptorSets)
            {
                // TODO: check descriptor_set_item valid?
                vk_descriptor_sets.push_back(descriptor_set_item->GetVkDescriptorSet());
            }

            TDevice *device = this->currentPipeline->GetDevice();
            device->GetDeviceDriver()->vkCmdBindDescriptorSets(this->vkCommandBuffer, vk_pipeline_bind_point, this->currentPipeline->GetPipelineLayout()->GetVkPipelineLayout(), firstSet, vk_descriptor_sets.size(), vk_descriptor_sets.data(), 0, nullptr);
        }
        else
        {
            throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TCommandBuffer::CmdBindDescriptorSets");
        }
    }

    //[[deprecated]]void CmdBindPipelineDescriptorSet(const TRefPtr<TPipelineDescriptorSet> &pipelineDescriptorSet);
    void CmdBindPipelineDescriptorSet(TPipelineDescriptorSet *pipelineDescriptorSet);
    // void CmdBindVertexBuffers(const std::vector<TRefPtr<TBuffer>> &vertexBuffers = {});
    // void CmdBindVertexBuffers(const std::vector<TBuffer *> &vertexBuffers = {});

    template <typename T>
    void CmdBindVertexBuffers(const T &vertexBuffers = {})
    {
        if (!vertexBuffers.empty())
        {
            TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

            std::vector<VkBuffer> vertex_buffers;
            std::vector<VkDeviceSize> offsets;

            // for (TBuffer *buffer_item : vertexBuffers)
            for (auto &buffer_item : vertexBuffers)
            {
                vertex_buffers.push_back(buffer_item->GetVkBuffer());
                offsets.push_back(0);
            }

            device->GetDeviceDriver()->vkCmdBindVertexBuffers(this->vkCommandBuffer, 0, vertexBuffers.size(), vertex_buffers.data(), offsets.data());
        }
    }

    // void CmdSetViewport(const std::vector<TViewport> &viewports = {});
    // void CmdSetScissor(const std::vector<TScissor> &scissors = {});

    template <typename T>
    void CmdSetViewport(const T &viewports = {})
    {
        if (!viewports.empty())
        {
            TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

            std::vector<VkViewport> vk_viewports;
            // for (const TViewport &viewport_item : viewports)
            for (auto &viewport_item : viewports)
            {
                VkViewport vk_viewport;
                vk_viewport.x = viewport_item.GetX();
                vk_viewport.y = viewport_item.GetY();
                vk_viewport.width = viewport_item.GetWidth();
                vk_viewport.height = viewport_item.GetHeight();
                vk_viewport.minDepth = viewport_item.GetMinDepth();
                vk_viewport.maxDepth = viewport_item.GetMaxDepth();

                vk_viewports.push_back(vk_viewport);
            }

            device->GetDeviceDriver()->vkCmdSetViewport(this->vkCommandBuffer, 0, vk_viewports.size(), vk_viewports.data());
        }
    }

    template <typename T>
    void CmdSetScissor(const T &scissors = {})
    {
        if (!scissors.empty())
        {
            TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

            std::vector<VkRect2D> vk_scissors;
            // for (const TScissor &scissor_item : scissors)
            for (auto &scissor_item : scissors)
            {
                VkRect2D vk_scissor;
                vk_scissor.offset.x = scissor_item.GetOffsetX();
                vk_scissor.offset.y = scissor_item.GetOffsetY();
                vk_scissor.extent.width = scissor_item.GetWidth();
                vk_scissor.extent.height = scissor_item.GetHeight();

                vk_scissors.push_back(vk_scissor);
            }

            device->GetDeviceDriver()->vkCmdSetScissor(this->vkCommandBuffer, 0, vk_scissors.size(), vk_scissors.data());
        }
    }

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

    // void CmdPipelineBarrier(TPipelineStages srcStages, TPipelineStages dstStages, const std::vector<TMemoryBarrier> &memoryBarriers = {}, const std::vector<TBufferMemoryBarrier> &bufferBarriers = {}, const std::vector<TImageMemoryBarrier> &imageBarriers = {});
    template <typename T, typename U, typename V>
    void CmdPipelineBarrier(TPipelineStages srcStages, TPipelineStages dstStages, const T &memoryBarriers = {}, const U &bufferBarriers = {}, const V &imageBarriers = {})
    {
        TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

        std::vector<TMemoryBarrier> memory_barriers = memoryBarriers; // FIXME: Maybe unnecessary copies
        std::vector<VkMemoryBarrier> vk_memory_barriers;
        for (TMemoryBarrier &memory_barrier_item : memory_barriers)
        {
            VkMemoryBarrier vk_memory_barrier = {};
            vk_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_BARRIER;
            vk_memory_barrier.pNext = nullptr;
            vk_memory_barrier.srcAccessMask = memory_barrier_item.GetSrcAccess();
            vk_memory_barrier.dstAccessMask = memory_barrier_item.GetDstAccess();

            vk_memory_barriers.push_back(vk_memory_barrier);
        }

        std::vector<TBufferMemoryBarrier> buffer_barriers = bufferBarriers; // FIXME: Maybe unnecessary copies
        std::vector<VkBufferMemoryBarrier> vk_buffer_memory_barriers;
        for (TBufferMemoryBarrier &memory_barrier_item : buffer_barriers)
        {
            VkBufferMemoryBarrier vk_buffer_memory_barrier = {};
            vk_buffer_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            vk_buffer_memory_barrier.pNext = nullptr;
            vk_buffer_memory_barrier.srcAccessMask = memory_barrier_item.GetSrcAccess();
            vk_buffer_memory_barrier.dstAccessMask = memory_barrier_item.GetDstAccess();
            vk_buffer_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vk_buffer_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vk_buffer_memory_barrier.buffer = memory_barrier_item.GetBuffer()->GetVkBuffer();
            vk_buffer_memory_barrier.offset = memory_barrier_item.GetOffset();
            vk_buffer_memory_barrier.size = memory_barrier_item.GetSize();

            vk_buffer_memory_barriers.push_back(vk_buffer_memory_barrier);
        }

        std::vector<TImageMemoryBarrier> image_barriers = imageBarriers; // FIXME: Maybe unnecessary copies
        std::vector<VkImageMemoryBarrier> vk_image_memory_barriers;
        for (TImageMemoryBarrier &image_barrier_item : image_barriers)
        {
            VkImageMemoryBarrier vk_image_memory_barrier = {};
            vk_image_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            vk_image_memory_barrier.pNext = nullptr;
            vk_image_memory_barrier.srcAccessMask = image_barrier_item.GetSrcAccess();
            vk_image_memory_barrier.dstAccessMask = image_barrier_item.GetDstAccess();
            vk_image_memory_barrier.oldLayout = (VkImageLayout)image_barrier_item.GetOldLayout();
            vk_image_memory_barrier.newLayout = (VkImageLayout)image_barrier_item.GetNewLayout();
            vk_image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vk_image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            vk_image_memory_barrier.image = image_barrier_item.GetImage()->GetVkImage();
            vk_image_memory_barrier.subresourceRange.aspectMask = image_barrier_item.GetAspects();
            vk_image_memory_barrier.subresourceRange.baseMipLevel = image_barrier_item.GetBaseMipLevel();
            vk_image_memory_barrier.subresourceRange.levelCount = image_barrier_item.GetLevelCount();
            vk_image_memory_barrier.subresourceRange.baseArrayLayer = image_barrier_item.GetBaseArrayLayer();
            vk_image_memory_barrier.subresourceRange.layerCount = image_barrier_item.GetLayerCount();

            vk_image_memory_barriers.push_back(vk_image_memory_barrier);
        }

        device->GetDeviceDriver()->vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, vk_memory_barriers.size(), vk_memory_barriers.data(), vk_buffer_memory_barriers.size(), vk_buffer_memory_barriers.data(), vk_image_memory_barriers.size(), vk_image_memory_barriers.data());
    }

    void CmdPipelineMemoryBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TMemoryBarrier &memoryBarrier);
    void CmdPipelineBufferBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TBufferMemoryBarrier &bufferBarrier);
    void CmdPipelineImageBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TImageMemoryBarrier &imageBarrier);

    void CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, const TRefPtr<TImage> &image, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, const TRefPtr<TImageView> &imageView);

    //[[deprecated]] void CmdFillBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, uint32_t data = 0);
    void CmdFillBuffer(TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, uint32_t data = 0);
    //[[deprecated]] void CmdFillBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, float data = 0.0f);
    void CmdFillBuffer(TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, float data = 0.0f);

    // The special value VK_WHOLE_SIZE is not accepted for the size parameter to
    // vkCmdUpdateBuffer() because it is also used as the size of the host memory region that is the
    // source of the data. The maximum size of data that can be placed in a buffer with
    // vkCmdUpdateBuffer() is 65,536 bytes.
    //[[deprecated]] void CmdUpdateBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset, TDeviceSize size, const void *data);
    void CmdUpdateBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, const void *data);
    //[[deprecated]] void CmdCopyBuffer(const TRefPtr<TBuffer> &srcBuffer, const TRefPtr<TBuffer> &dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size);
    void CmdCopyBuffer(TBuffer *srcBuffer, TBuffer *dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size);

    // - The VkImageSubresourceRange::aspectMask members of the elements of the pRanges array must each only include VK_IMAGE_ASPECT_COLOR_BIT
    // - imageLayout specifies the current layout of the image subresource ranges to be cleared, and must be VK_IMAGE_LAYOUT_GENERAL or VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL.
    //[[deprecated]] void CmdClearColorImage(const TRefPtr<TImage> &image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    //[[deprecated]] void CmdClearColorImage(const TRefPtr<TImage> &image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects);
    //[[deprecated]] void CmdClearColorImage(const TRefPtr<TImage> &image, TImageLayout layout, float r, float g, float b, float a);
    //[[deprecated]] void CmdClearColorImage(const TRefPtr<TImageView> &imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0);
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects);
    void CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a);
    void CmdClearColorImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0);

    // imageLayout specifies the current layout of the image subresource ranges to be cleared, and must be VK_IMAGE_LAYOUT_GENERAL or VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL.
    //[[deprecated]] void CmdClearDepthStencilImage(const TRefPtr<TImage> &image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    //[[deprecated]] void CmdClearDepthStencilImage(const TRefPtr<TImage> &image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects);
    //[[deprecated]] void CmdClearDepthStencilImage(const TRefPtr<TImageView> &imageView, TImageLayout layout, float depth = 0, uint32_t stencil = 0);
    void CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects);
    void CmdClearDepthStencilImage(TImageView *imageView, TImageLayout layout, float depth = 0, uint32_t stencil = 0);

    //[[deprecated]] void CmdClearImage(const TRefPtr<TImage> &image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    //[[deprecated]] void CmdClearImage(const TRefPtr<TImage> &image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects);
    //[[deprecated]] void CmdClearImage(const TRefPtr<TImage> &image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil);
    //[[deprecated]] void CmdClearImage(const TRefPtr<TImageView> &imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0, float depth = 0, uint32_t stencil = 0);
    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects);
    void CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil);
    void CmdClearImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0, float depth = 0, uint32_t stencil = 0);

    //[[deprecated]] void CmdCopyBufferToImage(const TRefPtr<TBuffer> &srcBuffer, const TRefPtr<TImage> &dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    //[[deprecated]] void CmdCopyImageToBuffer(const TRefPtr<TImage> &srcImage, TImageLayout layout, const TRefPtr<TBuffer> &dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    //[[deprecated]] void CmdCopyImage(const TRefPtr<TImage> &srcImage, TImageLayout srcLayout, const TRefPtr<TImage> &dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth);
    void CmdCopyBufferToImage(TBuffer *srcBuffer, TImage *dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    void CmdCopyImageToBuffer(TImage *srcImage, TImageLayout layout, TBuffer *dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
    void CmdCopyImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth);

    //[[deprecated]] void CmdBindIndexBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset = 0, TIndexType indexType = TIndexType::UINT32);
    void CmdBindIndexBuffer(TBuffer *buffer, TDeviceSize offset = 0, TIndexType indexType = TIndexType::UINT32);
    void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

    //[[deprecated]] void CmdBlitImage(const TRefPtr<TImage> &srcImage, TImageLayout srcLayout, const TRefPtr<TImage> &dstImage, TImageLayout dstLayout, int32_t srcStartOffsetX, int32_t srcStartOffsetY, int32_t srcStartOffsetZ, int32_t srcEndOffsetX, int32_t srcEndOffsetY, int32_t srcEndOffsetZ, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t dstStartOffsetX, int32_t dstStartOffsetY, int32_t dstStartOffsetZ, int32_t dstEndOffsetX, int32_t dstEndOffsetY, int32_t dstEndOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, TFilter filter = TFilter::LINEAR);
    void CmdBlitImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, int32_t srcStartOffsetX, int32_t srcStartOffsetY, int32_t srcStartOffsetZ, int32_t srcEndOffsetX, int32_t srcEndOffsetY, int32_t srcEndOffsetZ, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t dstStartOffsetX, int32_t dstStartOffsetY, int32_t dstStartOffsetZ, int32_t dstEndOffsetX, int32_t dstEndOffsetY, int32_t dstEndOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, TFilter filter = TFilter::LINEAR);

    //[[deprecated]] void CmdResolveImage(const TRefPtr<TImage> &srcImage, TImageLayout srcLayout, const TRefPtr<TImage> &dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcOffsetX, int32_t srcOffsety, int32_t srcOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstOffsetX, int32_t dstOffsety, int32_t dstOffsetZ, uint32_t width, uint32_t height, uint32_t depth);
    void CmdResolveImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcOffsetX, int32_t srcOffsety, int32_t srcOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstOffsetX, int32_t dstOffsety, int32_t dstOffsetZ, uint32_t width, uint32_t height, uint32_t depth);

    void CmdSetLineWidth(float lineWidth);

    //[[deprecated]] void CmdPushConstants(const TRefPtr<TPipelineLayout> &pipelineLayout, uint32_t offset, uint32_t size, const void *values);
    void CmdPushConstants(TPipelineLayout *pipelineLayout, uint32_t offset, uint32_t size, const void *values);
    void CmdPushConstants(uint32_t offset, uint32_t size, const void *values);

    void CmdDispatch(uint32_t workGroupsX, uint32_t workGroupsY, uint32_t workGroupsZ);

    void CmdTransferDeviceQueue();

    void CmdDrawIndexedIndirect();
    void CmdDrawIndirect();

    void CmdClearAttachments();

    void CmdDispatchIndirect();

    virtual bool Valid() const override;
};

class TSecondaryCommandBuffer : public Turbo::Core::TCommandBufferBase
{
  public:
    friend class TCommandBufferPool;

  public:
    TSecondaryCommandBuffer(const TRefPtr<TCommandBufferPool> &commandBufferPool);

  protected:
    virtual ~TSecondaryCommandBuffer();

  public:
    void Begin() = delete;
    //[[deprecated]] void Begin(const TRefPtr<TRenderPass> &renderPass, const TRefPtr<TFramebuffer> &framebuffer, uint32_t subpass);
    void Begin(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t subpass);

  public:
    virtual std::string ToString() const override;
};

class TCommandBuffer : public Turbo::Core::TCommandBufferBase
{
  public:
    friend class TCommandBufferPool;

  public:
    //[[deprecated]] TCommandBuffer(const TRefPtr<TCommandBufferPool> &commandBufferPool);
    TCommandBuffer(TCommandBufferPool *commandBufferPool);

  protected:
    virtual ~TCommandBuffer();

  public:
    //[[deprecated]] void CmdExecuteCommand(const TRefPtr<TSecondaryCommandBuffer> &secondaryCommandBuffer);
    void CmdExecuteCommand(TSecondaryCommandBuffer *secondaryCommandBuffer);

  public:
    virtual std::string ToString() const override;
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TCOMMANDBUFFER_H