#include "TCommandBuffer.h"
// #include "TCommandBufferPool.h"
#include "TDescriptor.h"
// #include "TDescriptorSet.h"
// #include "TDevice.h"
// #include "TDeviceQueue.h"
#include "TException.h"
#include "TFramebuffer.h"
#include "TPhysicalDevice.h"
#include "TPipelineDescriptorSet.h"
// #include "TPipelineLayout.h"
#include "TRenderPass.h"
#include "TRenderingPipeline.h"
#include "TScissor.h"
#include "TShader.h"
#include "TSubpass.h"
// #include "TVulkanLoader.h"

void Turbo::Core::TCommandBufferBase::InternalCreate()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkCommandPool command_pool = this->commandBufferPool->GetVkCommandPool();
    uint32_t command_buffer_count = 1;

    VkCommandBufferLevel vk_command_buffer_level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    switch (this->level)
    {
    case TCommandBufferLevel::PRIMARY: {
        vk_command_buffer_level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    }
    break;
    case TCommandBufferLevel::SECONDARY: {
        vk_command_buffer_level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    }
    break;
    }

    VkCommandBufferAllocateInfo vk_command_buffer_allocate_info = {};
    vk_command_buffer_allocate_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    vk_command_buffer_allocate_info.pNext = nullptr;
    vk_command_buffer_allocate_info.commandPool = command_pool;
    vk_command_buffer_allocate_info.level = vk_command_buffer_level;
    vk_command_buffer_allocate_info.commandBufferCount = 1;

    VkResult result = device->GetDeviceDriver()->vkAllocateCommandBuffers(vk_device, &vk_command_buffer_allocate_info, &this->vkCommandBuffer);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TCommandBuffer::InternalCreate::vkAllocateCommandBuffers");
    }
}

void Turbo::Core::TCommandBufferBase::InternalDestroy()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkCommandPool command_pool = this->commandBufferPool->GetVkCommandPool();
    uint32_t command_buffer_count = 1;
    device->GetDeviceDriver()->vkFreeCommandBuffers(vk_device, command_pool, command_buffer_count, &this->vkCommandBuffer);
}

// Turbo::Core::TCommandBufferBase::TCommandBufferBase(const TRefPtr<TCommandBufferPool> &commandBufferPool, TCommandBufferLevel level) : Turbo::Core::TVulkanHandle()
//{
//     if (commandBufferPool->Valid())
//     {
//         this->commandBufferPool = commandBufferPool;
//         this->level = level;
//
//         this->InternalCreate();
//     }
//     else
//     {
//         throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TCommandBuffer::TCommandBuffer");
//     }
// }

Turbo::Core::TCommandBufferBase::TCommandBufferBase(TCommandBufferPool *commandBufferPool, TCommandBufferLevel level) : Turbo::Core::TVulkanHandle()
{
    if (!Turbo::Core::TReferenced::Valid(commandBufferPool))
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TCommandBuffer::TCommandBuffer");
    }

    {
        this->commandBufferPool = commandBufferPool;
        this->level = level;
        this->InternalCreate();
    }
}

Turbo::Core::TCommandBufferBase::~TCommandBufferBase()
{
    this->InternalDestroy();

    if (this->vkCommandBufferInheritanceInfo != nullptr)
    {
        delete this->vkCommandBufferInheritanceInfo;
    }
}

VkCommandBuffer Turbo::Core::TCommandBufferBase::GetVkCommandBuffer()
{
    return this->vkCommandBuffer;
}

Turbo::Core::TCommandBufferLevel Turbo::Core::TCommandBufferBase::GetLevel() const
{
    return this->level;
}

Turbo::Core::TCommandBufferPool *Turbo::Core::TCommandBufferBase::GetCommandBufferPool()
{
    return this->commandBufferPool;
}

bool Turbo::Core::TCommandBufferBase::Begin()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

    switch (this->level)
    {
    case TCommandBufferLevel::PRIMARY: {
        this->vkCommandBufferInheritanceInfo = nullptr;
    }
    break;
    case TCommandBufferLevel::SECONDARY: {

        if (this->vkCommandBufferInheritanceInfo == nullptr)
        {
            this->vkCommandBufferInheritanceInfo = new VkCommandBufferInheritanceInfo();
        }
        this->vkCommandBufferInheritanceInfo->sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        this->vkCommandBufferInheritanceInfo->pNext = NULL;
        this->vkCommandBufferInheritanceInfo->renderPass = this->currentRenderPass->GetVkRenderPass();
        this->vkCommandBufferInheritanceInfo->subpass = this->currentSubpass;
        this->vkCommandBufferInheritanceInfo->framebuffer = this->currentFramebuffer->GetVkFramebuffer();
        this->vkCommandBufferInheritanceInfo->occlusionQueryEnable = VK_FALSE;
        this->vkCommandBufferInheritanceInfo->queryFlags = 0;
        this->vkCommandBufferInheritanceInfo->pipelineStatistics = 0;
    }
    break;
    }

    VkCommandBufferBeginInfo vk_command_buffer_begin_info;
    vk_command_buffer_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vk_command_buffer_begin_info.pNext = nullptr;
    vk_command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    vk_command_buffer_begin_info.pInheritanceInfo = this->vkCommandBufferInheritanceInfo;

    VkResult result = device->GetDeviceDriver()->vkBeginCommandBuffer(this->vkCommandBuffer, &vk_command_buffer_begin_info);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }
    return false;
}

