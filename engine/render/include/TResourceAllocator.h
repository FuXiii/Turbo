#pragma once
#ifndef TURBO_RENDER_TRESOURCEALLOCATOR_H
#define TURBO_RENDER_TRESOURCEALLOCATOR_H
#include "TContext.h"
#include "TImage.h"
#include "TSampler.h"

namespace Turbo
{
namespace Core
{
class TImage;
class TBuffer;
class TCommandBufferPool;
class TCommandBuffer;
class TSampler;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Render
{
class TResourceAllocator
{
  private:
    TContext *context = nullptr;

  public:
    TResourceAllocator(TContext *context);
    ~TResourceAllocator();

    TContext *GetContext();

    Turbo::Core::TImage *CreateImage(const Turbo::Render::TImage::Descriptor &descriptor);
    void DestroyImage(Turbo::Core::TImage *image);

    Turbo::Core::TBuffer *CreateBuffer(const Turbo::Render::TBuffer::Descriptor &descriptor);
    void DestroyBuffer(Turbo::Core::TBuffer *buffer);

    Turbo::Core::TCommandBuffer *AllocateCommandBuffer();
    void FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer);

    Turbo::Core::TSampler *CreateSampler(const Turbo::Render::TSampler::Descriptor &descriptor);
    void DestroySampler(Turbo::Core::TSampler *sampler);
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TRESOURCEALLOCATOR_H