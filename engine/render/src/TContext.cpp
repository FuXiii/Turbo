#include "render/include/TContext.h"
#include "render/include/TImage.h"
#include "render/include/TPipeline.h"
#include "render/include/TRenderPass.h"
#include "render/include/TShader.h"
#include "vulkan/vulkan_core.h"
#include <algorithm>
#include <core/include/TAttachment.h>
#include <core/include/TBuffer.h>
#include <core/include/TCommandBuffer.h>
#include <core/include/TCommandBufferPool.h>
#include <core/include/TCore.h>
#include <core/include/TDeviceQueue.h>
#include <core/include/TException.h>
#include <core/include/TFence.h>
#include <core/include/TFramebuffer.h>
#include <core/include/TGraphicsPipeline.h>
#include <core/include/TImage.h>
#include <core/include/TInstance.h>
#include <core/include/TPhysicalDevice.h>
#include <core/include/TRenderPass.h>
#include <core/include/TSampler.h>
#include <core/include/TScissor.h>
#include <core/include/TVersion.h>
#include <core/include/TViewport.h>
#include <core/include/TVulkanLoader.h>
#include <cstdint>
#include <framegraph/include/TFrameGraph.hpp>
#include <stdint.h>
#include <vector>

// FIXME:没有实现完全
void Turbo::Render::TGraphicsPipelinePool::CreateGraphicsPipeline(Turbo::Render::TRenderPass &renderPass, uint32_t subpass, Turbo::Render::TGraphicsPipeline &graphicsPipeline)
{
    if (this->context != nullptr)
    {
        Turbo::Core::TDevice *device = this->context->GetDevice();

        Turbo::Core::TRenderPass *render_pass = renderPass.renderPass;

        std::vector<Turbo::Core::TVertexBinding *> vertex_binding_ptrs = this->context->GetVertexBindings();

        std::vector<Turbo::Core::TVertexBinding> vertex_bindings;
        for (Turbo::Core::TVertexBinding *vertex_binding_ptr_item : vertex_binding_ptrs)
        {
            vertex_bindings.push_back(*vertex_binding_ptr_item);
        }

        Turbo::Render::TVertexShader *render_vertex_shader = graphicsPipeline.GetVertexShader();
        Turbo::Render::TFragmentShader *render_fragment_shader = graphicsPipeline.GetFragmentShader();
        if (render_vertex_shader == nullptr || render_fragment_shader == nullptr)
        {
            // FIXME: retur false;//create failed
            throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TGraphicsPipelinePool::CreateGraphicsPipeline", "Please make sure set valid shader");
        }

        // TRenderPass *renderPass,
        // uint32_t subpass,
        // std::vector<TVertexBinding> &vertexBindings,
        // TVertexShader *vertexShader,
        // TFragmentShader *fragmentShader,
        // TTopologyType topology = TTopologyType::TRIANGLE_LIST,
        // bool primitiveRestartEnable = false,
        // bool depthClampEnable = false,
        // bool rasterizerDiscardEnable = false,
        // TPolygonMode polygonMode = TPolygonMode::FILL,
        // TCullModes cullMode = TCullModeBits::MODE_BACK_BIT,
        // TFrontFace frontFace = TFrontFace::CLOCKWISE,
        // bool depthBiasEnable = false,
        // float depthBiasConstantFactor = 0,
        // float depthBiasClamp = 0,
        // float depthBiasSlopeFactor = 0,
        // float lineWidth = 1,
        // bool multisampleEnable = false,
        // TSampleCountBits sample = TSampleCountBits::SAMPLE_1_BIT,
        // bool depthTestEnable = true,
        // bool depthWriteEnable = true,
        // TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL,
        // bool depthBoundsTestEnable = false,
        // bool stencilTestEnable = false,
        // TStencilOp frontFailOp = TStencilOp::KEEP,
        // TStencilOp frontPassOp = TStencilOp::KEEP,
        // TStencilOp frontDepthFailOp = TStencilOp::KEEP,
        // TCompareOp frontCompareOp = TCompareOp::ALWAYS,
        // uint32_t frontCompareMask = 0,
        // uint32_t frontWriteMask = 0,
        // uint32_t frontReference = 0,
        // TStencilOp backFailOp = TStencilOp::KEEP,
        // TStencilOp backPassOp = TStencilOp::KEEP,
        // TStencilOp backDepthFailOp = TStencilOp::KEEP,
        // TCompareOp backCompareOp = TCompareOp::ALWAYS,
        // uint32_t backCompareMask = 0,
        // uint32_t backWriteMask = 0,
        // uint32_t backReference = 0,
        // float minDepthBounds = 0,
        // float maxDepthBounds = 0,
        // bool logicOpEnable = false,
        // TLogicOp logicOp = TLogicOp::NO_OP,
        // bool blendEnable = false,
        // TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO,
        // TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO,
        // TBlendOp colorBlendOp = TBlendOp::ADD,
        // TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO,
        // TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO,
        // TBlendOp alphaBlendOp = TBlendOp::ADD,
        // float constantR = 1,
        // float constantG = 1,
        // float constantB = 1,
        // float constantA = 1

        Turbo::Core::TGraphicsPipeline *new_graphics_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, subpass, vertex_bindings, render_vertex_shader->GetVertexShader(), render_fragment_shader->GetFragmentShader());
        this->graphicsPipelineMap[renderPass.renderPass][subpass].push_back(new_graphics_pipeline);
        graphicsPipeline.graphicsPipeline = new_graphics_pipeline;
    }
}

// FIXME:没有实现完全
bool Turbo::Render::TGraphicsPipelinePool::Find(Turbo::Render::TRenderPass &renderPass, uint32_t subpass, Turbo::Render::TGraphicsPipeline &graphicsPipeline)
{
    if (renderPass.IsValid())
    {
        auto render_passs_map = this->graphicsPipelineMap.find(renderPass.renderPass);
        if (render_passs_map != this->graphicsPipelineMap.end())
        {
            auto subpass_map = render_passs_map->second.find(subpass);
            if (subpass_map != render_passs_map->second.end())
            {
                for (Turbo::Core::TGraphicsPipeline *graphics_pipeline_item : subpass_map->second)
                {
                    // TODO: find compatible pipeline
                    return false;
                }
            }
        }
    }

    return false;
}

Turbo::Render::TGraphicsPipelinePool::TGraphicsPipelinePool(TContext *context)
{
    if (context != nullptr)
    {
        this->context = context;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TGraphicsPipelinePool::TGraphicsPipelinePool", "Please make sure set valid TContext* parameter");
    }
}

Turbo::Render::TGraphicsPipelinePool::~TGraphicsPipelinePool()
{
    this->GC();
}

bool Turbo::Render::TGraphicsPipelinePool::Allocate(Turbo::Render::TRenderPass &renderPass, uint32_t subpass, Turbo::Render::TGraphicsPipeline &graphicsPipeline)
{
    if (this->Find(renderPass, subpass, graphicsPipeline))
    {
        return true;
    }

    this->CreateGraphicsPipeline(renderPass, subpass, graphicsPipeline);
    return true;
}

