#include "TResourceAllocator.h"
#include <core/include/TException.h>

Turbo::Render::TResourceAllocator::TResourceAllocator(TContext *context)
{
    if (context != nullptr)
    {
        this->context = context;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TResourceAllocator::TResourceAllocator(TContext *context)", "Please use an available context");
    }
}

Turbo::Render::TResourceAllocator::~TResourceAllocator()
{
    this->context = nullptr;
}

Turbo::Core::TImage *Turbo::Render::TResourceAllocator::CreateImage(const TImage::Descriptor &descriptor)
{
    return this->context->CreateImage(descriptor);
}

void Turbo::Render::TResourceAllocator::DestroyImage(Turbo::Core::TImage *image)
{
    this->context->DestroyImage(image);
}