#include "TVendorInfo.h"
#include <sstream>

Turbo::Core::TVendorInfo::TVendorInfo() : Turbo::Core::TInfo()
{
    this->vendorType = TVendorType::UNDEFINED;
    this->vendorID = 0;
}

Turbo::Core::TVendorInfo::TVendorInfo(TVendorType vendorType, uint32_t vendorID) : Turbo::Core::TInfo()
{
    this->vendorID = vendorID;
    this->vendorType = vendorType;
}

Turbo::Core::TVendorInfo::~TVendorInfo()
{
}

Turbo::Core::TVendorType Turbo::Core::TVendorInfo::GetVendorType() const
{
    return this->vendorType;
}

uint32_t Turbo::Core::TVendorInfo::GetVendorID() const
{
    return this->vendorID;
}

std::string Turbo::Core::TVendorInfo::ToString()
{
    std::stringstream ss;

    switch (this->vendorType)
    {
    case Turbo::Core::TVendorType::UNDEFINED:
        ss << "Undefined";
        break;
    case Turbo::Core::TVendorType::VIVANTE:
        ss << "Vivante";
        break;
    case Turbo::Core::TVendorType::VERISILICON:
        ss << "Verisilicon";
        break;
    case Turbo::Core::TVendorType::KAZAN:
        ss << "Kazan";
        break;
    case Turbo::Core::TVendorType::CODEPLAY:
        ss << "Codeplay";
        break;
    case Turbo::Core::TVendorType::MESA:
        ss << "Mesa";
        break;
    case Turbo::Core::TVendorType::NVIDIA:
        ss << "Nvidia";
        break;
    case Turbo::Core::TVendorType::INTEL:
        ss << "Intel";
        break;
    default:
        ss << "Undefined";
        break;
    }

    ss << "(" << this->vendorID << ")";

    return ss.str();
}