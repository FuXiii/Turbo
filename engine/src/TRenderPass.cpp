#include "TRenderPass.h"
#include "TAttachment.h"
#include "TDevice.h"
#include "TException.h"
#include "TSubpass.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TRenderPass::InternalCreate()
{
    std::vector<VkAttachmentDescription> vk_attachment_descriptions;
    for (TAttachment &attachment_item : this->attachments)
    {
        VkAttachmentDescription vk_attachment_description = {};
        vk_attachment_description.flags = 0;
        vk_attachment_description.format = attachment_item.GetVkFormat();
        vk_attachment_description.samples = attachment_item.GetVkSampleCountFlagBits();
        vk_attachment_description.loadOp = attachment_item.GetLoadOp();
        vk_attachment_description.storeOp = attachment_item.GetStoreOp();
        vk_attachment_description.stencilLoadOp = attachment_item.GetStencilLoadOp();
        vk_attachment_description.stencilStoreOp = attachment_item.GetStencilStoreOp();
        vk_attachment_description.initialLayout = attachment_item.GetInitialLayout();
        vk_attachment_description.finalLayout = attachment_item.GetFinalLayout();

        vk_attachment_descriptions.push_back(vk_attachment_description);
    }

    VkPipelineBindPoint vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;

    switch (this->type)
    {
    case TPipelineType::Compute: {
        vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;
    }
    break;
    case TPipelineType::Graphics: {
        vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
    }
    break;
    }

    std::vector<VkSubpassDescription> vk_subpass_descriptions;
    for (TSubpass &subpass_item : this->subpasses)
    {
        VkSubpassDescription vk_subpass_description = {};
        vk_subpass_description.flags = 0;
        vk_subpass_description.pipelineBindPoint = vk_pipeline_bind_point;
        vk_subpass_description.inputAttachmentCount = subpass_item.GetInputAttachmentReferences()->size();
        vk_subpass_description.pInputAttachments = subpass_item.GetInputAttachmentReferences()->data();
        vk_subpass_description.colorAttachmentCount = subpass_item.GetColorAttachmentReferences()->size();
        vk_subpass_description.pColorAttachments = subpass_item.GetColorAttachmentReferences()->data();
        vk_subpass_description.pResolveAttachments = nullptr;
        vk_subpass_description.pDepthStencilAttachment = subpass_item.GetDepthStencilAttachmentReference();
        vk_subpass_description.preserveAttachmentCount = 0;
        vk_subpass_description.pPreserveAttachments = nullptr;

        vk_subpass_descriptions.push_back(vk_subpass_description);
    }

    VkRenderPassCreateInfo vk_render_pass_create_info;
    vk_render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    vk_render_pass_create_info.pNext = nullptr;
    vk_render_pass_create_info.flags = 0;
    vk_render_pass_create_info.attachmentCount = vk_attachment_descriptions.size();
    vk_render_pass_create_info.pAttachments = vk_attachment_descriptions.data();
    vk_render_pass_create_info.subpassCount = vk_subpass_descriptions.size();
    vk_render_pass_create_info.pSubpasses = vk_subpass_descriptions.data();
    vk_render_pass_create_info.dependencyCount = 0;
    vk_render_pass_create_info.pDependencies = nullptr;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateRenderPass(vk_device, &vk_render_pass_create_info, allocator, &this->vkRenderPass);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
    }
}

void Turbo::Core::TRenderPass::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vkDestroyRenderPass(vk_device, this->vkRenderPass, allocator);
}

Turbo::Core::TRenderPass::TRenderPass(TDevice *device, TPipelineType type, std::vector<TAttachment> &attachments, std::vector<TSubpass> &subpasses) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr || subpasses.size() == 0)
    {
        this->device = device;
        this->type = type;
        this->subpasses = subpasses;
        this->attachments = attachments;

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TRenderPass::~TRenderPass()
{
    this->InternalDestroy();
}

VkRenderPass Turbo::Core::TRenderPass::GetVkRenderPass()
{
    return this->vkRenderPass;
}

Turbo::Core::TDevice *Turbo::Core::TRenderPass::GetDevice()
{
    return this->device;
}

const std::vector<Turbo::Core::TAttachment> &Turbo::Core::TRenderPass::GetAttachments()
{
    return this->attachments;
}

const std::vector<Turbo::Core::TSubpass> &Turbo::Core::TRenderPass::GetSubpasses()
{
    return this->subpasses;
}

std::string Turbo::Core::TRenderPass::ToString()
{
    return std::string();
}