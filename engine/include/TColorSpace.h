#pragma once
#ifndef TCOLORSPACE_H
#define TCOLORSPACE_H
#include "TObject.h"

namespace Turbo
{
	namespace Core
	{
		typedef enum class TColorSpaceType
		{
			UNDEFINED = -1,
			SRGB_NONLINEAR = VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		}TColorSpaceTypeEnum;

		class TColorSpace :public TObject
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
	}
}
#endif // !TCOLORSPACE_H