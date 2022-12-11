#pragma once
#ifndef TURBO_RENDER_TRESOURCEALLOCATOR_H
#define TURBO_RENDER_TRESOURCEALLOCATOR_H
#include "TContext.h"
#include "TImage.h"

namespace Turbo
{
namespace Core
{
class TImage;
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

    Turbo::Core::TImage *CreateImage(const TImage::Descriptor &descriptor);
    void DestroyImage(Turbo::Core::TImage *image);
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TRESOURCEALLOCATOR_H