# Dynamic Rendering 动态渲染

`VK_KHR_dynamic_rendering` 和 `Vulkan 1.3` 增加了新方式用于表示 `render passes` 而不需要创建 `VkFramebuffer` and `VkRenderPass` ， 代价是不能实现 `subpass` 功能，这意味着动态渲染只适用于不使用 `subpasses` 的渲染（单次绘制）。

`VK_KHR_dynamic_rendering_local_read` 和 `Vulkan 1.4` 在核心和扩展标准中将提供 `subpass` 的大部分功能。任何 `subpass` 的功能都可以多可以通过创建多个动态渲染对象实现. 本地读取未涵盖的功能将导致大多数或所有供应商在内部拆分 `subpass` 。

`VK_QCOM_render_pass_shader_resolve` 在动态渲染中没有等价功能提供给，使用该扩展需要使用老的功能除非/直到有可替代功能出现。

除了使用供应商提供的扩展之外，应用推荐从 `Vulkan 1.3` 版本开始使用 `vkCmdBeginRendering` 和 `vkCmdEndRendering` 管理 `render passes` 。

```CXX
// Provided by VK_VERSION_1_3
void vkCmdBeginRendering(
    VkCommandBuffer                             commandBuffer,
    const VkRenderingInfo*                      pRenderingInfo);

// Provided by VK_KHR_dynamic_rendering
// Equivalent to vkCmdBeginRendering
void vkCmdBeginRenderingKHR(
    VkCommandBuffer                             commandBuffer,
    const VkRenderingInfo*                      pRenderingInfo);
```

如果 `pRenderingInfo->flags` 包含 `VK_RENDERING_RESUMING_BIT` 则将会接着上次渲染的内容继续绘制。

如果 `VkRenderingInfo->pNext` 链中包括 `VkTileMemorySizeInfoQCOM` 的话，则将会忽略 `pRenderingInfo` 。

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkPhysicalDeviceVulkan13Features {
    VkStructureType    sType;
    void*              pNext;
    VkBool32           robustImageAccess;
    VkBool32           inlineUniformBlock;
    VkBool32           descriptorBindingInlineUniformBlockUpdateAfterBind;
    VkBool32           pipelineCreationCacheControl;
    VkBool32           privateData;
    VkBool32           shaderDemoteToHelperInvocation;
    VkBool32           shaderTerminateInvocation;
    VkBool32           subgroupSizeControl;
    VkBool32           computeFullSubgroups;
    VkBool32           synchronization2;
    VkBool32           textureCompressionASTC_HDR;
    VkBool32           shaderZeroInitializeWorkgroupMemory;
    VkBool32           dynamicRendering;
    VkBool32           shaderIntegerDotProduct;
    VkBool32           maintenance4;
} VkPhysicalDeviceVulkan13Features;
```

其中 `dynamicRendering` 特性必须激活。

## 查询设备是否支持动态渲染

```CXX
// Provided by VK_VERSION_1_1
typedef struct VkPhysicalDeviceFeatures2 {
    VkStructureType             sType;
    void*                       pNext;
    VkPhysicalDeviceFeatures    features;
} VkPhysicalDeviceFeatures2;
```

```CXX
// Provided by VK_VERSION_1_1
void vkGetPhysicalDeviceFeatures2(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2*                  pFeatures);

// Provided by VK_KHR_get_physical_device_properties2
// Equivalent to vkGetPhysicalDeviceFeatures2
void vkGetPhysicalDeviceFeatures2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2*                  pFeatures);
```

通过 `vkGetPhysicalDeviceFeatures2` 的 `pNext` 增加指向 `VkPhysicalDeviceVulkan13Features` 的指针实例获取。

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkPhysicalDeviceDynamicRenderingFeatures {
    VkStructureType    sType;
    void*              pNext;
    VkBool32           dynamicRendering;
} VkPhysicalDeviceDynamicRenderingFeatures;

// Provided by VK_KHR_dynamic_rendering
// Equivalent to VkPhysicalDeviceDynamicRenderingFeatures
typedef VkPhysicalDeviceDynamicRenderingFeatures VkPhysicalDeviceDynamicRenderingFeaturesKHR;
```

与上面 `VkPhysicalDeviceVulkan13Features` 使用相同，通过 `vkGetPhysicalDeviceFeatures2` 的 `pNext` 增加指向 `VkPhysicalDeviceDynamicRenderingFeatures` 的指针实例获取。

## 激活动态渲染

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkDeviceCreateInfo {
    VkStructureType                    sType;
    const void*                        pNext;
    VkDeviceCreateFlags                flags;
    uint32_t                           queueCreateInfoCount;
    const VkDeviceQueueCreateInfo*     pQueueCreateInfos;
    // enabledLayerCount is legacy and not used
    uint32_t                           enabledLayerCount;
    // ppEnabledLayerNames is legacy and not used
    const char* const*                 ppEnabledLayerNames;
    uint32_t                           enabledExtensionCount;
    const char* const*                 ppEnabledExtensionNames;
    const VkPhysicalDeviceFeatures*    pEnabledFeatures;
} VkDeviceCreateInfo;
```

`VkDeviceCreateInfo->pNext` 链中增加 `VkPhysicalDeviceVulkan13Features` 指针实例。如果此种情况下则不能增加 `VkPhysicalDeviceDynamicRenderingFeatures` 实例指针了，否则可使用。

## VkRenderingInfo

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkRenderingInfo {
    VkStructureType                     sType;
    const void*                         pNext;
    VkRenderingFlags                    flags;
    VkRect2D                            renderArea;
    uint32_t                            layerCount;
    uint32_t                            viewMask;
    uint32_t                            colorAttachmentCount;
    const VkRenderingAttachmentInfo*    pColorAttachments;
    const VkRenderingAttachmentInfo*    pDepthAttachment;
    const VkRenderingAttachmentInfo*    pStencilAttachment;
} VkRenderingInfo;

// Provided by VK_KHR_dynamic_rendering, VK_QCOM_tile_properties with VK_KHR_dynamic_rendering or VK_VERSION_1_3
// Equivalent to VkRenderingInfo
typedef VkRenderingInfo VkRenderingInfoKHR;
```

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkRenderingAttachmentInfo {
    VkStructureType          sType;
    const void*              pNext;
    VkImageView              imageView;
    VkImageLayout            imageLayout;
    VkResolveModeFlagBits    resolveMode;
    VkImageView              resolveImageView;
    VkImageLayout            resolveImageLayout;
    VkAttachmentLoadOp       loadOp;
    VkAttachmentStoreOp      storeOp;
    VkClearValue             clearValue;
} VkRenderingAttachmentInfo;
```

## 结束动态绘制

```CXX
// Provided by VK_VERSION_1_3
void vkCmdEndRendering(
    VkCommandBuffer                             commandBuffer);

// Provided by VK_KHR_dynamic_rendering
// Equivalent to vkCmdEndRendering
void vkCmdEndRenderingKHR(
    VkCommandBuffer                             commandBuffer);
```