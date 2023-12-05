#include "TSurface.h"
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"
#include <cstddef>

Turbo::Extension::TColorSpace::TColorSpace(TColorSpaceType colorSpaceType)
{
    this->colorSpaceType = colorSpaceType;
}

Turbo::Extension::TColorSpace::~TColorSpace()
{
}

Turbo::Extension::TColorSpaceType Turbo::Extension::TColorSpace::GetColorSpaceType() const
{
    return this->colorSpaceType;
}

VkColorSpaceKHR Turbo::Extension::TColorSpace::GetVkColorSpaceKHR() const
{
    return static_cast<VkColorSpaceKHR>(this->colorSpaceType);
}

std::string Turbo::Extension::TColorSpace::ToString() const
{
    switch (this->colorSpaceType)
    {
    case Turbo::Extension::TColorSpaceType::UNDEFINED: {
        return std::string("UNDEFINED");
    }
    break;
    case Turbo::Extension::TColorSpaceType::SRGB_NONLINEAR: {
        return std::string("SRGB_NONLINEAR");
    }
    break;
    default: {
        return std::string("UNDEFINED");
    }
    break;
    }

    return std::string();
}

Turbo::Extension::TSurfaceFormat::TSurfaceFormat()
{
    Turbo::Core::TFormatProperties format_properties = {};
    format_properties.bufferFeatures = 0;
    format_properties.linearTilingFeatures = 0;
    format_properties.optimalTilingFeatures = 0;

    this->format = Turbo::Core::TFormatInfo(Turbo::Core::TFormatType::UNDEFINED, format_properties);
    this->colorSpace = TColorSpace(TColorSpaceType::UNDEFINED);
}

Turbo::Extension::TSurfaceFormat::TSurfaceFormat(Turbo::Core::TFormatInfo format, TColorSpace colorSpace)
{
    this->format = format;
    this->colorSpace = colorSpace;
}

Turbo::Extension::TSurfaceFormat::~TSurfaceFormat()
{
}

Turbo::Core::TFormatInfo Turbo::Extension::TSurfaceFormat::GetFormat() const
{
    return this->format;
}

Turbo::Extension::TColorSpace Turbo::Extension::TSurfaceFormat::GetColorSpace() const
{
    return this->colorSpace;
}

std::string Turbo::Extension::TSurfaceFormat::ToString() const
{
    return std::string();
}

void Turbo::Extension::TSurface::InternalCreate()
{
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = device->GetPhysicalDevice()->GetInstance();
    if (instance.Valid())
    {
        if (!this->isExternalHandle)
        {
#if defined(TURBO_PLATFORM_WINDOWS)
            if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE) && instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE))
            {
                VkWin32SurfaceCreateInfoKHR win32_surface_create_info = {};
                win32_surface_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
                win32_surface_create_info.pNext = nullptr;
                win32_surface_create_info.flags = 0;
                win32_surface_create_info.hinstance = this->hinstance;
                win32_surface_create_info.hwnd = this->hwnd;

                VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
                VkResult result = this->vkCreateWin32SurfaceKHR(instance->GetVkInstance(), &win32_surface_create_info, allocator, &this->vkSurfaceKHR);
                if (result != VK_SUCCESS)
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSurface::InternalCreate::vkCreateWin32SurfaceKHR");
                }
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::InternalCreate", "please enable VK_KHR_SURFACE and VK_KHR_WIN32_SURFACE extensions");
            }
#elif defined(__APPLE__)
#elif defined(TURBO_PLATFORM_ANDROID)
            if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE) && instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_ANDROID_SURFACE))
            {
                VkAndroidSurfaceCreateInfoKHR vk_android_surface_create_info_khr = {};
                vk_android_surface_create_info_khr.sType = VkStructureType::VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
                vk_android_surface_create_info_khr.pNext = nullptr;
                vk_android_surface_create_info_khr.flags = 0;
                vk_android_surface_create_info_khr.window = this->nativeWindow;

                VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
                VkResult result = this->vkCreateAndroidSurfaceKHR(instance->GetVkInstance(), &vk_android_surface_create_info_khr, allocator, &this->vkSurfaceKHR);
                if (result != VK_SUCCESS)
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSurface::InternalCreate::vkCreateAndroidSurfaceKHR");
                }
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::InternalCreate", "please enable VK_KHR_SURFACE and VK_KHR_WIN32_SURFACE extensions");
            }