// void Turbo::Core::TCommandBufferBase::CmdBeginRenderPass(const TRefPtr<TRenderPass> &renderPass, const TRefPtr<TFramebuffer> &framebuffer, TSubpassContents subpassContents, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height)
//{
//     TDevice *device = renderPass->GetDevice();
//     TPhysicalDevice *physical_device = device->GetPhysicalDevice();
//
//     std::vector<TAttachment> attachemnts = renderPass->GetAttachments();
//     uint32_t attachemnts_count = attachemnts.size();
//
//     // TODO: Attachment clear color data should define in attachemnt
//     std::vector<VkClearValue> vk_clear_values;
//     for (uint32_t attachment_index = 0; attachment_index < attachemnts_count; attachment_index++)
//     {
//         TFormatInfo format_info = attachemnts[attachment_index].GetFormat();
//         TPhysicalDevice *physical_device = this->commandBufferPool->GetDeviceQueue()->GetDevice()->GetPhysicalDevice();
//         TFormatFeatures format_feature = physical_device->GetOptimalFeatures(format_info);
//
//         if ((format_feature & TFormatFeatureBits::FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == TFormatFeatureBits::FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
//         {
//             VkClearValue vk_clear_value = {};
//             vk_clear_value.depthStencil.depth = 1.0f;
//             vk_clear_value.depthStencil.stencil = 0.0f;
//
//             vk_clear_values.push_back(vk_clear_value);
//         }
//         else if ((format_feature & TFormatFeatureBits::FEATURE_COLOR_ATTACHMENT_BIT) == TFormatFeatureBits::FEATURE_COLOR_ATTACHMENT_BIT)
//         {
//             TFormatDataTypes format_data_types = format_info.GetFormatDataType();
//
//             VkClearColorValue vk_clear_color_value = {};
//             if ((format_data_types & TFormatDataTypeBits::SIGNED_INTEGER) == TFormatDataTypeBits::SIGNED_INTEGER)
//             {
//                 vk_clear_color_value.int32[0] = (int32_t)0;
//                 vk_clear_color_value.int32[1] = (int32_t)0;
//                 vk_clear_color_value.int32[2] = (int32_t)0;
//                 vk_clear_color_value.int32[3] = (int32_t)0;
//             }
//             else if ((format_data_types & TFormatDataTypeBits::UNSIGNED_INTEGER) == TFormatDataTypeBits::UNSIGNED_INTEGER)
//             {
//                 vk_clear_color_value.uint32[0] = (uint32_t)0;
//                 vk_clear_color_value.uint32[1] = (uint32_t)0;
//                 vk_clear_color_value.uint32[2] = (uint32_t)0;
//                 vk_clear_color_value.uint32[3] = (uint32_t)0;
//             }
//             else
//             {
//                 vk_clear_color_value.float32[0] = 0;
//                 vk_clear_color_value.float32[1] = 0;
//                 vk_clear_color_value.float32[2] = 0;
//                 vk_clear_color_value.float32[3] = 0;
//             }
//
//             VkClearValue vk_clear_value = {};
//             vk_clear_value.color = vk_clear_color_value;
//
//             vk_clear_values.push_back(vk_clear_value);
//         }
//         else
//         {
//             VkClearValue vk_clear_value = {};
//             vk_clear_values.push_back(vk_clear_value);
//         }
//     }
//
//     VkRenderPassBeginInfo vk_render_pass_begin_info = {};
//     vk_render_pass_begin_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//     vk_render_pass_begin_info.pNext = nullptr;
//     vk_render_pass_begin_info.renderPass = renderPass->GetVkRenderPass();
//     vk_render_pass_begin_info.framebuffer = framebuffer->GetVkFramebuffer();
//     vk_render_pass_begin_info.renderArea.offset.x = offsetX;
//     vk_render_pass_begin_info.renderArea.offset.y = offsetY;
//     vk_render_pass_begin_info.renderArea.extent.width = width;
//     vk_render_pass_begin_info.renderArea.extent.height = height;
//     if (width == TURBO_WHOLE_EXTENT)
//     {
//         vk_render_pass_begin_info.renderArea.extent.width = framebuffer->GetWidth();
//     }
//     if (height == TURBO_WHOLE_EXTENT)
//     {
//         vk_render_pass_begin_info.renderArea.extent.height = framebuffer->GetHeight();
//     }
//     vk_render_pass_begin_info.clearValueCount = vk_clear_values.size();
//     vk_render_pass_begin_info.pClearValues = vk_clear_values.data();
//
//     switch (subpassContents)
//     {
//     case TSubpassContents::INLINE: {
//         device->GetDeviceDriver()->vkCmdBeginRenderPass(this->vkCommandBuffer, &vk_render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
//     }
//     break;
//     case TSubpassContents::SECONDARY_COMMAND_BUFFERS: {
//         device->GetDeviceDriver()->vkCmdBeginRenderPass(this->vkCommandBuffer, &vk_render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
//     }
//     break;
//     }
//
//     this->currentRenderPass = renderPass;
//     this->currentFramebuffer = framebuffer;
// }

