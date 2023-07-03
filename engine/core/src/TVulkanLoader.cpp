#include "TVulkanLoader.h"

#if defined(TURBO_PLATFORM_LINUX)
#include <dlfcn.h>
#elif defined(TURBO_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

#if defined(VK_VERSION_1_0)
PFN_vkGetInstanceProcAddr Turbo::Core::vkGetInstanceProcAddr = nullptr;
PFN_vkEnumerateInstanceVersion Turbo::Core::vkEnumerateInstanceVersion = nullptr;
PFN_vkCreateInstance Turbo::Core::vkCreateInstance = nullptr;
PFN_vkEnumerateInstanceExtensionProperties Turbo::Core::vkEnumerateInstanceExtensionProperties = nullptr;
PFN_vkEnumerateInstanceLayerProperties Turbo::Core::vkEnumerateInstanceLayerProperties = nullptr;
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
#elif defined(TURBO_PLATFORM_LINUX)
    void *library = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!library)
    {
        library = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    }

    if (!library)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Core::TVulkanLoader::TVulkanLoader", "Not found Vulkan Loader, please download and install Vulkan Runtime");
    }

    // loader
    Turbo::Core::vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(library, "vkGetInstanceProcAddr");
    assert(Turbo::Core::vkGetInstanceProcAddr && "Turbo::Core::vkGetInstanceProcAddr");
#else
    // libvulkan.1.dylib//on macOS
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Core::TVulkanLoader::TVulkanLoader", "Please implement this platform definition");
#endif

