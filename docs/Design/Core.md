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

* 2023/4/10
  >
  >* 创建`Tessellation`章节
  >* 创建`Device Tessellation Feature`章节
  >* 创建`Tessellation Graphics Pipeline`章节

* 2023/4/16
  >
  >* 创建`Mesh Shader`章节
  >* 创建`Mesh Shader 接口函数`章节
  >* 创建`Mesh Shader Feature`章节

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

```CXX
//in Turbo::Core
class TRenderingAttachments
{
};
```

为了在`Turbo`中能够执行`Dynamic Rendering`指令，`TCommandBufferBase`中需要提供相应接口。

```CXX
//in Turbo::Core::TCommandBufferBase
void CmdBeginRendering(const TRenderingAttachments& renderingAttachment);
```

```CXX
//in Turbo::Core::TCommandBufferBase
void CmdEndRendering();
```

## Tessellation

细分（`Tessellation`）。用于细分网格。

细分流程分为如下三个阶段：

1. 细分控制着色器（可编程） Tessellation Control Shader
2. 生成细分图元（不可编程） Tessellation Primitive Generator
3. 细分评估（计算）着色器（可编程） Tessellation Evaluation Shader

### Device Tessellation Feature

`Vulkan`中想要使用细分特性，需要查看和激活对应的设备`feature`。有关的细分的`feature`如下：

```CXX
//in VkPhysicalDeviceFeatures
VkBool32    tessellationShader;
VkBool32    shaderTessellationAndGeometryPointSize;
```

* 如果设备获取的特性中`tessellationShader`为`VK_FALSE`的话，说明该设备不支持细分特性。
* `shaderTessellationAndGeometryPointSize`用于说明细分控制着色器和细分评估着色器和几何着色器中的内置变量`PointSize`是否有效可用，如果为`VK_FALSE`的话相关着色器禁止对其进行读写

```CXX
//in VkPhysicalDeviceVulkan11Features 
VkBool32           multiviewTessellationShader;
```

* `multiviewTessellationShader`用于说明一个`RenderPass`中的细分着色器是否支持`multiview rendering`，如果该特性为`VK_FALSE`的话，对于`view mask`非零的管线其不能包括任何细分着色器

```CXX
//in VkPhysicalDeviceVulkan12Features 
VkBool32           shaderOutputLayer;
```

* `shaderOutputLayer` indicates whether the implementation supports the ShaderLayer SPIR-V capability enabling variables decorated with the Layer built-in to be exported from vertex or tessellation evaluation shaders. If this feature is not enabled, the Layer built-in decoration must not be used on outputs in vertex or tessellation evaluation shaders.（没看懂干啥的，好像是顶点着色器和细分着评估色器中有个内置`Layer`声明，这个`shaderOutputLayer`好像是用来指示这个内置`Layer`声明能不能用）

还有一个特性需要明确一下：

```CXX
//in VkPhysicalDeviceFeatures  
VkBool32           fillModeNonSolid;
```

* `fillModeNonSolid`用于说明是否支持点`point`和线框`wireframe`的填充模式，如果该特性不支持的话`VK_POLYGON_MODE_POINT`和`VK_POLYGON_MODE_LINE`是不能使用的（很多时候是将模型渲染成线框看细分结果）

### Tessellation Graphics Pipeline

根据`Vulkan`标准，细分着色器是指定于:

```CXX
//位于VkGraphicsPipelineCreateInfo中
const VkPipelineShaderStageCreateInfo* pStages;
```

对于`Tessellation`的配置是指定于：

```CXX
//位于VkGraphicsPipelineCreateInfo中
const VkPipelineTessellationStateCreateInfo* pTessellationState;
```

根据`Vulkan`标准，有如下要求：

