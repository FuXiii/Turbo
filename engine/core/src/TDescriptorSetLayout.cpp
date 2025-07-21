#include "TDescriptorSetLayout.h"
#include "TDescriptor.h"
#include "TDevice.h"
#include "TException.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include <sstream>

Turbo::Core::TDescriptorSetLayout::TLayout::TLayout(const TBindings &bindings)
{
    this->bindings = bindings;
}

std::size_t Turbo::Core::TDescriptorSetLayout::TLayout::GetCount() const
{
    return this->bindings.size();
}

bool Turbo::Core::TDescriptorSetLayout::TLayout::Empty() const
{
    return this->bindings.empty();
}

Turbo::Core::TDescriptorSetLayout::TLayout::TBindings::const_iterator Turbo::Core::TDescriptorSetLayout::TLayout::begin() const
{
    return this->bindings.begin();
}

Turbo::Core::TDescriptorSetLayout::TLayout::TBindings::const_iterator Turbo::Core::TDescriptorSetLayout::TLayout::end() const
{
    return this->bindings.end();
}

void Turbo::Core::TDescriptorSetLayout::TLayout::Merge(TDescriptorSetLayout::TLayout::TBinding binding, const TDescriptor &descriptor)
{
    TBindings bindings;
    bindings.insert({binding, descriptor});
    this->Merge(bindings);
}

void Turbo::Core::TDescriptorSetLayout::TLayout::Merge(const TBindings &bindings)
{
    TDescriptorSetLayout::TLayout layout(bindings);
    this->Merge(layout);
}

void Turbo::Core::TDescriptorSetLayout::TLayout::Merge(const TDescriptorSetLayout::TLayout &layout)
{
    for (auto &item : layout)
    {
        auto &binding = item.first;
        auto find_result = this->bindings.find(binding);
        if (find_result == this->bindings.end())
        {
            this->bindings.insert({binding, item.second});
        }
    }
}

bool Turbo::Core::TDescriptorSetLayout::TLayout::Has(const TDescriptorSetLayout::TLayout::TBinding &binding) const
{
    return this->bindings.find(binding) != this->bindings.end();
}

Turbo::Core::TDescriptor &Turbo::Core::TDescriptorSetLayout::TLayout::operator[](Turbo::Core::TDescriptorSetLayout::TLayout::TBinding &&binding)
{
    return this->bindings[std::forward<Turbo::Core::TDescriptorSetLayout::TLayout::TBinding>(binding)];
}

bool Turbo::Core::TDescriptorSetLayout::TLayout::operator==(const TDescriptorSetLayout::TLayout &other) const
{
    return this->bindings == other.bindings;
}

bool Turbo::Core::TDescriptorSetLayout::TLayout::operator!=(const TDescriptorSetLayout::TLayout &other) const
{
    return !((*this) == other);
}

std::string Turbo::Core::TDescriptorSetLayout::TLayout::ToString() const
{
    std::stringstream ss;
    {
        ss << "{";
        if (!this->bindings.empty())
        {
            ss << "\"bindings\":";
            ss << "[";
            {
                auto iter = this->bindings.begin();
                while (iter != this->bindings.end())
                {
                    ss << "{\"" << (*iter).first << "\"" << ":" << (*iter).second.ToString() << "}";
                    ++iter;
                    if (iter != this->bindings.end())
                    {
                        ss << ",";
                    }
                }
            }
            ss << "]";
        }
        ss << "}";
    }
    return ss.str();
}

