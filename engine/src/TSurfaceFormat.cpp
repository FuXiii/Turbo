#include "TSurfaceFormat.h"

Turbo::Core::TSurfaceFormat::TSurfaceFormat()
{
	this->format = TFormatInfo(TFormatType::UNDEFINED);
	this->colorSpace = TColorSpace(TColorSpaceType::UNDEFINED);
}

Turbo::Core::TSurfaceFormat::TSurfaceFormat(TFormatInfo format, TColorSpace colorSpace)
{
	this->format = format;
	this->colorSpace = colorSpace;
}

Turbo::Core::TSurfaceFormat::~TSurfaceFormat()
{
}

Turbo::Core::TFormatInfo Turbo::Core::TSurfaceFormat::GetFormat()
{
	return this->format;
}

Turbo::Core::TColorSpace Turbo::Core::TSurfaceFormat::GetColorSpace()
{
	return this->colorSpace;
}

std::string Turbo::Core::TSurfaceFormat::ToString()
{
	return std::string();
}