void Turbo::Render::TGraphicsPipelinePool::GC()
{
    for (auto &render_pass_item : this->graphicsPipelineMap)
    {
        for (auto &subpass_item : render_pass_item.second)
        {
            for (Turbo::Core::TGraphicsPipeline *graphics_pipeline_item : subpass_item.second)
            {
                delete graphics_pipeline_item; // FIXME: <<<---此时发生了内存泄漏
            }
        }
    }

    this->graphicsPipelineMap.clear();
}

Turbo::Render::TRenderPassPool::TRenderPassPool(TContext *context)
{
    if (context != nullptr)
    {
        this->context = context;
        this->framebufferPool = new TFramebufferPool(context);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TRenderPassPool::TRenderPassPool", "Please make sure set valid TContext* parameter");
    }
}

Turbo::Render::TRenderPassPool::~TRenderPassPool()
{
    // TODO:release this->renderPassProxies;
    delete this->framebufferPool;

    for (Turbo::Core::TRenderPass *render_pass_item : this->renderPasses)
    {
        delete render_pass_item;
    }

    this->context = nullptr;
}

void Turbo::Render::TRenderPassPool::CreateRenderPass(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:Create Turbo::Core::TRenderPass from Turbo::Render::TRenderPass
    // TODO:this->renderPass = new Turbo::Core::TRenderPass(...);

    // TODO: 这里在构建Attachment时需要指定ImageLayout，由于ImageLayout可以有很多种布局，目前并不知道之前Image的布局，目前默认为UNDEFINED。而在Filament中大部分Image的布局都为UNDEFINED或者GENERAL（这个布局用的最多）
    // TODO: Filament中对于Attachment的initialLayout多为UNDEFINED或者GENERAL，对于finalLayout多为GENERAL，如果作为采样纹理的话则为SHADER_READ_ONLY_OPTIMAL
    // TODO: 详情请参考Filament中/filament/backend/src/vulkan/VulkanFboCache.cpp : VkRenderPass VulkanFboCache::getRenderPass(RenderPassKey config) noexcept
    if (this->context != nullptr)
    {
        Turbo::Core::TDevice *device = this->context->GetDevice();

        std::vector<VkImage> frame_buffer_layout;
        std::vector<Turbo::Core::TAttachment> core_attachments;
        std::vector<Turbo::Core::TSubpass> core_subpasses;

        std::vector<Turbo::Render::TSubpass> subpasses = renderPass.GetSubpasses();
        for (Turbo::Render::TSubpass &subpass_item : subpasses)
        {
            Turbo::Core::TSubpass core_subpass(Turbo::Core::TPipelineType::Graphics);

            std::vector<Turbo::Render::TColorImage> color_attachments = subpass_item.GetColorAttachments();
            std::vector<Turbo::Render::TImage> input_attachments = subpass_item.GetInputAttachments();
            Turbo::Render::TDepthStencilImage depth_stencil_attachment = subpass_item.GetDepthStencilAttachment();

            for (Turbo::Render::TColorImage &core_color_attachment_item : color_attachments)
            {
                bool is_found = false;
                uint32_t frame_buffer_layout_index = 0;
                for (; frame_buffer_layout_index < frame_buffer_layout.size(); frame_buffer_layout_index++)
                {
                    VkImage frame_buffer_vk_image = frame_buffer_layout[frame_buffer_layout_index];
                    if (core_color_attachment_item.IsValid())
                    {
                        VkImage attachment_vk_image = core_color_attachment_item.image->GetVkImage();
                        if (frame_buffer_vk_image == attachment_vk_image)
                        {
                            is_found = true;
                            break;
                        }
                    }
                }

                if (is_found)
                {
                    Turbo::Render::TFormat format = core_color_attachment_item.GetFormat();
                    Turbo::Render::TSampleCountBits sample_count_bits = core_color_attachment_item.GetSampleCountBits();
                    Turbo::Core::TFormatInfo format_info = device->GetPhysicalDevice()->GetFormatInfo((Turbo::Core::TFormatType)format);

                    Turbo::Core::TAttachment color_attachment(format_info, (Turbo::Core::TSampleCountBits)sample_count_bits, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::GENERAL, Core::TImageLayout::GENERAL);
                    core_subpass.AddColorAttachmentReference(frame_buffer_layout_index, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);
                }
                else
                {
                    frame_buffer_layout.push_back(core_color_attachment_item.image->GetVkImage());

                    Turbo::Render::TFormat format = core_color_attachment_item.GetFormat();
                    Turbo::Render::TSampleCountBits sample_count_bits = core_color_attachment_item.GetSampleCountBits();
                    Turbo::Core::TFormatInfo format_info = device->GetPhysicalDevice()->GetFormatInfo((Turbo::Core::TFormatType)format);

                    Turbo::Core::TAttachment color_attachment(format_info, (Turbo::Core::TSampleCountBits)sample_count_bits, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::GENERAL, Core::TImageLayout::GENERAL);
                    core_attachments.push_back(color_attachment);
                    core_subpass.AddColorAttachmentReference(frame_buffer_layout.size() - 1, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);
                }
            }

            for (Turbo::Render::TImage &core_input_attachment_item : input_attachments)
            {
                bool is_found = false;
                uint32_t frame_buffer_layout_index = 0;
                for (; frame_buffer_layout_index < frame_buffer_layout.size(); frame_buffer_layout_index++)
                {
                    VkImage frame_buffer_vk_image = frame_buffer_layout[frame_buffer_layout_index];
                    if (core_input_attachment_item.IsValid())
                    {
                        VkImage attachment_vk_image = core_input_attachment_item.image->GetVkImage();
                        if (frame_buffer_vk_image == attachment_vk_image)
                        {
                            is_found = true;
                            break;
                        }
                    }
                }

                if (is_found)
                {
                    Turbo::Render::TFormat format = core_input_attachment_item.GetFormat();
                    Turbo::Render::TSampleCountBits sample_count_bits = core_input_attachment_item.GetSampleCountBits();
                    Turbo::Core::TFormatInfo format_info = device->GetPhysicalDevice()->GetFormatInfo((Turbo::Core::TFormatType)format);

                    Turbo::Core::TAttachment input_attachment(format_info, (Turbo::Core::TSampleCountBits)sample_count_bits, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::GENERAL, Core::TImageLayout::GENERAL);
                    core_subpass.AddInputAttachmentReference(frame_buffer_layout_index, Turbo::Core::TImageLayout::ATTACHMENT_OPTIMAL);
                }
                else
                {
                    frame_buffer_layout.push_back(core_input_attachment_item.image->GetVkImage());

                    Turbo::Render::TFormat format = core_input_attachment_item.GetFormat();
                    Turbo::Render::TSampleCountBits sample_count_bits = core_input_attachment_item.GetSampleCountBits();
                    Turbo::Core::TFormatInfo format_info = device->GetPhysicalDevice()->GetFormatInfo((Turbo::Core::TFormatType)format);

                    Turbo::Core::TAttachment input_attachment(format_info, (Turbo::Core::TSampleCountBits)sample_count_bits, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::GENERAL, Core::TImageLayout::GENERAL);
                    core_attachments.push_back(input_attachment);
                    core_subpass.AddInputAttachmentReference(frame_buffer_layout.size() - 1, Turbo::Core::TImageLayout::ATTACHMENT_OPTIMAL);
                }
            }

            if (depth_stencil_attachment.IsValid())
            {
                bool is_found = false;
                uint32_t frame_buffer_layout_index = 0;
                for (; frame_buffer_layout_index < frame_buffer_layout.size(); frame_buffer_layout_index++)
                {
                    VkImage frame_buffer_vk_image = frame_buffer_layout[frame_buffer_layout_index];
                    if (depth_stencil_attachment.IsValid())
                    {
                        VkImage attachment_vk_image = depth_stencil_attachment.image->GetVkImage();
                        if (frame_buffer_vk_image == attachment_vk_image)
                        {
                            is_found = true;
                            break;
                        }
                    }
                }

                if (is_found)
                {
                    Turbo::Render::TFormat format = depth_stencil_attachment.GetFormat();
                    Turbo::Render::TSampleCountBits sample_count_bits = depth_stencil_attachment.GetSampleCountBits();
                    Turbo::Core::TFormatInfo format_info = device->GetPhysicalDevice()->GetFormatInfo((Turbo::Core::TFormatType)format);

                    Turbo::Core::TAttachment depth_stencil_attachment(format_info, (Turbo::Core::TSampleCountBits)sample_count_bits, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::GENERAL, Core::TImageLayout::GENERAL);
                    core_subpass.SetDepthStencilAttachmentReference(frame_buffer_layout_index, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                }
                else
                {
                    frame_buffer_layout.push_back(depth_stencil_attachment.image->GetVkImage());

                    Turbo::Render::TFormat format = depth_stencil_attachment.GetFormat();
                    Turbo::Render::TSampleCountBits sample_count_bits = depth_stencil_attachment.GetSampleCountBits();
                    Turbo::Core::TFormatInfo format_info = device->GetPhysicalDevice()->GetFormatInfo((Turbo::Core::TFormatType)format);

                    Turbo::Core::TAttachment depth_stencil_attachment(format_info, (Turbo::Core::TSampleCountBits)sample_count_bits, Turbo::Core::TLoadOp::LOAD, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::GENERAL, Core::TImageLayout::GENERAL);
                    core_attachments.push_back(depth_stencil_attachment);
                    core_subpass.SetDepthStencilAttachmentReference(frame_buffer_layout.size() - 1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                }
            }

            core_subpasses.push_back(core_subpass);
        }

        Turbo::Core::TRenderPass *new_render_pass = new Turbo::Core::TRenderPass(device, core_attachments, core_subpasses);
        renderPass.renderPass = new_render_pass;
        this->renderPasses.push_back(new_render_pass);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TRenderPassPool::TRenderPassProxy::Create", "Please make sure set valid TContext* parameter");
    }
}

bool Turbo::Render::TRenderPassPool::Find(Turbo::Render::TRenderPass &renderPass)
{
    // first we need to judge if renderPass which input from external was empty we need return empty TRenderPassProxy
    // if(renderPass.IsEmpty())...

    auto find_result = std::find_if(this->renderPasses.begin(), this->renderPasses.end(), [&](Turbo::Core::TRenderPass *render_pass_item) {
        std::vector<Turbo::Render::TSubpass> render_subpasses = renderPass.GetSubpasses();
        Turbo::Core::TRenderPass *core_render_pass = render_pass_item;
        if (core_render_pass != nullptr)
        {
            std::vector<Turbo::Core::TAttachment> core_attachments = core_render_pass->GetAttachments();
            std::vector<Turbo::Core::TSubpass> core_subpasses = core_render_pass->GetSubpasses();

            size_t render_subpass_count = render_subpasses.size();
            size_t core_subpass_count = core_subpasses.size();
            if (render_subpass_count == /*<=*/core_subpass_count) // TODO:try to test <=, if available
            {
                for (uint32_t subpass_index = 0; subpass_index < render_subpass_count; subpass_index++)
                {
                    Turbo::Core::TSubpass &core_subpass = core_subpasses[subpass_index];
                    Turbo::Render::TSubpass &render_subpass = render_subpasses[subpass_index];

                    std::vector<VkAttachmentReference> *core_color_attachment_references = core_subpass.GetColorAttachmentReferences();
                    std::vector<VkAttachmentReference> *core_input_attachment_references = core_subpass.GetInputAttachmentReferences();
                    VkAttachmentReference *core_depth_stencil_attachment_reference = core_subpass.GetDepthStencilAttachmentReference();

                    std::vector<Turbo::Core::TAttachment> core_color_attachments;
                    for (VkAttachmentReference &core_color_attachment_item : *core_color_attachment_references)
                    {
                        core_color_attachments.push_back(core_attachments[core_color_attachment_item.attachment]);
                    }

                    std::vector<Turbo::Core::TAttachment> core_input_attachments;
                    for (VkAttachmentReference &core_input_attachment_item : *core_input_attachment_references)
                    {
                        core_input_attachments.push_back(core_attachments[core_input_attachment_item.attachment]);
                    }

                    // Use vector to storage depth stencil the purpose is to determine whether DepthStencilAttachment exists
                    // usually we will only have one DepthStencilAttachment or without DepthStencilAttachment
                    std::vector<Turbo::Core::TAttachment> core_depth_stencil_attachments;
                    if (core_depth_stencil_attachment_reference != nullptr && core_depth_stencil_attachment_reference->attachment != UINT32_MAX)
                    {
                        core_depth_stencil_attachments.push_back(core_attachments[core_depth_stencil_attachment_reference->attachment]);
                    }

                    std::vector<Turbo::Render::TColorImage> render_color_attachments = render_subpass.GetColorAttachments();
                    std::vector<Turbo::Render::TImage> render_input_attachments = render_subpass.GetInputAttachments();
                    std::vector<Turbo::Render::TDepthStencilImage> render_depth_stencil_attachments;
                    Turbo::Render::TDepthStencilImage render_depth_stencil_attachment = render_subpass.GetDepthStencilAttachment();
                    if (render_depth_stencil_attachment.IsValid())
                    {
                        render_depth_stencil_attachments.push_back(render_depth_stencil_attachment);
                    }

                    // To Compare Core:: and Render::
                    uint32_t core_color_attachments_count = core_color_attachments.size();
                    uint32_t core_input_attachments_count = core_input_attachments.size();
                    uint32_t core_depth_stencil_attachments_count = core_depth_stencil_attachments.size();

                    uint32_t render_color_attachments_count = render_color_attachments.size();
                    uint32_t render_input_attachments_count = render_input_attachments.size();
                    uint32_t render_depth_stencil_attachment_count = render_depth_stencil_attachments.size();

                    if (core_color_attachments_count < render_color_attachments_count)
                    {
                        return false;
                    }

                    if (core_input_attachments_count < render_input_attachments_count)
                    {
                        return false;
                    }

                    if (core_depth_stencil_attachments_count < render_depth_stencil_attachment_count)
                    {
                        return false;
                    }

                    // TODO: try to compare attachments between Core and Render
                    /*
                    Vulkan Spec: Two attachment references are compatible if they have matching format and sample count
                    {
                        Attachment A;
                        Attachment B;

                        A == B
                        {
                            A.format == B.format;
                            A.sample == B.sample;
                        }
                    }

                    Vulkan Spec: Two arrays of attachment references are compatible if all corresponding pairs of attachments are
                    compatible. If the arrays are of different lengths, attachment references not present in the smaller
                    array are treated as VK_ATTACHMENT_UNUSED
                    {
                        std::vector<Attachment> A;
                        std::vector<Attachment> B;

                        A.[0][1][2][3][4][5][6][7][8][9]
                        B.[0][1][2][3][4][5][/][/][/][/]

                        A == B
                        {
                            A.[0] == B.[0];
                            A.[1] == B.[1];
                            A.[2] == B.[2];
                            A.[3] == B.[3];
                            A.[4] == B.[4];
                            A.[5] == B.[5];
                            A.[6] == B.[/];
                            A.[7] == B.[/];
                            A.[8] == B.[/];
                            A.[9] == B.[/];
                        }
                    }

                    Vulkan Spec: Two render passes are compatible if their corresponding color, input, resolve, and depth/stencil
                    attachment references are compatible and if they are otherwise identical except for:
                    {
                        RenderPass A;
                        RenderPass B;

                        A == B
                        {
                            A.colors == B.colors
                            A.inputs == B.inputs
                            A.depthStencil == B.depthStencil
                        }
                    }
                    */

                    for (uint32_t color_attachment_index = 0; color_attachment_index < render_color_attachments_count; color_attachment_index++)
                    {
                        Turbo::Core::TAttachment &core_color_attachment = core_color_attachments[color_attachment_index];
                        Turbo::Render::TColorImage &render_color_attachment = render_color_attachments[color_attachment_index];

                        Turbo::Core::TFormatType core_format = core_color_attachment.GetFormat().GetFormatType();
                        Turbo::Core::TFormatType render_format = (Turbo::Core::TFormatType)render_color_attachment.GetFormat();

                        if (core_format != render_format)
                        {
                            return false;
                        }

                        Turbo::Core::TSampleCountBits core_sample_count = core_color_attachment.GetVkSampleCountFlagBits();
                        Turbo::Core::TSampleCountBits render_sample_count = (Turbo::Core::TSampleCountBits)render_color_attachment.GetSampleCountBits();

                        if (core_sample_count != render_sample_count)
                        {
                            return false;
                        }
                    }

                    for (uint32_t input_attachment_index = 0; input_attachment_index < render_input_attachments_count; input_attachment_index++)
                    {
                        Turbo::Core::TAttachment &core_input_attachment = core_input_attachments[input_attachment_index];
                        Turbo::Render::TImage &render_input_attachment = render_input_attachments[input_attachment_index];

                        Turbo::Core::TFormatType core_format = core_input_attachment.GetFormat().GetFormatType();
                        Turbo::Core::TFormatType render_format = (Turbo::Core::TFormatType)render_input_attachment.GetFormat();

                        if (core_format != render_format)
                        {
                            return false;
                        }

                        Turbo::Core::TSampleCountBits core_sample_count = core_input_attachment.GetVkSampleCountFlagBits();
                        Turbo::Core::TSampleCountBits render_sample_count = (Turbo::Core::TSampleCountBits)render_input_attachment.GetSampleCountBits();

                        if (core_sample_count != render_sample_count)
                        {
                            return false;
                        }
                    }

                    for (uint32_t depth_stencil_attachment_index = 0; depth_stencil_attachment_index < render_depth_stencil_attachment_count; depth_stencil_attachment_index++)
                    {
                        Turbo::Core::TAttachment &core_depth_stencil_attachment = core_depth_stencil_attachments[depth_stencil_attachment_index];
                        Turbo::Render::TDepthStencilImage &render_depth_stencil_attachment = render_depth_stencil_attachments[depth_stencil_attachment_index];

                        Turbo::Core::TFormatType core_format = core_depth_stencil_attachment.GetFormat().GetFormatType();
                        Turbo::Core::TFormatType render_format = (Turbo::Core::TFormatType)render_depth_stencil_attachment.GetFormat();

                        if (core_format != render_format)
                        {
                            return false;
                        }

                        Turbo::Core::TSampleCountBits core_sample_count = core_depth_stencil_attachment.GetVkSampleCountFlagBits();
                        Turbo::Core::TSampleCountBits render_sample_count = (Turbo::Core::TSampleCountBits)render_depth_stencil_attachment.GetSampleCountBits();

                        if (core_sample_count != render_sample_count)
                        {
                            return false;
                        }
                    }
                }

                return true;
            }
        }

        return false;
    });

    if (find_result != this->renderPasses.end())
    {
        renderPass.renderPass = (*find_result);
        return true;
    }

    return false;
}

bool Turbo::Render::TRenderPassPool::Allocate(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:return a valid TRenderPassProxy
    // TODO:find a valid RenderPassProxy
    // TODO:if not found create a new RenderPassProxy/RenderPass
    // TODO:if found return what we want
    if (!renderPass.IsEmpty())
    {
        if (this->Find(renderPass))
        {
            if (this->framebufferPool->Allocate(renderPass))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // create a new RenderPass/TRenderPassProxy
        this->CreateRenderPass(renderPass); // FIXME:maybe create failed?
        if (this->framebufferPool->Allocate(renderPass))
        {
            return true;
        }
    }

    return false;
}

void Turbo::Render::TRenderPassPool::Free(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:let renderPass into pool
    // this->renderPassProxies[x].Destroy();
}

void Turbo::Render::TRenderPassPool::GC()
{
    this->framebufferPool->GC();
}

Turbo::Render::TFramebufferPool::TFramebufferPool(TContext *context)
{
    if (context != nullptr)
    {
        this->context = context;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TFramebufferPool::TFramebufferPool", "Please make sure set valid TContext* parameter");
    }
}

Turbo::Render::TFramebufferPool::~TFramebufferPool()
{
    for (Turbo::Core::TFramebuffer *frame_buffer_item : this->framebuffers)
    {
        delete frame_buffer_item;
    }

    this->context = nullptr;
}

void Turbo::Render::TFramebufferPool::CreateFramebuffer(Turbo::Render::TRenderPass &renderPass)
{
    // TODO:Create a new Turbo::Core::TFramebuffer
    // TODO:add into this->framebuffers
    std::vector<Turbo::Render::TImage> render_pass_attachments = renderPass.GetAttachments();

    std::vector<Turbo::Core::TImageView *> attachments;
    for (Turbo::Render::TImage &image_item : render_pass_attachments)
    {
        attachments.push_back(image_item.imageView);
    }
    Turbo::Core::TFramebuffer *new_frame_buffer = new Turbo::Core::TFramebuffer(renderPass.renderPass, attachments);
    renderPass.framebuffer = new_frame_buffer;
    this->framebuffers.push_back(new_frame_buffer);
}

bool Turbo::Render::TFramebufferPool::Find(Turbo::Render::TRenderPass &renderPass)
{
    auto find_result = std::find_if(this->framebuffers.begin(), this->framebuffers.end(), [&](Turbo::Core::TFramebuffer *frame_buffer_item) {
        Turbo::Core::TFramebuffer *core_frame_buffer = frame_buffer_item;
        if (core_frame_buffer != nullptr && renderPass.renderPass != nullptr)
        {
            std::vector<Turbo::Core::TImageView *> frame_buffer_attachments = core_frame_buffer->GetAttachments();
            std::vector<Turbo::Render::TImage> render_pass_attachments = renderPass.GetAttachments();

            uint32_t frame_buffer_attachment_count = frame_buffer_attachments.size();
            uint32_t render_pass_attachment_count = render_pass_attachments.size();
            if (frame_buffer_attachment_count >= render_pass_attachment_count)
            {
                for (uint32_t attachment_index = 0; attachment_index < render_pass_attachment_count; attachment_index++)
                {
                    Turbo::Core::TImage *frame_buffer_attachment_image = frame_buffer_attachments[attachment_index]->GetImage();
                    Turbo::Core::TImage *render_pass_attachment_image = render_pass_attachments[attachment_index].imageView->GetImage();

                    if (frame_buffer_attachment_image != render_pass_attachment_image)
                    {
                        return false;
                    }
                }

                return true;
            }
        }
        return false;
    });

    if (find_result != this->framebuffers.end())
    {
        renderPass.framebuffer = (*find_result);
        return true;
    }

    return false;
}

bool Turbo::Render::TFramebufferPool::Allocate(Turbo::Render::TRenderPass &renderPass)
{
    if (this->Find(renderPass))
    {
        return true;
    }

    // TODO:Create a new Framebuffer
    this->CreateFramebuffer(renderPass); // FIXME: if create failed?
    return true;
}

void Turbo::Render::TFramebufferPool::Free(Turbo::Render::TRenderPass &renderPass)
{
    //
}

void Turbo::Render::TFramebufferPool::GC()
{
    for (Turbo::Core::TFramebuffer *frame_buffer_item : this->framebuffers)
    {
        delete frame_buffer_item;
    }

    this->framebuffers.clear();
}

Turbo::Render::TContext::TContext()
{
    // check Vulkan support
    if (!Turbo::Core::TInstance::IsSupportVulkan())
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Render::TContext::TContext()", "Unsupport Vulkan, please try to update your GPU drive or install Vulkan Runtime");
    }

    Turbo::Core::TVersion support_vulkan_version = Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion();

    // temp instance for collect information
    Turbo::Core::TInstance *temp_instance = new Turbo::Core::TInstance();
    Turbo::Core::TVersion temp_vulkan_version = temp_instance->GetVulkanVersion();
    Turbo::Core::TPhysicalDevice *temp_best_physical_device = temp_instance->GetBestPhysicalDevice();

    if (temp_best_physical_device->GetVkPhysicalDevice() != VK_NULL_HANDLE)
    {
        if (temp_best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion() >= support_vulkan_version.GetValidVulkanVersion())
        {
            Turbo::Core::TVersion instance_support_vulkan_version = support_vulkan_version.GetValidVulkanVersion();
            delete temp_instance;
            temp_instance = new Turbo::Core::TInstance(nullptr, nullptr, &instance_support_vulkan_version);
        }
        else
        {
            Turbo::Core::TVersion device_support_vulkan_version = temp_best_physical_device->GetDeviceApiVersion().GetValidVulkanVersion();
            delete temp_instance;
            temp_instance = new Turbo::Core::TInstance(nullptr, nullptr, &device_support_vulkan_version);
        }
    }

    Turbo::Core::TVersion instance_support_version = temp_instance->GetVulkanVersion();
    std::vector<Turbo::Core::TLayerInfo> instance_support_layers = temp_instance->GetSupportLayers();
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions = temp_instance->GetSupportExtensions();

    delete temp_instance;

    Turbo::Core::TLayerInfo khronos_validation_layer;
    for (Turbo::Core::TLayerInfo &layer : instance_support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation_layer = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_instance_layer;
    if (khronos_validation_layer.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_instance_layer.push_back(khronos_validation_layer);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;

    Turbo::Core::TExtensionInfo vk_khr_surface_extension;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            vk_khr_surface_extension = extension;
            break;
        }
    }

    if (vk_khr_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_surface_extension);
    }

#if defined(TURBO_PLATFORM_WINDOWS)
    Turbo::Core::TExtensionInfo vk_khr_win32_surface_extension;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE)
        {
            vk_khr_win32_surface_extension = extension;
            break;
        }
    }

    if (vk_khr_win32_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_win32_surface_extension);
    }
