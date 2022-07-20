#include "TVulkanLoader.h"

#ifdef TURBO_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#if defined(VK_VERSION_1_0)
PFN_vkGetInstanceProcAddr Turbo::Core::vkGetInstanceProcAddr = nullptr;
PFN_vkCreateInstance Turbo::Core::vkCreateInstance = nullptr;
PFN_vkEnumerateInstanceExtensionProperties Turbo::Core::vkEnumerateInstanceExtensionProperties = nullptr;
PFN_vkEnumerateInstanceLayerProperties Turbo::Core::vkEnumerateInstanceLayerProperties = nullptr;

PFN_vkDestroyInstance Turbo::Core::vkDestroyInstance = nullptr;
PFN_vkEnumeratePhysicalDevices Turbo::Core::vkEnumeratePhysicalDevices = nullptr;
PFN_vkGetPhysicalDeviceProperties Turbo::Core::vkGetPhysicalDeviceProperties = nullptr;
PFN_vkEnumerateDeviceLayerProperties Turbo::Core::vkEnumerateDeviceLayerProperties = nullptr;
PFN_vkEnumerateDeviceExtensionProperties Turbo::Core::vkEnumerateDeviceExtensionProperties = nullptr;
PFN_vkGetPhysicalDeviceQueueFamilyProperties Turbo::Core::vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
PFN_vkGetPhysicalDeviceMemoryProperties Turbo::Core::vkGetPhysicalDeviceMemoryProperties = nullptr;
PFN_vkGetPhysicalDeviceFeatures Turbo::Core::vkGetPhysicalDeviceFeatures = nullptr;
PFN_vkGetPhysicalDeviceFormatProperties Turbo::Core::vkGetPhysicalDeviceFormatProperties = nullptr;
PFN_vkGetPhysicalDeviceImageFormatProperties Turbo::Core::vkGetPhysicalDeviceImageFormatProperties = nullptr;
PFN_vkCreateDevice Turbo::Core::vkCreateDevice = nullptr;

PFN_vkGetDeviceProcAddr Turbo::Core::vkGetDeviceProcAddr = nullptr;
PFN_vkDestroyDevice Turbo::Core::vkDestroyDevice = nullptr;
#endif

#if defined(VK_VERSION_1_1)
PFN_vkEnumerateInstanceVersion Turbo::Core::vkEnumerateInstanceVersion = nullptr;
#endif

Turbo::Core::TVulkanLoader *Turbo::Core::TVulkanLoader::vulkanLoader = nullptr;

Turbo::Core::TVulkanLoader::TVulkanLoader()
{
#ifdef TURBO_PLATFORM_WINDOWS
    HMODULE library = LoadLibraryA("vulkan-1.dll");
    if (!library)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Core::TVulkanLoader::TVulkanLoader", "Not found Vulkan Loader, please download and install Vulkan Runtime");
    }

    // loader
    Turbo::Core::vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(library, "vkGetInstanceProcAddr");
    assert(Turbo::Core::vkGetInstanceProcAddr && "Turbo::Core::vkGetInstanceProcAddr");
#else
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Core::TVulkanLoader::TVulkanLoader", "Please implement this platform definition");
#endif

