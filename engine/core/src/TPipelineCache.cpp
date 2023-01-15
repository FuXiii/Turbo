#include "TPipelineCache.h"
#include "TDevice.h"
#include "TException.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

void Turbo::Core::TPipelineCache::InternalCreate()
{
    VkPipelineCacheCreateInfo vk_pipeline_cache_create_info = {};
    vk_pipeline_cache_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    vk_pipeline_cache_create_info.pNext = nullptr;
    vk_pipeline_cache_create_info.flags = 0;
    vk_pipeline_cache_create_info.initialDataSize = this->size;
    vk_pipeline_cache_create_info.pInitialData = this->data;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = VkResult::VK_ERROR_UNKNOWN;

    result = this->device->GetDeviceDriver()->vkCreatePipelineCache(vk_device, &vk_pipeline_cache_create_info, allocator, &this->vkPipelineCache);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TPipelineCache::::InternalCreate::vkCreatePipelineCache");
    }
}

void Turbo::Core::TPipelineCache::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    device->GetDeviceDriver()->vkDestroyPipelineCache(vk_device, this->vkPipelineCache, allocator);
}

Turbo::Core::TPipelineCache::TPipelineCache(TDevice *device)
{
    if (device != nullptr)
    {
        this->device = device;
        this->size = 0;
        this->data = nullptr;
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineCache::TPipelineCache");
    }
}

Turbo::Core::TPipelineCache::TPipelineCache(TDevice *device, size_t size, void *data)
{
    if (device != nullptr)
    {
        this->device = device;
        this->size = 0;
        this->data = nullptr;
        if (size != 0 && data != nullptr)
        {
            this->size = size;
            this->data = data;
        }
        this->InternalCreate();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TPipelineCache::TPipelineCache");
    }
}

Turbo::Core::TPipelineCache::~TPipelineCache()
{
    this->InternalDestroy();
    this->size = 0;
    this->data = nullptr;
}

VkPipelineCache Turbo::Core::TPipelineCache::GetVkPipelineCache()
{
    return this->vkPipelineCache;
}

size_t Turbo::Core::TPipelineCache::GetSize()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkResult result = VkResult::VK_ERROR_UNKNOWN;
    size_t cache_size;
    result = this->device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, this->vkPipelineCache, &cache_size, nullptr);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::FAIL, "Turbo::Core::TPipelineCache::GetSize");
    }
    return cache_size;
}

Turbo::Core::TResult Turbo::Core::TPipelineCache::GetData(size_t size, void *dst)
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkResult result = VkResult::VK_ERROR_UNKNOWN;
    size_t cache_size = size;
    result = this->device->GetDeviceDriver()->vkGetPipelineCacheData(vk_device, this->vkPipelineCache, &cache_size, dst);
    if (result != VkResult::VK_SUCCESS || result != VkResult::VK_INCOMPLETE)
    {
        throw Turbo::Core::TException(TResult::FAIL, "Turbo::Core::TPipelineCache::GetData");
    }

    switch (result)
    {
    case VkResult::VK_SUCCESS: {
        return TResult::SUCCESS;
    }
    break;
    case VkResult::VK_INCOMPLETE: {
        return TResult::SUCCESS;
    }
    break;
    default: {
    }
    break;
    }

    return TResult::FAIL;
}

std::string Turbo::Core::TPipelineCache::ToString()
{
    return std::string();
}