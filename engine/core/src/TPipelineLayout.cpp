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

const Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffset &Turbo::Core::TPipelineLayout::TLayout::TPushConstants::GetConstantOffset(const Turbo::Core::TShaderType &shaderType) const
{
    auto find_result = this->constants.find(shaderType);
    if (find_result != this->constants.end())
    {
        return find_result->second.first;
    }

    return 0;
}

bool Turbo::Core::TPipelineLayout::TLayout::TPushConstants::operator==(const TPushConstants &other) const
{
    if (this == &other)
    {
        return true;
    }

    if (other.constants.size() == this->constants.size())
    {
        for (auto &item : other.constants)
        {
            auto find_result = this->constants.find(item.first);
            if (find_result != this->constants.end())
            {
                if (find_result->second.first != item.second.first || find_result->second.second != item.second.second)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

bool Turbo::Core::TPipelineLayout::TLayout::TPushConstants::operator!=(const TPushConstants &other) const
{
    return !((*this) == other);
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

void Turbo::Core::TPipelineLayout::TLayout::Merge(const TPipelineLayout::TLayout::TSet &set, const Turbo::Core::TDescriptorSetLayout::TLayout &layout)
{
    auto find_result = this->sets.find(set);
    if (find_result != this->sets.end())
    {
        find_result->second.Merge(layout);
    }
    else
    {
        this->sets[set] = layout;
    }
}

bool Turbo::Core::TPipelineLayout::TLayout::operator==(const TPipelineLayout::TLayout &other) const
{
    if (this == &other)
    {
        return true;
    }

    if (other.pushConstants != this->pushConstants)
    {
        return false;
    }

    if (this->sets.size() == other.sets.size())
    {
        for (auto &item : other.sets)
        {
            auto find_result = this->sets.find(item.first);
            if (find_result != this->sets.end())
            {
                if (find_result->second != item.second)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

bool Turbo::Core::TPipelineLayout::TLayout::operator!=(const TPipelineLayout::TLayout &other) const
{
    return !((*this) == other);
}

bool Turbo::Core::TPipelineLayout::TLayout::operator>(const TPipelineLayout::TLayout &other) const
{
    if (this == &other)
    {
        return false;
    }

    if (this->sets.size() > other.sets.size())
    {
        // NOTE: Vulkan Spec: Two pipeline layouts are defined to be “compatible for push constants” if they were created with identical push constant ranges.
        // NOTE: parent set push constant will Logically compatible child push constant
        if (this->pushConstants != other.pushConstants)
        {
            return false;
        }

        for (auto &item : other.sets)
        {
            auto find_result = this->sets.find(item.first);
            if (find_result != this->sets.end())
            {
                if (find_result->second > item.second)
                {
                    // NOTE: To do nothing!
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
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
                auto &set = this->sets.at((*iter));
                ss << "{\"" << (*iter) << "\""
                   << ":" << set.ToString() << "}";
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
                auto set = set_item.first;
                auto descriptor_set_layout = set_item.second;

                vk_descriptor_set_layouts.push_back(descriptor_set_layout->GetVkDescriptorSetLayout());
                {
                    this->descriptorSetLayouts[set] = descriptor_set_layout;
                }
            }
        }

        std::vector<VkPushConstantRange> vk_push_constant_ranges;
        {
            std::unordered_map<Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TOffset, std::unordered_map<Turbo::Core::TPipelineLayout::TLayout::TPushConstants::TSize, VkShaderStageFlags>> push_constant_map;
            {
                auto &push_constants = this->layout.GetPushConstants().GetConstants();
                for (auto &item : push_constants)
                {
                    auto shader_stage = static_cast<VkShaderStageFlagBits>(item.first);
                    auto offset = item.second.first;
                    auto size = item.second.second;

                    auto offset_find_result = push_constant_map.find(offset);
                    if (offset_find_result != push_constant_map.end())
                    {
                        auto &size_map = offset_find_result->second;
                        auto size_find_result = size_map.find(size);
                        if (size_find_result != size_map.end())
                        {
                            push_constant_map[offset][size] |= shader_stage;
                        }
                        else
                        {
                            push_constant_map[offset][size] = shader_stage;
                        }
                    }
                    else
                    {
                        push_constant_map[offset][size] = shader_stage;
                    }
                }
            }

            for (auto &offset_item : push_constant_map)
            {
                auto offset = offset_item.first;
                for (auto &size_item : offset_item.second)
                {
                    auto size = size_item.first;
                    auto stage_flags = size_item.second;

                    {
                        VkPushConstantRange vk_push_constant_range = {};
                        vk_push_constant_range.stageFlags = stage_flags;
                        vk_push_constant_range.offset = offset;
                        vk_push_constant_range.size = size;
                        if (vk_push_constant_range.size != 0)
                        {
                            vk_push_constant_ranges.push_back(vk_push_constant_range);
                        }
                    }
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

// Turbo::Core::TPipelineLayout::TPipelineLayout(TDevice *device, const std::vector<TDescriptorSetLayout *> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors)
//{
//     if (Turbo::Core::TReferenced::Valid(device))
//     {
//         this->device = device;
//         this->descriptorSetLayouts = Turbo::Core::PtrsToRefs(descriptorSetLayouts);
//         this->pushConstantDescriptors = pushConstantDescriptors;
//         this->InternalCreate();
//     }
//     else
//     {
//         throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineLayout::TPipelineLayout");
//     }
// }
//
// Turbo::Core::TPipelineLayout::TPipelineLayout(TDevice *device, const std::vector<TRefPtr<TDescriptorSetLayout>> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors)
//{
//     if (Turbo::Core::TReferenced::Valid(device))
//     {
//         this->device = device;
//         this->descriptorSetLayouts = descriptorSetLayouts;
//         this->pushConstantDescriptors = pushConstantDescriptors;
//         this->InternalCreate();
//     }
//     else
//     {
//         throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineLayout::TPipelineLayout");
//     }
// }

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

    this->descriptorSetLayouts.clear();
}

std::unordered_map<Turbo::Core::TPipelineLayout::TLayout::TSet, Turbo::Core::TDescriptorSetLayout *> Turbo::Core::TPipelineLayout::GetDescriptorSetLayouts()
{
    std::unordered_map<Turbo::Core::TPipelineLayout::TLayout::TSet, Turbo::Core::TDescriptorSetLayout *> result;
    for (auto &item : this->descriptorSetLayouts)
    {
        result[item.first] = item.second;
    }

    return result;
}

const Turbo::Core::TPipelineLayout::TLayout &Turbo::Core::TPipelineLayout::GetLayout() const
{
    return this->layout;
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