void Turbo::Core::TCommandBufferBase::CmdBeginRenderPass(TRenderPass *renderPass, TFramebuffer *framebuffer, TSubpassContents subpassContents, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height)
{
    if (!Turbo::Core::TReferenced::Valid(renderPass, framebuffer))
    {
        return;
    }

    {
        TDevice *device = renderPass->GetDevice();
        TPhysicalDevice *physical_device = device->GetPhysicalDevice();

        std::vector<TAttachment> attachemnts = renderPass->GetAttachments();
        uint32_t attachemnts_count = attachemnts.size();

        // TODO: Attachment clear color data should define in attachemnt
        std::vector<VkClearValue> vk_clear_values;
        for (uint32_t attachment_index = 0; attachment_index < attachemnts_count; attachment_index++)
        {
            TFormatInfo format_info = attachemnts[attachment_index].GetFormat();
            TPhysicalDevice *physical_device = this->commandBufferPool->GetDeviceQueue()->GetDevice()->GetPhysicalDevice();
            TFormatFeatures format_feature = physical_device->GetOptimalFeatures(format_info);

            if ((format_feature & TFormatFeatureBits::FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == TFormatFeatureBits::FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            {
                VkClearValue vk_clear_value = {};
                vk_clear_value.depthStencil.depth = 1.0f;
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

        switch (subpassContents)
        {
        case TSubpassContents::INLINE: {
            device->GetDeviceDriver()->vkCmdBeginRenderPass(this->vkCommandBuffer, &vk_render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
        }
        break;
        case TSubpassContents::SECONDARY_COMMAND_BUFFERS: {
            device->GetDeviceDriver()->vkCmdBeginRenderPass(this->vkCommandBuffer, &vk_render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
        }
        break;
        }

        this->currentRenderPass = renderPass;
        this->currentFramebuffer = framebuffer;
    }
}

// void Turbo::Core::TCommandBufferBase::CmdBindPipeline(const TRefPtr<TPipeline> &pipeline)
//{
//     TDevice *device = pipeline->GetDevice();
//     VkPipelineBindPoint vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;
//
//     switch (pipeline->GetType())
//     {
//     case TPipelineType::Compute: {
//         vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;
//     }
//     break;
//     case TPipelineType::Graphics: {
//         vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
//     }
//     break;
//     }
//
//     device->GetDeviceDriver()->vkCmdBindPipeline(this->vkCommandBuffer, vk_pipeline_bind_point, pipeline->GetVkPipeline());
//
//     this->currentPipeline = pipeline;
// }

void Turbo::Core::TCommandBufferBase::CmdBindPipeline(TPipeline *pipeline)
{
    if (!Turbo::Core::TReferenced::Valid(pipeline))
    {
        return;
    }

    TDevice *device = pipeline->GetDevice();
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

    device->GetDeviceDriver()->vkCmdBindPipeline(this->vkCommandBuffer, vk_pipeline_bind_point, pipeline->GetVkPipeline());

    this->currentPipeline = pipeline;
}

void Turbo::Core::TCommandBufferBase::CmdBindDescriptorSets(uint32_t firstSet, const std::vector<TRefPtr<TDescriptorSet>> &descriptorSets)
{
    std::vector<TDescriptorSet *> descriptor_sets;
    for (auto &item : descriptorSets)
    {
        descriptor_sets.push_back(item);
    }
    this->CmdBindDescriptorSets(firstSet, descriptor_sets);
}

void Turbo::Core::TCommandBufferBase::CmdBindDescriptorSets(uint32_t firstSet, const std::vector<TDescriptorSet *> &descriptorSets)
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
        for (TDescriptorSet *descriptor_set_item : descriptorSets)
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

// void Turbo::Core::TCommandBufferBase::CmdBindPipelineDescriptorSet(const TRefPtr<TPipelineDescriptorSet> &pipelineDescriptorSet)
//{
//     std::vector<TRefPtr<Turbo::Core::TDescriptorSet>> descriptor_sets = pipelineDescriptorSet->GetDescriptorSet();
//
//     for (TRefPtr<Turbo::Core::TDescriptorSet> &descriptor_set_item : descriptor_sets)
//     {
//         uint32_t first_set = descriptor_set_item->GetSet();
//         std::vector<Turbo::Core::TDescriptorSet *> descriptor_set{descriptor_set_item};
//         this->CmdBindDescriptorSets(first_set, descriptor_set);
//     }
// }

void Turbo::Core::TCommandBufferBase::CmdBindPipelineDescriptorSet(TPipelineDescriptorSet *pipelineDescriptorSet)
{
    if (!Turbo::Core::TReferenced::Valid(pipelineDescriptorSet))
    {
        return;
    }

    std::vector<TRefPtr<Turbo::Core::TDescriptorSet>> descriptor_sets = pipelineDescriptorSet->GetDescriptorSet();

    for (TRefPtr<Turbo::Core::TDescriptorSet> &descriptor_set_item : descriptor_sets)
    {
        uint32_t first_set = descriptor_set_item->GetSet();
        std::vector<Turbo::Core::TDescriptorSet *> descriptor_set{descriptor_set_item};
        this->CmdBindDescriptorSets(first_set, descriptor_set);
    }
}

void Turbo::Core::TCommandBufferBase::CmdBindVertexBuffers(const std::vector<TBuffer *> &vertexBuffers)
{
    if (!vertexBuffers.empty())
    {
        TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

        std::vector<VkBuffer> vertex_buffers;
        std::vector<VkDeviceSize> offsets;

        for (TBuffer *buffer_item : vertexBuffers)
        {
            vertex_buffers.push_back(buffer_item->GetVkBuffer());
            offsets.push_back(0);
        }

        device->GetDeviceDriver()->vkCmdBindVertexBuffers(this->vkCommandBuffer, 0, vertexBuffers.size(), vertex_buffers.data(), offsets.data());
    }
}

void Turbo::Core::TCommandBufferBase::CmdBindVertexBuffers(const std::vector<TRefPtr<TBuffer>> &vertexBuffers)
{
    std::vector<TBuffer *> vertex_buffers;
    for (auto &item : vertexBuffers)
    {
        vertex_buffers.push_back(item);
    }

    this->CmdBindVertexBuffers(vertex_buffers);
}

void Turbo::Core::TCommandBufferBase::CmdSetViewport(const std::vector<TViewport> &viewports)
{
    if (!viewports.empty())
    {
        TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

        std::vector<VkViewport> vk_viewports;
        for (const TViewport &viewport_item : viewports)
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

void Turbo::Core::TCommandBufferBase::CmdSetScissor(const std::vector<TScissor> &scissors)
{
    if (!scissors.empty())
    {
        TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

        std::vector<VkRect2D> vk_scissors;
        for (const TScissor &scissor_item : scissors)
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

void Turbo::Core::TCommandBufferBase::CmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdDraw(this->vkCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void Turbo::Core::TCommandBufferBase::CmdNextSubpass()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdNextSubpass(this->vkCommandBuffer, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
    this->currentSubpass += 1;
}

void Turbo::Core::TCommandBufferBase::CmdEndRenderPass()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdEndRenderPass(this->vkCommandBuffer);
    this->currentPipeline = nullptr;
    this->currentRenderPass = nullptr;
}

void Turbo::Core::TCommandBufferBase::CmdBeginRendering(const TRenderingAttachments &renderingAttachment, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    if (device->GetEnableDeviceFeatures().dynamicRendering)
    {
        std::vector<VkRenderingAttachmentInfo> color_attachment_infos;
        for (const Turbo::Core::TRenderingAttachments::TRenderingAttachment &rendering_attach_item : renderingAttachment.colorAttachments)
        {
            VkRenderingAttachmentInfo vk_rendering_attachment_info;
            vk_rendering_attachment_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
            vk_rendering_attachment_info.pNext = nullptr;
            vk_rendering_attachment_info.imageView = VK_NULL_HANDLE;
            vk_rendering_attachment_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
            if (rendering_attach_item.imageView.Valid())
            {
                vk_rendering_attachment_info.imageView = rendering_attach_item.imageView->GetVkImageView();
                vk_rendering_attachment_info.imageLayout = static_cast<VkImageLayout>(rendering_attach_item.layout);
            }

            vk_rendering_attachment_info.resolveMode = VkResolveModeFlagBits::VK_RESOLVE_MODE_NONE;
            vk_rendering_attachment_info.resolveImageView = VK_NULL_HANDLE;
            vk_rendering_attachment_info.resolveImageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
            if (rendering_attach_item.resolveImageView.Valid())
            {
                vk_rendering_attachment_info.resolveMode = static_cast<VkResolveModeFlagBits>(rendering_attach_item.resolveModeBits);
                vk_rendering_attachment_info.resolveImageView = rendering_attach_item.resolveImageView->GetVkImageView();
                vk_rendering_attachment_info.resolveImageLayout = static_cast<VkImageLayout>(rendering_attach_item.resolveLayout);
            }

            vk_rendering_attachment_info.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            vk_rendering_attachment_info.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
            vk_rendering_attachment_info.clearValue = {};
            if (rendering_attach_item.imageView.Valid())
            {
                vk_rendering_attachment_info.loadOp = static_cast<VkAttachmentLoadOp>(rendering_attach_item.loadOp);
                vk_rendering_attachment_info.storeOp = static_cast<VkAttachmentStoreOp>(rendering_attach_item.storeOp);

                Turbo::Core::TFormatType format_type = rendering_attach_item.imageView->GetFormat().GetFormatType();
                TPhysicalDevice *physical_device = device->GetPhysicalDevice();
                TFormatFeatures format_feature = physical_device->GetOptimalFeatures(format_type);
                Turbo::Core::TFormatInfo format_info = physical_device->GetFormatInfo(format_type);
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

                vk_rendering_attachment_info.clearValue = vk_clear_value;
            }

            color_attachment_infos.push_back(vk_rendering_attachment_info);
        }

        VkRenderingAttachmentInfo depth_attachment_info = {};
        depth_attachment_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        depth_attachment_info.pNext = nullptr;
        depth_attachment_info.imageView = VK_NULL_HANDLE;
        depth_attachment_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        if (renderingAttachment.depthAttachment.imageView.Valid())
        {
            depth_attachment_info.imageView = renderingAttachment.depthAttachment.imageView->GetVkImageView();
            depth_attachment_info.imageLayout = static_cast<VkImageLayout>(renderingAttachment.depthAttachment.layout);
        }
        depth_attachment_info.resolveMode = VkResolveModeFlagBits::VK_RESOLVE_MODE_NONE;
        depth_attachment_info.resolveImageView = VK_NULL_HANDLE;
        depth_attachment_info.resolveImageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        if (renderingAttachment.depthAttachment.resolveImageView.Valid())
        {
            depth_attachment_info.resolveMode = static_cast<VkResolveModeFlagBits>(renderingAttachment.depthAttachment.resolveModeBits);
            depth_attachment_info.resolveImageView = renderingAttachment.depthAttachment.resolveImageView->GetVkImageView();
            depth_attachment_info.resolveImageLayout = static_cast<VkImageLayout>(renderingAttachment.depthAttachment.resolveLayout);
        }
        depth_attachment_info.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment_info.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment_info.clearValue = {};
        if (renderingAttachment.depthAttachment.imageView.Valid())
        {
            depth_attachment_info.loadOp = static_cast<VkAttachmentLoadOp>(renderingAttachment.depthAttachment.loadOp);
            depth_attachment_info.storeOp = static_cast<VkAttachmentStoreOp>(renderingAttachment.depthAttachment.storeOp);
            depth_attachment_info.clearValue.depthStencil.depth = 1.0f;
        }

        VkRenderingAttachmentInfo stencil_attachment_info = {};
        stencil_attachment_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        stencil_attachment_info.pNext = nullptr;
        stencil_attachment_info.imageView = VK_NULL_HANDLE;
        stencil_attachment_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        if (renderingAttachment.stencilAttachment.imageView.Valid())
        {
            stencil_attachment_info.imageView = renderingAttachment.stencilAttachment.imageView->GetVkImageView();
            stencil_attachment_info.imageLayout = static_cast<VkImageLayout>(renderingAttachment.stencilAttachment.layout);
        }
        stencil_attachment_info.resolveMode = VkResolveModeFlagBits::VK_RESOLVE_MODE_NONE;
        stencil_attachment_info.resolveImageView = VK_NULL_HANDLE;
        stencil_attachment_info.resolveImageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        if (renderingAttachment.stencilAttachment.resolveImageView.Valid())
        {
            stencil_attachment_info.resolveMode = static_cast<VkResolveModeFlagBits>(renderingAttachment.stencilAttachment.resolveModeBits);
            stencil_attachment_info.resolveImageView = renderingAttachment.stencilAttachment.resolveImageView->GetVkImageView();
            stencil_attachment_info.resolveImageLayout = static_cast<VkImageLayout>(renderingAttachment.stencilAttachment.resolveLayout);
        }
        stencil_attachment_info.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        stencil_attachment_info.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
        stencil_attachment_info.clearValue = {};
        if (renderingAttachment.stencilAttachment.imageView.Valid())
        {
            stencil_attachment_info.loadOp = static_cast<VkAttachmentLoadOp>(renderingAttachment.stencilAttachment.loadOp);
            stencil_attachment_info.storeOp = static_cast<VkAttachmentStoreOp>(renderingAttachment.stencilAttachment.storeOp);
            stencil_attachment_info.clearValue.depthStencil.stencil = 0;
        }

        VkRenderingInfo vk_rendering_info = {};
        vk_rendering_info.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDERING_INFO;
        vk_rendering_info.pNext = nullptr;
        vk_rendering_info.flags = 0;
        vk_rendering_info.renderArea.offset.x = offsetX;
        vk_rendering_info.renderArea.offset.y = offsetY;
        vk_rendering_info.renderArea.extent.width = width;
        vk_rendering_info.renderArea.extent.height = height;
        TRefPtr<Turbo::Core::TImage> color_attachment_first_image = nullptr;
        if (!renderingAttachment.colorAttachments.empty())
        {
            if (renderingAttachment.colorAttachments[0].imageView.Valid())
            {
                color_attachment_first_image = renderingAttachment.colorAttachments[0].imageView->GetImage();
            }
        }
        if (width == TURBO_WHOLE_EXTENT)
        {
            if (color_attachment_first_image.Valid())
            {
                vk_rendering_info.renderArea.extent.width = color_attachment_first_image->GetWidth();
            }
        }
        if (height == TURBO_WHOLE_EXTENT)
        {
            if (color_attachment_first_image.Valid())
            {
                vk_rendering_info.renderArea.extent.height = color_attachment_first_image->GetHeight();
            }
        }
        vk_rendering_info.layerCount = 0;
        if (color_attachment_first_image.Valid())
        {
            vk_rendering_info.layerCount = color_attachment_first_image->GetArrayLayers();
        }
        vk_rendering_info.viewMask = 0;
        vk_rendering_info.colorAttachmentCount = color_attachment_infos.size();
        vk_rendering_info.pColorAttachments = color_attachment_infos.data();
        vk_rendering_info.pDepthAttachment = &depth_attachment_info;
        vk_rendering_info.pStencilAttachment = &stencil_attachment_info;

        PFN_vkCmdBeginRendering pfn_vkCmdBeginRendering = device->GetDeviceDriver()->vkCmdBeginRendering;
        if (pfn_vkCmdBeginRendering != nullptr)
        {
            pfn_vkCmdBeginRendering(this->vkCommandBuffer, &vk_rendering_info);
        }
    }
}

void Turbo::Core::TCommandBufferBase::CmdEndRendering()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    if (device->GetEnableDeviceFeatures().dynamicRendering)
    {
        PFN_vkCmdEndRendering pfn_vkCmdEndRendering = device->GetDeviceDriver()->vkCmdEndRendering;
        if (pfn_vkCmdEndRendering != nullptr)
        {
            pfn_vkCmdEndRendering(this->vkCommandBuffer);
        }
    }
}

void Turbo::Core::TCommandBufferBase::CmdDrawMeshTasksEXT(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    if (device->GetEnableDeviceFeatures().meshShaderEXT)
    {
        PFN_vkCmdDrawMeshTasksEXT pfn_vkCmdDrawMeshTasksEXT = device->GetDeviceDriver()->vkCmdDrawMeshTasksEXT;
        if (pfn_vkCmdDrawMeshTasksEXT != nullptr)
        {
            pfn_vkCmdDrawMeshTasksEXT(this->vkCommandBuffer, groupCountX, groupCountY, groupCountZ);
        }
    }
}

void Turbo::Core::TCommandBufferBase::CmdDrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    if (device->GetEnableDeviceFeatures().meshShaderNV)
    {
        PFN_vkCmdDrawMeshTasksNV pfn_vkCmdDrawMeshTasksNV = device->GetDeviceDriver()->vkCmdDrawMeshTasksNV;
        if (pfn_vkCmdDrawMeshTasksNV != nullptr)
        {
            pfn_vkCmdDrawMeshTasksNV(this->vkCommandBuffer, taskCount, firstTask);
        }
    }
}

bool Turbo::Core::TCommandBufferBase::End()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    VkResult result = device->GetDeviceDriver()->vkEndCommandBuffer(this->vkCommandBuffer);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }
    return false;
}

bool Turbo::Core::TCommandBufferBase::Reset()
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    VkResult result = device->GetDeviceDriver()->vkResetCommandBuffer(this->vkCommandBuffer, 0);
    if (result == VkResult::VK_SUCCESS)
    {
        return true;
    }
    return false;
}

void Turbo::Core::TCommandBufferBase::CmdPipelineBarrier(TPipelineStages srcStages, TPipelineStages dstStages, const std::vector<TMemoryBarrier> &memoryBarriers, const std::vector<TBufferMemoryBarrier> &bufferBarriers, const std::vector<TImageMemoryBarrier> &imageBarriers)
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

void Turbo::Core::TCommandBufferBase::CmdPipelineMemoryBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TMemoryBarrier &memoryBarrier)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

    VkMemoryBarrier vk_memory_barrier = {};
    vk_memory_barrier.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    vk_memory_barrier.pNext = nullptr;
    vk_memory_barrier.srcAccessMask = memoryBarrier.GetSrcAccess();
    vk_memory_barrier.dstAccessMask = memoryBarrier.GetDstAccess();

    std::vector<VkMemoryBarrier> vk_memory_barriers;
    vk_memory_barriers.push_back(vk_memory_barrier);

    device->GetDeviceDriver()->vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, vk_memory_barriers.size(), vk_memory_barriers.data(), 0, nullptr, 0, nullptr);
}

void Turbo::Core::TCommandBufferBase::CmdPipelineBufferBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TBufferMemoryBarrier &bufferBarrier)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, vk_buffer_memory_barriers.size(), vk_buffer_memory_barriers.data(), 0, nullptr);
}

