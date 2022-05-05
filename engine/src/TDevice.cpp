#include "TDevice.h"
#include "TDeviceQueue.h"
#include "TException.h"
#include "TLayerInfo.h"
#include "TPhysicalDevice.h"
#include "TQueueFamilyInfo.h"
#include "TVmaAllocator.h"
#include "TVulkanAllocator.h"

void Turbo::Core::TDevice::AddChildHandle(TDeviceQueue *deviceQueue)
{
    if (deviceQueue != nullptr)
    {
        this->deviceQueues.push_back(deviceQueue);
    }
}

Turbo::Core::TDeviceQueue *Turbo::Core::TDevice::RemoveChildHandle(TDeviceQueue *deviceQueue)
{
    // Turbo::Core::TDeviceQueue *result = nullptr;
    // uint32_t index = 0;
    // bool is_found = false;
    // uint32_t device_queues_count = this->deviceQueues.size();

    // for (uint32_t device_queue_index = 0; device_queue_index < device_queues_count; device_queue_index++)
    // {
    //     if (this->deviceQueues[device_queue_index] == deviceQueue)
    //     {
    //         result = this->deviceQueues[device_queue_index];
    //         index = device_queue_index;
    //         is_found = true;
    //         break;
    //     }
    // }

    // if (result != nullptr && is_found)
    // {
    //     this->deviceQueues.erase(this->deviceQueues.begin() + index);
    // }

    // device_queues_count = this->deviceQueues.size();

    // // refresh DeviceQueue::index
    // std::vector<Turbo::Core::TQueueFamilyInfo> device_queue_familys = this->GetDeviceQueueFamilyInfos();
    // for (Turbo::Core::TQueueFamilyInfo &queue_family_item : device_queue_familys)
    // {
    //     uint32_t index = 0;
    //     for (uint32_t device_queue_index = 0; device_queue_index < device_queues_count; device_queue_index++)
    //     {
    //         if (this->deviceQueues[device_queue_index]->queueFamily.index == queue_family_item.index)
    //         {
    //             this->deviceQueues[device_queue_index]->index = index;
    //             index = index + 1;
    //         }
    //     }
    // }
    // return result;

    // We don't need to remove it,because queue created by device.we just delete is at ~TDevice()
    return nullptr;
}

void Turbo::Core::TDevice::InternalCreate()
{
    std::vector<Turbo::Core::TQueueFamilyInfo> device_queue_family_infos = this->GetDeviceQueueFamilyInfos();
    uint32_t device_queue_family_info_count = device_queue_family_infos.size();

    if (device_queue_family_info_count <= 0)
    {
        return; // Vulkan Specification:device queue create count must greater than zero.
    }

    std::vector<VkDeviceQueueCreateInfo> vk_device_queue_create_infos;
    vk_device_queue_create_infos.resize(device_queue_family_info_count);
    this->deviceQueuePriorities.resize(device_queue_family_info_count);
    for (uint32_t device_queue_index = 0; device_queue_index < device_queue_family_info_count; device_queue_index++)
    {
        TQueueFamilyInfo &queue_family_info = device_queue_family_infos[device_queue_index];
        uint32_t queue_family_index = queue_family_info.GetIndex();
        uint32_t queue_family_count = this->GetDeviceQueueCountByQueueFamily(queue_family_info);
        this->deviceQueuePriorities[queue_family_index].resize(queue_family_count, 0.0f);

        vk_device_queue_create_infos[device_queue_index].sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        vk_device_queue_create_infos[device_queue_index].pNext = nullptr;
        vk_device_queue_create_infos[device_queue_index].flags = 0;
        vk_device_queue_create_infos[device_queue_index].queueFamilyIndex = queue_family_index;
        vk_device_queue_create_infos[device_queue_index].queueCount = queue_family_count;
        vk_device_queue_create_infos[device_queue_index].pQueuePriorities = this->deviceQueuePriorities[queue_family_index].data();
    }

    size_t enable_layer_count = this->enabledLayers.size();
    std::vector<const char *> enable_layer_names(enable_layer_count);
    for (uint32_t enable_layer_index; enable_layer_index < enable_layer_count; enable_layer_index++)
    {
        enable_layer_names[enable_layer_index] = this->enabledLayers[enable_layer_index].GetName().c_str();
    }

    size_t enable_extension_count = this->enabledExtensions.size();
    std::vector<const char *> enable_extension_names(enable_extension_count);
    for (uint32_t enable_extension_index; enable_extension_index < enable_extension_count; enable_extension_index++)
    {
        enable_extension_names[enable_extension_index] = this->enabledExtensions[enable_extension_index].GetName().c_str();
    }

    VkDeviceCreateInfo vk_device_create_info = {};
    vk_device_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    vk_device_create_info.pNext = nullptr;
    vk_device_create_info.flags = 0;
    vk_device_create_info.queueCreateInfoCount = vk_device_queue_create_infos.size();
    vk_device_create_info.pQueueCreateInfos = vk_device_queue_create_infos.data();
    vk_device_create_info.enabledLayerCount = enable_layer_count;
    vk_device_create_info.ppEnabledLayerNames = enable_layer_names.data();
    vk_device_create_info.enabledExtensionCount = enable_extension_count;
    vk_device_create_info.ppEnabledExtensionNames = enable_extension_names.data();
    vk_device_create_info.pEnabledFeatures = &this->enabledFeatures;

    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = vkCreateDevice(this->physicalDevice->GetVkPhysicalDevice(), &vk_device_create_info, allocator, &this->vkDevice);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED);
    }

    if (this->vmaAllocator != nullptr)
    {
        this->vmaAllocator->InternalCreate();
    }

    for (TDeviceQueue *device_queue_item : this->deviceQueues)
    {
        device_queue_item->InternalCreate();
    }
}

