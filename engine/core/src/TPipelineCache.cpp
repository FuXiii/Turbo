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

Turbo::Core::TPipelineCache::TPipelineCache(const TRefPtr<TDevice> &device)
{
    if (device.Valid())
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

Turbo::Core::TPipelineCache::TPipelineCache(const TRefPtr<TDevice> &device, size_t size, void *data)
{
    if (device.Valid())
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
    if (result != VkResult::VK_SUCCESS)
    {
        if (result != VkResult::VK_INCOMPLETE)
        {
            throw Turbo::Core::TException(TResult::FAIL, "Turbo::Core::TPipelineCache::GetData");
        }
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

uint32_t Turbo::Core::TPipelineCache::GetHeaderSize()
{
    VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
    // this->GetData(sizeof(VkPipelineCacheHeaderVersionOne), &vk_pipeline_cache_header_version_one);// FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    {
        size_t cahce_size = this->GetSize();

        if (cahce_size < sizeof(VkPipelineCacheHeaderVersionOne))
        {
            return 0;
        }

        void *cahce_data = malloc(cahce_size);
        this->GetData(cahce_size, cahce_data);
        memcpy(&vk_pipeline_cache_header_version_one, cahce_data, sizeof(VkPipelineCacheHeaderVersionOne));
        free(cahce_data);
    }

    return vk_pipeline_cache_header_version_one.headerSize;
}

Turbo::Core::TPipelineCacheHeaderVersion Turbo::Core::TPipelineCache::GetHeaderVersion()
{
    VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
    // this->GetData(sizeof(VkPipelineCacheHeaderVersionOne), &vk_pipeline_cache_header_version_one);// FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中

    {
        size_t cahce_size = this->GetSize();

        if (cahce_size < sizeof(VkPipelineCacheHeaderVersionOne))
        {
            return TPipelineCacheHeaderVersion::ONE;
        }

        void *cahce_data = malloc(cahce_size);
        this->GetData(cahce_size, cahce_data);
        memcpy(&vk_pipeline_cache_header_version_one, cahce_data, sizeof(VkPipelineCacheHeaderVersionOne));
        free(cahce_data);
    }

    switch (vk_pipeline_cache_header_version_one.headerVersion)
    {
    case VK_PIPELINE_CACHE_HEADER_VERSION_ONE: {
        return TPipelineCacheHeaderVersion::ONE;
    }
    break;
    default: {
        return TPipelineCacheHeaderVersion::ONE;
    }
    break;
    }
    return TPipelineCacheHeaderVersion::ONE;
}

Turbo::Core::TVendorInfo Turbo::Core::TPipelineCache::GetVendor()
{
    VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
    // this->GetData(sizeof(VkPipelineCacheHeaderVersionOne), &vk_pipeline_cache_header_version_one);// FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    {
        size_t cahce_size = this->GetSize();

        if (cahce_size < sizeof(VkPipelineCacheHeaderVersionOne))
        {
            return Turbo::Core::TVendorInfo();
        }

        void *cahce_data = malloc(cahce_size);
        this->GetData(cahce_size, cahce_data);
        memcpy(&vk_pipeline_cache_header_version_one, cahce_data, sizeof(VkPipelineCacheHeaderVersionOne));
        free(cahce_data);
    }

    uint32_t vendor_id = vk_pipeline_cache_header_version_one.vendorID;

    TVendorType vendor_type = static_cast<TVendorType>(vendor_id);
    switch (vendor_type)
    {
    case Turbo::Core::TVendorType::UNDEFINED:
        vendor_type = TVendorType::UNDEFINED;
        break;
    case Turbo::Core::TVendorType::VIVANTE:
        vendor_type = TVendorType::VIVANTE;
        break;
    case Turbo::Core::TVendorType::VERISILICON:
        vendor_type = TVendorType::VERISILICON;
        break;
    case Turbo::Core::TVendorType::KAZAN:
        vendor_type = TVendorType::KAZAN;
        break;
    case Turbo::Core::TVendorType::CODEPLAY:
        vendor_type = TVendorType::CODEPLAY;
        break;
    case Turbo::Core::TVendorType::MESA:
        vendor_type = TVendorType::MESA;
        break;
    case Turbo::Core::TVendorType::NVIDIA:
        vendor_type = TVendorType::NVIDIA;
        break;
    case Turbo::Core::TVendorType::INTEL:
        vendor_type = TVendorType::INTEL;
        break;
    default:
        vendor_type = TVendorType::UNDEFINED;
        break;
    }

    return TVendorInfo(vendor_type, vendor_id);
}

uint32_t Turbo::Core::TPipelineCache::GetDeviceID()
{
    VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
    // this->GetData(sizeof(VkPipelineCacheHeaderVersionOne), &vk_pipeline_cache_header_version_one);// FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    {
        size_t cahce_size = this->GetSize();

        if (cahce_size < sizeof(VkPipelineCacheHeaderVersionOne))
        {
            return 0;
        }

        void *cahce_data = malloc(cahce_size);
        this->GetData(cahce_size, cahce_data);
        memcpy(&vk_pipeline_cache_header_version_one, cahce_data, sizeof(VkPipelineCacheHeaderVersionOne));
        free(cahce_data);
    }

    return vk_pipeline_cache_header_version_one.deviceID;
}

std::vector<uint8_t> Turbo::Core::TPipelineCache::GetUUID()
{
    VkPipelineCacheHeaderVersionOne vk_pipeline_cache_header_version_one = {};
    // this->GetData(sizeof(VkPipelineCacheHeaderVersionOne), &vk_pipeline_cache_header_version_one);// FIXME:出现了与Vulkan标准不符的行为，初步怀疑是NVIDIA驱动Bug，与NVIDIA沟通中
    {
        size_t cahce_size = this->GetSize();

        if (cahce_size < sizeof(VkPipelineCacheHeaderVersionOne))
        {
            return std::vector<uint8_t>(VK_UUID_SIZE, 0);
        }

        void *cahce_data = malloc(cahce_size);
        this->GetData(cahce_size, cahce_data);
        memcpy(&vk_pipeline_cache_header_version_one, cahce_data, sizeof(VkPipelineCacheHeaderVersionOne));
        free(cahce_data);
    }

    std::vector<uint8_t> cache_uuid;
    for (uint32_t cache_uuid_index = 0; cache_uuid_index < VK_UUID_SIZE; cache_uuid_index++)
    {
        cache_uuid.push_back(vk_pipeline_cache_header_version_one.pipelineCacheUUID[cache_uuid_index]);
    }
    return cache_uuid;
}

std::string Turbo::Core::TPipelineCache::ToString()
{
    return std::string();
}