#include "core/include/TException.h"
#include "core/include/TInstance.h"
#include <vulkan/vulkan.h>
#include "core/include/TVulkanAllocator.h"

// #include <vulkan/vulkan_core.h>
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif

#include <assert.h>
#include <exception>
#include <iostream>
#include <vector>

PFN_vkGetInstanceProcAddr vk_GetInstanceProcAddr = nullptr;
PFN_vkCreateInstance vk_CreateInstance = nullptr;
PFN_vkDestroyInstance vk_DestroyInstance = nullptr;
PFN_vkGetPhysicalDeviceFeatures2KHR vk_GetPhysicalDeviceFeatures2KHR = nullptr;
PFN_vkGetPhysicalDeviceFeatures2 vk_GetPhysicalDeviceFeatures2 = nullptr;
PFN_vkEnumeratePhysicalDevices vk_EnumeratePhysicalDevices = nullptr;
PFN_vkGetPhysicalDeviceProperties vk_GetPhysicalDeviceProperties = nullptr;
PFN_vkCreateDevice vk_CreateDevice = nullptr;
PFN_vkDestroyDevice vk_DestroyDevice = nullptr;
PFN_vkGetDeviceProcAddr vk_GetDeviceProcAddr = nullptr;
PFN_vkCreatePipelineLayout vk_CreatePipelineLayout = nullptr;
PFN_vkDestroyPipelineLayout vk_DestroyPipelineLayout = nullptr;
PFN_vkGetDeviceQueue vk_GetDeviceQueue = nullptr;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vk_GetPhysicalDeviceQueueFamilyProperties = nullptr;
PFN_vkQueuePresentKHR vk_QueuePresentKHR = nullptr;

void *VKAPI_PTR Allocation(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return Turbo::Core::TVulkanAllocator::Allocate(pUserData, size, alignment, allocationScope);
}

void *VKAPI_PTR Reallocate(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return Turbo::Core::TVulkanAllocator::Reallocate(pUserData, pOriginal, size, alignment, allocationScope);
}

void VKAPI_PTR Free(void *pUserData, void *pMemory)
{
    return Turbo::Core::TVulkanAllocator::Free(pUserData, pMemory);
}

void Test0(VkInstance vkInstance, VkPhysicalDevice vkPhysicalDevice, VkDevice vkDevice, const std::vector<VkQueue> &vkQueues)
{
    VkPhysicalDeviceMeshShaderFeaturesNV vk_physical_device_mesh_shader_features_nv = {};
    vk_physical_device_mesh_shader_features_nv.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
    vk_physical_device_mesh_shader_features_nv.pNext = nullptr;
    vk_physical_device_mesh_shader_features_nv.taskShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_nv.meshShader = VK_FALSE;

    VkPhysicalDeviceMeshShaderFeaturesEXT vk_physical_device_mesh_shader_features_ext = {};
    vk_physical_device_mesh_shader_features_ext.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
    vk_physical_device_mesh_shader_features_ext.pNext = &vk_physical_device_mesh_shader_features_nv;
    vk_physical_device_mesh_shader_features_ext.taskShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.meshShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.multiviewMeshShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.primitiveFragmentShadingRateMeshShader = VK_FALSE;
    vk_physical_device_mesh_shader_features_ext.meshShaderQueries = VK_FALSE;

    VkPhysicalDeviceFeatures2 vk_physical_device_features_2 = {};
    vk_physical_device_features_2.sType = VkStructureType::VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    vk_physical_device_features_2.pNext = &vk_physical_device_mesh_shader_features_ext;
    vk_physical_device_features_2.features = {};

    vk_GetPhysicalDeviceFeatures2(vkPhysicalDevice, &vk_physical_device_features_2);

    if (vk_physical_device_mesh_shader_features_nv.taskShader)
    {
        std::cout << "NV_taskShader::True" << std::endl;
    }
    else
    {
        std::cout << "NV_taskShader::False" << std::endl;
    }

    if (vk_physical_device_mesh_shader_features_nv.meshShader)
    {
        std::cout << "NV_meshShader::True" << std::endl;
    }
    else
    {
        std::cout << "NV_meshShader::False" << std::endl;
    }

    if (vk_physical_device_mesh_shader_features_ext.taskShader)
    {
        std::cout << "EXT_taskShader::True" << std::endl;
    }
    else
    {
        std::cout << "EXT_taskShader::False" << std::endl;
    }

    if (vk_physical_device_mesh_shader_features_ext.meshShader)
    {
        std::cout << "EXT_meshShader::True" << std::endl;
    }
    else
    {
        std::cout << "EXT_meshShader::False" << std::endl;
    }

    if (vk_physical_device_mesh_shader_features_ext.multiviewMeshShader)
    {
        std::cout << "EXT_multiviewMeshShader::True" << std::endl;
    }
    else
    {
        std::cout << "EXT_multiviewMeshShader::False" << std::endl;
    }

    if (vk_physical_device_mesh_shader_features_ext.primitiveFragmentShadingRateMeshShader)
    {
        std::cout << "EXT_primitiveFragmentShadingRateMeshShader::True" << std::endl;
    }
    else
    {
        std::cout << "EXT_primitiveFragmentShadingRateMeshShader::False" << std::endl;
    }

    if (vk_physical_device_mesh_shader_features_ext.meshShaderQueries)
    {
        std::cout << "EXT_meshShaderQueries::True" << std::endl;
    }
    else
    {
        std::cout << "EXT_meshShaderQueries::False" << std::endl;
    }
}

