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
    std::vector<TPushConstantDescriptor *> pipeline_push_constant_descriptors;

    std::vector<TDescriptorSetLayout *> descriptor_set_layouts;

    std::map</*set*/ uint32_t, std::vector<TDescriptor *>, bool (*)(uint32_t, uint32_t)> descriptor_set_map(DescriptorSetMapCompFunction);

    std::map</*constant_id*/ uint32_t, TSpecializationConstant> specialization_constant_map;

    for (TShader *shader_item : this->shaders)
    {
        std::vector<TUniformBufferDescriptor *> uniform_buffer_descriptors = shader_item->GetUniformBufferDescriptors();
        for (TUniformBufferDescriptor *uniform_buffer_descriptor_item : uniform_buffer_descriptors)
        {
            uint32_t set = uniform_buffer_descriptor_item->GetSet();
            descriptor_set_map[set].push_back(uniform_buffer_descriptor_item);
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

    this->pipelineLayout = new TPipelineLayout(this->device, descriptor_set_layouts, pipeline_push_constant_descriptors);
}

void Turbo::Core::TPipeline::InternalDestroy()
{
    delete this->pipelineLayout;
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TPipelineType type, std::vector<TShader *> &shaders, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr)
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
    if (device != nullptr && vertexShader != nullptr && fragmentShader != nullptr)
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)
{
    if (device != nullptr && vertexShader != nullptr && tessellationControlShader != nullptr && tessellationEvaluationShader != nullptr && fragmentShader != nullptr)
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(tessellationControlShader);
        this->shaders.push_back(tessellationEvaluationShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr && vertexShader != nullptr && geometryShader != nullptr && fragmentShader != nullptr)
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(geometryShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr && vertexShader != nullptr && tessellationControlShader != nullptr && tessellationEvaluationShader != nullptr && geometryShader != nullptr && fragmentShader != nullptr)
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(vertexShader);
        this->shaders.push_back(tessellationControlShader);
        this->shaders.push_back(tessellationEvaluationShader);
        this->shaders.push_back(geometryShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TMeshShader *meshShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)
{
    if (device != nullptr && meshShader != nullptr && fragmentShader != nullptr)
    {
        this->device = device;
        this->type = TPipelineType::Graphics;
        this->shaders.push_back(meshShader);
        this->shaders.push_back(fragmentShader);
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipeline::TPipeline");
    }
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TComputeShader *computeShader, TPipelineCache *pipelineCache) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr && computeShader != nullptr)
    {
        this->device = device;
        this->type = TPipelineType::Compute;
        this->shaders.push_back(computeShader);
        this->pipelineCache = pipelineCache;
        this->InternalCreate();
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

Turbo::Core::TPipelineType Turbo::Core::TPipeline::GetType()
{
    return this->type;
}

std::vector<Turbo::Core::TShader *> Turbo::Core::TPipeline::GetShaders()
{
    return this->shaders;
}

Turbo::Core::TDevice *Turbo::Core::TPipeline::GetDevice()
{
    return this->device;
}

Turbo::Core::TPipelineCache *Turbo::Core::TPipeline::GetPipelineCache()
{
    return this->pipelineCache;
}

std::vector<Turbo::Core::TSpecializationConstant> Turbo::Core::TPipeline::GetSpecializationConstants() const
{
    std::vector<TSpecializationConstant> specialization_constants;

    std::map</*constant_id*/ uint32_t, TSpecializationConstant> specialization_constant_map;

    for (TShader *shader_item : this->shaders)
    {
        const std::vector<TSpecializationConstant> &specialization_constant = shader_item->GetSpecializationConstants();
        for (const TSpecializationConstant &specialization_constant_item : specialization_constant)
        {
            uint32_t id = specialization_constant_item.GetConstantID();
            // const std::string &name = specialization_constant_item.GetName();
            // Turbo::Core::TDescriptorDataType data_type = specialization_constant_item.GetDescriptorDataType();
            // uint32_t width = specialization_constant_item.GetWidth();
            specialization_constant_map[id] = specialization_constant_item;
        }
    }

    for (auto &specialization_constant_item : specialization_constant_map)
    {
        specialization_constants.push_back(specialization_constant_item.second);
    }

    return specialization_constants;
}

std::string Turbo::Core::TPipeline::ToString()
{
    return std::string();
}
