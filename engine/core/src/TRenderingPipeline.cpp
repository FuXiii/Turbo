#include "TRenderingPipeline.h"
#include "TDevice.h"
#include "TException.h"
#include "TPipelineCache.h"
#include "TPipelineLayout.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include "vulkan/vulkan_core.h"

void Turbo::Core::TRenderingAttachments::AddColorAttachment(const TRefPtr<TImageView> &imageView, TImageLayout layout, const TRefPtr<TImageView> &resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp)
{
    TRenderingAttachment rendering_attachment = {};
    rendering_attachment.imageView = imageView;
    rendering_attachment.layout = layout;
    rendering_attachment.resolveImageView = resolveImageView;
    rendering_attachment.resolveLayout = resolveLayout;
    rendering_attachment.resolveModeBits = resolveModeBits;
    rendering_attachment.loadOp = loadOp;
    rendering_attachment.storeOp = storeOp;

    this->colorAttachments.push_back(rendering_attachment);
}

void Turbo::Core::TRenderingAttachments::AddColorAttachment(const TRefPtr<TImageView> &imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp)
{
    this->AddColorAttachment(imageView, layout, nullptr, TImageLayout::UNDEFINED, TResolveModeBits::NONE, loadOp, storeOp);
}

void Turbo::Core::TRenderingAttachments::SetDepthAttachment(const TRefPtr<TImageView> &imageView, TImageLayout layout, const TRefPtr<TImageView> &resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp)
{
    this->depthAttachment.imageView = imageView;
    this->depthAttachment.layout = layout;
    this->depthAttachment.resolveImageView = resolveImageView;
    this->depthAttachment.resolveLayout = resolveLayout;
    this->depthAttachment.resolveModeBits = resolveModeBits;
    this->depthAttachment.loadOp = loadOp;
    this->depthAttachment.storeOp = storeOp;
}

void Turbo::Core::TRenderingAttachments::SetDepthAttachment(const TRefPtr<TImageView> &imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp)
{
    this->SetDepthAttachment(imageView, layout, nullptr, TImageLayout::UNDEFINED, TResolveModeBits::NONE, loadOp, storeOp);
}

void Turbo::Core::TRenderingAttachments::SetStencilAttachment(const TRefPtr<TImageView> &imageView, TImageLayout layout, const TRefPtr<TImageView> &resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp)
{
    this->stencilAttachment.imageView = imageView;
    this->stencilAttachment.layout = layout;
    this->stencilAttachment.resolveImageView = resolveImageView;
    this->stencilAttachment.resolveLayout = resolveLayout;
    this->stencilAttachment.resolveModeBits = resolveModeBits;
    this->stencilAttachment.loadOp = loadOp;
    this->stencilAttachment.storeOp = storeOp;
}

void Turbo::Core::TRenderingAttachments::SetStencilAttachment(const TRefPtr<TImageView> &imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp)
{
    this->SetStencilAttachment(imageView, layout, nullptr, TImageLayout::UNDEFINED, TResolveModeBits::NONE, loadOp, storeOp);
}

void Turbo::Core::TAttachmentsFormat::AddColorAttachmentFormat(TFormatType formatType)
{
    this->colorAttachmentFormats.push_back(formatType);
}

void Turbo::Core::TAttachmentsFormat::SetDepthAttachmentFormat(TFormatType formatType)
{
    this->depthAttachmentFormat = formatType;
}

void Turbo::Core::TAttachmentsFormat::SetStencilAttachmentFormat(TFormatType formatType)
{
    this->stencilAttachmentFormat = formatType;
}

const std::vector<Turbo::Core::TFormatType> &Turbo::Core::TAttachmentsFormat::GetColorAttachmentFormats() const
{
    return this->colorAttachmentFormats;
}

const Turbo::Core::TFormatType &Turbo::Core::TAttachmentsFormat::GetDepthAttachmentFormat() const
{
    return this->depthAttachmentFormat;
}

const Turbo::Core::TFormatType &Turbo::Core::TAttachmentsFormat::GetStencilAttachmentFormat() const
{
    return this->stencilAttachmentFormat;
}

