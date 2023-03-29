# 核心设计

## 更新日志

* 2023/3/19
  >
  >* 更新`Dynamic Rendering`章节
  >* 创建`Device Feature`章节

* 2023/3/20
  >
  >* 创建更新`Device Feature`章节

* 2023/3/22
  >
  >* 创建更新`Dynamic Rendering的Pipeline`章节

* 2023/3/25
  >
  >* 更新`Dynamic Rendering的Pipeline`章节

* 2023/3/29
  >
  >* 更新`Dynamic Rendering的Pipeline`章节
  >* 创建`Dynamic Rendering的渲染`章节

---

## Device Feature

随着`Vulkan`标准的发展，每次新版本的发布都会带来新的特性，这就需要`Turbo`维护自己的特性，提供维护自己的`TFeature`数据

经常需要使用的特性如下：

* `geometryShader`在`Vulkan1.0`
* `tessellationShader`在`Vulkan1.0`
* `sampleRateShading`在`Vulkan1.0`
* `depthClamp`在`Vulkan1.0`
* `depthBiasClamp`在`Vulkan1.0`
* `wideLines`在`Vulkan1.0`
* `fillModeNonSolid`在`Vulkan1.0`
* `timelineSemaphore`在`Vulkan1.2`
* `dynamicRendering`在`Vulkan1.3`

`Vulkan`中获取核心特性和扩展的特性需要通过`vkGetPhysicalDeviceFeatures2`或者`vkGetPhysicalDeviceFeatures2KHR`函数获取。

*注意`vkGetPhysicalDeviceFeatures2`是`Vulkan1.1`的函数*

```CXX
// Provided by VK_VERSION_1_1
void vkGetPhysicalDeviceFeatures2(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2*                  pFeatures);
```

```CXX
// Provided by VK_KHR_get_physical_device_properties2
void vkGetPhysicalDeviceFeatures2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2*                  pFeatures);
```

```CXX
// Provided by VK_VERSION_1_1
typedef struct VkPhysicalDeviceFeatures2 {
    VkStructureType             sType;
    void*                       pNext;
    VkPhysicalDeviceFeatures    features;
} VkPhysicalDeviceFeatures2;
```

```CXX
// Provided by VK_KHR_get_physical_device_properties2
typedef VkPhysicalDeviceFeatures2 VkPhysicalDeviceFeatures2KHR;
```

*如果想要使用`vkGetPhysicalDeviceFeatures2KHR`需要激活`VK_KHR_get_physical_device_properties2`的`Instance`扩展*

其中`VkPhysicalDeviceFeatures2`结构体除了可以在`vkGetPhysicalDeviceFeatures2`和`vkGetPhysicalDeviceFeatures2KHR`中使用，也可以用于`VkDeviceCreateInfo`的`pNext`链中，相当于在创建`VkDevice`时通过`pEnabledFeatures`指定支持的特性。

对于`Vulkan1.1`的特性的获取是通过在`vkGetPhysicalDeviceFeatures2`函数中`VkPhysicalDeviceFeatures2`的`pNext`中指定`VkPhysicalDeviceVulkan11Features`

`VkDeviceCreateInfo`的`pNext`链中指定`VkPhysicalDeviceVulkan11Features`，用于激活`Vulkan1.1`特性

*注：`VkPhysicalDeviceVulkan11Features`是`Vulkan1.2`中定义的*

同获取`Vulkan1.2`的特性方式与获取`Vulkan1.1`特性一样，通过在`vkGetPhysicalDeviceFeatures2`函数中`VkPhysicalDeviceFeatures2`的`pNext`中指定`VkPhysicalDeviceVulkan12Features`

`VkDeviceCreateInfo`的`pNext`链中指定`VkPhysicalDeviceVulkan12Features`，用于激活`Vulkan1.2`特性

获取`Vulkan1.3`的特性方式与之前相同，通过在`vkGetPhysicalDeviceFeatures2`函数中`VkPhysicalDeviceFeatures2`的`pNext`中指定`VkPhysicalDeviceVulkan13Features`

`VkDeviceCreateInfo`的`pNext`链中指定`VkPhysicalDeviceVulkan13Features`，用于激活`Vulkan1.3`特性

在`Vulkan1.3`版本中，对于`DynamicRendering`特性，除了`VkBool32 VkPhysicalDeviceVulkan13Features::dynamicRendering`中可以获取之外，还可以通过在`vkGetPhysicalDeviceFeatures2`函数中`VkPhysicalDeviceFeatures2`的`pNext`中指定`VkPhysicalDeviceDynamicRenderingFeatures`获取

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkPhysicalDeviceDynamicRenderingFeatures {
    VkStructureType    sType;
    void*              pNext;
    VkBool32           dynamicRendering;
} VkPhysicalDeviceDynamicRenderingFeatures;
```

并且`VkPhysicalDeviceDynamicRenderingFeatures`也可以用于`VkDeviceCreateInfo`的`pNext`链中，用于激活`DynamicRendering`特性

*注：按照`Vulkan1.3`的标准，如果`VkDeviceCreateInfo::pNext`中已经链上了`VkPhysicalDeviceVulkan13Features`则不能再链接`VkPhysicalDeviceDynamicRenderingFeatures`*

## Dynamic Rendering

对于`Vulkan`中的`Dynamic Rendering`主要如下函数

* `vkCmdBeginRenderingKHR`或者`vkCmdBeginRendering`
* `vkCmdEndRenderingKHR`或者`vkCmdEndRendering`

```CXX
// Provided by VK_VERSION_1_3
void vkCmdBeginRendering(
    VkCommandBuffer                             commandBuffer,
    const VkRenderingInfo*                      pRenderingInfo);
