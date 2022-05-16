#include "TCommandBuffer.h"
#include "TCommandBufferPool.h"
#include "TDescriptorSet.h"
#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TException.h"
#include "TFramebuffer.h"
#include "TPipelineDescriptorSet.h"
#include "TPipelineLayout.h"
#include "TRenderPass.h"
#include "TScissor.h"
#include "TSubpass.h"

void Turbo::Core::TCommandBuffer::InternalCreate()
{
    VkDevice device = this->commandBufferPool->GetDeviceQueue()->GetDevice()->GetVkDevice();
    VkCommandPool command_pool = this->commandBufferPool->GetVkCommandPool();
    uint32_t command_buffer_count = 1;

    VkCommandBufferAllocateInfo vk_command_buffer_allocate_info = {};
    vk_command_buffer_allocate_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    vk_command_buffer_allocate_info.pNext = nullptr;
    vk_command_buffer_allocate_info.commandPool = command_pool;
    vk_command_buffer_allocate_info.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    vk_command_buffer_allocate_info.commandBufferCount = 1;

    VkResult result = vkAllocateCommandBuffers(device, &vk_command_buffer_allocate_info, &this->vkCommandBuffer);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TCommandBuffer::InternalCreate::vkAllocateCommandBuffers");
    }
}

void Turbo::Core::TCommandBuffer::InternalDestroy()
{
    VkDevice device = this->commandBufferPool->GetDeviceQueue()->GetDevice()->GetVkDevice();
    VkCommandPool command_pool = this->commandBufferPool->GetVkCommandPool();
    uint32_t command_buffer_count = 1;
    vkFreeCommandBuffers(device, command_pool, command_buffer_count, &this->vkCommandBuffer);
}

Turbo::Core::TCommandBuffer::TCommandBuffer(TCommandBufferPool *commandBufferPool) : Turbo::Core::TVulkanHandle()
{
    if (commandBufferPool != nullptr)
    {
        this->commandBufferPool = commandBufferPool;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TCommandBuffer::TCommandBuffer");
    }
}

Turbo::Core::TCommandBuffer::~TCommandBuffer()
{
    this->InternalDestroy();
}

VkCommandBuffer Turbo::Core::TCommandBuffer::GetVkCommandBuffer()
{
    return this->vkCommandBuffer;
}

bool Turbo::Core::TCommandBuffer::Begin()
{
    VkCommandBufferBeginInfo vk_command_buffer_begin_info;
    vk_command_buffer_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vk_command_buffer_begin_info.pNext = nullptr;
    vk_command_buffer_begin_info.flags = 0;
    vk_command_buffer_begin_info.pInheritanceInfo = nullptr;

    VkResult result = vkBeginCommandBuffer(this->vkCommandBuffer, &vk_command_buffer_begin_info);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }
    return false;
}