void Turbo::Core::TRenderingPipeline::InternalCreate()
{
    std::vector<VkPipelineShaderStageCreateInfo> vk_pipeline_shader_stage_create_infos;

    std::vector<TShader *> shaders = this->GetShaders();
    for (auto *shader_item : shaders)
    {
        VkPipelineShaderStageCreateInfo vk_pipeline_shader_stage_create_info = {};
        vk_pipeline_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vk_pipeline_shader_stage_create_info.pNext = nullptr;
        vk_pipeline_shader_stage_create_info.flags = 0;
        vk_pipeline_shader_stage_create_info.stage = shader_item->GetVkShaderStageFlagBits();
        vk_pipeline_shader_stage_create_info.module = shader_item->GetVkShaderModule();
        vk_pipeline_shader_stage_create_info.pName = "main";
        vk_pipeline_shader_stage_create_info.pSpecializationInfo = nullptr;

        vk_pipeline_shader_stage_create_infos.push_back(vk_pipeline_shader_stage_create_info);
    }

    std::vector<VkVertexInputBindingDescription> vk_vertex_input_binding_descriptions;
    std::vector<VkVertexInputAttributeDescription> vk_vertex_input_attribute_descriptions;

    for (TVertexBinding &vertex_binding_item : this->vertexBindings)
    {
        uint32_t binding = vertex_binding_item.GetBinding();

        std::vector<TVertexAttribute> vertex_attributes = vertex_binding_item.GetVertexAttributes();
        for (TVertexAttribute &vertex_attribute_item : vertex_attributes)
        {
            VkVertexInputAttributeDescription vk_vertex_input_attribute_description = {};
            vk_vertex_input_attribute_description.location = vertex_attribute_item.GetLocation();
            vk_vertex_input_attribute_description.binding = binding;
            vk_vertex_input_attribute_description.format = static_cast<VkFormat>(vertex_attribute_item.GetFormatType());
            vk_vertex_input_attribute_description.offset = vertex_attribute_item.GetOffset();

            vk_vertex_input_attribute_descriptions.push_back(vk_vertex_input_attribute_description);
        }
        VkVertexInputBindingDescription vk_vertex_input_binding_description = {};
        vk_vertex_input_binding_description.binding = binding;
        vk_vertex_input_binding_description.stride = vertex_binding_item.GetStride();
        vk_vertex_input_binding_description.inputRate = (VkVertexInputRate)vertex_binding_item.GetVertexRate();

        vk_vertex_input_binding_descriptions.push_back(vk_vertex_input_binding_description);
    }

    VkPipelineVertexInputStateCreateInfo vk_pipeline_vertex_input_state_create_info = {};
    vk_pipeline_vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vk_pipeline_vertex_input_state_create_info.pNext = nullptr;
    vk_pipeline_vertex_input_state_create_info.flags = 0;
    vk_pipeline_vertex_input_state_create_info.vertexBindingDescriptionCount = vk_vertex_input_binding_descriptions.size();
    vk_pipeline_vertex_input_state_create_info.pVertexBindingDescriptions = vk_vertex_input_binding_descriptions.data();
    vk_pipeline_vertex_input_state_create_info.vertexAttributeDescriptionCount = vk_vertex_input_attribute_descriptions.size();
    vk_pipeline_vertex_input_state_create_info.pVertexAttributeDescriptions = vk_vertex_input_attribute_descriptions.data();

    VkPipelineInputAssemblyStateCreateInfo vk_pipeline_input_assembly_state_create_info = {};
    vk_pipeline_input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vk_pipeline_input_assembly_state_create_info.pNext = nullptr;
    vk_pipeline_input_assembly_state_create_info.flags = 0;
    vk_pipeline_input_assembly_state_create_info.topology = (VkPrimitiveTopology)this->topology;
    vk_pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;
    if (this->primitiveRestartEnable)
    {
        vk_pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_TRUE;
    }

    std::vector<VkViewport> vk_viewports;
    VkViewport vk_viewport = {};
    vk_viewport.x = 0;
    vk_viewport.y = 0;
    vk_viewport.width = 1;
    vk_viewport.height = 1;
    vk_viewport.minDepth = 0;
    vk_viewport.maxDepth = 1;
    vk_viewports.push_back(vk_viewport);

    std::vector<VkRect2D> vk_scissors;
    VkRect2D vk_scissor = {};
    vk_scissor.offset.x = 0;
    vk_scissor.offset.y = 0;
    vk_scissor.extent.width = 1;
    vk_scissor.extent.height = 1;
    vk_scissors.push_back(vk_scissor);

    VkPipelineViewportStateCreateInfo vk_pipeline_viewport_state_create_info = {};
    vk_pipeline_viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vk_pipeline_viewport_state_create_info.pNext = nullptr;
    vk_pipeline_viewport_state_create_info.flags = 0;
    vk_pipeline_viewport_state_create_info.viewportCount = vk_viewports.size();
    vk_pipeline_viewport_state_create_info.pViewports = vk_viewports.data();
    vk_pipeline_viewport_state_create_info.scissorCount = vk_scissors.size();
    vk_pipeline_viewport_state_create_info.pScissors = vk_scissors.data();

    VkPipelineRasterizationStateCreateInfo vk_pipeline_rasteriaztion_state_create_info = {};
    vk_pipeline_rasteriaztion_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    vk_pipeline_rasteriaztion_state_create_info.pNext = nullptr;
    vk_pipeline_rasteriaztion_state_create_info.flags = 0;
    vk_pipeline_rasteriaztion_state_create_info.depthClampEnable = VK_FALSE;
    if (this->depthClampEnable)
    {
        vk_pipeline_rasteriaztion_state_create_info.depthClampEnable = VK_TRUE;
    }
    vk_pipeline_rasteriaztion_state_create_info.rasterizerDiscardEnable = VK_FALSE;
    if (this->rasterizerDiscardEnable)
    {
        vk_pipeline_rasteriaztion_state_create_info.rasterizerDiscardEnable = VK_TRUE;
    }
    vk_pipeline_rasteriaztion_state_create_info.polygonMode = (VkPolygonMode)this->polygonMode;
    vk_pipeline_rasteriaztion_state_create_info.cullMode = (VkCullModeFlags)this->cullMode;
    vk_pipeline_rasteriaztion_state_create_info.frontFace = (VkFrontFace)this->frontFace;
    vk_pipeline_rasteriaztion_state_create_info.depthBiasEnable = VK_FALSE;
    if (this->depthBiasEnable)
    {
        vk_pipeline_rasteriaztion_state_create_info.depthBiasEnable = VK_TRUE;
    }
    vk_pipeline_rasteriaztion_state_create_info.depthBiasConstantFactor = this->depthBiasConstantFactor;
    vk_pipeline_rasteriaztion_state_create_info.depthBiasClamp = this->depthBiasClamp;
    vk_pipeline_rasteriaztion_state_create_info.depthBiasSlopeFactor = this->depthBiasSlopeFactor;
    vk_pipeline_rasteriaztion_state_create_info.lineWidth = this->lineWidth;

    VkPipelineMultisampleStateCreateInfo vk_pipeline_multi_sample_state_create_info = {};
    vk_pipeline_multi_sample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    vk_pipeline_multi_sample_state_create_info.pNext = nullptr;
    vk_pipeline_multi_sample_state_create_info.flags = 0;
    vk_pipeline_multi_sample_state_create_info.sampleShadingEnable = VK_FALSE; // VK_FALSE;
    if (this->multisampleEnable)
    {
        vk_pipeline_multi_sample_state_create_info.sampleShadingEnable = VK_TRUE;
    }
    vk_pipeline_multi_sample_state_create_info.rasterizationSamples = (VkSampleCountFlagBits)this->sample;
    vk_pipeline_multi_sample_state_create_info.minSampleShading = 1.0f; //[0,1]
    vk_pipeline_multi_sample_state_create_info.pSampleMask = nullptr;
    vk_pipeline_multi_sample_state_create_info.alphaToCoverageEnable = VK_FALSE;
    vk_pipeline_multi_sample_state_create_info.alphaToOneEnable = VK_FALSE;

    VkPipelineDepthStencilStateCreateInfo vk_pipeline_depth_stencil_state_create_info;
    vk_pipeline_depth_stencil_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    vk_pipeline_depth_stencil_state_create_info.pNext = nullptr;
    vk_pipeline_depth_stencil_state_create_info.flags = 0;
    vk_pipeline_depth_stencil_state_create_info.depthTestEnable = VK_FALSE;
    if (this->depthTestEnable)
    {
        vk_pipeline_depth_stencil_state_create_info.depthTestEnable = VK_TRUE;
    }
    vk_pipeline_depth_stencil_state_create_info.depthWriteEnable = VK_FALSE;
    if (this->depthWriteEnable)
    {
        vk_pipeline_depth_stencil_state_create_info.depthWriteEnable = VK_TRUE;
    }
    vk_pipeline_depth_stencil_state_create_info.depthCompareOp = (VkCompareOp)this->depthCompareOp;
    vk_pipeline_depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
    if (this->depthBoundsTestEnable)
    {
        vk_pipeline_depth_stencil_state_create_info.depthBoundsTestEnable = VK_TRUE;
    }
    vk_pipeline_depth_stencil_state_create_info.stencilTestEnable = VK_FALSE;
    if (this->stencilTestEnable)
    {
        vk_pipeline_depth_stencil_state_create_info.stencilTestEnable = VK_TRUE;
    }
    vk_pipeline_depth_stencil_state_create_info.front.failOp = (VkStencilOp)this->frontFailOp;
    vk_pipeline_depth_stencil_state_create_info.front.passOp = (VkStencilOp)this->frontPassOp;
    vk_pipeline_depth_stencil_state_create_info.front.depthFailOp = (VkStencilOp)this->frontDepthFailOp;
    vk_pipeline_depth_stencil_state_create_info.front.compareOp = (VkCompareOp)this->frontCompareOp;
    vk_pipeline_depth_stencil_state_create_info.front.compareMask = this->frontCompareMask;
    vk_pipeline_depth_stencil_state_create_info.front.writeMask = this->frontWriteMask;
    vk_pipeline_depth_stencil_state_create_info.front.reference = this->frontReference;
    vk_pipeline_depth_stencil_state_create_info.back.failOp = (VkStencilOp)this->backFailOp;
    vk_pipeline_depth_stencil_state_create_info.back.passOp = (VkStencilOp)this->backPassOp;
    vk_pipeline_depth_stencil_state_create_info.back.depthFailOp = (VkStencilOp)this->backDepthFailOp;
    vk_pipeline_depth_stencil_state_create_info.back.compareOp = (VkCompareOp)this->backCompareOp;
    vk_pipeline_depth_stencil_state_create_info.back.compareMask = this->backCompareMask;
    vk_pipeline_depth_stencil_state_create_info.back.writeMask = this->backWriteMask;
    vk_pipeline_depth_stencil_state_create_info.back.reference = this->backReference;
    vk_pipeline_depth_stencil_state_create_info.minDepthBounds = this->minDepthBounds;
    vk_pipeline_depth_stencil_state_create_info.maxDepthBounds = this->maxDepthBounds;

    VkPipelineColorBlendAttachmentState vk_pipline_color_blend_attachment_state = {};
    vk_pipline_color_blend_attachment_state.blendEnable = VK_FALSE;
    if (this->blendEnable)
    {
        vk_pipline_color_blend_attachment_state.blendEnable = VK_TRUE;
    }
    vk_pipline_color_blend_attachment_state.srcColorBlendFactor = (VkBlendFactor)this->srcColorBlendFactor;
    vk_pipline_color_blend_attachment_state.dstColorBlendFactor = (VkBlendFactor)this->dstColorBlendFactor;
    vk_pipline_color_blend_attachment_state.colorBlendOp = (VkBlendOp)this->colorBlendOp;
    vk_pipline_color_blend_attachment_state.srcAlphaBlendFactor = (VkBlendFactor)this->srcAlphaBlendFactor;
    vk_pipline_color_blend_attachment_state.dstAlphaBlendFactor = (VkBlendFactor)this->dstAlphaBlendFactor;
    vk_pipline_color_blend_attachment_state.alphaBlendOp = (VkBlendOp)this->alphaBlendOp;
    vk_pipline_color_blend_attachment_state.colorWriteMask = VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;

    // In Vulkan1.3 Specification:
    // If renderPass is VK_NULL_HANDLE and the pipeline is being created with fragment output
    // interface state, pColorBlendState->attachmentCount must be equal to
    // VkPipelineRenderingCreateInfo::colorAttachmentCount

    std::vector<Turbo::Core::TFormatType> color_attachment_formats = this->renderingAttachments.GetColorAttachmentFormats();

    uint32_t color_blend_attachment_count = color_attachment_formats.size();
    std::vector<VkPipelineColorBlendAttachmentState> vk_pipeline_color_blend_attachment_states;
    for (uint32_t color_belnd_attachment_index = 0; color_belnd_attachment_index < color_blend_attachment_count; color_belnd_attachment_index++)
    {
        vk_pipeline_color_blend_attachment_states.push_back(vk_pipline_color_blend_attachment_state);
    }

    VkPipelineColorBlendStateCreateInfo vk_pipeline_color_blend_state_create_info;
    vk_pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    vk_pipeline_color_blend_state_create_info.pNext = nullptr;
    vk_pipeline_color_blend_state_create_info.flags = 0;
    vk_pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
    if (this->logicOpEnable)
    {
        vk_pipeline_color_blend_state_create_info.logicOpEnable = VK_TRUE;
    }
    vk_pipeline_color_blend_state_create_info.logicOp = (VkLogicOp)this->logicOp;
    vk_pipeline_color_blend_state_create_info.attachmentCount = vk_pipeline_color_blend_attachment_states.size();
    vk_pipeline_color_blend_state_create_info.pAttachments = vk_pipeline_color_blend_attachment_states.data();
    vk_pipeline_color_blend_state_create_info.blendConstants[0] = this->constantR;
    vk_pipeline_color_blend_state_create_info.blendConstants[1] = this->constantG;
    vk_pipeline_color_blend_state_create_info.blendConstants[2] = this->constantB;
    vk_pipeline_color_blend_state_create_info.blendConstants[3] = this->constantA;

    std::vector<VkDynamicState> vk_dynamic_states;
    vk_dynamic_states.push_back(VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT);
    vk_dynamic_states.push_back(VkDynamicState::VK_DYNAMIC_STATE_SCISSOR);
    vk_dynamic_states.push_back(VkDynamicState::VK_DYNAMIC_STATE_LINE_WIDTH);

    VkPipelineDynamicStateCreateInfo vk_pipeline_dynamic_state_create_info;
    vk_pipeline_dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    vk_pipeline_dynamic_state_create_info.pNext = nullptr;
    vk_pipeline_dynamic_state_create_info.flags = 0;
    vk_pipeline_dynamic_state_create_info.dynamicStateCount = vk_dynamic_states.size();
    vk_pipeline_dynamic_state_create_info.pDynamicStates = vk_dynamic_states.data();

    VkPipelineRenderingCreateInfo vk_pipeline_rendering_create_info = {};
    vk_pipeline_rendering_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    vk_pipeline_rendering_create_info.pNext = nullptr;
    vk_pipeline_rendering_create_info.viewMask = 0;
    vk_pipeline_rendering_create_info.colorAttachmentCount = color_attachment_formats.size();
    vk_pipeline_rendering_create_info.pColorAttachmentFormats = (const VkFormat *)color_attachment_formats.data();
    vk_pipeline_rendering_create_info.depthAttachmentFormat = (VkFormat)this->renderingAttachments.GetDepthAttachmentFormat();
    vk_pipeline_rendering_create_info.stencilAttachmentFormat = (VkFormat)this->renderingAttachments.GetStencilAttachmentFormat();

    VkGraphicsPipelineCreateInfo vk_graphics_pipeline_create_info = {};
    vk_graphics_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    vk_graphics_pipeline_create_info.pNext = &vk_pipeline_rendering_create_info;
    vk_graphics_pipeline_create_info.flags = 0;
    vk_graphics_pipeline_create_info.stageCount = vk_pipeline_shader_stage_create_infos.size();
    vk_graphics_pipeline_create_info.pStages = vk_pipeline_shader_stage_create_infos.data();
    vk_graphics_pipeline_create_info.pVertexInputState = &vk_pipeline_vertex_input_state_create_info;
    vk_graphics_pipeline_create_info.pInputAssemblyState = &vk_pipeline_input_assembly_state_create_info;
    vk_graphics_pipeline_create_info.pTessellationState = nullptr;
    vk_graphics_pipeline_create_info.pViewportState = &vk_pipeline_viewport_state_create_info;
    vk_graphics_pipeline_create_info.pRasterizationState = &vk_pipeline_rasteriaztion_state_create_info;
    vk_graphics_pipeline_create_info.pMultisampleState = &vk_pipeline_multi_sample_state_create_info;
    vk_graphics_pipeline_create_info.pDepthStencilState = &vk_pipeline_depth_stencil_state_create_info;
    vk_graphics_pipeline_create_info.pColorBlendState = &vk_pipeline_color_blend_state_create_info;
    vk_graphics_pipeline_create_info.pDynamicState = &vk_pipeline_dynamic_state_create_info;
    vk_graphics_pipeline_create_info.layout = this->GetPipelineLayout()->GetVkPipelineLayout();
    vk_graphics_pipeline_create_info.renderPass = VK_NULL_HANDLE;
    vk_graphics_pipeline_create_info.subpass = 0;
    vk_graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    vk_graphics_pipeline_create_info.basePipelineIndex = -1;

    TDevice *device = this->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    TPipelineCache *pipeline_cache = this->GetPipelineCache();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = VkResult::VK_ERROR_UNKNOWN;
    if (pipeline_cache != nullptr && pipeline_cache->GetVkPipelineCache() != VK_NULL_HANDLE)
    {
        result = device->GetDeviceDriver()->vkCreateGraphicsPipelines(vk_device, pipeline_cache->GetVkPipelineCache(), 1, &vk_graphics_pipeline_create_info, allocator, &this->vkPipeline);
    }
    else
    {
        result = device->GetDeviceDriver()->vkCreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, 1, &vk_graphics_pipeline_create_info, allocator, &this->vkPipeline);
    }

    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TGraphicsPipeline::InternalCreate::vkCreateGraphicsPipelines");
    }
}

