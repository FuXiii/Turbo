#pragma once
#ifndef TSURFACE_H
#define TSURFACE_H
#include "TImage.h"
#include "TInfo.h"
#include "TQueueFamilyInfo.h"
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
    explicit TSurface(...);
#elif defined(ANDROID) || defined(__ANDROID__)
    explicit TSurface(Turbo::Core::TDevice *device, ANativeWindow *window);
#elif defined(__linux) || defined(__linux__)
    explicit TSurface(...);
#elif defined(__unix) || defined(__unix__)
    explicit TSurface(...);
#else
#endif
    // TDevice *device and vkSurfaceKHR should come frome same VkInstance,and make sure you had open the correct extensions
    explicit TSurface(Turbo::Core::TDevice *device, VkSurfaceKHR vkSurfaceKHR);

    ~TSurface();

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
#endif // !TSURFACE_H