#elif defined(TURBO_PLATFORM_LINUX)
    Turbo::Core::TExtensionInfo vk_khr_wayland_surface_extension;
    Turbo::Core::TExtensionInfo vk_khr_xcb_surface_extension;
    Turbo::Core::TExtensionInfo vk_khr_xlib_surface_extension;

    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (vk_khr_wayland_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            vk_khr_wayland_surface_extension = extension;
        }
        else if (vk_khr_xcb_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            vk_khr_xcb_surface_extension = extension;
        }
        else if (vk_khr_xlib_surface_extension.GetExtensionType() == Turbo::Core::TExtensionType::UNDEFINED && extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            vk_khr_xlib_surface_extension = extension;
        }
    }

    if (vk_khr_wayland_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_wayland_surface_extension);
    }

    if (vk_khr_xcb_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_xcb_surface_extension);
    }

    if (vk_khr_xlib_surface_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_instance_extensions.push_back(vk_khr_xlib_surface_extension);
    }
#else
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Render::TContext::TContext()", "Turbo had not implemented this platform");
#endif

    // create instance
    this->instance = new Turbo::Core::TInstance(&enable_instance_layer, &enable_instance_extensions, &instance_support_version);
    std::cout << "Support Vulkan Version:" << support_vulkan_version.ToString() << std::endl;
    std::cout << "Turbo Vulkan Version:" << this->instance->GetVulkanVersion().ToString() << std::endl;

    this->physicalDevice = this->instance->GetBestPhysicalDevice();

    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = this->physicalDevice->GetSupportExtensions();

    Turbo::Core::TExtensionInfo vk_khr_swapchain_extension;
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            vk_khr_swapchain_extension = extension;
            break;
        }
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    if (vk_khr_swapchain_extension.GetExtensionType() != Turbo::Core::TExtensionType::UNDEFINED)
    {
        enable_device_extensions.push_back(vk_khr_swapchain_extension);
    }

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;
    
    this->device = new Turbo::Core::TDevice(this->physicalDevice, nullptr, &enable_device_extensions, &physical_device_features);
    this->graphicsQueue = this->device->GetBestGraphicsQueue();

    this->commandBufferPool = new Turbo::Core::TCommandBufferPool(this->graphicsQueue);
    this->currentCommandBuffer.commandBuffer = this->commandBufferPool->Allocate();
    this->currentCommandBuffer.fence = new Turbo::Core::TFence(this->device);
    this->currentCommandBuffer.commandBuffer->Begin();

    this->renderPassPool = new TRenderPassPool(this);
    this->graphicsPipelinePool = new TGraphicsPipelinePool(this);
}

