#pragma once
#ifndef TURBO_CORE_TSURFACE_H
#define TURBO_CORE_TSURFACE_H
#include "TImage.h"
#include "TInfo.h"
#include "TQueueFamilyInfo.h"
#include "TVulkanHandle.h"

#if defined(TURBO_PLATFORM_WINDOWS)
#include <windows.h>
#include "vulkan/vulkan_win32.h"
#elif defined(TURBO_PLATFORM_APPLE)
#elif defined(TURBO_PLATFORM_ANDROID)
#include "vulkan/vulkan_android.h"
#elif defined(TURBO_PLATFORM_LINUX)
#include <wayland-client.h>
#include "vulkan/vulkan_wayland.h"
#include <xcb/xcb.h>
#include "vulkan/vulkan_xcb.h"
#include <X11/Xlib.h>
#include "vulkan/vulkan_xlib.h"
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
class TSurface;

typedef enum class TColorSpaceType
{
    UNDEFINED = -1,
    SRGB_NONLINEAR = VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
} TColorSpaceTypeEnum;

class TColorSpace : public Turbo::Core::TInfo
{
  public:
    friend class TSurface;

  private:
    TColorSpaceType colorSpaceType;

  public:
    explicit TColorSpace(TColorSpaceType colorSpaceType = TColorSpaceType::SRGB_NONLINEAR);
    ~TColorSpace();

  public:
    TColorSpaceType GetColorSpaceType();
    VkColorSpaceKHR GetVkColorSpaceKHR();

    virtual std::string ToString() override;
};

class TSurfaceFormat : public Turbo::Core::TInfo
{
  public:
    friend class Turbo::Extension::TSurface;

  private:
    Turbo::Core::TFormatInfo format;
    TColorSpace colorSpace;

  public:
    explicit TSurfaceFormat();
    explicit TSurfaceFormat(Turbo::Core::TFormatInfo format, TColorSpace colorSpace);
    ~TSurfaceFormat();

  public:
    Turbo::Core::TFormatInfo GetFormat();
    TColorSpace GetColorSpace();

    virtual std::string ToString() override;
};

typedef enum TSurfaceTransformBits
{
    TRANSFORM_IDENTITY_BIT = 0x00000001,
    TRANSFORM_ROTATE_90_BIT = 0x00000002,
    TRANSFORM_ROTATE_180_BIT = 0x00000004,
    TRANSFORM_ROTATE_270_BIT = 0x00000008,
    TRANSFORM_HORIZONTAL_MIRROR_BIT = 0x00000010,
    TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT = 0x00000020,
    TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT = 0x00000040,
    TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT = 0x00000080,
    TRANSFORM_INHERIT_BIT = 0x00000100,
} TSurfaceTransformBits;
typedef VkFlags TSurfaceTransforms;

typedef enum TCompositeAlphaBits
{
    ALPHA_OPAQUE_BIT = 0x00000001,
    ALPHA_PRE_MULTIPLIED_BIT = 0x00000002,
    ALPHA_POST_MULTIPLIED_BIT = 0x00000004,
    ALPHA_INHERIT_BIT = 0x00000008,
} TCompositeAlphaBits;
typedef VkFlags TCompositeAlphas;

typedef enum class TPresentMode
{
    IMMEDIATE = 0,
    MAILBOX = 1,
    FIFO = 2,
    FIFO_RELAXED = 3,
} TPresentMode;

class TSurface : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT Turbo::Core::TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;

    bool isExternalHandle = false;

    std::vector<Turbo::Core::TQueueFamilyInfo> supportQueueFamilys;

    uint32_t minImageCount;
    uint32_t maxImageCount;
    Turbo::Core::TExtent2D currentExtent;
    Turbo::Core::TExtent2D minImageExtent;
    Turbo::Core::TExtent2D maxImageExtent;
    uint32_t maxImageArrayLayers;

    Turbo::Extension::TSurfaceTransforms supportedTransforms;
    Turbo::Extension::TSurfaceTransformBits currentTransform;
    Turbo::Extension::TCompositeAlphas supportedCompositeAlpha;
    Turbo::Core::TImageUsages supportedUsageFlags;

    std::vector<Turbo::Extension::TSurfaceFormat> surfaceFormats;
    std::vector<Turbo::Extension::TPresentMode> presentModes;

#if defined(TURBO_PLATFORM_WINDOWS)
    HINSTANCE hinstance = nullptr;
    HWND hwnd = nullptr;

    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
#elif defined(__APPLE__)
#elif defined(TURBO_PLATFORM_ANDROID)
    ANativeWindow *nativeWindow;

    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = nullptr;
#elif defined(TURBO_PLATFORM_LINUX)
    // wayland
    struct wl_display *waylandDisplay = nullptr;
    struct wl_surface *waylandSurface = nullptr;

    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR = nullptr;

    // xcb
    xcb_connection_t *xcbConnection = nullptr;
    xcb_window_t xcbWindow;

    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;

    // xlib
    Display *xlibDpy = nullptr;
    Window xlibWindow;

    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR = nullptr;

#elif defined(__unix) || defined(__unix__)
#else
#endif
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;

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
    explicit TSurface(...);
#elif defined(TURBO_PLATFORM_ANDROID)
    explicit TSurface(Turbo::Core::TDevice *device, ANativeWindow *window); // FIXME:VK_KHR_ANDROID_SURFACE
#elif defined(TURBO_PLATFORM_LINUX)
    explicit TSurface(Turbo::Core::TDevice *device, wl_display *display, wl_surface *surface);
    explicit TSurface(Turbo::Core::TDevice *device, xcb_connection_t *connection, xcb_window_t window);
    explicit TSurface(Turbo::Core::TDevice *device, Display *dpy, Window window);
#elif defined(__unix) || defined(__unix__)
    explicit TSurface(...);
#else
#endif
    // TDevice *device and vkSurfaceKHR should come frome same VkInstance,and make sure you had open the correct extensions
    explicit TSurface(Turbo::Core::TDevice *device, VkSurfaceKHR vkSurfaceKHR);

  protected:
    virtual ~TSurface();

  public:
    VkSurfaceKHR GetVkSurfaceKHR();

    uint32_t GetMinImageCount();
    uint32_t GetMaxImageCount();
    Turbo::Core::TExtent2D GetCurrentExtent();
    uint32_t GetCurrentWidth();
    uint32_t GetCurrentHeight();
    Turbo::Core::TExtent2D GetMinImageExtent();
    uint32_t GetMinWidth();
    uint32_t GetMinHeight();
    Turbo::Core::TExtent2D GetMaxImageExtent();
    uint32_t GetMaxWidth();
    uint32_t GetMaxHeight();
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

    std::vector<Turbo::Extension::TSurfaceFormat> GetSurfaceFormats();
    std::vector<Turbo::Extension::TPresentMode> GetPresentModes();

    std::vector<Turbo::Core::TQueueFamilyInfo> GetSupportQueueFamilys();
    Turbo::Extension::TSurfaceTransforms GetSupportedTransforms();
    Turbo::Extension::TSurfaceTransformBits GetCurrentTransform();
    Turbo::Extension::TCompositeAlphas GetSupportedCompositeAlpha();
    Turbo::Core::TImageUsages GetSupportedUsages();

    Turbo::Core::TDevice *GetDevice();

    // Inherited via TObject
    virtual std::string ToString() override;
};
} // namespace Extension
} // namespace Turbo
#endif // !TURBO_CORE_TSURFACE_H