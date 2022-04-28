#include "TSubpass.h"

Turbo::Core::TSubpass::TSubpass() : Turbo::Core::TInfo()
{
    this->colors = new std::vector<VkAttachmentReference>();
    this->resolves = new std::vector<VkAttachmentReference>();
    this->inputs = new std::vector<VkAttachmentReference>();
    this->preserves = new std::vector<VkAttachmentReference>();
    this->depthStencil = new VkAttachmentReference();
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

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetColorAttachmentReferences()
{
    return this->colors;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetResolveAttachmentReferences()
{
    return this->resolves;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetInputAttachmentReferences()
{
    return this->inputs;
}

std::vector<VkAttachmentReference> *Turbo::Core::TSubpass::GetPreserveAttachmentReferences()
{
    return this->preserves;
}

VkAttachmentReference *Turbo::Core::TSubpass::GetDepthStencilAttachmentReference()
{
    return this->depthStencil;
}

void Turbo::Core::TSubpass::Clear()
{
    this->colors->clear();
    this->resolves->clear();
    this->inputs->clear();
    this->preserves->clear();
}

std::string Turbo::Core::TSubpass::ToString()
{
    return std::string();
}