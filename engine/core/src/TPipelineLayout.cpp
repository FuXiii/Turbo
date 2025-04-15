#include "TPipelineLayout.h"
#include "TDescriptor.h"
#include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

#include <sstream>
#include <bitset>

Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TPushConstants(const TPushConstants::TConstants &constants)
{
    this->constants = constants;
}

Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TPushConstants(TPushConstants::TConstants &&constants)
{
    this->constants = std::move(constants);
}

const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TConstants &Turbo::Core::TPipelineLayout::TLayout::TPushConstants::GetConstants() const
{
    return this->constants;
}

bool Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Empty() const
{
    return this->constants.empty();
}

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Merge(TPushConstants::TOffset offset, TPushConstants::TSize size, VkShaderStageFlags flags)
{
    auto &size_map = this->constants[offset];
    auto find_size_iter = size_map.find(size);
    if (find_size_iter != size_map.end())
    {
        find_size_iter->second |= flags;
    }
    else
    {
        find_size_iter->second = flags;
    }
}

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Merge(const TPushConstants &pushConstants)
{
    const auto &source_constants = pushConstants.constants;
    for (auto &offset_item : source_constants)
    {
        TOffset offset = offset_item.first;
        for (auto &size_item : offset_item.second)
        {
            TSize size = size_item.first;
            VkShaderStageFlags vk_shader_stage_flags = size_item.second;

            {
                auto offset_find_result = this->constants.find(offset);
                if (offset_find_result != this->constants.end())
                {
                    auto &this_size_map = offset_find_result->second;
                    auto size_find_result = this_size_map.find(size);
                    if (size_find_result != this_size_map.end())
                    {
                        (size_find_result->second) |= vk_shader_stage_flags;
                    }
                    else
                    {
                        this_size_map.insert({size, vk_shader_stage_flags});
                    }
                }
                else
                {
                    this->constants[offset][size] = vk_shader_stage_flags;
                }
            }
        }
    }
}

std::string Turbo::Core::TPipelineLayout::TLayout::TPushConstants::ToString() const
{
    std::string result;
    {
        std::stringstream ss;

        for (auto &offset_item : this->constants)
        {
            auto offset = offset_item.first;
            for (auto &size_item : offset_item.second)
            {
                auto size = size_item.first;
                auto shader_stage_flags = size_item.second;

                ss << "(offset:" << offset << ", size: " << size << "): " << std::bitset<sizeof(shader_stage_flags) * 8>(shader_stage_flags) << std::endl;
            }
        }

        result = std::move(ss.str());
        if (!result.empty())
        {
            result.pop_back();
        }
    }

    return result;
}

Turbo::Core::TPipelineLayout::TLayout::TLayout(const TPipelineLayout::TLayout::TSets &sets, const TPipelineLayout::TLayout::TPushConstants &pushConstants)
{
    this->sets = sets;
    this->pushConstants = pushConstants;
}

Turbo::Core::TPipelineLayout::TLayout::TLayout(const TPipelineLayout::TLayout::TSets &sets)
{
    this->sets = sets;
}

Turbo::Core::TPipelineLayout::TLayout::TLayout(const TPipelineLayout::TLayout::TPushConstants &pushConstants)
{
    this->pushConstants = pushConstants;
}

Turbo::Core::TPipelineLayout::TLayout::TLayout(TPipelineLayout::TLayout::TSets &&sets, TPipelineLayout::TLayout::TPushConstants &&pushConstants)
{
    this->sets = std::move(sets);
    this->pushConstants = std::move(pushConstants);
}

void Turbo::Core::TPipelineLayout::TLayout::Merge(const TPipelineLayout::TLayout::TSets &sets)
{
    for (auto &set_item : sets)
    {
        TSet set = set_item.first;
        auto set_find_result = this->sets.find(set);
        if (set_find_result != this->sets.end())
        {
            this->sets[set].Merge(set_item.second);
        }
        else
        {
            this->sets[set] = set_item.second;
        }
    }
}

void Turbo::Core::TPipelineLayout::TLayout::Merge(const TPipelineLayout::TLayout::TPushConstants &pushConstants)
{
    this->pushConstants.Merge(pushConstants);
}

void Turbo::Core::TPipelineLayout::TLayout::Merge(const TShader::TLayout::TPushConstant &pushConstant)
{
    this->pushConstants.Merge(pushConstant.GetOffset(), pushConstant.GetSize(), pushConstant.GetVkShaderStageFlags());
}

