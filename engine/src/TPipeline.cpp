#include "TPipeline.h"
#include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TPipelineLayout.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include <map>

void Turbo::Core::TPipeline::InternalCreate()
{
    std::vector<TDescriptorSetLayout *> descriptor_set_layouts;

    std::map</*set*/ uint32_t, std::vector<TDescriptor *>> descriptor_set_map;

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
    }

    for (std::pair<const unsigned int, std::vector<Turbo::Core::TDescriptor *>> &descriptor_set_item : descriptor_set_map)
    {
        // memory delete in TPipelineLayout::~TPipelineLayout()
        TDescriptorSetLayout *descriptor_set_layout = new TDescriptorSetLayout(this->device, descriptor_set_item.second);
        descriptor_set_layouts.push_back(descriptor_set_layout);
    }

    this->pipelineLayout = new TPipelineLayout(this->device, descriptor_set_layouts);
}

void Turbo::Core::TPipeline::InternalDestroy()
{
    delete this->pipelineLayout;
}

Turbo::Core::TPipeline::TPipeline(TDevice *device, TPipelineType type, std::vector<TShader *> &shaders) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr)
    {
        this->device = device;
        this->type = type;
        this->shaders = shaders;
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

std::string Turbo::Core::TPipeline::ToString()
{
    return std::string();
}
