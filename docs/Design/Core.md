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

* 2023/4/17
  >
  >* 更新`Mesh Shader`章节

* 2023/4/18
  >
  >* 创建`获取 Vulkan API`章节
  >* 创建`实例级别`章节
  >* 创建`物理设备级别`章节
  >* 创建`设备级别`章节
  >* 创建`获取Vulkan版本`章节
  >* 创建`是否支持Vulkan`章节

* 2023/4/19
  >
  >* 更新`获取 Vulkan API`章节
  >* 更新`物理设备级别`章节
  >* 更新`设备级别`章节

* 2023/4/21
  >
  >* 更新`Mesh Shader`章节
  >* 创建`Vulkan标准和扩展`章节

* 2023/4/24
  >
  >* 创建`Mesh Shader GraphicsPipeline`章节

* 2023/4/25
  >
  >* 更新`Mesh Shader GraphicsPipeline`章节

* 2023/4/27
  >
  >* 创建`Specialization Constants`章节

* 2023/5/2
  >
  >* 更新`VkSpecializationInfo`章节

* 2023/5/4
  >
  >* 创建`特化判断流程`章节

* 2023/5/6
  >
  >* 创建`Ray Tracing`章节
  >* 创建`历史回顾`章节
  >* 创建`光追标准`章节

* 2023/5/7
  >
  >* 更新`光追标准`章节
  >* 创建`扩展结构`章节

* 2023/5/8
  >
  >* 更新`扩展结构`章节
  >* 创建`加速结构（Acceleration Structures）`章节
  >* 创建`Host端的延迟操作（Deferred Host Operations）`章节

* 2023/5/9
  >
  >* 创建`光追管线`章节
  >* 创建`光线查询`章节
  >* 创建`正路在此`章节
  >* 创建`加速结构的创建`章节

* 2023/5/10
  >
  >* 更新`加速结构的创建`章节
  >* 创建`资源使用和同步`章节
  >* 创建`结论和资源`章节

---

## 获取 Vulkan API

由于`Vulkan`中有三种级别的函数

* 实例级别（instance-level）的函数（使用`vkGetInstanceProcAddr`获取）
* 物理设备级别（physical-device-level）的函数（使用`vkGetInstanceProcAddr`获取）
* 设备级别（device-level）的函数（使用`vkGetDeviceProcAddr`获取）

随着`Vulkan`的发展版本的更新，会随着版本的更新增加新的函数，如果创建`VkInstance`时指定的版本和物理设备获取的函数的`Vulkan`发布版本（`VkPhysicalDeviceProperties::apiVersion`）早的话，就算返回了有效函数也不应该使用（但有例外，见下文`物理设备级别`章节），比如：创建`VkInstance`时指定的版本为`Vulkan1.0`而之后使用该实例去获取`Vulkan1.1`发布的`vkBindBufferMemory2`函数，此时就算返回的函数可用也不应该使用。

### 实例级别

实例级别的函数广义上是指`Vulkan`中的所有可使用`VkInstance`，`VkPhysicalDevice`，`VkDevice`，`VkQueue`或者`VkCommandBuffer`调度的函数，并且在获取这些函数时需要传递一个有效`VkInstance`。

其中有一些函数比较特殊，为全局函数，在获取时不需要传递有效`VkInstance`，直接传递`VK_NULL_HANDLE`既可以有效获取：

* vkEnumerateInstanceVersion
* vkEnumerateInstanceExtensionProperties
* vkEnumerateInstanceLayerProperties
* vkCreateInstance

### 物理设备级别

物理设备级别的函数是指接口函数第一个参数是`VkPhysicalDevice`的函数，比如`vkGetPhysicalDeviceProperties`或者`vkGetPhysicalDeviceFeatures`等函数。

物理设备级别函数的获取有特例，可以获取`Vulkan`高版本的物理设备级别函数进行使用（前提是物理设备支持高版本的`Vulkan`）。比如：创建`VkInstance`时指定的版本为`Vulkan1.0`而之后使用该实例去获取`Vulkan1.1`发布的`vkGetPhysicalDeviceFeatures2`函数，如果设备支持高版本的`Vulkan`并且返回正常则可使用该函数，反之则不行

### 设备级别

设备级别的函数是指接口函数第一个参数是`VkDevice`，`VkQueue`或`VkCommandBuffer`的函数，比如`vkCmdDraw`或`vkCreateBuffer`等。

## 获取Vulkan版本

由于历史原因，在`Vulkan1.0`版本中获取不到系统支持的`Vulkan`版本，只能获取到显卡设备支持的`Vulkan`版本，后来在`Vulkan1.1`后增加了`vkEnumerateInstanceVersion`函数来获取`Vulkan`版本，而`vkEnumerateInstanceVersion`为全局函数，可能会返回`nullptr`（当系统只支持`Vulkan1.0`时）。所以需要有一个判断流程用于获取`Vulkan`版本