void Turbo::Core::TRenderingPipeline::InternalDestroy()
{
    TDevice *device = this->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    device->GetDeviceDriver()->vkDestroyPipeline(vk_device, this->vkPipeline, allocator);
}

Turbo::Core::TRenderingPipeline::TRenderingPipeline(const TAttachmentsFormat &renderingAttachments, std::vector<TVertexBinding> &vertexBindings, const TRefPtr<TVertexShader> &vertexShader, const TRefPtr<TFragmentShader> &fragmentShader, TTopologyType topology, bool primitiveRestartEnable, bool depthClampEnable, bool rasterizerDiscardEnable, TPolygonMode polygonMode, TCullModes cullMode, TFrontFace frontFace, bool depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, bool multisampleEnable, TSampleCountBits sample, bool depthTestEnable, bool depthWriteEnable, TCompareOp depthCompareOp, bool depthBoundsTestEnable, bool stencilTestEnable, TStencilOp frontFailOp, TStencilOp frontPassOp, TStencilOp frontDepthFailOp, TCompareOp frontCompareOp, uint32_t frontCompareMask, uint32_t frontWriteMask, uint32_t frontReference, TStencilOp backFailOp, TStencilOp backPassOp, TStencilOp backDepthFailOp, TCompareOp backCompareOp, uint32_t backCompareMask, uint32_t backWriteMask, uint32_t backReference, float minDepthBounds, float maxDepthBounds, bool logicOpEnable, TLogicOp logicOp, bool blendEnable, TBlendFactor srcColorBlendFactor, TBlendFactor dstColorBlendFactor, TBlendOp colorBlendOp, TBlendFactor srcAlphaBlendFactor, TBlendFactor dstAlphaBlendFactor, TBlendOp alphaBlendOp, float constantR, float constantG, float constantB, float constantA) : Turbo::Core::TPipeline(vertexShader->GetDevice(), vertexShader, fragmentShader)
{
    Turbo::Core::TPhysicalDeviceFeatures physical_device_feature = vertexShader->GetDevice()->GetEnableDeviceFeatures();
    if (physical_device_feature.dynamicRendering)
    {
        // VkPipelineRenderingCreateInfo
        this->renderingAttachments = renderingAttachments;

        // VkPipelineVertexInputStateCreateInfo
        this->vertexBindings = vertexBindings;

        // VkPipelineInputAssemblyStateCreateInfo
        this->topology = topology;
        this->primitiveRestartEnable = primitiveRestartEnable;

        // VkPipelineTessellationStateCreateInfo
        // 目前为 nullptr

        // VkPipelineViewportStateCreateInfo
        // dynamic

        // VkPipelineRasterizationStateCreateInfo
        this->depthClampEnable = depthClampEnable;
        this->rasterizerDiscardEnable = rasterizerDiscardEnable;
        this->polygonMode = polygonMode;
        this->cullMode = cullMode;
        this->frontFace = frontFace;
        this->depthBiasEnable = depthBiasEnable;
        this->depthBiasConstantFactor = depthBiasConstantFactor;
        this->depthBiasClamp = depthBiasClamp;
        this->depthBiasSlopeFactor = depthBiasSlopeFactor;
        this->lineWidth = lineWidth;

        // VkPipelineMultisampleStateCreateInfo
        this->multisampleEnable = multisampleEnable;
        this->sample = sample;

        // VkPipelineDepthStencilStateCreateInfo
        this->depthTestEnable = depthTestEnable;
        this->depthWriteEnable = depthWriteEnable;
        this->depthCompareOp = depthCompareOp;
        this->depthBoundsTestEnable = depthBoundsTestEnable;
        this->stencilTestEnable = stencilTestEnable;
        this->frontFailOp = frontFailOp;
        this->frontPassOp = frontPassOp;
        this->frontDepthFailOp = frontDepthFailOp;
        this->frontCompareOp = frontCompareOp;
        this->frontCompareMask = frontCompareMask;
        this->frontWriteMask = frontWriteMask;
        this->frontReference = frontReference;
        this->backFailOp = backFailOp;
        this->backPassOp = backPassOp;
        this->backDepthFailOp = backDepthFailOp;
        this->backCompareOp = backCompareOp;
        this->backCompareMask = backCompareMask;
        this->backWriteMask = backWriteMask;
        this->backReference = backReference;
        this->minDepthBounds = minDepthBounds;
        this->maxDepthBounds = maxDepthBounds;

        // VkPipelineColorBlendStateCreateInfo
        this->logicOpEnable = logicOpEnable;
        this->logicOp = logicOp;
        this->blendEnable = blendEnable;
        this->srcColorBlendFactor = srcColorBlendFactor;
        this->dstColorBlendFactor = dstColorBlendFactor;
        this->colorBlendOp = colorBlendOp;
        this->srcAlphaBlendFactor = srcAlphaBlendFactor;
        this->dstAlphaBlendFactor = dstAlphaBlendFactor;
        this->alphaBlendOp = alphaBlendOp;
        this->constantR = constantR;
        this->constantG = constantG;
        this->constantB = constantB;
        this->constantA = constantA;

        // VkPipelineDynamicStateCreateInfo
        //  目前为 viewport scissor linewidth

        // VkPipelineLayout VkGraphicsPipelineCreateInfo::layout
        // 使用传进来的Shader来创建

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::UNSUPPORTED, "The Dynamic Rendering did not enabled", "Please check the Dynamic Rendering feature");
    }
}

Turbo::Core::TRenderingPipeline::~TRenderingPipeline()
{
    this->InternalDestroy();
}
