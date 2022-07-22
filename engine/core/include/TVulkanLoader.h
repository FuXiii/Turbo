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

#define VULKAN_CORE
#define VULKAN_EXTENSION

#if defined(VK_VERSION_1_0)
extern VULKAN_GLOBAL_API VULKAN_CORE PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern VULKAN_GLOBAL_API VULKAN_CORE PFN_vkCreateInstance vkCreateInstance;
extern VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkDestroyInstance vkDestroyInstance;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkDestroyInstance vkDestroyInstance;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkCreateDevice vkCreateDevice;
extern VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyDevice vkDestroyDevice;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkAllocateMemory vkAllocateMemory;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkBindBufferMemory vkBindBufferMemory;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkBindImageMemory vkBindImageMemory;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBeginQuery vkCmdBeginQuery;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindPipeline vkCmdBindPipeline;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBlitImage vkCmdBlitImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdClearAttachments vkCmdClearAttachments;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdClearColorImage vkCmdClearColorImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyImage vkCmdCopyImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDispatch vkCmdDispatch;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDraw vkCmdDraw;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdEndQuery vkCmdEndQuery;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdFillBuffer vkCmdFillBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdNextSubpass vkCmdNextSubpass;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdPushConstants vkCmdPushConstants;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdResetEvent vkCmdResetEvent;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdResolveImage vkCmdResolveImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetEvent vkCmdSetEvent;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetScissor vkCmdSetScissor;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetViewport vkCmdSetViewport;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdWaitEvents vkCmdWaitEvents;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateBuffer vkCreateBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateBufferView vkCreateBufferView;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateCommandPool vkCreateCommandPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateComputePipelines vkCreateComputePipelines;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateEvent vkCreateEvent;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateFence vkCreateFence;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateFramebuffer vkCreateFramebuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateImage vkCreateImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateImageView vkCreateImageView;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreatePipelineCache vkCreatePipelineCache;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateQueryPool vkCreateQueryPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateRenderPass vkCreateRenderPass;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateSampler vkCreateSampler;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateSemaphore vkCreateSemaphore;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateShaderModule vkCreateShaderModule;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyBuffer vkDestroyBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyBufferView vkDestroyBufferView;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyCommandPool vkDestroyCommandPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyEvent vkDestroyEvent;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyFence vkDestroyFence;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyImage vkDestroyImage;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyImageView vkDestroyImageView;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyPipeline vkDestroyPipeline;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyQueryPool vkDestroyQueryPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyRenderPass vkDestroyRenderPass;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroySampler vkDestroySampler;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroySemaphore vkDestroySemaphore;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyShaderModule vkDestroyShaderModule;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkEndCommandBuffer vkEndCommandBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkFreeMemory vkFreeMemory;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDeviceQueue vkGetDeviceQueue;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetEventStatus vkGetEventStatus;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetFenceStatus vkGetFenceStatus;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkMapMemory vkMapMemory;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkMergePipelineCaches vkMergePipelineCaches;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkQueueBindSparse vkQueueBindSparse;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkQueueSubmit vkQueueSubmit;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkQueueWaitIdle vkQueueWaitIdle;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetCommandBuffer vkResetCommandBuffer;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetCommandPool vkResetCommandPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetDescriptorPool vkResetDescriptorPool;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetEvent vkResetEvent;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetFences vkResetFences;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkSetEvent vkSetEvent;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkUnmapMemory vkUnmapMemory;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
extern VULKAN_DEVICE_API VULKAN_CORE PFN_vkWaitForFences vkWaitForFences;
#endif

#if defined(VK_VERSION_1_1)
extern VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
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
    Function LoadInstanceFunction(TInstance *instance, const char *name);

    template <typename Function>
    Function LoadInstanceFunction(VkInstance instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunction(TInstance *instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunction(VkInstance instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunction(TDevice *device, const char *name);

    template <typename Function>
    Function LoadDeviceFunction(VkDevice device, const char *name);

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
Function Turbo::Core::TVulkanLoader::LoadInstanceFunction(TInstance *instance, const char *name)
{
    return this->LoadInstanceFunction<Function>(instance->GetVkInstance(), name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadInstanceFunction(VkInstance instance, const char *name)
{
    return this->Load<TLoaderType::INSTANCE, Function>(instance, name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunction(TInstance *instance, const char *name)
{
    return this->LoadDeviceFunction<Function>(instance->GetVkInstance(), name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunction(VkInstance instance, const char *name)
{
    return this->LoadInstanceFunction<Function>(instance, name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunction(TDevice *device, const char *name)
{
    return this->LoadDeviceFunction<Function>(device->GetVkDevice(), name);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunction(VkDevice device, const char *name)
{
    return this->Load<TLoaderType::DEVICE, Function>(device, name);
}
#endif // !TVULKANLOADER_H