```mermaid
graph TD;
LoadVulkanLib("加载Vulkan动态库")
IsLoadVulkanLib{"是否加载"}
ReturnVersion000("返回版本号0.0.0")
GetvkEnumerateInstanceVersion("获取vkEnumerateInstanceVersion函数")
IsGetvkEnumerateInstanceVersion{"是否获取到"}
GetvkCreateInstance("获取vkCreateInstance函数")
IsGetvkCreateInstance{"是否获取到"}
IsCreateVkInstance{"是否成功创建VkInstance"}
GetvkDestroyInstance("获取vkDestroyInstance函数")
IsGetvkDestroyInstance{"是否获取到"}

LoadVulkanLib-->IsLoadVulkanLib
IsLoadVulkanLib--"加载成功（注：返回前卸载）"-->GetvkEnumerateInstanceVersion
IsLoadVulkanLib--加载失败-->ReturnVersion000

GetvkEnumerateInstanceVersion-->IsGetvkEnumerateInstanceVersion
IsGetvkEnumerateInstanceVersion--成功获取-->ReturnSupportVersion("返回支持的Vulkan版本")
IsGetvkEnumerateInstanceVersion--获取失败-->GetvkCreateInstance

GetvkCreateInstance-->IsGetvkCreateInstance
IsGetvkCreateInstance--成功获取-->TryToCreateVkInstance("尝试创建VkInstance1.0")
IsGetvkCreateInstance--获取失败-->ReturnVersion000

TryToCreateVkInstance-->IsCreateVkInstance
IsCreateVkInstance--成功创建-->GetvkDestroyInstance
IsCreateVkInstance--创建失败-->ReturnVersion000

GetvkDestroyInstance-->IsGetvkDestroyInstance
IsGetvkDestroyInstance--成功获取-->DestroyInstance("销毁之前创建的VkInstance")
IsGetvkDestroyInstance--获取失败-->ReturnException("返回一个异常")

DestroyInstance-->ReturnSupportVersion1.0("返回支持的Vulkan1.0")
```

## 是否支持Vulkan

通过获取`Vulkan`版本判断

```mermaid
graph TD;
GetVulkanVersion("获取Vulkan版本")
IfGetVersion000OrGetException{"返回版本0.0.0或者抛出异常"}


GetVulkanVersion-->IfGetVersion000OrGetException
IfGetVersion000OrGetException--是-->ReturnFalse("返回不支持Vulkan")
IfGetVersion000OrGetException--否-->ReturnTrue("返回支持Vulkan")
```

## Vulkan标准和扩展

随着`Vulkan`的发展，每一个`Vulkan`核心版本都会发布新的扩展，并在下一个`Vulkan`核心版中将之前的一部分扩展提升为核心，而`Turbo`也需要随着`Vulkan`的发展进行适配。首要原则是能使用核心标准，尽量使用核心标准，如果设备支持的核心标准过低（导致高版本的`Vulkan`核心功能还处于扩展状态），尝试获取扩展（在用户开启对应扩展功能之后才去获取扩展功能，否则就算用户使用`Turbo`调用了扩展函数，`Turbo`也不会做任何扩展函数调用）。

如果低版本的`Vulkan`实例获取高版本的`Vulkan`核心`API`，所有的高版本的函数皆为`nullptr`，即使调用也不做任何事情。（注：`Vulkan1.0`版本的核心`API`一定不能为`nullptr`）

```mermaid
graph TD;

ThrowException0("抛出异常")
ThrowException1("抛出异常")
Start(("Vulkan\n核心和扩展"))
IfSupportInVulkanCore{"Vulkan核心是否支持该功能\n(通过当前实例的版本)"}
UseVulkanCore("使用Vulkan核心")
GetCoreVulkanAPI("获取Vulkan核心API")
IsGetLegalVulkanCoreAPI{"是否获得合法Vulkan核心API指针"}
UseLegalVulkanCoreAPI("使用合法Vulkan核心API指针")

IfEnableExtension{"是否计划激活对应功能扩展"}
IfSupportExtension{"设备是否支持对应功能扩展"}
Donothing("什么都不做\n（此时对应的扩展函数指针为nullptr，Turbo在判断对应的函数指针为nullptr后直接跳过）")
GetExtensionVulkanAPI("获取Vulkan扩展API")
IsExtensionVulkanAPILegal{"是否获得合法Vulkan扩展API指针"}
UseLegalVulkanExtensionAPI("使用合法Vulkan扩展API指针")

Start-->IfSupportInVulkanCore
IfSupportInVulkanCore--支持-->UseVulkanCore-->GetCoreVulkanAPI-->IsGetLegalVulkanCoreAPI
IsGetLegalVulkanCoreAPI--不合法-->ThrowException0
IsGetLegalVulkanCoreAPI--合法-->UseLegalVulkanCoreAPI
IfSupportInVulkanCore--不支持-->IfEnableExtension

IfEnableExtension--未计划激活-->Donothing
IfEnableExtension--计划激活-->IfSupportExtension--支持-->GetExtensionVulkanAPI
IfSupportExtension--不支持-->Donothing

GetExtensionVulkanAPI-->IsExtensionVulkanAPILegal
IsExtensionVulkanAPILegal--不合法-->ThrowException1
IsExtensionVulkanAPILegal--合法-->UseLegalVulkanExtensionAPI
```

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

`Vulkan`中还有一个名为`VK_EXT_mesh_shader`的设备扩展，该扩展为更加通用的扩展。与`VK_NV_mesh_shader`不完全一样，有区别。

