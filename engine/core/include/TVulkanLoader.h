#pragma once
#ifndef TURBO_CORE_TVULKANLOADER_H
#define TURBO_CORE_TVULKANLOADER_H
#include "TDevice.h"
#include "TException.h"
#include "TInstance.h"
#include <assert.h>

namespace Turbo
{
namespace Core
{
struct TGlobalFunctionTable
{
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = nullptr;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkCreateInstance vkCreateInstance = nullptr;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
};
using TGlobalDriver = TGlobalFunctionTable;

struct TInstanceFunctionTable
{
#if defined(VK_VERSION_1_0)
    VULKAN_INSTANCE_API VULKAN_CORE PFN_vkDestroyInstance vkDestroyInstance = nullptr;
    VULKAN_INSTANCE_API VULKAN_CORE PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
    VULKAN_INSTANCE_API VULKAN_CORE PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;
#endif
#if defined(VK_VERSION_1_1)
    VULKAN_INSTANCE_API VULKAN_CORE PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups = nullptr;
#endif

#if defined(VK_KHR_device_group_creation)
    VULKAN_INSTANCE_API VULKAN_EXTENSION PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR = nullptr;
#endif
};
using TInstanceDriver = TInstanceFunctionTable;

struct TPhysicalDeviceFunctionTable
{
#if defined(VK_VERSION_1_0)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkCreateDevice vkCreateDevice = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
#endif
#if defined(VK_VERSION_1_1)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2 = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties = nullptr;
#endif
#if defined(VK_VERSION_1_2)
#endif
#if defined(VK_VERSION_1_3)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties = nullptr;
#endif

#if defined(VK_KHR_get_physical_device_properties2)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR = nullptr;
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR = nullptr;
#endif
#if defined(VK_KHR_external_memory_capabilities)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR = nullptr;
#endif
#if defined(VK_KHR_external_fence_capabilities)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR = nullptr;
#endif
#if defined(VK_KHR_external_semaphore_capabilities)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = nullptr;
#endif
};
using TPhysicalDeviceDriver = TPhysicalDeviceFunctionTable;

struct TDeviceFunctionTable
{
#if defined(VK_VERSION_1_0)
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyDevice vkDestroyDevice = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkAllocateMemory vkAllocateMemory = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkBeginCommandBuffer vkBeginCommandBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkBindBufferMemory vkBindBufferMemory = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkBindImageMemory vkBindImageMemory = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBeginQuery vkCmdBeginQuery = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindPipeline vkCmdBindPipeline = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBlitImage vkCmdBlitImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdClearAttachments vkCmdClearAttachments = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdClearColorImage vkCmdClearColorImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyBuffer vkCmdCopyBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyImage vkCmdCopyImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDispatch vkCmdDispatch = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDraw vkCmdDraw = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDrawIndexed vkCmdDrawIndexed = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDrawIndirect vkCmdDrawIndirect = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdEndQuery vkCmdEndQuery = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdEndRenderPass vkCmdEndRenderPass = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdExecuteCommands vkCmdExecuteCommands = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdFillBuffer vkCmdFillBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdNextSubpass vkCmdNextSubpass = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdPushConstants vkCmdPushConstants = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdResetEvent vkCmdResetEvent = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdResetQueryPool vkCmdResetQueryPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdResolveImage vkCmdResolveImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetDepthBias vkCmdSetDepthBias = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetEvent vkCmdSetEvent = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetLineWidth vkCmdSetLineWidth = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetScissor vkCmdSetScissor = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetStencilReference vkCmdSetStencilReference = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetViewport vkCmdSetViewport = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdWaitEvents vkCmdWaitEvents = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateBuffer vkCreateBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateBufferView vkCreateBufferView = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateComputePipelines vkCreateComputePipelines = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateDescriptorPool vkCreateDescriptorPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateEvent vkCreateEvent = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateFence vkCreateFence = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateFramebuffer vkCreateFramebuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateImage vkCreateImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateImageView vkCreateImageView = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreatePipelineCache vkCreatePipelineCache = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreatePipelineLayout vkCreatePipelineLayout = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateQueryPool vkCreateQueryPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateRenderPass vkCreateRenderPass = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateSampler vkCreateSampler = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateSemaphore vkCreateSemaphore = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCreateShaderModule vkCreateShaderModule = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyBuffer vkDestroyBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyBufferView vkDestroyBufferView = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyEvent vkDestroyEvent = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyFence vkDestroyFence = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyFramebuffer vkDestroyFramebuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyImage vkDestroyImage = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyImageView vkDestroyImageView = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyPipeline vkDestroyPipeline = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyPipelineCache vkDestroyPipelineCache = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyQueryPool vkDestroyQueryPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyRenderPass vkDestroyRenderPass = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroySampler vkDestroySampler = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroySemaphore vkDestroySemaphore = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDestroyShaderModule vkDestroyShaderModule = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkDeviceWaitIdle vkDeviceWaitIdle = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkEndCommandBuffer vkEndCommandBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkFreeCommandBuffers vkFreeCommandBuffers = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkFreeDescriptorSets vkFreeDescriptorSets = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkFreeMemory vkFreeMemory = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetEventStatus vkGetEventStatus = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetFenceStatus vkGetFenceStatus = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetPipelineCacheData vkGetPipelineCacheData = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetQueryPoolResults vkGetQueryPoolResults = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkMapMemory vkMapMemory = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkMergePipelineCaches vkMergePipelineCaches = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkQueueBindSparse vkQueueBindSparse = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkQueueSubmit vkQueueSubmit = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkQueueWaitIdle vkQueueWaitIdle = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetCommandBuffer vkResetCommandBuffer = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetCommandPool vkResetCommandPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetDescriptorPool vkResetDescriptorPool = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetEvent vkResetEvent = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkResetFences vkResetFences = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkSetEvent vkSetEvent = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkUnmapMemory vkUnmapMemory = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkWaitForFences vkWaitForFences = nullptr;
#endif

#if defined(VK_VERSION_1_1)
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdDispatchBase vkCmdDispatchBase = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupport = nullptr;
#endif

#if defined(VK_VERSION_1_2)
    //  TODO: VkPhysicalDeviceBufferDeviceAddressFeatures
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress = nullptr;
#endif

#if defined(VK_VERSION_1_3)
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdBeginRendering vkCmdBeginRendering = nullptr;
    VULKAN_DEVICE_API VULKAN_CORE PFN_vkCmdEndRendering vkCmdEndRendering = nullptr;
#endif

#if defined(VK_EXT_mesh_shader)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT = nullptr;
#endif

#if defined(VK_NV_mesh_shader)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV = nullptr;
#endif

#if defined(VK_KHR_acceleration_structure)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR = nullptr;
#endif
#if defined(VK_KHR_ray_tracing_pipeline)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR = nullptr;
#endif

#if defined(VK_EXT_buffer_device_address)
    // TODO: VkPhysicalDeviceBufferAddressFeaturesEXT
    // TODO: VkPhysicalDeviceBufferDeviceAddressFeaturesEXT
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT = nullptr;
#endif
#if defined(VK_KHR_buffer_device_address)
    //  TODO: VkPhysicalDeviceBufferDeviceAddressFeaturesKHR
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR = nullptr;
#endif

#if defined(VK_KHR_device_group)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR = nullptr;
#endif

#if defined(VK_KHR_surface)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR = nullptr;
#endif
#if defined(VK_KHR_swapchain)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR = nullptr;
#endif

#if defined(VK_EXT_tooling_info)
    VULKAN_PHYSICAL_DEVICE_API VULKAN_EXTENSION PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT = nullptr;
#endif

#if defined(VK_KHR_maintenance3)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR = nullptr;
#endif

#if defined(VK_KHR_deferred_host_operations)
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR = nullptr;
    VULKAN_DEVICE_API VULKAN_EXTENSION PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR = nullptr;
#endif

