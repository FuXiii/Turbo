#include "TDescriptorPool.h"
#include "TDevice.h"
#include "TException.h"
#include "TPipelineDescriptorSet.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include "TPipelineLayout.h"

Turbo::Core::TDescriptorSize::TDescriptorSize(TDescriptor::TType type, uint32_t count)
{
    this->type = type;
    this->count = count;
}

Turbo::Core::TDescriptor::TType Turbo::Core::TDescriptorSize::GetDescriptorType() const
{
    return this->type;
}

uint32_t Turbo::Core::TDescriptorSize::GetDescriptorCount() const
{
    return this->count;
}

std::string Turbo::Core::TDescriptorSize::ToString() const
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
        case TDescriptor::TType::SAMPLER: {

            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::COMBINED_IMAGE_SAMPLER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::SAMPLED_IMAGE: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::STORAGE_IMAGE: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::UNIFORM_TEXEL_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::STORAGE_TEXEL_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::UNIFORM_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::STORAGE_BUFFER: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::STORAGE_BUFFER_DYNAMIC: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::INPUT_ATTACHMENT: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
            vk_descriptor_pool_size.descriptorCount = descriptor_size_item.GetDescriptorCount();

            vk_descriptor_pool_sizes.push_back(vk_descriptor_pool_size);
        }
        break;
        case TDescriptor::TType::ACCELERATION_STRUCTURE: {
            VkDescriptorPoolSize vk_descriptor_pool_size = {};
            vk_descriptor_pool_size.type = VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
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

Turbo::Core::TDescriptorPool::TDescriptorPool(TDevice *device, uint32_t maxSetsCount, const std::vector<TDescriptorSize> &descriptorSizes) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device))
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
        uint32_t acceleration_structure_count = 0;

        for (const TDescriptorSize &descriptor_size_item : descriptorSizes)
        {
            switch (descriptor_size_item.GetDescriptorType())
            {
            case TDescriptor::TType::SAMPLER: {
                sampler_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::COMBINED_IMAGE_SAMPLER: {
                combined_image_sampler_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::SAMPLED_IMAGE: {
                sampled_image_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_IMAGE: {
                storage_image_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::UNIFORM_TEXEL_BUFFER: {
                uniform_texel_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_TEXEL_BUFFER: {
                storage_texel_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::UNIFORM_BUFFER: {
                uniform_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_BUFFER: {
                storage_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC: {
                uniform_buffer_dynamic_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_BUFFER_DYNAMIC: {
                storage_buffer_dynamic_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::INPUT_ATTACHMENT: {
                input_attachment_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::ACCELERATION_STRUCTURE: {
                acceleration_structure_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            }
        }

        if (sampler_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::SAMPLER, sampler_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (combined_image_sampler_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::COMBINED_IMAGE_SAMPLER, combined_image_sampler_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (sampled_image_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::SAMPLED_IMAGE, sampled_image_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_image_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_IMAGE, storage_image_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_texel_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::UNIFORM_TEXEL_BUFFER, uniform_texel_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_texel_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_TEXEL_BUFFER, storage_texel_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::UNIFORM_BUFFER, uniform_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_BUFFER, storage_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_buffer_dynamic_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC, uniform_buffer_dynamic_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_buffer_dynamic_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_BUFFER_DYNAMIC, storage_buffer_dynamic_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (input_attachment_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::INPUT_ATTACHMENT, input_attachment_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (acceleration_structure_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::ACCELERATION_STRUCTURE, acceleration_structure_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorPool::TDescriptorPool");
    }
}

Turbo::Core::TDescriptorPool::TDescriptorPool(TDevice *device, const std::vector<TDescriptorSize> &descriptorSizes) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;

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
        uint32_t acceleration_structure_count = 0;

        for (const TDescriptorSize &descriptor_size_item : descriptorSizes)
        {
            switch (descriptor_size_item.GetDescriptorType())
            {
            case TDescriptor::TType::SAMPLER: {
                sampler_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::COMBINED_IMAGE_SAMPLER: {
                combined_image_sampler_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::SAMPLED_IMAGE: {
                sampled_image_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_IMAGE: {
                storage_image_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::UNIFORM_TEXEL_BUFFER: {
                uniform_texel_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_TEXEL_BUFFER: {
                storage_texel_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::UNIFORM_BUFFER: {
                uniform_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_BUFFER: {
                storage_buffer_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC: {
                uniform_buffer_dynamic_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::STORAGE_BUFFER_DYNAMIC: {
                storage_buffer_dynamic_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::INPUT_ATTACHMENT: {
                input_attachment_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            case TDescriptor::TType::ACCELERATION_STRUCTURE: {
                acceleration_structure_count += descriptor_size_item.GetDescriptorCount();
            }
            break;
            }
        }

        if (sampler_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::SAMPLER, sampler_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (combined_image_sampler_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::COMBINED_IMAGE_SAMPLER, combined_image_sampler_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (sampled_image_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::SAMPLED_IMAGE, sampled_image_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_image_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_IMAGE, storage_image_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_texel_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::UNIFORM_TEXEL_BUFFER, uniform_texel_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_texel_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_TEXEL_BUFFER, storage_texel_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::UNIFORM_BUFFER, uniform_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_buffer_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_BUFFER, storage_buffer_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (uniform_buffer_dynamic_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::UNIFORM_BUFFER_DYNAMIC, uniform_buffer_dynamic_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (storage_buffer_dynamic_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::STORAGE_BUFFER_DYNAMIC, storage_buffer_dynamic_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (input_attachment_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::INPUT_ATTACHMENT, input_attachment_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        if (acceleration_structure_count != 0)
        {
            TDescriptorSize descriptor_size(TDescriptor::TType::ACCELERATION_STRUCTURE, acceleration_structure_count);
            this->descriptorSizes.push_back(descriptor_size);
        }

        {
            this->maxSetsCount += sampler_count;
            this->maxSetsCount += combined_image_sampler_count;
            this->maxSetsCount += sampled_image_count;
            this->maxSetsCount += storage_image_count;
            this->maxSetsCount += uniform_texel_buffer_count;
            this->maxSetsCount += storage_texel_buffer_count;
            this->maxSetsCount += uniform_buffer_count;
            this->maxSetsCount += storage_buffer_count;
            this->maxSetsCount += uniform_buffer_dynamic_count;
            this->maxSetsCount += storage_buffer_dynamic_count;
            this->maxSetsCount += input_attachment_count;
            this->maxSetsCount += acceleration_structure_count;
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
    for (; this->pipelineDescriptorSets.size() > 0;)
    {
        this->Free(this->pipelineDescriptorSets[0]);
    }

    this->InternalDestroy();
    this->descriptorSizes.clear();
}

Turbo::Core::TDescriptorSet *Turbo::Core::TDescriptorPool::Allocate(TDescriptorSetLayout *descriptorSetLayout)
{
    if (Turbo::Core::TReferenced::Valid(descriptorSetLayout))
    {
        Turbo::Core::TDescriptorSet *result = new Turbo::Core::TDescriptorSet(this, descriptorSetLayout);
        this->descriptorSets.push_back(result);
        return result;
    }

    return nullptr;
}

Turbo::Core::TDescriptorSet *Turbo::Core::TDescriptorPool::Allocate(const Turbo::Core::TDescriptorSetLayout::TLayout &layout)
{
    // if (!layout.Empty() && this->device.Valid())
    if (this->device.Valid())
    {
        Turbo::Core::TDescriptorSet *result = new Turbo::Core::TDescriptorSet(this, this->device->GetLayoutManager().GetOrCreateLayout(layout));
        this->descriptorSets.push_back(result);
        return result;
    }

    return nullptr;
}

void Turbo::Core::TDescriptorPool::Free(TDescriptorSet *descriptorSet)
{
    // free descriptorSet;
    if (Turbo::Core::TReferenced::Valid(descriptorSet))
    {
        uint32_t index = 0;
        bool is_found = false;
        for (auto &item : this->descriptorSets)
        {
            if (item == descriptorSet)
            {
                is_found = true;
                break;
            }
            index = index + 1;
        }

        if (is_found)
        {
            // delete this->commandBuffers[index];
            this->descriptorSets.erase(this->descriptorSets.begin() + index);
            descriptorSet = nullptr;
        }
    }
}

Turbo::Core::TPipelineDescriptorSet *Turbo::Core::TDescriptorPool::Allocate(TPipelineLayout *pipelineLayout)
{
    this->pipelineDescriptorSets.push_back(new Turbo::Core::TPipelineDescriptorSet(this, pipelineLayout));
    return this->pipelineDescriptorSets.back();
}

Turbo::Core::TPipelineDescriptorSet *Turbo::Core::TDescriptorPool::Allocate(const TPipelineLayout::TLayout &layout)
{
    // if (!layout.Empty() && this->device.Valid())
    if (this->device.Valid())
    {
        auto result = new Turbo::Core::TPipelineDescriptorSet(this, this->device->GetLayoutManager().GetOrCreateLayout(layout));
        this->pipelineDescriptorSets.push_back(result);
        return result;
    }

    return nullptr;
}

void Turbo::Core::TDescriptorPool::Free(TPipelineDescriptorSet *pipelineDescriptorSet)
{
    // free pipelineDescriptorSet;
    uint32_t index = 0;
    bool is_found = false;
    for (Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> &pipeline_descriptor_set_item : this->pipelineDescriptorSets)
    {
        if (pipeline_descriptor_set_item == pipelineDescriptorSet)
        {
            is_found = true;
            break;
        }
        index = index + 1;
    }

    if (is_found)
    {
        // delete this->commandBuffers[index];
        this->pipelineDescriptorSets.erase(this->pipelineDescriptorSets.begin() + index);
        //// pipelineDescriptorSet.Release(); // NOTE: It will force release memory
        //
        //// NOTE: Now reference count should be 1
        // if (pipelineDescriptorSet.ReferenceCount() != 1)
        //{
        //     // FIXME: maybe need throw a exception?
        // }
        pipelineDescriptorSet = nullptr;
    }
}

Turbo::Core::TDevice *Turbo::Core::TDescriptorPool::GetDevice()
{
    return this->device;
}

VkDescriptorPool Turbo::Core::TDescriptorPool::GetVkDescriptorPool()
{
    return this->vkDescriptorPool;
}

std::string Turbo::Core::TDescriptorPool::ToString() const
{
    return std::string();
}

bool Turbo::Core::TDescriptorPool::Valid() const
{
    if (this->vkDescriptorPool != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}
