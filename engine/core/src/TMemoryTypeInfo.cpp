#include "TMemoryTypeInfo.h"

Turbo::Core::TMemoryTypeInfo::TMemoryTypeInfo() : Turbo::Core::TInfo()
{
}

Turbo::Core::TMemoryTypeInfo::~TMemoryTypeInfo()
{
}

Turbo::Core::TMemoryHeapInfo Turbo::Core::TMemoryTypeInfo::GetMemoryHeap()
{
	return this->memoryHeap;
}

uint32_t Turbo::Core::TMemoryTypeInfo::GetIndex()
{
	return this->index;
}

bool Turbo::Core::TMemoryTypeInfo::IsDeviceLocal()
{
	if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsHostVisible()
{
	if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsHostCoherent()
{
	if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsHostCached()
{
	if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsLazilyAllocated()
{
	if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsProtected()
{
	if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_PROTECTED_BIT)
	{
		return true;
	}

	return false;
}

std::string Turbo::Core::TMemoryTypeInfo::ToString()
{
	return std::string();
}