其中`VK_NV_mesh_shader`依赖`VK_KHR_get_physical_device_properties2`扩展

* `VK_KHR_get_physical_device_properties2`在`Vulkan1.1`标准中被提升为核心标准

其中`VK_EXT_mesh_shader`依赖`VK_KHR_spirv_1_4`扩展

* `VK_KHR_spirv_1_4`在`Vulkan1.2`标准中被提升为核心标准
* `VK_KHR_spirv_1_4`依赖`VK_KHR_shader_float_controls`扩展
* `VK_KHR_shader_float_controls`在`Vulkan1.2`标准中被提升为核心标准
* `VK_KHR_shader_float_controls`依赖`VK_KHR_get_physical_device_properties2`扩展

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
    void vkGetPhysicalDeviceFeatures2KHR(
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

### Mesh Shader GraphicsPipeline

当创建`GraphicsPipeline`图形管线时使用`Mesh Shader`有一些限值

* 对于`VkGraphicsPipelineCreateInfo::pVertexInputState`如果使用了`Mesh Shader`的话这个参数将会被忽略
* 对于`VkGraphicsPipelineCreateInfo::pInputAssemblyState`如果使用了`Mesh Shader`的话这个参数将会被忽略
* 对于`VkGraphicsPipelineCreateInfo::pStages`中指定的`Shader`只能是一下两种组合不能混合使用：

  ```mermaid
  graph LR
  style id0 stroke-dasharray: 5 5
  MeshShader
  id0(TaskShader)-->MeshShader
  ```

  ```mermaid
  graph LR
  style id0 stroke-dasharray: 5 5
  style id1 stroke-dasharray: 5 5
  style id2 stroke-dasharray: 5 5
  VertxShader

  FragmentShader
  VertxShader-->id0(TessellationControlShader)-->id1(TessellationEvaluationShader)-->id2(GeometryShader)-->FragmentShader
  ```

* 如果使用了`Mesh Shader`的话`VkGraphicsPipelineCreateInfo::pDynamicStates`中不能包含`VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY`，`VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE`，`VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE`，`VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT`，`VK_DYNAMIC_STATE_VERTEX_INPUT_EXT`
* 对于`VkGraphicsPipelineCreateInfo::pTessellationState`如果使用了`Mesh Shader`的话这个参数将会被忽略(`Vulkan`标准中没有明文指出该限值，只是说如果`VkGraphicsPipelineCreateInfo::pStages`有细分着色器的话此项目需要合法，通过`Shader`组合限制可推出该条目)

## Specialization Constants

`Specialization Constants`用于在着色器中定义特化常量，这个`特化`的意思为在创建该着色器的`Pipeline`中可以指定该常量的值。

在`GLSL`中声明如下：

```CXX
layout(constant_id = 17) const int arraySize = 12;
```

根据`GLSL`标准：`12`为用户设置`arraySize`的默认值，`17`是用户自定义设置的特化常量`id`号。并且特化常量只能声明成标量数据，包括`bool`，`int`，`uint`，`float`，`double`类型。

`GLSL`内置的常量也可以声明成特化常量，比如：

```CXX
layout(constant_id = 31) gl_MaxClipDistances; 
```

如果在该内置常量使用后再声明成特化常量的话会在编译时产生编译错误，也就是要求一个常量要么是非特化的普通常量，要么是特化常量，不能两个都是。

对于特化常量最常见的用法是通过使用`local_size_{xyz}_id`特化常量来分别指定计算着色器的`gl_WorkGroupSize`每个分量的`id`。比如：

```CXX
layout(local_size_x_id = 18, local_size_z_id = 19) in;
```

这里没有设置`local_size_y_id`表明`gl_WorkGroupSize.y`是一个非特化常量值，也就是设定`gl_WorkGroupSize`为部分特化向量值。

对于`x`和`z`的值可以滞后进行特化。在生成`SPIR-V`之后，使用`id`号分别为`18`和`19`进行分别声明工作组纬度大小：

```CXX
layout(local_size_x = 32, local_size_y = 32) in; // 大小为 (32,32,1)
layout(local_size_x_id = 18) in; // x的constant_id 
layout(local_size_z_id = 19) in; // z的constant_id
```

现有标准对于声明`local_size_x`，`local_size_y`和`local_size_z`并不会改变。对于相同的`local-size`的`id`设置了不同的值或者在使用后再设置特化值都会产生编译错误。对于顺序、位置、声明和重复都不会导致错误

### VkSpecializationInfo

在`Vulkan`中通过创建`Pipeline`时设置`VkPipelineShaderStageCreateInfo::pSpecializationInfo`进行特化常量的数值设定。

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineShaderStageCreateInfo {
    VkStructureType                     sType;
    const void*                         pNext;
    VkPipelineShaderStageCreateFlags    flags;
    VkShaderStageFlagBits               stage;
    VkShaderModule                      module;
    const char*                         pName;
    const VkSpecializationInfo*         pSpecializationInfo;
} VkPipelineShaderStageCreateInfo;
```

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkSpecializationInfo {
    uint32_t                           mapEntryCount;
    const VkSpecializationMapEntry*    pMapEntries;
    size_t                             dataSize;
    const void*                        pData;
} VkSpecializationInfo;
```

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkSpecializationMapEntry {
    uint32_t    constantID;
    uint32_t    offset;
    size_t      size;
} VkSpecializationMapEntry;
```

其中`Vulkan`标准要求：`VkSpecializationInfo`中所有的`VkSpecializationMapEntry`的`constantID`不能重复
其中`Vulkan`标准要求：如果特化常量的类型为`bool`类型的话`VkSpecializationMapEntry`中的`size`大小必须是`VkBool32`的大小值

每一个`Shader`都会有自己的特化常量，对于特化常量的对外接口应该位于每个`Shader`中，大致流程如下：

```CXX
TShader* some_shader = new TShader();
some_shader->SetConstant(0, true);
some_shader->SetConstant(1, 10);
some_shader->SetConstant(8, 20.0);
some_shader->SetConstant(13, 400);
```

之后在创建`VkPipeline`时根据指定的多个`Shader`中的特化常量进行数据设置。

* 如果用户通过的`TShader::SetConstant`设置的`id`在`Shader`代码中并没有声明的话，不会造成任何问题，`Turbo`将会跳过该`id`（详情请查看后文的`特化判断流程`）
* 如果用户在`Shader`代码中已经声明了某个`id`的特化常量，而未通过的`TShader::SetConstant`设置特化常量值，则使用`Shader`代码中声明的默认值

#### 特化判断流程

```mermaid
graph TD;
Start(("开始"))-->IsConstantEmpty{"用户设置的特化集是否为空"}
IsConstantEmpty--空-->Donothing("什么也不做")
IsConstantEmpty--非空-->GetIDAndValue("遍历特化集中每一个用户设置的特化常量对应ID和Value值")
GetIDAndValue-->IsSpecializationConstantsDeclaredInShader{"Shader中是否声明了相应的特化常量\n按照ID和Value的数值类型判断"}
IsSpecializationConstantsDeclaredInShader--未声明相应ID或类型对应不上-->Donothing
IsSpecializationConstantsDeclaredInShader--合法-->StatisticalCalculation("统计计算VkSpecializationInfo")
```

## Ray Tracing

### 历史回顾

对于基于硬件的实时光追，最早是`NVIDIA`在2018年10月10日在`DirectX 12`中正式发布的名为`DirectX Raytracing (DXR)`的功能。而`Khronos`组织也在2018年的1月成立了`Vulkan`光追组制定`Vulkan`的光追标准，并在2020年3月发布了一个临时扩展（没记错的话应该是`Vulkan 1.2`，当时的扩展名为`VK_KHR_ray_tracing`，而`VK_NV_ray_tracing`扩展在这之前就已经发布了），经过一些用户和硬件供应商的反馈，最终在2020年的11月份发布了`Vulkan`实时光追标准的最终版（`Vulkan 1.2.162`）。`Vulkan`实时光追标准的发布是里程碑式的存在，标志着第一个工业级开源、跨平台、跨设备的光追加速标准问世（甚至可以在移动设备上使用光追加速）。

### 光追标准

最终发布版与临时发布版大体上没什么区别，最终发布版如下：

`Vulkan`的扩展标准：

* [VK_KHR_acceleration_structure](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_KHR_acceleration_structure.html)
* [VK_KHR_ray_tracing_pipeline](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_KHR_ray_tracing_pipeline.html)
* [VK_KHR_ray_query](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_KHR_ray_query.html)
* [VK_KHR_pipeline_library](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_KHR_pipeline_library.html)
* [VK_KHR_deferred_host_operations](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_KHR_deferred_host_operations.html)

`SPIR-V`的扩展标准：

* [SPV_KHR_ray_tracing](http://htmlpreview.github.io/?https://github.com/KhronosGroup/SPIRV-Registry/blob/master/extensions/KHR/SPV_KHR_ray_tracing.html)
* [SPV_KHR_ray_query](http://htmlpreview.github.io/?https://github.com/KhronosGroup/SPIRV-Registry/blob/master/extensions/KHR/SPV_KHR_ray_query.html)

`GLSL`的扩展标准：

* [GLSL_EXT_ray_tracing](https://github.com/KhronosGroup/GLSL/blob/master/extensions/ext/GLSL_EXT_ray_tracing.txt)
* [GLSL_EXT_ray_query](https://github.com/KhronosGroup/GLSL/blob/master/extensions/ext/GLSL_EXT_ray_query.txt)
* [GLSL_EXT_ray_flags_primitive_culling](https://github.com/KhronosGroup/GLSL/blob/master/extensions/ext/GLSL_EXT_ray_flags_primitive_culling.txt)

### 扩展结构

最终发布版与临时发布版最显著的区别在于临时发布版的`VK_KHR_ray_tracing`扩展在最终发布版中被分成了三个扩展:

* `VK_KHR_acceleration_structure`：用于加速结构的构建和管理
* `VK_KHR_ray_tracing_pipeline`：用于光追着色器阶段和光追管线
* `VK_KHR_ray_query`：用于所有着色器阶段的内部光线查询

`Khronos`标准组采纳了一些市场调研和厂家的意见，提供单独的光线查询功能而不需要创建光追管线，所以原先的`VK_KHR_ray_tracing`扩展被细分，用于避免重复和依赖。实现（设备制造商）可以实现`VK_KHR_ray_tracing_pipeline`或者`VK_KHR_ray_query`之一，或是两者都实现，取决于市场需求。两个扩展都依赖于`VK_KHR_acceleration_structure`扩展，该扩展用于提供基本的加速结构的管理。对于桌面级别的设备供应商任致力于都支持`VK_KHR_ray_tracing_pipeline`和`VK_KHR_ray_query`扩展。

现在`Vulkan`光线追踪扩展标准由原先的临时提升到了核心中，也就是将光追扩展接口从`vulkan_beta.h`中移动到`vulkan_core.h`，所以用户不再需要声明`#define VK_ENABLE_BETA_EXTENSIONS`来激活`Vulkan`光线追踪的功能

这些扩展的依赖有些许变化，现在需要依赖`Vulkan 1.1`和`SPIR-V 1.4`。`VK_KHR_acceleration_structure`需要依赖`Vulkan 1.1`，`VK_EXT_descriptor_indexing`，`VK_KHR_buffer_device_address`和`VK_KHR_deferred_host_operations`。同样我们也意识到这么多依赖链很是繁琐，如果使用`Vulkan 1.2`事情将变得简单，但是并不是所有的平台都支持实现了`Vulkan 1.2`标准，并且我们并不想在光追标准中再增加其他的人为限值。我们也考虑到将`VK_KHR_deferred_host_operations`作为显示依赖是因为在创建管线时需要延迟操作需要该扩展。我们将`VK_KHR_pipeline_library`作为`VK_KHR_ray_tracing_pipeline`的一种相对松弛的扩展依赖而不是严格依赖，所以仅仅在使用相关扩展功能是才去激活扩展，用于减少负载。此外`VK_KHR_acceleration_structure`，`VK_KHR_ray_tracing_pipeline`和`VK_KHR_ray_query`都需要最小支持`SPIR-V 1.4`（该版本中增加了对于着色器的入口函数的改变），`SPIR-V 1.5`也可以在`Vulkan 1.2`上被使用。


功能方面，如下的标准需要所有的设备支持：

`VK_KHR_acceleration_structure`要求：
* `VK_KHR_deferred_host_operations`
* `accelerationStructure`
* `descriptorBindingAccelerationStructureUpdateAfterBind`
* `descriptorIndexing`有关的所有特性（如果支持`Vulkan 1.2`）或者使用`VK_EXT_descriptor_indexing`扩展
* `Vulkan 1.2`的`bufferDeviceAddress`或者`VK_KHR_buffer_device_address`扩展

设备如果支持`VK_KHR_ray_tracing_pipeline`要求：
* `VK_KHR_acceleration_structure`
* `rayTracingPipeline`
* `rayTracingPipelineTraceRaysIndirect`
* `rayTraversalPrimitiveCulling`（如果支持`VK_KHR_ray_query`）
* `VK_KHR_pipeline_library`

设备如果支持`VK_KHR_ray_query`要求：
* `VK_KHR_acceleration_structure`
* `rayQuery`

此外这些扩展有一些可选的功能。

对于`VK_KHR_acceleration_structure`要求：
* `accelerationStructureCaptureReplay`
* `accelerationStructureIndirectBuild`
* `accelerationStructureHostCommands`

对于`VK_KHR_ray_tracing_pipeline`要求：
* `rayTracingPipelineShaderGroupHandleCaptureReplay`
* `rayTracingPipelineShaderGroupHandleCaptureReplayMixed`
* `rayTraversalPrimitiveCulling`(如果不支持`VK_KHR_ray_query`)

### 加速结构（Acceleration Structures）

`Vulkan`对于光追扩展标准的最终版中变化最大的就是加速结构的创建和布局。

我们接纳了`API`转换层作者们的意见（比如[vkd3d-proton](https://github.com/HansKristian-Work/vkd3d-proton)），对于将`DXR`层置于`Vulkan`光追加速结构的顶层是不明智的。这使得加速结构的创建大小和存储在`VkBuffer`中发生了改变，而不是使用单独专用的加速结构进行存储。对应的变化位于`VkAccelerationStructureKHR`和`VkAccelerationStructureNV`句柄不再是指代同一实例句柄，并且不能混用。与之相似的结构或者函数在使用这个两个句柄时也不再相同，也不能混用。

我们同时增加了加速结构类型声明`VK_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR`。可以用在当加速结构在创建时还不知道确切的加速结构类型（在顶部或是在底部），确切的加速结构类型必须是`VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR`或者`VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR`，并且一旦创建就不能改变。开发`Vulkan`应用时不应该使用`VK_ACCELERATION_STRUCTURE_TYPE_GENERIC_KHR`，这会在将来影响设备功能和效率。

我们也接纳了`Vulkan`层实现作者们的意见（比如[MoltenVK](https://github.com/KhronosGroup/MoltenVK)），意见指出一些光追要求（比如设备地址）使得`Vulkan`在层级上难以在其他`API`上实现。不幸的是对于像支持`DXR`的奇偶校验同样是不可能的。我们希望在未来的其他`API`版本中会支持这些特性。

我们听说开发者比较喜欢统一的创建接口和构造参数，比如同一`VK_NV_ray_tracing`和`DXR`。我们将加速结构的创建更改成基于大小的创建，并且这个大小可与构造时的同一个结构体中计算出来（`vkGetAccelerationStructureBuildSizesKHR`），或者来自于压缩查询（`vkCmdWriteAccelerationStructuresPropertiesKHR`）。我们同时得知一些开发商需要在创建时得到更多信息，所以我们将`pMaxPrimitiveCounts`增加到了`vkGetAccelerationStructureBuildSizesKHR`中。

之前在几何描述某些方面是存在冲突的并且在自动代码生成方面不尽如人意（比如验证层（`validation layers`）），并且解决了由于`ppGeometries`的二元对立导致的歧义，并且增加了`pGeometries`，这要求二者在使用时只能指定其中一个使用。

其他的一些增加包括：加速结构的创建时间捕获和回溯标志位。`nullDescriptor`支持加速结构与`VK_EXT_robustness2`和`VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT`相互作用。

最后，我们将所有的扩展更改成一致使用设备地址。这其中有很多命名建议，其中一些已被采纳，并且一些名称和命名风格被修改成统一并且可扩展的方式。有关更多细节请查阅`VK_KHR_acceleration_structure`的问题`3`和`4`的更新日志。

### Host端的延迟操作（Deferred Host Operations）

*注：`Host`端一般指`CPU`端*

我么改造了`vkBuildAccelerationStructuresKHR`和`vkCreateRayTracingPipelinesKHR`指令的延迟`Host`端操作。取消了使用`pNext`对于各种独立创建和构造的链式操作，现在延迟操作通过指令的上层参数来设置指令是否为延迟操作。当指令为延迟操作的话，应用必须在延迟操作结束之后获取返回的数据。如果之前有一些延迟操作并且没有其他的措施防止不清晰的行为发生，这时难以明确合适能够获取安全数据。我们相信新的语义是清晰的并且对于并行友好，但是付诸的代价就是需要一直开启`VK_KHR_deferred_host_operations`扩展。

![Acceleration-Structures](../images/2020-Deferred-Host-Operations-enable-Acceleration-Structures-to-be-built-using-multiple-CPU-cores-for-faster-frame-rates-and-elimination-of-frame-stuttering-2.jpg)

*如上图为：`Host`延迟操作使用加速结构在多核`CPU`上部署已达到更高的帧率和限制帧停顿*

### 光追管线

对于光追管线的改变并不多，基本上都是改变内部的`SPIR-V`和着色器编译链相关。

此外`vkCreateRayTracingPipelinesKHR`中增加了适用延迟操作的修改，并且将`VkPipelineLibraryCreateInfoKHR`和`VkRayTracingPipelineInterfaceCreateInfoKHR`更改成可选项，这样如果`VkPipelineLibraryCreateInfoKHR`和`VkRayTracingPipelineInterfaceCreateInfoKHR`没有使用就不须要激活`VK_KHR_pipeline_library`扩展了。

对于光追管线最大的改变就是明确的增加了栈大小的管理。光追管线在进行光线追踪时需要调用所有的有关执行链，这潜在会有大量的着色器集。当着色器执行时，驱动实现可能会使用栈去存储参数数据，这也间接要求栈要足够大，进而可以处理所有着色器的任何执行链的调用。默认的栈大小可能相当的大，所以我们给应用提供在管线编译之后使用更优栈大小的可能。一般当应用可以计算出紧凑型栈内存理应使用更优的策略。并且增加了一个新的动态状态`VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR`用于使用光追管线去查询着色器组`vkGetRayTracingShaderGroupStackSizeKHR`的栈大小时使用，与此对应的设置管线栈大小的函数即为`vkCmdSetRayTracingPipelineStackSizeKHR`。

另一个新增特性来自于`DXR`层的积极反馈，即可以通过加速结构地址进行光线追踪。为此，加速结构的设备地址可通过`vkGetAccelerationStructureDeviceAddressKHR`将结果缓存在缓存中或者其他着色器资源中。对于着色器（`SPIR-V`），则可以使用`OpConvertUToAccelerationStructureKHR`显示转换声明`OpTypeAccelerationStructureKHR`描述符类型（在`GLSL`中使用`accelerationStructureEXT`构造）。其结果之后可以用于加速结构中使用`OpTraceRayKHR`指令进行追踪（`traceRayEXT()`）,此种转换是单向的，并且没有其他操作支持加速结构描述符。

![Ray-tracing-pipelines](../images/2020-Ray-tracing-pipelines-provide-implicit-management-of-ray-intersections-3.jpg)

*如上图为：光追管线提供的隐式光线求交管理*

`SPIR-V`标准组也对`SPIR-V`的扩展提供了反馈，将`Payload`参数更改成`OpTraceRayKHR`并且将`Callable Data`参数更改成`OpExecuteCallableKHR`。之前这些参数使用像`GLSL`中的`location`布局来进行匹配，然而这些`location`在`SPIR-V`没有意义，而是直接使用合适的存储类指针直接替换，为了实现这些需要新的指令操作码声明`OpTraceRayKHR`和`OpExecuteCallableKHR`。可以就与`SPV_NV_ray_tracing`进行划分防止混淆了。

另一个`SPIR-V`的改变来自于内部对于`OpIgnoreIntersectionKHR`和`OpTerminateRayKHR`的反馈，将这些转变成终止指令，因为他们会终止调用者的调用。这也必须是块中的最后一条指令。同样，这需要新的指令操作码。这对`GLSL`的冲击较大，这些函数指令将不再是内置函数，而是跳转语句，当在使用着色器时现在是仅仅使用`ignoreIntersectionEXT`而不是`ignoreIntersectionEXT()`。

`SPIR-V`对于光追管线的的变化总结就是：提供了新功能和枚举`RayTracingKHR`,使得驱动实现和工具链在已过时的`SPIR-V`和最终版之间进行区分。并且对于还对`ShaderRecordBufferKHR`所需的显式布局进行了一些澄清，并将其视为`StorageBuffer`的存储类。同时我们也规定了返回值和对于`OpReportIntersectionKHR`的值`T`越界所返回的相关行为，并澄清一部分位域用于各种光追参数。

对于`VK_KHR_acceleration_structure`扩展，我们将设备缓存地址独立出来，这样就可以使用光线追踪指令通过`VkStridedDeviceAddressRegionKHR`去获取着色器绑定表中的缓存设备地址。`vkCmdTraceRaysIndirectKHR`也与之相似的，通过缓存设备地址间接获取参数。

我们也更新了`Vulkan 1.2`与`VK_KHR_vulkan_memory_model`扩展之间的交互，并且要求一些内置变量成为`Volatile`变量，供着色器使用。

其他的改变包括增加创建2023年5月9日15:23:32捕获和着色器组句柄的回溯标志位，增加了一些之前忽略的属性和限值，和一些为了明确用途的重命名。有关更多细节请查阅`VK_KHR_ray_tracing_pipeline`的问题`3`和`4`，`SPV_KHR_ray_tracing`的问题`2`还有扩展的更新日志。

### 光线查询

考虑到`Vulkan`接口对于表面光线查询欠缺，大部分的光线查询改变都在`SPIR-V`扩展和交互中。

`SPV_KHR_ray_query`也是支持通过加速结构地址进行发射光线查询，并且增加`OpConvertUToAccelerationStructureKHR`用于将加速结构设备地址转变成`OpTypeAccelerationStructureKHR`描述符。这些在之后可以通过加速结构使用`OpRayQueryInitializeKHR`进行追踪。

对于光追管线，也新增了功能和枚举`RayQueryKHR`，这可以是的驱动的实现者将老版的临时版本与最终版明确区分开。我们同时为提出遮罩使用一套位域值，并且不允许查询`AABB`图元的候选`T`值。

最终，我们对于光线参数进行了数量上的限值，要求`HitT`作为光线间隔用于`OpRayQueryGenerateIntersectionKHR`，并且限值追踪顶级的加速结构。

对于更多的细节和其他改变，请查阅`VK_KHR_ray_query`的问题`1`，`SPV_KHR_ray_query`的问题`1`和扩展更新日志。

![Acceleration-Structures](../images/2020-Ray_Queries_provide_explicit_ray_management_from_within_any_shader-4.jpg)

*如上图为：光线查询提供从任意着色器中明确光线管理*

### 正路在此

这一章将会给出创建加速结构新流程的纵览和对于资源创建与光追进行同步的快速入门。

#### 加速结构的创建

*注：加速结构的创建和构建是两个不同的东西，创建指的是创建加速结构句柄，构建指的是创建加速结构内部数据和结构*

为了创建加速结构，应用必须首先确定加速结构需要的大小。对于创建时的加速结构、缓存大小和更新可通过`vkGetAccelerationStructureBuildSizesKHR`指令使用`VkAccelerationStructureBuildSizesInfoKHR`获得。对于创建加速结构时指定的`shape`和`type`位于`VkAccelerationStructureBuildGeometryInfoKHR`结构体中，该结构体之后也被用于真正的加速结构构建，但是此时加速结构的参数和几何数据并不需要全都填充完善（虽然可以填补完善），仅仅完善加速结构的类型、几何类型、数量和最大大小即可。这个大小可以支持任意足够相似的加速结构。对于加速结构目标会进行紧凑拷贝，这需要从`vkCmdWriteAccelerationStructuresPropertiesKHR`指令中获取大小。一旦需求的大小确定了，为加速结构创建`VkBuffer`（`accelerationStructureSize`），并且一个或多个`VkBuffer`用于创建（`buildScratchSize`）和更新（`updateScratchSize`）缓冲。

之后，加速结构`VkAccelerationStructureKHR`对象就可以使用`vkCreateAccelerationStructureKHR`指令根据`type`和`size`创建，并将结果存放在`VkAccelerationStructureCreateInfoKHR`中指定的`buffer`的`offset`位置中。与`Vulkan`中的其他资源不同，指定的这一部分魂村将会完全用于加速结构，并不需要额外的缓存用于加速结构的查询或者内存绑定。如果你愿意，多个加速结构甚至可以放在同一个`VkBuffer`中，只需要多个加速结构之间不互相覆盖即可。

最后，使用`vkCmdBuildAccelerationStructuresKHR`指令可以用于去构建加速结构。此构建使用的是与创建时相同的`VkAccelerationStructureBuildGeometryInfoKHR`结构体，但是此时此时需要指定所有的几何数据（顶点，索引，变换，`aabbs`包围盒，和实例）和缓存数据。一旦构建完成此加速结构将会完全自完善（`self-contained`），并且构建的输入和缓存可以重复利用除非在之后的构建更新中有计划使用他们。

#### 资源使用和同步

本章将提供各种缓存使用纵观，并且简单讲解光追操作有关的同步使用。

用于备份加速结构的缓存将会使用`VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR`进行创建。缓存将会使用`VK_BUFFER_USAGE_STORAGE_BUFFER_BIT`用于暂存空间，并且使用`VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR`用于加速结构的构建输入（比如顶点，索引，变换，`aabbs`包围盒，和实例）。如果缓存用于着色器绑定表将会使用`VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR`创建，并且如果使用间接构建的话追踪参数将会使用`VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT`创建使用。

为了与加速结构的构建指令（`vkCmdBuildAccelerationStructuresKHR`和`vkCmdBuildAccelerationStructuresIndirectKHR`）进行同步，需要使用`VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR`管线阶段声明。访问加速结构的数据源或目标数据缓存将使用`VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR`或`VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR`。构建时访问输入缓存（顶点，索引，变换，`aabbs`包围盒，和实例）使用`VK_ACCESS_SHADER_READ_BIT`访问类型并且访问间接参数使用`VK_ACCESS_INDIRECT_COMMAND_READ_BIT`访问类型。

为了与加速结构的拷贝指令（`vkCmdWriteAccelerationStructuresPropertiesKHR`、`vkCmdCopyAccelerationStructureKHR`、`vkCmdCopyAccelerationStructureToMemoryKHR`和`vkCmdCopyMemoryToAccelerationStructureKHR`）进行同步，同样需要使用`VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR`管线阶段。访问加速结构的读写使用`VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR`或`VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR`，此外通过设备地址访问的缓存使用`VK_ACCESS_TRANSFER_READ_BIT`或`VK_ACCESS_TRANSFER_WRITE_BIT`访问类型。

与光追指令（`vkCmdTraceRaysKHR`或`vkCmdTraceRaysIndirectKHR`）进行同步，`VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR`管线阶段用于着色器访问绑定的缓存表使用`VK_ACCESS_SHADER_READ_BIT`访问，对于访问间接数据在`VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT`管线阶段使用`VK_ACCESS_INDIRECT_COMMAND_READ_BIT`进行访问。

与加速结构在任何图形，计算或是光追管线阶段进行光线查询同步，对应的管线阶段使用`VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR`进行数据访问。

![Acceleration-Structures](../images/2020-Comparing-Vulkan-Ray-Tracing-and-DXR.-It-is-straightforward-to-port-code-between-the-two-APIs-including-re-use-of-ray-tracing-shaders-written-in-HLSL-5_.jpg)

*如上图为：`Vulkan`的光追与`DXR`的比较。两者之间进行移植非常简单，包括可共享的`HLSL`光追着色器*

### 结论和资源

现在最终版的`Vulkan`光追扩展已经发布，现在支持临时版本的嵌入式也会在不久支持最终版光追扩展，有关其他工具和组件可通过[GitHub](https://khr.io/vulkanrtfeedback)进行查阅。我们推荐所有的开发者转去使用最终的`Khronos`发布的`Vulkan`光追最终版。

支持`Vulkan`光追扩展的`NVIDIA`显卡驱动以及哪些显卡支持的信息可在[developer.nvidia.com/vulkan-driver](https://developer.nvidia.com/vulkan-driver)中找到。对于`AMD`显卡驱动可在[https://www.amd.com/en/support/kb/release-notes/rn-rad-win-20-11-2-vrt-beta](https://www.amd.com/en/support/kb/release-notes/rn-rad-win-20-11-2-vrt-beta)中找到。光追扩展同样可以通过2021年的`Intel`的`Xe-HPG`显卡支持，通过定期的驱动程序更新提供支持。

有关如何使用`Vulkan`光追用于混合渲染，同时使用光栅化和光追，以此来达到令人信服和画面同时保持交互程度请查看[Vulkan Ray Tracing Best Practices for Hybrid Rendering](https://www.khronos.org/blog/vulkan-ray-tracing-best-practices-for-hybrid-rendering)博客讨论有关在`Wolfenstein:Youngblood`（德军总部：新血脉）中使用最终版光追扩展实现的反射效果。

也可以查阅现今进行更新后支持光追扩展的[NVIDIA Vulkan Ray Tracing Tutorial](https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR)和2020年6月份发布的[NVIDIA Nsight Graphics developer tool](https://news.developer.nvidia.com/nvidia-announces-nsight-graphics-2020-6/)。请关注更多即将发布的有关的生产驱动程序、工具和示例公告。

`Vulkan`工作组同样鼓励开发者和内容创建社区使用`Vulkan`的光追扩展并积极反馈问题。这可通过[Khronos Developer Slack](https://khr.io/slack)和[Vulkan GitHub Issues Tracker](https://github.com/KhronosGroup/Vulkan-Docs/issues)进行讨论和反馈。

欢迎来到便携式、跨厂商、跨平台光线追踪加速的时代！
