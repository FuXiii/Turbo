#include "TGraphicsPipeline.h"
#include "TDevice.h"
#include "TException.h"
#include "TRenderPass.h"
#include "TShader.h"
#include "TVulkanAllocator.h"

Turbo::Core::TVertexAttribute::TVertexAttribute(uint32_t location, TFormatInfo format, uint32_t offset) : Turbo::Core::TInfo()
{
    this->location = location;
    this->format = format;
    this->offset = offset;
}

Turbo::Core::TVertexAttribute::~TVertexAttribute()
{
}

uint32_t Turbo::Core::TVertexAttribute::GetLocation()
{
    return this->location;
}

Turbo::Core::TFormatInfo Turbo::Core::TVertexAttribute::GetFormat()
{
    return this->format;
}

uint32_t Turbo::Core::TVertexAttribute::GetOffset()
{
    return this->offset;
}

std::string Turbo::Core::TVertexAttribute::ToString()
{
    return std::string();
}

Turbo::Core::TVertexBinding::TVertexBinding(uint32_t binding, uint32_t stride, TVertexRate rate) : Turbo::Core::TInfo()
{
    this->binding = binding;
    this->stride = stride;
    this->rate = rate;
}

Turbo::Core::TVertexBinding::~TVertexBinding()
{
}

void Turbo::Core::TVertexBinding::AddAttribute(uint32_t location, TFormatInfo format, uint32_t offset)
{
    TVertexAttribute vertex_attribute(location, format, offset);
    this->AddAttribute(vertex_attribute);
}

void Turbo::Core::TVertexBinding::AddAttribute(TVertexAttribute &vertexAttribute)
{
    this->vertexAttributes.push_back(vertexAttribute);
}

uint32_t Turbo::Core::TVertexBinding::GetBinding()
{
    return this->binding;
}

uint32_t Turbo::Core::TVertexBinding::GetStride()
{
    return this->stride;
}

Turbo::Core::TVertexRate Turbo::Core::TVertexBinding::GetVertexRate()
{
    return this->rate;
}

const std::vector<Turbo::Core::TVertexAttribute> &Turbo::Core::TVertexBinding::GetVertexAttributes()
{
    return this->vertexAttributes;
}

std::string Turbo::Core::TVertexBinding::ToString()
{
    return std::string();
}