    // FIXME:VK_KHR_swapchain
    // FIXME:VK_KHR_surface
};
using TDeviceDriver = TDeviceFunctionTable;

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

    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkCreateInstance vkCreateInstance;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    VULKAN_GLOBAL_API VULKAN_CORE PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

  private:
    TVulkanLoader();
    ~TVulkanLoader();

    template <TLoaderType type, typename Function>
    Function Load(void *context, const char *name, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr);

  public:
    static TVulkanLoader *Instance();
    static void Destroy();

    static TVersion GetVulkanVersion();

    template <typename Function>
    Function LoadInstanceFunction(TInstance *instance, const char *name);

    template <typename Function>
    Function LoadInstanceFunction(VkInstance instance, const char *name);

    template <typename Function>
    Function LoadDeviceFunction(TDevice *device, const char *name);

    template <typename Function>
    Function LoadDeviceFunction(VkDevice device, const char *name, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr);

    TGlobalDriver LoadGlobalDriver();
    TInstanceDriver LoadInstanceDriver(TInstance *instance);
    TPhysicalDeviceDriver LoadPhysicalDeviceDriver(TPhysicalDevice *physicalDevice);
    TDeviceDriver LoadDeviceDriver(TDevice *device);

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo

template <Turbo::Core::TVulkanLoader::TLoaderType type, typename Function>
Function Turbo::Core::TVulkanLoader::Load(void *context, const char *name, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr)
{
    Function result{};

    switch (type)
    {
    case TLoaderType::INSTANCE: {
        // result = (Function)Turbo::Core::vkGetInstanceProcAddr((VkInstance)context, name);
        result = (Function)this->vkGetInstanceProcAddr((VkInstance)context, name);
    }
    break;
    case TLoaderType::DEVICE: {
        if (vkGetDeviceProcAddr == nullptr)
        {
            throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Core::TVulkanLoader::Load(...)", "can not use a nullptr PFN_vkGetDeviceProcAddr to load a device function");
        }

        result = (Function)vkGetDeviceProcAddr((VkDevice)context, name);
    }
    break;
    }

    if (!result)
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::UNSUPPORTED, "Turbo::Core::TVulkanLoader::Load(...)", name);
    }

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
Function Turbo::Core::TVulkanLoader::LoadDeviceFunction(TDevice *device, const char *name)
{
    PFN_vkGetDeviceProcAddr vk_get_device_proc_addr = device->GetPhysicalDevice()->GetInstance()->GetInstanceDriver()->vkGetDeviceProcAddr;
    return this->LoadDeviceFunction<Function>(device->GetVkDevice(), name, vk_get_device_proc_addr);
}

template <typename Function>
Function Turbo::Core::TVulkanLoader::LoadDeviceFunction(VkDevice device, const char *name, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr)
{
    return this->Load<TLoaderType::DEVICE, Function>(device, name, vkGetDeviceProcAddr);
}
#endif // !TURBO_CORE_TVULKANLOADER_H