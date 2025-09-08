<h1 align="center">Turbo</h2>
 <p align="center">
    <a href="./README.md">简体中文</a>
  </p>
<p align="center">Turbo is rendering engine base Vulkan</p>
<p align="center">
  <img alt="LICENSE" src="https://img.shields.io/github/license/yiyungent/afdian-action.svg?style=flat"/>
  <img alt="repo size" src="https://img.shields.io/github/repo-size/FuXiii/Turbo.svg?style=flat"/>
  <img alt="QQ Group" src="https://img.shields.io/badge/QQ%20Group-128656761-deepgreen?logo=tencentqq"/>
  <img alt="Email" src="https://img.shields.io/badge/Email-g1018505124@outlook.com-deepgreen"/>
  <a href="https://afdian.net/@TurboEngine">
    <img alt="爱发电" src="https://afdian.moeci.com/1/badge.svg"/>
  </a>
</p>

<p align="center">
  <img alt="TurboImage" src="./docs/TurboImage.png"/>
</p>

<!--## Platform-->
# Platform

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

<!--
>* [体积云教程](https://afdian.net/p/9ed29bbe64a411ed8c7752540025c377)：详情请阅览[`./docs/VolumetricCloud.md`](./docs/VolumetricCloud.md)（进行中）<研究光照散射中>
-->

<!--
## Document
-->

# Document

* Relative document can be read in [Essentials.of.Vulkan](https://fuxiii.github.io/Essentials.of.Vulkan/index.html). such as [Vulkan KHR Ray Tracing standard](https://fuxiii.github.io/Essentials.of.Vulkan/InformalEssay/KHRRayTracing.html).
* Deltail design document please reference `docs/TurboDesign.drawio`(need install `drawwio`).
* Still need to fix issue，please check `docs/Issue.md`.
* Programming record video [Turbo engine record](https://space.bilibili.com/34673516)

<!--
## Modular
-->

# Modular

>![Core](https://img.shields.io/badge/Core-Preliminary_Completion-brightgreen?style=flat-square&>logo=appveyor)
>
>| item | info |
>| :- |:-|
>|namespace|`Turbo::Core`|
>|doc|`./docs/TurboDesign.drawio:Core` and [Core.md](./docs/Design/Core.md)|
>|path|`./engine/core`|
>|dependency|discrete|
>|describe|`Core` as kernel modular communicate with `Vulkan`. It is the most important bridge between `Core` and `Vulkan`. All the calling in upper strecuture will return to `Core`。|

>![FrameGraph](https://img.shields.io/badge/FrameGraph-Developing-orange?style=flat-square&logo=appveyor)
>
>| item | info |
>| :- |:-|
>|namespace|`Turbo::FrameGraph`|
>|doc|`./docs/TurboDesign.drawio:FrameGraph`和[FrameGraph.md](./docs/Design/FrameGraph.md)|
>|path|`./engine/framegraph`|
>|dependency|discrete|
>|describe|`FrameGraph` used to record the resource, rendering configure and command in a frame|

>![Render](https://img.shields.io/badge/Render-Developing-orange?style=flat-square&logo=appveyor)
>
>| item | info |
>| :- |:-|
>|namespace|`Turbo::Render`|
>|doc|[FrameGraphAdvance.md](./docs/Design/FrameGraphAdvance.md)|
>|path|`./engine/render`|
>|dependency|`Core`和`FrameGraph`。|
>|describe|Due to the complexity of directly using `Core`，`Render` modular will combine `Core` and `FrameGraph` for a more clearly work pass for get free from `Core`. |

<!--
| 模块 | 命名空间 | 文档 | 目录 | 依赖 | 说明 |
| :- |:-|:-:|:-:|:-:|:-:|
| `Core`|`Turbo::Core`| [TurboDesign::Core](./docs/TurboDesign.drawio:Core)和[Core.md](./docs/Design/Core.md) |`./engine/core`|独立模块|`Core`作为核心模块直接与`Vulkan`沟通，是上层与`Vulkan`底层最直接的桥梁，`Turbo`中所有的`GPU`相关工作最终都会从上层回到`Core`层。|
| `FrameGraph`|`Turbo::FrameGraph`|  [TurboDesign::Core](./docs/TurboDesign.drawio:FrameGraph)和[FrameGraph.md](./docs/Design/FrameGraph.md) |`./engine/framegraph`|独立模块|`FrameGraph`用于描述一帧中的资源、渲染配置和渲染指令|
| `Render`| `Turbo::Render`| [FrameGraphAdvance.md](./docs/Design/FrameGraphAdvance.md)|`./engine/render`|`Core`和`FrameGraph`|由于直接使用`Core`层进行渲染相对来说还是会比较繁琐吃力一些，`Render`模块的出现就是将`Core`和`FrameGraph`结合起来，提供更加方便的工作流，将开发者从繁杂的`Core`层脱离出来，提供更加简单易用的设计架构|
-->

> [!NOTE]
> for now mainly developing `Core`

<!--
## Clone
-->
# Clone

```
git clone --recursive https://github.com/FuXiii/Turbo.git
```

> [!NOTE]
> Third party maintained by `submodule` in `git`, so need `--recursive` to automatically download third party.

<!--
## Build
-->
# Build

> [!NOTE]
>
> * For now `Turbo` have two main branches : `master` and `dev`. The `master` is default branch, it ensure that compilation and develope passed without deadly bug. The `dev` usually develope new feature, if you want know what changed in develope forward, please check `dev` branch, this branch not promise can compile smoothly.
> * `Turbo` engine kernel in `./engine/core` is a discrete modular and can be islanded compile. You can directly copy them to yourself project. Will output lib file which named `TCore` .
> * `Turbo` engine will use kernel to develope new feature, about how to use them can reference codes under `./main.cpp` and `./samples` which supply many demo.
> * `Core` modular used thirdparty：
>
>   | lib | info |
>   | :------------ |:---------------|
>   |[glslang](https://github.com/KhronosGroup/glslang)|Compile `Shader` code string to `SPIR-V`|
>   |[SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) | Parse `SPIR-V`，fetch declare interface in `Shader`.（Maybe consider to use  `SPIRV-Reflect` forsake `SPIRV-Cross` ）|
>   |[SPIRV-Reflect](https://github.com/KhronosGroup/SPIRV-Reflect) |Parse `SPIR-V`，fetch declare interface in `Shader`.|
>   |[VulkanMemoryAllocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) | Manage `Vulkan` memory resource|
>
> * `Non-Core` modular used thirdparty：
>
>   | lib | info |
>   | :------------ |:---------------|
>   |[glfw](https://github.com/glfw/glfw)|Window lib|
>   |[glm](https://github.com/g-truc/glm)|Use for vector and matrix calculate|
>   |[imgui](https://github.com/ocornut/imgui)|Use for draw UI|
>   |[KTX-Software](https://github.com/KhronosGroup/KTX-Software)|`Khronos Texture` uniform texture standard|
>   |[tinygltf](https://github.com/syoyo/tinygltf)|Read `gltf` |

> [!IMPORTANT]\
> [Vulkan SDK](https://vulkan.lunarg.com/) is not necessity for `Turbo` (Not dependent [Vulkan SDK](https://vulkan.lunarg.com/))

<details>
<summary>Windows</summary>

1. Need check the ``bin`` path under the ``git`` had been add in system enironment(`KTX-Sofware` compile dependent `bash.exe`, normaly it will under `git/bin`).
2. Please install `Python` (Many thirdparty use `Python` script ，after install please make sure `${Python install path}/Python{Version}/`目录和`${Python install path}/Python{Version}/Scripts` had been add in system enironment)
3. use `./CMakeLists.txt`
4. related `CMake` variable:

    ```CPP
    TCORE_SHARED_LIBS=ON // Turbo core will output dynamic lib，if you want output static lib set to OFF (default is OFF)
    ```

```
cd ${Custome path}
git clone --recursive https://github.com/FuXiii/Turbo.git
cd ./Turbo
mkdir build
cd ./build
cmake ..
cmake --build . --config Release
```

</details>

<details>
<summary>Linux</summary>

1. Config environment

  ```cxx
  sudo apt install libx11-dev       //cmake
  sudo apt install libxrandr-dev    //glfw 
  sudo apt install libxinerama-dev  //glfw 
  sudo apt install libxcursor-dev   //glfw 
  sudo apt install libxi-dev        //glfw 
  sudo apt install libwayland-dev   //build 
  sudo apt install build-essential  //build 
  sudo apt install freeglut3-dev    //build 
  ```

2. use `./CMakeLists.txt`
4. related `CMake` variable:

    ```CPP
    TCORE_SHARED_LIBS=ON // Turbo core will output dynamic lib，if you want output static lib set to OFF (default is OFF)
    ```

```cmd
sudo apt install libx11-dev      
sudo apt install libxrandr-dev   
sudo apt install libxinerama-dev 
sudo apt install libxcursor-dev  
sudo apt install libxi-dev       
sudo apt install libwayland-dev  
sudo apt install build-essential 
sudo apt install freeglut3-dev   

cd ${Custome path}
git clone --recursive https://github.com/FuXiii/Turbo.git
cd ./Turbo
mkdir build
cd ./build
cmake ..
make
```

</details>

> [!NOTE]
> If you meet some annoying question please check [`FAQ`](./docs/FAQ.md) first, if it still can't fixed your problem please create an `Issue`.

# Sample

在 `samples` 下有很多示例供开发者查阅。

In `samples` folder there has many demo for developer.

> Among which `HelloTriangle（HelloTriangle.cpp）` is the demo which recommend to beginner(major code about `400` lines)。By drawing a triangle to show the core usage, it is relatively simple and friendly to beginners.

<!--
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
-->

# Vulkan Map

| Core | Vulkan |
| :------------ |---------------:|
|`Turbo::Core::TLayerInfo` |`Vulkan` info about `Layer`|
|`Turbo::Core::TExtensionInfo` |`Vulkan` info about `extension`|
|`Turbo::Core::TVersion` |Common version|
|`Turbo::Core::TInstance` |`VkInstance` encapsulation|
|`Turbo::Core::TPhysicalDevice` |`VkPhysicalDevice` encapsulation|
|`Turbo::Core::TDevice` |`VkDevice` encapsulation|
|`Turbo::Core::TDeviceQueue` |`VkQueue` encapsulation|
|`Turbo::Core::TImage` |`VkImage` encapsulation|
|`Turbo::Core::TImageView` |`VkImageView` encapsulation|
|`Turbo::Core::TCommandBufferPool` |`VkCommandPool` encapsulation|
|`Turbo::Core::TCommandBuffer` |`VkCommandBuffer` encapsulation|
|`Turbo::Core::TBuffer` |`VkBuffer` encapsulation|
|`Turbo::Core::TShader` |`VkShaderModule` encapsulation|
|`Turbo::Core::TDescriptorPool` |`VkDescriptorPool` encapsulation|
|`Turbo::Core::TSampler` |`VkSampler` encapsulation|
|`Turbo::Core::TSubpass` |`Subpass` abstruct|
|`Turbo::Core::TAttachment` |`Attachment` abstruct|
|`Turbo::Core::TRenderPass` |`VkRenderPass` encapsulation|
|`Turbo::Core::TVertexBinding` |`Vulkan` vertex attribute abstruct|
|`Turbo::Core::TViewport` |`Viewport` abstruct|
|`Turbo::Core::TScissor` |`Scissor` abstruct|
|`Turbo::Core::TDescriptor` |`Descriptor` abstruct|
|`Turbo::Core::TDescriptorSetLayout` |`VkDescriptorSetLayout`encapsulation|
|`Turbo::Core::TPipelineLayout` |`VkPipelineLayout`encapsulation|
|`Turbo::Core::TPipeline` |`TPipelineLayout`encapsulation，internal have `VkPipeline vkPipeline` member variable，but create in child class|
|`Turbo::Core::TGraphicsPipeline` |Inherited from `Turbo::Core::TPipeline`, graphic pipeline `VkPipeline` encapsulation，will create `Turbo::Core::TPipeline::vkPipeline` which in it's parent|
|`Turbo::Core::TDescriptorSet` |`VkDescriptorSet`encapsulation|
|`Turbo::Core::TPipelineDescriptorSet` |Manage all the `Descriptor Set` and abstruct，corresponding to multiple `Turbo::Core::TDescriptorSet`|
|`Turbo::Core::TFramebuffer` |`VkFramebuffer` encapsulation|
|`Turbo::Core::TSemaphore` |`VkSemaphore` encapsulation|
|`Turbo::Core::TFence` |`VkFence` encapsulation|
|`Turbo::Extension::TSurface` |`VkSurfaceKHR` encapsulation|
|`Turbo::Extension::TSwapchain` |`VkSwapchainKHR` encapsulation|

<!--
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
-->

<!--
# Log
-->
# Log

> [!NOTE]
> Log file move to [Daily updates](./Changelog.md)

# Sponsor

If you want sponsor, welcome use `爱发电`.

> [!WARNING]
> Minors are prohibited for sponsor

<p align="center">
  <a href="https://indiafightscorona.giveindia.org">
    <img src="./docs/Aifadian/aifadian.jpg" alt="Give india logo" width="45%" />
  </a>
</p>

## Sponsored Project

<a href="https://indiafightscorona.giveindia.org">
 <img alt="Essentials.of.Vulkan" src="https://img.shields.io/badge/Essentials.of.Vulkan-151718?style=for-the-badge&logo=vulkan&logoColor=a41e22"/>
</a>