void Turbo::Core::TCommandBuffer::CmdBeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height)
{
    TPhysicalDevice *physical_device = renderPass->GetDevice()->GetPhysicalDevice();

    std::vector<TAttachment> attachemnts = renderPass->GetAttachments();
    uint32_t attachemnts_count = attachemnts.size();

    // TODO: Attachment clear color data should define in attachemnt
    std::vector<VkClearValue> vk_clear_values;
    for (uint32_t attachment_index = 0; attachment_index < attachemnts_count; attachment_index++)
    {
        TFormatInfo format_info = attachemnts[attachment_index].GetFormat();
        TFormatFeatures format_feature = format_info.GetOptimalFeatures(physical_device);

        if ((format_feature & TFormatFeatureBits::FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == TFormatFeatureBits::FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            VkClearValue vk_clear_value = {};
            vk_clear_value.depthStencil.depth = 0.0f;
            vk_clear_value.depthStencil.stencil = 0.0f;

            vk_clear_values.push_back(vk_clear_value);
        }
        else if ((format_feature & TFormatFeatureBits::FEATURE_COLOR_ATTACHMENT_BIT) == TFormatFeatureBits::FEATURE_COLOR_ATTACHMENT_BIT)
        {

            TFormatDataTypes format_data_types = format_info.GetFormatDataType();

            VkClearColorValue vk_clear_color_value = {};
            if ((format_data_types & TFormatDataTypeBits::SIGNED_INTEGER) == TFormatDataTypeBits::SIGNED_INTEGER)
            {
                vk_clear_color_value.int32[0] = (int32_t)0;
                vk_clear_color_value.int32[1] = (int32_t)0;
                vk_clear_color_value.int32[2] = (int32_t)0;
                vk_clear_color_value.int32[3] = (int32_t)0;
            }
            else if ((format_data_types & TFormatDataTypeBits::UNSIGNED_INTEGER) == TFormatDataTypeBits::UNSIGNED_INTEGER)
            {
                vk_clear_color_value.uint32[0] = (uint32_t)0;
                vk_clear_color_value.uint32[1] = (uint32_t)0;
                vk_clear_color_value.uint32[2] = (uint32_t)0;
                vk_clear_color_value.uint32[3] = (uint32_t)0;
            }
            else
            {
                vk_clear_color_value.float32[0] = 0;
                vk_clear_color_value.float32[1] = 0;
                vk_clear_color_value.float32[2] = 0;
                vk_clear_color_value.float32[3] = 0;
            }

            VkClearValue vk_clear_value = {};
            vk_clear_value.color = vk_clear_color_value;

            vk_clear_values.push_back(vk_clear_value);
        }
        else
        {
            VkClearValue vk_clear_value = {};
            vk_clear_values.push_back(vk_clear_value);
        }
    }

    VkRenderPassBeginInfo vk_render_pass_begin_info = {};
    vk_render_pass_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    vk_render_pass_begin_info.pNext = nullptr;
    vk_render_pass_begin_info.renderPass = renderPass->GetVkRenderPass();
    vk_render_pass_begin_info.framebuffer = framebuffer->GetVkFramebuffer();
    vk_render_pass_begin_info.renderArea.offset.x = offsetX;
    vk_render_pass_begin_info.renderArea.offset.y = offsetY;
    vk_render_pass_begin_info.renderArea.extent.width = width;
    vk_render_pass_begin_info.renderArea.extent.height = height;
    if (width == TURBO_WHOLE_EXTENT)
    {
        vk_render_pass_begin_info.renderArea.extent.width = framebuffer->GetWidth();
    }
    if (height == TURBO_WHOLE_EXTENT)
    {
        vk_render_pass_begin_info.renderArea.extent.height = framebuffer->GetHeight();
    }
    vk_render_pass_begin_info.clearValueCount = vk_clear_values.size();
    vk_render_pass_begin_info.pClearValues = vk_clear_values.data();

    vkCmdBeginRenderPass(this->vkCommandBuffer, &vk_render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    this->currentRenderPass = renderPass;
}

void Turbo::Core::TCommandBuffer::CmdBindPipeline(TPipeline *pipeline)
{
    VkPipelineBindPoint vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;

    switch (pipeline->GetType())
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

    vkCmdBindPipeline(this->vkCommandBuffer, vk_pipeline_bind_point, pipeline->GetVkPipeline());

    this->currentPipeline = pipeline;
}

void Turbo::Core::TCommandBuffer::CmdBindDescriptorSets(uint32_t firstSet, std::vector<TDescriptorSet *> &descriptorSets)
{
    VkPipelineBindPoint vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;

    if (this->currentPipeline != nullptr)
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
        for (TDescriptorSet *descriptor_set_item : descriptorSets)
        {
            vk_descriptor_sets.push_back(descriptor_set_item->GetVkDescriptorSet());
        }

        vkCmdBindDescriptorSets(this->vkCommandBuffer, vk_pipeline_bind_point, this->currentPipeline->GetPipelineLayout()->GetVkPipelineLayout(), firstSet, vk_descriptor_sets.size(), vk_descriptor_sets.data(), 0, nullptr);
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TCommandBuffer::CmdBindDescriptorSets");
    }
}

void Turbo::Core::TCommandBuffer::CmdBindPipelineDescriptorSet(uint32_t firstSet, TPipelineDescriptorSet *pipelineDescriptorSet)
{
    std::vector<Turbo::Core::TDescriptorSet *> descriptor_sets = pipelineDescriptorSet->GetDescriptorSet();
    this->CmdBindDescriptorSets(firstSet, descriptor_sets);
}

void Turbo::Core::TCommandBuffer::CmdBindVertexBuffers(std::vector<TBuffer *> &vertexBuffers)
{
    std::vector<VkBuffer> vertex_buffers;
    std::vector<VkDeviceSize> offsets;

    for (TBuffer *buffer_item : vertexBuffers)
    {
        vertex_buffers.push_back(buffer_item->GetVkBuffer());
        offsets.push_back(0);
    }

    vkCmdBindVertexBuffers(this->vkCommandBuffer, 0, vertexBuffers.size(), vertex_buffers.data(), offsets.data());
}

void Turbo::Core::TCommandBuffer::CmdSetViewport(std::vector<TViewport> &viewports)
{
    std::vector<VkViewport> vk_viewports;
    for (TViewport &viewport_item : viewports)
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

    vkCmdSetViewport(this->vkCommandBuffer, 0, vk_viewports.size(), vk_viewports.data());
}

void Turbo::Core::TCommandBuffer::CmdSetScissor(std::vector<TScissor> &scissors)
{
    std::vector<VkRect2D> vk_scissors;
    for (TScissor &scissor_item : scissors)
    {
        VkRect2D vk_scissor;
        vk_scissor.offset.x = scissor_item.GetOffsetX();
        vk_scissor.offset.y = scissor_item.GetOffsetY();
        vk_scissor.extent.width = scissor_item.GetWidth();
        vk_scissor.extent.height = scissor_item.GetHeight();

        vk_scissors.push_back(vk_scissor);
    }

    vkCmdSetScissor(this->vkCommandBuffer, 0, vk_scissors.size(), vk_scissors.data());
}

void Turbo::Core::TCommandBuffer::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(this->vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void Turbo::Core::TCommandBuffer::CmdNextSubpass()
{
    vkCmdNextSubpass(this->vkCommandBuffer, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
}

void Turbo::Core::TCommandBuffer::CmdEndRenderPass()
{
    vkCmdEndRenderPass(this->vkCommandBuffer);
    this->currentPipeline = nullptr;
    this->currentRenderPass = nullptr;
}

bool Turbo::Core::TCommandBuffer::End()
{
    VkResult result = vkEndCommandBuffer(this->vkCommandBuffer);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }
    return false;
}

bool Turbo::Core::TCommandBuffer::Reset()
{
    VkResult result = vkResetCommandBuffer(this->vkCommandBuffer, 0);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }
    return false;
}

void Turbo::Core::TCommandBuffer::CmdPipelineBarrier(TPipelineStages srcStages, TPipelineStages dstStages, std::vector<TMemoryBarrier> &memoryBarriers, std::vector<TBufferMemoryBarrier> &bufferBarriers, std::vector<TImageMemoryBarrier> &imageBarriers)
{
    std::vector<VkMemoryBarrier> vk_memory_barriers;
    for (TMemoryBarrier &memory_barrier_item : memoryBarriers)
    {
        VkMemoryBarrier vk_memory_barrier = {};
        vk_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        vk_memory_barrier.pNext = nullptr;
        vk_memory_barrier.srcAccessMask = memory_barrier_item.GetSrcAccess();
        vk_memory_barrier.dstAccessMask = memory_barrier_item.GetDstAccess();

        vk_memory_barriers.push_back(vk_memory_barrier);
    }

    std::vector<VkBufferMemoryBarrier> vk_buffer_memory_barriers;
    for (TBufferMemoryBarrier &memory_barrier_item : bufferBarriers)
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

    std::vector<VkImageMemoryBarrier> vk_image_memory_barriers;
    for (TImageMemoryBarrier &image_barrier_item : imageBarriers)
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

    vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, vk_memory_barriers.size(), vk_memory_barriers.data(), vk_buffer_memory_barriers.size(), vk_buffer_memory_barriers.data(), vk_image_memory_barriers.size(), vk_image_memory_barriers.data());
}

