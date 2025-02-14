<h1 align="center">Turbo</h2>
<p align="center">Turbo 是基于 Vulkan 开发的跨平台渲染引擎</p>
<p align="center">
  <img alt="LICENSE" src="https://img.shields.io/github/license/yiyungent/afdian-action.svg?style=flat"/>
  <img alt="repo size" src="https://img.shields.io/github/repo-size/FuXiii/Turbo.svg?style=flat"/>
  <img alt="QQ Group" src="https://img.shields.io/badge/QQ%20Group-128656761-deepgreen?logo=tencentqq"/>
  <img alt="Email" src="https://img.shields.io/badge/Email-g1018505124@outlook.com-deepgreen"/>
  <a href="https://afdian.net/@TurboEngine">
    <img alt="爱发电" src="https://afdian.moeci.com/1/badge.svg"/>
  </a>
</p>

![TurboImage](./docs/TurboImage.png)

<!--## Platform-->
# 平台

<p align="center">
  <img alt="Linux" src="https://img.shields.io/badge/Linux-Support-brightgreen?logo=linux&logoColor=f5f5f5"/>
  <img alt="Windows" src="https://img.shields.io/badge/Windows-Support-brightgreen?logo=windows&logoColor=f5f5f5"/>
  <img alt="IOS" src="https://img.shields.io/badge/IOS-Future-lightgrey?logo=apple&logoColor=f5f5f5"/>
  <img alt="Android" src="https://img.shields.io/badge/Android-Support-brightgreen?logo=Android&logoColor=f5f5f5"/>
  <img alt="HarmonyOS" src="https://img.shields.io/badge/HarmonyOS-Future-lightgrey?logo=harmonyos&logoColor=f5f5f5"/>
  <img alt="Web" src="https://img.shields.io/badge/Web(WebGPU)-Construction-orange?logo=internetexplorer&logoColor=f5f5f5"/>
</p>

<!--
## Version

当前版本 0.0.0.12
-->

<!--
## State  

* 研究开发 `Vulkan` 硬件实时光追。
* 研究开发`C/C++`开发`WebGPU`项目，并发布到浏览器页面中。  
  * `./sample` 下新增 `PureCCppWebGPUTest` 项目，详细信息请参考 [WebGPU : C/C++](./docs/Design/WebGPU_CCPP.md) 文档。
  * `./sample` 下新增 `PureCCppMiniWebGPUTest` 项目。
-->

# 赞助

想赞助的小伙伴，欢迎使用`爱发电`赞助，请量力而为。

> [!WARNING]
> 未成年人禁止投喂

<p align="center">
  <a href="https://indiafightscorona.giveindia.org">
    <img src="./docs/Aifadian/aifadian.jpg" alt="Give india logo" width="50%" />
  </a>
</p>