//<loade global commands function>
#if defined(VK_VERSION_1_0)
    Turbo::Core::vkEnumerateInstanceVersion = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceVersion>(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    Turbo::Core::vkCreateInstance = this->Load<TLoaderType::INSTANCE, PFN_vkCreateInstance>(VK_NULL_HANDLE, "vkCreateInstance");
    Turbo::Core::vkEnumerateInstanceExtensionProperties = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceExtensionProperties>(VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
    Turbo::Core::vkEnumerateInstanceLayerProperties = this->Load<TLoaderType::INSTANCE, PFN_vkEnumerateInstanceLayerProperties>(VK_NULL_HANDLE, "vkEnumerateInstanceLayerProperties");
#endif

#if defined(VK_VERSION_1_1)
#endif
    //</loade global commands function>
}

Turbo::Core::TVulkanLoader::~TVulkanLoader()
{
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

Turbo::Core::TVersion Turbo::Core::TVulkanLoader::GetVulkanVersion()
{
#ifdef TURBO_PLATFORM_WINDOWS
    HMODULE library = LoadLibraryA("vulkan-1.dll");
    if (!library)
    {
        return TVersion(0, 0, 0, 0);
    }
    PFN_vkGetInstanceProcAddr vk_get_instance_proc_addr = (PFN_vkGetInstanceProcAddr)(void (*)(void))GetProcAddress(library, "vkGetInstanceProcAddr");
#elif defined(TURBO_PLATFORM_LINUX)
    void *library = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!library)
    {
        library = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    }

    if (!library)
    {
        return TVersion(0, 0, 0, 0);
    }
    PFN_vkGetInstanceProcAddr vk_get_instance_proc_addr = (PFN_vkGetInstanceProcAddr)dlsym(library, "vkGetInstanceProcAddr");
#else
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Core::TVulkanLoader::TVulkanLoader::GetVulkanVersion()", "Please implement this platform definition");
#endif

    auto uinstall_vulkan_lib = [&]() {
#ifdef TURBO_PLATFORM_WINDOWS
        if (library)
        {
            FreeLibrary(library);
        }
#elif defined(TURBO_PLATFORM_LINUX)
        if (library)
        {
            dlclose(library);
        }
#else
        throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Core::TVulkanLoader::TVulkanLoader::GetVulkanVersion()", "Please implement this platform definition");
#endif
    }; // end of lambda expression

    if (!vk_get_instance_proc_addr)
    {
        uinstall_vulkan_lib();
        return TVersion(0, 0, 0, 0);
    }

    PFN_vkEnumerateInstanceVersion vk_enumerate_instance_version = (PFN_vkEnumerateInstanceVersion)vk_get_instance_proc_addr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
    if (vk_enumerate_instance_version)
    {
        uint32_t vulkan_version = 0;
        VkResult result = vk_enumerate_instance_version(&vulkan_version);
        if (result != VkResult::VK_SUCCESS)
        {
            uinstall_vulkan_lib();
            return TVersion(0, 0, 0, 0);
        }

        uinstall_vulkan_lib();
        return TVersion(VK_VERSION_MAJOR(vulkan_version), VK_VERSION_MINOR(vulkan_version), VK_VERSION_PATCH(vulkan_version), 0);
    }

    // TODO:try to create Vulkan1.0
    PFN_vkCreateInstance vk_create_instance = (PFN_vkCreateInstance)vk_get_instance_proc_addr(VK_NULL_HANDLE, "vkCreateInstance");
    if (!vk_create_instance)
    {
        uinstall_vulkan_lib();
        return TVersion(0, 0, 0, 0);
    }

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

    VkInstance vk_instance = VK_NULL_HANDLE;
    VkResult result = vk_create_instance(&vk_instance_create_info, nullptr, &vk_instance);
    if (result != VkResult::VK_SUCCESS)
    {
        uinstall_vulkan_lib();
        return TVersion(0, 0, 0, 0);
    }

    PFN_vkDestroyInstance vk_destroy_instance = (PFN_vkDestroyInstance)vk_get_instance_proc_addr(vk_instance, "vkDestroyInstance");
    if (!vk_destroy_instance)
    {
        uinstall_vulkan_lib();
        throw Turbo::Core::TException(TResult::FAIL);
    }

    vk_destroy_instance(vk_instance, nullptr);
    uinstall_vulkan_lib();
    return TVersion(1, 0, 0, 0);
}

Turbo::Core::TInstanceDriver Turbo::Core::TVulkanLoader::LoadInstanceDriver(TInstance *instance)
{
    Turbo::Core::TInstanceDriver instance_driver = {};

    Turbo::Core::TVersion instance_version = instance->GetVulkanVersion();

    VkInstance vk_instance = instance->GetVkInstance();
#if defined(VK_VERSION_1_0)
    instance_driver.vkDestroyInstance = this->LoadInstanceFunction<PFN_vkDestroyInstance>(vk_instance, "vkDestroyInstance");
    instance_driver.vkEnumeratePhysicalDevices = this->LoadInstanceFunction<PFN_vkEnumeratePhysicalDevices>(vk_instance, "vkEnumeratePhysicalDevices");
    instance_driver.vkGetDeviceProcAddr = this->LoadInstanceFunction<PFN_vkGetDeviceProcAddr>(vk_instance, "vkGetDeviceProcAddr");
#endif

    Turbo::Core::TVersion vulkan_version_1_1 = Turbo::Core::TVersion(1, 1, 0, 0);
    if (instance_version >= vulkan_version_1_1)
    {
#if defined(VK_VERSION_1_1)
        instance_driver.vkEnumeratePhysicalDeviceGroups = this->LoadInstanceFunction<PFN_vkEnumeratePhysicalDeviceGroups>(vk_instance, "vkEnumeratePhysicalDeviceGroups");
#endif
    }

#if defined(VK_KHR_device_group_creation)
    if (instance->IsEnabledExtension(TExtensionType::VK_KHR_DEVICE_GROUP_CREATION))
    {
        if (instance->IsSupportExtension(TExtensionType::VK_KHR_DEVICE_GROUP_CREATION))
        {
            instance_driver.vkEnumeratePhysicalDeviceGroupsKHR = this->LoadInstanceFunction<PFN_vkEnumeratePhysicalDeviceGroupsKHR>(vk_instance, "vkEnumeratePhysicalDeviceGroupsKHR");
        }
    }
#endif

    return instance_driver;
}

Turbo::Core::TPhysicalDeviceDriver Turbo::Core::TVulkanLoader::LoadPhysicalDeviceDriver(TPhysicalDevice *physicalDevice)
{
    Turbo::Core::TPhysicalDeviceDriver physical_device_driver = {};
    Turbo::Core::TInstance *instance = physicalDevice->GetInstance();
    Turbo::Core::TVersion instance_version = instance->GetVulkanVersion();
    VkInstance vk_instance = instance->GetVkInstance();
    VkPhysicalDevice vk_physical_device = physicalDevice->GetVkPhysicalDevice();
#if defined(VK_VERSION_1_0)
    physical_device_driver.vkGetPhysicalDeviceFeatures = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFeatures>(vk_instance, "vkGetPhysicalDeviceFeatures");
    physical_device_driver.vkGetPhysicalDeviceFormatProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFormatProperties>(vk_instance, "vkGetPhysicalDeviceFormatProperties");
    physical_device_driver.vkGetPhysicalDeviceImageFormatProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceImageFormatProperties>(vk_instance, "vkGetPhysicalDeviceImageFormatProperties");
    physical_device_driver.vkGetPhysicalDeviceProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceProperties>(vk_instance, "vkGetPhysicalDeviceProperties");
    physical_device_driver.vkGetPhysicalDeviceQueueFamilyProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    physical_device_driver.vkGetPhysicalDeviceMemoryProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceMemoryProperties>(vk_instance, "vkGetPhysicalDeviceMemoryProperties");
    physical_device_driver.vkCreateDevice = this->LoadInstanceFunction<PFN_vkCreateDevice>(vk_instance, "vkCreateDevice");
    physical_device_driver.vkEnumerateDeviceExtensionProperties = this->LoadInstanceFunction<PFN_vkEnumerateDeviceExtensionProperties>(vk_instance, "vkEnumerateDeviceExtensionProperties");
    physical_device_driver.vkEnumerateDeviceLayerProperties = this->LoadInstanceFunction<PFN_vkEnumerateDeviceLayerProperties>(vk_instance, "vkEnumerateDeviceLayerProperties");
    physical_device_driver.vkGetPhysicalDeviceSparseImageFormatProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(vk_instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif

    VkPhysicalDeviceProperties vk_physical_device_properties = {};
    physical_device_driver.vkGetPhysicalDeviceProperties(vk_physical_device, &vk_physical_device_properties);

    uint32_t physical_device_vulkan_api_version = vk_physical_device_properties.apiVersion;
    Turbo::Core::TVersion physical_device_vulkan_version(VK_VERSION_MAJOR(physical_device_vulkan_api_version), VK_VERSION_MINOR(physical_device_vulkan_api_version), VK_VERSION_PATCH(physical_device_vulkan_api_version), 0);

    Turbo::Core::TVersion vulkan_version_1_1 = Turbo::Core::TVersion(1, 1, 0, 0);
    if (instance_version >= vulkan_version_1_1 || physical_device_vulkan_version >= vulkan_version_1_1)
    {
#if defined(VK_VERSION_1_1)
        physical_device_driver.vkGetPhysicalDeviceFeatures2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFeatures2>(vk_instance, "vkGetPhysicalDeviceFeatures2");
        physical_device_driver.vkGetPhysicalDeviceProperties2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceProperties2>(vk_instance, "vkGetPhysicalDeviceProperties2");
        physical_device_driver.vkGetPhysicalDeviceFormatProperties2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFormatProperties2>(vk_instance, "vkGetPhysicalDeviceFormatProperties2");
        physical_device_driver.vkGetPhysicalDeviceImageFormatProperties2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceImageFormatProperties2>(vk_instance, "vkGetPhysicalDeviceImageFormatProperties2");
        physical_device_driver.vkGetPhysicalDeviceQueueFamilyProperties2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceQueueFamilyProperties2>(vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties2");
        physical_device_driver.vkGetPhysicalDeviceMemoryProperties2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceMemoryProperties2>(vk_instance, "vkGetPhysicalDeviceMemoryProperties2");
        physical_device_driver.vkGetPhysicalDeviceSparseImageFormatProperties2 = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSparseImageFormatProperties2>(vk_instance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
        physical_device_driver.vkGetPhysicalDeviceExternalBufferProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceExternalBufferProperties>(vk_instance, "vkGetPhysicalDeviceExternalBufferProperties");
        physical_device_driver.vkGetPhysicalDeviceExternalFenceProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceExternalFenceProperties>(vk_instance, "vkGetPhysicalDeviceExternalFenceProperties");
        physical_device_driver.vkGetPhysicalDeviceExternalSemaphoreProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceExternalSemaphoreProperties>(vk_instance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
#endif
    }

#if defined(VK_VERSION_1_2)
#endif
    Turbo::Core::TVersion vulkan_version_1_3 = Turbo::Core::TVersion(1, 3, 0, 0);
    if (instance_version >= vulkan_version_1_3 || physical_device_vulkan_version >= vulkan_version_1_3)
    {
#if defined(VK_VERSION_1_3)
        physical_device_driver.vkGetPhysicalDeviceToolProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceToolProperties>(vk_instance, "vkGetPhysicalDeviceToolProperties");
#endif
    }

#if defined(VK_KHR_get_physical_device_properties2)
    if (instance->IsEnabledExtension(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2))
    {
        if (instance->IsSupportExtension(TExtensionType::VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2))
        {
            physical_device_driver.vkGetPhysicalDeviceFeatures2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFeatures2KHR>(vk_instance, "vkGetPhysicalDeviceFeatures2KHR");
            physical_device_driver.vkGetPhysicalDeviceProperties2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceProperties2KHR>(vk_instance, "vkGetPhysicalDeviceProperties2KHR");
            physical_device_driver.vkGetPhysicalDeviceFormatProperties2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFormatProperties2KHR>(vk_instance, "vkGetPhysicalDeviceFormatProperties2KHR");
            physical_device_driver.vkGetPhysicalDeviceImageFormatProperties2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceImageFormatProperties2KHR>(vk_instance, "vkGetPhysicalDeviceImageFormatProperties2KHR");
            physical_device_driver.vkGetPhysicalDeviceQueueFamilyProperties2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR>(vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
            physical_device_driver.vkGetPhysicalDeviceMemoryProperties2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceMemoryProperties2KHR>(vk_instance, "vkGetPhysicalDeviceMemoryProperties2KHR");
            physical_device_driver.vkGetPhysicalDeviceSparseImageFormatProperties2KHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR>(vk_instance, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
        }
    }
#endif

#if defined(VK_KHR_external_memory_capabilities)
    if (instance->IsEnabledExtension(TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES))
    {
        if (instance->IsSupportExtension(TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES))
        {
            physical_device_driver.vkGetPhysicalDeviceExternalBufferPropertiesKHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR>(vk_instance, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
        }
    }
#endif

#if defined(VK_KHR_external_fence_capabilities)
    if (instance->IsEnabledExtension(TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES))
    {
        if (instance->IsSupportExtension(TExtensionType::VK_KHR_EXTERNAL_FENCE_CAPABILITIES))
        {
            physical_device_driver.vkGetPhysicalDeviceExternalFencePropertiesKHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR>(vk_instance, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
        }
    }
#endif

#if defined(VK_KHR_external_semaphore_capabilities)
    if (instance->IsEnabledExtension(TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES))
    {
        if (instance->IsSupportExtension(TExtensionType::VK_KHR_EXTERNAL_SEMAPHORE_CAPABILITIES))
        {
            physical_device_driver.vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR>(vk_instance, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
        }
    }
#endif

    return physical_device_driver;
}

Turbo::Core::TDeviceDriver Turbo::Core::TVulkanLoader::LoadDeviceDriver(TDevice *device)
{
    TDeviceDriver device_driver = {};
    Turbo::Core::TVersion vulkan_version = device->GetPhysicalDevice()->GetInstance()->GetVulkanVersion();

#if defined(VK_VERSION_1_0)
    device_driver.vkDestroyDevice = this->LoadDeviceFunction<PFN_vkDestroyDevice>(device, "vkDestroyDevice");
    device_driver.vkAllocateCommandBuffers = this->LoadDeviceFunction<PFN_vkAllocateCommandBuffers>(device, "vkAllocateCommandBuffers");
    device_driver.vkAllocateDescriptorSets = this->LoadDeviceFunction<PFN_vkAllocateDescriptorSets>(device, "vkAllocateDescriptorSets");
    device_driver.vkAllocateMemory = this->LoadDeviceFunction<PFN_vkAllocateMemory>(device, "vkAllocateMemory");
    device_driver.vkBeginCommandBuffer = this->LoadDeviceFunction<PFN_vkBeginCommandBuffer>(device, "vkBeginCommandBuffer");
    device_driver.vkBindBufferMemory = this->LoadDeviceFunction<PFN_vkBindBufferMemory>(device, "vkBindBufferMemory");
    device_driver.vkBindImageMemory = this->LoadDeviceFunction<PFN_vkBindImageMemory>(device, "vkBindImageMemory");
    device_driver.vkCmdBeginQuery = this->LoadDeviceFunction<PFN_vkCmdBeginQuery>(device, "vkCmdBeginQuery");
    device_driver.vkCmdBeginRenderPass = this->LoadDeviceFunction<PFN_vkCmdBeginRenderPass>(device, "vkCmdBeginRenderPass");
    device_driver.vkCmdBindDescriptorSets = this->LoadDeviceFunction<PFN_vkCmdBindDescriptorSets>(device, "vkCmdBindDescriptorSets");
    device_driver.vkCmdBindIndexBuffer = this->LoadDeviceFunction<PFN_vkCmdBindIndexBuffer>(device, "vkCmdBindIndexBuffer");
    device_driver.vkCmdBindPipeline = this->LoadDeviceFunction<PFN_vkCmdBindPipeline>(device, "vkCmdBindPipeline");
    device_driver.vkCmdBindVertexBuffers = this->LoadDeviceFunction<PFN_vkCmdBindVertexBuffers>(device, "vkCmdBindVertexBuffers");
    device_driver.vkCmdBlitImage = this->LoadDeviceFunction<PFN_vkCmdBlitImage>(device, "vkCmdBlitImage");
    device_driver.vkCmdClearAttachments = this->LoadDeviceFunction<PFN_vkCmdClearAttachments>(device, "vkCmdClearAttachments");
    device_driver.vkCmdClearColorImage = this->LoadDeviceFunction<PFN_vkCmdClearColorImage>(device, "vkCmdClearColorImage");
    device_driver.vkCmdClearDepthStencilImage = this->LoadDeviceFunction<PFN_vkCmdClearDepthStencilImage>(device, "vkCmdClearDepthStencilImage");
    device_driver.vkCmdCopyBuffer = this->LoadDeviceFunction<PFN_vkCmdCopyBuffer>(device, "vkCmdCopyBuffer");
    device_driver.vkCmdCopyBufferToImage = this->LoadDeviceFunction<PFN_vkCmdCopyBufferToImage>(device, "vkCmdCopyBufferToImage");
    device_driver.vkCmdCopyImage = this->LoadDeviceFunction<PFN_vkCmdCopyImage>(device, "vkCmdCopyImage");
    device_driver.vkCmdCopyImageToBuffer = this->LoadDeviceFunction<PFN_vkCmdCopyImageToBuffer>(device, "vkCmdCopyImageToBuffer");
    device_driver.vkCmdCopyQueryPoolResults = this->LoadDeviceFunction<PFN_vkCmdCopyQueryPoolResults>(device, "vkCmdCopyQueryPoolResults");
    device_driver.vkCmdDispatch = this->LoadDeviceFunction<PFN_vkCmdDispatch>(device, "vkCmdDispatch");
    device_driver.vkCmdDispatchIndirect = this->LoadDeviceFunction<PFN_vkCmdDispatchIndirect>(device, "vkCmdDispatchIndirect");
    device_driver.vkCmdDraw = this->LoadDeviceFunction<PFN_vkCmdDraw>(device, "vkCmdDraw");
    device_driver.vkCmdDrawIndexed = this->LoadDeviceFunction<PFN_vkCmdDrawIndexed>(device, "vkCmdDrawIndexed");
    device_driver.vkCmdDrawIndexedIndirect = this->LoadDeviceFunction<PFN_vkCmdDrawIndexedIndirect>(device, "vkCmdDrawIndexedIndirect");
    device_driver.vkCmdDrawIndirect = this->LoadDeviceFunction<PFN_vkCmdDrawIndirect>(device, "vkCmdDrawIndirect");
    device_driver.vkCmdEndQuery = this->LoadDeviceFunction<PFN_vkCmdEndQuery>(device, "vkCmdEndQuery");
    device_driver.vkCmdEndRenderPass = this->LoadDeviceFunction<PFN_vkCmdEndRenderPass>(device, "vkCmdEndRenderPass");
    device_driver.vkCmdExecuteCommands = this->LoadDeviceFunction<PFN_vkCmdExecuteCommands>(device, "vkCmdExecuteCommands");
    device_driver.vkCmdFillBuffer = this->LoadDeviceFunction<PFN_vkCmdFillBuffer>(device, "vkCmdFillBuffer");
    device_driver.vkCmdNextSubpass = this->LoadDeviceFunction<PFN_vkCmdNextSubpass>(device, "vkCmdNextSubpass");
    device_driver.vkCmdPipelineBarrier = this->LoadDeviceFunction<PFN_vkCmdPipelineBarrier>(device, "vkCmdPipelineBarrier");
    device_driver.vkCmdPushConstants = this->LoadDeviceFunction<PFN_vkCmdPushConstants>(device, "vkCmdPushConstants");
    device_driver.vkCmdResetEvent = this->LoadDeviceFunction<PFN_vkCmdResetEvent>(device, "vkCmdResetEvent");
    device_driver.vkCmdResetQueryPool = this->LoadDeviceFunction<PFN_vkCmdResetQueryPool>(device, "vkCmdResetQueryPool");
    device_driver.vkCmdResolveImage = this->LoadDeviceFunction<PFN_vkCmdResolveImage>(device, "vkCmdResolveImage");
    device_driver.vkCmdSetBlendConstants = this->LoadDeviceFunction<PFN_vkCmdSetBlendConstants>(device, "vkCmdSetBlendConstants");
    device_driver.vkCmdSetDepthBias = this->LoadDeviceFunction<PFN_vkCmdSetDepthBias>(device, "vkCmdSetDepthBias");
    device_driver.vkCmdSetDepthBounds = this->LoadDeviceFunction<PFN_vkCmdSetDepthBounds>(device, "vkCmdSetDepthBounds");
    device_driver.vkCmdSetEvent = this->LoadDeviceFunction<PFN_vkCmdSetEvent>(device, "vkCmdSetEvent");
    device_driver.vkCmdSetLineWidth = this->LoadDeviceFunction<PFN_vkCmdSetLineWidth>(device, "vkCmdSetLineWidth");
    device_driver.vkCmdSetScissor = this->LoadDeviceFunction<PFN_vkCmdSetScissor>(device, "vkCmdSetScissor");
    device_driver.vkCmdSetStencilCompareMask = this->LoadDeviceFunction<PFN_vkCmdSetStencilCompareMask>(device, "vkCmdSetStencilCompareMask");
    device_driver.vkCmdSetStencilReference = this->LoadDeviceFunction<PFN_vkCmdSetStencilReference>(device, "vkCmdSetStencilReference");
    device_driver.vkCmdSetStencilWriteMask = this->LoadDeviceFunction<PFN_vkCmdSetStencilWriteMask>(device, "vkCmdSetStencilWriteMask");
    device_driver.vkCmdSetViewport = this->LoadDeviceFunction<PFN_vkCmdSetViewport>(device, "vkCmdSetViewport");
    device_driver.vkCmdUpdateBuffer = this->LoadDeviceFunction<PFN_vkCmdUpdateBuffer>(device, "vkCmdUpdateBuffer");
    device_driver.vkCmdWaitEvents = this->LoadDeviceFunction<PFN_vkCmdWaitEvents>(device, "vkCmdWaitEvents");
    device_driver.vkCmdWriteTimestamp = this->LoadDeviceFunction<PFN_vkCmdWriteTimestamp>(device, "vkCmdWriteTimestamp");
    device_driver.vkCreateBuffer = this->LoadDeviceFunction<PFN_vkCreateBuffer>(device, "vkCreateBuffer");
    device_driver.vkCreateBufferView = this->LoadDeviceFunction<PFN_vkCreateBufferView>(device, "vkCreateBufferView");
    device_driver.vkCreateCommandPool = this->LoadDeviceFunction<PFN_vkCreateCommandPool>(device, "vkCreateCommandPool");
    device_driver.vkCreateComputePipelines = this->LoadDeviceFunction<PFN_vkCreateComputePipelines>(device, "vkCreateComputePipelines");
    device_driver.vkCreateDescriptorPool = this->LoadDeviceFunction<PFN_vkCreateDescriptorPool>(device, "vkCreateDescriptorPool");
    device_driver.vkCreateDescriptorSetLayout = this->LoadDeviceFunction<PFN_vkCreateDescriptorSetLayout>(device, "vkCreateDescriptorSetLayout");
    device_driver.vkCreateEvent = this->LoadDeviceFunction<PFN_vkCreateEvent>(device, "vkCreateEvent");
    device_driver.vkCreateFence = this->LoadDeviceFunction<PFN_vkCreateFence>(device, "vkCreateFence");
    device_driver.vkCreateFramebuffer = this->LoadDeviceFunction<PFN_vkCreateFramebuffer>(device, "vkCreateFramebuffer");
    device_driver.vkCreateGraphicsPipelines = this->LoadDeviceFunction<PFN_vkCreateGraphicsPipelines>(device, "vkCreateGraphicsPipelines");
    device_driver.vkCreateImage = this->LoadDeviceFunction<PFN_vkCreateImage>(device, "vkCreateImage");
    device_driver.vkCreateImageView = this->LoadDeviceFunction<PFN_vkCreateImageView>(device, "vkCreateImageView");
    device_driver.vkCreatePipelineCache = this->LoadDeviceFunction<PFN_vkCreatePipelineCache>(device, "vkCreatePipelineCache");
    device_driver.vkCreatePipelineLayout = this->LoadDeviceFunction<PFN_vkCreatePipelineLayout>(device, "vkCreatePipelineLayout");
    device_driver.vkCreateQueryPool = this->LoadDeviceFunction<PFN_vkCreateQueryPool>(device, "vkCreateQueryPool");
    device_driver.vkCreateRenderPass = this->LoadDeviceFunction<PFN_vkCreateRenderPass>(device, "vkCreateRenderPass");
    device_driver.vkCreateSampler = this->LoadDeviceFunction<PFN_vkCreateSampler>(device, "vkCreateSampler");
    device_driver.vkCreateSemaphore = this->LoadDeviceFunction<PFN_vkCreateSemaphore>(device, "vkCreateSemaphore");
    device_driver.vkCreateShaderModule = this->LoadDeviceFunction<PFN_vkCreateShaderModule>(device, "vkCreateShaderModule");
    device_driver.vkDestroyBuffer = this->LoadDeviceFunction<PFN_vkDestroyBuffer>(device, "vkDestroyBuffer");
    device_driver.vkDestroyBufferView = this->LoadDeviceFunction<PFN_vkDestroyBufferView>(device, "vkDestroyBufferView");
    device_driver.vkDestroyCommandPool = this->LoadDeviceFunction<PFN_vkDestroyCommandPool>(device, "vkDestroyCommandPool");
    device_driver.vkDestroyDescriptorPool = this->LoadDeviceFunction<PFN_vkDestroyDescriptorPool>(device, "vkDestroyDescriptorPool");
    device_driver.vkDestroyDescriptorSetLayout = this->LoadDeviceFunction<PFN_vkDestroyDescriptorSetLayout>(device, "vkDestroyDescriptorSetLayout");
    device_driver.vkDestroyEvent = this->LoadDeviceFunction<PFN_vkDestroyEvent>(device, "vkDestroyEvent");
    device_driver.vkDestroyFence = this->LoadDeviceFunction<PFN_vkDestroyFence>(device, "vkDestroyFence");
    device_driver.vkDestroyFramebuffer = this->LoadDeviceFunction<PFN_vkDestroyFramebuffer>(device, "vkDestroyFramebuffer");
    device_driver.vkDestroyImage = this->LoadDeviceFunction<PFN_vkDestroyImage>(device, "vkDestroyImage");
    device_driver.vkDestroyImageView = this->LoadDeviceFunction<PFN_vkDestroyImageView>(device, "vkDestroyImageView");
    device_driver.vkDestroyPipeline = this->LoadDeviceFunction<PFN_vkDestroyPipeline>(device, "vkDestroyPipeline");
    device_driver.vkDestroyPipelineCache = this->LoadDeviceFunction<PFN_vkDestroyPipelineCache>(device, "vkDestroyPipelineCache");
    device_driver.vkDestroyPipelineLayout = this->LoadDeviceFunction<PFN_vkDestroyPipelineLayout>(device, "vkDestroyPipelineLayout");
    device_driver.vkDestroyQueryPool = this->LoadDeviceFunction<PFN_vkDestroyQueryPool>(device, "vkDestroyQueryPool");
    device_driver.vkDestroyRenderPass = this->LoadDeviceFunction<PFN_vkDestroyRenderPass>(device, "vkDestroyRenderPass");
    device_driver.vkDestroySampler = this->LoadDeviceFunction<PFN_vkDestroySampler>(device, "vkDestroySampler");
    device_driver.vkDestroySemaphore = this->LoadDeviceFunction<PFN_vkDestroySemaphore>(device, "vkDestroySemaphore");
    device_driver.vkDestroyShaderModule = this->LoadDeviceFunction<PFN_vkDestroyShaderModule>(device, "vkDestroyShaderModule");
    device_driver.vkDeviceWaitIdle = this->LoadDeviceFunction<PFN_vkDeviceWaitIdle>(device, "vkDeviceWaitIdle");
    device_driver.vkEndCommandBuffer = this->LoadDeviceFunction<PFN_vkEndCommandBuffer>(device, "vkEndCommandBuffer");
    device_driver.vkFlushMappedMemoryRanges = this->LoadDeviceFunction<PFN_vkFlushMappedMemoryRanges>(device, "vkFlushMappedMemoryRanges");
    device_driver.vkFreeCommandBuffers = this->LoadDeviceFunction<PFN_vkFreeCommandBuffers>(device, "vkFreeCommandBuffers");
    device_driver.vkFreeDescriptorSets = this->LoadDeviceFunction<PFN_vkFreeDescriptorSets>(device, "vkFreeDescriptorSets");
    device_driver.vkFreeMemory = this->LoadDeviceFunction<PFN_vkFreeMemory>(device, "vkFreeMemory");
    device_driver.vkGetBufferMemoryRequirements = this->LoadDeviceFunction<PFN_vkGetBufferMemoryRequirements>(device, "vkGetBufferMemoryRequirements");
    device_driver.vkGetDeviceMemoryCommitment = this->LoadDeviceFunction<PFN_vkGetDeviceMemoryCommitment>(device, "vkGetDeviceMemoryCommitment");
    device_driver.vkGetDeviceQueue = this->LoadDeviceFunction<PFN_vkGetDeviceQueue>(device, "vkGetDeviceQueue");
    device_driver.vkGetEventStatus = this->LoadDeviceFunction<PFN_vkGetEventStatus>(device, "vkGetEventStatus");
    device_driver.vkGetFenceStatus = this->LoadDeviceFunction<PFN_vkGetFenceStatus>(device, "vkGetFenceStatus");
    device_driver.vkGetImageMemoryRequirements = this->LoadDeviceFunction<PFN_vkGetImageMemoryRequirements>(device, "vkGetImageMemoryRequirements");
    device_driver.vkGetImageSparseMemoryRequirements = this->LoadDeviceFunction<PFN_vkGetImageSparseMemoryRequirements>(device, "vkGetImageSparseMemoryRequirements");
    device_driver.vkGetImageSubresourceLayout = this->LoadDeviceFunction<PFN_vkGetImageSubresourceLayout>(device, "vkGetImageSubresourceLayout");
    device_driver.vkGetPipelineCacheData = this->LoadDeviceFunction<PFN_vkGetPipelineCacheData>(device, "vkGetPipelineCacheData");
    device_driver.vkGetQueryPoolResults = this->LoadDeviceFunction<PFN_vkGetQueryPoolResults>(device, "vkGetQueryPoolResults");
    device_driver.vkGetRenderAreaGranularity = this->LoadDeviceFunction<PFN_vkGetRenderAreaGranularity>(device, "vkGetRenderAreaGranularity");
    device_driver.vkInvalidateMappedMemoryRanges = this->LoadDeviceFunction<PFN_vkInvalidateMappedMemoryRanges>(device, "vkInvalidateMappedMemoryRanges");
    device_driver.vkMapMemory = this->LoadDeviceFunction<PFN_vkMapMemory>(device, "vkMapMemory");
    device_driver.vkMergePipelineCaches = this->LoadDeviceFunction<PFN_vkMergePipelineCaches>(device, "vkMergePipelineCaches");
    device_driver.vkQueueBindSparse = this->LoadDeviceFunction<PFN_vkQueueBindSparse>(device, "vkQueueBindSparse");
    device_driver.vkQueueSubmit = this->LoadDeviceFunction<PFN_vkQueueSubmit>(device, "vkQueueSubmit");
    device_driver.vkQueueWaitIdle = this->LoadDeviceFunction<PFN_vkQueueWaitIdle>(device, "vkQueueWaitIdle");
    device_driver.vkResetCommandBuffer = this->LoadDeviceFunction<PFN_vkResetCommandBuffer>(device, "vkResetCommandBuffer");
    device_driver.vkResetCommandPool = this->LoadDeviceFunction<PFN_vkResetCommandPool>(device, "vkResetCommandPool");
    device_driver.vkResetDescriptorPool = this->LoadDeviceFunction<PFN_vkResetDescriptorPool>(device, "vkResetDescriptorPool");
    device_driver.vkResetEvent = this->LoadDeviceFunction<PFN_vkResetEvent>(device, "vkResetEvent");
    device_driver.vkResetFences = this->LoadDeviceFunction<PFN_vkResetFences>(device, "vkResetFences");
    device_driver.vkSetEvent = this->LoadDeviceFunction<PFN_vkSetEvent>(device, "vkSetEvent");
    device_driver.vkUnmapMemory = this->LoadDeviceFunction<PFN_vkUnmapMemory>(device, "vkUnmapMemory");
    device_driver.vkUpdateDescriptorSets = this->LoadDeviceFunction<PFN_vkUpdateDescriptorSets>(device, "vkUpdateDescriptorSets");
    device_driver.vkWaitForFences = this->LoadDeviceFunction<PFN_vkWaitForFences>(device, "vkWaitForFences");
#endif

#if defined(VK_VERSION_1_1)
    if (vulkan_version >= Turbo::Core::TVersion(1, 1, 0, 0))
    {
        device_driver.vkCmdDispatchBase = this->LoadDeviceFunction<PFN_vkCmdDispatchBase>(device, "vkCmdDispatchBase");
        device_driver.vkCmdSetDeviceMask = this->LoadDeviceFunction<PFN_vkCmdSetDeviceMask>(device, "vkCmdSetDeviceMask");
        device_driver.vkGetDeviceGroupPeerMemoryFeatures = this->LoadDeviceFunction<PFN_vkGetDeviceGroupPeerMemoryFeatures>(device, "vkGetDeviceGroupPeerMemoryFeatures");
        device_driver.vkGetDescriptorSetLayoutSupport = this->LoadDeviceFunction<PFN_vkGetDescriptorSetLayoutSupport>(device, "vkGetDescriptorSetLayoutSupport");
    }
#endif

#if defined(VK_VERSION_1_2)
    if (vulkan_version >= Turbo::Core::TVersion(1, 2, 0, 0))
    {
        device_driver.vkGetBufferDeviceAddress = this->LoadDeviceFunction<PFN_vkGetBufferDeviceAddress>(device, "vkGetBufferDeviceAddress");
        device_driver.vkGetBufferOpaqueCaptureAddress = this->LoadDeviceFunction<PFN_vkGetBufferOpaqueCaptureAddress>(device, "vkGetBufferOpaqueCaptureAddress");
        device_driver.vkGetDeviceMemoryOpaqueCaptureAddress = this->LoadDeviceFunction<PFN_vkGetDeviceMemoryOpaqueCaptureAddress>(device, "vkGetDeviceMemoryOpaqueCaptureAddress");
    }
#endif

#if defined(VK_VERSION_1_3)
    // FIXME: Determine the version of Vulkan version
    // TODO: add the Extension version function of VK_KHR_dynamic_rendering
    // TODO: vkCmdBeginRenderingKHR(After TDevice::TDevice() fixed the extension dependencies)
    // TODO: vkCmdEndRenderingKHR(After TDevice::TDevice() fixed the extension dependencies)
    if (vulkan_version >= Turbo::Core::TVersion(1, 3, 0, 0))
    {
        device_driver.vkCmdBeginRendering = this->LoadDeviceFunction<PFN_vkCmdBeginRendering>(device, "vkCmdBeginRendering");
        device_driver.vkCmdEndRendering = this->LoadDeviceFunction<PFN_vkCmdEndRendering>(device, "vkCmdEndRendering");
    }
#endif

#if defined(VK_EXT_mesh_shader)
    if (device->IsEnabledExtension(TExtensionType::VK_EXT_MESH_SHADER))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_EXT_MESH_SHADER))
        {
            device_driver.vkCmdDrawMeshTasksEXT = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksEXT>(device, "vkCmdDrawMeshTasksEXT");
            device_driver.vkCmdDrawMeshTasksIndirectCountEXT = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectCountEXT>(device, "vkCmdDrawMeshTasksIndirectCountEXT");
            device_driver.vkCmdDrawMeshTasksIndirectEXT = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectEXT>(device, "vkCmdDrawMeshTasksIndirectEXT");
        }
    }
#endif

#if defined(VK_NV_mesh_shader)
    if (device->IsEnabledExtension(TExtensionType::VK_NV_MESH_SHADER))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_NV_MESH_SHADER))
        {
            device_driver.vkCmdDrawMeshTasksIndirectCountNV = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectCountNV>(device, "vkCmdDrawMeshTasksIndirectCountNV");
            device_driver.vkCmdDrawMeshTasksIndirectNV = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectNV>(device, "vkCmdDrawMeshTasksIndirectNV");
            device_driver.vkCmdDrawMeshTasksNV = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksNV>(device, "vkCmdDrawMeshTasksNV");
        }
    }
#endif

#if defined(VK_KHR_acceleration_structure)
    if (device->IsEnabledExtension(TExtensionType::VK_KHR_ACCELERATION_STRUCTURE))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_ACCELERATION_STRUCTURE))
        {
            device_driver.vkBuildAccelerationStructuresKHR = this->LoadDeviceFunction<PFN_vkBuildAccelerationStructuresKHR>(device, "vkBuildAccelerationStructuresKHR");
            device_driver.vkCmdBuildAccelerationStructuresIndirectKHR = this->LoadDeviceFunction<PFN_vkCmdBuildAccelerationStructuresIndirectKHR>(device, "vkCmdBuildAccelerationStructuresIndirectKHR");
            device_driver.vkCmdBuildAccelerationStructuresKHR = this->LoadDeviceFunction<PFN_vkCmdBuildAccelerationStructuresKHR>(device, "vkCmdBuildAccelerationStructuresKHR");
            device_driver.vkCmdCopyAccelerationStructureKHR = this->LoadDeviceFunction<PFN_vkCmdCopyAccelerationStructureKHR>(device, "vkCmdCopyAccelerationStructureKHR");
            device_driver.vkCmdCopyAccelerationStructureToMemoryKHR = this->LoadDeviceFunction<PFN_vkCmdCopyAccelerationStructureToMemoryKHR>(device, "vkCmdCopyAccelerationStructureToMemoryKHR");
            device_driver.vkCmdCopyMemoryToAccelerationStructureKHR = this->LoadDeviceFunction<PFN_vkCmdCopyMemoryToAccelerationStructureKHR>(device, "vkCmdCopyMemoryToAccelerationStructureKHR");
            device_driver.vkCmdWriteAccelerationStructuresPropertiesKHR = this->LoadDeviceFunction<PFN_vkCmdWriteAccelerationStructuresPropertiesKHR>(device, "vkCmdWriteAccelerationStructuresPropertiesKHR");
            device_driver.vkCopyAccelerationStructureKHR = this->LoadDeviceFunction<PFN_vkCopyAccelerationStructureKHR>(device, "vkCopyAccelerationStructureKHR");
            device_driver.vkCopyAccelerationStructureToMemoryKHR = this->LoadDeviceFunction<PFN_vkCopyAccelerationStructureToMemoryKHR>(device, "vkCopyAccelerationStructureToMemoryKHR");
            device_driver.vkCopyMemoryToAccelerationStructureKHR = this->LoadDeviceFunction<PFN_vkCopyMemoryToAccelerationStructureKHR>(device, "vkCopyMemoryToAccelerationStructureKHR");
            device_driver.vkCreateAccelerationStructureKHR = this->LoadDeviceFunction<PFN_vkCreateAccelerationStructureKHR>(device, "vkCreateAccelerationStructureKHR");
            device_driver.vkDestroyAccelerationStructureKHR = this->LoadDeviceFunction<PFN_vkDestroyAccelerationStructureKHR>(device, "vkDestroyAccelerationStructureKHR");
            device_driver.vkGetAccelerationStructureBuildSizesKHR = this->LoadDeviceFunction<PFN_vkGetAccelerationStructureBuildSizesKHR>(device, "vkGetAccelerationStructureBuildSizesKHR");
            device_driver.vkGetAccelerationStructureDeviceAddressKHR = this->LoadDeviceFunction<PFN_vkGetAccelerationStructureDeviceAddressKHR>(device, "vkGetAccelerationStructureDeviceAddressKHR");
            device_driver.vkGetDeviceAccelerationStructureCompatibilityKHR = this->LoadDeviceFunction<PFN_vkGetDeviceAccelerationStructureCompatibilityKHR>(device, "vkGetDeviceAccelerationStructureCompatibilityKHR");
            device_driver.vkWriteAccelerationStructuresPropertiesKHR = this->LoadDeviceFunction<PFN_vkWriteAccelerationStructuresPropertiesKHR>(device, "vkWriteAccelerationStructuresPropertiesKHR");
        }
    }
#endif

#if defined(VK_EXT_buffer_device_address)
    if (device->IsEnabledExtension(TExtensionType::VK_EXT_BUFFER_DEVICE_ADDRESS))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_EXT_BUFFER_DEVICE_ADDRESS))
        {
            device_driver.vkGetBufferDeviceAddressEXT = this->LoadDeviceFunction<PFN_vkGetBufferDeviceAddressEXT>(device, "vkGetBufferDeviceAddressEXT");
        }
    }
