#include "TFramebuffer.h"
#include "TAttachment.h"
#include "TDevice.h"
#include "TException.h"
#include "TImage.h"
#include "TImageView.h"
#include "TRenderPass.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TFramebuffer::InternalCreate()
{
    std::vector<VkImageView> image_view_attachments;
    for (TImageView *attachemt_item : this->attachments)
    {
        image_view_attachments.push_back(attachemt_item->GetVkImageView());
    }

    VkFramebufferCreateInfo vk_framebuffer_create_info = {};
    vk_framebuffer_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    vk_framebuffer_create_info.pNext = nullptr;
    vk_framebuffer_create_info.flags = 0;
    vk_framebuffer_create_info.renderPass = this->renderPass->GetVkRenderPass();
    vk_framebuffer_create_info.attachmentCount = image_view_attachments.size();
    vk_framebuffer_create_info.pAttachments = image_view_attachments.data();
    vk_framebuffer_create_info.width = this->attachments[0]->GetImage()->GetWidth();
    vk_framebuffer_create_info.height = this->attachments[0]->GetImage()->GetHeight();
    vk_framebuffer_create_info.layers = 1;

    this->width = vk_framebuffer_create_info.width;
    this->height = vk_framebuffer_create_info.height;

    TDevice *device = this->renderPass->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = device->GetDeviceDriver()->vkCreateFramebuffer(vk_device, &vk_framebuffer_create_info, allocator, &this->vkFramebuffer);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TFramebuffer::InternalCreate::vkCreateFramebuffer");
    }
}

void Turbo::Core::TFramebuffer::InternalDestroy()
{
    TDevice *device = this->renderPass->GetDevice();
    VkDevice vk_device = device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    device->GetDeviceDriver()->vkDestroyFramebuffer(vk_device, this->vkFramebuffer, allocator);
}

Turbo::Core::TFramebuffer::TFramebuffer(const TRefPtr<TRenderPass> &renderPass, std::vector<TRefPtr<TImageView>> &attachments) : Turbo::Core::TVulkanHandle()
{
    // OLD: if (renderPass != nullptr && attachments.size() > 0)
    if (renderPass.Valid())
    {
        this->renderPass = renderPass;
        this->attachments = attachments;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TFramebuffer::TFramebuffer");
    }
}

Turbo::Core::TFramebuffer::~TFramebuffer()
{
    this->InternalDestroy();
}

VkFramebuffer Turbo::Core::TFramebuffer::GetVkFramebuffer()
{
    return this->vkFramebuffer;
}

uint32_t Turbo::Core::TFramebuffer::GetWidth() const
{
    return this->width;
}

uint32_t Turbo::Core::TFramebuffer::GetHeight() const
{
    return this->height;
}

const Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> &Turbo::Core::TFramebuffer::GetRenderPass()
{
    return this->renderPass;
}

const std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> &Turbo::Core::TFramebuffer::GetAttachments()
{
    return this->attachments;
}

std::string Turbo::Core::TFramebuffer::ToString()
{
    return std::string();
}