### 赞助项目
>
>* [Vulkan 入门精要](https://fuxiii.github.io/Essentials.of.Vulkan/)

* [![Next][Next.js]][Next-url]

[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/

<!--
>* [体积云教程](https://afdian.net/p/9ed29bbe64a411ed8c7752540025c377)：详情请阅览[`./docs/VolumetricCloud.md`](./docs/VolumetricCloud.md)（进行中）<研究光照散射中>
-->

## Document

* **相关文档现在可以在[Vulkan入门精要](https://fuxiii.github.io/Essentials.of.Vulkan/index.html)中查阅。比如[Vulkan KHR 光线追踪标准](https://fuxiii.github.io/Essentials.of.Vulkan/InformalEssay/KHRRayTracing.html)。**
* **详细设计文档请参考`docs/TurboDesign.drawio`(需要安装`drawwio`)**
* **目前存在的问题待解决，请查看`docs/Issue.md`**
* 开发记录录像请浏览 [Turbo引擎开发记录](https://space.bilibili.com/34673516)

## Modular

目前`Turbo`中有以下模块

![Core](https://img.shields.io/badge/Core-初步完成-brightgreen?style=flat-square&logo=appveyor)

* 命名空间：`Turbo::Core`
* 文档：`./docs/TurboDesign.drawio:Core`和[Core.md](./docs/Design/Core.md)
* 目录：`./engine/core`
* 依赖：独立模块，无依赖。
* 说明：`Core`作为核心模块直接与`Vulkan`沟通，是上层与`Vulkan`底层最直接的桥梁，`Turbo`中所有的`GPU`相关工作最终都会从上层回到`Core`层。

![FrameGraph](https://img.shields.io/badge/FrameGraph-初步完成-brightgreen?style=flat-square&logo=appveyor)

* 命名空间：`Turbo::FrameGraph`
* 文档：`./docs/TurboDesign.drawio:FrameGraph`和[FrameGraph.md](./docs/Design/FrameGraph.md)
* 目录：`./engine/framegraph`
* 依赖：独立模块，无依赖。
* 说明：`FrameGraph`用于描述一帧中的资源、渲染配置和渲染指令

![Render](https://img.shields.io/badge/Render-开发中-orange?style=flat-square&logo=appveyor)

* 命名空间：`Turbo::Render`
* 文档：[FrameGraphAdvance.md](./docs/Design/FrameGraphAdvance.md)
* 目录：`./engine/render`
* 依赖：`Core`和`FrameGraph`。
* 说明：由于直接使用`Core`层进行渲染相对来说还是会比较繁琐吃力一些，`Render`模块的出现就是将`Core`和`FrameGraph`结合起来，提供更加方便的工作流，将开发者从繁杂的`Core`层脱离出来，提供更加简单易用的设计架构

## Clone

```CMD
git clone --recursive git@github.com:FuXiii/Turbo.git
```

## Build

* 首先您需要知道的：
  * `Turbo`目前主要有两个分支：`master`和`dev`。其中`master`是主分支，确保可以正常编译和使用，而`dev`分支会经常开发新功能，想看开发推进的可以查看`dev`分支，但不保证能够正常编译。
  * `Turbo`引擎被设计成各种模块，有`Core`核心模块, 有`FrameGraph`模块等。
  * 目前`Turbo`的核心模块基本完成(未完成部分和相关问题请参考下面的`RoadMap`章节和`docs/Issue.md`文档)
  * `Turbo`引擎的核心位于`./engine/core`，这是一个单独的模块，您可以直接将他拷贝出来放到自己的工程中
  * `Turbo`引擎会使用核心进行渲染，有关如何使用该核心，目前可以参考`./main.cpp`和`./samples`文件夹下各个示例。该文件中有最新的核心实例代码，同时也是引擎的一部分。
  * 核心会用到的第三方库为：
    * `glslang` : 用于将`Shader`代码字符串编译成`Spir-V`
    * `SPIRV-Cross` : 用于解析`Spir-V`,获取`Shader`中相关声明
    * `VulkanMemoryAllocator` : 用于分配`Vulkan`资源内存
  * ~~本人对于`CMake`并不是非常精通，有关核心是如何寻找`Vulkan`库的，我直接配置的绝对路径，请修改`engine/core/CMakeLists.txt`中的此行代码：~~
    >
    > ```CMake
    > #已遗弃
    > ##set_target_properties(vulkan PROPERTIES IMPORTED_LOCATION F:/VulkanSDK/1.3.204.1/Lib/vulkan-1.lib)
    > ```
    >
    ~~请将该行修改成您电脑上的`Vulkan`库目录。
    该问题将会在不远的未来通过动态加载库文件得到解决。~~
    >┗|｀O′|┛oO 2022/7/27 该问题已修改完成，详情请参考下面的`如何编译Turbo`章节的`注`

  * `Turbo`非核心模块，也就是目前除了`./engine/core`之外，基本上就是`Turbo`的非核心了，之后将在核心之外，基于核心开发高级模块，比如`FrameGraph`之类的，目前非核心模块已有核心模块使用案例，位于`./main.cpp`和`./samples`文件夹下，就像前面说的，用例使用如下第三方库：
    * [`glfw`](https://github.com/glfw/glfw) :窗口库
    * [`glm`](https://github.com/g-truc/glm) : 用于向量矩阵等运算
    * [`imgui`](https://github.com/ocornut/imgui) : 用于绘制界面UI
    * [`KTX-Software`](https://github.com/KhronosGroup/KTX-Software) : `Khronos Texture`通用纹理标准
    * [`tinygltf`](https://github.com/syoyo/tinygltf) : 读取`gltf`文件库

* 如何编译`Turbo`
  * 请安装[Vulkan SDK](https://vulkan.lunarg.com/)
    * ( ***注**：2022/7/27 对于`Windows`系统，目前`Turbo`已经完成了动态加载`Vulkan`函数，~~`Vulkan SDK`目前对于`Turbo`不是必需品~~(有些第三方依赖需要`Vulkan SDK`，比如`VulkanMemoryAllocator`)，`Vulkan`的`Runtime`是`Turbo`的必须品，正常`Windows`都会自带该运行时库，如果没有请安装[Vulkan Latest Runtime](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-runtime.exe)即可，~~`Linux`系统等有空适配一下~~(2022/11/14 `Linux`适配完成))
      >[Vulkan Loader 文档有这么一句：](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/docs/LoaderApplicationInterface.md)
      In previous versions of the loader, it was possible to statically link the loader. **This was removed and is no longer possible**. The decision to remove static linking was because of changes to the driver which made older applications that statically linked unable to find newer drivers.
  * `Turbo`的核心可以单独编译，编译相关的`CMakeLists.txt`位于`./engine/core/CMakeLists.txt`。将会输出名为`TCore`的库文件。

    * 2023/9/21 添加了`TCORE_SHARED_LIBS`的`CMake`选项。用于配置是否输出`TCore`的动态库。默认情况下为`OFF`，也就是输出静态库，如果想输出动态库请设置为`ON`。

  * 如果您想直接编译`Turbo`
    1. 首先请查看环境变量中是否已经加入了`git`的`bin`目录，`KTX-Sofware`编译依赖`bash.exe`，正常该程序位于`git`的`bin`目录下
    2. 请安装`python`。第三方库很多`CMake`使用`Python`脚本运行，安装完后请确保`Python`的`{Python的安装目录}/Python{版本号}/`目录和`{Python的安装目录}/Python{版本号}/Scripts`目录加入到了环境变量中
    3. ~~请修改`engine/core/CMakeLists.txt`中的`Vulkan`库目录为您自己的目录~~
    4. 之后使用`./CMakeLists.txt`即可
    5. 设置相关`CMake`参数如下：

        ```CPP
        TCORE_SHARED_LIBS=ON //2023/9/21 Turbo的核心库输出为动态库，如果想输出为静态库请设置为OFF
        ```

  *注：如果编译有遇到问题请查看[`常见问题文档`](./docs/FAQ.md)如果还是没有解决方法请提`Issue`*

## Trifles

* 整理一下头文件，有点乱，去掉不必要的头文件

## RoadMap

*注：该`RoadMap`章节信息有滞后性，引擎具体细节和开发计划请查看后面的[更新日志](./Changelog.md)(更新的比较频繁)*

2022/5/15

>* `Turbo` 引擎对于 `Vulkan 1.0` 的相关核心功能已初步成型，由于`Vulkan`本身的复杂性还有很多特性没有实现，在此做一下记录，记录该引擎以实现的特性和未来计划实现特性，该特性记录将会和`docs/Issue.md`文档配合使用。
>* `Core`核心层将会作为单独完整的库发布,位于`./engine/core`中, 现在核心库已分离成单独的库，将会输出`TCore.lib`或`TCore.a`库文件
>* `Turbo`将使用该`Core`核心继续开发高级特性
>* 未来有时间录制`Vulkan教程`和`引擎开发教程`视频

2022/5/28

>* 实现完`Vulkan`的`input attachment`也就是完成了延迟渲染相关工作，`Turbo`核心现在对于图形的核心封装设计基本上接近尾声，延迟渲染之后将会开展`FrameGraph`相关设计实现工作。有关几何着色器，细分着色器，光线追踪等将会在`Turbo`的核心继续展开
>* `FrameGraph`将会是`Turbo`引擎实现高级特性的重点，`ECS（Entity Component System）`或者`EC（Entity Component）`之类管理场景的相关技术不是`Turbo`引擎的重点，`Turbo`引擎目前的重点目标是不依赖任何窗口库（用户自定义），不依赖任何场景结构技术（用户自己实现自己的场景结构），`Turbo`可以很轻松的嵌入其中。

2022/5/30

>* `Turbo`核心基本实现完成，接下来开始`FrameGraph`的设计开发。敬请期待。

2022/6/7

>* 已开始写系列的使用示例。

2022/8/10

>* 开始设计实现`engine`层

### 已完成示例

* `HelloTriangle` - 将会使用核心绘制`IMGUI`和三角形
* `PureHelloTriangle` - 将会使用核心绘制三角形
* `PureCombinedImageSampler` - 将会使用核心绘制三角形,在此基础上使用`纹理采样器一体式`对纹理采样
* `PureSeparateImageSampler` - 将会使用核心绘制三角形,在此基础上使用`纹理采样器分离式`对纹理采样
* `PureIndexDraw` - 将会使用核心，基于索引渲染绘制一个方形面片，并在此基础上使用`纹理采样器分离式`对纹理采样
* `FrameGraphSample` - `FrameGraph`的示例，目前该示例仅供测试（`FrameGraph`未实现完成），您可以通过该示例了解`FrameGraph`的基本流程

### 已完成特性

1.`Vulkan`核心特性的基本封装，包括：

* `Turbo::Core::TLayerInfo` - `Vulkan`相关验证层信息
* `Turbo::Core::TExtensionInfo` - `Vulkan`相关扩展信息
* `Turbo::Core::TVersion` - 通用版本
* `Turbo::Core::TInstance` - `VkInstance`的相关封装
* `Turbo::Core::TPhysicalDevice` - `VkPhysicalDevice`相关封装
* `Turbo::Core::TDevice` - `VkDevice`相关封装
* `Turbo::Core::TDeviceQueue` - `VkQueue`相关封装
* `Turbo::Core::TImage` - `VkImage`相关封装
* `Turbo::Core::TImageView` - `VkImageView`相关封装
* `Turbo::Core::TCommandBufferPool` - `VkCommandPool`相关封装
* `Turbo::Core::TCommandBuffer` - `VkCommandBuffer`相关封装
* `Turbo::Core::TBuffer` - `VkBuffer`相关封装
* `Turbo::Core::TShader` - `VkShaderModule`相关封装
* `Turbo::Core::TDescriptorPool` - `VkDescriptorPool`相关封装
* `Turbo::Core::TSampler` - `VkSampler`相关封装
* `Turbo::Core::TSubpass` - `Vulkan`中`Subpass`抽象
* `Turbo::Core::TAttachment` - `Vulkan`中`Attachment`抽象
* `Turbo::Core::TRenderPass` - `VkRenderPass`相关封装
* `Turbo::Core::TVertexBinding` - `Vulkan`中顶点属性抽象
* `Turbo::Core::TViewport` - `Vulkan`中`Viewport`抽象
* `Turbo::Core::TScissor` - `Vulkan`中`Scissor`抽象
* `Turbo::Core::TDescriptor` - `Vulkan`中各种描述符抽象
* `Turbo::Core::TDescriptorSetLayout` - `VkDescriptorSetLayout`的相关封装
* `Turbo::Core::TPipelineLayout` - `VkPipelineLayout`的相关封装
* `Turbo::Core::TPipeline` - `TPipelineLayout`的相关封装,内部有`VkPipeline vkPipeline`成员变量，但创建是在其子类中创建
* `Turbo::Core::TGraphicsPipeline` - 继承自`Turbo::Core::TPipeline`,图形渲染管线`VkPipeline`的相关封装，回去创建父类中的`Turbo::Core::TPipeline::vkPipeline`
* `Turbo::Core::TDescriptorSet` - `VkDescriptorSet`的相关封装
* `Turbo::Core::TPipelineDescriptorSet` - 管线所有有关描述符集的总和抽象，对应多个`Turbo::Core::TDescriptorSet`
* `Turbo::Core::TFramebuffer` - `VkFramebuffer`相关封装
* `Turbo::Core::TSemaphore` - `VkSemaphore`相关封装
* `Turbo::Core::TFence` - `VkFence`相关封装
* `Turbo::Extension::TSurface` - `VkSurfaceKHR`相关封装
* `Turbo::Extension::TSwapchain` - `VkSwapchainKHR`相关封装

### 计划完成特性

* `Turbo`引擎目前以`Core`核心层为主（优先级不分先后）
  * **[ ✓ ]** 生成`mipmap`
  * **[ ✓ ]** 混合渲染
  * **[ ✓ ]** 天空盒
  * HDR（应该算是延迟渲染的一部分）
  * **[ ✓ ]** 实例化渲染
  * 间接渲染
  * **[ ✓ ]** 细分着色器
  * **[ ✓ ]** 几何着色器
  * `Vulkan`光追标准（提供示例，还未设置`Turbo`接口）
  * 多线程
  * **[ ✓ ]** 计算着色器
  * **[ ✓ ]** 计算管线
  * **[ ✓ ]** 延迟渲染

* 非`Core`：跨平台窗口层抽象

* 非`Core`：`IMGUI`

* 非`Core`：`KTX` 和 `glTF`

* 非`Core`：`FrameGraph`层

* 非`Core`：`FrameGraph`层实现`PBR`

* 非`Core`：`ECS`层

* 2022/7/13 搞了个`鸿蒙OS(Harmony OS)`系统的手机，有时间适配一下鸿蒙设备。

## Log

> **(2024/12/17)** 日志文件转移到了 [更新日志](./Changelog.md)