int main()
{
    std::cout << "IsSupportVulkan:" << Turbo::Core::TInstance::IsSupportVulkan() << std::endl;
    std::cout << "Vulkan:" << Turbo::Core::TInstance::GetVulkanInstanceVersion().ToString() << std::endl;
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    HMODULE library = LoadLibraryA("vulkan-1.dll");
    if (!library)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "Can not found vulkan-1.dll");
    }

    vk_GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(library, "vkGetInstanceProcAddr");
    assert(vk_GetInstanceProcAddr && "vkGetInstanceProcAddr");

    vk_CreateInstance = (PFN_vkCreateInstance)vk_GetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
    assert(vk_CreateInstance && "vkCreateInstance");

    VkApplicationInfo vk_application_info = {};
    vk_application_info.sType = VkStructureType::VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vk_application_info.pNext = nullptr;
    vk_application_info.pApplicationName = nullptr;
    vk_application_info.applicationVersion = 0;
    vk_application_info.pEngineName = nullptr;
    vk_application_info.engineVersion = 0;
    vk_application_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    std::vector<std::string> enable_instance_layers;
    enable_instance_layers.push_back("VK_LAYER_KHRONOS_validation");
    std::vector<const char *> instance_enabled_layer_names;
    instance_enabled_layer_names.push_back(enable_instance_layers[0].c_str());

    std::vector<std::string> enable_instance_extensions;
    enable_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    enable_instance_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    {
        // enable_instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    }
    std::vector<const char *> instance_enabled_extension_names;
    for (const std::string &instance_extension : enable_instance_extensions)
    {
        instance_enabled_extension_names.push_back(instance_extension.c_str());
    }

    VkInstanceCreateInfo vk_instance_create_info = {};
    vk_instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vk_instance_create_info.pNext = nullptr;
    vk_instance_create_info.flags = 0;
    vk_instance_create_info.pApplicationInfo = &vk_application_info;
    vk_instance_create_info.enabledLayerCount = instance_enabled_layer_names.size();
    vk_instance_create_info.ppEnabledLayerNames = instance_enabled_layer_names.data();
    vk_instance_create_info.enabledExtensionCount = instance_enabled_extension_names.size();
    vk_instance_create_info.ppEnabledExtensionNames = instance_enabled_extension_names.data();

    VkAllocationCallbacks vk_allocation_callbacks = {};
    vk_allocation_callbacks.pUserData = nullptr;
    vk_allocation_callbacks.pfnAllocation = &Allocation;
    vk_allocation_callbacks.pfnReallocation = &Reallocate;
    vk_allocation_callbacks.pfnFree = &Free;
    vk_allocation_callbacks.pfnInternalAllocation = nullptr;
    vk_allocation_callbacks.pfnInternalFree = nullptr;

    VkInstance vk_instance = VK_NULL_HANDLE;
    VkResult result = vk_CreateInstance(&vk_instance_create_info, &vk_allocation_callbacks, &vk_instance);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "vkCreateInstance failed");
    }
    else
    {
        std::cout << "vkCreateInstance success" << std::endl;
    }

    // vk_GetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)vk_GetInstanceProcAddr(vk_instance, "vkGetPhysicalDeviceFeatures2KHR");
    // assert(vk_GetPhysicalDeviceFeatures2KHR && "vkGetPhysicalDeviceFeatures2KHR");
    {
        vk_GetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)vk_GetInstanceProcAddr(vk_instance, "vkGetPhysicalDeviceFeatures2");
        assert(vk_GetPhysicalDeviceFeatures2 && "vkGetPhysicalDeviceFeatures2");
    }

    vk_DestroyInstance = (PFN_vkDestroyInstance)vk_GetInstanceProcAddr(vk_instance, "vkDestroyInstance");
    assert(vk_DestroyInstance && "vkDestroyInstance");

    vk_EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)vk_GetInstanceProcAddr(vk_instance, "vkEnumeratePhysicalDevices");
    assert(vk_EnumeratePhysicalDevices && "vkEnumeratePhysicalDevices");

    vk_GetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)vk_GetInstanceProcAddr(vk_instance, "vkGetPhysicalDeviceProperties");
    assert(vk_GetPhysicalDeviceProperties && "vkEnumeratePhysicalDevices");

    vk_CreateDevice = (PFN_vkCreateDevice)vk_GetInstanceProcAddr(vk_instance, "vkCreateDevice");
    assert(vk_CreateDevice && "vkCreateDevice");

    vk_DestroyDevice = (PFN_vkDestroyDevice)vk_GetInstanceProcAddr(vk_instance, "vkDestroyDevice");
    assert(vk_DestroyDevice && "vkDestroyDevice");

    vk_GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vk_GetInstanceProcAddr(vk_instance, "vkGetDeviceProcAddr");
    assert(vk_GetDeviceProcAddr && "vkGetDeviceProcAddr");

    vk_GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)vk_GetInstanceProcAddr(vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    assert(vk_GetPhysicalDeviceQueueFamilyProperties && "vkGetPhysicalDeviceQueueFamilyProperties");

    uint32_t physcial_device_count = 0;
    result = vk_EnumeratePhysicalDevices(vk_instance, &physcial_device_count, nullptr);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "vkEnumeratePhysicalDevices get count failed");
    }

    std::vector<VkPhysicalDevice> physcail_devices(physcial_device_count);
    result = vk_EnumeratePhysicalDevices(vk_instance, &physcial_device_count, physcail_devices.data());
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "vkEnumeratePhysicalDevices failed");
    }

    VkPhysicalDevice target_physical_device = VK_NULL_HANDLE;
    std::string target_physical_device_name;
    for (VkPhysicalDevice physcail_devices_item : physcail_devices)
    {
        VkPhysicalDeviceProperties vk_physical_device_properties = {};
        vk_GetPhysicalDeviceProperties(physcail_devices_item, &vk_physical_device_properties);

        if (vk_physical_device_properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            target_physical_device = physcail_devices_item;
            target_physical_device_name = std::string(vk_physical_device_properties.deviceName);
            break;
        }
        else if (vk_physical_device_properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        {
            target_physical_device = physcail_devices_item;
            target_physical_device_name = std::string(vk_physical_device_properties.deviceName);
        }
    }

    if (target_physical_device == VK_NULL_HANDLE)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "Not found suitable GPU");
    }
    else
    {
        std::cout << "Select Physical Device:" << target_physical_device_name << std::endl;
    }

    uint32_t queue_family_count = 0;
    vk_GetPhysicalDeviceQueueFamilyProperties(target_physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> vk_queue_family_properties(queue_family_count);
    vk_GetPhysicalDeviceQueueFamilyProperties(target_physical_device, &queue_family_count, vk_queue_family_properties.data());

    std::vector<VkDeviceQueueCreateInfo> vk_device_queue_create_infos;
    float queue_prioritie = 0;
    for (uint32_t queue_family_index = 0; queue_family_index < queue_family_count; queue_family_index++)
    {
        if (vk_queue_family_properties[queue_family_index].queueCount > 0)
        {
            VkDeviceQueueCreateInfo vk_device_queue_create_info = {};
            vk_device_queue_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            vk_device_queue_create_info.pNext = nullptr;
            vk_device_queue_create_info.flags = 0;
            vk_device_queue_create_info.queueFamilyIndex = queue_family_index;
            vk_device_queue_create_info.queueCount = 1;
            vk_device_queue_create_info.pQueuePriorities = &queue_prioritie;

            vk_device_queue_create_infos.push_back(vk_device_queue_create_info);
        }
    }

    std::vector<std::string> enable_device_extensions;
    enable_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    std::vector<const char *> device_enabled_extension_names;
    device_enabled_extension_names.push_back(enable_device_extensions[0].c_str());

    VkDeviceCreateInfo vk_device_create_info = {};
    vk_device_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    vk_device_create_info.pNext = nullptr;
    vk_device_create_info.flags = 0;
    vk_device_create_info.queueCreateInfoCount = vk_device_queue_create_infos.size();
    vk_device_create_info.pQueueCreateInfos = vk_device_queue_create_infos.data();
    vk_device_create_info.enabledLayerCount = 0;
    vk_device_create_info.ppEnabledLayerNames = nullptr;
    vk_device_create_info.enabledExtensionCount = device_enabled_extension_names.size();
    vk_device_create_info.ppEnabledExtensionNames = device_enabled_extension_names.data();
    vk_device_create_info.pEnabledFeatures = nullptr;

    VkDevice vk_device = VK_NULL_HANDLE;
    result = vk_CreateDevice(target_physical_device, &vk_device_create_info, nullptr, &vk_device);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::FAIL, "vkCreateDevice failed");
    }
    std::cout << "vkCreateDevice success" << std::endl;

    vk_GetDeviceQueue = (PFN_vkGetDeviceQueue)vk_GetDeviceProcAddr(vk_device, "vkGetDeviceQueue");
    assert(vk_GetDeviceQueue && "vkGetDeviceQueue");

    vk_CreatePipelineLayout = (PFN_vkCreatePipelineLayout)vk_GetDeviceProcAddr(vk_device, "vkCreatePipelineLayout");
    assert(vk_CreatePipelineLayout && "vkCreatePipelineLayout");

    vk_DestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)vk_GetDeviceProcAddr(vk_device, "vkDestroyPipelineLayout");
    assert(vk_DestroyPipelineLayout && "vkDestroyPipelineLayout");

    vk_QueuePresentKHR = (PFN_vkQueuePresentKHR)vk_GetDeviceProcAddr(vk_device, "vkQueuePresentKHR");
    assert(vk_QueuePresentKHR && "vkQueuePresentKHR");
    
    std::vector<VkQueue> vk_queues;
    for (VkDeviceQueueCreateInfo &vk_device_queue_create_info_item : vk_device_queue_create_infos)
    {
        VkQueue vk_queue = VK_NULL_HANDLE;
        vk_GetDeviceQueue(vk_device, vk_device_queue_create_info_item.queueFamilyIndex, 0, &vk_queue);
        vk_queues.push_back(vk_queue);
    }

    // TODO: Test Code ...
    Test0(vk_instance, target_physical_device, vk_device, vk_queues);

    vk_DestroyDevice(vk_device, nullptr);
    std::cout << "vkDestroyDevice success" << std::endl;

    vk_DestroyInstance(vk_instance, &vk_allocation_callbacks);
    std::cout << "vkDestroyInstance success" << std::endl;

    return 0;
#endif
}
