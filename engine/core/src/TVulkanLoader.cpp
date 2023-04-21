#include "TVulkanLoader.h"

#if defined(TURBO_PLATFORM_LINUX)
#include <dlfcn.h>
#elif defined(TURBO_PLATFORM_WINDOWS)
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
PFN_vkGetPhysicalDeviceSparseImageFormatProperties Turbo::Core::vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
PFN_vkCreateDevice Turbo::Core::vkCreateDevice = nullptr;

PFN_vkGetDeviceProcAddr Turbo::Core::vkGetDeviceProcAddr = nullptr;
PFN_vkDestroyDevice Turbo::Core::vkDestroyDevice = nullptr;

PFN_vkAllocateCommandBuffers Turbo::Core::vkAllocateCommandBuffers = nullptr;
PFN_vkAllocateDescriptorSets Turbo::Core::vkAllocateDescriptorSets = nullptr;
PFN_vkAllocateMemory Turbo::Core::vkAllocateMemory = nullptr;
PFN_vkBeginCommandBuffer Turbo::Core::vkBeginCommandBuffer = nullptr;
PFN_vkBindBufferMemory Turbo::Core::vkBindBufferMemory = nullptr;
PFN_vkBindImageMemory Turbo::Core::vkBindImageMemory = nullptr;
PFN_vkCmdBeginQuery Turbo::Core::vkCmdBeginQuery = nullptr;
PFN_vkCmdBeginRenderPass Turbo::Core::vkCmdBeginRenderPass = nullptr;
PFN_vkCmdBindDescriptorSets Turbo::Core::vkCmdBindDescriptorSets = nullptr;
PFN_vkCmdBindIndexBuffer Turbo::Core::vkCmdBindIndexBuffer = nullptr;
PFN_vkCmdBindPipeline Turbo::Core::vkCmdBindPipeline = nullptr;
PFN_vkCmdBindVertexBuffers Turbo::Core::vkCmdBindVertexBuffers = nullptr;
PFN_vkCmdBlitImage Turbo::Core::vkCmdBlitImage = nullptr;
PFN_vkCmdClearAttachments Turbo::Core::vkCmdClearAttachments = nullptr;
PFN_vkCmdClearColorImage Turbo::Core::vkCmdClearColorImage;
PFN_vkCmdClearDepthStencilImage Turbo::Core::vkCmdClearDepthStencilImage = nullptr;
PFN_vkCmdCopyBuffer Turbo::Core::vkCmdCopyBuffer = nullptr;
PFN_vkCmdCopyBufferToImage Turbo::Core::vkCmdCopyBufferToImage = nullptr;
PFN_vkCmdCopyImage Turbo::Core::vkCmdCopyImage = nullptr;
PFN_vkCmdCopyImageToBuffer Turbo::Core::vkCmdCopyImageToBuffer = nullptr;
PFN_vkCmdCopyQueryPoolResults Turbo::Core::vkCmdCopyQueryPoolResults = nullptr;
PFN_vkCmdDispatch Turbo::Core::vkCmdDispatch = nullptr;
PFN_vkCmdDispatchIndirect Turbo::Core::vkCmdDispatchIndirect = nullptr;
PFN_vkCmdDraw Turbo::Core::vkCmdDraw = nullptr;
PFN_vkCmdDrawIndexed Turbo::Core::vkCmdDrawIndexed = nullptr;
PFN_vkCmdDrawIndexedIndirect Turbo::Core::vkCmdDrawIndexedIndirect = nullptr;
PFN_vkCmdDrawIndirect Turbo::Core::vkCmdDrawIndirect = nullptr;
PFN_vkCmdEndQuery Turbo::Core::vkCmdEndQuery = nullptr;
PFN_vkCmdEndRenderPass Turbo::Core::vkCmdEndRenderPass = nullptr;
PFN_vkCmdExecuteCommands Turbo::Core::vkCmdExecuteCommands = nullptr;
PFN_vkCmdFillBuffer Turbo::Core::vkCmdFillBuffer = nullptr;
PFN_vkCmdNextSubpass Turbo::Core::vkCmdNextSubpass = nullptr;
PFN_vkCmdPipelineBarrier Turbo::Core::vkCmdPipelineBarrier = nullptr;
PFN_vkCmdPushConstants Turbo::Core::vkCmdPushConstants = nullptr;
PFN_vkCmdResetEvent Turbo::Core::vkCmdResetEvent = nullptr;
PFN_vkCmdResetQueryPool Turbo::Core::vkCmdResetQueryPool = nullptr;
PFN_vkCmdResolveImage Turbo::Core::vkCmdResolveImage = nullptr;
PFN_vkCmdSetBlendConstants Turbo::Core::vkCmdSetBlendConstants = nullptr;
PFN_vkCmdSetDepthBias Turbo::Core::vkCmdSetDepthBias = nullptr;
PFN_vkCmdSetDepthBounds Turbo::Core::vkCmdSetDepthBounds = nullptr;
PFN_vkCmdSetEvent Turbo::Core::vkCmdSetEvent = nullptr;
PFN_vkCmdSetLineWidth Turbo::Core::vkCmdSetLineWidth = nullptr;
PFN_vkCmdSetScissor Turbo::Core::vkCmdSetScissor = nullptr;
PFN_vkCmdSetStencilCompareMask Turbo::Core::vkCmdSetStencilCompareMask = nullptr;
PFN_vkCmdSetStencilReference Turbo::Core::vkCmdSetStencilReference = nullptr;
PFN_vkCmdSetStencilWriteMask Turbo::Core::vkCmdSetStencilWriteMask = nullptr;
PFN_vkCmdSetViewport Turbo::Core::vkCmdSetViewport = nullptr;
PFN_vkCmdUpdateBuffer Turbo::Core::vkCmdUpdateBuffer = nullptr;
PFN_vkCmdWaitEvents Turbo::Core::vkCmdWaitEvents = nullptr;
PFN_vkCmdWriteTimestamp Turbo::Core::vkCmdWriteTimestamp = nullptr;
PFN_vkCreateBuffer Turbo::Core::vkCreateBuffer = nullptr;
PFN_vkCreateBufferView Turbo::Core::vkCreateBufferView = nullptr;
PFN_vkCreateCommandPool Turbo::Core::vkCreateCommandPool = nullptr;
PFN_vkCreateComputePipelines Turbo::Core::vkCreateComputePipelines = nullptr;
PFN_vkCreateDescriptorPool Turbo::Core::vkCreateDescriptorPool = nullptr;
PFN_vkCreateDescriptorSetLayout Turbo::Core::vkCreateDescriptorSetLayout = nullptr;
PFN_vkCreateEvent Turbo::Core::vkCreateEvent = nullptr;
PFN_vkCreateFence Turbo::Core::vkCreateFence = nullptr;
PFN_vkCreateFramebuffer Turbo::Core::vkCreateFramebuffer = nullptr;
PFN_vkCreateGraphicsPipelines Turbo::Core::vkCreateGraphicsPipelines = nullptr;
PFN_vkCreateImage Turbo::Core::vkCreateImage = nullptr;
PFN_vkCreateImageView Turbo::Core::vkCreateImageView = nullptr;
PFN_vkCreatePipelineCache Turbo::Core::vkCreatePipelineCache = nullptr;
PFN_vkCreatePipelineLayout Turbo::Core::vkCreatePipelineLayout = nullptr;
PFN_vkCreateQueryPool Turbo::Core::vkCreateQueryPool = nullptr;
PFN_vkCreateRenderPass Turbo::Core::vkCreateRenderPass = nullptr;
PFN_vkCreateSampler Turbo::Core::vkCreateSampler = nullptr;
PFN_vkCreateSemaphore Turbo::Core::vkCreateSemaphore = nullptr;
PFN_vkCreateShaderModule Turbo::Core::vkCreateShaderModule = nullptr;
PFN_vkDestroyBuffer Turbo::Core::vkDestroyBuffer = nullptr;
PFN_vkDestroyBufferView Turbo::Core::vkDestroyBufferView = nullptr;
PFN_vkDestroyCommandPool Turbo::Core::vkDestroyCommandPool = nullptr;
PFN_vkDestroyDescriptorPool Turbo::Core::vkDestroyDescriptorPool = nullptr;
PFN_vkDestroyDescriptorSetLayout Turbo::Core::vkDestroyDescriptorSetLayout = nullptr;
PFN_vkDestroyEvent Turbo::Core::vkDestroyEvent = nullptr;
PFN_vkDestroyFence Turbo::Core::vkDestroyFence = nullptr;
PFN_vkDestroyFramebuffer Turbo::Core::vkDestroyFramebuffer = nullptr;
PFN_vkDestroyImage Turbo::Core::vkDestroyImage = nullptr;
PFN_vkDestroyImageView Turbo::Core::vkDestroyImageView = nullptr;
PFN_vkDestroyPipeline Turbo::Core::vkDestroyPipeline = nullptr;
PFN_vkDestroyPipelineCache Turbo::Core::vkDestroyPipelineCache = nullptr;
PFN_vkDestroyPipelineLayout Turbo::Core::vkDestroyPipelineLayout = nullptr;
PFN_vkDestroyQueryPool Turbo::Core::vkDestroyQueryPool = nullptr;
PFN_vkDestroyRenderPass Turbo::Core::vkDestroyRenderPass = nullptr;
PFN_vkDestroySampler Turbo::Core::vkDestroySampler = nullptr;
PFN_vkDestroySemaphore Turbo::Core::vkDestroySemaphore = nullptr;
PFN_vkDestroyShaderModule Turbo::Core::vkDestroyShaderModule = nullptr;
PFN_vkDeviceWaitIdle Turbo::Core::vkDeviceWaitIdle = nullptr;
PFN_vkEndCommandBuffer Turbo::Core::vkEndCommandBuffer = nullptr;
PFN_vkFlushMappedMemoryRanges Turbo::Core::vkFlushMappedMemoryRanges = nullptr;
PFN_vkFreeCommandBuffers Turbo::Core::vkFreeCommandBuffers = nullptr;
PFN_vkFreeDescriptorSets Turbo::Core::vkFreeDescriptorSets = nullptr;
PFN_vkFreeMemory Turbo::Core::vkFreeMemory = nullptr;
PFN_vkGetBufferMemoryRequirements Turbo::Core::vkGetBufferMemoryRequirements = nullptr;
PFN_vkGetDeviceMemoryCommitment Turbo::Core::vkGetDeviceMemoryCommitment = nullptr;
PFN_vkGetDeviceQueue Turbo::Core::vkGetDeviceQueue = nullptr;
PFN_vkGetEventStatus Turbo::Core::vkGetEventStatus = nullptr;
PFN_vkGetFenceStatus Turbo::Core::vkGetFenceStatus = nullptr;
PFN_vkGetImageMemoryRequirements Turbo::Core::vkGetImageMemoryRequirements = nullptr;
PFN_vkGetImageSparseMemoryRequirements Turbo::Core::vkGetImageSparseMemoryRequirements = nullptr;
PFN_vkGetImageSubresourceLayout Turbo::Core::vkGetImageSubresourceLayout = nullptr;
PFN_vkGetPipelineCacheData Turbo::Core::vkGetPipelineCacheData = nullptr;
PFN_vkGetQueryPoolResults Turbo::Core::vkGetQueryPoolResults = nullptr;
PFN_vkGetRenderAreaGranularity Turbo::Core::vkGetRenderAreaGranularity = nullptr;
PFN_vkInvalidateMappedMemoryRanges Turbo::Core::vkInvalidateMappedMemoryRanges = nullptr;
PFN_vkMapMemory Turbo::Core::vkMapMemory = nullptr;
PFN_vkMergePipelineCaches Turbo::Core::vkMergePipelineCaches = nullptr;
PFN_vkQueueBindSparse Turbo::Core::vkQueueBindSparse = nullptr;
PFN_vkQueueSubmit Turbo::Core::vkQueueSubmit = nullptr;
PFN_vkQueueWaitIdle Turbo::Core::vkQueueWaitIdle = nullptr;
PFN_vkResetCommandBuffer Turbo::Core::vkResetCommandBuffer = nullptr;
PFN_vkResetCommandPool Turbo::Core::vkResetCommandPool = nullptr;
PFN_vkResetDescriptorPool Turbo::Core::vkResetDescriptorPool = nullptr;
PFN_vkResetEvent Turbo::Core::vkResetEvent = nullptr;
PFN_vkResetFences Turbo::Core::vkResetFences = nullptr;
PFN_vkSetEvent Turbo::Core::vkSetEvent = nullptr;
PFN_vkUnmapMemory Turbo::Core::vkUnmapMemory = nullptr;
PFN_vkUpdateDescriptorSets Turbo::Core::vkUpdateDescriptorSets = nullptr;
PFN_vkWaitForFences Turbo::Core::vkWaitForFences = nullptr;
#endif