Turbo::Render::TContext::~TContext()
{
    delete this->graphicsPipelinePool;
    delete this->renderPassPool;

    if (this->currentCommandBuffer.commandBuffer != nullptr)
    {
        this->commandBufferPool->Free(this->currentCommandBuffer.commandBuffer);
        this->currentCommandBuffer.commandBuffer = nullptr;
    }

    if (this->currentCommandBuffer.fence != nullptr)
    {
        delete this->currentCommandBuffer.fence;
        this->currentCommandBuffer.fence = nullptr;
    }

    delete this->commandBufferPool;
    this->commandBufferPool = nullptr;
    this->graphicsQueue = nullptr;
    delete this->device;
    this->physicalDevice = nullptr;
    delete this->instance;
}

Turbo::Core::TImage *Turbo::Render::TContext::CreateImage(const TImage::Descriptor &descriptor)
{
    TImageCreateFlags image_create_flags = descriptor.flags;
    TFormat format = descriptor.format;
    uint32_t width = descriptor.width;
    uint32_t height = descriptor.height;
    uint32_t depth = descriptor.depth;
    uint32_t layers = descriptor.layers;
    uint32_t mip_levels = descriptor.mipLevels;
    TImageUsages usages = descriptor.usages;
    TDomain domain = descriptor.domain;

    if (width == 0 && height == 0 && depth == 0)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Core::TImage *Turbo::Render::TContext::CreateImage(const TImage::Descriptor &descriptor)", "Try to create image, but all three dimensions are zero");
    }

    Turbo::Core::TDevice *device = this->device;

    VkImageCreateFlags vk_image_create_flags = 0;
    if ((image_create_flags & Turbo::Render::TImageCreateFlagBits::CUBE) == Turbo::Render::TImageCreateFlagBits::CUBE)
    {
        vk_image_create_flags |= VkImageCreateFlagBits::VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }

    Turbo::Core::TImageType type = Turbo::Core::TImageType::DIMENSION_3D;
    if (depth == 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_2D;
    }
    if (height == 1)
    {
        type = Turbo::Core::TImageType::DIMENSION_1D;
    }

    Turbo::Core::TFormatType format_type = static_cast<Turbo::Core::TFormatType>(format);

    Turbo::Core::TSampleCountBits sample_count_bits = Turbo::Core::TSampleCountBits::SAMPLE_1_BIT;

    Turbo::Core::TImageTiling image_tiling = Turbo::Core::TImageTiling::OPTIMAL;

    Turbo::Core::TImageUsages image_usages = usages;

    Turbo::Core::TMemoryFlags memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;

    if ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU)
    {
        image_tiling = Turbo::Core::TImageTiling::LINEAR;
    }
    else
    {
        image_tiling = Turbo::Core::TImageTiling::OPTIMAL;
    }

    // GPU only
    if (((domain & Turbo::Render::TDomainBits::CPU) != Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;
    }
    // Staging copy for upload
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE;
    }
    // Readback
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_RANDOM;
    }
    // Advanced data uploading(Both CPU and GPU domain can access)
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE | Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD;
    }

    Turbo::Core::TImageLayout layout = Turbo::Core::TImageLayout::UNDEFINED;

    return new Turbo::Core::TImage(this->device, vk_image_create_flags, type, format_type, width, height, depth, mip_levels, layers, sample_count_bits, image_tiling, image_usages, memory_flags, layout);
}

