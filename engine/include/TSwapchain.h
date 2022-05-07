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
}
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

    // <for external VkSurfaceKHR>
    T_VULKAN_HANDLE_PARENT Turbo::Core::TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;
    // <for external VkSurfaceKHR>

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

    std::vector<Turbo::Core::TImage *> images;

  private:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped);
    explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped);
    explicit TSwapchain(Turbo::Core::TDevice *device, VkSurfaceKHR vkSurfaceKHR, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped);
    ~TSwapchain();

  public:
    virtual std::string ToString() override;
};
} // namespace Extension
} // namespace Turbo
#endif // !TSWAPCHAIN_H