void Turbo::Core::TDevice::InternalDestroy()
{
    for (TDeviceQueue *device_queue_item : this->deviceQueues)
    {
        device_queue_item->InternalDestroy();
    }

    if (this->vmaAllocator != nullptr)
    {
        this->vmaAllocator->InternalDestroy();
    }

    this->deviceQueuePriorities.clear();

    // this->physicalDevice->ResetQueueCountMap();

    VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    if (this->vkDevice != VK_NULL_HANDLE)
    {
        vkDestroyDevice(this->vkDevice, allocator);
        this->vkDevice = VK_NULL_HANDLE;
    }
}

Turbo::Core::TDevice::TDevice(TPhysicalDevice *physicalDevice, std::vector<TLayerInfo> *enabledLayers, std::vector<TExtensionInfo> *enabledExtensions, VkPhysicalDeviceFeatures *enabledFeatures) : Turbo::Core::TVulkanHandle()
{
    /*
    Vulkan Spec::1.2.156
    [VkDeviceQueueCreateInfo]If the protected memory feature is not enabled, the VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT
    bit of flags must not be set
    */

    if (physicalDevice != nullptr && physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE)
    {
        this->physicalDevice = physicalDevice;

        if (enabledLayers != nullptr)
        {
            this->enabledLayers = *enabledLayers;
        }

        if (enabledExtensions != nullptr)
        {
            this->enabledExtensions = *enabledExtensions;
        }

        if (enabledFeatures != nullptr)
        {
            this->enabledFeatures = *enabledFeatures;
        }
        else
        {
            this->enabledFeatures = {0};
        }

        this->InternalCreate();

        this->physicalDevice->AddChildHandle(this);

        // Create TDeviceQueues
        std::vector<TQueueFamilyInfo> queue_family_infos = this->GetPhysicalDevice()->GetQueueFamilys();
        for (TQueueFamilyInfo queue_family_info_item : queue_family_infos)
        {
            new TDeviceQueue(this, queue_family_info_item, 0);
        }

        this->vmaAllocator = new TVmaAllocator(this);
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
    }
}

Turbo::Core::TDevice::~TDevice()
{
    if (this->vkDevice != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(this->vkDevice);

        delete this->vmaAllocator;
        this->vmaAllocator = nullptr;

        this->physicalDevice->RemoveChildHandle(this);

        for (TDeviceQueue *device_queue_item : this->deviceQueues)
        {
            delete device_queue_item;
        }

        this->deviceQueues.clear();

        this->InternalDestroy();
    }
}

VkDevice Turbo::Core::TDevice::GetVkDevice()
{
    return this->vkDevice;
}