void Turbo::Render::TContext::DestroyImage(Turbo::Core::TImage *image)
{
    if (image != nullptr && image->GetVkImage() != VK_NULL_HANDLE)
    {
        delete image;
    }
}

Turbo::Core::TBuffer *Turbo::Render::TContext::CreateBuffer(const TBuffer::Descriptor &descriptor)
{
    TImageUsages usages = descriptor.usages;
    uint32_t size = descriptor.size;
    TDomain domain = descriptor.domain;

    Turbo::Core::TBufferUsages buffer_usages = usages;
    Turbo::Core::TMemoryFlags memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;

    // GPU only
    if (((domain & Turbo::Render::TDomainBits::CPU) != Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY;
    }
    // Staging copy for upload
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_SRC) == Turbo::Render::TImageUsageBits::TRANSFER_SRC) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE;
    }
    // Readback
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) != Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_RANDOM;
    }
    // Advanced data uploading(Both CPU and GPU domain can access)
    else if (((usages & Turbo::Render::TImageUsageBits::TRANSFER_DST) == Turbo::Render::TImageUsageBits::TRANSFER_DST) && ((domain & Turbo::Render::TDomainBits::CPU) == Turbo::Render::TDomainBits::CPU) && ((domain & Turbo::Render::TDomainBits::GPU) == Turbo::Render::TDomainBits::GPU))
    {
        memory_flags = Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE | Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD;
    }

    Turbo::Core::TImageUsages image_usages = usages;

    return new Turbo::Core::TBuffer(this->device, 0, buffer_usages, memory_flags, size);
}