//<loade global commands function>
#if defined(VK_VERSION_1_0)
    Turbo::Core::vkCreateInstance = this->Load<TLoaderType::INSTANCE, PFN_vkCreateInstance>(VK_NULL_HANDLE, "vkCreateInstance");
    Turbo::Core::vkEnumerateInstanceExtensionProperties = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceExtensionProperties>(VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
    Turbo::Core::vkEnumerateInstanceLayerProperties = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceLayerProperties>(VK_NULL_HANDLE, "vkEnumerateInstanceLayerProperties");
#endif

#if defined(VK_VERSION_1_1)
    Turbo::Core::vkEnumerateInstanceVersion = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceVersion>(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
#endif
    //</loade global commands function>
}

Turbo::Core::TVulkanLoader::~TVulkanLoader()
{
}

void Turbo::Core::TVulkanLoader::LoadAllInstanceFunctions(TInstance *instance)
{
    VkInstance vk_instance = instance->GetVkInstance();
// load all instance vulkan api function, Vulkan Loader version
#if defined(VK_VERSION_1_0)
    Turbo::Core::vkDestroyInstance = this->LoadInstanceFunsction<PFN_vkDestroyInstance>(vk_instance, "vkDestroyInstance");
    Turbo::Core::vkEnumeratePhysicalDevices = this->LoadInstanceFunsction<PFN_vkEnumeratePhysicalDevices>(vk_instance, "vkEnumeratePhysicalDevices");
    Turbo::Core::vkGetPhysicalDeviceProperties = this->LoadInstanceFunsction<PFN_vkGetPhysicalDeviceProperties>(vk_instance, "vkGetPhysicalDeviceProperties");
    Turbo::Core::vkEnumerateDeviceLayerProperties = this->LoadInstanceFunsction<PFN_vkEnumerateDeviceLayerProperties>(vk_instance, "vkEnumerateDeviceLayerProperties");
    Turbo::Core::vkEnumerateDeviceExtensionProperties = this->LoadInstanceFunsction<PFN_vkEnumerateDeviceExtensionProperties>(vk_instance, "vkEnumerateDeviceExtensionProperties");
    Turbo::Core::vkGetPhysicalDeviceQueueFamilyProperties = this->LoadInstanceFunsction<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    Turbo::Core::vkGetPhysicalDeviceMemoryProperties = this->LoadInstanceFunsction<PFN_vkGetPhysicalDeviceMemoryProperties>(vk_instance, "vkGetPhysicalDeviceMemoryProperties");
    Turbo::Core::vkGetPhysicalDeviceFeatures = this->LoadInstanceFunsction<PFN_vkGetPhysicalDeviceFeatures>(vk_instance, "vkGetPhysicalDeviceFeatures");
    Turbo::Core::vkGetPhysicalDeviceFormatProperties = this->LoadInstanceFunsction<PFN_vkGetPhysicalDeviceFormatProperties>(vk_instance, "vkGetPhysicalDeviceFormatProperties");
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties = this->LoadInstanceFunsction<PFN_vkGetPhysicalDeviceImageFormatProperties>(vk_instance, "vkGetPhysicalDeviceImageFormatProperties");
    Turbo::Core::vkCreateDevice = this->LoadInstanceFunsction<PFN_vkCreateDevice>(vk_instance, "vkCreateDevice");

    Turbo::Core::vkGetDeviceProcAddr = this->LoadInstanceFunsction<PFN_vkGetDeviceProcAddr>(vk_instance, "vkGetDeviceProcAddr");

#endif
}

void Turbo::Core::TVulkanLoader::LoadAllDeviceFunctions(TInstance *instance)
{
    VkInstance vk_instance = instance->GetVkInstance();
    // load all device vulkan api function, Vulkan Loader version
#if defined(VK_VERSION_1_0)
    Turbo::Core::vkDestroyDevice = this->LoadInstanceFunsction<PFN_vkDestroyDevice>(vk_instance, "vkDestroyDevice");
#endif
}

Turbo::Core::TVulkanLoader *Turbo::Core::TVulkanLoader::Instance()
{
    if (TVulkanLoader::vulkanLoader == nullptr)
    {
        TVulkanLoader::vulkanLoader = new TVulkanLoader();
    }

    return TVulkanLoader::vulkanLoader;
}

void Turbo::Core::TVulkanLoader::Destroy()
{
    if (TVulkanLoader::vulkanLoader != nullptr)
    {
        delete TVulkanLoader::vulkanLoader;
    }
}

void Turbo::Core::TVulkanLoader::LoadAll(TInstance *instance)
{
    this->LoadAllInstanceFunctions(instance);
    this->LoadAllDeviceFunctions(instance);
}

Turbo::Core::TVersion Turbo::Core::TVulkanLoader::GetVulkanVersion()
{
    uint32_t version = 0;

#if defined(VK_VERSION_1_1)
    PFN_vkEnumerateInstanceVersion pfn_vk_enumerate_instance_version = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceVersion>(nullptr, "vkEnumerateInstanceVersion");
    if (pfn_vk_enumerate_instance_version != nullptr && pfn_vk_enumerate_instance_version(&version) == VkResult::VK_SUCCESS)
    {
        uint32_t major = VK_VERSION_MAJOR(version);
        uint32_t minor = VK_VERSION_MINOR(version);
        uint32_t patch = VK_VERSION_PATCH(version);

        return TVersion(major, minor, patch, 0);
    }
#endif
    if (TInstance::IsSupportVulkan())
    {
        return TVersion(1, 0, 0, 0);
    }

    return TVersion(0, 0, 0, 0);
}

std::string Turbo::Core::TVulkanLoader::ToString()
{
    return std::string();
}
