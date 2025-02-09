#include "TVersion.h"
#include <sstream>

Turbo::Core::TVersion Turbo::Core::TVersion::TurboVersion(0, 0, 0, 16);

Turbo::Core::TVersion::TVersion() : Turbo::Core::TInfo()
{
    this->major = 0;
    this->minor = 0;
    this->patch = 0;
    this->develop = 0;
    this->variant = 0;
}

Turbo::Core::TVersion::TVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t develop, uint32_t variant) : Turbo::Core::TInfo()
{
    this->major = major;
    this->minor = minor;
    this->patch = patch;
    this->develop = develop;
    this->variant = variant;
}

Turbo::Core::TVersion::TVersion(const TVersion &version) : Turbo::Core::TInfo()
{
    if (this != &version)
    {
        this->major = version.major;
        this->minor = version.minor;
        this->patch = version.patch;
        this->develop = version.develop;
        this->variant = version.variant;
    }
}

Turbo::Core::TVersion::~TVersion()
{
}

uint32_t Turbo::Core::TVersion::GetVulkanVersion() const
{
    return VK_MAKE_API_VERSION(0, this->major, this->minor, this->patch);
}

Turbo::Core::TVersion Turbo::Core::TVersion::GetValidVulkanVersion() const
{
    return TVersion(this->major, this->minor, 0, 0, 0);
}

uint32_t Turbo::Core::TVersion::GetMajor() const
{
    return this->major;
}

uint32_t Turbo::Core::TVersion::GetMinor() const
{
    return this->minor;
}

uint32_t Turbo::Core::TVersion::GetPatch() const
{
    return this->patch;
}

uint32_t Turbo::Core::TVersion::GetDevelop() const
{
    return this->develop;
}

uint32_t Turbo::Core::TVersion::GetVariant() const
{
    return this->variant;
}

Turbo::Core::TVersion &Turbo::Core::TVersion::operator=(const TVersion &version)
{
    if (this != &version)
    {
        this->major = version.major;
        this->minor = version.minor;
        this->patch = version.patch;
        this->develop = version.develop;
    }
    return *this;
}

bool Turbo::Core::TVersion::operator==(const TVersion &version) const
{
    return this->major == version.major && this->minor == version.minor && this->patch == version.patch && this->develop == version.develop;
}

bool Turbo::Core::TVersion::operator!=(const TVersion &version) const
{
    return this->major != version.major || this->minor != version.minor || this->patch != version.patch || this->develop != version.develop;
}

bool Turbo::Core::TVersion::operator>(const TVersion &version) const
{
    if (this->major > version.major)
    {
        return true;
    }
    else
    {
        if (this->major == version.major)
        {
            if (this->minor > version.minor)
            {
                return true;
            }
            else
            {
                if (this->minor == version.minor)
                {
                    if (this->patch > version.patch)
                    {
                        return true;
                    }
                    else
                    {
                        if (this->patch == version.patch)
                        {
                            if (this->develop > version.develop)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TVersion::operator<(const TVersion &version) const
{
    if (this->major < version.major)
    {
        return true;
    }
    else
    {
        if (this->major == version.major)
        {
            if (this->minor < version.minor)
            {
                return true;
            }
            else
            {
                if (this->minor == version.minor)
                {
                    if (this->patch < version.patch)
                    {
                        return true;
                    }
                    else
                    {
                        if (this->patch == version.patch)
                        {
                            if (this->develop < version.develop)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TVersion::operator<=(const TVersion &version) const
{
    if (*this < version || *this == version)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TVersion::operator>=(const TVersion &version) const
{
    if (*this > version || *this == version)
    {
        return true;
    }

    return false;
}

std::string Turbo::Core::TVersion::ToString() const
{
    std::stringstream ss;
    ss << this->major << "." << this->minor << "." << this->patch << "." << this->develop;
    return ss.str();
}