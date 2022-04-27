#include "TVersion.h"
#include <sstream>

Turbo::Core::TVersion Turbo::Core::TVersion::TurboVersion(0, 0, 0, 16);

Turbo::Core::TVersion::TVersion() : TObject()
{
	this->major = 0;
	this->minor = 0;
	this->patch = 0;
	this->develop = 0;
}

Turbo::Core::TVersion::TVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t develop) : TObject()
{
	this->major = major;
	this->minor = minor;
	this->patch = patch;
	this->develop = develop;
}

Turbo::Core::TVersion::TVersion(const TVersion &version) : TObject()
{
	if (this != &version)
	{
		this->major = version.major;
		this->minor = version.minor;
		this->patch = version.patch;
		this->develop = version.develop;
	}
}

Turbo::Core::TVersion::~TVersion()
{
}

uint32_t Turbo::Core::TVersion::GetVulkanVersion()
{
	return VK_MAKE_VERSION(this->major, this->minor, this->patch);
}

Turbo::Core::TVersion Turbo::Core::TVersion::GetValidVulkanVersion()
{
	return TVersion(this->major, this->minor, 0, 0);
}

uint32_t Turbo::Core::TVersion::GetMajor()
{
	return this->major;
}

uint32_t Turbo::Core::TVersion::GetMinor()
{
	return this->minor;
}

uint32_t Turbo::Core::TVersion::GetPatch()
{
	return this->patch;
}

uint32_t Turbo::Core::TVersion::GetDevelop()
{
	return this->develop;
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

	if (this->minor > version.minor)
	{
		return true;
	}

	if (this->patch > version.patch)
	{
		return true;
	}

	if (this->develop > version.develop)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TVersion::operator<(const TVersion &version) const
{
	if (this->major < version.major)
	{
		return true;
	}

	if (this->minor < version.minor)
	{
		return true;
	}

	if (this->patch < version.patch)
	{
		return true;
	}

	if (this->develop < version.develop)
	{
		return true;
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

std::string Turbo::Core::TVersion::ToString()
{
	std::stringstream ss;
	ss << this->major << "." << this->minor << "." << this->patch << "." << this->develop;
	return ss.str();
}