void Turbo::Core::TGraphicsPipeline::InternalCreate()
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
            vk_vertex_input_attribute_description.format = vertex_attribute_item.GetFormat().GetVkFormat();
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
    for (TViewport &viewport_item : this->viewports)
    {
        VkViewport vk_viewport = {};
        vk_viewport.x = viewport_item.GetX();
        vk_viewport.y = viewport_item.GetY();
        vk_viewport.width = viewport_item.GetWidth();
        vk_viewport.height = viewport_item.GetHeight();
        vk_viewport.minDepth = viewport_item.GetMinDepth();
        vk_viewport.maxDepth = viewport_item.GetMaxDepth();

        vk_viewports.push_back(vk_viewport);
    }

    std::vector<VkRect2D> vk_scissors;
    for (TScissor &scissors_item : this->scissors)
    {
        VkRect2D vk_scissor = {};
        vk_scissor.offset.x = scissors_item.GetOffsetX();
        vk_scissor.offset.y = scissors_item.GetOffsetY();
        vk_scissor.extent.width = scissors_item.GetWidth();
        vk_scissor.extent.height = scissors_item.GetHeight();
        vk_scissors.push_back(vk_scissor);
    }

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
    vk_pipeline_depth_stencil_state_create_info.depthTestEnable = VK_TRUE;
    vk_pipeline_depth_stencil_state_create_info.depthWriteEnable = VK_TRUE;
    vk_pipeline_depth_stencil_state_create_info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    vk_pipeline_depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
    vk_pipeline_depth_stencil_state_create_info.stencilTestEnable = VK_FALSE;
    vk_pipeline_depth_stencil_state_create_info.front.failOp = VK_STENCIL_OP_KEEP;
    vk_pipeline_depth_stencil_state_create_info.front.passOp = VK_STENCIL_OP_KEEP;
    vk_pipeline_depth_stencil_state_create_info.front.depthFailOp = VK_STENCIL_OP_KEEP;
    vk_pipeline_depth_stencil_state_create_info.front.compareOp = VK_COMPARE_OP_ALWAYS;
    vk_pipeline_depth_stencil_state_create_info.front.compareMask = 0;
    vk_pipeline_depth_stencil_state_create_info.front.writeMask = 0;
    vk_pipeline_depth_stencil_state_create_info.front.reference = 0;
    vk_pipeline_depth_stencil_state_create_info.back = vk_pipeline_depth_stencil_state_create_info.front;
    vk_pipeline_depth_stencil_state_create_info.minDepthBounds = 0.0f;
    vk_pipeline_depth_stencil_state_create_info.maxDepthBounds = 0.0f;

    VkPipelineColorBlendAttachmentState vk_pipline_color_blend_attachment_state = {};
    vk_pipline_color_blend_attachment_state.blendEnable = VK_FALSE;
    vk_pipline_color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    vk_pipline_color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    vk_pipline_color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
    vk_pipline_color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    vk_pipline_color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    vk_pipline_color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
    vk_pipline_color_blend_attachment_state.colorWriteMask = VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT | VkColorComponentFlagBits::VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo vk_pipeline_color_blend_state_create_info;
    vk_pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    vk_pipeline_color_blend_state_create_info.pNext = nullptr;
    vk_pipeline_color_blend_state_create_info.flags = 0;
    vk_pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
    vk_pipeline_color_blend_state_create_info.logicOp = VK_LOGIC_OP_NO_OP;
    vk_pipeline_color_blend_state_create_info.attachmentCount = 1;
    vk_pipeline_color_blend_state_create_info.pAttachments = &vk_pipline_color_blend_attachment_state;
    vk_pipeline_color_blend_state_create_info.blendConstants[0] = 1.0f;
    vk_pipeline_color_blend_state_create_info.blendConstants[1] = 1.0f;
    vk_pipeline_color_blend_state_create_info.blendConstants[2] = 1.0f;
    vk_pipeline_color_blend_state_create_info.blendConstants[3] = 1.0f;

    std::vector<VkDynamicState> vk_dynamic_states{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo vk_pipeline_dynamic_state_create_info;
    vk_pipeline_dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    vk_pipeline_dynamic_state_create_info.pNext = nullptr;
    vk_pipeline_dynamic_state_create_info.flags = 0;
    vk_pipeline_dynamic_state_create_info.dynamicStateCount = 2;
    vk_pipeline_dynamic_state_create_info.pDynamicStates = vk_dynamic_states.data();

    VkRenderPass vk_render_pass = this->renderPass->GetVkRenderPass();

    VkGraphicsPipelineCreateInfo vk_graphics_pipeline_create_info = {};
    vk_graphics_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    vk_graphics_pipeline_create_info.pNext = nullptr;
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
    vk_graphics_pipeline_create_info.layout = this->GetVkPipelineLayout();
    vk_graphics_pipeline_create_info.renderPass = vk_render_pass;
    vk_graphics_pipeline_create_info.subpass = this->subpass;
    vk_graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    vk_graphics_pipeline_create_info.basePipelineIndex = 0;

    VkDevice vk_device = this->renderPass->GetDevice()->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateGraphicsPipelines(vk_device, VK_NULL_HANDLE, 1, &vk_graphics_pipeline_create_info, allocator, &this->vkPipeline);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TGraphicsPipeline::InternalCreate::vkCreateGraphicsPipelines");
    }
}

void Turbo::Core::TGraphicsPipeline::InternalDestroy()
{
    VkDevice vk_device = this->renderPass->GetDevice()->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

    vkDestroyPipeline(vk_device, this->vkPipeline, allocator);
}