#endif
#if defined(VK_KHR_buffer_device_address)
    if (device->IsEnabledExtension(TExtensionType::VK_KHR_BUFFER_DEVICE_ADDRESS))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_BUFFER_DEVICE_ADDRESS))
        {
            device_driver.vkGetBufferDeviceAddressKHR = this->LoadDeviceFunction<PFN_vkGetBufferDeviceAddressKHR>(device, "vkGetBufferDeviceAddressKHR");
            device_driver.vkGetBufferOpaqueCaptureAddressKHR = this->LoadDeviceFunction<PFN_vkGetBufferOpaqueCaptureAddressKHR>(device, "vkGetBufferOpaqueCaptureAddressKHR");
            device_driver.vkGetDeviceMemoryOpaqueCaptureAddressKHR = this->LoadDeviceFunction<PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR>(device, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
        }
    }
#endif

#if defined(VK_KHR_device_group)
    if (device->IsEnabledExtension(TExtensionType::VK_KHR_DEVICE_GROUP))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_DEVICE_GROUP))
        {
            device_driver.vkCmdDispatchBaseKHR = this->LoadDeviceFunction<PFN_vkCmdDispatchBaseKHR>(device, "vkCmdDispatchBaseKHR");
            device_driver.vkCmdSetDeviceMaskKHR = this->LoadDeviceFunction<PFN_vkCmdSetDeviceMaskKHR>(device, "vkCmdSetDeviceMaskKHR");
            device_driver.vkGetDeviceGroupPeerMemoryFeaturesKHR = this->LoadDeviceFunction<PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR>(device, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
#if defined(VK_KHR_surface)
            if (device->IsEnabledExtension(TExtensionType::VK_KHR_SURFACE))
            {
                if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_SURFACE))
                {
                    // NOTE: If VK_KHR_surface is supported:
                    if (device_driver.vkGetDeviceGroupPresentCapabilitiesKHR == nullptr)
                    {
                        device_driver.vkGetDeviceGroupPresentCapabilitiesKHR = this->LoadDeviceFunction<PFN_vkGetDeviceGroupPresentCapabilitiesKHR>(device, "vkGetDeviceGroupPresentCapabilitiesKHR");
                    }
                    if (device_driver.vkGetDeviceGroupSurfacePresentModesKHR == nullptr)
                    {
                        device_driver.vkGetDeviceGroupSurfacePresentModesKHR = this->LoadDeviceFunction<PFN_vkGetDeviceGroupSurfacePresentModesKHR>(device, "vkGetDeviceGroupSurfacePresentModesKHR");
                    }
                    if (device_driver.vkGetPhysicalDevicePresentRectanglesKHR == nullptr)
                    {
                        device_driver.vkGetPhysicalDevicePresentRectanglesKHR = this->LoadDeviceFunction<PFN_vkGetPhysicalDevicePresentRectanglesKHR>(device, "vkGetPhysicalDevicePresentRectanglesKHR");
                    }
                }
            }