void Turbo::Core::TCommandBufferBase::CmdPipelineImageBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TImageMemoryBarrier &imageBarrier)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdPipelineBarrier(this->vkCommandBuffer, (VkPipelineStageFlags)srcStages, (VkPipelineStageFlags)dstStages, VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, vk_image_memory_barriers.size(), vk_image_memory_barriers.data());
}

void Turbo::Core::TCommandBufferBase::CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, const TRefPtr<TImage> &image, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    TImageMemoryBarrier image_memory_barrier(srcAccess, dstAccess, image, oldLayout, newLayout, aspects, baseMipLevel, levelCount, baseArrayLayer, layerCount);
    this->CmdPipelineImageBarrier(srcStages, dstStages, image_memory_barrier);
}

void Turbo::Core::TCommandBufferBase::CmdTransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, const TRefPtr<TImageView> &imageView)
{
    TImageMemoryBarrier image_memory_barrier(srcAccess, dstAccess, imageView, oldLayout, newLayout);
    this->CmdPipelineImageBarrier(srcStages, dstStages, image_memory_barrier);
}

// void Turbo::Core::TCommandBufferBase::CmdFillBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset, TDeviceSize size, uint32_t data)
//{
//     TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
//     device->GetDeviceDriver()->vkCmdFillBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, data);
// }