void Turbo::Core::TDescriptorSetLayout::InternalCreate()
{
    if (this->descriptors.empty()) // Create use layout
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings(this->layout.GetCount());
        {
            size_t index = 0;
            for (auto &item : this->layout)
            {
                VkDescriptorSetLayoutBinding vk_descriptor_set_layout_binding = {};
                vk_descriptor_set_layout_binding.binding = static_cast<uint32_t>(item.first);
                vk_descriptor_set_layout_binding.descriptorType = static_cast<VkDescriptorType>(item.second.GetType());
                vk_descriptor_set_layout_binding.descriptorCount = static_cast<uint32_t>(item.second.GetCount());
                vk_descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_ALL;
                if (vk_descriptor_set_layout_binding.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
                {
                    vk_descriptor_set_layout_binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
                }
                vk_descriptor_set_layout_binding.pImmutableSamplers = nullptr;

                {
                    bindings[index] = vk_descriptor_set_layout_binding;
                    index += 1;
                }
            }
        }

        VkDescriptorSetLayoutCreateInfo vk_descriptor_set_layout_create_info = {};
        vk_descriptor_set_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        vk_descriptor_set_layout_create_info.pNext = nullptr;
        vk_descriptor_set_layout_create_info.flags = 0;
        vk_descriptor_set_layout_create_info.bindingCount = bindings.size();
        vk_descriptor_set_layout_create_info.pBindings = bindings.data();

        auto driver = this->device->GetDeviceDriver();
        VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
        VkResult result = driver->vkCreateDescriptorSetLayout(this->device->GetVkDevice(), &vk_descriptor_set_layout_create_info, allocator, &(this->vkDescriptorSetLayout));
        if (result != VkResult::VK_SUCCESS)
        {
            throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSetLayout::InternalCreate");
        }
    }
    else
    {
        std::map</*binding*/ uint32_t, std::vector<TDescriptor *>> binding_map;
        for (TDescriptor *descriptor_item : this->descriptors)
        {
            uint32_t binding = descriptor_item->GetBinding();
            TRefPtr<TShader> shader = descriptor_item->GetShader();
            if (shader.Valid())
            {
                binding_map[binding].push_back(descriptor_item);
            }
        }

        std::vector<VkDescriptorSetLayoutBinding> bindings;
        for (auto &descriptors_item : binding_map)
        {
            std::vector<Turbo::Core::TDescriptor *> &descriptors = descriptors_item.second;
            if (descriptors.size() > 0)
            {
                TDescriptor *descriptor = descriptors[0];
                if (descriptor != nullptr)
                {
                    VkDescriptorSetLayoutBinding vk_descriptor_set_layout_binding = {};
                    vk_descriptor_set_layout_binding.binding = descriptor->GetBinding();
                    vk_descriptor_set_layout_binding.descriptorType = descriptor->GetVkDescriptorType();
                    vk_descriptor_set_layout_binding.descriptorCount = descriptor->GetCount();
                    vk_descriptor_set_layout_binding.stageFlags = 0;
                    for (TDescriptor *descriptor_item : descriptors)
                    {
                        TRefPtr<TShader> shader = descriptor_item->GetShader();
                        if (shader.Valid())
                        {
                            vk_descriptor_set_layout_binding.stageFlags |= descriptor_item->GetShader()->GetVkShaderStageFlags();
                        }
                    }
                    vk_descriptor_set_layout_binding.pImmutableSamplers = nullptr;

                    bindings.push_back(vk_descriptor_set_layout_binding);
                }
                else
                {
                    throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSetLayout::InternalCreate");
                }
            }
        }

        VkDescriptorSetLayoutCreateInfo vk_descriptor_set_layout_create_info = {};
        vk_descriptor_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        vk_descriptor_set_layout_create_info.pNext = nullptr;
        vk_descriptor_set_layout_create_info.flags = 0;
        vk_descriptor_set_layout_create_info.bindingCount = bindings.size();
        vk_descriptor_set_layout_create_info.pBindings = bindings.data();

        VkDevice vk_device = this->device->GetVkDevice();
        VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
        VkResult result = this->device->GetDeviceDriver()->vkCreateDescriptorSetLayout(vk_device, &vk_descriptor_set_layout_create_info, allocator, &this->vkDescriptorSetLayout);
        if (result != VK_SUCCESS)
        {
            throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TDescriptorSetLayout::InternalCreate::vkCreateDescriptorSetLayout");
        }
    }
}

void Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->device->GetDeviceDriver()->vkDestroyDescriptorSetLayout(vk_device, this->vkDescriptorSetLayout, allocator);
}

Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout(TDevice *device, const std::vector<TDescriptor *> &descriptors) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->descriptors = descriptors;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout");
    }
}

Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout(TDevice *device, const TDescriptorSetLayout::TLayout &layout) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->layout = layout;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout");
    }
}

Turbo::Core::TDescriptorSetLayout::~TDescriptorSetLayout()
{
    this->InternalDestroy();

    // TODO: release Turbo::Core::TNaNDescriptor
    for (TDescriptor *depcriptor_item : this->descriptors)
    {
        if (depcriptor_item->GetShader() == nullptr) // this is Turbo::Core::TNaNDescriptor
        {
            delete depcriptor_item;
        }
    }
}

uint32_t Turbo::Core::TDescriptorSetLayout::GetSet() const
{
    return this->descriptors[0]->GetSet();
}

VkDescriptorSetLayout Turbo::Core::TDescriptorSetLayout::GetVkDescriptorSetLayout()
{
    return this->vkDescriptorSetLayout;
}

Turbo::Core::TDescriptor::TType Turbo::Core::TDescriptorSetLayout::GetDescriptorType(uint32_t binding) const
{
    for (TDescriptor *dscriptor_item : this->descriptors)
    {
        if (dscriptor_item->GetBinding() == binding)
        {
            return dscriptor_item->GetType();
        }
    }

    throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TDescriptorSetLayout::TDescriptorSetLayout", "not finding the type binding please check the binding index");
}

std::string Turbo::Core::TDescriptorSetLayout::ToString() const
{
    return std::string();
}

bool Turbo::Core::TDescriptorSetLayout::Valid() const
{
    if (this->vkDescriptorSetLayout != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}