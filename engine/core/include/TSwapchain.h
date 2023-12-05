#pragma once
#ifndef TURBO_CORE_TSWAPCHAIN_H
#define TURBO_CORE_TSWAPCHAIN_H
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
    T_VULKAN_HANDLE_PARENT Turbo::Core::TRefPtr<TSurface> surface = nullptr;
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

    Turbo::Core::TRefPtr<TSwapchain> oldSwapchain = nullptr;

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImage>> images;

    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;

  private:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    [[deprecated]] explicit TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped);
    [[deprecated]] explicit TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped);

    explicit TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatType formatType, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped);
    explicit TSwapchain(const Turbo::Core::TRefPtr<TSurface> &surface, uint32_t minImageCount, Turbo::Core::TFormatType formatType, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped);

    explicit TSwapchain(const Turbo::Core::TRefPtr<TSwapchain> &oldSwapchain);

  protected:
    virtual ~TSwapchain();

  public:
    const Turbo::Core::TRefPtr<TSurface> &GetSurface();
    const std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImage>> &GetImages();

    Turbo::Core::TResult AcquireNextImage(uint64_t timeout, const Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> &signalSemphore, const Turbo::Core::TRefPtr<Turbo::Core::TFence> &signalFence, uint32_t *index);
    Turbo::Core::TResult AcquireNextImageUntil(const Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> &signalSemphore, const Turbo::Core::TRefPtr<Turbo::Core::TFence> &signalFence, uint32_t *index);

    VkSwapchainKHR GetVkSwapchainKHR();

    uint32_t GetMinImageCount() const;
    Turbo::Core::TFormatInfo GetFormat() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetImageArrayLayers() const;
    Turbo::Core::TImageUsages GetUsages() const;
    TSurfaceTransformBits GetTransform() const;
    TCompositeAlphaBits GetCompositeAlpha() const;
    TPresentMode GetPresentMode() const;
    bool GetIsClipped() const;

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Extension
} // namespace Turbo
#endif // !TURBO_CORE_TSWAPCHAIN_H