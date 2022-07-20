#pragma once
#ifndef TVULKANLOADER_H
#define TVULKANLOADER_H
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include <assert.h>

namespace Turbo
{
namespace Core
{
#define VULKAN_GLOBAL_API
#define VULKAN_INSTANCE_API
#define VULKAN_DEVICE_API

#if defined(VK_VERSION_1_0)
extern VULKAN_GLOBAL_API PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern VULKAN_GLOBAL_API PFN_vkCreateInstance vkCreateInstance;
extern VULKAN_GLOBAL_API PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern VULKAN_GLOBAL_API PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

extern VULKAN_INSTANCE_API PFN_vkDestroyInstance vkDestroyInstance;
extern VULKAN_INSTANCE_API PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern VULKAN_INSTANCE_API PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern VULKAN_INSTANCE_API PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
extern VULKAN_INSTANCE_API PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern VULKAN_INSTANCE_API PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern VULKAN_INSTANCE_API PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
extern VULKAN_INSTANCE_API PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
extern VULKAN_INSTANCE_API PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
extern VULKAN_INSTANCE_API PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
extern VULKAN_INSTANCE_API PFN_vkCreateDevice vkCreateDevice;
extern VULKAN_INSTANCE_API PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

extern VULKAN_DEVICE_API PFN_vkDestroyDevice vkDestroyDevice;
#endif

#if defined(VK_VERSION_1_1)
extern VULKAN_GLOBAL_API PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
#endif

class TDevice;

class TVulkanLoader : public TObject
{
  private:
    static TVulkanLoader *vulkanLoader;

    enum class TLoaderType
    {
        INSTANCE,
        DEVICE
    };

  private:
    TVulkanLoader();
    ~TVulkanLoader();

    template <TLoaderType type, typename Function>
    Function Load(void *context, const char *name);

    void LoadAllInstanceFunctions(TInstance *instance);
    void LoadAllDeviceFunctions(TInstance *instance);

  public:
    static TVulkanLoader *Instance();
    static void Destroy();

    void LoadAll(TInstance *instance);

    template <typename Function>
    Function LoadInstanceFunsction(TInstance *instance, const char *name);

    template <typename Function>
    Function LoadInstanceFunsction(VkInstance instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunsction(TInstance *instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunsction(VkInstance instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunsction(TDevice *device, const char *name);

    template <typename Function>
    Function LoadDeviceFunsction(VkDevice device, const char *name);

    TVersion GetVulkanVersion();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

template <Turbo::Core::TVulkanLoader::TLoaderType type, typename Function>
Function Turbo::Core::TVulkanLoader::Load(void *context, const char *name)
{
    Function result{};

    switch (type)
    {
    case TLoaderType::INSTANCE:
        result = (Function)Turbo::Core::vkGetInstanceProcAddr((VkInstance)context, name);
        break;
    case TLoaderType::DEVICE:
        result = (Function)Turbo::Core::vkGetDeviceProcAddr((VkDevice)context, name);
        break;
    }

    assert(result && name);
    return result;
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadInstanceFunsction(TInstance *instance, const char *name)
{
    return this->LoadInstanceFunsction<Function>(instance->GetVkInstance(), name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadInstanceFunsction(VkInstance instance, const char *name)
{
    return this->Load<TLoaderType::INSTANCE, Function>(instance, name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunsction(TInstance *instance, const char *name)
{
    return this->LoadDeviceFunsction<Function>(instance->GetVkInstance(), name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunsction(VkInstance instance, const char *name)
{
    return this->LoadInstanceFunsction<Function>(instance, name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunsction(TDevice *device, const char *name)
{
    return this->LoadDeviceFunsction<Function>(device->GetVkDevice(), name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunsction(VkDevice device, const char *name)
{
    return this->Load<TLoaderType::DEVICE, Function>(device, name);
}
#endif // !TVULKANLOADER_H