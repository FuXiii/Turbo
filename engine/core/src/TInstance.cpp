#include "TInstance.h"
#include "TDevice.h"
#include "TException.h"
#include "TPhysicalDevice.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

size_t Turbo::Core::TInstance::GetEnabledLayerCount() const
{
    return this->enabledLayers.size();
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TInstance::GetEnabledLayers() const
{
    return this->enabledLayers;
}

bool Turbo::Core::TInstance::IsEnabledLayer(std::string layerName) const
{
    if (!layerName.empty())
    {
        TLayerType layer_type = TLayerInfo::GetLayerTypeByLayerName(layerName);
        size_t enabled_layers_count = this->enabledLayers.size();
        if (layer_type != TLayerType::UNDEFINED)
        {
            for (size_t layer_index = 0; layer_index < enabled_layers_count; layer_index++)
            {
                if (this->enabledLayers[layer_index].GetLayerType() == layer_type)
                {
                    return true;
                }
            }
        }
        else
        {
            for (size_t layer_index = 0; layer_index < enabled_layers_count; layer_index++)
            {
                if (this->enabledLayers[layer_index].GetName() == layerName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TInstance::IsEnabledLayer(TLayerType layerType) const
{
    return this->IsEnabledLayer(TLayerInfo::GetLayerNameByLayerType(layerType));
}

size_t Turbo::Core::TInstance::GetEnabledExtensionCount() const
{
    return this->enabledExtensions.size();
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TInstance::GetEnabledExtensions() const
{
    return this->enabledExtensions;
}

bool Turbo::Core::TInstance::IsEnabledExtension(std::string extensionName) const
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

bool Turbo::Core::TInstance::IsEnabledExtension(TExtensionType extensionType) const
{
    return this->IsEnabledExtension(TExtensionInfo::GetExtensionNameByExtensionType(extensionType));
}

std::string Turbo::Core::TInstance::ToString() const
{
    return std::string();
}

bool Turbo::Core::TInstance::Valid() const
{
    if (this->vkInstance != VK_NULL_HANDLE)
    {
        return true;
    }
    return false;
}

Turbo::Core::TInstance::TInstance(std::vector<TLayerInfo> *enabledLayers, std::vector<TExtensionInfo> *enabledExtensions, TVersion *vulkanVersion) : Turbo::Core::TVulkanHandle()
{
    this->vkInstance = VK_NULL_HANDLE;

    VkResult result = this->CreateVkInstance(enabledLayers, enabledExtensions, vulkanVersion);
    if (result != VkResult::VK_SUCCESS)
    {
        this->vulkanVersion = TVersion(0, 0, 0, 0);
        throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Core::TInstance::TInstance");
    }
}

Turbo::Core::TInstance::~TInstance()
{
    for (TPhysicalDevice *physical_device : this->physicalDevices)
    {
        delete physical_device;
    }

    this->physicalDevices.clear();

    this->InternalDestroy();
}

VkInstance Turbo::Core::TInstance::GetVkInstance()
{
    return this->vkInstance;
}

Turbo::Core::TVersion Turbo::Core::TInstance::GetVulkanVersion() const
{
    return this->vulkanVersion;
}

VkResult Turbo::Core::TInstance::CreateVkInstance(std::vector<TLayerInfo> *enabledLayers, std::vector<TExtensionInfo> *enabledExtensions, TVersion *vulkanVersion)
{
    VkResult result = VkResult::VK_NOT_READY;

    this->supportLayers = TLayerInfo::GetInstanceLayers();
    this->supportExtensions = TExtensionInfo::GetInstanceExtensions();

    if (vulkanVersion != nullptr || enabledLayers != nullptr || enabledExtensions != nullptr)
    {
        if (vulkanVersion != nullptr)
        {
            this->vulkanVersion = *vulkanVersion;
        }
        else
        {
            this->vulkanVersion = TVersion(1, 0, 0, 0);
        }

        if (enabledLayers != nullptr)
        {
            this->enabledLayers = *enabledLayers;
        }

        if (enabledExtensions != nullptr)
        {
            this->enabledExtensions = *enabledExtensions;
        }

        this->InternalCreate();
    }
    else
    {
        this->vulkanVersion = TVersion(1, 0, 0, 0);
        this->InternalCreate();
    }

    uint32_t physical_device_count = 0;
    result = this->GetInstanceDriver()->vkEnumeratePhysicalDevices(this->vkInstance, &physical_device_count, nullptr);
    if (result != VkResult::VK_SUCCESS || physical_device_count == 0)
    {
        return result;
    }

    for (uint32_t physical_device_index = 0; physical_device_index < physical_device_count; physical_device_index++)
    {
        new Turbo::Core::TPhysicalDevice(this, physical_device_index);
    }

    return VkResult::VK_SUCCESS;
}

bool Turbo::Core::TInstance::IsHaveHandle(const TRefPtr<TPhysicalDevice> &physicalDevice)
{
    if (physicalDevice.Valid())
    {
        if (physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE)
        {
            for (TPhysicalDevice *physical_device_item : this->physicalDevices)
            {
                if (physical_device_item->GetVkPhysicalDevice() == physicalDevice->GetVkPhysicalDevice())
                {
                    return true;
                }
            }

            return false;
        }
    }

    return true;
}

void Turbo::Core::TInstance::AddChildHandle(const TRefPtr<TPhysicalDevice> &physicalDevice)
{
    if (physicalDevice.Valid())
    {
        if (!this->IsHaveHandle(physicalDevice))
        {
            this->physicalDevices.push_back(physicalDevice);
        }
    }
}

Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> Turbo::Core::TInstance::RemoveChildHandle(const TRefPtr<TPhysicalDevice> &physicalDevice)
{
    // We don't need to remove it,because physical device created by instance.we just delete is at ~TInstance()
    return nullptr;
}

void Turbo::Core::TInstance::InspectExtensionAndVersionDependencies(TExtensionType extensionType)
{
    switch (extensionType)
    {
    case TExtensionType::UNDEFINED: {
    }
    break;
    case TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2: {
        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2))
        {
            this->enabledExtensions.push_back(this->GetExtensionByType(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2));
        }
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_MEMORY_CAPABILITIES: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_EXTERNAL_MEMORY_CAPABILITIES))
        {
            this->enabledExtensions.push_back(this->GetExtensionByType(TExtensionType::VK_KHR_EXTERNAL_MEMORY_CAPABILITIES));
        }
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES))
        {
            this->enabledExtensions.push_back(this->GetExtensionByType(TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES));
        }
    }
    break;
    case TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES: {
        this->InspectExtensionAndVersionDependencies(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2);

        if (!this->IsEnabledExtension(TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES))
        {
            this->enabledExtensions.push_back(this->GetExtensionByType(TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES));
        }
    }
    break;
    default: {
    }
    break;
    }
}

void Turbo::Core::TInstance::InternalCreate()
{
    TVulkanLoader::Instance();

    size_t enable_layer_count = this->enabledLayers.size();
    std::vector<const char *> enable_layer_names(enable_layer_count);
    for (uint32_t enable_layer_index = 0; enable_layer_index < enable_layer_count; enable_layer_index++)
    {
        enable_layer_names[enable_layer_index] = this->enabledLayers[enable_layer_index].GetName().c_str();
    }

    for (uint32_t enabled_extensions_index = 0; enabled_extensions_index < this->enabledExtensions.size(); enabled_extensions_index++)
    {
        this->InspectExtensionAndVersionDependencies(this->enabledExtensions[enabled_extensions_index].GetExtensionType());
    }

    size_t enable_extension_count = this->enabledExtensions.size();
    std::vector<const char *> enable_extension_names(enable_extension_count);
    for (uint32_t enable_extension_index = 0; enable_extension_index < enable_extension_count; enable_extension_index++)
    {
        enable_extension_names[enable_extension_index] = this->enabledExtensions[enable_extension_index].GetName().c_str();
    }

    VkApplicationInfo vk_application_info = {};

    vk_application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vk_application_info.pNext = nullptr;
    vk_application_info.pApplicationName = nullptr;
    vk_application_info.applicationVersion = 0;
    vk_application_info.pEngineName = nullptr;
    vk_application_info.engineVersion = 0;
    vk_application_info.apiVersion = this->vulkanVersion.GetValidVulkanVersion().GetVulkanVersion();

    VkInstanceCreateInfo instance_create_info = {};
    instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext = nullptr;
    instance_create_info.flags = 0;
    instance_create_info.pApplicationInfo = &vk_application_info;
    instance_create_info.enabledLayerCount = static_cast<uint32_t>(enable_layer_count);
    instance_create_info.ppEnabledLayerNames = enable_layer_names.data();
    instance_create_info.enabledExtensionCount = static_cast<uint32_t>(enable_extension_count);
    instance_create_info.ppEnabledExtensionNames = enable_extension_names.data();

    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = Turbo::Core::TVulkanLoader::Instance()->LoadGlobalDriver().vkCreateInstance(&instance_create_info, allocator, &this->vkInstance);
    if (result != VK_SUCCESS)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INITIALIZATION_FAILED, "Turbo::Core::TInstance::InternalCreate::vkCreateInstance");
    }

    this->supportLayers = TLayerInfo::GetInstanceLayers();
    this->supportExtensions = TExtensionInfo::GetInstanceExtensions();

    this->instanceDriver = new TInstanceDriver();

    // NOTE: for current [this->referenceCount is 0] and [TVulkanLoader::LoadInstanceDriver(const TRefPtr<TInstance> &instance, ...)] use
    // NOTE: [const TRefPtr<TInstance> &instance] , it will cause: If out from [TVulkanLoader::LoadInstanceDriver(...)] function
    // NOTE: [TRefPtr] will check [this->referenceCount is 0] and then trigger [Release] call. Let [delete this] to be called.
    // OLD:*this->instanceDriver = TVulkanLoader::Instance()->LoadInstanceDriver(this);//NOTE: it will trigger [delete this]
    TRefPtr<TInstance> temp_ref_instance = this;
    *this->instanceDriver = TVulkanLoader::Instance()->LoadInstanceDriver(this);
    temp_ref_instance.Unbind();

    // for (TPhysicalDevice *physical_device_item : this->physicalDevices)
    //{
    //     physical_device_item->InternalCreate();
    // }
}