#elif defined(TURBO_PLATFORM_LINUX)
            if (this->waylandDisplay != nullptr || this->waylandSurface != nullptr)
            {
                VkWaylandSurfaceCreateInfoKHR wayland_surface_create_info = {};
                wayland_surface_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
                wayland_surface_create_info.pNext = nullptr;
                wayland_surface_create_info.flags = 0;
                wayland_surface_create_info.display = this->waylandDisplay;
                wayland_surface_create_info.surface = this->waylandSurface;

                VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
                VkResult result = this->vkCreateWaylandSurfaceKHR(instance->GetVkInstance(), &wayland_surface_create_info, allocator, &this->vkSurfaceKHR);
                if (result != VK_SUCCESS)
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSurface::InternalCreate::vkCreateWaylandSurfaceKHR");
                }
            }
            else if (this->xcbConnection != nullptr)
            {
                VkXcbSurfaceCreateInfoKHR xcb_surface_create_info = {};
                xcb_surface_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
                xcb_surface_create_info.pNext = nullptr;
                xcb_surface_create_info.flags = 0;
                xcb_surface_create_info.connection = this->xcbConnection;
                xcb_surface_create_info.window = this->xcbWindow;

                VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
                VkResult result = this->vkCreateXcbSurfaceKHR(instance->GetVkInstance(), &xcb_surface_create_info, allocator, &this->vkSurfaceKHR);
                if (result != VK_SUCCESS)
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSurface::InternalCreate::vkCreateXcbSurfaceKHR");
                }
            }
            else if (this->xlibDpy != nullptr)
            {
                VkXlibSurfaceCreateInfoKHR xlib_surface_create_info = {};
                xlib_surface_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
                xlib_surface_create_info.pNext = nullptr;
                xlib_surface_create_info.flags = 0;
                xlib_surface_create_info.dpy = this->xlibDpy;
                xlib_surface_create_info.window = this->xlibWindow;

                VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
                VkResult result = this->vkCreateXlibSurfaceKHR(instance->GetVkInstance(), &xlib_surface_create_info, allocator, &this->vkSurfaceKHR);
                if (result != VK_SUCCESS)
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Extension::TSurface::InternalCreate::vkCreateXlibSurfaceKHR");
                }
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::InternalCreate", "The parameter invalid please check wayland, xcb or xlib parameter");
            }
#elif defined(__unix) || defined(__unix__)
#else
#endif
        }

        this->GetSurfaceSupportQueueFamilys();
        this->GetSurfaceCapabilities();
        this->GetSurfaceSupportSurfaceFormats();
        this->GetSurfaceSupportPresentationMode();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::InternalCreate");
    }
}

void Turbo::Extension::TSurface::InternalDestroy()
{
    if (!this->isExternalHandle)
    {
        if (this->device.Valid())
        {
            Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = device->GetPhysicalDevice()->GetInstance();
            VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();

            if (instance.Valid() && this->vkSurfaceKHR != VK_NULL_HANDLE)
            {
                this->vkDestroySurfaceKHR(instance->GetVkInstance(), this->vkSurfaceKHR, allocator);
            }
        }
    }
}