void Turbo::Render::TContext::DestroyBuffer(Turbo::Core::TBuffer *buffer)
{
    if (buffer != nullptr && buffer->GetVkBuffer() != VK_NULL_HANDLE)
    {
        delete buffer;
    }
}

Turbo::Core::TCommandBuffer *Turbo::Render::TContext::AllocateCommandBuffer()
{
    return this->commandBufferPool->Allocate();
}

void Turbo::Render::TContext::FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer)
{
    this->commandBufferPool->Free(commandBuffer);
}

Turbo::Core::TSampler *Turbo::Render::TContext::CreateSampler(const Turbo::Render::TSampler::Descriptor &descriptor)
{
    return new Turbo::Core::TSampler(this->device, (Turbo::Core::TFilter)descriptor.min, (Turbo::Core::TFilter)descriptor.max, (Turbo::Core::TMipmapMode)descriptor.mipmap, (Turbo::Core::TAddressMode)descriptor.U, (Turbo::Core::TAddressMode)descriptor.V, (Turbo::Core::TAddressMode)descriptor.W);
}

void Turbo::Render::TContext::DestroySampler(Turbo::Core::TSampler *sampler)
{
    if (sampler != nullptr && sampler->GetVkSampler() != VK_NULL_HANDLE)
    {
        delete sampler;
    }
}

void Turbo::Render::TContext::ClearTexture(Turbo::Render::TTexture2D &texture2D, float r, float g, float b, float a)
{
    if (texture2D.IsValid())
    {
        this->currentCommandBuffer.commandBuffer->CmdClearColorImage(texture2D.imageView, Turbo::Core::TImageLayout::GENERAL);
    }
}

void Turbo::Render::TContext::BeginRenderPass(const Turbo::FrameGraph::TRenderPass &renderPass)
{
    // TODO: convert Turbo::FrameGraph::TRenderPass to Turbo::Render::TRenderPass
}

