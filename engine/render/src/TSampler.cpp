#include "render/include/TSampler.h"
#include "render/include/TResourceAllocator.h"
#include <core/include/TSampler.h>

void Turbo::Render::TSampler::Create(const std::string &name, const Descriptor &descriptor, void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        this->sampler = resource_allocator->CreateSampler(descriptor);
        this->allocator = allocator;
        this->descriptor = descriptor;
    }
}

void Turbo::Render::TSampler::Destroy(void *allocator)
{
    if (allocator != nullptr)
    {
        Turbo::Render::TResourceAllocator *resource_allocator = static_cast<Turbo::Render::TResourceAllocator *>(allocator);
        // FIXME: call [sampler = nullptr] in [resource_allocator->DestroySampler(...)];
        resource_allocator->DestroySampler(this->sampler);
        memset(&this->descriptor, 0, sizeof(this->descriptor));
    }
}

bool Turbo::Render::TSampler::IsValid() const
{
    if (this->sampler != nullptr && this->sampler->GetVkSampler() != VK_NULL_HANDLE)
    {
        return true;
    }

    return false;
}
