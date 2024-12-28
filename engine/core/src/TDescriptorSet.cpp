#include "TDescriptorSet.h"
// #include "TBuffer.h"

// #include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TImageView.h"
#include "TSampler.h"
// #include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

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

    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkResult result = device->GetDeviceDriver()->vkAllocateDescriptorSets(vk_device, &decriptor_set_allocate_info, &this->vkDescriptorSet);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TDescriptorSet::InternalCreate::vkAllocateDescriptorSets");
    }
}

void Turbo::Core::TDescriptorSet::InternalDestroy()
{
    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkDescriptorPool vk_descriptor_pool = this->descriptorPool->GetVkDescriptorPool();

    device->GetDeviceDriver()->vkFreeDescriptorSets(vk_device, vk_descriptor_pool, 1, &this->vkDescriptorSet);
}

Turbo::Core::TDescriptorSet::TDescriptorSet(TDescriptorPool *descriptorPool, TDescriptorSetLayout *descriptorSetLayout) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(descriptorPool, descriptorSetLayout))
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

uint32_t Turbo::Core::TDescriptorSet::GetSet() const
{
    return this->descriptorSetLayout->GetSet();
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TBuffer *> &buffers)
{
    Turbo::Core::TDescriptorType descriptor_type = this->descriptorSetLayout->GetDescriptorType(binding);

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
    vk_write_descriptor_set.dstArrayElement = dstArrayElement;
    vk_write_descriptor_set.descriptorCount = buffers.size();
    vk_write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    if (descriptor_type == Turbo::Core::TDescriptorType::STORAGE_BUFFER)
    {
        vk_write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }
    vk_write_descriptor_set.pImageInfo = nullptr;
    vk_write_descriptor_set.pBufferInfo = vk_descriptor_buffer_infos.data();
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    device->GetDeviceDriver()->vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TBuffer>> &buffers)
{
    std::vector<TBuffer *> temp_buffers;
    for (auto &item : buffers)
    {
        temp_buffers.push_back(item);
    }

    this->BindData(binding, dstArrayElement, temp_buffers);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, TBuffer *buffer, uint32_t dstArrayElement)
{
    std::vector<TBuffer *> buffers;
    buffers.push_back(buffer);
    this->BindData(binding, dstArrayElement, buffers);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<std::pair<TImageView *, TSampler *>> &combinedImageSamplers)
{
    std::vector<VkDescriptorImageInfo> vk_descriptor_image_infos;
    for (auto &combined_image_sampler_item : combinedImageSamplers)
    {
        TImageView *image_view = combined_image_sampler_item.first;
        TSampler *sampler = combined_image_sampler_item.second;

        VkDescriptorImageInfo vk_descriptor_image_info = {};
        vk_descriptor_image_info.sampler = sampler->GetVkSampler();
        vk_descriptor_image_info.imageView = image_view->GetVkImageView();
        vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        vk_descriptor_image_infos.push_back(vk_descriptor_image_info);
    }

    VkWriteDescriptorSet vk_write_descriptor_set;
    vk_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_write_descriptor_set.pNext = nullptr;
    vk_write_descriptor_set.dstSet = this->vkDescriptorSet;
    vk_write_descriptor_set.dstBinding = binding;
    vk_write_descriptor_set.dstArrayElement = dstArrayElement;
    vk_write_descriptor_set.descriptorCount = combinedImageSamplers.size();
    vk_write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    vk_write_descriptor_set.pImageInfo = vk_descriptor_image_infos.data();
    vk_write_descriptor_set.pBufferInfo = nullptr;
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    device->GetDeviceDriver()->vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<std::pair<TRefPtr<TImageView>, TRefPtr<TSampler>>> &combinedImageSamplers)
{
    std::vector<std::pair<TImageView *, TSampler *>> combined_image_samplers;
    for (auto &item : combinedImageSamplers)
    {
        combined_image_samplers.push_back(std::make_pair(item.first.Get(), item.second.Get()));
        // combined_image_samplers.push_back(std::make_pair(item.first, item.second));
    }

    this->BindData(binding, dstArrayElement, combined_image_samplers);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, const std::pair<TImageView *, TSampler *> &combinedImageSampler, uint32_t dstArrayElement)
{
    std::vector<std::pair<TImageView *, TSampler *>> combined_image_samplers;
    combined_image_samplers.push_back(combinedImageSampler);
    this->BindData(binding, dstArrayElement, combined_image_samplers);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TImageView *> &imageViews)
{
    std::vector<VkDescriptorImageInfo> vk_descriptor_image_infos;

    Turbo::Core::TDescriptorType descriptor_type = this->descriptorSetLayout->GetDescriptorType(binding);
    switch (descriptor_type)
    {
    case Turbo::Core::TDescriptorType::SAMPLED_IMAGE:
    case Turbo::Core::TDescriptorType::INPUT_ATTACHMENT: {
        for (TImageView *image_view_item : imageViews)
        {
            VkDescriptorImageInfo vk_descriptor_image_info = {};
            vk_descriptor_image_info.sampler = VK_NULL_HANDLE;
            vk_descriptor_image_info.imageView = image_view_item->GetVkImageView();
            vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            vk_descriptor_image_infos.push_back(vk_descriptor_image_info);
        }
    }
    case Turbo::Core::TDescriptorType::STORAGE_IMAGE: {
        for (TImageView *image_view_item : imageViews)
        {
            VkDescriptorImageInfo vk_descriptor_image_info = {};
            vk_descriptor_image_info.sampler = VK_NULL_HANDLE;
            vk_descriptor_image_info.imageView = image_view_item->GetVkImageView();
            vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;

            vk_descriptor_image_infos.push_back(vk_descriptor_image_info);
        }
    }
    break;
    default: {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSet::BindData", "this descriptor binding type not compatible current layout binding type");
    }
    break;
    }

    VkWriteDescriptorSet vk_write_descriptor_set;
    vk_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_write_descriptor_set.pNext = nullptr;
    vk_write_descriptor_set.dstSet = this->vkDescriptorSet;
    vk_write_descriptor_set.dstBinding = binding;
    vk_write_descriptor_set.dstArrayElement = dstArrayElement;
    vk_write_descriptor_set.descriptorCount = imageViews.size();
    vk_write_descriptor_set.descriptorType = (VkDescriptorType)descriptor_type;
    vk_write_descriptor_set.pImageInfo = vk_descriptor_image_infos.data();
    vk_write_descriptor_set.pBufferInfo = nullptr;
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    device->GetDeviceDriver()->vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TImageView>> &imageViews)
{
    std::vector<TImageView *> image_views;
    for (auto &item : imageViews)
    {
        image_views.push_back(item);
    }

    this->BindData(binding, dstArrayElement, imageViews);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, TImageView *imageView, uint32_t dstArrayElement)
{
    std::vector<TImageView *> image_views;
    image_views.push_back(imageView);
    this->BindData(binding, dstArrayElement, image_views);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TSampler *> &samplers)
{
    std::vector<VkDescriptorImageInfo> vk_descriptor_image_infos;
    for (TSampler *sampler_item : samplers)
    {
        VkDescriptorImageInfo vk_descriptor_image_info = {};
        vk_descriptor_image_info.sampler = sampler_item->GetVkSampler();
        vk_descriptor_image_info.imageView = VK_NULL_HANDLE;
        vk_descriptor_image_info.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

        vk_descriptor_image_infos.push_back(vk_descriptor_image_info);
    }

    VkWriteDescriptorSet vk_write_descriptor_set;
    vk_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_write_descriptor_set.pNext = nullptr;
    vk_write_descriptor_set.dstSet = this->vkDescriptorSet;
    vk_write_descriptor_set.dstBinding = binding;
    vk_write_descriptor_set.dstArrayElement = dstArrayElement;
    vk_write_descriptor_set.descriptorCount = samplers.size();
    vk_write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    vk_write_descriptor_set.pImageInfo = vk_descriptor_image_infos.data();
    vk_write_descriptor_set.pBufferInfo = nullptr;
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    device->GetDeviceDriver()->vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TSampler>> &samplers)
{
    std::vector<TSampler *> temp_samples;
    for (auto &item : samplers)
    {
        temp_samples.push_back(item);
    }
    this->BindData(binding, dstArrayElement, samplers);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, TSampler *sampler, uint32_t dstArrayElement)
{
    std::vector<TSampler *> temp_samples;
    temp_samples.push_back(sampler);
    this->BindData(binding, dstArrayElement, temp_samples);
}

