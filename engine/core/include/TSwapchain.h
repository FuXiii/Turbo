#pragma once
#ifndef TSWAPCHAIN_H
#define TSWAPCHAIN_H
#include "TFormatInfo.h"
#include "TSurface.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TSemaphore;
class TFence;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Extension
{
class TSwapchain : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TSurface *surface = nullptr;
    T_VULKAN_HANDLE_HANDLE VkSwapchainKHR vkSwapchainKHR = VK_NULL_HANDLE;

    uint32_t minImageCount;
    Turbo::Core::TFormatInfo format;
    uint32_t width;
    uint32_t height;
    uint32_t imageArrayLayers;
    Turbo::Core::TImageUsages usages;
    TSurfaceTransformBits transform;
    TCompositeAlphaBits compositeAlpha;
    TPresentMode presentMode;
    bool isClipped;

    TSwapchain *oldSwapchain = nullptr;

    std::vector<Turbo::Core::TImage *> images;

  private:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped);
    explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped);
    explicit TSwapchain(TSwapchain *oldSwapchain);
    ~TSwapchain();

    TSurface *GetSurface();
    const std::vector<Turbo::Core::TImage *> &GetImages();

    Turbo::Core::TResult AcquireNextImage(uint64_t timeout, Turbo::Core::TSemaphore *signalSemphore, Turbo::Core::TFence *signalFence, uint32_t *index);
    Turbo::Core::TResult AcquireNextImageUntil(Turbo::Core::TSemaphore *signalSemphore, Turbo::Core::TFence *signalFence, uint32_t *index);

    VkSwapchainKHR GetVkSwapchainKHR();

    uint32_t GetMinImageCount();
    Turbo::Core::TFormatInfo GetFormat();
    uint32_t GetWidth();
    uint32_t GetHeight();
    uint32_t GetImageArrayLayers();
    Turbo::Core::TImageUsages GetUsages();
    TSurfaceTransformBits GetTransform();
    TCompositeAlphaBits GetCompositeAlpha();
    TPresentMode GetPresentMode();
    bool GetIsClipped();

  public:
    virtual std::string ToString() override;
};
} // namespace Extension
} // namespace Turbo
#endif // !TSWAPCHAIN_H