```

```CXX
// Provided by VK_KHR_dynamic_rendering
void vkCmdBeginRenderingKHR(
    VkCommandBuffer                             commandBuffer,
    const VkRenderingInfo*                      pRenderingInfo);
```

```CXX
// Provided by VK_VERSION_1_3
void vkCmdEndRendering(
    VkCommandBuffer                             commandBuffer);
```

```CXX
// Provided by VK_KHR_dynamic_rendering
void vkCmdEndRenderingKHR(
    VkCommandBuffer                             commandBuffer);
```

### Dynamic Rendering的Pipeline

对于`Dynamic Rendering`，不需要提前创建`Framebuffer`和`RenderPass`，对于`Framebuffer`和`RenderPass`的指定是在`vkCmdBeginRenderingKHR`构建时指定的，由于不需要`RenderPass`了，所以需要`Turbo`提供不需要`TRenderPass`和`Subpass`(`Vulkan`中对于`Dynamic Rendering`默认只支持一个`Subpass`的情况)的`GraphicsPipeline`版本。

对于提供不需要`TRenderPass`的`GraphicsPipeline`版本，目前有两种选择

1. 将当前`TGraphicsPipeline`提供一个没有`TRenderPass`和`Subpass`的版本，用于适配支持`Dynamic Rendering`的`Pipeline`(在此基础上可以考虑从`TGraphicsPipeline`派生一个`TRenderingPipeline`)

2. 从`TPipeline`派生出一个新的类`TRenderingPipeline`（内容和`TGraphicsPipeline`基本相同），专门用于适配支持`Dynamic Rendering`的`Pipeline`

根据`Vulkan`标准创建支持`Dynamic Rendering`需要在创建`Graphics Pipeline`时指定`VkPipelineRenderingCreateInfoKHR`（对于扩展）或者`VkPipelineRenderingCreateInfo`（对于标准），所以采用方式`2`中从`TPipeline`派生一个`TRenderingPipeline`比较符合一般直觉

```CXX
//in Turbo::Core
class TRenderingPipeline : public Turbo::Core::TPipeline
{
  //大部分与TGraphicsPipeline一样
  //不需要指定RenderPass
  //不需要指定Subpass
};
```

创建`Dynamic Rendering Pipeline`时不需要指定`RenderPass`和`Subpass`了，只需要指定要使用的附件`Attachment`的格式

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkPipelineRenderingCreateInfo {
  VkStructureType sType;
  const void* pNext;
  uint32_t viewMask;
  uint32_t colorAttachmentCount;
  const VkFormat* pColorAttachmentFormats;
  VkFormat depthAttachmentFormat;
  VkFormat stencilAttachmentFormat;
} VkPipelineRenderingCreateInfo;
```

对于`Dynamic Rendering`需要指定的附件`Attachment`仅仅为对应的`VkFormat`。为此声明一个`TAttachmentsFormats`用于记录各个附件的格式

```CXX
//in Turbo::Core
class TAttachmentsFormats
{
  private:
    std::vector<TFormatType> colorAttachmentFormats;
    TFormatType depthAttachmentFormat = TFormatType::UNDEFINED;
    TFormatType stencilAttachmentFormat = TFormatType::UNDEFINED;

    void AddColorAttachmentFormat(TFormatType formatType);
    void SetDepthAttachmentFormat(TFormatType formatType);
    void SetStencilAttachmentFormat(TFormatType formatType);
};
```

### Dynamic Rendering的渲染

对于`Dynamic Rendering`的渲染最基本的就两个调用函数：

```CXX
// Provided by VK_VERSION_1_3
void vkCmdBeginRendering(
    VkCommandBuffer                             commandBuffer,
    const VkRenderingInfo*                      pRenderingInfo);
```
```CXX
// Provided by VK_VERSION_1_3
void vkCmdEndRendering(
    VkCommandBuffer                             commandBuffer);
```

而对于`vkCmdBeginRendering`在渲染时需要指定`VkRenderingInfo`

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
```

默认情况下`viewMask`设置为`0`表示所有的附件的`layer`都按照`layerCount`的值来设置。最终要的是设置`VkRenderingAttachmentInfo`用于指定真正的附件。

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

为此声明一个`TRenderingAttachments`用于记录各个附件


为了在`Turbo`中能够执行`Dynamic Rendering`指令，`TCommandBufferBase`中需要提供相应接口。

```CXX
//in Turbo::Core::TCommandBufferBase
void CmdBeginRendering(const TRenderingAttachments& renderingAttachment);
```

```CXX
//in Turbo::Core::TCommandBufferBase
void CmdEndRendering();
```
