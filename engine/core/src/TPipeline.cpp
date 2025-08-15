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
    for (auto &shader : this->shaders)
    {
        layout << (*shader);
    }
    this->pipelineLayout = this->device->GetLayoutManager().GetOrCreateLayout(layout); // FIXME: Use new TLayout to create!

    return;

    std::vector<TPushConstantDescriptor *> pipeline_push_constant_descriptors;

    std::vector<TRefPtr<TDescriptorSetLayout>> descriptor_set_layouts;

    std::map</*set*/ uint32_t, std::vector<TDescriptor *>, bool (*)(uint32_t, uint32_t)> descriptor_set_map(DescriptorSetMapCompFunction);

    // std::map</*constant_id*/ uint32_t, TSpecializationConstant> specialization_constant_map;

    for (TShader *shader_item : this->shaders)
    {
        std::vector<TUniformBufferDescriptor *> uniform_buffer_descriptors = shader_item->GetUniformBufferDescriptors();
        for (TUniformBufferDescriptor *uniform_buffer_descriptor_item : uniform_buffer_descriptors)
        {
            uint32_t set = uniform_buffer_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(uniform_buffer_descriptor_item);
        }

        std::vector<TStorageBufferDescriptor *> storage_buffer_descriptors = shader_item->GetStorageBufferDescriptors();
        for (TStorageBufferDescriptor *storage_buffer_descriptor_item : storage_buffer_descriptors)
        {
            uint32_t set = storage_buffer_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(storage_buffer_descriptor_item);
        }

        std::vector<TCombinedImageSamplerDescriptor *> combined_image_sampler_descriptors = shader_item->GetCombinedImageSamplerDescriptors();
        for (TCombinedImageSamplerDescriptor *combined_image_sampler_descriptor_item : combined_image_sampler_descriptors)
        {
            uint32_t set = combined_image_sampler_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(combined_image_sampler_descriptor_item);
        }

        std::vector<TSampledImageDescriptor *> sampled_image_descriptors = shader_item->GetSampledImageDescriptors();
        for (TSampledImageDescriptor *sampled_image_descriptor_item : sampled_image_descriptors)
        {
            uint32_t set = sampled_image_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(sampled_image_descriptor_item);
        }

        std::vector<TSamplerDescriptor *> sampler_descriptors = shader_item->GetSamplerDescriptors();
        for (TSamplerDescriptor *sampler_descriptor_item : sampler_descriptors)
        {
            uint32_t set = sampler_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(sampler_descriptor_item);
        }

        std::vector<TPushConstantDescriptor *> push_constant_descriptors = shader_item->GetPushConstantDescriptors();
        for (TPushConstantDescriptor *push_constant_descriptor_item : push_constant_descriptors)
        {
            pipeline_push_constant_descriptors.push_back(push_constant_descriptor_item);
        }

        std::vector<TInputAttachmentDescriptor *> input_attachment_descriptors = shader_item->GetInputAttachmentDescriptors();
        for (TInputAttachmentDescriptor *input_attachment_descriptor_item : input_attachment_descriptors)
        {
            uint32_t set = input_attachment_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(input_attachment_descriptor_item);
        }

        std::vector<TStorageImageDescriptor *> storage_image_descriptors = shader_item->GetStorageImageDescriptors();
        for (TStorageImageDescriptor *storage_image_descriptors_item : storage_image_descriptors)
        {
            uint32_t set = storage_image_descriptors_item->GetSet();
            descriptor_set_map[set].push_back(storage_image_descriptors_item);
        }

        std::vector<TAccelerationStructureDescriptor *> acceleration_structure_descriptors = shader_item->GetAccelerationStructureDescriptors();
        for (TAccelerationStructureDescriptor *acceleration_structure_descriptors_item : acceleration_structure_descriptors)
        {
            uint32_t set = acceleration_structure_descriptors_item->GetSet();
            descriptor_set_map[set].push_back(acceleration_structure_descriptors_item);
        }
    }

    {
        std::map</*set*/ uint32_t, std::vector<TDescriptor *>, bool (*)(uint32_t, uint32_t)> temp_descriptor_set_map = descriptor_set_map;
        for (std::map</*set*/ uint32_t, std::vector<TDescriptor *>, bool (*)(uint32_t, uint32_t)>::iterator it = temp_descriptor_set_map.begin(); it != temp_descriptor_set_map.end(); ++it)
        {
            std::map</*set*/ uint32_t, std::vector<TDescriptor *>, bool (*)(uint32_t, uint32_t)>::iterator current_it = it;
            std::map</*set*/ uint32_t, std::vector<TDescriptor *>, bool (*)(uint32_t, uint32_t)>::iterator next_it = ++current_it;
            current_it = it;

            if (next_it != temp_descriptor_set_map.end())
            {
                uint32_t current_set = current_it->first;
                uint32_t next_set = next_it->first;
                while ((current_set + 1) != next_set)
                {
                    current_set = current_set + 1;
                    // FIXME: need delete in [TPipelineLayout::~TPipelineLayout()]. TNaNDescriptor [name] is <NaN> and [shader] is <nullptr>
                    descriptor_set_map[current_set].push_back(new TNaNDescriptor(current_set));
                }
            }
        }
    }

    for (std::pair<const uint32_t, std::vector<Turbo::Core::TDescriptor *>> &descriptor_set_item : descriptor_set_map)
    {
        // memory delete in TPipelineLayout::~TPipelineLayout()
        TDescriptorSetLayout *descriptor_set_layout = new TDescriptorSetLayout(this->device, descriptor_set_item.second);
        descriptor_set_layouts.push_back(descriptor_set_layout);
    }

    // this->pipelineLayout = new TPipelineLayout(this->device, descriptor_set_layouts, pipeline_push_constant_descriptors);
    {
        Turbo::Core::TPipelineLayout::TLayout layout;
        for (auto &shader : this->shaders)
        {
            layout << (*shader);
        }
        this->pipelineLayout = this->device->GetLayoutManager().GetOrCreateLayout(layout);
    }
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

