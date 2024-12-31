#include "TPipelineLayout.h"
#include "TDescriptor.h"
#include "TDescriptorSetLayout.h"
#include "TDevice.h"
#include "TException.h"
#include "TShader.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

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