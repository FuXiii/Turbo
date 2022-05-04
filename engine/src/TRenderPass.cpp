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
        vk_attachment_description.format = attachment_item.GetFormat().GetVkFormat();
        vk_attachment_description.samples = (VkSampleCountFlagBits)attachment_item.GetVkSampleCountFlagBits();
        vk_attachment_description.loadOp = (VkAttachmentLoadOp)attachment_item.GetLoadOp();
        vk_attachment_description.storeOp = (VkAttachmentStoreOp)attachment_item.GetStoreOp();
        vk_attachment_description.stencilLoadOp = (VkAttachmentLoadOp)attachment_item.GetStencilLoadOp();
        vk_attachment_description.stencilStoreOp = (VkAttachmentStoreOp)attachment_item.GetStencilStoreOp();
        vk_attachment_description.initialLayout = (VkImageLayout)attachment_item.GetInitialLayout();
        vk_attachment_description.finalLayout = (VkImageLayout)attachment_item.GetFinalLayout();

        vk_attachment_descriptions.push_back(vk_attachment_description);
    }

    uint32_t subpass_index = 0;
    uint32_t subpasses_count = this->subpasses.size();

    std::vector<VkSubpassDependency> vk_subpass_dependencys;

    if (subpasses_count > 0)
    {
        VkSubpassDependency top_subpass_external_dependency = {};
        top_subpass_external_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        top_subpass_external_dependency.dstSubpass = 0;
        top_subpass_external_dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        top_subpass_external_dependency.dstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        top_subpass_external_dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        top_subpass_external_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        top_subpass_external_dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        vk_subpass_dependencys.push_back(top_subpass_external_dependency);
    }

    std::vector<VkSubpassDescription> vk_subpass_descriptions;
    for (TSubpass &subpass_item : this->subpasses)
    {
        VkPipelineBindPoint vk_pipeline_bind_point = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;

        switch (subpass_item.GetPipelineType())
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

        VkSubpassDescription vk_subpass_description = {};
        vk_subpass_description.flags = 0;
        vk_subpass_description.pipelineBindPoint = vk_pipeline_bind_point;
        vk_subpass_description.inputAttachmentCount = subpass_item.GetInputAttachmentReferences()->size();
        vk_subpass_description.pInputAttachments = subpass_item.GetInputAttachmentReferences()->data();
        vk_subpass_description.colorAttachmentCount = subpass_item.GetColorAttachmentReferences()->size();
        vk_subpass_description.pColorAttachments = subpass_item.GetColorAttachmentReferences()->data();
        vk_subpass_description.pResolveAttachments = subpass_item.GetResolveAttachmentReferences()->data();
        vk_subpass_description.pDepthStencilAttachment = subpass_item.GetDepthStencilAttachmentReference();
        vk_subpass_description.preserveAttachmentCount = 0;
        vk_subpass_description.pPreserveAttachments = nullptr;

        vk_subpass_descriptions.push_back(vk_subpass_description);

        if ((subpass_index + 1) <= (subpasses_count - 1))
        {
            VkSubpassDependency vk_subpass_dependency = {};
            vk_subpass_dependency.srcSubpass = subpass_index;
            vk_subpass_dependency.dstSubpass = subpass_index + 1;
            vk_subpass_dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            vk_subpass_dependency.dstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            vk_subpass_dependency.srcAccessMask = 0;
            vk_subpass_dependency.dstAccessMask = 0;
            vk_subpass_dependency.dependencyFlags = VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT;

            vk_subpass_dependencys.push_back(vk_subpass_dependency);
        }

        subpass_index = subpass_index + 1;
    }

    if (subpasses_count > 0)
    {
        VkSubpassDependency end_subpass_external_dependency = {};
        end_subpass_external_dependency.srcSubpass = this->subpasses.size() - 1;
        end_subpass_external_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
        end_subpass_external_dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
        end_subpass_external_dependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        end_subpass_external_dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        end_subpass_external_dependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        end_subpass_external_dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        vk_subpass_dependencys.push_back(end_subpass_external_dependency);
    }

    VkRenderPassCreateInfo vk_render_pass_create_info;
    vk_render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    vk_render_pass_create_info.pNext = nullptr;
    vk_render_pass_create_info.flags = 0;
    vk_render_pass_create_info.attachmentCount = vk_attachment_descriptions.size();
    vk_render_pass_create_info.pAttachments = vk_attachment_descriptions.data();
    vk_render_pass_create_info.subpassCount = vk_subpass_descriptions.size();
    vk_render_pass_create_info.pSubpasses = vk_subpass_descriptions.data();
    vk_render_pass_create_info.dependencyCount = vk_subpass_dependencys.size();
    vk_render_pass_create_info.pDependencies = vk_subpass_dependencys.data();

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

Turbo::Core::TRenderPass::TRenderPass(TDevice *device, std::vector<TAttachment> &attachments, std::vector<TSubpass> &subpasses) : Turbo::Core::TVulkanHandle()
{
    if (device != nullptr || subpasses.size() == 0)
    {
        this->device = device;
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