#if defined(TURBO_PLATFORM_WINDOWS)
Turbo::Extension::TSurface::TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &device, HINSTANCE hinstance, HWND hwnd)
{
    if (device.Valid())
    {
        this->isExternalHandle = false;
        this->device = device;
        this->hinstance = hinstance;
        this->hwnd = hwnd;

        Turbo::Core::TInstance *instance = this->device->GetPhysicalDevice()->GetInstance();
        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE))
        {
            this->vkCreateWin32SurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkCreateWin32SurfaceKHR>(instance, "vkCreateWin32SurfaceKHR");
            this->vkGetPhysicalDeviceWin32PresentationSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_win32_surface extension");
        }

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE))
        {
            this->vkDestroySurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
            this->vkGetPhysicalDeviceSurfaceSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            this->vkGetPhysicalDeviceSurfaceFormatsKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            this->vkGetPhysicalDeviceSurfacePresentModesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_surface extension");
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::TSurface");
    }
}
#elif defined(__APPLE__)
Turbo::Extension::TSurface::TSurface(...)
{
}
#elif defined(TURBO_PLATFORM_ANDROID)
Turbo::Extension::TSurface::TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &device, ANativeWindow *window)
{
    if (device.Valid())
    {
        this->isExternalHandle = false;
        this->device = device;
        this->nativeWindow = window;

        Turbo::Core::TInstance *instance = this->device->GetPhysicalDevice()->GetInstance();
        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_ANDROID_SURFACE))
        {
            this->vkCreateAndroidSurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkCreateAndroidSurfaceKHR>(instance, "vkCreateAndroidSurfaceKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_android_surface extension");
        }

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE))
        {
            this->vkDestroySurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
            this->vkGetPhysicalDeviceSurfaceSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            this->vkGetPhysicalDeviceSurfaceFormatsKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            this->vkGetPhysicalDeviceSurfacePresentModesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_surface extension");
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::TSurface");
    }
}
#elif defined(TURBO_PLATFORM_LINUX)
Turbo::Extension::TSurface::TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &device, wl_display *display, wl_surface *surface)
{
    if (device.Valid())
    {
        this->isExternalHandle = false;
        this->device = device;
        this->waylandDisplay = display;
        this->waylandSurface = surface;

        Turbo::Core::TInstance *instance = this->device->GetPhysicalDevice()->GetInstance();

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE))
        {
            this->vkCreateWaylandSurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkCreateWaylandSurfaceKHR>(instance, "vkCreateWaylandSurfaceKHR");
            this->vkGetPhysicalDeviceWaylandPresentationSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(instance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_wayland_surface extension");
        }

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE))
        {
            this->vkDestroySurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
            this->vkGetPhysicalDeviceSurfaceSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            this->vkGetPhysicalDeviceSurfaceFormatsKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            this->vkGetPhysicalDeviceSurfacePresentModesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_surface extension");
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::TSurface");
    }
}

Turbo::Extension::TSurface::TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &device, xcb_connection_t *connection, xcb_window_t window)
{
    if (device.Valid())
    {
        this->isExternalHandle = false;
        this->device = device;
        this->xcbConnection = connection;
        this->xcbWindow = window;

        Turbo::Core::TInstance *instance = this->device->GetPhysicalDevice()->GetInstance();

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE))
        {
            this->vkCreateXcbSurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkCreateXcbSurfaceKHR>(instance, "vkCreateXcbSurfaceKHR");
            this->vkGetPhysicalDeviceXcbPresentationSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_xcb_surface extension");
        }

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE))
        {
            this->vkDestroySurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
            this->vkGetPhysicalDeviceSurfaceSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            this->vkGetPhysicalDeviceSurfaceFormatsKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            this->vkGetPhysicalDeviceSurfacePresentModesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_surface extension");
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::TSurface");
    }
}

Turbo::Extension::TSurface::TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &device, Display *dpy, Window window)
{
    if (device.Valid())
    {
        this->isExternalHandle = false;
        this->device = device;
        this->xlibDpy = dpy;
        this->xlibWindow = window;

        Turbo::Core::TInstance *instance = this->device->GetPhysicalDevice()->GetInstance();

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE))
        {
            this->vkCreateXlibSurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkCreateXlibSurfaceKHR>(instance, "vkCreateXlibSurfaceKHR");
            this->vkGetPhysicalDeviceXlibPresentationSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>(instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_xlib_surface extension");
        }

        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE))
        {
            this->vkDestroySurfaceKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkDestroySurfaceKHR>(instance, "vkDestroySurfaceKHR");
            this->vkGetPhysicalDeviceSurfaceSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            this->vkGetPhysicalDeviceSurfaceFormatsKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            this->vkGetPhysicalDeviceSurfacePresentModesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_surface extension");
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::TSurface");
    }
}

#elif defined(__unix) || defined(__unix__)
Turbo::Extension::TSurface::TSurface(...)
{
}
#else
#endif

Turbo::Extension::TSurface::TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &device, VkSurfaceKHR vkSurfaceKHR)
{
    if (device.Valid() && vkSurfaceKHR != VK_NULL_HANDLE)
    {
        this->isExternalHandle = true;
        this->device = device;
        this->vkSurfaceKHR = vkSurfaceKHR;

        Turbo::Core::TInstance *instance = this->device->GetPhysicalDevice()->GetInstance();
        if (instance->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SURFACE))
        {
            this->vkGetPhysicalDeviceSurfaceSupportKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
            this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
            this->vkGetPhysicalDeviceSurfaceFormatsKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
            this->vkGetPhysicalDeviceSurfacePresentModesKHR = Turbo::Core::TVulkanLoader::Instance()->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::EXTENSION_NOT_PRESENT, "Turbo::Extension::TSurface::TSurface", "Please enable the VK_KHR_surface extension");
        }

        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::TSurface");
    }
}

