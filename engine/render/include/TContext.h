#pragma once
#ifndef TURBO_RENDER_TCONTEXT_H
#define TURBO_RENDER_TCONTEXT_H
#include <stdint.h>

namespace Turbo
{
namespace Core
{
class TInstance;
class TPhysicalDevice;
class TDevice;
class TDeviceQueue;
class TImage;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Render
{

class TContext
{
  private:
    Turbo::Core::TInstance *instance = nullptr;
    Turbo::Core::TPhysicalDevice *physicalDevice = nullptr;
    Turbo::Core::TDevice *device = nullptr;
    Turbo::Core::TDeviceQueue *graphicsQueue = nullptr;

  public:
    TContext();
    ~TContext();

    Turbo::Core::TImage *CreateImage(uint32_t width, uint32_t height,uint32_t depth,uint32_t layer/*, flags,TImageType*/);
    //Turbo::Core::TImage *CreateCubeImage(uint32_t width, uint32_t height,uint32_t depth,uint32_t layer/*, flags,TImage*/);
};

} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TCONTEXT_H