>[VUID-VkGraphicsPipelineCreateInfo-pStages-00729](https://registry.khronos.org/vulkan/specs/1.3/html/chap10.html#VUID-VkGraphicsPipelineCreateInfo-pStages-00729)  
>If the pipeline is being created with `pre-rasterization shader state` and `pStages` includes a tessellation control shader stage, it `must` include a tessellation evaluation shader stage
>
>如果`VkGraphicsPipelineCreateInfo`中的`pStages`中包括细分控制着色器话，`VkGraphicsPipelineCreateInfo`中的`pStages`必须也包含一个细分评估着色器

>[VUID-VkGraphicsPipelineCreateInfo-pStages-00730](https://registry.khronos.org/vulkan/specs/1.3/html/chap10.html#VUID-VkGraphicsPipelineCreateInfo-pStages-00730)  
>If the pipeline is being created with `pre-rasterization shader state` and `pStages` includes a tessellation evaluation shader stage, it `must` include a tessellation control shader stage
>
>如果`VkGraphicsPipelineCreateInfo`中的`pStages`中包括细分评估着色器话，`VkGraphicsPipelineCreateInfo`中的`pStages`必须也包含一个细分控制着色器

>[VUID-VkGraphicsPipelineCreateInfo-pStages-00731](https://registry.khronos.org/vulkan/specs/1.3/html/chap10.html#VUID-VkGraphicsPipelineCreateInfo-pStages-00731)  
>If the pipeline is being created with `pre-rasterization shader state` and `pStages` includes a tessellation control shader stage and a tessellation evaluation shader stage, pTessellationState `must` be a valid pointer to a valid `VkPipelineTessellationStateCreateInfo`structure
>
>如果`VkGraphicsPipelineCreateInfo`中的`pStages`中包括细分控制着色器和细分评估着色器的话，`VkGraphicsPipelineCreateInfo`中的`pTessellationState`必须是个有效的`VkPipelineTessellationStateCreateInfo`结构值

>[VUID-VkGraphicsPipelineCreateInfo-pStages-00736](https://registry.khronos.org/vulkan/specs/1.3/html/chap10.html#VUID-VkGraphicsPipelineCreateInfo-pStages-00736)  
>If the pipeline is being created with `pre-rasterization shader state` and `pStages` includes tessellation shader stages, the `topology` member of pInputAssembly `must` be `VK_PRIMITIVE_TOPOLOGY_PATCH_LIST`
>
>如果`VkGraphicsPipelineCreateInfo`中的`pStages`中包括细分着色器的话，`VkGraphicsPipelineCreateInfo`中的`pInputAssemblyState`中的`topology`成员值必须是`VK_PRIMITIVE_TOPOLOGY_PATCH_LIST`

>[VUID-VkGraphicsPipelineCreateInfo-topology-00737](https://registry.khronos.org/vulkan/specs/1.3/html/chap10.html#VUID-VkGraphicsPipelineCreateInfo-topology-00737)  
>If the pipeline is being created with `pre-rasterization shader state` and the topology member of `pInputAssembly` is `VK_PRIMITIVE_TOPOLOGY_PATCH_LIST`, pStages `must` include tessellation shader stages
>
>如果，`VkGraphicsPipelineCreateInfo`中的`pInputAssemblyState`中的`topology`成员值是`VK_PRIMITIVE_TOPOLOGY_PATCH_LIST`的话，`VkGraphicsPipelineCreateInfo`中的`pStages`中必须包括细分着色器

>[VUID-VkPipelineTessellationStateCreateInfo-patchControlPoints-01214](https://registry.khronos.org/vulkan/specs/1.3/html/chap22.html#VUID-VkPipelineTessellationStateCreateInfo-patchControlPoints-01214)  
>`patchControlPoints` must be greater than zero and less than or equal to `VkPhysicalDeviceLimits::maxTessellationPatchSize`
>
>如果，`VkGraphicsPipelineCreateInfo`中的`pTessellationState`中的`patchControlPoints`成员变量必须大于`0`，并且小于等于`VkPhysicalDeviceLimits::maxTessellationPatchSize`

综上：

* 如果想要使用细分特性，必须同时指定对应的细分控制着色器和细分评估着色器
* 如果想要使用细分特性，`VkGraphicsPipelineCreateInfo`中的`pTessellationState`必须是个有效值
* 如果想要使用细分特性，`VkGraphicsPipelineCreateInfo`中的`pInputAssemblyState`中的`topology`必须是`VK_PRIMITIVE_TOPOLOGY_PATCH_LIST`

## Mesh Shader

`Mesh Shader`在`Vulkan`中作为`NVIDIA`显卡上的一个设备扩展，被命名为`VK_NV_mesh_shader`。该扩展依赖于`VK_KHR_get_physical_device_properties2`扩展

有两种新的可编程着色器

* Task Shader
* Mesh Shader

这两个着色器可以用于替代如下过程

* 获取顶点属性
* 顶点着色器
* 细分着色器
* 几何着色器

同时该扩展同时会开启`SPV_NV_mesh_shader`的`SPIR-V`扩展

### Mesh Shader Feature

需要查看设备是否支持`Mesh Shader`特性，之后再去激活相关特性。对于特性获取有两种方式：

1. 通过`Vulkan 1.1`的`vkGetPhysicalDeviceFeatures2`函数：

    ```CXX
    // Provided by VK_VERSION_1_1
    void vkGetPhysicalDeviceFeatures2(
      VkPhysicalDevice physicalDevice,
      VkPhysicalDeviceFeatures2* pFeatures);

    ```

2. 通过`VK_KHR_get_physical_device_properties2`扩展获得：

    ```CXX
    // Provided by VK_KHR_get_physical_device_properties2
    void vkGetPhysicalDeviceFeatures2(
      VkPhysicalDevice physicalDevice,
      VkPhysicalDeviceFeatures2* pFeatures);

    ```

### Mesh Shader 接口函数

新增如下接口函数

```CXX
// Provided by VK_NV_mesh_shader
void vkCmdDrawMeshTasksIndirectCountNV(
  VkCommandBuffer commandBuffer,
  VkBuffer buffer,
  VkDeviceSize offset,
  VkBuffer countBuffer,
  VkDeviceSize countBufferOffset,
  uint32_t maxDrawCount,
  uint32_t stride)
```

```CXX
// Provided by VK_NV_mesh_shader
void vkCmdDrawMeshTasksIndirectNV(
  VkCommandBuffer commandBuffer,
  VkBuffer buffer,
  VkDeviceSize offset,
  uint32_t drawCount,
  uint32_t stride);

```

```CXX
// Provided by VK_NV_mesh_shader
void vkCmdDrawMeshTasksNV(
  VkCommandBuffer commandBuffer,
  uint32_t taskCount,
  uint32_t firstTask);
```

主要看一下`vkCmdDrawMeshTasksNV`函数，其中

* `taskCount`是设置本地工作组（`local workgroup`）在`X`轴处的数量，对于`Y`轴和`Z`轴其数量隐示默认为`1`
* `firstTask`是`X`轴上的第一个工作组的`ID`

当`vkCmdDrawMeshTasksNV`被调用时会有`taskCount`个本地工作组组成全局工作组
