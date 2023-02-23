#include "TComputePipeline.h"
#include "TDevice.h"
#include "TPipelineLayout.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include "vulkan/vulkan_core.h"

void Turbo::Core::TComputePipeline::InternalCreate()
{
    TShader *computer_shader = this->GetShaders()[0];

    VkPipelineShaderStageCreateInfo vk_pipeline_shader_stage_create_info = {};
    vk_pipeline_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vk_pipeline_shader_stage_create_info.pNext = nullptr;
    vk_pipeline_shader_stage_create_info.flags = 0;
    vk_pipeline_shader_stage_create_info.stage = computer_shader->GetVkShaderStageFlagBits();
    vk_pipeline_shader_stage_create_info.module = computer_shader->GetVkShaderModule();
    vk_pipeline_shader_stage_create_info.pName = "main";
    vk_pipeline_shader_stage_create_info.pSpecializationInfo = nullptr;

    VkComputePipelineCreateInfo vk_compute_pipeline_create_info = {};
    vk_compute_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    vk_compute_pipeline_create_info.pNext = nullptr;
    vk_compute_pipeline_create_info.flags = 0;
    vk_compute_pipeline_create_info.stage = vk_pipeline_shader_stage_create_info;
    vk_compute_pipeline_create_info.layout = this->GetPipelineLayout()->GetVkPipelineLayout();
    vk_compute_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    vk_compute_pipeline_create_info.basePipelineIndex = -1;

    TDevice *device = computer_shader->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    TPipelineCache *pipeline_cache = this->GetPipelineCache();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = VkResult::VK_ERROR_UNKNOWN;
    if (pipeline_cache != nullptr && pipeline_cache->GetVkPipelineCache() != VK_NULL_HANDLE)
    {
        result = device->GetDeviceDriver()->vkCreateComputePipelines(vk_device, pipeline_cache->GetVkPipelineCache(), 1, &vk_compute_pipeline_create_info, allocator, &this->vkPipeline);
    }
    else
    {
        result = device->GetDeviceDriver()->vkCreateComputePipelines(vk_device, VK_NULL_HANDLE, 1, &vk_compute_pipeline_create_info, allocator, &this->vkPipeline);
    }
    
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TComputePipeline::InternalCreate::vkCreateGraphicsPipelines");
    }
}

void Turbo::Core::TComputePipeline::InternalDestroy()
{
    TShader *computer_shader = this->GetShaders()[0];
    TDevice *device = computer_shader->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    device->GetDeviceDriver()->vkDestroyPipeline(vk_device, this->vkPipeline, allocator);
}

Turbo::Core::TComputePipeline::TComputePipeline(TComputeShader *computeShader) : Turbo::Core::TPipeline(computeShader->GetDevice(), computeShader)
{
    this->InternalCreate();
}

Turbo::Core::TComputePipeline::TComputePipeline(TPipelineCache *pipelineCache, TComputeShader *computeShader) : Turbo::Core::TPipeline(computeShader->GetDevice(), computeShader, pipelineCache)
{
}

Turbo::Core::TComputePipeline::~TComputePipeline()
{
    this->InternalDestroy();
}

std::string Turbo::Core::TComputePipeline::ToString()
{
    return std::string();
}