void Turbo::Core::TDescriptorSet::BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<VkAccelerationStructureKHR> &accelerationStructures)
{
    VkWriteDescriptorSetAccelerationStructureKHR vk_write_descriptor_set_acceleration_structure_khr;
    vk_write_descriptor_set_acceleration_structure_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    vk_write_descriptor_set_acceleration_structure_khr.pNext = nullptr;
    vk_write_descriptor_set_acceleration_structure_khr.accelerationStructureCount = accelerationStructures.size();
    vk_write_descriptor_set_acceleration_structure_khr.pAccelerationStructures = accelerationStructures.data();

    VkWriteDescriptorSet vk_write_descriptor_set;
    vk_write_descriptor_set.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    vk_write_descriptor_set.pNext = &vk_write_descriptor_set_acceleration_structure_khr;
    vk_write_descriptor_set.dstSet = this->vkDescriptorSet;
    vk_write_descriptor_set.dstBinding = binding;
    vk_write_descriptor_set.dstArrayElement = dstArrayElement;
    vk_write_descriptor_set.descriptorCount = accelerationStructures.size();
    vk_write_descriptor_set.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    vk_write_descriptor_set.pImageInfo = nullptr;
    vk_write_descriptor_set.pBufferInfo = nullptr;
    vk_write_descriptor_set.pTexelBufferView = nullptr;

    TDevice *device = this->descriptorPool->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    device->GetDeviceDriver()->vkUpdateDescriptorSets(vk_device, 1, &vk_write_descriptor_set, 0, nullptr);
}

std::string Turbo::Core::TDescriptorSet::ToString() const
{
    return std::string();
}

bool Turbo::Core::TDescriptorSet::Valid() const
{
    if (this->vkDescriptorSet != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}