Turbo::Core::TPipeline::TPipeline(const TRefPtr<TDevice> &device, TPipelineType type, std::vector<TRefPtr<TShader>> &shaders, const TRefPtr<TPipelineCache> &pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (device.Valid())
    {
        this->device = device;
        this->type = type;
        this->shaders = shaders;
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    // if (device.Valid() && vertexShader.Valid() && fragmentShader.Valid())
    if (Turbo::Core::TReferenced::Valid(device, vertexShader, fragmentShader))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)
{
    // if (device.Valid() && vertexShader.Valid() && tessellationControlShader.Valid() && tessellationEvaluationShader.Valid() && fragmentShader.Valid())
    if (Turbo::Core::TReferenced::Valid(device, vertexShader, tessellationControlShader, tessellationEvaluationShader, fragmentShader))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(tessellationControlShader);
        this->shaders.push_back(tessellationEvaluationShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    // if (device.Valid() && vertexShader.Valid() && geometryShader.Valid() && fragmentShader.Valid())
    if (Turbo::Core::TReferenced::Valid(device, vertexShader, geometryShader, fragmentShader))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(geometryShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    // if (device.Valid() && vertexShader.Valid() && tessellationControlShader.Valid() && tessellationEvaluationShader.Valid() && geometryShader.Valid() && fragmentShader.Valid())
    if (Turbo::Core::TReferenced::Valid(device, vertexShader, tessellationControlShader, tessellationEvaluationShader, geometryShader, fragmentShader))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(tessellationControlShader);
        this->shaders.push_back(tessellationEvaluationShader);
        this->shaders.push_back(geometryShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TMeshShader *meshShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)
{
    // if (device.Valid() && meshShader.Valid() && fragmentShader.Valid())
    if (Turbo::Core::TReferenced::Valid(device, meshShader, fragmentShader))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(meshShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TComputeShader *computeShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    // if (device.Valid() && computeShader.Valid())
    if (Turbo::Core::TReferenced::Valid(device, computeShader))
    {
        this->device = device;
        this->type = TPipelineType::Compute;
        this->shaders.push_back(computeShader);
        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, const TPipelineLayout::TLayout &layout, const std::initializer_list<TShader *> &shaders, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        bool is_have_compute_shader = false; // FIXME: Need more elegant way to confirm pipeline type
        for (auto &shader : shaders)
        {
            if (Turbo::Core::TReferenced::Valid(shader))
            {
                this->shaders.push_back(shader);
                if (shader->GetType() == TShaderType::COMPUTE)
                {
                    is_have_compute_shader = true;
                }
            }
        }
        if (is_have_compute_shader)
        {
            this->type = TPipelineType::Compute;
        }

        this->pipelineCache = Turbo::Core::TReferenced::Valid(pipelineCache) ? pipelineCache : nullptr;

        {
            Turbo::Core::TPipelineLayout::TLayout temp_layout;
            {
                for (auto &shader : this->shaders)
                {
                    temp_layout << (*shader);
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
                for (auto &shader : this->shaders)
                {
                    temp_layout << (*shader);
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

std::vector<Turbo::Core::TShader *> Turbo::Core::TPipeline::GetShaders()
{
    std::vector<Turbo::Core::TShader *> result;
    for (auto &shader : this->shaders)
    {
        result.push_back(shader);
    }
    return result;
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