void Turbo::Core::TPipelineLayout::TLayout::Merge(const Turbo::Core::TShader::TLayout &layout)
{
    this->Merge(layout.GetSets());
    this->Merge(layout.GetPushConstant());
}

void Turbo::Core::TPipelineLayout::InternalCreate()
{
    std::vector<VkDescriptorSetLayout> vk_descriptor_set_layouts;

    for (TDescriptorSetLayout *descriptor_set_layout_item : this->descriptorSetLayouts)
    {
        vk_descriptor_set_layouts.push_back(descriptor_set_layout_item->GetVkDescriptorSetLayout());
    }

    std::vector<VkPushConstantRange> vk_push_constant_ranges;
    for (TPushConstantDescriptor *push_constant_descriptor_item : this->pushConstantDescriptors)
    {
        VkPushConstantRange vk_push_constant_range = {};
        vk_push_constant_range.stageFlags = push_constant_descriptor_item->GetShader()->GetVkShaderStageFlags();
        vk_push_constant_range.offset = 0;
        vk_push_constant_range.size = push_constant_descriptor_item->GetSize();
        vk_push_constant_ranges.push_back(vk_push_constant_range);
    }

    VkPipelineLayoutCreateInfo vk_pipline_layout_create_info = {};
    vk_pipline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vk_pipline_layout_create_info.pNext = nullptr;
    vk_pipline_layout_create_info.flags = 0;
    vk_pipline_layout_create_info.setLayoutCount = vk_descriptor_set_layouts.size();
    vk_pipline_layout_create_info.pSetLayouts = vk_descriptor_set_layouts.data();

    // NOTE:In Vulkan Spec:Any two elements of pPushConstantRanges must not include the same stage in stageFlags
    vk_pipline_layout_create_info.pushConstantRangeCount = vk_push_constant_ranges.size();
    vk_pipline_layout_create_info.pPushConstantRanges = vk_push_constant_ranges.data();

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = this->device->GetDeviceDriver()->vkCreatePipelineLayout(vk_device, &vk_pipline_layout_create_info, allocator, &this->vkPipelineLayout);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TPipelineLayout::InternalCreate::vkCreatePipelineLayout");
    }
}

void Turbo::Core::TPipelineLayout::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->device->GetDeviceDriver()->vkDestroyPipelineLayout(vk_device, this->vkPipelineLayout, allocator);
}

Turbo::Core::TPipelineLayout::TPipelineLayout(TDevice *device, const std::vector<TDescriptorSetLayout *> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors)
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->descriptorSetLayouts = Turbo::Core::PtrsToRefs(descriptorSetLayouts);
        this->pushConstantDescriptors = pushConstantDescriptors;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineLayout::TPipelineLayout");
    }
}

Turbo::Core::TPipelineLayout::TPipelineLayout(TDevice *device, const std::vector<TRefPtr<TDescriptorSetLayout>> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors)
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->descriptorSetLayouts = descriptorSetLayouts;
        this->pushConstantDescriptors = pushConstantDescriptors;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineLayout::TPipelineLayout");
    }
}

Turbo::Core::TPipelineLayout::TPipelineLayout(TDevice *device, const TPipelineLayout::TLayout &layout)
{
    throw Turbo::Core::TException(TResult::UNIMPLEMENTED, "Turbo::Core::TPipelineLayout::TPipelineLayout(TDevice *device, const TPipelineLayout::TLayout &layout)", "unimplemented");
}

Turbo::Core::TPipelineLayout::~TPipelineLayout()
{
    this->InternalDestroy();

    for (TRefPtr<TDescriptorSetLayout> &descriptor_set_layout_item : this->descriptorSetLayouts)
    {
        descriptor_set_layout_item = nullptr;
    }
}

std::vector<Turbo::Core::TDescriptorSetLayout *> Turbo::Core::TPipelineLayout::GetDescriptorSetLayouts()
{
    return Turbo::Core::RefsToPtrs(this->descriptorSetLayouts);
}

const std::vector<Turbo::Core::TPushConstantDescriptor *> &Turbo::Core::TPipelineLayout::GetPushConstantDescriptors()
{
    return this->pushConstantDescriptors;
}

VkPipelineLayout Turbo::Core::TPipelineLayout::GetVkPipelineLayout()
{
    return this->vkPipelineLayout;
}

std::string Turbo::Core::TPipelineLayout::ToString() const
{
    return std::string();
}

bool Turbo::Core::TPipelineLayout::Valid() const
{
    if (this->vkPipelineLayout != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}