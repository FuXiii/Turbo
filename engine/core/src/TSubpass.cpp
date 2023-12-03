#include "TSubpass.h"

Turbo::Core::TSubpass::TSubpass(TPipelineType type) : Turbo::Core::TInfo()
{
    this->type = type;

    this->colors = new std::vector<VkAttachmentReference>();
    this->resolves = new std::vector<VkAttachmentReference>();
    this->inputs = new std::vector<VkAttachmentReference>();
    this->preserves = new std::vector<VkAttachmentReference>();
    this->depthStencil = new VkAttachmentReference();

    this->depthStencil->attachment = UINT32_MAX;
    this->depthStencil->layout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
}

Turbo::Core::TSubpass::~TSubpass()
{
    delete this->colors;
    delete this->resolves;
    delete this->inputs;
    delete this->preserves;
    delete this->depthStencil;
}

Turbo::Core::TSubpass::TSubpass(const TSubpass &obj)
{
    this->colors = new std::vector<VkAttachmentReference>();
    this->resolves = new std::vector<VkAttachmentReference>();
    this->inputs = new std::vector<VkAttachmentReference>();
    this->preserves = new std::vector<VkAttachmentReference>();
    this->depthStencil = new VkAttachmentReference();

    *this->colors = *obj.colors;
    *this->resolves = *obj.resolves;
    *this->inputs = *obj.inputs;
    *this->preserves = *obj.preserves;
    *this->depthStencil = *obj.depthStencil;
}

void Turbo::Core::TSubpass::AddColorAttachmentReference(uint32_t attachment, TImageLayout layout)
{
    VkAttachmentReference attachment_reference = {};
    attachment_reference.attachment = attachment;
    attachment_reference.layout = (VkImageLayout)layout;
    this->colors->push_back(attachment_reference);
}

void Turbo::Core::TSubpass::AddResolveAttachmentReference(uint32_t attachment, TImageLayout layout)
{
    VkAttachmentReference attachment_reference = {};
    attachment_reference.attachment = attachment;
    attachment_reference.layout = (VkImageLayout)layout;
    this->resolves->push_back(attachment_reference);
}

void Turbo::Core::TSubpass::AddInputAttachmentReference(uint32_t attachment, TImageLayout layout)
{
    VkAttachmentReference attachment_reference = {};
    attachment_reference.attachment = attachment;
    attachment_reference.layout = (VkImageLayout)layout;
    this->inputs->push_back(attachment_reference);
}

void Turbo::Core::TSubpass::AddPreserveAttachmentReference(uint32_t attachment, TImageLayout layout)
{
    VkAttachmentReference attachment_reference = {};
    attachment_reference.attachment = attachment;
    attachment_reference.layout = (VkImageLayout)layout;
    this->preserves->push_back(attachment_reference);
}

void Turbo::Core::TSubpass::SetDepthStencilAttachmentReference(uint32_t attachment, TImageLayout layout)
{
    this->depthStencil->attachment = attachment;
    this->depthStencil->layout = (VkImageLayout)layout;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetColorAttachmentReferences() const
{
    return this->colors;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetResolveAttachmentReferences() const
{
    return this->resolves;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetInputAttachmentReferences() const
{
    return this->inputs;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetPreserveAttachmentReferences() const
{
    return this->preserves;
}

VkAttachmentReference *Turbo::Core::TSubpass::GetDepthStencilAttachmentReference() const
{
    if (this->depthStencil->attachment != UINT32_MAX)
    {
        return this->depthStencil;
    }

    return nullptr;
}

void Turbo::Core::TSubpass::Clear()
{
    this->colors->clear();
    this->resolves->clear();
    this->inputs->clear();
    this->preserves->clear();
}

Turbo::Core::TPipelineType Turbo::Core::TSubpass::GetPipelineType() const
{
    return this->type;
}

std::string Turbo::Core::TSubpass::ToString()
{
    return std::string();
}