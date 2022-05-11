#include "TPipeline.h"
#include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TShader.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TPipeline::InternalCreate()
{
    std::vector<VkDescriptorSetLayout> vk_descriptor_set_layouts;

    for (TShader *shader_item : this->shaders)
    {
        std::vector<TDescriptorSetLayout *> descriptor_set_layouts = shader_item->GetDescriptorSetLayouts();
        for (TDescriptorSetLayout *descriptor_set_layout_item : descriptor_set_layouts)
        {
            vk_descriptor_set_layouts.push_back(descriptor_set_layout_item->GetVkDescriptorSetLayout());
        }
    }

    VkPipelineLayoutCreateInfo vk_pipline_layout_create_info = {};
    vk_pipline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vk_pipline_layout_create_info.pNext = nullptr;
    vk_pipline_layout_create_info.flags = 0;
    vk_pipline_layout_create_info.setLayoutCount = vk_descriptor_set_layouts.size();
    vk_pipline_layout_create_info.pSetLayouts = vk_descriptor_set_layouts.data();
    vk_pipline_layout_create_info.pushConstantRangeCount = 0;
    vk_pipline_layout_create_info.pPushConstantRanges = nullptr;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreatePipelineLayout(vk_device, &vk_pipline_layout_create_info, allocator, &this->vkPipelineLayout);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TPipeline::InternalCreate::vkCreatePipelineLayout");
    }
}

void Turbo::Core::TPipeline::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

    vkDestroyPipelineLayout(vk_device, this->vkPipelineLayout, allocator);
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

VkPipelineLayout Turbo::Core::TPipeline::GetVkPipelineLayout()
{
    return this->vkPipelineLayout;
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