Turbo::Extension::TSurface::~TSurface()
{
    this->InternalDestroy();
}

void Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys()
{
    Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();
    size_t queue_family_count = physical_device->GetQueueFamilyCount();
    for (size_t queue_family_index = 0; queue_family_index < queue_family_count; queue_family_index++)
    {
        Turbo::Core::TQueueFamilyInfo queue_family = physical_device->GetQueueFamilyByIndex(static_cast<uint32_t>(queue_family_index));

        VkBool32 is_support_surface = VK_FALSE;

        VkResult result = this->vkGetPhysicalDeviceSurfaceSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex(), this->vkSurfaceKHR, &is_support_surface);
        if (result == VK_SUCCESS)
        {
            if (is_support_surface == VK_TRUE)
            {
                if (!this->isExternalHandle)
                {
#if defined(TURBO_PLATFORM_WINDOWS)
                    VkBool32 is_support_win32_presentation = this->vkGetPhysicalDeviceWin32PresentationSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex());
                    if (is_support_win32_presentation == VK_TRUE)
                    {
                        this->supportQueueFamilys.push_back(queue_family);
                    }
                    else
                    {
                        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys", "this device unsupport present this surface to the window, please change another device");
                    }
#elif defined(__APPLE__)
#elif defined(TURBO_PLATFORM_ANDROID)
                    this->supportQueueFamilys.push_back(queue_family);
#elif defined(TURBO_PLATFORM_LINUX)
                    if (this->waylandDisplay != nullptr || this->waylandSurface != nullptr)
                    {
                        VkBool32 is_support_wayland_presentation = this->vkGetPhysicalDeviceWaylandPresentationSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex(), this->waylandDisplay);
                        if (is_support_wayland_presentation == VK_TRUE)
                        {
                            this->supportQueueFamilys.push_back(queue_family);
                        }
                        else
                        {
                            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys", "this device unsupport present this surface to the window, please change another device");
                        }
                    }
                    else if (this->xcbConnection != nullptr)
                    {
                        VkBool32 is_support_xcb_presentation = this->vkGetPhysicalDeviceXcbPresentationSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex(), this->xcbConnection, this->xcbWindow);
                        if (is_support_xcb_presentation == VK_TRUE)
                        {
                            this->supportQueueFamilys.push_back(queue_family);
                        }
                        else
                        {
                            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys", "this device unsupport present this surface to the window, please change another device");
                        }
                    }
                    else if (this->xlibDpy != nullptr)
                    {
                        VkBool32 is_support_xlib_presentation = this->vkGetPhysicalDeviceXlibPresentationSupportKHR(physical_device->GetVkPhysicalDevice(), queue_family.GetIndex(), this->xlibDpy, this->xlibWindow);
                        if (is_support_xlib_presentation == VK_TRUE)
                        {
                            this->supportQueueFamilys.push_back(queue_family);
                        }
                        else
                        {
                            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys", "this device unsupport present this surface to the window, please change another device");
                        }
                    }
                    else
                    {
                        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys", "The parameter invalid please check wayland, xcb or xlib parameter");
                    }

#elif defined(__unix) || defined(__unix__)
#else
#endif
                }
                else
                {
                    this->supportQueueFamilys.push_back(queue_family);
                }
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result), "Turbo::Extension::TSurface::vkGetPhysicalDeviceSurfaceSupportKHR", "this device unsupport this surface please change another device");
        }
    }

    if (this->supportQueueFamilys.size() == 0)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportQueueFamilys", "this device unsupport this surface");
    }
}