Turbo::Core::TPhysicalDevice *Turbo::Core::TDevice::GetPhysicalDevice()
{
    return this->physicalDevice;
}

size_t Turbo::Core::TDevice::GetEnabledLayersCount()
{
    return this->enabledLayers.size();
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TDevice::GetEnabledLayers()
{
    return this->enabledLayers;
}

size_t Turbo::Core::TDevice::GetEnabledExtensionCount()
{
    return this->enabledExtensions.size();
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TDevice::GetEnabledExtensions()
{
    return this->enabledExtensions;
}

bool Turbo::Core::TDevice::IsEnabledExtension(std::string extensionName)
{
    if (!extensionName.empty())
    {
        TExtensionType extension_type = TExtensionInfo::GetExtensionTypeByExtensionName(extensionName);
        size_t enabled_extension_count = this->enabledExtensions.size();
        if (extension_type != TExtensionType::UNDEFINED)
        {
            for (size_t extension_index = 0; extension_index < enabled_extension_count; extension_index++)
            {
                if (this->enabledExtensions[extension_index].GetExtensionType() == extension_type)
                {
                    return true;
                }
            }
        }
        else
        {
            for (size_t extension_index = 0; extension_index < enabled_extension_count; extension_index++)
            {
                if (this->enabledExtensions[extension_index].GetName() == extensionName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TDevice::IsEnabledExtension(TExtensionType extensionType)
{
    return this->IsEnabledExtension(TExtensionInfo::GetExtensionNameByExtensionType(extensionType));
}

VkPhysicalDeviceFeatures Turbo::Core::TDevice::GetEnableDeviceFeatures()
{
    return this->enabledFeatures;
}

uint32_t Turbo::Core::TDevice::GetDeviceQueueCountByQueueFamily(TQueueFamilyInfo queueFamily)
{
    uint32_t count = 0;

    for (auto *item : this->deviceQueues)
    {
        if (item->GetQueueFamily().GetIndex() == queueFamily.GetIndex())
        {
            count = count + 1;
        }
    }

    return count;
}

std::vector<Turbo::Core::TQueueFamilyInfo> Turbo::Core::TDevice::GetDeviceQueueFamilyInfos()
{
    std::vector<Turbo::Core::TQueueFamilyInfo> result;
    for (TDeviceQueue *device_queue_item : this->deviceQueues)
    {
        bool is_found = false;
        for (TQueueFamilyInfo &item : result)
        {
            if (item.GetIndex() == device_queue_item->GetQueueFamily().GetIndex())
            {
                is_found = true;
                break;
            }
        }

        if (!is_found)
        {
            result.push_back(device_queue_item->GetQueueFamily());
        }
    }

    return result;
}

Turbo::Core::TVmaAllocator *Turbo::Core::TDevice::GetVmaAllocator()
{
    return this->vmaAllocator;
}

Turbo::Core::TDeviceQueue *Turbo::Core::TDevice::GetBestGraphicsQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportGraphics() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TDeviceQueue *Turbo::Core::TDevice::GetBestComputeQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportCompute() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TDeviceQueue *Turbo::Core::TDevice::GetBestTransferQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportTransfer() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TDeviceQueue *Turbo::Core::TDevice::GetBestSparseBindingQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportSparse() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

Turbo::Core::TDeviceQueue *Turbo::Core::TDevice::GetBestProtectedQueue()
{
    TDeviceQueue *result = nullptr;
    uint32_t device_queues_size = this->deviceQueues.size();
    uint32_t index = UINT32_MAX;
    uint32_t score = 0;
    for (uint32_t device_queue_index = 0; device_queue_index < device_queues_size; device_queue_index++)
    {
        TQueueFamilyInfo queue_family_info = this->deviceQueues[device_queue_index]->queueFamily;
        if (queue_family_info.IsSupportProtected() && queue_family_info.score > score)
        {
            index = device_queue_index;
            score = score;
        }
    }

    if (index != UINT32_MAX)
    {
        result = this->deviceQueues[index];
    }

    return result;
}

void Turbo::Core::TDevice::WaitIdle()
{
    VkResult result = vkDeviceWaitIdle(this->vkDevice);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::FAIL);
    }
}

std::string Turbo::Core::TDevice::ToString()
{
    return std::string();
}
