#include "TVmaAllocator.h"
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

// #define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

void Turbo::Core::TVmaAllocator::InternalCreate()
{
    TPhysicalDevice *physical_device = this->device->GetPhysicalDevice();
    TInstance *instance = physical_device->GetInstance();
    Turbo::Core::TVersion vulkan_version = instance->GetVulkanVersion();

    const TDeviceDriver *driver = this->device->GetDeviceDriver();

    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = Turbo::Core::TVulkanLoader::Instance()->LoadGlobalDriver().vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = instance->GetInstanceDriver()->vkGetDeviceProcAddr;
    // vulkanFunctions.vkGetPhysicalDeviceProperties = instance->GetInstanceDriver()->vkGetPhysicalDeviceProperties;

    VmaAllocatorCreateFlags vma_allocator_create_flags = 0;
    if ((physical_device->GetDeviceFeatures().bufferDeviceAddress && this->device->GetEnableDeviceFeatures().bufferDeviceAddress) || (vulkan_version >= Turbo::Core::TVersion(1, 2, 0, 0)))
    {
        vma_allocator_create_flags |= VmaAllocatorCreateFlagBits::VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    }

    VmaAllocatorCreateInfo vma_allocator_create_info = {};
    vma_allocator_create_info.flags = vma_allocator_create_flags;
    vma_allocator_create_info.instance = instance->GetVkInstance();
    vma_allocator_create_info.physicalDevice = physical_device->GetVkPhysicalDevice();
    vma_allocator_create_info.device = this->device->GetVkDevice();
    vma_allocator_create_info.vulkanApiVersion = vulkan_version.GetVulkanVersion();
    vma_allocator_create_info.pAllocationCallbacks = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    vma_allocator_create_info.pVulkanFunctions = &vulkanFunctions;

    VkResult result = vmaCreateAllocator(&vma_allocator_create_info, (VmaAllocator *)this->vmaAllocator);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TVmaAllocator::InternalCreate");
    }
}

void Turbo::Core::TVmaAllocator::InternalDestroy()
{
    VmaAllocator *vma_allocator = (VmaAllocator *)(this->vmaAllocator);
    vmaDestroyAllocator(*vma_allocator);
}

Turbo::Core::TVmaAllocator::TVmaAllocator(TDevice *device) : Turbo::Core::TVulkanHandle()
{
    if (Turbo::Core::TReferenced::Valid(device))
    {
        this->device = device;
        this->vmaAllocator = malloc(sizeof(VmaAllocator));
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TVmaAllocator::TVmaAllocator");
    }
}

Turbo::Core::TVmaAllocator::~TVmaAllocator()
{
    this->InternalDestroy();
    free(this->vmaAllocator);
    this->vmaAllocator = nullptr;
}

void *Turbo::Core::TVmaAllocator::GetVmaAllocator()
{
    return this->vmaAllocator;
}

std::string Turbo::Core::TVmaAllocator::ToString() const
{
    return std::string();
}

bool Turbo::Core::TVmaAllocator::Valid() const
{
    if (this->vmaAllocator != nullptr)
    {
        return true;
    }
    return false;
}