void Turbo::Core::TCommandBufferBase::CmdFillBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, uint32_t data)
{
    if (!Turbo::Core::TReferenced::Valid(buffer))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdFillBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, data);
}

// void Turbo::Core::TCommandBufferBase::CmdFillBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset, TDeviceSize size, float data)
//{
//     TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
//     device->GetDeviceDriver()->vkCmdFillBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, *(const uint32_t *)&data);
// }

void Turbo::Core::TCommandBufferBase::CmdFillBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, float data)
{
    if (!Turbo::Core::TReferenced::Valid(buffer))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdFillBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, *(const uint32_t *)&data);
}

// void Turbo::Core::TCommandBufferBase::CmdUpdateBuffer(const TRefPtr<TBuffer> &buffer, TDeviceSize offset, TDeviceSize size, const void *data)
//{
//     if (buffer.Valid() && data != nullptr)
//     {
//         TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
//         device->GetDeviceDriver()->vkCmdUpdateBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, data);
//     }
// }

void Turbo::Core::TCommandBufferBase::CmdUpdateBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, const void *data)
{
    if (!Turbo::Core::TReferenced::Valid(buffer))
    {
        return;
    }

    {
        TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
        device->GetDeviceDriver()->vkCmdUpdateBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, size, data);
    }
}

