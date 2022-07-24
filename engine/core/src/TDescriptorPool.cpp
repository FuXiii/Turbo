#include "TDescriptorPool.h"
#include "TDevice.h"
#include "TException.h"
#include "TPipelineDescriptorSet.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

Turbo::Core::TDescriptorSize::TDescriptorSize(TDescriptorType type, uint32_t count)
{
    this->type = type;
    this->count = count;
}

Turbo::Core::TDescriptorType Turbo::Core::TDescriptorSize::GetDescriptorType()
{
    return this->type;
}

uint32_t Turbo::Core::TDescriptorSize::GetDescriptorCount()
{
    return this->count;
}

std::string Turbo::Core::TDescriptorSize::ToString()
{
    return std::string();
}

void Turbo::Core::TDescriptorPool::InternalCreate()
{
    std::vector<VkDescriptorPoolSize> vk_descriptor_pool_sizes;
    for (TDescriptorSize &descriptor_size_item : this->descriptorSizes)
    {
        switch (descriptor_size_item.GetDescriptorType())
        {
        case TDescriptorType::SAMPLER: {

            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::COMBINED_IMAGE_SAMPLER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::SAMPLED_IMAGE: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::STORAGE_IMAGE: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::UNIFORM_TEXEL_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::STORAGE_TEXEL_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::UNIFORM_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::STORAGE_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::UNIFORM_BUFFER_DYNAMIC: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::STORAGE_BUFFER_DYNAMIC: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptorType::INPUT_ATTACHMENT: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        }
    }

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = nullptr;
    descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptorPoolCreateInfo.maxSets = this->maxSetsCount;
    if (descriptorPoolCreateInfo.maxSets == 0)
    {
        descriptorPoolCreateInfo.maxSets = 1024;
    }
    descriptorPoolCreateInfo.poolSizeCount = vk_descriptor_pool_sizes.size();
    descriptorPoolCreateInfo.pPoolSizes = vk_descriptor_pool_sizes.data();

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = this->device->GetDeviceDriver()->vkCreateDescriptorPool(vk_device, &descriptorPoolCreateInfo, allocator, &this->vkDescriptorPool);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TDescriptorPool::InternalCreate::vkCreateDescriptorPool");
    }
}

void Turbo::Core::TDescriptorPool::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->device->GetDeviceDriver()->vkDestroyDescriptorPool(vk_device, this->vkDescriptorPool, allocator);
}

Turbo::Core::TDescriptorPool::TDescriptorPool(TDevice *device, uint32_t maxSetsCount, std::vector<TDescriptorSize> &descriptorSizes) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr)
    {
        this->device = device;
        this->maxSetsCount = maxSetsCount;

        uint32_t sampler_count = 0;
        uint32_t combined_image_sampler_count = 0;
        uint32_t sampled_image_count = 0;
        uint32_t storage_image_count = 0;
        uint32_t uniform_texel_buffer_count = 0;
        uint32_t storage_texel_buffer_count = 0;
        uint32_t uniform_buffer_count = 0;
        uint32_t storage_buffer_count = 0;
        uint32_t uniform_buffer_dynamic_count = 0;
        uint32_t storage_buffer_dynamic_count = 0;
        uint32_t input_attachment_count = 0;

        for (TDescriptorSize &descriptor_size_item : descriptorSizes)
        {
            switch (descriptor_size_item.GetDescriptorType())
            {
            case TDescriptorType::SAMPLER: {
                sampler_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::COMBINED_IMAGE_SAMPLER: {
                combined_image_sampler_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::SAMPLED_IMAGE: {
                sampled_image_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::STORAGE_IMAGE: {
                storage_image_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::UNIFORM_TEXEL_BUFFER: {
                uniform_texel_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::STORAGE_TEXEL_BUFFER: {
                storage_texel_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::UNIFORM_BUFFER: {
                uniform_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::STORAGE_BUFFER: {
                storage_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::UNIFORM_BUFFER_DYNAMIC: {
                uniform_buffer_dynamic_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::STORAGE_BUFFER_DYNAMIC: {
                storage_buffer_dynamic_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptorType::INPUT_ATTACHMENT: {
                input_attachment_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            }
        }

        if (sampler_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::SAMPLER, sampler_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (combined_image_sampler_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::COMBINED_IMAGE_SAMPLER, combined_image_sampler_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (sampled_image_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::SAMPLED_IMAGE, sampled_image_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_image_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::STORAGE_IMAGE, storage_image_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_texel_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::UNIFORM_TEXEL_BUFFER, uniform_texel_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_texel_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::STORAGE_TEXEL_BUFFER, storage_texel_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::UNIFORM_BUFFER, uniform_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::STORAGE_BUFFER, storage_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_buffer_dynamic_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::UNIFORM_BUFFER_DYNAMIC, uniform_buffer_dynamic_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_buffer_dynamic_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::STORAGE_BUFFER_DYNAMIC, storage_buffer_dynamic_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (input_attachment_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptorType::INPUT_ATTACHMENT, input_attachment_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorPool::TDescriptorPool");
    }
}

Turbo::Core::TDescriptorPool::~TDescriptorPool()
{
    this->InternalDestroy();
    this->descriptorSizes.clear();
}

Turbo::Core::TPipelineDescriptorSet *Turbo::Core::TDescriptorPool::Allocate(TPipelineLayout *pipelineLayout)
{
    return new Turbo::Core::TPipelineDescriptorSet(this, pipelineLayout);
}

void Turbo::Core::TDescriptorPool::Free(TPipelineDescriptorSet *pipelineDescriptorSet)
{
    delete pipelineDescriptorSet;
}

Turbo::Core::TDevice *Turbo::Core::TDescriptorPool::GetDevice()
{
    return this->device;
}

VkDescriptorPool Turbo::Core::TDescriptorPool::GetVkDescriptorPool()
{
    return this->vkDescriptorPool;
}

std::string Turbo::Core::TDescriptorPool::ToString()
{
    return std::string();
}
