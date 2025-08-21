#include "TPipeline.h"
#include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TPipelineLayout.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include <map>

bool DescriptorSetMapCompFunction(uint32_t lhs, uint32_t rhs)
{
    return lhs < rhs;
}

void Turbo::Core::TPipeline::InternalCreate()
{
    Turbo::Core::TPipelineLayout::TLayout layout;
    for (auto &shader_stage : this->shaderStages)
    {
        layout << *(shader_stage->GetShader());
    }
    this->pipelineLayout = this->device->GetLayoutManager().GetOrCreateLayout(layout); // FIXME: Use new TLayout to create!

    return;
}

void Turbo::Core::TPipeline::InternalDestroy()
{
    // delete this->pipelineLayout;
}

std::pair<Turbo::Core::TRefPtr<Turbo::Core::TMemory>, Turbo::Core::TRefPtr<Turbo::Core::TMemory>> Turbo::Core::TPipeline::ShaderStageToSpecializationInfo(TShaderStage *shaderStage)
{
    if (!Turbo::Core::TReferenced::Valid(shaderStage))
    {
        return {nullptr, nullptr};
    }

    auto shader_stage = shaderStage;
    auto compute_shader = shader_stage->GetShader();

    std::vector<VkSpecializationMapEntry> vk_specialization_map_entrys;
    size_t data_size = 0;
    void *specialization_constants_data = nullptr;
    {
        auto &stage_specializations_constants = shader_stage->GetSpecializationConstants();
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
        else
        {
            return {nullptr, nullptr};
        }
    }

    TMemory *map_entries = new TMemory(vk_specialization_map_entrys.size() * sizeof(VkSpecializationMapEntry));
    memcpy(map_entries->Data(), vk_specialization_map_entrys.data(), map_entries->Size());

    TMemory *data = new TMemory(specialization_constants_data, data_size);

    return {map_entries, data};
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, const TPipelineLayout::TLayout &layout, const std::initializer_list<TShaderStage *> &shaderStages, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device) && !std::empty(shaderStages))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaderStages = Turbo::Core::PtrsToRefs(shaderStages);

        if (shaderStages.size() == 1)
        {
            auto compute_shader = this->shaderStages.front();
            if (compute_shader->Valid() && compute_shader->GetShader()->GetType() == TShaderType::COMPUTE)
            {
                this->type = TPipelineType::Compute;
            }
        }

        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;

        {
            Turbo::Core::TPipelineLayout::TLayout temp_layout;
            {
                for (auto &shader : this->shaderStages)
                {
                    temp_layout << *(shader->GetShader());
                }

                auto &Constants = layout.GetPushConstants().GetConstants();
                for (auto &item : Constants)
                {
                    temp_layout.Merge(item.first, item.second.first);
                }
            }

            if (layout == temp_layout) // NOTE: Check layout compatible
            {
                this->pipelineLayout = this->device->GetLayoutManager().GetOrCreateLayout(layout);
            }
            else
            {
                this->pipelineLayout = this->device->GetLayoutManager().GetOrCreateLayout(temp_layout);
            }
        }
        // this->InternalCreate();//NOTE: Don't need call InternalCreate() to create pipeline layout, because we had create pipelie layout previous

        // this->specializationConstants = specializationConstants;
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::~TPipeline()
{
    this->InternalDestroy();
}

Turbo::Core::TPipelineLayout *Turbo::Core::TPipeline::GetPipelineLayout()
{
    return this->pipelineLayout;
}

VkPipeline Turbo::Core::TPipeline::GetVkPipeline()
{
    return this->vkPipeline;
}

Turbo::Core::TPipelineType Turbo::Core::TPipeline::GetType() const
{
    return this->type;
}

std::vector<Turbo::Core::TShaderStage *> Turbo::Core::TPipeline::GetShaderStages() const
{
    return Turbo::Core::RefsToPtrs(this->shaderStages);
}

Turbo::Core::TDevice *Turbo::Core::TPipeline::GetDevice()
{
    return this->device;
}

Turbo::Core::TPipelineCache *Turbo::Core::TPipeline::GetPipelineCache()
{
    return this->pipelineCache;
}

// const Turbo::Core::TPipeline::TSpecializationConstants &Turbo::Core::TPipeline::GetSpecializationConstants() const
//{
//     return this->specializationConstants;
// }

// std::vector<Turbo::Core::TSpecializationConstant> Turbo::Core::TPipeline::GetSpecializationConstants() const
// {
//     std::vector<TSpecializationConstant> specialization_constants;

//     std::map</*constant_id*/ uint32_t, TSpecializationConstant> specialization_constant_map;

//     for (TShader *shader_item : this->shaders)
//     {
//         const std::vector<TSpecializationConstant> &specialization_constant = shader_item->GetSpecializationConstants();
//         for (const TSpecializationConstant &specialization_constant_item : specialization_constant)
//         {
//             uint32_t id = specialization_constant_item.GetConstantID();
//             // const std::string &name = specialization_constant_item.GetName();
//             // Turbo::Core::TDescriptorDataType data_type = specialization_constant_item.GetDescriptorDataType();
//             // uint32_t width = specialization_constant_item.GetWidth();
//             specialization_constant_map[id] = specialization_constant_item;
//         }
//     }

//     for (auto &specialization_constant_item : specialization_constant_map)
//     {
//         specialization_constants.push_back(specialization_constant_item.second);
//     }

//     return specialization_constants;
// }

std::string Turbo::Core::TPipeline::ToString() const
{
    return std::string();
}

bool Turbo::Core::TPipeline::Valid() const
{
    if (this->pipelineLayout->Valid() && this->vkPipeline != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}