// void Turbo::Core::TCommandBufferBase::CmdCopyBuffer(const TRefPtr<TBuffer> &srcBuffer, const TRefPtr<TBuffer> &dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size)
//{
//     TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
//
//     VkBufferCopy vk_buffer_copy = {};
//     vk_buffer_copy.srcOffset = srcOffset;
//     vk_buffer_copy.dstOffset = dstOffset;
//     vk_buffer_copy.size = size;
//
//     device->GetDeviceDriver()->vkCmdCopyBuffer(this->vkCommandBuffer, srcBuffer->GetVkBuffer(), dstBuffer->GetVkBuffer(), 1, &vk_buffer_copy);
// }

void Turbo::Core::TCommandBufferBase::CmdCopyBuffer(TBuffer *srcBuffer, TBuffer *dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size)
{
    if (!Turbo::Core::TReferenced::Valid(srcBuffer, dstBuffer))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

    VkBufferCopy vk_buffer_copy = {};
    vk_buffer_copy.srcOffset = srcOffset;
    vk_buffer_copy.dstOffset = dstOffset;
    vk_buffer_copy.size = size;

    device->GetDeviceDriver()->vkCmdCopyBuffer(this->vkCommandBuffer, srcBuffer->GetVkBuffer(), dstBuffer->GetVkBuffer(), 1, &vk_buffer_copy);
}

void Turbo::Core::TCommandBufferBase::CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (!Turbo::Core::TReferenced::Valid(image))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdClearColorImage(this->vkCommandBuffer, image->GetVkImage(), (VkImageLayout)layout, &vk_clear_color_value, 1, &vk_image_subresource_range);
}