bool Turbo::Render::TContext::BeginRenderPass(Turbo::Render::TRenderPass &renderPass)
{
    // TODO: create Turbo::Core::TRenderPass and Turbo::Core::TFramebuffer
    if (this->renderPassPool->Allocate(renderPass))
    {
        if (renderPass.IsValid())
        {
            Turbo::Core::TFramebuffer *framebuffer = renderPass.framebuffer;
            Turbo::Core::TRenderPass *render_pass = renderPass.renderPass;
            this->currentCommandBuffer.commandBuffer->CmdBeginRenderPass(render_pass, framebuffer);
            this->currentRenderPass = renderPass;
            return true;
        }
    }

    return false;
}

void Turbo::Render::TContext::BindVeretxAttribute(const Turbo::Render::TVertexBuffer &vertexBuffer, Turbo::Render::TAttributeID attributeID, uint32_t location)
{
    Turbo::Render::TVertexBuffer::TAttribute attribute = vertexBuffer.GetAttribute(attributeID);
    if (attribute.IsValid())
    {
        if (vertexBuffer.IsValid())
        {
            bool is_found_vertex_buffer = false;
            uint32_t vertex_buffer_index = 0;
            for (Turbo::Core::TBuffer *vertex_buffer_item : this->vertexBuffers)
            {
                if (vertex_buffer_item == vertexBuffer.buffer)
                {
                    is_found_vertex_buffer = true;
                    break;
                }

                vertex_buffer_index = vertex_buffer_index + 1;
            }

            if (!is_found_vertex_buffer)
            {
                this->vertexBuffers.push_back(vertexBuffer.buffer);
                vertex_buffer_index = this->vertexBuffers.size() - 1;

                uint32_t stride = vertexBuffer.GetStride();
                Turbo::Core::TVertexRate vertex_rate = vertexBuffer.GetRate() == Turbo::Render::TVertexBuffer::TRate::VERTEX ? Turbo::Core::TVertexRate::VERTEX : Turbo::Core::TVertexRate::INSTANCE;

                Turbo::Core::TVertexBinding *vertex_binding = new Turbo::Core::TVertexBinding(vertex_buffer_index, stride, vertex_rate);
                this->vertexBindings.push_back(vertex_binding);
            }

            this->vertexBindings[vertex_buffer_index]->AddAttribute(location, (Turbo::Core::TFormatType)(attribute.GetFormat()), attribute.GetOffset());
        }
    }
}

void Turbo::Render::TContext::BindPipeline(const Turbo::Render::TComputePipeline &computePipeline)
{
    // TODO: create Turbo::Core::TComputePipeline if didn't create before
}

void Turbo::Render::TContext::BindPipeline(const Turbo::Render::TGraphicsPipeline &graphicsPipeline)
{
    this->currentGraphicsPipeline = graphicsPipeline;
}

void Turbo::Render::TContext::BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TTexture2D> &texture2Ds)
{
    auto set_map = this->descriptorMap.find(set);
    if (set_map != this->descriptorMap.end())
    {
        auto binding_map = set_map->second.find(binding);
        if (binding_map != set_map->second.end())
        {
            // TODO:更新Set和Binding对应的缓存
            // 说明之前已有描述符资源绑定到此，需要在之前的【描述符资源数组】中移除并更新到目标【描述符资源数组】中
            TDescriptorMapType descriptor_map_type = binding_map->second;
            switch (descriptor_map_type)
            {
            case TDescriptorMapType::UNDEFINED: {
            }
            break;
            case TDescriptorMapType::UNIFROM_BUFFER_MAP: {
                this->uniformBufferMap.at({set, binding}).clear();
            }
            break;
            case TDescriptorMapType::COMBINED_IMAGE_SAMPLER_MAP: {
                this->combinedImageSamplerMap.at({set, binding}).clear();
            }
            break;
            case TDescriptorMapType::SAMPLED_IMAGE_MAP: {
                this->sampledImageMap.at({set, binding}).clear();
            }
            break;
            case TDescriptorMapType::SAMPLER_MAP: {
                this->samplerMap.at({set, binding}).clear();
            }
            break;
            }

            // TODO:更新到目标【描述符资源数组】中
            std::vector<Turbo::Core::TImageView *> &sampled_images = this->sampledImageMap.at({set, binding});
            for (const Turbo::Render::TTexture2D &texture_2d_item : texture2Ds)
            {
                sampled_images.push_back(texture_2d_item.imageView);
            }

            // 更新绑定类型
            binding_map->second = TDescriptorMapType::SAMPLED_IMAGE_MAP;
        }
        else
        {
            // TODO:增加新的Binding（BindingMap增加新项目）。并将std::vector<各种uinform资源类型>存入相应缓存
            // 说明找到了Set，但没有Binding
            std::vector<Turbo::Core::TImageView *> &sampled_images = this->sampledImageMap[{set, binding}];
            for (const Turbo::Render::TTexture2D &texture_2d_item : texture2Ds)
            {
                sampled_images.push_back(texture_2d_item.imageView);
            }

            set_map->second[binding] = TDescriptorMapType::SAMPLED_IMAGE_MAP;
        }
    }
    else
    {
        // TODO:增加新的Set，Binding映射（SetMap中增加新项）。并将std::vector<各种uinform资源类型>存入相应缓存
        std::vector<Turbo::Core::TImageView *> &sampled_images = this->sampledImageMap[{set, binding}];
        for (const Turbo::Render::TTexture2D &texture_2d_item : texture2Ds)
        {
            sampled_images.push_back(texture_2d_item.imageView);
        }
        this->descriptorMap[set][binding] = TDescriptorMapType::SAMPLED_IMAGE_MAP;
    }
}

void Turbo::Render::TContext::BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TTexture2D &texture2D)
{
    std::vector<Turbo::Render::TTexture2D> texture2ds;
    texture2ds.push_back(texture2D);
    this->BindDescriptor(set, binding, texture2ds);
}

void Turbo::Render::TContext::BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TTexture3D> &texture3Ds)
{
    auto set_map = this->descriptorMap.find(set);
    if (set_map != this->descriptorMap.end())
    {
        auto binding_map = set_map->second.find(binding);
        if (binding_map != set_map->second.end())
        {
            // TODO:更新Set和Binding对应的缓存
            // 说明之前已有描述符资源绑定到此，需要在之前的【描述符资源数组】中移除并更新到目标【描述符资源数组】中
            TDescriptorMapType descriptor_map_type = binding_map->second;
            switch (descriptor_map_type)
            {
            case TDescriptorMapType::UNDEFINED: {
            }
            break;
            case TDescriptorMapType::UNIFROM_BUFFER_MAP: {
                this->uniformBufferMap.at({set, binding}).clear();
            }
            break;
            case TDescriptorMapType::COMBINED_IMAGE_SAMPLER_MAP: {
                this->combinedImageSamplerMap.at({set, binding}).clear();
            }
            break;
            case TDescriptorMapType::SAMPLED_IMAGE_MAP: {
                this->sampledImageMap.at({set, binding}).clear();
            }
            break;
            case TDescriptorMapType::SAMPLER_MAP: {
                this->samplerMap.at({set, binding}).clear();
            }
            break;
            }

            // TODO:更新到目标【描述符资源数组】中
            std::vector<Turbo::Core::TImageView *> &sampled_images = this->sampledImageMap.at({set, binding});
            for (const Turbo::Render::TTexture3D &texture_3d_item : texture3Ds)
            {
                sampled_images.push_back(texture_3d_item.imageView);
            }

            // 更新绑定类型
            binding_map->second = TDescriptorMapType::SAMPLED_IMAGE_MAP;
        }
        else
        {
            // TODO:增加新的Binding（BindingMap增加新项目）。并将std::vector<各种uinform资源类型>存入相应缓存
            // 说明找到了Set，但没有Binding
            std::vector<Turbo::Core::TImageView *> &sampled_images = this->sampledImageMap[{set, binding}];
            for (const Turbo::Render::TTexture3D &texture_3d_item : texture3Ds)
            {
                sampled_images.push_back(texture_3d_item.imageView);
            }

            set_map->second[binding] = TDescriptorMapType::SAMPLED_IMAGE_MAP;
        }
    }
    else
    {
        // TODO:增加新的Set，Binding映射（SetMap中增加新项）。并将std::vector<各种uinform资源类型>存入相应缓存
        std::vector<Turbo::Core::TImageView *> &sampled_images = this->sampledImageMap[{set, binding}];
        for (const Turbo::Render::TTexture3D &texture_3d_item : texture3Ds)
        {
            sampled_images.push_back(texture_3d_item.imageView);
        }
        this->descriptorMap[set][binding] = TDescriptorMapType::SAMPLED_IMAGE_MAP;
    }
}

