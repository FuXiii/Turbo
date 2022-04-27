#include "TCommandBuffer.h"
#include "TBuffer.h"
#include "TCommandBufferPool.h"
#include "TDescriptorSet.h"
#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TException.h"
#include "TFramebuffer.h"
#include "TPipeline.h"
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
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
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
    this->commandBufferPool = commandBufferPool;

    this->InternalCreate();
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

void Turbo::Core::TCommandBuffer::BeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height)
{
    uint32_t attachemnt_count = renderPass->GetAttachments().size();

    VkClearValue vk_clear_value_color = {};
    vk_clear_value_color.color.float32[0] = 1.0f;
    vk_clear_value_color.color.float32[1] = 1.0f;
    vk_clear_value_color.color.float32[2] = 1.0f;
    vk_clear_value_color.color.float32[3] = 1.0f;
    vk_clear_value_color.depthStencil.depth = 1.0f;
    vk_clear_value_color.depthStencil.stencil = 0.0f;

    std::vector<VkClearValue> vk_clear_values(attachemnt_count, vk_clear_value_color);

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
}

void Turbo::Core::TCommandBuffer::BindPipeline(TPipeline *pipeline)
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
}

void Turbo::Core::TCommandBuffer::BindDescriptorSets(TPipeline *pipeline, uint32_t firstSet, std::vector<TDescriptorSet *> &descriptorSets)
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

    std::vector<VkDescriptorSet> vk_descriptor_sets;
    for (TDescriptorSet *descriptor_set_item : descriptorSets)
    {
        vk_descriptor_sets.push_back(descriptor_set_item->GetVkDescriptorSet());
    }

    vkCmdBindDescriptorSets(this->vkCommandBuffer, vk_pipeline_bind_point, pipeline->GetVkPipelineLayout(), firstSet, vk_descriptor_sets.size(), vk_descriptor_sets.data(), 0, nullptr);
}

void Turbo::Core::TCommandBuffer::BindVertexBuffers(std::vector<TBuffer *> &vertexBuffers)
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

void Turbo::Core::TCommandBuffer::SetViewport(std::vector<TViewport> &viewports)
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

void Turbo::Core::TCommandBuffer::SetScissor(std::vector<TScissor> &scissors)
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

void Turbo::Core::TCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(this->vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void Turbo::Core::TCommandBuffer::NextSubpass()
{
    vkCmdNextSubpass(this->vkCommandBuffer, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
}

void Turbo::Core::TCommandBuffer::EndRenderPass()
{
    vkCmdEndRenderPass(this->vkCommandBuffer);
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

std::string Turbo::Core::TCommandBuffer::ToString()
{
    return std::string();
}
