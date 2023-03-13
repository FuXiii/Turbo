#include <vulkan/vulkan_core.h>

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

#include <assert.h>
#include <exception>
#include <iostream>

PFN_vkGetInstanceProcAddr vk_GetInstanceProcAddr = nullptr;
PFN_vkCreateInstance vk_CreateInstance = nullptr;
PFN_vkDestroyInstance vk_DestroyInstance = nullptr;

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

    VkInstanceCreateInfo vk_instance_create_info = {};
    vk_instance_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vk_instance_create_info.pNext = nullptr;
    vk_instance_create_info.flags = 0;
    vk_instance_create_info.pApplicationInfo = &vk_application_info;
    vk_instance_create_info.enabledLayerCount = 0;
    vk_instance_create_info.ppEnabledLayerNames = nullptr;
    vk_instance_create_info.enabledExtensionCount = 0;
    vk_instance_create_info.ppEnabledExtensionNames = nullptr;

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
    vk_DestroyInstance(instance, &vk_allocation_callbacks);

    std::cout << "vkDestroyInstance success" << std::endl;

    system("pause");

    return 0;
#endif
}