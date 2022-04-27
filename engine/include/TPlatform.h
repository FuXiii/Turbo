#pragma once
#ifndef TPLATFORM_H
#define TPLATFORM_H
#include "TObject.h"

/*
1.ANDROID "vulkan_android.h"
2.FUCHSIA <zircon/types.h> "vulkan_fuchsia.h"
3.IOS_MVK "vulkan_ios.h"
4.MACOS_MVK "vulkan_macos.h"
5.METAL "vulkan_metal.h"
6.VI_NN "vulkan_vi.h"
7.WAYLAND <wayland-client.h> "vulkan_wayland.h"
8.WIN32  <windows.h> "vulkan_win32.h"
9.XCB <xcb/xcb.h> "vulkan_xcb.h"
10.XLIB <X11/Xlib.h> "vulkan_xlib.h"
11.XLIB_XRANDR <X11/Xlib.h> <X11/extensions/Xrandr.h> "vulkan_xlib_xrandr.h"
12.GGP <ggp_c/vulkan_types.h> "vulkan_ggp.h"
13.BETA "vulkan_beta.h"
*/

namespace Turbo
{
	namespace Core
	{
		typedef enum class TPlatformType
		{
			UNDEFINED = 0,
			WINDOWS,
			APPLE,
			UNIX,
			LINUX,
			ANDROID
		}TPlatformTypeEnum;

		class TPlatform :public TObject
		{
		public:
			static TPlatformType GetCurrentPlatform();
			static uint32_t GetCurrentPlatformBits();
		};
	}
}
#endif // !TPLATFORM_H