void Turbo::Core::TInstance::InternalDestroy()
{
    for (TPhysicalDevice *physical_device_item : this->physicalDevices)
    {
        physical_device_item->InternalDestroy();
    }

    this->supportLayers.clear();
    this->supportExtensions.clear();

    if (this->vkInstance != VK_NULL_HANDLE)
    {
        VkAllocationCallbacks *allocator = TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
        this->instanceDriver->vkDestroyInstance(this->vkInstance, allocator);
        this->vkInstance = VK_NULL_HANDLE;
        delete this->instanceDriver;
    }
}

bool Turbo::Core::TInstance::IsSupportVulkan()
{
    try
    {
        TVersion vulkan_instance_version = TInstance::GetVulkanInstanceVersion();
        if (vulkan_instance_version != TVersion(0, 0, 0, 0))
        {
            return true;
        }
    }
    catch (std::exception &e)
    {
        return false;
    }

    return false;
}

Turbo::Core::TVersion Turbo::Core::TInstance::GetVulkanInstanceVersion()
{
    return TVulkanLoader::GetVulkanVersion();
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TInstance::GetSupportExtensions() const
{
    return this->supportExtensions;
}

bool Turbo::Core::TInstance::IsSupportExtension(std::string extensionName) const
{
    if (!extensionName.empty())
    {
        TExtensionType extension_type = TExtensionInfo::GetExtensionTypeByExtensionName(extensionName);
        size_t support_extension_count = this->supportExtensions.size();
        if (extension_type != TExtensionType::UNDEFINED)
        {
            for (size_t extension_index = 0; extension_index < support_extension_count; extension_index++)
            {
                if (this->supportExtensions[extension_index].GetExtensionType() == extension_type)
                {
                    return true;
                }
            }
        }
        else
        {
            for (size_t extension_index = 0; extension_index < support_extension_count; extension_index++)
            {
                if (this->supportExtensions[extension_index].GetName() == extensionName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TInstance::IsSupportExtension(TExtensionType extensionType) const
{
    return this->IsSupportExtension(TExtensionInfo::GetExtensionNameByExtensionType(extensionType));
}

size_t Turbo::Core::TInstance::GetSupportExtensionCount() const
{
    return this->supportExtensions.size();
}

std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TInstance::GetSupportLayers() const
{
    return this->supportLayers;
}

bool Turbo::Core::TInstance::IsSupportLayer(std::string layerName) const
{
    if (!layerName.empty())
    {
        TLayerType layer_type = TLayerInfo::GetLayerTypeByLayerName(layerName);
        size_t support_layers_count = this->supportLayers.size();
        if (layer_type != TLayerType::UNDEFINED)
        {
            for (size_t layer_index = 0; layer_index < support_layers_count; layer_index++)
            {
                if (this->supportLayers[layer_index].GetLayerType() == layer_type)
                {
                    return true;
                }
            }
        }
        else
        {
            for (size_t layer_index = 0; layer_index < support_layers_count; layer_index++)
            {
                if (this->supportLayers[layer_index].GetName() == layerName)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Turbo::Core::TInstance::IsSupportLayer(TLayerType layerType) const
{
    return this->IsSupportLayer(TLayerInfo::GetLayerNameByLayerType(layerType));
}

size_t Turbo::Core::TInstance::GetSupportLayerCount() const
{
    return this->supportLayers.size();
}

uint32_t Turbo::Core::TInstance::GetPhysicalDeviceCount() const
{
    return this->physicalDevices.size();
}

const Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> &Turbo::Core::TInstance::GetPhysicalDevice(uint32_t index)
{
    if (index > this->physicalDevices.size() - 1)
    {
        throw Turbo::Core::TException(TResult::OUT_OF_RANGE, "Turbo::Core::TInstance::GetPhysicalDevice");
    }

    return this->physicalDevices[index];
}

const std::vector<Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice>> &Turbo::Core::TInstance::GetPhysicalDevices()
{
    return this->physicalDevices;
}

Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> Turbo::Core::TInstance::GetBestPhysicalDevice()
{
    uint32_t physical_device_count = this->GetPhysicalDeviceCount();

    if (physical_device_count <= 0)
    {
        return nullptr;
    }

    uint32_t socre = 0;
    uint32_t index = 0;

    for (uint32_t physical_device_index = 0; physical_device_index < physical_device_count; physical_device_index++)
    {
        if (this->physicalDevices[physical_device_index]->performanceScore > socre)
        {
            socre = this->physicalDevices[physical_device_index]->performanceScore;
            index = physical_device_index;
        }
    }

    return this->physicalDevices[index];
}

const Turbo::Core::TInstanceDriver *Turbo::Core::TInstance::GetInstanceDriver()
{
    return this->instanceDriver;
}

Turbo::Core::TExtensionInfo Turbo::Core::TInstance::GetExtensionByType(TExtensionType extensionType) const
{
    TExtensionInfo result;
    for (const TExtensionInfo &type_item : this->supportExtensions)
    {
        if (type_item.GetExtensionType() == extensionType)
        {
            result = type_item;
            break;
        }
    }

    return result;
}
