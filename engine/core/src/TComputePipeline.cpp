#include "TComputePipeline.h"
#include "TDevice.h"
#include "TPipelineLayout.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include "vulkan/vulkan_core.h"

void Turbo::Core::TComputePipeline::InternalCreate()
{
    auto shader_stage = this->GetShaderStages().at(0);
    auto compute_shader = shader_stage.GetShader();

    std::vector<VkSpecializationMapEntry> vk_specialization_map_entrys;
    size_t data_size = 0;
    void *specialization_constants_data = nullptr;
    {
        auto &stage_specializations_constants = shader_stage.GetSpecializationConstants();
        if (!stage_specializations_constants.Empty())
        {
            auto &shader_specialization_constants = compute_shader->GetSpecializationConstants();

            if (shader_specialization_constants.size() <= stage_specializations_constants.Size())
            {
                for (auto &item : shader_specialization_constants)
                {
                    auto id = item.first;
                    auto &shader_specialization_constant = item.second;

                    TShaderStage::TSpecializationConstants::TSpecializationConstant specialization_constant;
                    auto is_has_specialization_constant = stage_specializations_constants.Has(id, &specialization_constant);
                    if (is_has_specialization_constant)
                    {
                        VkSpecializationMapEntry vk_specialization_map_entry = {};
                        vk_specialization_map_entry.constantID = id;
                        vk_specialization_map_entry.offset = data_size;
                        vk_specialization_map_entry.size = shader_specialization_constant.GetWidth();
                        vk_specialization_map_entrys.push_back(vk_specialization_map_entry);
                        data_size += vk_specialization_map_entry.size;
                    }
                }
            }
            else
            {
                for (auto &item : stage_specializations_constants)
                {
                    auto id = item.first;
                    auto &stage_specialization_constant = item.second;

                    auto find_result = shader_specialization_constants.find(id);
                    if (find_result != shader_specialization_constants.end())
                    {
                        VkSpecializationMapEntry vk_specialization_map_entry = {};
                        vk_specialization_map_entry.constantID = id;
                        vk_specialization_map_entry.offset = data_size;
                        vk_specialization_map_entry.size = find_result->second.GetWidth();
                        vk_specialization_map_entrys.push_back(vk_specialization_map_entry);
                        data_size += vk_specialization_map_entry.size;
                    }
                }
            }

            specialization_constants_data = malloc(data_size);
            for (VkSpecializationMapEntry &vk_specialization_map_entry_item : vk_specialization_map_entrys)
            {
                uint32_t constant_id = vk_specialization_map_entry_item.constantID;
                uint32_t offset = vk_specialization_map_entry_item.offset;

                auto &specializations_constant = stage_specializations_constants.Get(constant_id);

                size_t size = std::min(vk_specialization_map_entry_item.size, std::size_t(specializations_constant.GetSize()));

                TDescriptorDataType data_type = specializations_constant.GetType();
                switch (data_type)
                {
                case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN: {
                    VkBool32 value = specializations_constant.GetBool() ? VK_TRUE : VK_FALSE;
                    memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, sizeof(VkBool32));
                }
                break;
                case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT: {
                    int value = specializations_constant.GetInt();
                    memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
                }
                break;
                case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT: {
                    uint32_t value = specializations_constant.GetUint32();
                    memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
                }
                break;
                case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT: {
                    float value = specializations_constant.GetFloat();
                    memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
                }
                break;
                case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE: {
                    double value = specializations_constant.GetDouble();
                    memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
                }
                break;
                default: {
                }
                }
            }
        }
    }

    VkSpecializationInfo vk_specialization_info = {};
    vk_specialization_info.mapEntryCount = vk_specialization_map_entrys.size();
    vk_specialization_info.pMapEntries = vk_specialization_map_entrys.data();
    vk_specialization_info.dataSize = data_size;
    vk_specialization_info.pData = specialization_constants_data;

    VkPipelineShaderStageCreateInfo vk_pipeline_shader_stage_create_info = {};
    vk_pipeline_shader_stage_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vk_pipeline_shader_stage_create_info.pNext = nullptr;
    vk_pipeline_shader_stage_create_info.flags = 0;
    vk_pipeline_shader_stage_create_info.stage = compute_shader->GetVkShaderStageFlagBits();
    vk_pipeline_shader_stage_create_info.module = compute_shader->GetVkShaderModule();
    vk_pipeline_shader_stage_create_info.pName = "main";
    vk_pipeline_shader_stage_create_info.pSpecializationInfo = &vk_specialization_info;

    VkComputePipelineCreateInfo vk_compute_pipeline_create_info = {};
    vk_compute_pipeline_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    vk_compute_pipeline_create_info.pNext = nullptr;
    vk_compute_pipeline_create_info.flags = 0;
    vk_compute_pipeline_create_info.stage = vk_pipeline_shader_stage_create_info;
    vk_compute_pipeline_create_info.layout = this->GetPipelineLayout()->GetVkPipelineLayout();
    vk_compute_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    vk_compute_pipeline_create_info.basePipelineIndex = -1;

    TDevice *device = compute_shader->GetDevice();
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

    free(specialization_constants_data);

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

// Turbo::Core::TComputePipeline::TComputePipeline(TComputeShader *computeShader) : Turbo::Core::TPipeline(computeShader->GetDevice(), computeShader)
//{
//     this->InternalCreate();
// }
//
// Turbo::Core::TComputePipeline::TComputePipeline(TPipelineCache *pipelineCache, TComputeShader *computeShader) : Turbo::Core::TPipeline(computeShader->GetDevice(), computeShader, pipelineCache)
//{
//     this->InternalCreate();
// }

Turbo::Core::TComputePipeline::TComputePipeline(const TPipelineLayout::TLayout &layout, TComputeShader *computeShader, TPipelineCache *pipelineCache) : Turbo::Core::TPipeline(computeShader->GetDevice(), layout, {computeShader}, pipelineCache)
{
    this->InternalCreate();
}

Turbo::Core::TComputePipeline::~TComputePipeline()
{
    this->InternalDestroy();
}

std::string Turbo::Core::TComputePipeline::ToString() const
{
    return std::string();
}