void Turbo::Core::TCommandBuffer::CmdPipelineMemoryBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TMemoryBarrier &memoryBarrier)
{
    VkMemoryBarrier vk_memory_barrier = {};
    vk_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    vk_memory_barrier.pNext = nullptr;
    vk_memory_barrier.srcAccessMask = memoryBarrier.GetSrcAccess();
    vk_memory_barrier.dstAccessMask = memoryBarrier.GetDstAccess();

    std::vector<VkMemoryBarrier> vk_memory_barriers;
    vk_memory_barriers.push_back(vk_memory_barrier);

    vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, vk_memory_barriers.size(), vk_memory_barriers.data(), 0, nullptr, 0, nullptr);
}

void Turbo::Core::TCommandBuffer::CmdPipelineBufferBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TBufferMemoryBarrier &bufferBarrier)
{
    VkBufferMemoryBarrier vk_buffer_memory_barrier = {};
    vk_buffer_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    vk_buffer_memory_barrier.pNext = nullptr;
    vk_buffer_memory_barrier.srcAccessMask = bufferBarrier.GetSrcAccess();
    vk_buffer_memory_barrier.dstAccessMask = bufferBarrier.GetDstAccess();
    vk_buffer_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vk_buffer_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vk_buffer_memory_barrier.buffer = bufferBarrier.GetBuffer()->GetVkBuffer();
    vk_buffer_memory_barrier.offset = bufferBarrier.GetOffset();
    vk_buffer_memory_barrier.size = bufferBarrier.GetSize();

    std::vector<VkBufferMemoryBarrier> vk_buffer_memory_barriers;
    vk_buffer_memory_barriers.push_back(vk_buffer_memory_barrier);

    vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, vk_buffer_memory_barriers.size(), vk_buffer_memory_barriers.data(), 0, nullptr);
}

