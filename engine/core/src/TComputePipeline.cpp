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

    const std::vector<TSpecializationConstant> &specialization_constants = computer_shader->GetSpecializationConstants();
    const std::map<uint32_t, TShader::TConstValue> &specializations = computer_shader->GetSpecializations();

    std::vector<VkSpecializationMapEntry> vk_specialization_map_entrys;
    size_t data_size = 0;
    for (auto &specialization_item : specializations)
    {
        uint32_t id = specialization_item.first;
        TShader::TConstValue value = specialization_item.second;

        for (const TSpecializationConstant &specialization_constant_item : specialization_constants)
        {
            uint32_t constant_id = specialization_constant_item.GetConstantID();
            if (constant_id == id)
            {
                TDescriptorDataType constant_type = specialization_constant_item.GetDescriptorDataType();
                uint32_t constant_width = specialization_constant_item.GetWidth() / 8;
                if (constant_type == TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN)
                {
                    constant_width = sizeof(VkBool32);
                }

                if (constant_type != TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN && constant_type == value.dataType && constant_width != 0)
                {
                    VkSpecializationMapEntry vk_specialization_map_entry = {};
                    vk_specialization_map_entry.constantID = id;
                    vk_specialization_map_entry.offset = data_size;
                    vk_specialization_map_entry.size = constant_width;
                    vk_specialization_map_entrys.push_back(vk_specialization_map_entry);
                    data_size = data_size + constant_width;
                }
                break;
            }
        }
    }

    void *specialization_constants_data = malloc(data_size);
    for (VkSpecializationMapEntry &vk_specialization_map_entry_item : vk_specialization_map_entrys)
    {
        uint32_t constant_id = vk_specialization_map_entry_item.constantID;
        uint32_t offset = vk_specialization_map_entry_item.offset;
        size_t size = vk_specialization_map_entry_item.size;

        TDescriptorDataType data_type = specializations.at(constant_id).dataType;
        TShader::TConstant data_value = specializations.at(constant_id).value;
        switch (data_type)
        {
        case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN: {
            VkBool32 value = data_value.boolValue ? VK_TRUE : VK_FALSE;
            memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
        }
        break;
        case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT: {
            int value = data_value.intValue;
            memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
        }
        break;
        case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT: {
            uint32_t value = data_value.uintValue;
            memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
        }
        break;
        case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT: {
            float value = data_value.floatValue;
            memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
        }
        break;
        case TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE: {
            double value = data_value.doubleValue;
            memcpy(static_cast<uint8_t *>(specialization_constants_data) + offset, &value, size);
        }
        break;
        default: {
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
    vk_pipeline_shader_stage_create_info.stage = computer_shader->GetVkShaderStageFlagBits();
    vk_pipeline_shader_stage_create_info.module = computer_shader->GetVkShaderModule();
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

Turbo::Core::TComputePipeline::TComputePipeline(const TRefPtr<TComputeShader> &computeShader) : Turbo::Core::TPipeline(computeShader->GetDevice(), computeShader)
{
    this->InternalCreate();
}

Turbo::Core::TComputePipeline::TComputePipeline(const TRefPtr<TPipelineCache> &pipelineCache, const TRefPtr<TComputeShader> &computeShader) : Turbo::Core::TPipeline(computeShader->GetDevice(), computeShader, pipelineCache)
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