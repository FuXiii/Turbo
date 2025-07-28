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
#include <forward_list>

Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TPushConstants(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset, const TPushConstants::TSize &size)
{
    if (size != 0)
    {
        this->Merge(shaderType, offset, size);
    }
}

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

Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets::TOffsetsMap::const_iterator Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets::begin() const
{
    return this->offsets.begin();
}

Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets::TOffsetsMap::const_iterator Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets::end() const
{
    return this->offsets.end();
}

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets::Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset)
{
    if (offset % 4 == 0)
    {
        this->offsets[shaderType] = offset;
    }
}

// void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::RefreshPushConstantsOffset(const Turbo::Core::TShaderType &startShaderType)
//{
//     std::vector<Turbo::Core::TShaderType> shader_types;
//     {
//         for (auto &item : this->constants)
//         {
//             // if (static_cast<std::size_t>(item.first) <= static_cast<std::size_t>(startShaderType))
//             if (item.first >= startShaderType)
//             {
//                 shader_types.push_back(item.first);
//             }
//         }
//
//         std::sort(shader_types.begin(), shader_types.end());
//     }
//
//     Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffset temp_offset = 0;
//     for (auto &item : shader_types)
//     {
//         auto &offset_and_size = this->constants.at(item);
//         auto &current_offset = offset_and_size.first;
//         if (current_offset == 0)
//         {
//             current_offset = temp_offset;
//         }
//         else
//         {
//             temp_offset = current_offset + offset_and_size.second;
//         }
//     }
// }

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset, const TPushConstants::TSize &size)
{
    if ((offset % 4) == 0 && (size % 4) == 0 && size != 0)
    {
        this->constants[shaderType] = std::make_pair(offset, size);
    }
    else
    {
        // TODO: Wwarning!
    }
}

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset)
{
    if ((offset % 4) == 0)
    {
        auto find_result = this->constants.find(shaderType);
        if (find_result != this->constants.end())
        {
            find_result->second.first = offset;
        }
        else
        {
            // NOTE: Here we need init it for adapt operator<<(const TOffsets& offsets)
            // NOTE: the push constant size will over write by shader layout
            {
                this->constants[shaderType] = std::make_pair(offset, 0);
            }
        }
    }
    else
    {
        // TODO: Wwarning!
    }
}

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Merge(const TPushConstants &pushConstants)
{
    const auto &source_constants = pushConstants.constants;
    if (!source_constants.empty())
    {
        for (auto &source_item : source_constants)
        {
            this->Merge(source_item.first, source_item.second.first, source_item.second.second);
        }
    }
}

void Turbo::Core::TPipelineLayout::TLayout::TPushConstants::Merge(const Turbo::Core::TShader::TLayout::TPushConstant &pushConstant)
{
    if (!pushConstant.Empty())
    {
        auto temp_size = pushConstant.GetSize();
        if ((temp_size % 4) == 0 && temp_size != 0)
        {
            auto find_result = this->constants.find(pushConstant.GetShaderType());
            if (find_result != this->constants.end())
            {
                find_result->second.second = pushConstant.GetSize();
            }
            else
            {
                this->Merge(pushConstant.GetShaderType(), 0, pushConstant.GetSize());
            }
        }
        else
        {
            // TODO: Wwarning!
        }
    }
}

const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TSize &Turbo::Core::TPipelineLayout::TLayout::TPushConstants::GetConstantSize(const Turbo::Core::TShaderType &shaderType) const
{
    auto find_result = this->constants.find(shaderType);
    if (find_result != this->constants.end())
    {
        return find_result->second.second;
    }

    return 0;
}