void Turbo::Render::TContext::BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TTexture3D &texture3D)
{
    std::vector<Turbo::Render::TTexture3D> texture3ds;
    texture3ds.push_back(texture3D);
    this->BindDescriptor(set, binding, texture3ds);
}

void Turbo::Render::TContext::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    this->graphicsPipelinePool->Allocate(this->currentRenderPass, this->currentSubpass, this->currentGraphicsPipeline); // FIXME: <<<---此时发生了内存泄漏!!!
    this->currentCommandBuffer.commandBuffer->CmdBindPipeline(this->currentGraphicsPipeline.graphicsPipeline);

    //////////////////////this->currentCommandBuffer.commandBuffer->CmdBindPipelineDescriptorSet(pipeline_descriptor_set); //FIXME: 待实现
    this->currentCommandBuffer.commandBuffer->CmdBindVertexBuffers(this->vertexBuffers);

    Turbo::Core::TViewport viewport(0, 0, this->currentRenderPass.framebuffer->GetWidth(), this->currentRenderPass.framebuffer->GetHeight(), 0, 1);
    std::vector<Turbo::Core::TViewport> viewports;
    viewports.push_back(viewport);

    Turbo::Core::TScissor scissor(0, 0, this->currentRenderPass.framebuffer->GetWidth(), this->currentRenderPass.framebuffer->GetHeight());
    std::vector<Turbo::Core::TScissor> scissors;
    scissors.push_back(scissor);

    this->currentCommandBuffer.commandBuffer->CmdSetViewport(viewports);
    this->currentCommandBuffer.commandBuffer->CmdSetScissor(scissors);
    this->currentCommandBuffer.commandBuffer->CmdDraw(vertexCount, instanceCount, firstVertex, firstInstance);

    {
        for (Turbo::Core::TVertexBinding *vertex_binding_item : this->vertexBindings)
        {
            delete vertex_binding_item;
        }
    }

    this->vertexBindings.clear();
    this->vertexBuffers.clear();
}

void Turbo::Render::TContext::EndRenderPass()
{
    // TODO: CmdBindVertexBuffers(...)

    // FIXME: delete
    // FIXME: 在DrawCall绘制时记录顶点缓冲数组指令，之后销毁
    // FIXME: 销毁放在此处仅用于内存回收
    {
        for (Turbo::Core::TVertexBinding *vertex_binding_item : this->vertexBindings)
        {
            delete vertex_binding_item;
        }
    }

    this->vertexBindings.clear();
    this->vertexBuffers.clear();

    this->currentCommandBuffer.commandBuffer->CmdEndRenderPass();
}

void Turbo::Render::TContext::Flush()
{
    if (this->currentCommandBuffer.commandBuffer != nullptr && this->currentCommandBuffer.commandBuffer->GetVkCommandBuffer() != VK_NULL_HANDLE)
    {
        this->currentCommandBuffer.commandBuffer->End();

        this->graphicsQueue->Submit(nullptr, nullptr, this->currentCommandBuffer.commandBuffer, this->currentCommandBuffer.fence);
        this->commandBuffers.push_back(this->currentCommandBuffer);

        this->currentCommandBuffer.commandBuffer = this->commandBufferPool->Allocate();
        this->currentCommandBuffer.fence = new Turbo::Core::TFence(this->device);
        this->currentCommandBuffer.commandBuffer->Begin();
    }
}

bool Turbo::Render::TContext::Wait(uint64_t timeout)
{
    Turbo::Core::TFences fences;
    for (Turbo::Render::TCommandBuffer &command_buffer_item : this->commandBuffers)
    {
        fences.Add(command_buffer_item.fence);
    }

    Turbo::Core::TResult result = fences.Wait(timeout);
    if (result == Turbo::Core::TResult::TIMEOUT)
    {
        for (std::vector<Turbo::Render::TCommandBuffer>::iterator it = this->commandBuffers.begin(); it != this->commandBuffers.end();)
        {
            Turbo::Render::TCommandBuffer command_buffer = *it;
            result = command_buffer.fence->Wait(0);

            if (result == Turbo::Core::TResult::SUCCESS)
            {
                delete command_buffer.fence;
                this->commandBufferPool->Free(command_buffer.commandBuffer);
                it = this->commandBuffers.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return false;
    }

    for (Turbo::Render::TCommandBuffer &command_buffer_item : this->commandBuffers)
    {
        delete command_buffer_item.fence;
        this->commandBufferPool->Free(command_buffer_item.commandBuffer);
    }

    /*
        TODO:这里是回收资源的好去处(此时CommandBuffer已经执行完成，可以回收不再使用的资源了)
    */

    this->commandBuffers.clear();
    return true;
}

Turbo::Core::TInstance *Turbo::Render::TContext::GetInstance()
{
    return this->instance;
}

Turbo::Core::TPhysicalDevice *Turbo::Render::TContext::GetPhysicalDevice()
{
    return this->physicalDevice;
}

Turbo::Core::TDevice *Turbo::Render::TContext::GetDevice()
{
    return this->device;
}

Turbo::Core::TDeviceQueue *Turbo::Render::TContext::GetDeviceQueue()
{
    return this->graphicsQueue;
}

std::vector<Turbo::Core::TVertexBinding *> Turbo::Render::TContext::GetVertexBindings()
{
    return this->vertexBindings;
}

Turbo::Render::TCommandBuffer Turbo::Render::TContext::GetCommandBuffer()
{
    return this->currentCommandBuffer;
}

Turbo::Core::TImage *Turbo::Render::TContext::GetTextureImage(Turbo::Render::TTexture2D texture2d)
{
    if (texture2d.IsValid())
    {
        return texture2d.image;
    }

    return nullptr;
}

void Turbo::Render::TContext::GC()
{
    this->graphicsPipelinePool->GC();
    this->renderPassPool->GC();
}