void Turbo::Core::TCommandBuffer::CmdPipelineImageBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TImageMemoryBarrier &imageBarrier)
{
    VkImageMemoryBarrier vk_image_memory_barrier = {};
    vk_image_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    vk_image_memory_barrier.pNext = nullptr;
    vk_image_memory_barrier.srcAccessMask = imageBarrier.GetSrcAccess();
    vk_image_memory_barrier.dstAccessMask = imageBarrier.GetDstAccess();
    vk_image_memory_barrier.oldLayout = (VkImageLayout)imageBarrier.GetOldLayout();
    vk_image_memory_barrier.newLayout = (VkImageLayout)imageBarrier.GetNewLayout();
    vk_image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vk_image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    vk_image_memory_barrier.image = imageBarrier.GetImage()->GetVkImage();
    vk_image_memory_barrier.subresourceRange.aspectMask = imageBarrier.GetAspects();
    vk_image_memory_barrier.subresourceRange.baseMipLevel = imageBarrier.GetBaseMipLevel();
    vk_image_memory_barrier.subresourceRange.levelCount = imageBarrier.GetLevelCount();
    vk_image_memory_barrier.subresourceRange.baseArrayLayer = imageBarrier.GetBaseArrayLayer();
    vk_image_memory_barrier.subresourceRange.layerCount = imageBarrier.GetLayerCount();

    std::vector<VkImageMemoryBarrier> vk_image_memory_barriers;
    vk_image_memory_barriers.push_back(vk_image_memory_barrier);

    vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, vk_image_memory_barriers.size(), vk_image_memory_barriers.data());
}

void Turbo::Core::TCommandBuffer::CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, TImage *image, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    TImageMemoryBarrier image_memory_barrier(srcAccess, dstAccess, image, oldLayout, newLayout, aspects, baseMipLevel, levelCount, baseArrayLayer, layerCount);
    this->CmdPipelineImageBarrier(srcStages, dstStages, image_memory_barrier);
}

void Turbo::Core::TCommandBuffer::CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, TImageView *imageView)
{
    TImageMemoryBarrier image_memory_barrier(srcAccess, dstAccess, imageView, oldLayout, newLayout);
    this->CmdPipelineImageBarrier(srcStages, dstStages, image_memory_barrier);
}

void Turbo::Core::TCommandBuffer::CmdFillBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, uint32_t data)
{
    vkCmdFillBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, data);
}

void Turbo::Core::TCommandBuffer::CmdFillBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, float data)
{
    vkCmdFillBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, *(const uint32_t *)&data);
}

void Turbo::Core::TCommandBuffer::CmdUpdateBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, const void *data)
{
    if (buffer != nullptr && data != nullptr)
    {
        vkCmdUpdateBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, data);
    }
}

void Turbo::Core::TCommandBuffer::CmdCopyBuffer(TBuffer *srcBuffer, TBuffer *dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size)
{
    VkBufferCopy vk_buffer_copy = {};
    vk_buffer_copy.srcOffset = srcOffset;
    vk_buffer_copy.dstOffset = dstOffset;
    vk_buffer_copy.size = size;

    vkCmdCopyBuffer(this->vkCommandBuffer, srcBuffer->GetVkBuffer(), dstBuffer->GetVkBuffer(), 1, &vk_buffer_copy);
}

