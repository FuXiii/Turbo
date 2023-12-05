#include "TMemoryTypeInfo.h"

Turbo::Core::TMemoryTypeInfo::TMemoryTypeInfo() : Turbo::Core::TInfo()
{
}

Turbo::Core::TMemoryTypeInfo::~TMemoryTypeInfo()
{
}

Turbo::Core::TMemoryHeapInfo Turbo::Core::TMemoryTypeInfo::GetMemoryHeap() const
{
    return this->memoryHeap;
}

uint32_t Turbo::Core::TMemoryTypeInfo::GetIndex() const
{
    return this->index;
}

bool Turbo::Core::TMemoryTypeInfo::IsDeviceLocal() const
{
    if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsHostVisible() const
{
    if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsHostCoherent() const
{
    if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsHostCached() const
{
    if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsLazilyAllocated() const
{
    if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
    {
        return true;
    }

    return false;
}

bool Turbo::Core::TMemoryTypeInfo::IsProtected() const
{
    if (this->memoryPropertyFlags & VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_PROTECTED_BIT)
    {
        return true;
    }

    return false;
}

std::string Turbo::Core::TMemoryTypeInfo::ToString()const
{
    return std::string();
}
