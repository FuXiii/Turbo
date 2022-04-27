#include "TAttachment.h"

Turbo::Core::TAttachment::TAttachment(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout) : Turbo::Core::TInfo()
{
    this->format = format;
    this->samples = samples;
    this->loadOp = loadOp;
    this->storeOp = storeOp;
    this->stencilLoadOp = stencilLoadOp;
    this->stencilStoreOp = stencilStoreOp;
    this->initialLayout = initialLayout;
    this->finalLayout = finalLayout;
}

Turbo::Core::TAttachment::~TAttachment()
{
}

VkFormat Turbo::Core::TAttachment::GetVkFormat()
{
    return this->format;
}

VkSampleCountFlagBits Turbo::Core::TAttachment::GetVkSampleCountFlagBits()
{
    return this->samples;
}

VkAttachmentLoadOp Turbo::Core::TAttachment::GetLoadOp()
{
    return this->loadOp;
}

VkAttachmentStoreOp Turbo::Core::TAttachment::GetStoreOp()
{
    return this->storeOp;
}

VkAttachmentLoadOp Turbo::Core::TAttachment::GetStencilLoadOp()
{
    return this->stencilLoadOp;
}

VkAttachmentStoreOp Turbo::Core::TAttachment::GetStencilStoreOp()
{
    return this->stencilStoreOp;
}

VkImageLayout Turbo::Core::TAttachment::GetInitialLayout()
{
    return this->initialLayout;
}

VkImageLayout Turbo::Core::TAttachment::GetFinalLayout()
{
    return this->finalLayout;
}

std::string Turbo::Core::TAttachment::ToString()
{
    return std::string();
}
