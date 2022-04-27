#include "TColorSpace.h"

Turbo::Core::TColorSpace::TColorSpace(TColorSpaceType colorSpaceType)
{
	this->colorSpaceType = colorSpaceType;
}

Turbo::Core::TColorSpace::~TColorSpace()
{

}

Turbo::Core::TColorSpaceType Turbo::Core::TColorSpace::GetColorSpaceType()
{
	return this->colorSpaceType;
}

VkColorSpaceKHR Turbo::Core::TColorSpace::GetVkColorSpaceKHR()
{
	return static_cast<VkColorSpaceKHR>(this->colorSpaceType);
}

std::string Turbo::Core::TColorSpace::ToString()
{
	switch (this->colorSpaceType)
	{
	case Turbo::Core::TColorSpaceType::UNDEFINED:
	{
		return std::string("UNDEFINED");
	}
		break;
	case Turbo::Core::TColorSpaceType::SRGB_NONLINEAR:
	{
		return std::string("SRGB_NONLINEAR");
	}
		break;
	default:
	{
		return std::string("UNDEFINED");
	}
		break;
	}

	return std::string();
}
