#include "TAttachment.h"

Turbo::Core::TAttachment::TAttachment(TFormatInfo format, TSampleCountBits samples, TLoadOp loadOp, TStoreOp storeOp, TLoadOp stencilLoadOp, TStoreOp stencilStoreOp, TImageLayout initialLayout, TImageLayout finalLayout) : Turbo::Core::TInfo()
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

Turbo::Core::TFormatInfo Turbo::Core::TAttachment::GetFormat() const
{
    return this->format;
}

Turbo::Core::TSampleCountBits Turbo::Core::TAttachment::GetVkSampleCountFlagBits() const
{
    return this->samples;
}

Turbo::Core::TLoadOp Turbo::Core::TAttachment::GetLoadOp() const
{
    return this->loadOp;
}

Turbo::Core::TStoreOp Turbo::Core::TAttachment::GetStoreOp() const
{
    return this->storeOp;
}

Turbo::Core::TLoadOp Turbo::Core::TAttachment::GetStencilLoadOp() const
{
    return this->stencilLoadOp;
}

Turbo::Core::TStoreOp Turbo::Core::TAttachment::GetStencilStoreOp() const
{
    return this->stencilStoreOp;
}

Turbo::Core::TImageLayout Turbo::Core::TAttachment::GetInitialLayout() const
{
    return this->initialLayout;
}

Turbo::Core::TImageLayout Turbo::Core::TAttachment::GetFinalLayout() const
{
    return this->finalLayout;
}

std::string Turbo::Core::TAttachment::ToString()
{
    return std::string();
}
