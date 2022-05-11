#include "TDescriptorSet.h"
#include "TBuffer.h"
#include "TDescriptorPool.h"
#include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TImageView.h"
#include "TSampler.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TDescriptorSet::InternalCreate()
{
    VkDescriptorPool vk_descriptor_pool = this->descriptorPool->GetVkDescriptorPool();

    VkDescriptorSetLayout vk_descriptor_set_layout = this->descriptorSetLayout->GetVkDescriptorSetLayout();

    VkDescriptorSetAllocateInfo decriptor_set_allocate_info = {};
    decriptor_set_allocate_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    decriptor_set_allocate_info.pNext = nullptr;
    decriptor_set_allocate_info.descriptorPool = vk_descriptor_pool;
    decriptor_set_allocate_info.descriptorSetCount = 1;
    decriptor_set_allocate_info.pSetLayouts = &vk_descriptor_set_layout;

    VkDevice vk_device = this->descriptorPool->GetDevice()->GetVkDevice();
    VkResult result = vkAllocateDescriptorSets(vk_device, &decriptor_set_allocate_info, &this->vkDescriptorSet);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TDescriptorSet::InternalCreate::vkAllocateDescriptorSets");
    }
}

void Turbo::Core::TDescriptorSet::InternalDestroy()
{
    VkDevice vk_device = this->descriptorPool->GetDevice()->GetVkDevice();
    VkDescriptorPool vk_descriptor_pool = this->descriptorPool->GetVkDescriptorPool();

    vkFreeDescriptorSets(vk_device, vk_descriptor_pool, 1, &this->vkDescriptorSet);
}

Turbo::Core::TDescriptorSet::TDescriptorSet(TDescriptorPool *descriptorPool, TDescriptorSetLayout *descriptorSetLayout) : Turbo::Core::TVulkanHandle()
{
    if (descriptorPool != nullptr && descriptorSetLayout != nullptr)
    {
        this->descriptorPool = descriptorPool;
        this->descriptorSetLayout = descriptorSetLayout;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSet::TDescriptorSet");
    }
}

Turbo::Core::TDescriptorSet::~TDescriptorSet()
{
    this->InternalDestroy();
}

VkDescriptorSet Turbo::Core::TDescriptorSet::GetVkDescriptorSet()
{
    return this->vkDescriptorSet;
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t arrayElement, std::vector<TBuffer *> buffers)
{
    std::vector<VkDescriptorBufferInfo> vk_descriptor_buffer_infos;
    for (TBuffer *buffer_item : buffers)
    {
        VkDescriptorBufferInfo vk_descriptor_buffer_info;
        vk_descriptor_buffer_info.buffer = buffer_item->GetVkBuffer();
        vk_descriptor_buffer_info.offset = 0;
        vk_descriptor_buffer_info.range = VK_WHOLE_SIZE;

        vk_descriptor_buffer_infos.push_back(vk_descriptor_buffer_info);
    }

    VkWriteDescriptorSet vk_write_descriptor_set;
    vk_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_write_descriptor_set.pNext = nullptr;
    vk_write_descriptor_set.dstSet = this->vkDescriptorSet;
    vk_write_descriptor_set.dstBinding = binding;
    vk_write_descriptor_set.dstArrayElement = arrayElement;
    vk_write_descriptor_set.descriptorCount = buffers.size();
    vk_write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    vk_write_descriptor_set.pImageInfo = nullptr;
    vk_write_descriptor_set.pBufferInfo = vk_descriptor_buffer_infos.data();
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    VkDevice vk_device = this->descriptorPool->GetDevice()->GetVkDevice();
    vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t arrayElement, TImageView *imageView, TSampler *sampler)
{
    VkDescriptorImageInfo vk_descriptor_image_info = {};
    vk_descriptor_image_info.sampler = sampler->GetVkSampler();
    vk_descriptor_image_info.imageView = imageView->GetVkImageView();
    vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet vk_write_descriptor_set;
    vk_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_write_descriptor_set.pNext = nullptr;
    vk_write_descriptor_set.dstSet = this->vkDescriptorSet;
    vk_write_descriptor_set.dstBinding = binding;
    vk_write_descriptor_set.dstArrayElement = arrayElement;
    vk_write_descriptor_set.descriptorCount = 1;
    vk_write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    vk_write_descriptor_set.pImageInfo = &vk_descriptor_image_info;
    vk_write_descriptor_set.pBufferInfo = nullptr;
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    VkDevice vk_device = this->descriptorPool->GetDevice()->GetVkDevice();
    vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

std::string Turbo::Core::TDescriptorSet::ToString()
{
    return std::string();
}
