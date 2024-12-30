#include "TPipelineDescriptorSet.h"
#include "TDescriptorSet.h"
#include "TException.h"
#include "TPipelineLayout.h"
#include "TDescriptorSetLayout.h"
#include "TDescriptorPool.h"
#include "TBuffer.h"
#include <sstream>

void Turbo::Core::TPipelineDescriptorSet::InternalCreate()
{
    std::vector<TRefPtr<TDescriptorSetLayout>> descriptor_set_layouts = this->pipelineLayout->GetDescriptorSetLayouts();
    for (TDescriptorSetLayout *descriptor_set_layout_item : descriptor_set_layouts)
    {
        descriptorSets.push_back(new TDescriptorSet(this->descriptorPool, descriptor_set_layout_item));
    }
}

void Turbo::Core::TPipelineDescriptorSet::InternalDestroy()
{
    for (TRefPtr<TDescriptorSet> &descriptor_set_item : this->descriptorSets)
    {
        descriptor_set_item = nullptr;
    }
    this->descriptorSets.clear();
}

Turbo::Core::TPipelineDescriptorSet::TPipelineDescriptorSet(const TRefPtr<TDescriptorPool> &descriptorPool, const TRefPtr<TPipelineLayout> &pipelineLayout)
{
    if (descriptorPool.Valid() && pipelineLayout.Valid())
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

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<TBuffer *> &buffers)
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

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TBuffer>> &buffers)
{
    this->BindData(set, binding, dstArrayElement, Turbo::Core::RefsToPtrs(buffers));
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, TBuffer *buffer, uint32_t dstArrayElement)
{
    std::vector<TBuffer *> buffers;
    buffers.push_back(buffer);

    this->BindData(set, binding, dstArrayElement, buffers);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, const TRefPtr<TBuffer> &buffer, uint32_t dstArrayElement)
{
    this->BindData(set, binding, buffer.Get(), dstArrayElement);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<std::pair<TImageView *, TSampler *>> &combinedImageSamplers)
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

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<std::pair<TRefPtr<TImageView>, TRefPtr<TSampler>>> &combinedImageSamplers)
{
    std::vector<std::pair<TImageView *, TSampler *>> combined_image_samplers;
    for (auto &item : combinedImageSamplers)
    {
        combined_image_samplers.push_back(std::make_pair(item.first.Get(), item.second.Get()));
    }

    this->BindData(set, binding, dstArrayElement, combined_image_samplers);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, const std::pair<TImageView *, TSampler *> &combinedImageSampler, uint32_t dstArrayElement)
{
    std::vector<std::pair<TImageView *, TSampler *>> combined_image_samplers;
    combined_image_samplers.push_back(combinedImageSampler);

    this->BindData(set, binding, dstArrayElement, combined_image_samplers);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, const std::pair<TRefPtr<TImageView>, TRefPtr<TSampler>> &combinedImageSampler, uint32_t dstArrayElement)
{
    std::pair<TImageView *, TSampler *> combined_image_sampler = std::make_pair(combinedImageSampler.first.Get(), combinedImageSampler.second.Get());
    this->BindData(set, binding, combined_image_sampler, dstArrayElement);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<TImageView *> &imageViews)
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

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TImageView>> &imageViews)
{
    this->BindData(set, binding, dstArrayElement, Turbo::Core::RefsToPtrs(imageViews));
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, TImageView *imageView)
{
    std::vector<TImageView *> image_views;
    image_views.push_back(imageView);
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const TRefPtr<TImageView> &imageViews)
{
}

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<TSampler *> &samplers)
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

void Turbo::Core::TPipelineDescriptorSet::BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TSampler>> &samplers)
{
    this->BindData(set, binding, dstArrayElement, Turbo::Core::RefsToPtrs(samplers));
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

std::string Turbo::Core::TPipelineDescriptorSet::ToString() const
{
    return std::string();
}

bool Turbo::Core::TPipelineDescriptorSet::Valid() const
{
    for (const TRefPtr<TDescriptorSet> &descriptor_set_item : this->descriptorSets)
    {
        if (!descriptor_set_item.Valid())
        {
            return false;
        }
    }

    return true;
}