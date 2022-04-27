#include "TDescriptorSetLayout.h"
#include "TDescriptor.h"
#include "TDevice.h"
#include "TException.h"
#include "TShader.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TDescriptorSetLayout::InternalCreate()
{
    uint32_t binding_count = this->descriptors.size();

    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(binding_count);

    for (uint32_t binding_index = 0; binding_index < binding_count; binding_index++)
    {
        TDescriptor *descriptor = this->descriptors[binding_index];
        if (descriptor != nullptr)
        {
            bindings[binding_index].binding = descriptor->GetBinding();
            bindings[binding_index].descriptorType = descriptor->GetVkDescriptorType();
            bindings[binding_index].descriptorCount = descriptor->GetCount();
            bindings[binding_index].stageFlags = this->shader->GetVkShaderStageFlags();
            bindings[binding_index].pImmutableSamplers = nullptr;
        }
        else
        {
            throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
        }
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = binding_count;
    descriptorSetLayoutCreateInfo.pBindings = bindings.data();

    VkDevice vk_device = this->shader->GetDevice()->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateDescriptorSetLayout(vk_device, &descriptorSetLayoutCreateInfo, allocator, &this->vkDescriptorSetLayout);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
    }
}

void Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout::InternalDestroy()
{
    VkDevice vk_device = this->shader->GetDevice()->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vkDestroyDescriptorSetLayout(vk_device, this->vkDescriptorSetLayout, allocator);
}

Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout(TShader *shader, std::vector<TDescriptor *> &descriptors) : Turbo::Core::TVulkanHandle()
{
    if (shader != nullptr)
    {
        this->shader = shader;
        this->descriptors = descriptors;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TDescriptorSetLayout::~TDescriptorSetLayout()
{
}

VkDescriptorSetLayout Turbo::Core::TDescriptorSetLayout::GetVkDescriptorSetLayout()
{
    return this->vkDescriptorSetLayout;
}

std::string Turbo::Core::TDescriptorSetLayout::ToString()
{
    return std::string();
}
