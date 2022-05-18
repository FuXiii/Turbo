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
        TShader *shader = descriptor->GetShader();
        if (descriptor != nullptr)
        {
            bindings[binding_index].binding = descriptor->GetBinding();
            bindings[binding_index].descriptorType = descriptor->GetVkDescriptorType();
            bindings[binding_index].descriptorCount = descriptor->GetCount();
            bindings[binding_index].stageFlags = VK_SHADER_STAGE_ALL;
            if (shader != nullptr)
            {
                bindings[binding_index].stageFlags = descriptor->GetShader()->GetVkShaderStageFlags();
            }
            bindings[binding_index].pImmutableSamplers = nullptr;
        }
        else
        {
            throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSetLayout::InternalCreate");
        }
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = binding_count;
    descriptorSetLayoutCreateInfo.pBindings = bindings.data();

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateDescriptorSetLayout(vk_device, &descriptorSetLayoutCreateInfo, allocator, &this->vkDescriptorSetLayout);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TDescriptorSetLayout::InternalCreate::vkCreateDescriptorSetLayout");
    }
}

void Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vkDestroyDescriptorSetLayout(vk_device, this->vkDescriptorSetLayout, allocator);
}

Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout(TDevice *device, std::vector<TDescriptor *> &descriptors) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr)
    {
        this->device = device;
        this->descriptors = descriptors;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout");
    }
}

uint32_t Turbo::Core::TDescriptorSetLayout::GetSet()
{
    return this->descriptors[0]->GetSet();
}

Turbo::Core::TDescriptorSetLayout::~TDescriptorSetLayout()
{
    this->InternalDestroy();
}

VkDescriptorSetLayout Turbo::Core::TDescriptorSetLayout::GetVkDescriptorSetLayout()
{
    return this->vkDescriptorSetLayout;
}

std::string Turbo::Core::TDescriptorSetLayout::ToString()
{
    return std::string();
}