void Turbo::Extension::TSurface::GetSurfaceCapabilities() const
{
    Turbo::Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

    VkSurfaceCapabilitiesKHR surface_capanilities;
    VkResult result = this->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_capanilities);
    if (result == VK_SUCCESS)
    {
        this->minImageCount = surface_capanilities.minImageCount;
        this->maxImageCount = surface_capanilities.maxImageCount;
        if (this->maxImageCount < this->minImageCount)
        {
            this->maxImageCount = this->minImageCount;
        }
        this->currentExtent = surface_capanilities.currentExtent;
        this->minImageExtent = surface_capanilities.minImageExtent;
        this->maxImageExtent = surface_capanilities.maxImageExtent;
        this->maxImageArrayLayers = surface_capanilities.maxImageArrayLayers;

        this->supportedTransforms = surface_capanilities.supportedTransforms;
        this->currentTransform = (TSurfaceTransformBits)surface_capanilities.currentTransform;
        this->supportedCompositeAlpha = surface_capanilities.supportedCompositeAlpha;
        this->supportedUsageFlags = surface_capanilities.supportedUsageFlags;
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result), "Turbo::Extension::TSurface::GetSurfaceCapabilities::vkGetPhysicalDeviceSurfaceCapabilitiesKHR", "this device unsupport this surface");
    }
}

void Turbo::Extension::TSurface::GetSurfaceSupportSurfaceFormats()
{
    Turbo::Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

    uint32_t surface_format_count = 0;
    VkResult result = this->vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_format_count, nullptr);
    if (result == VK_SUCCESS && surface_format_count > 0)
    {
        std::vector<VkSurfaceFormatKHR> surface_formats;
        surface_formats.resize(surface_format_count);

        result = this->vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &surface_format_count, surface_formats.data());
        if (result == VK_SUCCESS)
        {
            std::vector<Turbo::Core::TFormatInfo> support_formats = physical_device->GetSupportFormats();
            size_t support_formats_size = support_formats.size();

            for (size_t surface_format_index = 0; surface_format_index < surface_format_count; surface_format_index++)
            {
                if (support_formats_size > 0)
                {
                    for (size_t support_format_index = 0; support_format_index < support_formats_size; support_format_index++)
                    {
                        if (support_formats[support_format_index].GetVkFormat() == surface_formats[surface_format_index].format)
                        {
                            Turbo::Extension::TSurfaceFormat surface_format;
                            surface_format.format = support_formats[support_format_index];
                            surface_format.colorSpace = Turbo::Extension::TColorSpace(static_cast<Turbo::Extension::TColorSpaceType>(surface_formats[surface_format_index].colorSpace));

                            this->surfaceFormats.push_back(surface_format);
                            break;
                        }
                    }
                }
                else
                {
                    throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportSurfaceFormats", "this device unsupport this surface");
                }
            }

            if (this->surfaceFormats.size() == 0)
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportSurfaceFormats", "this device unsupport this surface");
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportSurfaceFormats::vkGetPhysicalDeviceSurfaceFormatsKHR", "this device unsupport this surface");
        }
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportSurfaceFormats::vkGetPhysicalDeviceSurfaceFormatsKHR", "this device unsupport this surface");
    }
}

void Turbo::Extension::TSurface::GetSurfaceSupportPresentationMode()
{
    Turbo::Core::TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();

    uint32_t present_mode_count = 0;
    VkResult result = this->vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &present_mode_count, nullptr);
    if (result == VK_SUCCESS)
    {
        if (present_mode_count > 0)
        {
            std::vector<VkPresentModeKHR> vk_present_mode_khrs;
            vk_present_mode_khrs.resize(present_mode_count);
            result = this->vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device->GetVkPhysicalDevice(), this->vkSurfaceKHR, &present_mode_count, vk_present_mode_khrs.data());
            if (result == VK_SUCCESS)
            {
                for (size_t present_mode_index = 0; present_mode_index < present_mode_count; present_mode_index++)
                {
                    switch (vk_present_mode_khrs[present_mode_index])
                    {
                    case VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR: {
                        this->presentModes.push_back(TPresentMode::IMMEDIATE);
                    }
                    break;
                    case VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR: {
                        this->presentModes.push_back(TPresentMode::MAILBOX);
                    }
                    break;
                    case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR: {
                        this->presentModes.push_back(TPresentMode::FIFO);
                    }
                    break;
                    case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR: {
                        this->presentModes.push_back(TPresentMode::FIFO_RELAXED);
                    }
                    break;
                    default: {
                    }
                    break;
                    }
                }
            }
            else
            {
                throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportPresentationMode::vkGetPhysicalDeviceSurfacePresentModesKHR", "this device unsupport this surface");
            }
        }
        else
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Extension::TSurface::GetSurfaceSupportPresentationMode::vkGetPhysicalDeviceSurfacePresentModesKHR", "this device unsupport this surface");
        }
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::VkResultToTResult(result), "Turbo::Extension::TSurface::GetSurfaceSupportPresentationMode::vkGetPhysicalDeviceSurfacePresentModesKHR", "this device unsupport this surface");
    }
}

