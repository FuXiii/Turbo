#pragma once
#ifndef TSURFACE_H
#define TSURFACE_H
#include "TObject.h"
#include "TQueueFamilyInfo.h"
#include "TSurfaceFormat.h"

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include "vulkan/vulkan_win32.h"
#endif

namespace Turbo
{
	namespace Core
	{
		class TPhysicalDevice;

		typedef uint32_t TPresentModeFlags;

		class TSurface :public TObject
		{
		private:
			TPhysicalDevice* physicalDevice = nullptr;
			VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;
			std::vector<TQueueFamilyInfo> supportQueueFamilys;

			uint32_t minImageCount;
			uint32_t maxImageCount;
			TExtent2D currentExtent;
			TExtent2D minImageExtent;
			TExtent2D maxImageExtent;
			uint32_t maxImageArrayLayers;

			TSurfaceTransformFlagsKHR supportedTransforms;
			TSurfaceTransformFlagBitsKHR currentTransform;
			TCompositeAlphaFlagsKHR supportedCompositeAlpha;
			TImageUsageFlags supportedUsageFlags;

			std::vector<TSurfaceFormat> surfaceFormats;
			std::vector<TPresentMode> presentModes;

			/*
			{
				IMMEDIATE=0x001,
				MAILBOX=0x002,
				FIFO=0x004,
				FIFO_RELAXED=0x008
			}
			*/
			TPresentModeFlags presentModeFlags;

		private:
			void GetSurfaceSupportQueueFamilys();
			void GetSurfaceCapabilities();
			void GetSurfaceSupportSurfaceFormats();
			void GetSurfaceSupportPresentationMode();

		public:
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
			explicit TSurface(TPhysicalDevice* physicalDevice, HINSTANCE hinstance, HWND hwnd);
#elif defined(__APPLE__)
			explicit TSurface();
#elif defined(ANDROID) || defined(__ANDROID__) 
			explicit TSurface();
#elif defined(__linux) || defined(__linux__)
			explicit TSurface();
#elif defined(__unix) || defined(__unix__)
			explicit TSurface();
#else
			explicit TSurface();//��֧�ָ�ƽ̨��ֱ���׳��쳣
#endif
			~TSurface();

		public:
			VkSurfaceKHR GetVkSurfaceKHR();

			uint32_t GetMinImageCount();
			uint32_t GetMaxImageCount();
			TExtent2D GetCurrentExtent();
			TExtent2D GetMinImageExtent();
			TExtent2D GetMaxImageExtent();
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

			std::vector<TSurfaceFormat> GetSurfaceFormats();
			std::vector<TPresentMode> GetPresentModes();

			TSurfaceTransformFlagsKHR GetSupportedTransforms();
			TSurfaceTransformFlagBitsKHR GetCurrentTransform();
			TCompositeAlphaFlagsKHR GetSupportedCompositeAlpha();
			TImageUsageFlags GetSupportedUsageFlags();

			TPhysicalDevice* GetPhysicalDevice();

			// Inherited via TObject
			virtual std::string ToString() override;
		};
	}
}
#endif // !TSURFACE_H