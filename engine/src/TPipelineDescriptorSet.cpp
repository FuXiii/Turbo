#include "TPipelineDescriptorSet.h"
#include "TDescriptorSet.h"
#include "TException.h"
#include "TPipelineLayout.h"
#include <sstream>

void Turbo::Core::TPipelineDescriptorSet::InternalCreate()
{
    std::vector<TDescriptorSetLayout *> descriptor_set_layouts = this->pipelineLayout->GetDescriptorSetLayout();
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

Turbo::Core::TPipelineDescriptorSet::TPipelineDescriptorSet(TDescriptorPool *descriptorPool, TPipelineLayout *pipelineLayout)
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

const std::vector<Turbo::Core::TDescriptorSet *> &Turbo::Core::TPipelineDescriptorSet::GetDescriptorSet()
{
    return this->descriptorSets;
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t arrayElement, std::vector<TBuffer *> &buffers)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, arrayElement, buffers);
            return;
        }
    }

    std::stringstream ss;
    ss << "There not have TDescriptorSet set=" << set << " ,please check the number of set";
    throw Turbo::Core::TException(TResult::UNSUPPORTED, "Turbo::Core::TPipelineDescriptorSet::BindData", ss.str());
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t arrayElement, TImageView *imageView, TSampler *sampler)
{
    for (TDescriptorSet *descriptor_set_item : this->descriptorSets)
    {
        if (descriptor_set_item->GetSet() == set)
        {
            // TODO: to find is have the binding descriptor? throw TException
            descriptor_set_item->BindData(binding, arrayElement, imageView, sampler);
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