VkSurfaceKHR Turbo::Extension::TSurface::GetVkSurfaceKHR()
{
    return this->vkSurfaceKHR;
}

uint32_t Turbo::Extension::TSurface::GetMinImageCount() const
{
    return this->minImageCount;
}

uint32_t Turbo::Extension::TSurface::GetMaxImageCount() const
{
    return this->maxImageCount;
}

Turbo::Core::TExtent2D Turbo::Extension::TSurface::GetCurrentExtent() const
{
    this->GetSurfaceCapabilities();

    return this->currentExtent;
}

uint32_t Turbo::Extension::TSurface::GetCurrentWidth() const
{
    this->GetSurfaceCapabilities();

    return this->currentExtent.width;
}

uint32_t Turbo::Extension::TSurface::GetCurrentHeight() const
{
    this->GetSurfaceCapabilities();

    return this->currentExtent.height;
}

Turbo::Core::TExtent2D Turbo::Extension::TSurface::GetMinImageExtent() const
{
    return this->minImageExtent;
}

uint32_t Turbo::Extension::TSurface::GetMinWidth() const
{
    return this->minImageExtent.width;
}

uint32_t Turbo::Extension::TSurface::GetMinHeight() const
{
    return this->minImageExtent.height;
}

Turbo::Core::TExtent2D Turbo::Extension::TSurface::GetMaxImageExtent() const
{
    return this->maxImageExtent;
}

uint32_t Turbo::Extension::TSurface::GetMaxWidth() const
{
    return this->maxImageExtent.width;
}

uint32_t Turbo::Extension::TSurface::GetMaxHeight() const
{
    return this->maxImageExtent.height;
}

uint32_t Turbo::Extension::TSurface::GetMaxImageArrayLayers() const
{
    return this->maxImageArrayLayers;
}

bool Turbo::Extension::TSurface::IsSupportIdentityTransform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportRotate90Transform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportRotate180Transform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportRotate270Transform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorTransform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorRotate90Transform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorRotate180Transform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportHorizontalMirrorRotate270Transform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportInheritTransform() const
{
    if (this->supportedTransforms & VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaOpaque() const
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaPreMultiplied() const
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaPostMultiplied() const
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportCompositeAlphaInherit() const
{
    if (this->supportedCompositeAlpha & VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
    {
        return true;
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeImmediate() const
{
    for (const TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::IMMEDIATE)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeMailbox() const
{
    for (const TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::MAILBOX)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeFifo() const
{
    for (const TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::FIFO)
        {
            return true;
        }
    }

    return false;
}

bool Turbo::Extension::TSurface::IsSupportPresentModeFifoRelaxed() const
{
    for (const TPresentMode &present_mode_item : this->presentModes)
    {
        if (present_mode_item == TPresentMode::FIFO_RELAXED)
        {
            return true;
        }
    }

    return false;
}

std::vector<Turbo::Extension::TSurfaceFormat> Turbo::Extension::TSurface::GetSurfaceFormats() const
{
    return this->surfaceFormats;
}

std::vector<Turbo::Extension::TPresentMode> Turbo::Extension::TSurface::GetPresentModes() const
{
    return this->presentModes;
}

std::vector<Turbo::Core::TQueueFamilyInfo> Turbo::Extension::TSurface::GetSupportQueueFamilys() const
{
    return this->supportQueueFamilys;
}

Turbo::Extension::TSurfaceTransforms Turbo::Extension::TSurface::GetSupportedTransforms() const
{
    return this->supportedTransforms;
}

Turbo::Extension::TSurfaceTransformBits Turbo::Extension::TSurface::GetCurrentTransform() const
{
    this->GetSurfaceCapabilities();

    return this->currentTransform;
}

Turbo::Extension::TCompositeAlphas Turbo::Extension::TSurface::GetSupportedCompositeAlpha() const
{
    return this->supportedCompositeAlpha;
}

Turbo::Core::TImageUsages Turbo::Extension::TSurface::GetSupportedUsages() const
{
    return this->supportedUsageFlags;
}

const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &Turbo::Extension::TSurface::GetDevice()
{
    return this->device;
}

std::string Turbo::Extension::TSurface::ToString() const
{
    return std::string();
}

bool Turbo::Extension::TSurface::Valid() const
{
    if (this->vkSurfaceKHR != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}