void Turbo::Core::TCommandBuffer::CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    TFormatInfo image_format = image->GetFormat();
    TFormatDataTypes format_data_types = image_format.GetFormatDataType();

    VkClearColorValue vk_clear_color_value = {};
    if ((format_data_types & TFormatDataTypeBits::SIGNED_INTEGER) == TFormatDataTypeBits::SIGNED_INTEGER)
    {
        vk_clear_color_value.int32[0] = (int32_t)r;
        vk_clear_color_value.int32[1] = (int32_t)g;
        vk_clear_color_value.int32[2] = (int32_t)b;
        vk_clear_color_value.int32[3] = (int32_t)a;
    }
    else if ((format_data_types & TFormatDataTypeBits::UNSIGNED_INTEGER) == TFormatDataTypeBits::UNSIGNED_INTEGER)
    {
        vk_clear_color_value.uint32[0] = (uint32_t)r;
        vk_clear_color_value.uint32[1] = (uint32_t)g;
        vk_clear_color_value.uint32[2] = (uint32_t)b;
        vk_clear_color_value.uint32[3] = (uint32_t)a;
    }
    else
    {
        vk_clear_color_value.float32[0] = r;
        vk_clear_color_value.float32[1] = g;
        vk_clear_color_value.float32[2] = b;
        vk_clear_color_value.float32[3] = a;
    }

    VkImageSubresourceRange vk_image_subresource_range = {};
    vk_image_subresource_range.aspectMask = aspects;
    vk_image_subresource_range.baseMipLevel = baseMipLevel;
    vk_image_subresource_range.levelCount = levelCount;
    vk_image_subresource_range.baseArrayLayer = baseMipLevel;
    vk_image_subresource_range.layerCount = layerCount;

    vkCmdClearColorImage(this->vkCommandBuffer, image->GetVkImage(), (VkImageLayout)layout, &vk_clear_color_value, 1, &vk_image_subresource_range);
}

void Turbo::Core::TCommandBuffer::CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects)
{
    this->CmdClearColorImage(image, layout, r, g, b, a, aspects, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
}

void Turbo::Core::TCommandBuffer::CmdClearColorImage(TImageView *imageView, TImageLayout layout, float r, float g, float b, float a)
{
    this->CmdClearColorImage(imageView->GetImage(), layout, r, g, b, a, imageView->GetAspects(), imageView->GetBaseMipLevel(), imageView->GetLevelCount(), imageView->GetBaseArrayLayer(), imageView->GetLayerCount());
}

void Turbo::Core::TCommandBuffer::CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    VkClearDepthStencilValue vk_clear_depth_stencil_value = {};
    vk_clear_depth_stencil_value.depth = depth;
    vk_clear_depth_stencil_value.stencil = stencil;

    VkImageSubresourceRange vk_image_subresource_range = {};
    vk_image_subresource_range.aspectMask = aspects;
    vk_image_subresource_range.baseMipLevel = baseMipLevel;
    vk_image_subresource_range.levelCount = levelCount;
    vk_image_subresource_range.baseArrayLayer = baseMipLevel;
    vk_image_subresource_range.layerCount = layerCount;

    vkCmdClearDepthStencilImage(this->vkCommandBuffer, image->GetVkImage(), (VkImageLayout)layout, &vk_clear_depth_stencil_value, 1, &vk_image_subresource_range);
}

void Turbo::Core::TCommandBuffer::CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects)
{
    this->CmdClearDepthStencilImage(image, layout, depth, stencil, aspects, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
}

void Turbo::Core::TCommandBuffer::CmdClearDepthStencilImage(TImageView *imageView, TImageLayout layout, float depth, uint32_t stencil)
{
    this->CmdClearDepthStencilImage(imageView->GetImage(), layout, depth, stencil, imageView->GetAspects(), imageView->GetBaseMipLevel(), imageView->GetLevelCount(), imageView->GetBaseArrayLayer(), imageView->GetLayerCount());
}

void Turbo::Core::TCommandBuffer::CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    TImageUsages image_usages = image->GetUsages();
    if ((image_usages & TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT) == TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT)
    {
        this->CmdClearDepthStencilImage(image, layout, depth, stencil, aspects, baseMipLevel, levelCount, baseArrayLayer, layerCount);
    }
    else
    {
        this->CmdClearColorImage(image, layout, r, g, b, a, aspects, baseMipLevel, levelCount, baseArrayLayer, layerCount);
    }
}

