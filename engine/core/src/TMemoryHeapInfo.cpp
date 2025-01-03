#include "TMemoryHeapInfo.h"

Turbo::Core::TMemoryHeapInfo::TMemoryHeapInfo() : Turbo::Core::TInfo()
{
    this->memoryHeapFlags = 0;
    this->size = 0;
}

Turbo::Core::TMemoryHeapInfo::~TMemoryHeapInfo()
{
}

bool Turbo::Core::TMemoryHeapInfo::IsDeviceLocal() const
{
    if (this->memoryHeapFlags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
    {
        return true;
    }
    return false;
}

bool Turbo::Core::TMemoryHeapInfo::IsMultiInstance() const
{
    if (this->memoryHeapFlags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)
    {
        return true;
    }
    return false;
}

Turbo::Core::TDeviceSize Turbo::Core::TMemoryHeapInfo::GetByteSize() const
{
    return this->size;
}

std::string Turbo::Core::TMemoryHeapInfo::ToString()const
{
    return std::string();
}
