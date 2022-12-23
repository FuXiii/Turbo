#include "TBuffer.h"
#include "TResourceAllocator.h"
#include <core/include/TBuffer.h>
#include <core/include/TException.h>
#include <core/include/TPhysicalDevice.h>

void Turbo::Render::TBuffer::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        this->buffer = resource_allocator->CreateBuffer(descriptor);
    }
}

void Turbo::Render::TBuffer::Destroy(void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        resource_allocator->DestroyBuffer(this->buffer);
    }
}

Turbo::Render::TBufferUsages Turbo::Render::TBuffer::GetUsages()
{
    return this->descriptor.usages;
}

uint64_t Turbo::Render::TBuffer::GetSize()
{
    return this->descriptor.size;
}

Turbo::Render::TDomain Turbo::Render::TBuffer::GetDomain()
{
    return this->descriptor.domain;
}