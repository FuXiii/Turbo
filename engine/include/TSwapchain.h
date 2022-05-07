#pragma once
#ifndef TSWAPCHAIN_H
#define TSWAPCHAIN_H
#include "TObject.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TSurface;

class TSwapchain : public TObject
{
  private:
    TDevice *device = nullptr;
    VkSwapchainKHR vkSwapchainKHR = VK_NULL_HANDLE;

  public:
    explicit TSwapchain(TDevice *device, TSurface *surface);
    ~TSwapchain();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TSWAPCHAIN_H