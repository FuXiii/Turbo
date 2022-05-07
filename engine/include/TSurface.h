#pragma once
#ifndef TSURFACE_H
#define TSURFACE_H
#include "TQueueFamilyInfo.h"
#include "TSurfaceFormat.h"
#include "TVulkanHandle.h"

#if defined(TURBO_PLATFORM_WINDOWS)
#include <windows.h>

#include <vulkan/vulkan_win32.h>
#elif defined(TURBO_PLATFORM_APPLE)
#elif defined(TURBO_PLATFORM_ANDROID)
#include "vulkan_android.h"
#elif defined(TURBO_PLATFORM_LINUX)
#elif defined(TURBO_PLATFORM_UNIX)
#endif

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

typedef VkFlags TPresentModeFlags;

class TSurface : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT Turbo::Core::TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;

    std::vector<Turbo::Core::TQueueFamilyInfo> supportQueueFamilys;

    uint32_t minImageCount;
    uint32_t maxImageCount;
    Turbo::Core::TExtent2D currentExtent;
    Turbo::Core::TExtent2D minImageExtent;
    Turbo::Core::TExtent2D maxImageExtent;
    uint32_t maxImageArrayLayers;

    Turbo::Core::TSurfaceTransformFlagsKHR supportedTransforms;
    Turbo::Core::TSurfaceTransformFlagBitsKHR currentTransform;
    Turbo::Core::TCompositeAlphaFlagsKHR supportedCompositeAlpha;
    Turbo::Core::TImageUsageFlags supportedUsageFlags;

    std::vector<Turbo::Core::TSurfaceFormat> surfaceFormats;
    std::vector<Turbo::Core::TPresentMode> presentModes;

    /*
    {
        IMMEDIATE=0x001,
        MAILBOX=0x002,
        FIFO=0x004,
        FIFO_RELAXED=0x008
    }
    */
    TPresentModeFlags presentModeFlags;

#if defined(TURBO_PLATFORM_WINDOWS)
    HINSTANCE hinstance;
    HWND hwnd;
#elif defined(__APPLE__)
#elif defined(ANDROID) || defined(__ANDROID__)
#elif defined(__linux) || defined(__linux__)
#elif defined(__unix) || defined(__unix__)
#else
#endif

  private:
    void GetSurfaceSupportQueueFamilys();
    void GetSurfaceCapabilities();
    void GetSurfaceSupportSurfaceFormats();
    void GetSurfaceSupportPresentationMode();

  private:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
#if defined(TURBO_PLATFORM_WINDOWS)
    explicit TSurface(Turbo::Core::TDevice *device, HINSTANCE hinstance, HWND hwnd);
#elif defined(__APPLE__)
    explicit TSurface();
#elif defined(ANDROID) || defined(__ANDROID__)
    explicit TSurface(Turbo::Core::TDevice *device, ANativeWindow *window);
#elif defined(__linux) || defined(__linux__)
    explicit TSurface();
#elif defined(__unix) || defined(__unix__)
    explicit TSurface();
#else
    explicit TSurface(VkSurfaceKHR vkSurfaceKHR);
#endif
    ~TSurface();

  public:
    VkSurfaceKHR GetVkSurfaceKHR();

    uint32_t GetMinImageCount();
    uint32_t GetMaxImageCount();
    Turbo::Core::TExtent2D GetCurrentExtent();
    Turbo::Core::TExtent2D GetMinImageExtent();
    Turbo::Core::TExtent2D GetMaxImageExtent();
    uint32_t GetMaxImageArrayLayers();

    bool IsSupportIdentityTransform();
    bool IsSupportRotate90Transform();
    bool IsSupportRotate180Transform();
    bool IsSupportRotate270Transform();
    bool IsSupportHorizontalMirrorTransform();
    bool IsSupportHorizontalMirrorRotate90Transform();
    bool IsSupportHorizontalMirrorRotate180Transform();
    bool IsSupportHorizontalMirrorRotate270Transform();
    bool IsSupportInheritTransform();

    bool IsSupportCompositeAlphaOpaque();
    bool IsSupportCompositeAlphaPreMultiplied();
    bool IsSupportCompositeAlphaPostMultiplied();
    bool IsSupportCompositeAlphaInherit();

    bool IsSupportPresentModeImmediate();
    bool IsSupportPresentModeMailbox();
    bool IsSupportPresentModeFifo();
    bool IsSupportPresentModeFifoRelaxed();

    std::vector<Turbo::Core::TSurfaceFormat> GetSurfaceFormats();
    std::vector<Turbo::Core::TPresentMode> GetPresentModes();

    Turbo::Core::TSurfaceTransformFlagsKHR GetSupportedTransforms();
    Turbo::Core::TSurfaceTransformFlagBitsKHR GetCurrentTransform();
    Turbo::Core::TCompositeAlphaFlagsKHR GetSupportedCompositeAlpha();
    Turbo::Core::TImageUsageFlags GetSupportedUsageFlags();

    Turbo::Core::TDevice *GetDevice();

    // Inherited via TObject
    virtual std::string ToString() override;
};
} // namespace Extension
} // namespace Turbo
#endif // !TSURFACE_H