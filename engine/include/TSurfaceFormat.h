#pragma once
#ifndef TSURFACEFORMAT_H
#include "TObject.h"
#include "TFormatInfo.h"
#include "TColorSpace.h"

namespace Turbo
{
	namespace Core
	{
		class TSurfaceFormat :public TObject
		{
		public:
			friend class TSurface;

		private:
			TFormatInfo format;
			TColorSpace colorSpace;

		public:
			explicit TSurfaceFormat();
			explicit TSurfaceFormat(TFormatInfo format, TColorSpace colorSpace);
			~TSurfaceFormat();

		public:
			TFormatInfo GetFormat();
			TColorSpace GetColorSpace();

			virtual std::string ToString() override;
		};
	}
}
#endif // !TSURFACEFORMAT_H