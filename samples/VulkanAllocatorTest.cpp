#include <vulkan/vulkan_core.h>

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
    return _aligned_malloc(size, alignment);
}

void *VKAPI_PTR Reallocate(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
    return _aligned_realloc(pOriginal, size, alignment);
}

void VKAPI_PTR Free(void *pUserData, void *pMemory)
{
    return _aligned_free(pMemory);
}

int main()
{
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    HMODULE library = LoadLibraryA("vulkan-1.dll");
    if (!library)
    {
        throw std::exception("Can not found vulkan-1.dll");
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
    std::vector<const char *> instance_enabled_extension_names;
    instance_enabled_extension_names.push_back(enable_instance_extensions[0].c_str());
    instance_enabled_extension_names.push_back(enable_instance_extensions[1].c_str());

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

    VkInstance instance = VK_NULL_HANDLE;
    VkResult result = vk_CreateInstance(&vk_instance_create_info, &vk_allocation_callbacks, &instance);
    if (result != VkResult::VK_SUCCESS)
    {
        throw std::exception("vkCreateInstance failed");
    }

    std::cout << "vkCreateInstance success" << std::endl;
    vk_DestroyInstance = (PFN_vkDestroyInstance)vk_GetInstanceProcAddr(instance, "vkDestroyInstance");
    assert(vk_DestroyInstance && "vkDestroyInstance");

    vk_EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)vk_GetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices");
    assert(vk_EnumeratePhysicalDevices && "vkEnumeratePhysicalDevices");

    vk_GetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)vk_GetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
    assert(vk_GetPhysicalDeviceProperties && "vkEnumeratePhysicalDevices");

    vk_CreateDevice = (PFN_vkCreateDevice)vk_GetInstanceProcAddr(instance, "vkCreateDevice");
    assert(vk_CreateDevice && "vkCreateDevice");

    vk_DestroyDevice = (PFN_vkDestroyDevice)vk_GetInstanceProcAddr(instance, "vkDestroyDevice");
    assert(vk_DestroyDevice && "vkDestroyDevice");

    vk_GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vk_GetInstanceProcAddr(instance, "vkGetDeviceProcAddr");
    assert(vk_GetDeviceProcAddr && "vkGetDeviceProcAddr");

    vk_GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)vk_GetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    assert(vk_GetPhysicalDeviceQueueFamilyProperties && "vkGetPhysicalDeviceQueueFamilyProperties");

    uint32_t physcial_device_count = 0;
    result = vk_EnumeratePhysicalDevices(instance, &physcial_device_count, nullptr);
    if (result != VkResult::VK_SUCCESS)
    {
        throw std::exception("vkEnumeratePhysicalDevices get count failed");
    }

    std::vector<VkPhysicalDevice> physcail_devices(physcial_device_count);
    result = vk_EnumeratePhysicalDevices(instance, &physcial_device_count, physcail_devices.data());
    if (result != VkResult::VK_SUCCESS)
    {
        throw std::exception("vkEnumeratePhysicalDevices failed");
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
        throw std::exception("Not found suitable GPU");
    }
    std::cout << "Select Physical Device:" << target_physical_device_name << std::endl;

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
        throw std::exception("vkCreateDevice failed");
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

    uint32_t index = 0;
    uint32_t loop_count = UINT32_MAX;

    while (index < loop_count)
    {
        VkPipelineLayoutCreateInfo vk_pipeline_layout_create_info = {};
        vk_pipeline_layout_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vk_pipeline_layout_create_info.pNext = nullptr;
        vk_pipeline_layout_create_info.flags = 0;
        vk_pipeline_layout_create_info.setLayoutCount = 0;
        vk_pipeline_layout_create_info.pSetLayouts = nullptr;
        vk_pipeline_layout_create_info.pushConstantRangeCount = 0;
        vk_pipeline_layout_create_info.pPushConstantRanges = nullptr;

        VkPipelineLayout vk_pipeline_layout = VK_NULL_HANDLE;
        result = vk_CreatePipelineLayout(vk_device, &vk_pipeline_layout_create_info, nullptr, &vk_pipeline_layout);
        if (result != VkResult::VK_SUCCESS)
        {
            throw std::exception("vkCreatePipelineLayout failed");
        }
        vk_DestroyPipelineLayout(vk_device, vk_pipeline_layout, nullptr);

        index++;

        std::cout << index << std::endl;
    }

    vk_DestroyDevice(vk_device, nullptr);
    std::cout << "vkDestroyDevice success" << std::endl;

    vk_DestroyInstance(instance, &vk_allocation_callbacks);
    std::cout << "vkDestroyInstance success" << std::endl;

    // system("pause");

    return 0;
#endif
}