#endif
#if defined(VK_KHR_swapchain)
            if (device->IsEnabledExtension(TExtensionType::VK_KHR_SWAPCHAIN))
            {
                if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_SWAPCHAIN))
                {
                    // NOTE: If VK_KHR_swapchain is supported:
                    if (device_driver.vkAcquireNextImage2KHR == nullptr)
                    {
                        device_driver.vkAcquireNextImage2KHR = this->LoadDeviceFunction<PFN_vkAcquireNextImage2KHR>(device, "vkAcquireNextImage2KHR");
                    }
                }
            }
#endif
        }
    }
#endif

#if defined(VK_EXT_tooling_info)
    if (device->IsEnabledExtension(TExtensionType::VK_EXT_TOOLING_INFO))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_EXT_TOOLING_INFO))
        {
            device_driver.vkGetPhysicalDeviceToolPropertiesEXT = this->LoadDeviceFunction<PFN_vkGetPhysicalDeviceToolPropertiesEXT>(device, "vkGetPhysicalDeviceToolPropertiesEXT");
        }
    }
#endif

#if defined(VK_KHR_maintenance3)
    if (device->IsEnabledExtension(TExtensionType::VK_KHR_MAINTENANCE3))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_MAINTENANCE3))
        {
            device_driver.vkGetDescriptorSetLayoutSupportKHR = this->LoadDeviceFunction<PFN_vkGetDescriptorSetLayoutSupportKHR>(device, "vkGetDescriptorSetLayoutSupportKHR");
        }
    }