void Turbo::Core::TCommandBufferBase::CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects)
{
    this->CmdClearColorImage(image, layout, r, g, b, a, aspects, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
}

void Turbo::Core::TCommandBufferBase::CmdClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a)
{
    this->CmdClearColorImage(image, layout, r, g, b, a, TImageAspectBits::ASPECT_COLOR_BIT, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
}

void Turbo::Core::TCommandBufferBase::CmdClearColorImage(TImageView *imageView, TImageLayout layout, float r, float g, float b, float a)
{
    if (!Turbo::Core::TReferenced::Valid(imageView))
    {
        return;
    }

    this->CmdClearColorImage(imageView->GetImage(), layout, r, g, b, a, imageView->GetAspects(), imageView->GetBaseMipLevel(), imageView->GetLevelCount(), imageView->GetBaseArrayLayer(), imageView->GetLayerCount());
}

void Turbo::Core::TCommandBufferBase::CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (!Turbo::Core::TReferenced::Valid(image))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

    VkClearDepthStencilValue vk_clear_depth_stencil_value = {};
    vk_clear_depth_stencil_value.depth = depth;
    vk_clear_depth_stencil_value.stencil = stencil;

    VkImageSubresourceRange vk_image_subresource_range = {};
    vk_image_subresource_range.aspectMask = aspects;
    vk_image_subresource_range.baseMipLevel = baseMipLevel;
    vk_image_subresource_range.levelCount = levelCount;
    vk_image_subresource_range.baseArrayLayer = baseMipLevel;
    vk_image_subresource_range.layerCount = layerCount;

    device->GetDeviceDriver()->vkCmdClearDepthStencilImage(this->vkCommandBuffer, image->GetVkImage(), (VkImageLayout)layout, &vk_clear_depth_stencil_value, 1, &vk_image_subresource_range);
}

void Turbo::Core::TCommandBufferBase::CmdClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects)
{
    this->CmdClearDepthStencilImage(image, layout, depth, stencil, aspects, 0, image->GetMipLevels(), 0, image->GetArrayLayers());
}

void Turbo::Core::TCommandBufferBase::CmdClearDepthStencilImage(TImageView *imageView, TImageLayout layout, float depth, uint32_t stencil)
{
    if (!Turbo::Core::TReferenced::Valid(imageView))
    {
        return;
    }

    this->CmdClearDepthStencilImage(imageView->GetImage(), layout, depth, stencil, imageView->GetAspects(), imageView->GetBaseMipLevel(), imageView->GetLevelCount(), imageView->GetBaseArrayLayer(), imageView->GetLayerCount());
}

void Turbo::Core::TCommandBufferBase::CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
    if (!Turbo::Core::TReferenced::Valid(image))
    {
        return;
    }

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

void Turbo::Core::TCommandBufferBase::CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects)
{
    if (!Turbo::Core::TReferenced::Valid(image))
    {
        return;
    }

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

void Turbo::Core::TCommandBufferBase::CmdClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil)
{
    if (!Turbo::Core::TReferenced::Valid(image))
    {
        return;
    }

    TImageUsages image_usages = image->GetUsages();
    if ((image_usages & TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT) == TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT)
    {
        this->CmdClearDepthStencilImage(image, layout, depth, stencil, TImageAspectBits::ASPECT_DEPTH_BIT | TImageAspectBits::ASPECT_STENCIL_BIT);
    }
    else
    {
        this->CmdClearColorImage(image, layout, r, g, b, a);
    }
}

void Turbo::Core::TCommandBufferBase::CmdClearImage(TImageView *imageView, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil)
{
    if (!Turbo::Core::TReferenced::Valid(imageView))
    {
        return;
    }

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

void Turbo::Core::TCommandBufferBase::CmdCopyBufferToImage(TBuffer *srcBuffer, TImage *dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth)
{
    if (!Turbo::Core::TReferenced::Valid(srcBuffer, dstImage))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdCopyBufferToImage(this->vkCommandBuffer, srcBuffer->GetVkBuffer(), dstImage->GetVkImage(), (VkImageLayout)layout, 1, &vk_buffer_image_copy);
}

void Turbo::Core::TCommandBufferBase::CmdCopyImageToBuffer(TImage *srcImage, TImageLayout layout, TBuffer *dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth)
{
    if (!Turbo::Core::TReferenced::Valid(srcImage, dstBuffer))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdCopyImageToBuffer(this->vkCommandBuffer, srcImage->GetVkImage(), (VkImageLayout)layout, dstBuffer->GetVkBuffer(), 1, &vk_buffer_image_copy);
}

void Turbo::Core::TCommandBufferBase::CmdCopyImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth)
{
    if (!Turbo::Core::TReferenced::Valid(srcImage, dstImage))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdCopyImage(this->vkCommandBuffer, srcImage->GetVkImage(), (VkImageLayout)srcLayout, dstImage->GetVkImage(), (VkImageLayout)dstLayout, 1, &vk_image_copy);
}

void Turbo::Core::TCommandBufferBase::CmdBindIndexBuffer(TBuffer *buffer, TDeviceSize offset, TIndexType indexType)
{
    if (!Turbo::Core::TReferenced::Valid(buffer))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdBindIndexBuffer(this->vkCommandBuffer, buffer->GetVkBuffer(), offset, (VkIndexType)indexType);
}

void Turbo::Core::TCommandBufferBase::CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdDrawIndexed(this->vkCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void Turbo::Core::TCommandBufferBase::CmdBlitImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, int32_t srcStartOffsetX, int32_t srcStartOffsetY, int32_t srcStartOffsetZ, int32_t srcEndOffsetX, int32_t srcEndOffsetY, int32_t srcEndOffsetZ, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t dstStartOffsetX, int32_t dstStartOffsetY, int32_t dstStartOffsetZ, int32_t dstEndOffsetX, int32_t dstEndOffsetY, int32_t dstEndOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, TFilter filter)
{
    if (!Turbo::Core::TReferenced::Valid(srcImage, dstImage))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    device->GetDeviceDriver()->vkCmdBlitImage(this->vkCommandBuffer, srcImage->GetVkImage(), src_image_layout, dstImage->GetVkImage(), dst_image_layout, 1, &vk_image_blit, vk_filter);
}