Turbo::Core::TGraphicsPipeline::TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, TTopologyType topology, bool primitiveRestartEnable, std::vector<TVertexBinding> &vertexBindings, std::vector<TViewport> &viewports, std::vector<TScissor> &scissors, bool depthClampEnable, bool rasterizerDiscardEnable, TPolygonMode polygonMode, TCullModes cullMode, TFrontFace frontFace, bool depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, bool multisampleEnable, TSampleCountBits sample, std::vector<TShader *> &shaders) : Turbo::Core::TPipeline(renderPass->GetDevice(), TPipelineType::Graphics, shaders)
{
    if (renderPass != nullptr)
    {
        // VkRenderPass VkGraphicsPipelineCreateInfo::renderPass
        this->renderPass = renderPass;

        // uint32_t VkGraphicsPipelineCreateInfo::subpass
        this->subpass = subpass;

        // VkPipelineVertexInputStateCreateInfo
        this->vertexBindings = vertexBindings;

        // VkPipelineInputAssemblyStateCreateInfo
        this->topology = topology;
        this->primitiveRestartEnable = primitiveRestartEnable;

        // VkPipelineTessellationStateCreateInfo
        // 目前为 nullptr

        // VkPipelineViewportStateCreateInfo
        this->viewports = viewports;
        this->scissors = scissors;

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
        // 目前为 VK_COMPARE_OP_ALWAYS

        // VkPipelineColorBlendStateCreateInfo
        //  目前为 关闭

        // VkPipelineDynamicStateCreateInfo
        //  目前为 空

        // VkPipelineLayout VkGraphicsPipelineCreateInfo::layout
        //使用传进来的Shader来创建

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TGraphicsPipeline::TGraphicsPipeline");
    }
}

Turbo::Core::TGraphicsPipeline::~TGraphicsPipeline()
{
    this->InternalDestroy();
}

Turbo::Core::TTopologyType Turbo::Core::TGraphicsPipeline::GetTopologyType()
{
    return this->topology;
}

bool Turbo::Core::TGraphicsPipeline::GetPrimitiveRestartEnable()
{
    return this->primitiveRestartEnable;
}

const std::vector<Turbo::Core::TVertexBinding> &Turbo::Core::TGraphicsPipeline::GetVertexBindings()
{
    return this->vertexBindings;
}

std::vector<Turbo::Core::TViewport> Turbo::Core::TGraphicsPipeline::GetViewports()
{
    return this->viewports;
}

std::vector<Turbo::Core::TScissor> Turbo::Core::TGraphicsPipeline::GetScissors()
{
    return this->scissors;
}

bool Turbo::Core::TGraphicsPipeline::GetDepthClampEnable()
{
    return this->depthClampEnable;
}

bool Turbo::Core::TGraphicsPipeline::GetRasterizerDiscardEnable()
{
    return this->rasterizerDiscardEnable;
}

Turbo::Core::TPolygonMode Turbo::Core::TGraphicsPipeline::GetPolygonMode()
{
    return this->polygonMode;
}

Turbo::Core::TCullModes Turbo::Core::TGraphicsPipeline::GetCullMode()
{
    return this->cullMode;
}

Turbo::Core::TFrontFace Turbo::Core::TGraphicsPipeline::GetFrontFace()
{
    return this->frontFace;
}

bool Turbo::Core::TGraphicsPipeline::GetDepthBiasEnable()
{
    return this->depthBiasEnable;
}

float Turbo::Core::TGraphicsPipeline::GetDepthBiasConstantFactor()
{
    return this->depthBiasConstantFactor;
}

float Turbo::Core::TGraphicsPipeline::GetDepthBiasClamp()
{
    return this->depthBiasClamp;
}

float Turbo::Core::TGraphicsPipeline::GetDepthBiasSlopeFactor()
{
    return this->depthBiasSlopeFactor;
}

float Turbo::Core::TGraphicsPipeline::GetLineWidth()
{
    return this->lineWidth;
}

std::string Turbo::Core::TGraphicsPipeline::ToString()
{
    return std::string();
}
