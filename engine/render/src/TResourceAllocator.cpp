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

Turbo::Render::TContext *Turbo::Render::TResourceAllocator::GetContext()
{
    return this->context;
}

Turbo::Core::TImage *Turbo::Render::TResourceAllocator::CreateImage(const TImage::Descriptor &descriptor)
{
    return this->context->CreateImage(descriptor);
}

void Turbo::Render::TResourceAllocator::DestroyImage(Turbo::Core::TImage *image)
{
    this->context->DestroyImage(image);
}

Turbo::Core::TBuffer *Turbo::Render::TResourceAllocator::CreateBuffer(const TBuffer::Descriptor &descriptor)
{
    return this->context->CreateBuffer(descriptor);
}

void Turbo::Render::TResourceAllocator::DestroyBuffer(Turbo::Core::TBuffer *buffer)
{
    this->context->DestroyBuffer(buffer);
}

Turbo::Core::TCommandBuffer *Turbo::Render::TResourceAllocator::AllocateCommandBuffer()
{
    return this->context->AllocateCommandBuffer();
}

void Turbo::Render::TResourceAllocator::FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer)
{
    this->context->FreeCommandBuffer(commandBuffer);
}