#endif

#if defined(VK_KHR_deferred_host_operations)
    if (device->IsEnabledExtension(TExtensionType::VK_KHR_DEFERRED_HOST_OPERATIONS))
    {
        if (device->GetPhysicalDevice()->IsSupportExtension(TExtensionType::VK_KHR_DEFERRED_HOST_OPERATIONS))
        {
            device_driver.vkCreateDeferredOperationKHR = this->LoadDeviceFunction<PFN_vkCreateDeferredOperationKHR>(device, "vkCreateDeferredOperationKHR");
            device_driver.vkDeferredOperationJoinKHR = this->LoadDeviceFunction<PFN_vkDeferredOperationJoinKHR>(device, "vkDeferredOperationJoinKHR");
            device_driver.vkDestroyDeferredOperationKHR = this->LoadDeviceFunction<PFN_vkDestroyDeferredOperationKHR>(device, "vkDestroyDeferredOperationKHR");
            device_driver.vkGetDeferredOperationMaxConcurrencyKHR = this->LoadDeviceFunction<PFN_vkGetDeferredOperationMaxConcurrencyKHR>(device, "vkGetDeferredOperationMaxConcurrencyKHR");
            device_driver.vkGetDeferredOperationResultKHR = this->LoadDeviceFunction<PFN_vkGetDeferredOperationResultKHR>(device, "vkGetDeferredOperationResultKHR");
        }
    }
#endif

    return device_driver;
}

std::string Turbo::Core::TVulkanLoader::ToString()
{
    return std::string();
}