void Turbo::Core::TCommandBuffer::CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects)
{
    TImageUsages image_usages = image->GetUsages();
    if ((image_usages & TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT) == TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT)
    {
        this->CmdClearDepthStencilImage(image, layout, depth, stencil, aspects);
    }
    else
    {
        this->CmdClearColorImage(image, layout, r, g, b, a, aspects);
    }
}

void Turbo::Core::TCommandBuffer::CmdClearImage(TImageView *imageView, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil)
{
    TImageUsages image_usages = imageView->GetImage()->GetUsages();
    if ((image_usages & TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT) == TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT)
    {
        this->CmdClearDepthStencilImage(imageView, layout, depth, stencil);
    }
    else
    {
        this->CmdClearColorImage(imageView, layout, r, g, b, a);
    }
}

void Turbo::Core::TCommandBuffer::CmdCopyBufferToImage(TBuffer *srcBuffer, TImage *dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth)
{
    VkImageSubresourceLayers vk_image_subresource_layers = {};
    vk_image_subresource_layers.aspectMask = aspects;
    vk_image_subresource_layers.mipLevel = mipLevel;
    vk_image_subresource_layers.baseArrayLayer = baseArrayLayer;
    vk_image_subresource_layers.layerCount = layerCount;

    VkBufferImageCopy vk_buffer_image_copy = {};
    vk_buffer_image_copy.bufferOffset = bufferOffset;
    vk_buffer_image_copy.bufferRowLength = bufferRowLength;
    vk_buffer_image_copy.bufferImageHeight = bufferImageHeight;
    vk_buffer_image_copy.imageSubresource = vk_image_subresource_layers;
    vk_buffer_image_copy.imageOffset.x = imageOffsetX;
    vk_buffer_image_copy.imageOffset.y = imageOffsetY;
    vk_buffer_image_copy.imageOffset.z = imageOffsetZ;
    vk_buffer_image_copy.imageExtent.width = imageWidth;
    vk_buffer_image_copy.imageExtent.height = imageHeight;
    vk_buffer_image_copy.imageExtent.depth = imageDepth;

    vkCmdCopyBufferToImage(this->vkCommandBuffer, srcBuffer->GetVkBuffer(), dstImage->GetVkImage(), (VkImageLayout)layout, 1, &vk_buffer_image_copy);
}

void Turbo::Core::TCommandBuffer::CmdCopyImageToBuffer(TImage *srcImage, TImageLayout layout, TBuffer *dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth)
{
    VkImageSubresourceLayers vk_image_subresource_layers = {};
    vk_image_subresource_layers.aspectMask = aspects;
    vk_image_subresource_layers.mipLevel = mipLevel;
    vk_image_subresource_layers.baseArrayLayer = baseArrayLayer;
    vk_image_subresource_layers.layerCount = layerCount;

    VkBufferImageCopy vk_buffer_image_copy = {};
    vk_buffer_image_copy.bufferOffset = bufferOffset;
    vk_buffer_image_copy.bufferRowLength = bufferRowLength;
    vk_buffer_image_copy.bufferImageHeight = bufferImageHeight;
    vk_buffer_image_copy.imageSubresource = vk_image_subresource_layers;
    vk_buffer_image_copy.imageOffset.x = imageOffsetX;
    vk_buffer_image_copy.imageOffset.y = imageOffsetY;
    vk_buffer_image_copy.imageOffset.z = imageOffsetZ;
    vk_buffer_image_copy.imageExtent.width = imageWidth;
    vk_buffer_image_copy.imageExtent.height = imageHeight;
    vk_buffer_image_copy.imageExtent.depth = imageDepth;

    vkCmdCopyImageToBuffer(this->vkCommandBuffer, srcImage->GetVkImage(), (VkImageLayout)layout, dstBuffer->GetVkBuffer(), 1, &vk_buffer_image_copy);
}