std::string Turbo::Core::TPipelineLayout::TLayout::TPushConstants::ToString() const
{
    std::stringstream ss;
    {
        std::vector<Turbo::Core::TShaderType> ordered_shader_types;
        {
            for (auto &constant_item : this->constants)
            {
                ordered_shader_types.push_back(constant_item.first);
            }
            std::sort(ordered_shader_types.begin(), ordered_shader_types.end());
        }

        auto iter = ordered_shader_types.begin();
        ss << "[";
        while (iter != ordered_shader_types.end())
        {
            auto &offset_and_size = this->constants.at((*iter));
            ss << "{";
            ss << "\"offset\":" << offset_and_size.first << ",";
            ss << "\"size\":" << offset_and_size.second << ",";
            ss << "\"shader types\":\"" << (*iter) << "\"";
            ss << "}";
            ++iter;
            if (iter != ordered_shader_types.end())
            {
                ss << ",";
            }
        }
        ss << "]";
    }

    return ss.str();
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

const Turbo::Core::TPipelineLayout::TLayout::TSets &Turbo::Core::TPipelineLayout::TLayout::GetSets() const
{
    return this->sets;
}

const Turbo::Core::TPipelineLayout::TLayout::TPushConstants &Turbo::Core::TPipelineLayout::TLayout::GetPushConstants() const
{
    return this->pushConstants;
}

const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TSize &Turbo::Core::TPipelineLayout::TLayout::GetPushConstantSize(const Turbo::Core::TShaderType &shaderType) const
{
    return this->pushConstants.GetConstantSize(shaderType);
}

bool Turbo::Core::TPipelineLayout::TLayout::Empty() const
{
    return this->sets.empty() && this->pushConstants.Empty();
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

void Turbo::Core::TPipelineLayout::TLayout::Merge(const Turbo::Core::TShader::TLayout &layout)
{
    this->Merge(layout.GetSets());
    this->pushConstants.Merge(layout.GetPushConstant());
}

void Turbo::Core::TPipelineLayout::TLayout::Merge(const Turbo::Core::TShaderType &shaderType, const TPushConstants::TOffset &offset)
{
    this->pushConstants.Merge(shaderType, offset);
}

std::size_t Turbo::Core::TPipelineLayout::TLayout::Hash() const
{
    return std::hash<Turbo::Core::TPipelineLayout::TLayout>{}(*(this));
}

std::string Turbo::Core::TPipelineLayout::TLayout::ToString() const
{
    std::stringstream ss;
    ss << "{";
    {
        if (!this->sets.empty())
        {
            std::vector<Turbo::Core::TPipelineLayout::TLayout::TSet> ordered_sets;
            {
                for (auto &set_item : this->sets)
                {
                    ordered_sets.push_back(set_item.first);
                }
                std::sort(ordered_sets.begin(), ordered_sets.end());
            }

            ss << "\"sets\":";
            ss << "[";
            auto iter = ordered_sets.begin();
            while (iter != ordered_sets.end())
            {
                auto set = this->sets.at((*iter));
                ss << "{\"" << (*iter) << "\"" << ":" << set.ToString() << "}";
                ++iter;
                if (iter != ordered_sets.end())
                {
                    ss << ",";
                }
            }
            ss << "],";
        }
    }
    ss << "\"push constants\":" << this->pushConstants.ToString();
    ss << "}";

    return ss.str();
}

void Turbo::Core::TPipelineLayout::InternalCreate()
{
    if (!this->layout.Empty())
    {
        std::vector<VkDescriptorSetLayout> vk_descriptor_set_layouts;
        {
            std::map<Turbo::Core::TPipelineLayout::TLayout::TSet, TDescriptorSetLayout *> ordered_set_map;
            {
                auto &sets = this->layout.GetSets();
                for (auto &set_item : sets)
                {
                    ordered_set_map.insert(std::make_pair(set_item.first, this->device->GetLayoutManager().GetOrCreateLayout(set_item.second)));
                }

                if (ordered_set_map.size() > 1)
                {
                    std::forward_list<Turbo::Core::TPipelineLayout::TLayout::TSet> fill_sets;
                    {
                        auto iter = ordered_set_map.begin();
                        {
                            if ((iter->first) != 0)
                            {
                                for (std::size_t step_index = 0; step_index < iter->first; ++step_index)
                                {
                                    fill_sets.push_front(step_index);
                                }
                            }
                        }
                        while (iter != ordered_set_map.end())
                        {
                            auto current_set = iter->first;
                            ++iter;
                            if (iter != ordered_set_map.end())
                            {
                                auto next_set = iter->first;
                                auto step = next_set - 1 - current_set;
                                for (std::size_t step_index = 1; step_index <= step; ++step_index)
                                {
                                    auto temp_fill_set = current_set + step_index;
                                    fill_sets.push_front(temp_fill_set);
                                }
                            }
                        }
                    }

                    if (!fill_sets.empty())
                    {
                        auto empty_descriptor_set_layout = this->device->GetLayoutManager().GetOrCreateLayout(TDescriptorSetLayout::TLayout());
                        for (auto &set_item : fill_sets)
                        {
                            ordered_set_map.insert(std::make_pair(set_item, empty_descriptor_set_layout));
                        }
                    }
                }
            }

            for (auto &set_item : ordered_set_map)
            {
                auto descriptor_set_layout = set_item.second;
                vk_descriptor_set_layouts.push_back(descriptor_set_layout->GetVkDescriptorSetLayout());
                {
                    this->descriptorSetLayouts.push_back(descriptor_set_layout);
                }
            }
        }

        std::vector<VkPushConstantRange> vk_push_constant_ranges;
        {
            auto &push_constants = this->layout.GetPushConstants().GetConstants();
            for (auto &item : push_constants)
            {
                VkPushConstantRange vk_push_constant_range = {};
                vk_push_constant_range.stageFlags = static_cast<VkShaderStageFlagBits>(item.first);
                vk_push_constant_range.offset = item.second.first;
                vk_push_constant_range.size = item.second.second;
                if (vk_push_constant_range.size != 0)
                {
                    vk_push_constant_ranges.push_back(vk_push_constant_range);
                }
            }
        }

        VkPipelineLayoutCreateInfo vk_pipline_layout_create_info = {};
        vk_pipline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vk_pipline_layout_create_info.pNext = nullptr;
        vk_pipline_layout_create_info.flags = 0;
        vk_pipline_layout_create_info.setLayoutCount = vk_descriptor_set_layouts.size();
        vk_pipline_layout_create_info.pSetLayouts = vk_descriptor_set_layouts.data();

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
    else if (!this->descriptorSetLayouts.empty() || !this->pushConstantDescriptors.empty()) // FIXME: Need to deprecate!
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
    else // TODO: Create an empty pipeline layout
    {
        VkPipelineLayoutCreateInfo vk_pipline_layout_create_info = {};
        vk_pipline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vk_pipline_layout_create_info.pNext = nullptr;
        vk_pipline_layout_create_info.flags = 0;
        vk_pipline_layout_create_info.setLayoutCount = 0;
        vk_pipline_layout_create_info.pSetLayouts = nullptr;

        // NOTE:In Vulkan Spec:Any two elements of pPushConstantRanges must not include the same stage in stageFlags
        vk_pipline_layout_create_info.pushConstantRangeCount = 0;
        vk_pipline_layout_create_info.pPushConstantRanges = nullptr;

        VkDevice vk_device = this->device->GetVkDevice();
        VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
        VkResult result = this->device->GetDeviceDriver()->vkCreatePipelineLayout(vk_device, &vk_pipline_layout_create_info, allocator, &this->vkPipelineLayout);
        if (result != VkResult::VK_SUCCESS)
        {
            throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TPipelineLayout::InternalCreate::vkCreatePipelineLayout");
        }
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
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->layout = layout;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineLayout::TPipelineLayout");
    }
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

Turbo::Core::TPipelineLayout::TLayout &operator<<(Turbo::Core::TPipelineLayout::TLayout &layout, const Turbo::Core::TShader &shader)
{
    layout.Merge(shader.GetLayout());
    return layout;
}

Turbo::Core::TPipelineLayout::TLayout &operator<<(Turbo::Core::TPipelineLayout::TLayout &layout, const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffsets &offsets)
{
    for (auto &item : offsets)
    {
        layout.Merge(item.first, item.second);
    }
    return layout;
}