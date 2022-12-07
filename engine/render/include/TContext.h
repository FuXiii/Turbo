#pragma once
#ifndef TCONTEXT_H
#define TCONTEXT_H

namespace Turbo
{
namespace Core
{
class TInstance;
class TPhysicalDevice;
class TDevice;
class TDeviceQueue;
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
};

} // namespace Render
} // namespace Turbo

#endif // !TCONTEXT_H