void Turbo::Core::TCommandBuffer::CmdCopyImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth)
{
    VkImageSubresourceLayers src_subresource = {};
    src_subresource.aspectMask = srcAspects;
    src_subresource.mipLevel = srcMipLevel;
    src_subresource.baseArrayLayer = srcBaseArrayLayer;
    src_subresource.layerCount = srcLayerCount;

    VkOffset3D src_offset = {};
    src_offset.x = srcImageOffsetX;
    src_offset.y = srcImageOffsetY;
    src_offset.z = srcImageOffsetZ;

    VkImageSubresourceLayers dst_subresource = {};
    dst_subresource.aspectMask = dstAspects;
    dst_subresource.mipLevel = dstMipLevel;
    dst_subresource.baseArrayLayer = dstBaseArrayLayer;
    dst_subresource.layerCount = dstLayerCount;

    VkOffset3D dst_offset = {};
    dst_offset.x = dstImageOffsetX;
    dst_offset.y = dstImageOffsetY;
    dst_offset.z = dstImageOffsetZ;

    VkExtent3D extent = {};
    extent.width = width;
    extent.height = height;
    extent.depth = depth;

    VkImageCopy vk_image_copy = {};
    vk_image_copy.srcSubresource = src_subresource;
    vk_image_copy.srcOffset = src_offset;
    vk_image_copy.dstSubresource = dst_subresource;
    vk_image_copy.dstOffset = dst_offset;
    vk_image_copy.extent = extent;

    vkCmdCopyImage(this->vkCommandBuffer, srcImage->GetVkImage(), (VkImageLayout)srcLayout, dstImage->GetVkImage(), (VkImageLayout)dstLayout, 1, &vk_image_copy);
}

void Turbo::Core::TCommandBuffer::CmdBindIndexBuffer(TBuffer *buffer, TDeviceSize offset, TIndexType indexType)
{
    vkCmdBindIndexBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, (VkIndexType)indexType);
}

void Turbo::Core::TCommandBuffer::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    vkCmdDrawIndexed(this->vkCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void Turbo::Core::TCommandBuffer::CmdBlitImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, int32_t srcStartOffsetX, int32_t srcStartOffsetY, int32_t srcStartOffsetZ, int32_t srcEndOffsetX, int32_t srcEndOffsetY, int32_t srcEndOffsetZ, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t dstStartOffsetX, int32_t dstStartOffsetY, int32_t dstStartOffsetZ, int32_t dstEndOffsetX, int32_t dstEndOffsetY, int32_t dstEndOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, TFilter filter)
{
    VkImageSubresourceLayers src_subresource = {};
    src_subresource.aspectMask = srcAspects;
    src_subresource.mipLevel = srcMipLevel;
    src_subresource.baseArrayLayer = srcBaseArrayLayer;
    src_subresource.layerCount = srcLayerCount;

    VkImageSubresourceLayers dst_subresource = {};
    dst_subresource.aspectMask = dstAspects;
    dst_subresource.mipLevel = dstMipLevel;
    dst_subresource.baseArrayLayer = dstBaseArrayLayer;
    dst_subresource.layerCount = dstLayerCount;

    VkImageBlit vk_image_blit = {};
    vk_image_blit.srcSubresource = src_subresource;
    vk_image_blit.srcOffsets[0].x = srcStartOffsetX;
    vk_image_blit.srcOffsets[0].y = srcStartOffsetY;
    vk_image_blit.srcOffsets[0].z = srcStartOffsetZ;
    vk_image_blit.srcOffsets[1].x = srcEndOffsetX;
    vk_image_blit.srcOffsets[1].y = srcEndOffsetY;
    vk_image_blit.srcOffsets[1].z = srcEndOffsetZ;
    vk_image_blit.dstSubresource = dst_subresource;
    vk_image_blit.dstOffsets[0].x = dstStartOffsetX;
    vk_image_blit.dstOffsets[0].y = dstStartOffsetY;
    vk_image_blit.dstOffsets[0].z = dstStartOffsetZ;
    vk_image_blit.dstOffsets[1].x = dstEndOffsetX;
    vk_image_blit.dstOffsets[1].y = dstEndOffsetY;
    vk_image_blit.dstOffsets[1].z = dstEndOffsetZ;

    VkImageLayout src_image_layout = (VkImageLayout)srcLayout;
    VkImageLayout dst_image_layout = (VkImageLayout)dstLayout;
    VkFilter vk_filter = (VkFilter)filter;

    vkCmdBlitImage(this->vkCommandBuffer, srcImage->GetVkImage(), src_image_layout, dstImage->GetVkImage(), dst_image_layout, 1, &vk_image_blit, vk_filter);
}

std::string Turbo::Core::TCommandBuffer::ToString()
{
    return std::string();
}