void Turbo::Core::TCommandBufferBase::CmdResolveImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcOffsetX, int32_t srcOffsety, int32_t srcOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstOffsetX, int32_t dstOffsety, int32_t dstOffsetZ, uint32_t width, uint32_t height, uint32_t depth)
{
    if (!Turbo::Core::TReferenced::Valid(srcImage, dstImage))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

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

    VkImageResolve vk_image_resolve = {};
    vk_image_resolve.srcSubresource = src_subresource;
    vk_image_resolve.srcOffset.x = srcOffsetX;
    vk_image_resolve.srcOffset.y = srcOffsety;
    vk_image_resolve.srcOffset.z = srcOffsetZ;
    vk_image_resolve.dstSubresource = dst_subresource;
    vk_image_resolve.dstOffset.x = dstOffsetX;
    vk_image_resolve.dstOffset.y = dstOffsety;
    vk_image_resolve.dstOffset.z = dstOffsetZ;
    vk_image_resolve.extent.width = width;
    vk_image_resolve.extent.height = height;
    vk_image_resolve.extent.depth = depth;

    device->GetDeviceDriver()->vkCmdResolveImage(this->vkCommandBuffer, srcImage->GetVkImage(), (VkImageLayout)srcLayout, dstImage->GetVkImage(), (VkImageLayout)dstLayout, 1, &vk_image_resolve);
}

void Turbo::Core::TCommandBufferBase::CmdPushConstants(TPipelineLayout *pipelineLayout, uint32_t offset, uint32_t size, const void *values)
{
    if (!Turbo::Core::TReferenced::Valid(pipelineLayout))
    {
        return;
    }

    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();

    VkShaderStageFlags vk_shader_stage_flags = 0;
    std::vector<TPushConstantDescriptor *> push_constant_descriptors = pipelineLayout->GetPushConstantDescriptors();
    for (TPushConstantDescriptor *push_constant_item : push_constant_descriptors)
    {
        vk_shader_stage_flags |= push_constant_item->GetShader()->GetVkShaderStageFlags();
    }

    device->GetDeviceDriver()->vkCmdPushConstants(this->vkCommandBuffer, pipelineLayout->GetVkPipelineLayout(), vk_shader_stage_flags, offset, size, values);
}

void Turbo::Core::TCommandBufferBase::CmdPushConstants(uint32_t offset, uint32_t size, const void *values)
{
    if (this->currentPipeline.Valid())
    {
        this->CmdPushConstants(this->currentPipeline->GetPipelineLayout(), offset, size, values);
    }
}

void Turbo::Core::TCommandBufferBase::CmdDispatch(uint32_t workGroupsX, uint32_t workGroupsY, uint32_t workGroupsZ)
{
    if (this->currentPipeline->GetType() == TPipelineType::Compute)
    {
        TDevice *device = this->currentPipeline->GetDevice();
        device->GetDeviceDriver()->vkCmdDispatch(this->vkCommandBuffer, workGroupsX, workGroupsY, workGroupsZ);
    }
    else
    {
        // TODO: throw some alarm?
    }
}

void Turbo::Core::TCommandBufferBase::CmdSetLineWidth(float lineWidth)
{
    TDevice *device = this->commandBufferPool->GetDeviceQueue()->GetDevice();
    device->GetDeviceDriver()->vkCmdSetLineWidth(this->vkCommandBuffer, lineWidth);
}

bool Turbo::Core::TCommandBufferBase::Valid() const
{
    if (this->vkCommandBuffer != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}

Turbo::Core::TSecondaryCommandBuffer::TSecondaryCommandBuffer(const TRefPtr<TCommandBufferPool> &commandBufferPool) : Turbo::Core::TCommandBufferBase(commandBufferPool, TCommandBufferLevel::SECONDARY)
{
}

Turbo::Core::TSecondaryCommandBuffer::~TSecondaryCommandBuffer()
{
}

void Turbo::Core::TSecondaryCommandBuffer::Begin(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t subpass)
{
    if (!Turbo::Core::TReferenced::Valid(renderPass, framebuffer))
    {
        return;
    }

    this->currentRenderPass = renderPass;
    this->currentFramebuffer = framebuffer;
    this->currentSubpass = subpass;

    TCommandBufferBase::Begin();
}

std::string Turbo::Core::TSecondaryCommandBuffer::ToString() const
{
    return std::string();
}

Turbo::Core::TCommandBuffer::TCommandBuffer(TCommandBufferPool *commandBufferPool) : Turbo::Core::TCommandBufferBase(commandBufferPool, TCommandBufferLevel::PRIMARY)
{
}

Turbo::Core::TCommandBuffer::~TCommandBuffer()
{
}

void Turbo::Core::TCommandBuffer::CmdExecuteCommand(TSecondaryCommandBuffer *secondaryCommandBuffer)
{
    if (!Turbo::Core::TReferenced::Valid(secondaryCommandBuffer))
    {
        return;
    }

    TCommandBufferPool *command_buffer_pool = this->GetCommandBufferPool();
    TDevice *device = command_buffer_pool->GetDeviceQueue()->GetDevice();

    VkCommandBuffer vk_secondary_command_buffer = secondaryCommandBuffer->GetVkCommandBuffer();
    device->GetDeviceDriver()->vkCmdExecuteCommands(this->GetVkCommandBuffer(), 1, &vk_secondary_command_buffer);
}

std::string Turbo::Core::TCommandBuffer::ToString() const
{
    return std::string();
}