#if defined(VK_VERSION_1_1)
#endif

#if defined(VK_VERSION_1_3)
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
    throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED, "Turbo::Core::TVulkanLoader::TVulkanLoader", "Please implement this platform definition");
#endif

//<loade global commands function>
#if defined(VK_VERSION_1_0)
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

void Turbo::Core::TVulkanLoader::LoadAllInstanceFunctions(TInstance *instance)
{
    VkInstance vk_instance = instance->GetVkInstance();
// load all instance vulkan api function, Vulkan Loader version
#if defined(VK_VERSION_1_0)
    Turbo::Core::vkDestroyInstance = this->LoadInstanceFunction<PFN_vkDestroyInstance>(vk_instance, "vkDestroyInstance");
    Turbo::Core::vkEnumeratePhysicalDevices = this->LoadInstanceFunction<PFN_vkEnumeratePhysicalDevices>(vk_instance, "vkEnumeratePhysicalDevices");
    Turbo::Core::vkGetPhysicalDeviceProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceProperties>(vk_instance, "vkGetPhysicalDeviceProperties");
    Turbo::Core::vkEnumerateDeviceLayerProperties = this->LoadInstanceFunction<PFN_vkEnumerateDeviceLayerProperties>(vk_instance, "vkEnumerateDeviceLayerProperties");
    Turbo::Core::vkEnumerateDeviceExtensionProperties = this->LoadInstanceFunction<PFN_vkEnumerateDeviceExtensionProperties>(vk_instance, "vkEnumerateDeviceExtensionProperties");
    Turbo::Core::vkGetPhysicalDeviceQueueFamilyProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vk_instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    Turbo::Core::vkGetPhysicalDeviceMemoryProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceMemoryProperties>(vk_instance, "vkGetPhysicalDeviceMemoryProperties");
    Turbo::Core::vkGetPhysicalDeviceFeatures = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFeatures>(vk_instance, "vkGetPhysicalDeviceFeatures");
    Turbo::Core::vkGetPhysicalDeviceFormatProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceFormatProperties>(vk_instance, "vkGetPhysicalDeviceFormatProperties");
    Turbo::Core::vkGetPhysicalDeviceImageFormatProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceImageFormatProperties>(vk_instance, "vkGetPhysicalDeviceImageFormatProperties");
    Turbo::Core::vkGetPhysicalDeviceSparseImageFormatProperties = this->LoadInstanceFunction<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(vk_instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
    Turbo::Core::vkCreateDevice = this->LoadInstanceFunction<PFN_vkCreateDevice>(vk_instance, "vkCreateDevice");

    Turbo::Core::vkGetDeviceProcAddr = this->LoadInstanceFunction<PFN_vkGetDeviceProcAddr>(vk_instance, "vkGetDeviceProcAddr");
#endif

#if defined(VK_VERSION_1_1)
#endif
}

void Turbo::Core::TVulkanLoader::LoadAllDeviceFunctions(TInstance *instance)
{
    VkInstance vk_instance = instance->GetVkInstance();
    // load all device vulkan api function, Vulkan Loader version
#if defined(VK_VERSION_1_0)
    Turbo::Core::vkDestroyDevice = this->LoadInstanceFunction<PFN_vkDestroyDevice>(vk_instance, "vkDestroyDevice");

    Turbo::Core::vkAllocateCommandBuffers = this->LoadInstanceFunction<PFN_vkAllocateCommandBuffers>(vk_instance, "vkAllocateCommandBuffers");
    Turbo::Core::vkAllocateDescriptorSets = this->LoadInstanceFunction<PFN_vkAllocateDescriptorSets>(vk_instance, "vkAllocateDescriptorSets");
    Turbo::Core::vkAllocateMemory = this->LoadInstanceFunction<PFN_vkAllocateMemory>(vk_instance, "vkAllocateMemory");
    Turbo::Core::vkBeginCommandBuffer = this->LoadInstanceFunction<PFN_vkBeginCommandBuffer>(vk_instance, "vkBeginCommandBuffer");
    Turbo::Core::vkBindBufferMemory = this->LoadInstanceFunction<PFN_vkBindBufferMemory>(vk_instance, "vkBindBufferMemory");
    Turbo::Core::vkBindImageMemory = this->LoadInstanceFunction<PFN_vkBindImageMemory>(vk_instance, "vkBindImageMemory");
    Turbo::Core::vkCmdBeginQuery = this->LoadInstanceFunction<PFN_vkCmdBeginQuery>(vk_instance, "vkCmdBeginQuery");
    Turbo::Core::vkCmdBeginRenderPass = this->LoadInstanceFunction<PFN_vkCmdBeginRenderPass>(vk_instance, "vkCmdBeginRenderPass");
    Turbo::Core::vkCmdBindDescriptorSets = this->LoadInstanceFunction<PFN_vkCmdBindDescriptorSets>(vk_instance, "vkCmdBindDescriptorSets");
    Turbo::Core::vkCmdBindIndexBuffer = this->LoadInstanceFunction<PFN_vkCmdBindIndexBuffer>(vk_instance, "vkCmdBindIndexBuffer");
    Turbo::Core::vkCmdBindPipeline = this->LoadInstanceFunction<PFN_vkCmdBindPipeline>(vk_instance, "vkCmdBindPipeline");
    Turbo::Core::vkCmdBindVertexBuffers = this->LoadInstanceFunction<PFN_vkCmdBindVertexBuffers>(vk_instance, "vkCmdBindVertexBuffers");
    Turbo::Core::vkCmdBlitImage = this->LoadInstanceFunction<PFN_vkCmdBlitImage>(vk_instance, "vkCmdBlitImage");
    Turbo::Core::vkCmdClearAttachments = this->LoadInstanceFunction<PFN_vkCmdClearAttachments>(vk_instance, "vkCmdClearAttachments");
    Turbo::Core::vkCmdClearColorImage = this->LoadInstanceFunction<PFN_vkCmdClearColorImage>(vk_instance, "vkCmdClearColorImage");
    Turbo::Core::vkCmdClearDepthStencilImage = this->LoadInstanceFunction<PFN_vkCmdClearDepthStencilImage>(vk_instance, "vkCmdClearDepthStencilImage");
    Turbo::Core::vkCmdCopyBuffer = this->LoadInstanceFunction<PFN_vkCmdCopyBuffer>(vk_instance, "vkCmdCopyBuffer");
    Turbo::Core::vkCmdCopyBufferToImage = this->LoadInstanceFunction<PFN_vkCmdCopyBufferToImage>(vk_instance, "vkCmdCopyBufferToImage");
    Turbo::Core::vkCmdCopyImage = this->LoadInstanceFunction<PFN_vkCmdCopyImage>(vk_instance, "vkCmdCopyImage");
    Turbo::Core::vkCmdCopyImageToBuffer = this->LoadInstanceFunction<PFN_vkCmdCopyImageToBuffer>(vk_instance, "vkCmdCopyImageToBuffer");
    Turbo::Core::vkCmdCopyQueryPoolResults = this->LoadInstanceFunction<PFN_vkCmdCopyQueryPoolResults>(vk_instance, "vkCmdCopyQueryPoolResults");
    Turbo::Core::vkCmdDispatch = this->LoadInstanceFunction<PFN_vkCmdDispatch>(vk_instance, "vkCmdDispatch");
    Turbo::Core::vkCmdDispatchIndirect = this->LoadInstanceFunction<PFN_vkCmdDispatchIndirect>(vk_instance, "vkCmdDispatchIndirect");
    Turbo::Core::vkCmdDraw = this->LoadInstanceFunction<PFN_vkCmdDraw>(vk_instance, "vkCmdDraw");
    Turbo::Core::vkCmdDrawIndexed = this->LoadInstanceFunction<PFN_vkCmdDrawIndexed>(vk_instance, "vkCmdDrawIndexed");
    Turbo::Core::vkCmdDrawIndexedIndirect = this->LoadInstanceFunction<PFN_vkCmdDrawIndexedIndirect>(vk_instance, "vkCmdDrawIndexedIndirect");
    Turbo::Core::vkCmdDrawIndirect = this->LoadInstanceFunction<PFN_vkCmdDrawIndirect>(vk_instance, "vkCmdDrawIndirect");
    Turbo::Core::vkCmdEndQuery = this->LoadInstanceFunction<PFN_vkCmdEndQuery>(vk_instance, "vkCmdEndQuery");
    Turbo::Core::vkCmdEndRenderPass = this->LoadInstanceFunction<PFN_vkCmdEndRenderPass>(vk_instance, "vkCmdEndRenderPass");
    Turbo::Core::vkCmdExecuteCommands = this->LoadInstanceFunction<PFN_vkCmdExecuteCommands>(vk_instance, "vkCmdExecuteCommands");
    Turbo::Core::vkCmdFillBuffer = this->LoadInstanceFunction<PFN_vkCmdFillBuffer>(vk_instance, "vkCmdFillBuffer");
    Turbo::Core::vkCmdNextSubpass = this->LoadInstanceFunction<PFN_vkCmdNextSubpass>(vk_instance, "vkCmdNextSubpass");
    Turbo::Core::vkCmdPipelineBarrier = this->LoadInstanceFunction<PFN_vkCmdPipelineBarrier>(vk_instance, "vkCmdPipelineBarrier");
    Turbo::Core::vkCmdPushConstants = this->LoadInstanceFunction<PFN_vkCmdPushConstants>(vk_instance, "vkCmdPushConstants");
    Turbo::Core::vkCmdResetEvent = this->LoadInstanceFunction<PFN_vkCmdResetEvent>(vk_instance, "vkCmdResetEvent");
    Turbo::Core::vkCmdResetQueryPool = this->LoadInstanceFunction<PFN_vkCmdResetQueryPool>(vk_instance, "vkCmdResetQueryPool");
    Turbo::Core::vkCmdResolveImage = this->LoadInstanceFunction<PFN_vkCmdResolveImage>(vk_instance, "vkCmdResolveImage");
    Turbo::Core::vkCmdSetBlendConstants = this->LoadInstanceFunction<PFN_vkCmdSetBlendConstants>(vk_instance, "vkCmdSetBlendConstants");
    Turbo::Core::vkCmdSetDepthBias = this->LoadInstanceFunction<PFN_vkCmdSetDepthBias>(vk_instance, "vkCmdSetDepthBias");
    Turbo::Core::vkCmdSetDepthBounds = this->LoadInstanceFunction<PFN_vkCmdSetDepthBounds>(vk_instance, "vkCmdSetDepthBounds");
    Turbo::Core::vkCmdSetEvent = this->LoadInstanceFunction<PFN_vkCmdSetEvent>(vk_instance, "vkCmdSetEvent");
    Turbo::Core::vkCmdSetLineWidth = this->LoadInstanceFunction<PFN_vkCmdSetLineWidth>(vk_instance, "vkCmdSetLineWidth");
    Turbo::Core::vkCmdSetScissor = this->LoadInstanceFunction<PFN_vkCmdSetScissor>(vk_instance, "vkCmdSetScissor");
    Turbo::Core::vkCmdSetStencilCompareMask = this->LoadInstanceFunction<PFN_vkCmdSetStencilCompareMask>(vk_instance, "vkCmdSetStencilCompareMask");
    Turbo::Core::vkCmdSetStencilReference = this->LoadInstanceFunction<PFN_vkCmdSetStencilReference>(vk_instance, "vkCmdSetStencilReference");
    Turbo::Core::vkCmdSetStencilWriteMask = this->LoadInstanceFunction<PFN_vkCmdSetStencilWriteMask>(vk_instance, "vkCmdSetStencilWriteMask");
    Turbo::Core::vkCmdSetViewport = this->LoadInstanceFunction<PFN_vkCmdSetViewport>(vk_instance, "vkCmdSetViewport");
    Turbo::Core::vkCmdUpdateBuffer = this->LoadInstanceFunction<PFN_vkCmdUpdateBuffer>(vk_instance, "vkCmdUpdateBuffer");
    Turbo::Core::vkCmdWaitEvents = this->LoadInstanceFunction<PFN_vkCmdWaitEvents>(vk_instance, "vkCmdWaitEvents");
    Turbo::Core::vkCmdWriteTimestamp = this->LoadInstanceFunction<PFN_vkCmdWriteTimestamp>(vk_instance, "vkCmdWriteTimestamp");
    Turbo::Core::vkCreateBuffer = this->LoadInstanceFunction<PFN_vkCreateBuffer>(vk_instance, "vkCreateBuffer");
    Turbo::Core::vkCreateBufferView = this->LoadInstanceFunction<PFN_vkCreateBufferView>(vk_instance, "vkCreateBufferView");
    Turbo::Core::vkCreateCommandPool = this->LoadInstanceFunction<PFN_vkCreateCommandPool>(vk_instance, "vkCreateCommandPool");
    Turbo::Core::vkCreateComputePipelines = this->LoadInstanceFunction<PFN_vkCreateComputePipelines>(vk_instance, "vkCreateComputePipelines");
    Turbo::Core::vkCreateDescriptorPool = this->LoadInstanceFunction<PFN_vkCreateDescriptorPool>(vk_instance, "vkCreateDescriptorPool");
    Turbo::Core::vkCreateDescriptorSetLayout = this->LoadInstanceFunction<PFN_vkCreateDescriptorSetLayout>(vk_instance, "vkCreateDescriptorSetLayout");
    Turbo::Core::vkCreateEvent = this->LoadInstanceFunction<PFN_vkCreateEvent>(vk_instance, "vkCreateEvent");
    Turbo::Core::vkCreateFence = this->LoadInstanceFunction<PFN_vkCreateFence>(vk_instance, "vkCreateFence");
    Turbo::Core::vkCreateFramebuffer = this->LoadInstanceFunction<PFN_vkCreateFramebuffer>(vk_instance, "vkCreateFramebuffer");
    Turbo::Core::vkCreateGraphicsPipelines = this->LoadInstanceFunction<PFN_vkCreateGraphicsPipelines>(vk_instance, "vkCreateGraphicsPipelines");
    Turbo::Core::vkCreateImage = this->LoadInstanceFunction<PFN_vkCreateImage>(vk_instance, "vkCreateImage");
    Turbo::Core::vkCreateImageView = this->LoadInstanceFunction<PFN_vkCreateImageView>(vk_instance, "vkCreateImageView");
    Turbo::Core::vkCreatePipelineCache = this->LoadInstanceFunction<PFN_vkCreatePipelineCache>(vk_instance, "vkCreatePipelineCache");
    Turbo::Core::vkCreatePipelineLayout = this->LoadInstanceFunction<PFN_vkCreatePipelineLayout>(vk_instance, "vkCreatePipelineLayout");
    Turbo::Core::vkCreateQueryPool = this->LoadInstanceFunction<PFN_vkCreateQueryPool>(vk_instance, "vkCreateQueryPool");
    Turbo::Core::vkCreateRenderPass = this->LoadInstanceFunction<PFN_vkCreateRenderPass>(vk_instance, "vkCreateRenderPass");
    Turbo::Core::vkCreateSampler = this->LoadInstanceFunction<PFN_vkCreateSampler>(vk_instance, "vkCreateSampler");
    Turbo::Core::vkCreateSemaphore = this->LoadInstanceFunction<PFN_vkCreateSemaphore>(vk_instance, "vkCreateSemaphore");
    Turbo::Core::vkCreateShaderModule = this->LoadInstanceFunction<PFN_vkCreateShaderModule>(vk_instance, "vkCreateShaderModule");
    Turbo::Core::vkDestroyBuffer = this->LoadInstanceFunction<PFN_vkDestroyBuffer>(vk_instance, "vkDestroyBuffer");
    Turbo::Core::vkDestroyBufferView = this->LoadInstanceFunction<PFN_vkDestroyBufferView>(vk_instance, "vkDestroyBufferView");
    Turbo::Core::vkDestroyCommandPool = this->LoadInstanceFunction<PFN_vkDestroyCommandPool>(vk_instance, "vkDestroyCommandPool");
    Turbo::Core::vkDestroyDescriptorPool = this->LoadInstanceFunction<PFN_vkDestroyDescriptorPool>(vk_instance, "vkDestroyDescriptorPool");
    Turbo::Core::vkDestroyDescriptorSetLayout = this->LoadInstanceFunction<PFN_vkDestroyDescriptorSetLayout>(vk_instance, "vkDestroyDescriptorSetLayout");
    Turbo::Core::vkDestroyEvent = this->LoadInstanceFunction<PFN_vkDestroyEvent>(vk_instance, "vkDestroyEvent");
    Turbo::Core::vkDestroyFence = this->LoadInstanceFunction<PFN_vkDestroyFence>(vk_instance, "vkDestroyFence");
    Turbo::Core::vkDestroyFramebuffer = this->LoadInstanceFunction<PFN_vkDestroyFramebuffer>(vk_instance, "vkDestroyFramebuffer");
    Turbo::Core::vkDestroyImage = this->LoadInstanceFunction<PFN_vkDestroyImage>(vk_instance, "vkDestroyImage");
    Turbo::Core::vkDestroyImageView = this->LoadInstanceFunction<PFN_vkDestroyImageView>(vk_instance, "vkDestroyImageView");
    Turbo::Core::vkDestroyPipeline = this->LoadInstanceFunction<PFN_vkDestroyPipeline>(vk_instance, "vkDestroyPipeline");
    Turbo::Core::vkDestroyPipelineCache = this->LoadInstanceFunction<PFN_vkDestroyPipelineCache>(vk_instance, "vkDestroyPipelineCache");
    Turbo::Core::vkDestroyPipelineLayout = this->LoadInstanceFunction<PFN_vkDestroyPipelineLayout>(vk_instance, "vkDestroyPipelineLayout");
    Turbo::Core::vkDestroyQueryPool = this->LoadInstanceFunction<PFN_vkDestroyQueryPool>(vk_instance, "vkDestroyQueryPool");
    Turbo::Core::vkDestroyRenderPass = this->LoadInstanceFunction<PFN_vkDestroyRenderPass>(vk_instance, "vkDestroyRenderPass");
    Turbo::Core::vkDestroySampler = this->LoadInstanceFunction<PFN_vkDestroySampler>(vk_instance, "vkDestroySampler");
    Turbo::Core::vkDestroySemaphore = this->LoadInstanceFunction<PFN_vkDestroySemaphore>(vk_instance, "vkDestroySemaphore");
    Turbo::Core::vkDestroyShaderModule = this->LoadInstanceFunction<PFN_vkDestroyShaderModule>(vk_instance, "vkDestroyShaderModule");
    Turbo::Core::vkDeviceWaitIdle = this->LoadInstanceFunction<PFN_vkDeviceWaitIdle>(vk_instance, "vkDeviceWaitIdle");
    Turbo::Core::vkEndCommandBuffer = this->LoadInstanceFunction<PFN_vkEndCommandBuffer>(vk_instance, "vkEndCommandBuffer");
    Turbo::Core::vkFlushMappedMemoryRanges = this->LoadInstanceFunction<PFN_vkFlushMappedMemoryRanges>(vk_instance, "vkFlushMappedMemoryRanges");
    Turbo::Core::vkFreeCommandBuffers = this->LoadInstanceFunction<PFN_vkFreeCommandBuffers>(vk_instance, "vkFreeCommandBuffers");
    Turbo::Core::vkFreeDescriptorSets = this->LoadInstanceFunction<PFN_vkFreeDescriptorSets>(vk_instance, "vkFreeDescriptorSets");
    Turbo::Core::vkFreeMemory = this->LoadInstanceFunction<PFN_vkFreeMemory>(vk_instance, "vkFreeMemory");
    Turbo::Core::vkGetBufferMemoryRequirements = this->LoadInstanceFunction<PFN_vkGetBufferMemoryRequirements>(vk_instance, "vkGetBufferMemoryRequirements");
    Turbo::Core::vkGetDeviceMemoryCommitment = this->LoadInstanceFunction<PFN_vkGetDeviceMemoryCommitment>(vk_instance, "vkGetDeviceMemoryCommitment");
    Turbo::Core::vkGetDeviceQueue = this->LoadInstanceFunction<PFN_vkGetDeviceQueue>(vk_instance, "vkGetDeviceQueue");
    Turbo::Core::vkGetEventStatus = this->LoadInstanceFunction<PFN_vkGetEventStatus>(vk_instance, "vkGetEventStatus");
    Turbo::Core::vkGetFenceStatus = this->LoadInstanceFunction<PFN_vkGetFenceStatus>(vk_instance, "vkGetFenceStatus");
    Turbo::Core::vkGetImageMemoryRequirements = this->LoadInstanceFunction<PFN_vkGetImageMemoryRequirements>(vk_instance, "vkGetImageMemoryRequirements");
    Turbo::Core::vkGetImageSparseMemoryRequirements = this->LoadInstanceFunction<PFN_vkGetImageSparseMemoryRequirements>(vk_instance, "vkGetImageSparseMemoryRequirements");
    Turbo::Core::vkGetImageSubresourceLayout = this->LoadInstanceFunction<PFN_vkGetImageSubresourceLayout>(vk_instance, "vkGetImageSubresourceLayout");
    Turbo::Core::vkGetPipelineCacheData = this->LoadInstanceFunction<PFN_vkGetPipelineCacheData>(vk_instance, "vkGetPipelineCacheData");
    Turbo::Core::vkGetQueryPoolResults = this->LoadInstanceFunction<PFN_vkGetQueryPoolResults>(vk_instance, "vkGetQueryPoolResults");
    Turbo::Core::vkGetRenderAreaGranularity = this->LoadInstanceFunction<PFN_vkGetRenderAreaGranularity>(vk_instance, "vkGetRenderAreaGranularity");
    Turbo::Core::vkInvalidateMappedMemoryRanges = this->LoadInstanceFunction<PFN_vkInvalidateMappedMemoryRanges>(vk_instance, "vkInvalidateMappedMemoryRanges");
    Turbo::Core::vkMapMemory = this->LoadInstanceFunction<PFN_vkMapMemory>(vk_instance, "vkMapMemory");
    Turbo::Core::vkMergePipelineCaches = this->LoadInstanceFunction<PFN_vkMergePipelineCaches>(vk_instance, "vkMergePipelineCaches");
    Turbo::Core::vkQueueBindSparse = this->LoadInstanceFunction<PFN_vkQueueBindSparse>(vk_instance, "vkQueueBindSparse");
    Turbo::Core::vkQueueSubmit = this->LoadInstanceFunction<PFN_vkQueueSubmit>(vk_instance, "vkQueueSubmit");
    Turbo::Core::vkQueueWaitIdle = this->LoadInstanceFunction<PFN_vkQueueWaitIdle>(vk_instance, "vkQueueWaitIdle");
    Turbo::Core::vkResetCommandBuffer = this->LoadInstanceFunction<PFN_vkResetCommandBuffer>(vk_instance, "vkResetCommandBuffer");
    Turbo::Core::vkResetCommandPool = this->LoadInstanceFunction<PFN_vkResetCommandPool>(vk_instance, "vkResetCommandPool");
    Turbo::Core::vkResetDescriptorPool = this->LoadInstanceFunction<PFN_vkResetDescriptorPool>(vk_instance, "vkResetDescriptorPool");
    Turbo::Core::vkResetEvent = this->LoadInstanceFunction<PFN_vkResetEvent>(vk_instance, "vkResetEvent");
    Turbo::Core::vkResetFences = this->LoadInstanceFunction<PFN_vkResetFences>(vk_instance, "vkResetFences");
    Turbo::Core::vkSetEvent = this->LoadInstanceFunction<PFN_vkSetEvent>(vk_instance, "vkSetEvent");
    Turbo::Core::vkUnmapMemory = this->LoadInstanceFunction<PFN_vkUnmapMemory>(vk_instance, "vkUnmapMemory");
    Turbo::Core::vkUpdateDescriptorSets = this->LoadInstanceFunction<PFN_vkUpdateDescriptorSets>(vk_instance, "vkUpdateDescriptorSets");
    Turbo::Core::vkWaitForFences = this->LoadInstanceFunction<PFN_vkWaitForFences>(vk_instance, "vkWaitForFences");
#endif

#if defined(VK_VERSION_1_3)
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

    return physical_device_driver;
}

Turbo::Core::TDeviceDriver Turbo::Core::TVulkanLoader::LoadDeviceDriver(TDevice *device)
{
    TDeviceDriver device_driver = {};
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

#if defined(VK_VERSION_1_3)
    // FIXME: Determine the version of Vulkan version
    Turbo::Core::TVersion vulkan_version = device->GetPhysicalDevice()->GetInstance()->GetVulkanVersion();
    if (vulkan_version >= Turbo::Core::TVersion(1, 3, 0, 0))
    {
        device_driver.vkCmdBeginRendering = this->LoadDeviceFunction<PFN_vkCmdBeginRendering>(device, "vkCmdBeginRendering");
        device_driver.vkCmdEndRendering = this->LoadDeviceFunction<PFN_vkCmdEndRendering>(device, "vkCmdEndRendering");
    }
#endif

#if defined(VK_EXT_mesh_shader)
    if (device->IsEnabledExtension(TExtensionType::VK_EXT_MESH_SHADER))
    {
        device_driver.vkCmdDrawMeshTasksEXT = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksEXT>(device, "vkCmdDrawMeshTasksEXT");
        device_driver.vkCmdDrawMeshTasksIndirectCountEXT = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectCountEXT>(device, "vkCmdDrawMeshTasksIndirectCountEXT");
        device_driver.vkCmdDrawMeshTasksIndirectEXT = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectEXT>(device, "vkCmdDrawMeshTasksIndirectEXT");
    }
#endif

#if defined(VK_NV_mesh_shader)
    if (device->IsEnabledExtension(TExtensionType::VK_NV_MESH_SHADER))
    {
        device_driver.vkCmdDrawMeshTasksIndirectCountNV = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectCountNV>(device, "vkCmdDrawMeshTasksIndirectCountNV");
        device_driver.vkCmdDrawMeshTasksIndirectNV = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksIndirectNV>(device, "vkCmdDrawMeshTasksIndirectNV");
        device_driver.vkCmdDrawMeshTasksNV = this->LoadDeviceFunction<PFN_vkCmdDrawMeshTasksNV>(device, "vkCmdDrawMeshTasksNV");
    }
#endif

    return device_driver;
}

std::string Turbo::Core::TVulkanLoader::ToString()
{
    return std::string();
}
