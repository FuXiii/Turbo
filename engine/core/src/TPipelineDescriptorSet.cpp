#include "TPipelineDescriptorSet.h"
#include "TDescriptorSet.h"
#include "TException.h"
#include "TPipelineLayout.h"
#include <sstream>

void Turbo::Core::TPipelineDescriptorSet::InternalCreate()
{
    std::vector<TDescriptorSetLayout *> descriptor_set_layouts = this->pipelineLayout->GetDescriptorSetLayouts();
    for (TDescriptorSetLayout *descriptor_set_layout_item : descriptor_set_layouts)
    {
        descriptorSets.push_back(new TDescriptorSet(this->descriptorPool, descriptor_set_layout_item));
    }
}

void Turbo::Core::TPipelineDescriptorSet::InternalDestroy()
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        delete descriptor_set_item;
    }
    this->descriptorSets.clear();
}

Turbo::Core::TPipelineDescriptorSet::TPipelineDescriptorSet(const TRefPtr<TDescriptorPool> &descriptorPool, const TRefPtr<TPipelineLayout> &pipelineLayout)
{
    if (descriptorPool != nullptr && pipelineLayout != nullptr)
    {
        this->descriptorPool = descriptorPool;
        this->pipelineLayout = pipelineLayout;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineDescriptorSet::TPipelineDescriptorSet");
    }
}
Turbo::Core::TPipelineDescriptorSet::~TPipelineDescriptorSet()
{
    this->InternalDestroy();
}

const std::vector<Turbo::Core::TRefPtr<Turbo::Core::TDescriptorSet>> &Turbo::Core::TPipelineDescriptorSet::GetDescriptorSet()
{
    return this->descriptorSets;
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TRefPtr<TBuffer>> &buffers)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, dstArrayElement, buffers);
            return;
        }
    }

    std::stringstream ss;
    ss << "There not have TDescriptorSet set=" << set << " ,please check the number of set";
    throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TPipelineDescriptorSet::BindData", ss.str());
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, const TRefPtr<TBuffer> &buffer, uint32_t dstArrayElement)
{
    std::vector<TBuffer *> buffers;
    buffers.push_back(buffer);

    this->BindData(set, binding, dstArrayElement, buffers);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<std::pair<TRefPtr<TImageView>, TRefPtr<TSampler>>> &combinedImageSamplers)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, dstArrayElement, combinedImageSamplers);
            return;
        }
    }

    std::stringstream ss;
    ss << "There not have TDescriptorSet set=" << set << " ,please check the number of set";
    throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TPipelineDescriptorSet::BindData", ss.str());
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TRefPtr<TImageView>> &imageViews)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, dstArrayElement, imageViews);
            return;
        }
    }

    std::stringstream ss;
    ss << "There not have TDescriptorSet set=" << set << " ,please check the number of set";
    throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TPipelineDescriptorSet::BindData", ss.str());
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TRefPtr<TSampler>> &samplers)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, dstArrayElement, samplers);
            return;
        }
    }

    std::stringstream ss;
    ss << "There not have TDescriptorSet set=" << set << " ,please check the number of set";
    throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TPipelineDescriptorSet::BindData", ss.str());
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<VkAccelerationStructureKHR> &accelerationStructures)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, dstArrayElement, accelerationStructures);
            return;
        }
    }

    std::stringstream ss;
    ss << "There not have TDescriptorSet set=" << set << " ,please check the number of set";
    throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TPipelineDescriptorSet::BindData", ss.str());
}

std::string Turbo::Core::TPipelineDescriptorSet::ToString()
{
    return std::string();
}