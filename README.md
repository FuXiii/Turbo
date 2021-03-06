# Turbo

Turbo是渲染引擎

## Version

当前版本 0.0.0.12

## State

* 开发中

## Document

* 使用MarkDown书写,使用docsify部署(之前是使用MkDocs)
* 使用MarkDown书写文字和文档，使用drawio绘制设计图表
* **详细设计文档请参考`docs/TurboDesign.drawio`(需要安装`drawwio`)**
* **目前存在的问题待解决，请查看`docs/Issue.md`**
* 开发记录录像请浏览 [Turbo引擎开发记录](https://space.bilibili.com/34673516)

## Build

* 首先您需要知道的：
  * `Turbo`引擎被设计成各种模块，有`Core`核心模块, 有`FrameGraph`模块等。
  * 目前`Turbo`的核心模块基本完成(未完成部分和相关问题请参考下面的`RoadMap`章节和`docs/Issue.md`文档)
  * `Turbo`引擎的核心位于`./engine/core`，这是一个单独的模块，您可以直接将他拷贝出来放到自己的工程中
  * `Turbo`引擎会使用核心进行渲染，有关如何使用该核心，目前可以参考`./main.cpp`。该文件中有最新的核心实例代码，同时也是引擎的一部分。
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

  * `Turbo`非核心模块，也就是目前除了`./engine/core`之外，基本上就是`Turbo`的非核心了，之后将在核心之外，基于核心开发高级模块，比如`FrameGraph`之类的，目前非核心模块已有核心模块使用案例，位于`./main.cpp`，就像前面说的，该用例使用如下第三方库：
    * `glfw` :窗口库
    * `glm` : 用于向量矩阵等运算
    * `imgui` : 用于绘制界面UI
    * `KTX-Sofware` : `Khronos Texture`通用纹理标准
    * `tinygltf` : 读取`gltf`文件库

* 如何编译`Turbo`
  * 请安装[Vulkan SDK](https://vulkan.lunarg.com/)
    * ( ***注**：2022/7/27 对于`Windows`系统，目前`Turbo`已经完成了动态加载`Vulkan`函数，~~`Vulkan SDK`目前对于`Turbo`不是必需品~~(有些第三方依赖需要`Vulkan SDK`，比如`VulkanMemoryAllocator`)，`Vulkan`的`Runtime`是`Turbo`的必须品，正常`Windows`都会自带该运行时库，如果没有请安装[Vulkan Latest Runtime](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-runtime.exe)即可，`Linux`系统等有空适配一下)
  * `Turbo`的核心可以单独编译，编译相关的`CMakeLists.txt`位于`./engine/core/CMakeLists.txt`。将会输出名为`TCore`的库文件。
  * 如果您想直接编译`Turbo`
    1. 首先请查看环境变量中是否已经加入了`git`的`bin`目录，`KTX-Sofware`编译依赖`bash.exe`，正常该程序位于`git`的`bin`目录下
    2. 请安装`python`。第三方库很多`CMake`使用`Python`脚本运行，安装完后请确保`Python`的`{Python的安装目录}/Python{版本号}/`目录和`{Python的安装目录}/Python{版本号}/Scripts`目录加入到了环境变量中
    3. ~~请修改`engine/core/CMakeLists.txt`中的`Vulkan`库目录为您自己的目录~~
    4. 之后使用`./CMakeLists.txt`即可
    5. 设置相关`CMake`参数如下：

        ```CPP
        KTX_FEATURE_LOADTEST_APPS=OFF//如果您想加载KTX测试，请设置ON
        KTX_FEATURE_DOC=OFF//如果您想生成KTX文档，请设置ON
        KTX_FEATURE_STATIC_LIBRARY=ON //目前Turbo按照静态库使用KTX

        //2022/7/30 关于解决Turbo核心库的依赖库问题解决，核心库对于VulkanMemoryAllocator使用动态加载Vulkan API方式，这也是Turbo引擎加载Vulkan API的方式
        VMA_STATIC_VULKAN_FUNCTIONS=0
        VMA_DYNAMIC_VULKAN_FUNCTIONS=1
        ```

* 如何运行
    1. 由于每个用户输出的目录都不一样，所以`./main.cpp`的示例程序使用的资源文件使用的是相对路径，所有的资源文件都指向`./asset/`目录，请在`./main.cpp`中全局搜索`asset`字符，替换成自己的目录即可。

## Trifles

* 整理一下头文件，有点乱，去掉不必要的头文件

## RoadMap

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
  * 实例化渲染
  * 间接渲染
  * 细分着色器
  * 几何着色器
  * `Vulkan`光追标准
  * 多线程
  * 计算着色器
  * 计算管线
  * **[ ✓ ]** 延迟渲染

* 非`Core`：跨平台窗口层抽象

* 非`Core`：`IMGUI`

* 非`Core`：`KTX` 和 `glTF`

* 非`Core`：`FrameGraph`层

* 非`Core`：`FrameGraph`层实现`PBR`

* 非`Core`：`ECS`层

* 2022/7/13 搞了个`鸿蒙OS(Harmony OS)`系统的手机，有时间适配一下鸿蒙设备。

## Log

* 2020/10/29
    >
    >* 完成`TInstance` `TLayer` `TObject`的文档，创建`TExtension`文档。目前跳转链接没有全部测试，有些页面还没有创建。
    >

* 2020/10/29
    >
    >* 完成`TExtension`文档。页内跳转还没写。`TExtensionType`值和描述还没写

* 2022/3/17 基本调通了Turbo在VSCodes上的部署。
   >
   > * 注意`TApplication::InitVulkan`对于验证层的修改。
   > * 搞清Turbo中一些函数返回局部变量，之后没有拷贝一份而直接传给Vulkan API后会卡住的问题
  > * 将TApplication改成TEngine，作为Turbo最高层抽象
  >
* 2022/3/18
    >
    >* 继承自`TInfo`的类用于实现Vulkan相关Info功能，其中的GetVkXXXInfo等相关信息函数是返回局部变量，需要函数返回立刻保存数据，可能会有问题，先暂时这样。

* 2022/3/19 设计架构
    >
    >* 重构`TAllcator`，创建`TVulkanAllocator`类专门用于Vulkan内存分配,并作为引擎的默认内存分配器
    >* 重写`TInstance::IsSupportVulkan()`函数，原先有隐藏的Bug
    >* 将`Turbo::Core::TPhysicalDeviceProxy`下的`static VkResult EnumeratePhysicalDevices(TInstance *instance)`改成私有
    >* `Turbo::Core::TEngine::InitVulkan()`函数中不应开启任何layer或extension，该函数仅仅是为用户提供收集Vulkan相关信息功能。
    >* TEngine中增加如下  
    >   * `uint32_t GetPhysicalDeviceCount();`
    >   * `std::vector\<TPhysicalDevice\> GetPhysicalDevices();`
    >   * `TPhysicalDevice GetBestPhysicalDevice();`
    >* `TInfo`下的`TInfoType`没用，去掉
    >* `TVendor`修改成`TVendorInfo`
    >* 创建`TPhysicalDeviceInfo`类 ~~（完成，待完善）~~
    >* `TQueueFamily`修改成`TQueueFamilyInfo`
    >* 仍需修改，未完成如下：
    >   * ~~`TExtension`~~
    >   * ~~`TLayer`~~
    >   * ~~`TFormat`~~
    >   * 等
    >
* 2022/3/20 设计架构

    >* 完成如下：
    >
    >   * `TExtension`转成`TExtensionInfo`
    >   * `TLayer`转成`TLayerInfos`
    >   * `TFormat`转成`TFormatInfo`
    >   * `TMemoryHeap`转成`TMemoryHeapInfo`
    >   * `TMemoryType`转成`TMemoryTypeInfo`
    >   * 完成`TPhysicalDeviceInfo`与`TPhysicalDevice`相结合
    >* `TPhysicalDeviceProxy::Instance(this->instance)`的调用转移到了`TInstance`的构造函数中,`TInstance`中增加了`TPhysicalDevice`数组，接下来的所有对于`TInstance`用法都是指针连接(防止拷贝浪费内存)
    >
    > * 如下函数从`TEngine`转移到了`Tinstance`中,除了`GetPhysicalDevice()`函数如下函数将会从`TPhysicalDeviceProxy`中移除
    >   * `uint32_t GetPhysicalDeviceCount();`
    >   * `TPhysicalDevice GetPhysicalDevice(uint32_t index);`
    >   * `std::vector<TPhysicalDevice> GetPhysicalDevices();`
    >   * `TPhysicalDevice GetBestPhysicalDevice();`
    >

* 2022/3/21 设计架构

    >* 完成如下：
    >   * `TPhysicalDevice`中增加`std::map<TQueueFamilyInfo, uint32_t *> availableQueueCountMap`，用于存储当前QueueFamily还有多少队列可用，用于DeviceQueue的创建，每成功创建一个DeviceQueue，相应的可用队列数减一，每销毁一个Queue相应的队列数加一。
    >   * `TPhysicalDevice::availableQueueCountMap`在`TPhysicalDevice::InitDeviceQueueParameter`函数中分配动态内存，在`TInstance`的构造函数中释放动态内存
    >   * `TPhysicalDevice`中增加`uint32_t GetAvailableQueueCount(TQueueFamilyInfo& queueFamily)`函数，用于获取可用队列个数，如果返回`0`说明没有可用队列，如果大于`0`说明存在可用队列
    >   * 将TPhysicalDevice如下
    >
    >        ```CXX
    >        TQueueFamilyInfo* bestGraphysicsQueueFamily = nullptr;
    >        TQueueFamilyInfo* bestComputeQueueFamily = nullptr;
    >        TQueueFamilyInfo* bestTransferQueueFamily = nullptr;
    >        TQueueFamilyInfo* bestSparseQueueFamily = nullptr;
    >        TQueueFamilyInfo* bestProtectedQueueFamily = nullptr;
    >       ```
    >
    >       修改成索引。指针指向成员vector变量是临时地址，会出现Bug
    >
    >       ```CXX
    >        uint32_t bestGraphysicsQueueFamilyIndex = UINT32_MAX;
    >        uint32_t bestComputeQueueFamilyIndex = UINT32_MAX;
    >        uint32_t bestTransferQueueFamilyIndex = UINT32_MAX;
    >        uint32_t bestSparseQueueFamilyIndex = UINT32_MAX;
    >        uint32_t bestProtectedQueueFamilyIndex = UINT32_MAX;
    >       ```
    >
    >       并修改相关函数 `TPhysicalDevice::GetBestXXX()`对于这类函数，如果Turbo找到了性能最好的队列族将会正常返回，如果没找到，将会返回一个全`0`，索引为`UINT32_MAX`的无效`TQueueFamilyInfo`
    >   * 重构`TDevice`、`TDeviceQueue`、`TDeviceCreateInfo`、`TDeviceQueueCreateInfo`：
    >     * 将`std::vector<TDeviceQueueCreateInfo> deviceQueueCreateInfos`从`TDeviceCreateInfo`中移除
    >     * 移除`TDeviceQueueCreateInfo::count`默认为 `1`
    >

* 2022/3/22 设计架构

    >* `TResult`增加`RESOURCE_STARVATION`，当发生可用资源匮乏，资源分配失败时会返回该结果
    >* `TPhysicalDevice`中增加`uint32_t Turbo::Core::TPhysicalDevice::GetAvailableQueueCount(uint32_t queueFamilyIndex)`函数，用于快速获取特定队列族的可用队列数
    >* `TPhysicalDevice`中增加`void Turbo::Core::TPhysicalDevice::AvailableQueueCountMinusOneByQueueFamily(uint32_t queueFamilyIndex)`私有函数，`TDeviceQueue`构造函数中使用，用于将对应可用队列族数量减一
    >* `TPhysicalDevice`中增加`void Turbo::Core::TPhysicalDevice::ResetQueueCountMap()`私有函数，`TDevice`析构构函数中使用，用于将之前可用的队列数量还原。
    >* `TDeviceQueue`基本完成，还要接下来的测试。

* 2022/3/29 设计架构
    >
    >* 增加 `TPool`
    >* 增加 `TCommandBufferPool` 继承自 `TPool`
    >* 未增加需增加 `TCommandBuffer` 继承自 `TObject`

* 2022/3/30 设计架构
    >
    >* `TDeviceQueue`增加 `VkQueue GetVkQueue()`函数
    >* `TDeviceQueue`增加 `TDevice *device`成员变量
    >* `TDeviceQueue`增加 `TDevice *GetDevice()`函数
    >* 修改`TPool`实现至头文件 `TCommandBufferPool`编译通过

* 2022/3/31 设计架构
    >
    >* `TDevice`增加 ~~`void Create()`~~ `InternalCreate()`函数(参考2022/4/1)，<font color=yellow> 未完待续 </font>
    >* `TDevice`增加 ~~`void Destroy()`~~ `InternalDestroy()`函数(参考2022/4/1)，<font color=yellow> 未完待续 </font>
    >* `TDevice`增加 `std::vector<TQueueFamilyInfo> GetDeviceQueueFamilyInfos()`函数
    >* 增加`TCommandBuffer`类，<font color=green> 基本完完成 </font>
    >* 修改`TPool::Allocate()`函数到`template <typename... Args> T *Allocate(Args &&...args)`

* 2022/4/1 设计架构
    >
    >* `TCommandBufferPool`增加 `VkCommandPool GetVkCommandPool()`函数
    >* `TCommandBuffer`构造函数实现完成
    >* 增加`TVulkanHandle`类开始重构Vulkan句柄封装类，首先是`TInstance`
    >* `TVulkanHandle`增加`virtual void InternalDestroy() = 0`函数
    >* `TVulkanHandle`增加`virtual void InternalCreate() = 0`函数
    >* `TVulkanHandle`增加`virtual void InternalRebuild()`函数
        >   * `TVulkanHandle::InternalRebuild()`内部会依次调用`TVulkanHandle::InternalDestroy()`和`TVulkanHandle::InternalCreate()`

* 2022/4/2 设计架构
    >
    >* `TPhysicalDevice`设计不合理，将值存储TPhysicalDevice改成指针存储。继承自`TVulkanHandle`。修改`TInstance`、`TPhysicalDevice`和`TPhysicalDeviceProxy`之间的关系，遗弃`TPhysicalDeviceProxy`<font color=yellow> 未完待续 </font>
     >* 初步完成`TInstance`创建`TPhysicalDevice`，使用`TPhysicalDevice::InternalRebuild()`很方便的创建，遗弃`TPhysicalDeviceProxy`
     >* `TPhysicalDeviceProxy`的功能转移到了`TPhysicalDevice`中
     >
* 2022/4/3 设计架构
    >
    >* `TInstance`和`TPhysicalDevice`基于数据驱动，基本重构完成，遗弃`TInstanceCreateInfo`和`VkAllocationCallbacks`,Turbo现在完全使用内置内存分配器`TVulkanAllcator`

* 2022/4/4 设计架构
    >
    >* 优化了`TInstance`和`TPhysicalDevice`的重构
    >* `TDevice`基本重构完成，遗弃了`TDeviceCreateInfo`和`VkAllocationCallbacks`,Turbo现在完全使用内置内存分配器`TVulkanAllcator`
    >* `TDeviceQueue`重构进行中

* 2022/4/5 设计架构
    >
    >* `TPhysicalDevice`中增加`void AvailableQueueCountPlussOneByQueueFamilyIndex(uint32_t queueFamilyIndex)`函数
    >* `TDeviceQueue`基于数据驱动，基本重构完成，遗弃了`TDeviceQueueCreateInfo`和`VkAllocationCallbacks`,Turbo现在完全使用内置内存分配器`TVulkanAllcator`
    >* `TCommandBufferPool`基于数据驱动，基本重构完成
    >* `TCommandBuffer`基于数据驱动，基本重构完成
    >* `Turbo`的`TVulkanHandle`派生类，现在拥有了内存自动回收能力，在`delete`掉某些`TVulkanHandle`派生类对现时，会自动回收与其有关的子对象（前提是所有对象均分布在堆上）。<font color=yellow> 也许Turbo不应该回收与其有关的对象内存，对于这部分内存回收应该由用户承担（除非是`Turbo`对象内部`new`出的内存，需要`Turbo`自己管理，Turboo只会负责回收`Vulkan`相关对象内存）。</font>

* 2022/4/6 设计架构
    >
    >* `TVulkanHandle`中增加（不允许拷贝或赋值）
    >
    >```CXX
    >   TVulkanHandle(TVulkanHandle const &) = delete;
    >   TVulkanHandle(TVulkanHandle &&) = delete;
    >   TVulkanHandle &operator=(TVulkanHandle const &) = delete;
    >   TVulkanHandle &operator=(TVulkanHandle &&) = delete;
    >```
    >
    >* 增加`TVmaAllocator`类，用于VulkanMemoryAllocator(VMA)内存分配 <font color=yellow>~~未完待续~~</font>
    >* `TDevice`中增加对`TVmaAllocator`对象的创建和销毁

* 2022/4/7 设计架构
    >
    >* `TVmaAllocator`类，初步完成
    >* `TBuffer`类，初步完成
    >* 遗弃`TDeviceMemory`类
    >* 遗弃`TBufferCreateInfo`类
    >* 遗弃`TImageCreateInfo`类
    >* 遗弃`TDeviceMemoryAllocateInfo`类
    >* `TImage`类，初步完成
    >* 遗弃`TImageViewCreateInfo`类
    >* `TImageView`类，初步完成

* 2022/4/8 设计架构
    >
    >* 遗弃原先自定义创建`TDeviceQueue`类对象，现在该类对象由`TDevice`统一管理，用户只需要调用如下函数即可获得
    >
    >```CXX
    > TDeviceQueue* GetBestGraphicsQueue();
    > TDeviceQueue* GetBestComputeQueue();
    > TDeviceQueue* GetBestTransferQueue();
    > TDeviceQueue* GetBestSparseBindingQueue();
    > TDeviceQueue* GetBestProtectedQueue();
    >```
    >
    >* 部分重构`TDevice`用于管理`TDeviceQueue`

* 2022/4/9 设计架构
    >
    >* 创建`TShader`类
    >* 原本想在`TShader`类中使用`glslang`来动态编译字符串`shader`到`spirv`的，但是`llvm-mingw32`编译完的库，无论如何都找不到`glslang`的库函数，有可能是`CMakeLists.txt`库配置有问题，暂时先去除该动态编译功能~~（也许该功能不应位于`Core`层面,或许应该有个`Shader编译器`之类的）~~
    >* `TShader.h`中提供`std::vector<char> ReadSpirVFile(const std::string &file)`函数用于读取`spirv`文件

* 2022/4/10 设计架构
    >
    >* 解决`glslang`库找不到的问题，在`Turbo`根目录下创建`thirdpart`文件夹，用于存放第三方库文件项目
    >

* 2022/4/11 设计架构
    >
    >* 添加`SPIRV-Cross`库用于实现反射`Shader`描述符信息
    >* 添加`TDescriptor`类，于`TDescriptor.h`中，用于描述`VkDescriptorSetLayoutBinding`
    >* 添加`TOutput`和`TInput`类，于`TShader.h`中，用于描述`Shader`的`layout (location=n) in/out [type](vec3...) name;`
    >* <font color=yellow> 未完待续 </font>

* 2022/4/12 设计架构
    >
    >* `TOutput`和`TInput`类,没有必要定义两份,修改成`TInterface`
    >* `TShader`增加`virtual void InternalParseSpirV()`成员变量,用于内部构造解析`SpirV`用于`Vulkan`相应创建数据使用<font color=yellow> 未完待续 </font>
    >* `TShader`增加`std::vector<TInterface> outputs`成员变量,表示`layout (location=n) in [type](vec3...) name;`
    >* `TShader`增加`std::vector<TInterface> inputs`成员变量,表示`layout (location=n) out [type](vec3...) name;`
    >* `TDescriptor.h`中增加`TShaderDataType`枚举，用于标明`Shader`中的数据类型
    >* `TInterface`中增加如下：
    >
    >   ```CXX
    >   uint32_t location;//(location=n)
    >   TShaderDataType dataType;
    >   uint32_t size;//vec2 vec3 vec4
    >   ```
    >
    >* `TShader`中`virtual void InternalParseSpirV()`完成`spirv_cross::SPIRType`到`VkDescriptorType`的映射
    >* <font color=yellow> 未完待续 </font>

* 2022/4/13 设计架构
  >
  >* 解决了`TShader`中使用`spirv-cross`运行时的崩溃
  >* `TShader`<font color=yellow> 未完待续 </font>

* 2022/4/14 设计架构
  >
  >* 继续`TShader`中使用`spirv-cross`对于`Shader`的解析，在`TShader`中应提供类似如下成员变量：
  >
  > ```CXX
  >//根据TDescriptorType中的定义声明不同类型的描述符成员变量
  >std::vector<TUniformBufferDescriptor> uniforBuffers;
  >std::vector<TSamplerDescriptor> samplers;
  >std::vector<TImageSamplerDescriptor> imageSamplers;
  >...//等等描述符类型
  > ```
  >
  > 这需要`Turbo`中声明`TUniformBufferDescriptor`等类。

* 2022/4/15 设计架构
  >
  >* 解决了`TDescriptor.h`中增加了`TUniformBufferDescriptor`和`TStructMember`类
  >
  >   * `TUniformBufferDescriptor`中包含`TStructMember`类
  >   * `TUniformBufferDescriptor`用于表示`UniformBuffer`描述符,
  >   * `TStructMember`用于表示`TUniformBufferDescriptor`中的数据结构

* 2022/4/16 设计架构
  >
  >* 继续`TUniformBufferDescriptor`

* 2022/4/17 设计架构
  >
  >* `TShader.cpp`中增加`Turbo::Core::TShaderDataType SpirvCrossSPIRTypeBaseTypeToTShaderDataType(spirv_cross::SPIRType::BaseType baseType)`函数，将`spirv_cross::SPIRType::BaseType`类型转换成`Turbo::Core::TShaderDataType`类型
  >* `TShader.cpp`中对`UniformBuffer`的解析基本完成
  >* `TShader.cpp`中对`TInterface`将改成继承自`Turbo::Core::TStructMember`

* 2022/4/18 设计架构
  >
  >* `TShader.cpp`中对`TInterface`的`input`和`output`的解析基本完成
  >* `Turbo::Core::TDescriptor`代表`Vulkan`的`VkDescriptorSetLayoutBinding`
  >* `Turbo::Core::TUniformBufferDescriptor`代表`Vulkan`的`VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER`
  >* 需要创建`TDescriptorSet`类继承自`Turbo::Core::TVulkanHandle`用于涵盖多个`Turbo::Core::TDescriptor`并创建`VkDescriptorSetLayout`（考虑是否创建`VkDescriptorSet`）

* 2022/4/19 设计架构
  >
  >* 创建`TDescriptorSet`类，构造函数为`Turbo::Core::TDescriptorSet::TDescriptorSet(TShader *shader, std::vector<TDescriptor *> &descriptors)`，其中的`std::vector<TDescriptor *> &descriptors`需要在外部`new`完传进来，该任务属于`Turbo`范畴（处理好`TDescriptorSet`和`TDescriptor`之间的联系）
  >
  >* 对于`throw Turbo::Core::TException(TResult，std::string);`对于`std::string`变量，`Turbo`没有使用上，需补上

* 2022/4/20 设计架构
  >
  >* 开始将`std::vector<T[XXX]Descriptor *>`向`std::vector<TDescriptorSet *> descriptorSets`中写入，`TShader`维护`std::vector<T[XXX]Descriptor *>`中的内存，当`TShader`销毁时`std::vector<T[XXX]Descriptor *>`数组中`T[XXX]Descriptor *`所指向的指针也会销毁。<font color=yellow> 未完待续 </font>
  >* ~~`TShader`中增加`void AddDescriptorToDescriptorSets(TDescriptor *descriptor)`内部函数，当每次`new`一个`TDescriptor`之后，会将刚`new`的`TDescriptor`通过`AddDescriptorToDescriptorSets`函数加入`std::vector<TDescriptorSet *> descriptorSets`中<font color=yellow> 未完待续 </font>~~ 遗弃(不符合`Turbo`统一设计思想)

* 2022/4/21 设计架构
  > *目前`TShader`中只实现了`TUniformBufferDescriptor`,将来待实现如下：
  >
  >```CXX
  >TSamplerDescriptor;
  >TCombinedImageSamplerDescriptor;
  >TSampledImageDescriptor;
  >TStorageBufferDescriptor;
  >TInputAttachmentDescriptor;
  >...
  >```
  >
  >* 创建`TPipeline`类，内部完成了`VkPipelineLayout`的创建。<font color=yellow> 未完待续 </font>
  >* `Turbo`中应该提供如下类<font color=yellow> 未完待续 </font>
  >
  > ```CXX
  >TGraphicsPipeline
  >TComputePipeline
  > ```
  >

* 2022/4/22 设计架构
  >
  >* 新增`TViewport`类
  >* 新增`TScissor`类
  >* 新增`TAttachment`类
  >* 新增`TSubpass`类
  >* 新增`TRenderPass`类
  >* 修改了`TPipeline`类，新增成员变量，内部的`TScissor`和`TViewport`目前是按值存的，将来改成按照指针存，节省内存。
  >* 以上只是初步完成，还有Bug需要修缮。<font color=yellow> 未完待续 </font>

* 2022/4/23 设计架构
  >
  >* 修缮了`TRenderPass`中的Bug
  >* `TCommandBuffer`中添加对于`Vulkan`中的`VkCmd[...]`的相关指令声明，未实现，待实现。
  >* `TCommandBuffer`中已实现完成如下：
  >
  >```CXX
  >bool Turbo::Core::TCommandBuffer::Begin()
  >bool Turbo::Core::TCommandBuffer::End()
  >bool Turbo::Core::TCommandBuffer::Reset()
  >```
  >
  >* `TCommandBuffer`中正在实现如下：
  >
  >```CXX
  >void Turbo::Core::TCommandBuffer::BeginRenderPass(TRenderPass *renderPass)
  >void Turbo::Core::TCommandBuffer::EndRenderPass()
  >```
  >
  >* 创建`TFrameBuffer`类
  >* `TCommandBuffer`中添加`TFrameBuffer`成员变量，当用户调用`Turbo::Core::TCommandBuffer::BeginRenderPass()`时会`new`出一个`TFrameBuffer`对象，当用户调用`Turbo::Core::TCommandBuffer::EndRenderPass()`时会`delete`销毁该对象。所以`BeginRenderPass()`和`EndRenderPass()`两个函数必须配合使用

* 2022/4/24 设计架构
  >
  >* 将`TFrameBuffer`修改成`TFramebuffer`
  >* `TShader`,`TDescriptor`,`TDescriptorSet`,`TPipeline`,`TSubpass`,`TAttachemt`,`TRenderPass`，`TFramebuffer`之间设计不合理，需要重构。尝试是否能够按照`Vulkan`标准规范设计

* 2022/4/25 设计架构
  >
  >* 将原先`TShader`中的`TDescriptorSet`改成了`TDescriptorSetLayout`
  >* 创建`TDescriptorSet`用于表示`VkDescriptorSet`，现在就不会出现`TDescriptorSetLayout`和`TDescriptorSet`命名歧义了
  >* 遗弃老的`TPipeline`，`TSubpass`，`TAttachemt`，`TRenderPass`，`TFramebuffer`相关设计，回归到`Vulkan`标准规范设计思路
  >* `TPipeline`,`TSubpass`,`TAttachemt`,`TRenderPass`，`TFramebuffer`重构中
  >* `TAttachemt`、`TSubpass`重构完成
  >* `TSubpass`新增`TAttachmentReference`类
  >* 创建`TDescriptorPool`、`TDescriptorSize`类

* 2022/4/26 设计架构
  >
  >* `TSubpass`移除`TAttachmentReference`类，用起来不方便
  >* `TPipeline`,`TSubpass`,`TAttachemt`,`TRenderPass`，`TFramebuffer`基本重构完成，但还不完善。需进一步完善
  >* `TCommandBuffer`实现基础命令，还不完善。需进一步完善
  >* `TDescriptorSet::BindData(...)`仅用于测试，须修改完善。
  >* `TBuffer`新增`void *Map()`和`void Unmap();`函数，用于映射内存
  >* 新增`TFence`类
  >* `TFence`中新增`TResult Wait()`如果超时将返回`TResult::TIMEOUT`否则返回`TResult::SUCCESS`
  >* `TFence`中新增`void WaitUntil()`将会一直等待，直到指令运行结束
  >* 新增`TSemaphore`类
  >* `TDeviceQueue`中新增`bool Submit(std::vector<TSemaphore *> &waitSemaphores, std::vector<TSemaphore *> &signalSemaphores, TCommandBuffer *commandBuffer, TFence *fence)`函数，用于提交指令。

* 2022/4/27 设计架构
  >
  >* 发布至`GitHub`和`Gitee`上
  >* 提炼`issue`

* 2022/4/28 设计架构

  >* `TBuffer`中增加`typedef enum TBufferUsageBits`枚举，与`Vulkan`标准一致
  >* `TBuffer`中增加`typedef VkFlags TBufferUsages`声明,用于声明`TBuffer`的`uage`，与`Vulkan`标准一致
  >* `Turbo.h`中增加`typedef VkDeviceSize TDeviceSize`用于`TBuffer`的构造函数中，与`Vulkan`标准一致
  >* `TImage`中增加`typedef enum TImageType`枚举，用于声明`TImage`的`type`,与`Vulkan`标准一致
  >* `TImage`中增加`typedef enum TSampleCountBits`枚举,与`Vulkan`标准一致
  >* `TImage`中增加`typedef VkFlags TSampleCounts`声明,用于`TImage`的构造函数中,与`Vulkan`标准一致
  >* `TImage`中增加`typedef enum TImageTiling`枚举,用于`TImage`的构造函数中,与`Vulkan`标准一致
  >* `TImage`中增加`typedef enum TImageUsageBits`枚举,与`Vulkan`标准一致
  >* `TImage`中增加`typedef VkFlags TImageUsages`声明,用于`TImage`的构造函数中,与`Vulkan`标准一致
  >* `TImage`中增加`typedef enum TImageLayout`枚举,用于`TImage`的构造函数中,与`Vulkan`标准一致
  >* `TImageView`中增加`typedef enum TImageViewType`枚举,用于`TImageView`的构造函数中,与`Vulkan`标准一致
  >* `TImageView`中增加`typedef enum TImageAspectBits`枚举,与`Vulkan`标准一致
  >* `TImageView`中增加`typedef VkFlags TImageAspects`声明,用于`TImageView`的构造函数中,与`Vulkan`标准一致
  >* `TAttachment`中增加`typedef enum TLoadOp`声明,用于`TAttachment`的构造函数中,与`Vulkan`标准一致
  >* `TAttachment`中增加`typedef enum TStoreOp`声明,用于`TAttachment`的构造函数中,与`Vulkan`标准一致
  >

* 2022/4/29 设计架构
  >
  >* `TAttachment`中重命名`typedef enum TStoreOp`声明,成`typedef enum class TStoreOp`,优化枚举内命名
  >* `TAttachment`中重命名`typedef enum TLoadOp`声明,成`typedef enum class TLoadOp`,优化枚举内命名
  >* `TImageLayout`中重命名`typedef enum TImageLayout`声明,成`typedef enum class TImageLayout`,优化枚举内命名
  >* `TFormatInfo`进一步设计优化，完善了如下:
  >   * `typedef enum TFormatContentType`、
  >   * `typedef enum class TFormatDataType`、
  >   * `typedef enum class TFormatCompression`、
  >   * `typedef enum class TFormatReduceFactor`
  >* `TFormatInfo`实现了`uint32_t GetTexelBlockSize()`函数，用于返回该格式下数据占用的字节数
  >* `TAttachment`构造函数中`VKFormat`替换成了`TFormatInfo`
  >* `TImage`增加`TSampleCountBits GetSampleCountBits()`函数，用于获取采样数
  >* `VkVertexInputBindingDescription`和`VkVertexInputAttributeDescription`基本描述框图`TurboDesign.drawwio`整理完成

* 2022/4/30 设计架构
  >
  >* `TPipeline.h`中增加`TVertexAttribute`，`TVertexBinding`类，分别用于表示`VkVertexInputAttributeDescription`和`VkVertexInputBindingDescription`
  >* `TPipeline.h`中增加`typedef enum class TVertexRate`类，用于表示`VkVertexInputRate`
  >* `TPipeline.h`中剔除`VkVertexInputBindingDescription`和`VkVertexInputAttributeDescription`的使用，改为使用`TVertexAttribute`，`TVertexBinding`

* 2022/5/1 设计架构
  >
  >* `TFormatInfo.h`中,构造函数声明去掉`explicit`，使得可直接使用`TFormatType`进行隐示类型转换到`TFormatInfo`
  >* `TPipeline.h`中增加`typedef enum class TTopologyType`用于对应`VkPrimitiveTopology`
  >* `TPipeline.h`中对于`VkPrimitiveTopology`替换成使用`TTopologyType`
  >* `TPipeline.h`中增加`typedef enum class TPolygonMode`用于对应`VkPolygonMode`
  >* `TPipeline.h`中对于`VkPolygonMode`替换成使用`TPolygonMode`
  >* `TPipeline.h`中增加`typedef enum TCullModeBits`用于对应`VkCullModeFlagBits`
  >* `TPipeline.h`中增加`typedef VkFlags TCullModes`用于对应`VkCullModeFlags`
  >* `TPipeline.h`中对于`VkCullModeFlags`替换成使用`TCullModes`
  >* `TPipeline.h`中增加`typedef enum class TFrontFace`用于对应`VkFrontFace`
  >* `TPipeline.h`中对于`VkFrontFace`替换成使用`TFrontFace`
  >* 创建`TGraphicsPipeline`继承自`TPipeline`,将原先`TPipeline`中有关图形管线的内容搬到了`TGraphicsPipeline`中

* 2022/5/2 设计架构
  >
  >* `TGraphicsPipeline`中增加`bool multisampleEnable`和`TSampleCountBits sample`的成员变量，提供对`Multisample`特性的支持
  >* `TCommandBuffer`中增加`TRenderPass *currentRenderPass`和`TPipeline *currentPipeline`的成员变量，提供对当前使用的`RenderPass`和`Pipeline`的引用

* 2022/5/3 设计架构
  >
  >* `TCommandBuffer`中实现了`void Turbo::Core::TCommandBuffer::NextSubpass()`的成员函数，提供对`vkCmdNextSubpass(...)`特性的支持
  >* `Turbo`核心现已支持多`Subpass`渲染
  >* 将`TRenderPass`的`TPipelineType`转移到了`TSubpass`中，符合`Vulkan`标准
  >* `TPipeline.h`中新增`typedef enum TPipelineStageBits`和`typedef VkFlags TPipelineStages`，对应`Vulkan`的`VkPipelineStageFlagBits`和`VkPipelineStageFlags`
  >* `Turbo.h`中新增`typedef enum TAccessBits`和`typedef VkFlags TAccess`，对应`Vulkan`的`VkAccessFlagBits`和`VkAccessFlags`
  >* 新增`TBarrier.h`和`TBarrier.cpp`，用于实现`VkMemoryBarrier`、`VkBufferMemoryBarrier`、`VkImageMemoryBarrier`<font color=yellow> 未完待续 </font>

* 2022/5/4 设计架构
  >
  >* `TBarrier.h`中增加了`TBarrier`、`TMemoryBarrier`、`TBufferMemoryBarrier`、`TImageMemoryBarrier`的成员函数，提供对`VkMemoryBarrier`、`VkBufferMemoryBarrier`、`VkImageMemoryBarrier`特性的支持
  >* `TImageView`中增加了获取成员变量的方法
  >* `TCommandBuffer`中增加并实现如下函数
  >
  >```CXX
  >void PipelineBarrier(TPipelineStages srcStages, TPipelineStages dstStages, std::vector<TMemoryBarrier> &memoryBarriers, std::vector<TBufferMemoryBarrier> &bufferBarriers, std::vector<TImageMemoryBarrier> &imageBarriers);
  > void PipelineMemoryBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TMemoryBarrier &memoryBarrier);
  > void PipelineBufferBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TBufferMemoryBarrier &bufferBarrier);
  > void PipelineImageBarrier(TPipelineStages srcStages, TPipelineStages dstStages, TImageMemoryBarrier &imageBarrier);
  >void TransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess, TImageLayout oldLayout, TImageLayout newLayout, TImage *image, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
  > void TransformImageLayout(TPipelineStages srcStages, TPipelineStages dstStages, TAccess srcAccess, TAccess dstAccess,TImageLayout oldLayout, TImageLayout newLayout, TImageView *imageView);
  >```
  >
  
* 2022/5/5 设计架构
  >
  >* `TCommandBuffer`中增加并实现如下
  >
  >```CXX
  >void FillBuffer(TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, uint32_t data = 0);
  >void FillBuffer(TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE, float data = 0.0f);
  >void UpdateBuffer(TBuffer *buffer, TDeviceSize offset, TDeviceSize size, const void *data);
  >void CopyBuffer(TBuffer *srcBuffer, TBuffer *dstBuffer, TDeviceSize srcOffset, TDeviceSize dstOffset, TDeviceSize size);
  >void ClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
  > void ClearColorImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, TImageAspects aspects);
  > void ClearColorImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0);
  > void ClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
  > void ClearDepthStencilImage(TImage *image, TImageLayout layout, float depth, uint32_t stencil, TImageAspects aspects);
  > void ClearDepthStencilImage(TImageView *imageView, TImageLayout layout, float depth = 0, uint32_t stencil = 0);
  > void ClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);
  > void ClearImage(TImage *image, TImageLayout layout, float r, float g, float b, float a, float depth, uint32_t stencil, TImageAspects aspects);
  > void ClearImage(TImageView *imageView, TImageLayout layout, float r = 0, float g = 0, float b = 0, float a = 0, float depth = 0, uint32_t stencil = 0);
  > ```
  >
  >* `TDeviceQueue`增加`void WaitIdle()`成员函数
  >* `TDevice`增加`void WaitIdle()`成员函数
  >* `TImage`增加`TImageUsages GetUsages()`、`uint32_t GetMipLevels()`、`uint32_t GetArrayLayers();` 成员函数
  >* `TFormatInfo.h`中将`typedef enum TFormatDataType`修改成`typedef enum TFormatDataTypeBits`、增加`typedef VkFlags TFormatDataTypes`声明
  >* `TFormatInfo`中增加`TFormatDataTypes GetFormatDataType()`成员函数，用于获取格式所对应的的数据类型（主要用于`ClearColor`中）

* 2022/5/6 设计架构
  >
  >* `TFormatInfo`增加`typedef enum TFormatFeatureBits`和`typedef VkFlags TFormatFeatures`用于对应`VkFormatFeatureFlagBits`和`VkFormatFeatureFlags`
  >* `TFormatInfo`增加如下函数:
  >
  >```CXX
  > TFormatFeatures GetlLinearFeatures(TPhysicalDevice *physicalDevice);
  > TFormatFeatures GetOptimalFeatures(TPhysicalDevice *physicalDevice);
  > TFormatFeatures GetlBufferFeatures(TPhysicalDevice *physicalDevice);
  >```
  >
  > 用于获取`VkFormatProperties`对应的数据
  >
  >* 解决了`TCommandBuffer::BeginRenderPass(...)`中`ClearColor`不生效的问题（考虑`ClearColor`的颜色声明放到`TAttachment`中，或者`BeginRenderPass(...)`提供颜色设置接口）
  >* `TCommandBuffer`中实现了如下指令函数：
  >
  >```CXX
  > void CopyBufferToImage(TBuffer *srcBuffer, TImage *dstImage, TImageLayout layout, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
  > void CopyImageToBuffer(TImage *srcImage, TImageLayout layout, TBuffer *dstBuffer, TDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight, TImageAspects aspects, uint32_t mipLevel, uint32_t baseArrayLayer, uint32_t layerCount, int32_t imageOffsetX, int32_t imageOffsetY, int32_t imageOffsetZ, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);
  > void CopyImage(TImage *srcImage, TImageLayout srcLayout, TImage *dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcImageOffsetX, int32_t srcImageOffsetY, int32_t srcImageOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstImageOffsetX, int32_t dstImageOffsetY, int32_t dstImageOffsetZ, uint32_t width, uint32_t height, uint32_t depth);
  >```
  >
  >* `Turbo.h`中增加如下声明:
  >
  >```CXX
  > #if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
  > #define TURBO_PLATFORM_WINDOWS
  > #elif defined(__APPLE__)
  > #define TURBO_PLATFORM_APPLE
  > #elif defined(ANDROID) || defined(__ANDROID__)
  > #define TURBO_PLATFORM_ANDROID
  > #elif defined(__linux) || defined(__linux__)
  > #define TURBO_PLATFORM_LINUX
  > #elif defined(__unix) || defined(__unix__)
  > #define TURBO_PLATFORM_UNIX
  > #else
  > #define TURBO_PLATFORM_UNDEFINED
  > #endif
  >```
  >
  >* `TDescriptor.h`中的`typedef enum class TShaderDataType`内容重命名。声明有冲突
  >* `Turbo`中新增`Turbo::Extension`命名空间
  >* 开始修缮`TSurface.h`将其加入`Turbo::Extension`命名空间中<font color=yellow> 未完待续 </font>

* 2022/5/7 设计架构
  >
  >* `TSurface.h`中`Turbo::Core::TImageUsageFlags TSurface::GetSupportedUsageFlags()`修改成`Turbo::Core::TImageUsages GetSupportedUsages()`
  >* `TSurface.h`中增加如下：
  >
  >```CXX
  > typedef enum TSurfaceTransformBits;
  > typedef VkFlags TSurfaceTransforms;
  > typedef enum TCompositeAlphaBits;
  > typedef VkFlags TCompositeAlphas;
  > typedef enum class TPresentMode;
  > uint32_t GetCurrentWidth();
  > uint32_t GetCurrentHeight();
  > uint32_t GetMinWidth();
  > uint32_t GetMinHeight();
  > uint32_t GetMaxWidth();
  > uint32_t GetMaxHeight();
  >```
  >
  >* 将`TColorSpace`类和`TSurfaceFormat`转移到`TSurface.h`中，并将`TColorSpace.h/.cpp`和`TSurfaceFormat.h/.cpp`移除
  >
  >* `TSurface.h`修缮完成
  >* 开始修缮`TSwapchain.h`将其加入`Turbo::Extension`命名空间中<font color=yellow> 未完待续 </font>
  >* `TImage.h`中加入`void *Map()`和`void Unmap()`成员函数,用于映射`host`可访问内存
  >* `TImage.h`中加入`Turbo::Extension::TSwapchain`的声明和`TImage()`默认无参构造函数，用于`Turbo::Extension::TSwapchain`构建`TSwaphainImage`，其为`TImage`的友元类
  >* `main.cpp`中将`GPU`图片拷贝到`CPU`的纯`Vulkan`代码改成`Turbo`规范
  >* 有些窗口库创建完窗口直接返回`VkSurfaceKHR`,所以`TSwapchain`需要对外提供一个接口构造函数`TSwapchain(TDevice* device, VkSurfaceKHR)`用于接收外部创建好的`VkSurfaceKHR`<font color=yellow> 未完待续 </font>
  >

* 2022/5/8 设计架构
  >
  >* 之前 `TSwapchain`对外部提供的`VkSurfaceKHR`的接口改成由`TSurface`提供，`TSwapchain`统一使用`TSurface`
  >* `TSwapchain`修缮完毕
  >* `TSwapchain`中增加`VkSwapchainKHR GetVkSwapchainKHR()`成员函数
  >* `TResult`新增`TResult::NOT_READY`和`TResult::SUBOPTIMAL`用于`Turbo::Extension::TSwapchain::AcquireNextImage(...)`的返回结果
  >* `./thirdparty`新增`SDL2`库用于创建窗口
  >* `TDevice`的`InternalCreate()`中有`Bug`，已修复
  >* `TCommandBuffer`的`BeginRenderPass()`中有`Bug`，已修复
  >* `TSemaphore`中的`VkPipelineStageFlags`改成`TPipelineStages`，统一`Turbo`标准
  >* `TDeviceQueue`中增加`TResult Present(Turbo::Extension::TSwapchain *swapchain, uint32_t imageIndex)`函数用于调用`vkQueuePresentKHR`
  >* `TDeviceQueue`中增加`bool IsSupportSurface(Turbo::Extension::TSurface *surface)`判断该队列是否支持该`TSurface`

* 2022/5/9 设计架构
  >
  >* 进入调整阶段
  >* `TCommandBuffer`中有关`VkCmd...`的成员函数前面加上`Cmd...`的前缀,与`CommandBuffer::Begin()`和`CommandBuffer::End()`等非命令成员函数分开，直接告诉用户哪些会是`VkCmd...`哪些不是
  >* `TFormatInfo`中删除有关`TFormatFeatureFlags`的成员变量，没用

* 2022/5/10 设计架构
  >
  >* `TBuffer`中增加`typedef enum class TIndexType`表明索引缓存的数据类型
  >* `TCommandBuffer`中增加如下函数：
  >
  >```CXX
  > void CmdBindIndexBuffer(TBuffer *buffer, TDeviceSize offset = 0, TIndexType indexType = TIndexType::UINT32);
  > void CmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
  >```
  >
  >用于索引渲染
  >
  >* `TAllocator`平台相关使用`Turbo.h`中定义的宏
  >* 开始整理`TException`的输出信息,和优化`TException`
  >* `TFormatInfo`增加`Get...Features(TDevice* device)`版本成员函数

* 2022/5/11 设计架构
  >
  >* 开始实现纹理特性
  >* `TDescriptor.h`中增加`TCombinedImageSamplerDescriptor`类，用于表示`VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER`
  >* `TShader.h`中增加`std::vector<TCombinedImageSamplerDescriptor *> combinedImageSamplerDescriptors`成员变量
  >* 新增`TSampler.h`中增加`TSampler`类并增加如下枚举定义：
  >
  >```CXX
  >typedef enum class TFilter;//对应VkFilter
  >typedef enum class TMipmapMode;//对应VkSamplerMipmapMode
  >typedef enum class TAddressMode;//对应VkSamplerAddressMode
  >typedef enum class TBorderColor;//对应VkBorderColor
  >```
  >
  >* 新增`TDescriptorSet`中增加`void BindData(uint32_t binding, uint32_t arrayElement, TImageView *imageView, TSampler *sampler)`，仅用于测试
  >* 将原先的`SDL2`窗口库换成`GLFW`窗口库,使用`SDL2`会有`main`入口函数重定向，并且终端的输出信息不显示等很奇怪的问题。
  >* 纹理特性初步完成
  >* `Descriptor`设计有重大逻辑漏洞，需要重构

* 2022/5/12 设计架构
  >
  >* 开始重构`Descriptor`
  >* `TShader`中移除有关`TDescriptorSetLayout`创建相关，相关任务转移到`TPipeline`中进行，`TShader`仅用于创建`VkShaderModule`和收集`Descriptor`数据
  >* `TDescriptorSetLayout`中移除有关`TShader`成员
  >* `TDescriptorSetLayout`中增加`uint32_t GetSet()`成员函数
  >* `TDescriptor`中增加`TShader*`成员变量，用于指代该描述符所属哪个`TShader`
  >* `TShader`中增加如下函数:
  >
  >```CXX
  > const std::vector<TUniformBufferDescriptor *> &GetUniformBufferDescriptors;
  > const std::vector<TCombinedImageSamplerDescriptor *> &GetCombinedImageSamplerDescriptors;
  >```
  >
  >目前`Turbo`只实现了这两个，更多待以后实现
  >
  >* （转移到`TPipelineLayout`中） 目前`TPipeline`中增加了`std::vector<TDescriptorSetLayout *> descriptorSetLayouts`成员变量，用于存储当前管线的描述符集合布局
  >* 新增`TPipelineLayout`类
  >* 新增`TPipeline`中开始使用`TPipelineLayout`
  >* 新增`TPipelineDescriptorSet`类
  >* `TDescriptorPool`适配`TPipelineDescriptorSet`
  >* `TDescriptorSet`中增加`uint32_t GetSet()`成员函数
  >* `TCommandBuffer`中增加`void CmdBindPipelineDescriptorSet(TPipelineDescriptorSet* pipelineDescriptorSet)`成员函数，用于适配`TPipelineDescriptorSet`
  >* `Descriptor`目前初步重构完成

* 2022/5/13 设计架构
  >
  >* `TResult::SUBOPTIMAL`更改成`TResult::MISMATCH`
  >* `TSurface`中有关获取当前大小的函数可以动态获取当前大小了，不需要`delete`之后重新`new`了
  >* `TSwapchain`中增加`TSwapchain(TSwapchain *oldSwapchain)`构造函数，用于重新创建`TSwapchain`

* 2022/5/14 设计架构
  >
  >* `TDescriptor.h`中的`typedef enum class TShaderDataType`更改成`typedef enum class TDescriptorDataType`
  >* `TDescriptor.h`中增加`class TSampledImageDescriptor`和`TSamplerDescriptor`，用于对应`VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE`和`VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER`
  >* `TShader`中增加`std::vector<TSampledImageDescriptor *> sampledImageDescriptors`和`std::vector<TSamplerDescriptor *> samplerDescriptors`
  >* 开始修缮`TDescriptorSet`中的`void BindData(...);`使其规范化,`TPipelineDescriptorSet`同理:
  >
  >```CXX
  > void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TBuffer *> buffers);//for uniform buffer
  > void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<std::pair<TImageView *, TSampler *>> &combinedImageSamplers);//combined image sampler
  > void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TImageView *> &imageViews);//sampled image
  > void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TSampler *> &sampler);//sampler
  >```

* 2022/5/16 设计架构
  >
  >* `TCommandBuffer`实现`CmdBlitImage`等价于`vkCmdBlitImage`
  >* `engine/`下新建`core`文件夹用于存放`Turbo`核心
  >* `engine/core/include/Turbo.h`更改成`TCore.h`
  >* `engine/core/include/Turbo.cpp`更改成`TCore.cpp`
  >* 现在`engine/core`将会输出单独的库文件作为`Turbo`的核心
  >* `TFormatInfo`中的`TFormatFeatures Get...Features(TPhysicalDevice *physicalDevice)`转移到了`TPhysicalDevice`中

* 2022/5/17 设计架构
  >
  >* `TPhysicalDevice`中增加如下函数(用于获取某一特定纹理格式的图片的限制属性)：
  >
  >```CXX
  >TExtent3D GetMaxImageExtent(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >TExtent3D GetMaxImageExtent(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >uint32_t GetMaxImageMipLevels(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >uint32_t GetMaxImageMipLevels(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >uint32_t GetMaxImageArrayLayers(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >uint32_t GetMaxImageArrayLayers(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >TSampleCounts GetSupportImageSampleCounts(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags); 
  >TSampleCounts GetSupportImageSampleCounts(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >TDeviceSize GetMaxImageResourceSize(TFormatType formatType, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >TDeviceSize GetMaxImageResourceSize(TFormatInfo &format, TImageType imageType, TImageTiling tiling, TImageUsages usages, VkImageCreateFlags imageFlags);
  >```
  >
  >* `TCommandBuffer`实现如下函数
  >
  >```CXX
  >void CmdResolveImage(TImage *srcImage, TImageLayout srcLayout, TImage*dstImage, TImageLayout dstLayout, TImageAspects srcAspects, uint32_t srcMipLevel, uint32_t srcBaseArrayLayer, uint32_t srcLayerCount, int32_t srcOffsetX, int32_t srcOffsety, int32_t srcOffsetZ, TImageAspects dstAspects, uint32_t dstMipLevel, uint32_t dstBaseArrayLayer, uint32_t dstLayerCount, int32_t dstOffsetX, int32_t dstOffsety, int32_t dstOffsetZ, uint32_t width, uint32_t height, uint32_t depth);
  >void CmdSetLineWidth(float lineWidth);
  >```
  >
  >* 将`TCommandBuffer`中的`void CmdBindPipelineDescriptorSet(uint32_t firstSet, TPipelineDescriptorSet *pipelineDescriptorSet);`修改成`void CmdBindPipelineDescriptorSet(TPipelineDescriptorSet *pipelineDescriptorSet);`
  >

* 2022/5/18 设计架构
  >
  >* `TDescriptor.h`中增加`class TNaNDescriptor`，用于表示无效占位描述符。
  >* 将`TDeviceQueue.h`中`:TDeviceQueue::Submit(...)`函数中有关`TSemaphore`数组引用改成数组指针
  >* 从`tinygltf`库中拷贝`stb_image.h`, `stb_image_write.h`, `json.hpp` 和 `tiny_gltf.h`到`./engine/include`中

* 2022/5/19 设计架构
  >
  >* `TGraphicsPipeline::InternalCreate()`中增加对于多个`fragment output interface state(片元着色器的输出)`应该对应相同个数的`VkPipelineColorBlendStateCreateInfo::attachmentCount`的适配。(也许这不是最优解，blend目前未想好如何设计)
  >* `TShader`中增加如下函数：
  >
  >```CXX
  >std::vector<TInterface> GetInputs();
  >std::vector<TInterface> GetOutputs();
  >TShaderType GetType();
  >```
  >
  
* 2022/5/20 设计架构
  >
  >* `TRenderPass`中增加`TSubpass GetSubpass(uint32_t subpass)`成员函数
  >* 增加`glm`第三方库

* 2022/5/21 设计架构
  >
  >* 移除原先拷贝的`tinygltf`头文件，将`tinygltf`库加到`./thirdparty`文件夹下
  >* 成功渲染`Suzanne`

* 2022/5/22 设计架构
  >
  >* 将`imgui`库加到`./thirdparty`文件夹下
  >* 将`KTX-Software`库加到`./thirdparty`文件夹下
  >* `TDescriptor`增加`TPushConstantDescriptor`用于描述`Vulkan`的`Push Constant`
  >* `TShader`增加`std::vector<TPushConstantDescriptor *> pushConstantDescriptors`和相应的获取函数。
  >* `TPipelineLayout`和`TPipeline`中适配`TPushConstantDescriptors`
  >* `TUniformBufferDescriptor`和`TPipeline`中增加`uint32_t size`成员属性和相应的获取函数，用于表示数据块大小
  >* `TCommandBuffer`中增加`void CmdPushConstants(...)`成员函数，对应`vkCmdPushConstants(...)`

* 2022/5/23 设计架构
  >
  >* `TBuffer`中增加`void Flush(...)`成员函数，用于刷新数据到`GPU domain`
  >* `TSubpass`中`GetDepthStencilAttachmentReference()`有`Bug`，现已修改完毕

* 2022/5/24 设计架构
  >
  >* `TGraphicsPipeline`中移除`std::vector<TViewport> &viewports`和`std::vector<TScissor> &scissors`成员，由于核心创建的`Pipeline`的`Viewport`和`Scissor`都是动态状态，`Vulkan`会忽略`VkPipelineViewportStateCreateInfo::pViewports`和`VkPipelineViewportStateCreateInfo::pScissors`
  >* `TGraphicsPipeline`中增加`typedef enum class TCompareOp`声明，对应`VkCompareOp`
  >* `TGraphicsPipeline`中增加`typedef enum class TStencilOp`声明，对应`VkStencilOp`
  >* `TGraphicsPipeline`中增加对深度测试和模板测试支持
  >* 调整`TGraphicsPipeline`构造函数的参数顺序，填入默认值

* 2022/5/25 设计架构
  >
  >* 成功渲染`KTX`纹理

* 2022/5/27 设计架构
  >
  >* 成功渲染`KTX`的`Cubemap`纹理
  >* 开始适配几何管线的`Blend`  
  >* `TGraphicsPipeline`中增加`typedef enum class TLogicOp`  声明，对应`VkLogicOp`
  >* `TGraphicsPipeline`中增加`typedef enum class TBlendFactor`  声明，对应`VkBlendFactor`
  >* `TGraphicsPipeline`中增加`typedef enum class TBlendOp`  声明，对应`VkBlendOp`
  >* `TGraphicsPipeline`构造函数中增加混合相关参数

* 2022/5/28 设计架构
  >
  >* 使用`Turbo`核心成功渲染`IMGUI`

* 2022/5/29 设计架构
  >
  >* 开始适配`input attachment`

* 2022/5/30 设计架构
  >
  >* `TDescriptor`中增加`class TInputAttachmentDescriptor` 对应`VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT`
  >* `TShader`中增加`std::vector<TInputAttachmentDescriptor *> inputAttachmentDescriptors;` 用于存`input attachment`，并增加`const std::vector<TInputAttachmentDescriptor *> &GetInputAttachmentDescriptors();`成员函数用于获取相关数据
  >* `TShader`中增加对于`input attachment`的解析
  >* `TPipeline`中增加对于`input attachment`的解析
  >* `TPipelineDescriptorSet::BindData(...)`中增加对于`input attachment`的适配
  >* `TDescriptorSetLayout`中增加`TDescriptorType GetDescriptorType(uint32_t binding)`成员函数,用于获取特性`binding`对应的描述符类型
  >* `input attachment`基本适配完成

* 2022/6/1 设计架构
  >
  >* 开始`FrameGraph`的设计，具体请参看`./docs/TurboDesign.drawio:FrameGraph`章节

* 2022/6/2 设计架构
  >
  >* 设计`FrameGraph`，具体请参看`./docs/TurboDesign.drawio:FrameGraph`章节

* 2022/6/3 设计架构
  >
  >* 设计`FrameGraph`，具体请参看`./docs/TurboDesign.drawio:FrameGraph`章节

* 2022/6/4 设计架构
  >
  >* 设计`FrameGraph`，具体请参看`./docs/TurboDesign.drawio:FrameGraph`章节

* 2022/6/5 设计架构
  >
  >* 设计`FrameGraph`，具体请参看`./docs/TurboDesign.drawio:FrameGraph`章节
  >* 最基本的`FrameGraph`初步设计完成，具体请参看`./docs/TurboDesign.drawio:FrameGraph`章节

* 2022/6/6 设计架构
  >
  >* 开始实现`FrameGraph`，详细设计请参看`./docs/TurboDesign.drawio:FrameGraph`章节
  >* `FrameGraph`，将会是一个独立的模块，不会依赖任何第三方库，甚至是`Turbo`，按照常见的标准容器实现。将会输出名为`TFrameGraph`的静态库文件
  >* `engine`文件夹下创建`/framegraph`文件夹，`FrameGraph`相关代码将会在这里实现
  >* `FrameGraph`将会在`Turbo::FrameGraph`命名空间下实现
  >* 增加`TFrameGraph.h`和`TFrameGraph.cpp`

* 2022/6/7 设计架构
  >
  >* 将第三方库配置成`Submodule`,`glm`库好像有问题，`MVP`矩阵好像不对导致渲染结果有问题，应该不是核心的问题，该问题先忽略，等到`FrameGraph`写完，开始写例子的时候再统一解决，加入`Issue`中
  >* 创建`./Turbo/samples`文件夹，用于存放示例代码。
  >* `FrameGraph`的`void Turbo::FrameGraph::TFrameGraph::Compile()`未实现待实现
  >* `FrameGraph`的`void Turbo::FrameGraph::TFrameGraph::Execute()`未实现待实现
  >* `./Turbo/samples`种增加`HelloTriangle`示例
  >* `./Turbo/samples`种增加`PureHelloTriangle`示例
  >* `./Turbo/samples`种增加`FrameGraph`测试示例

* 2022/6/13 设计架构
  >
  >* 适配`Linux`系统（基于`Deepin`）
  >* `Turbo::Core::TAllocator::Allocate(...)`中增加对于`Linux`的内存分配支持
  >* `TSurface`中增加对于`Linux`的支持：
  >
  >```CXX
  >//for wayland
  >#include <wayland-client.h>
  >#include <vulkan/vulkan_wayland.h>
  >
  >//for xcb
  >#include <xcb/xcb.h>
  >#include <vulkan/vulkan_xcb.h>
  >
  >//for xlib
  >#include <X11/Xlib.h>
  >#include <vulkan/vulkan_xlib.h>
  >  ```
  >
  >  `TSurface`中增加`TSurface(Turbo::Core::TDevice *device, wl_display *display, wl_surface *surface)`构造函数，用于适配`wayland`
  >  `TSurface`中`InternalCreate()`适配`Linux`的`wayland`
  >  `TSurface`中`GetSurfaceSupportQueueFamilys()`适配`Linux`的`wayland`

* 2022/6/15 设计架构
  >
  >* 适配`TSurface`基于`xcb`
  >* `TSurface`中增加`xcb_connection_t *connection`和`xcb_window_t window`成员变量，用于适配`xcb`
  >* `TSurface`中增加适配`xcb`相应的构造函数
  >* `TSurface`的`InternalCreate(...)`中通过`vkCreateXcbSurfaceKHR(...)`适配`xcb`
  >* 适配`TSurface`基于`xlib`
  >* `TSurface`中增加`Display *xlibDpy = nullptr`和`Window xlibWindow`成员变量，用于适配`xlib`
  >* `TSurface`中增加适配`xlib`相应的构造函数
  >* `TSurface`的`InternalCreate(...)`中通过`vkCreateXlibSurfaceKHR(...)`适配`xlib`
  >* `TSwapchain`的`InternalCreate(...)`中会去判断当前给的图片数量是否合法（判断数量是否在[最小值，最大值]之间），但是有些显卡设备返回的最小值为有效数据，最大值为`0`,比如`Intel(R) HD Graphics 6000 (BDW GT3)`集显设备就是出现最小值为`3`，最大值为`0`的情况，由此适配修改`Turbo::Extension::TSurface::GetSurfaceCapabilities()`函数
  >* 适配修改`Turbo::Extension::TSurface::GetSurfaceCapabilities()`函数，对应修改`maxImageCount`
  >* 修改`Turbo`中对于物理设备的计分算法，最好的图形设备没有返回预计的显卡，而返回了一个`CPU`,`void Turbo::Core::TPhysicalDevice::CalculatePerformanceScore()`中对于`CPU`的分值给的太高了，减小

* 2022/6/16 设计架构
  >
  >* 修改`./samples`下的示例，使其适配`Linux`
  >* `./samples`中增加`PureCombinedImageSampler.cpp`例子，用于演示带有纹理图片的采样器

* 2022/6/29 设计架构
  >
  >* `FrameGraph`将会转至`c++20`标准，将会尝试使用`concept`和`metaTemplate`方式编写
  >* `samples`中添加`PureSeparateImageSampler`示例，用于展示分离式纹理采样器的使用
  >* 对于资源的读写将会改用相对路径
  >* `samples`中添加`PureIndexDraw`示例，用于展示索引渲染

* 2022/6/30 设计架构
  >
  >* `TFrameGraph`中的`拷贝构造/赋值`和`移动构造/赋值`的相关函数使用`delete`声明去掉
  >* `TFrameGraph::AdddPass(...)`中对于模板形参`Setup`,`Execute`做出限制，限制为可调用实体，对`Execute`的大小作出限制，最大为`EXECUTE_MAX_LOAD`
  >* `TFrameGraph::TPassAgency(...)`中对于形参和模板形参`Execute`，由于是右值万能引用，所以使用`std::forward(...)`来完美转发
  >* `TFrameGraph::CreatePassNode(...)`中`TAgency*`模板参数修改为`std::unique_ptr<TAgency>&& agency`
  >* `TPassNode`中`TAgency*`相成员修改为`std::unique_ptr<TAgency>`
  >* 去掉`TPassNode`中析构函数的实现，使用编译器默认生成的
  >* `TFrameGraph::AddPass(...)`中增加如下代码,调用初始化入口函数：
  >
  >```CXX
  >std::invoke(setup, builder, pass_agency->GetData())
  >```
  >
  >* 增加`TAgency`的构造/析构函数

* 2022/7/4 设计架构
  >
  >* `FrameGraph`设计参考`Filement`做出的修改意见目前写在各行的注释中（`//*...`），考虑是否采纳，之后需要同步修改`FrameGraph`的设计文档
  >* 将`Filament`的`FrameGraph`结构分析加入`FrameGraph`文档，作为参考，与`c++`新标准的模板元编程和`concept`结合设计

* 2022/7/5 设计架构
  >
  >* 开始解析`Filament 1.9.9`的`FrameGraph`(截止2022/7/5 `filament`的最新版本为`1.24`对于`FrameGraph`做了很多更新，但总体思路没变),请在`./docs/TurboDesign.drawio::FrameGraph`的右侧查看（注：重点过程被标注为橘黄色）

* 2022/7/6 设计架构
  >
  >* 开始解析`Filament`的`FrameGraph::compile()`阶段，请在`./docs/TurboDesign.drawio::FrameGraph`的右侧查看

* 2022/7/7 设计架构
  >
  >* 开始解析`Filament`的`FrameGraph::execute()`阶段，请在`./docs/TurboDesign.drawio::FrameGraph`的右侧查看

* 2022/7/8 设计架构
  >
  >* 解析`Filament`的`FrameGraph::execute()`阶段,`reset()`函数调用位置标错了，修改回来。
  >* 开始解析`Filament`的`FrameGraph::execute()`阶段的`resources.get(data.rt);`，请在`./docs/TurboDesign.drawio::FrameGraph`的右侧查看
  >* 至此`Filament`的`FrameGraph`核心解析完毕，请在`./docs/TurboDesign.drawio::FrameGraph`的右侧查看
  >* `TFrameGraph.hpp`中增加如下,用于表示无效ID，并用该值初始化所有ID：
  >
  >```CXX
  >constexpr uint32_t TURBO_NVALID_ID = std::numeric_limits<uint32_t>::max();
  >```
  >
  >* `TFrameGraph::AddPass(name, setup, execute)`中的`execute`回调中`TResources`使用`const`声明，对应的`TBuilder::Get(...)`等函数需要适配，碰到再改
  >* `TFrameGraph::TBuilder::Create(...)`中的`Virtualizable`模板形参改回`T`
  >* `TFrameGraph::Create(...)`中的`Virtualizable`模板形参改回`T`
  
* 2022/7/9 设计架构
  >
  >* 修缮`TFrameGraph::TBuilder::Create(...)`函数
  >* `TFrameGraph::TResourceAgency(...)`中的`Virtualizable`模板形参改回`T`
  >* `TFrameGraph::TResourceAgency(...)`中增加`std::string name`属性，用于存储资源名称
  >* `TFrameGraph::TResourceAgency`中的`T *resource`改为`T resource`
  >* 新增`TVirtualResourceAgency`类继承自`TAgency`，将`TResourceAgency`改为继承自`T TVirtualResourceAgency`
  >* 将`TFrameGraph::std::vector<TAgency *> *agencys`成员变量改成`TFrameGraph::std::vector<TVirtualResourceAgency *> *agencys`
  >* 将`TNode`中的`name`成员变量移动到`TPassNode`中，`TResourceNoded`的名称位于其资源代理中
  >* 将`TResourceNode`中的`TAgencyID agencyID`成员变量换成`TVirtualResourceAgency*`指向对应的资源代理
  >* 将`TResourceNode`中的`TAgencyID agencyID`成员变量换成`TVirtualResourceAgency*`指向对应的资源代理
  >* 将`TFrameGraph::TBuilder::Create(...)`函数基本修缮完成
  >* 修缮`TFrameGraph::TBuilder::Write(...)`函数
  >* `TFrameGraph`中增加`TResourceNode &GetResourceNode(TResource resource)`函数
  >* `TVirtualResourceAgency`中增加`TVersion version`成员变量，并增加相应的`Set/Get`函数
  >* `TResourceNode`中增加`TVirtualResourceAgency *GetResourceAgency()`函数
  >* 遗弃`TFrameGraph`中的`CloneResourceNode(...)`函数
  >* 遗弃`TFrameGraph`中的`GetResourceAgency(...)`函数
  >* `TFrameGraph`中增加`bool IsValid(TPass pass)`函数
  >* `TFrameGraph::TBuilder::Write(...)`函数基本修缮完成
  >* 修缮`TResources::Get(TResource resource)`函数
  >* `TResourceNode`中增加`TPass writer`成员变量，用与表示资源的写入者

* 2022/7/10 设计架构
  >
  >* `TVirtualResourceAgency`类中增加`TPass firstUser`和`TPass lastUser`用于表示第一次和最后一次使用该资源的使用者
  >* `TVirtualResourceAgency`类中增加如下,用于表示该PassNode运行时需要创建和销毁的资源：
  >
  >```CXX
  >std::vector<TVirtualResourceAgency*> devirtualizes;
  >std::vector<TVirtualResourceAgency*> destroies;
  >```
  >
  >* `TVirtualResourceAgency`增加如下成员函数：
  >
  >```CXX
  >virtual void Create() = 0;
  >virtual void Destroy() = 0;
  >```
  >
  >* `TResourceAgency`增加如下成员函数：
  >
  >```CXX
  >virtual void Create() override;
  >virtual void Destroy() override;
  >```
  >
  >* 增加`class TPassExecutorAgency`类，用于定义`pass`代理的`Executor(...)`的虚函数回调
  >* 修改`class TPassAgency`类，继承自`TPassExecutorAgency`，并实现`Executor(...)`虚函数回调
  >* 将`TPassNode`类中的`std::unique_ptr<TAgency> agency`修改成`std::unique_ptr<TPassExecutorAgency> agency;`
  >* 将`TFrameGraph`类中的`void Execute();成员函数`修改成`void Execute(void *context);`
  >* 修改`TFrameGraph::CreatePassNode(...)`适配新的`TPassExecutorAgency`类
  >* 修改`TPassNode`构造函数，适配新的`TPassExecutorAgency`类
  >* `TFrameGraph`大框架基本上写完了，剩下的就是`Resource`和`PassNode`等特化和引擎提供的特性资源类，比如`PresentNode(PassNode的特化，有PresentData)`和`Texture(纹理资源)，DepthTexture(深度纹理)，ColorTexture(颜色纹理)`等
  >* 接下来需要实现`TFrameGraph`的`Blackboard`特性
  
* 2022/7/11 设计架构
  >
  >* 实现`FrameGraph`的`Blackboard`特性
  >* 实现`TFrameGraph`中增加`TBlackboard`类
  >* `TFrameGraph`中增加`TBlackboard`类成员变量
  >* `TFrameGraph`中增加`TBlackboard &GetBlackboard()`成员函数
  >* ~~需要特化`PresentPassNode`，显示节点一般作为结束节点，但是`FrameGraph`演讲中并没有显示的给出显示阶段往哪个资源身上写入，所以目前显示节点没有写入，也就是没有出度，没有出度在`FrameGraph::Compile()`阶段就会被剔除，这也许会造成一连串的剔除，所以需要专门特化一个`PresentPassNode`或是采用`SideEffect`解决(目前`TFrameGraph`并没有引入`SideEffect`特性)~~
  当前的算法正常并不会剔除`PresentPass`，`FrameGraph`目前的剔除算法首先基于资源引用数进行的剔除，目前显示节点不向任何资源写入，只是读取资源，正常不会发生递归性的一连串剔除
  >* `TFrameGraph`还需要经过测试，目前还没有严格测试

* 2022/7/12 设计架构
  >
  >* 修复Bug:`TFrameGraph::Compile()`中计算资源的创建者和销毁者时，`FrameGraph`的所有资源可能会被全部剔除，造成空图，这会导致资源的创建者和销毁者为无效节点（比如强制`PresentPass`往资源中写入，由于`PresentPass`为结束节点，正常不应该再写入资源，这时该写入的资源没有出度，也就是没有人使用该节点，这时`Compile()`阶段会发生资源剔除，导致一连串的资源剔除，最坏的情况会剔除成空图，这时资源的创建者和销毁者不会被赋值，从而导致创建者和销毁者的节点`id`无效非法）
  >
  >* 有关`TFrameGraph`如何使用和测试，请参考`./samples/FrameGraphSample.cpp`
  >* `TFrameGraph::Execute(...)`中增加对`PassNode`的引用数的判断，如果引用数大于零，说明该节点有使用者，运行节点的`Executor`回调，否则没有使用，不去调用回调。(注意：这将会导致`PresentNode`的运行回调不会被调用，考虑使用`SideEffect`特性强制驱动，如果一个`PassNode`有`SideEffect`特性，说明在节点不会被剔除)
  >* `TPassNode`中增加`bool sideEffect`成员变量
  >* `TFrameGraph::TBuilder`中增加`TBuilder &SideEffect();`成员函数，用于设置`PassNode`的`sideEffect`成员
  >* `TFrameGraph::Execute()`中增加对于`sideEffect`的判断

* 2022/7/13 设计架构
  >
  >* 修改`SideEffect`强制不剔除`PassNode`，应该修改其`refCount`
  >* 移除`TVirtualResourceAgency::SetVersion(...)`函数，通过直接修改成员变量完成
  >* 至此最基础的`FrameGraph`实现完成，接下来将会于`./engine/include`和`./engine/src`中实现更高级的资源和特性(比如`Surface`,`Material`和各种`Resource`等)

* 2022/7/16 设计架构
  >
  >* 修改`./main.cpp`中的绝对路径修改到相对路径
  >* 开始尝试动态加载`Vulkan`库来获取函数
  >* 核心中添加`TVulkanLoader`类用于实现`Vulkan`的`Loader`
  >* 核心中添加`TCore::TResult`中增加`UNIMPLEMENTED`用于表示`引擎未实现先关定义，请实现`

* 2022/7/17 设计架构
  >
  >* `TVulkanLoader`中增加如下私有函数，用于加载`Vulkan API`的函数：
  >
  >```CXX
  >template <TLoaderType type, typename Function>
  >Function LoadAll(void *context, const char*name);
  >```
  >
  >* `TVulkanLoader`中增加如下函数：
  >
  >```CXX
  >void Load(TInstance *instance);
  >void LoadAllInstanceFunctions(TInstance *instance);
  >void LoadAllDeviceFunctions(TInstance *instance);
  >
  >template <typename Function>
  >Function LoadInstanceFunction(TInstance *instance, const char *name);
  >template <typename Function>
  >Function LoadInstanceFunction(VkInstance instance, const char *name);
  >template <typename Function>
  >Function LoadDeviceFunction(TInstance*instance, const char*name);
  >template <typename Function>
  >Function LoadDeviceFunction(VkInstance instance, const char*name);
  >TVersion GetVulkanVersion();
  >```
  >
  >* `TInstance::InternalCreate()`中增加对于`TVulkanLoader`的相关调用，用于加载`Vulkan API`函数
  >* `TInstance::InternalCreate()`中对于`vkCreateInstance`的相关调用，更改成`Turbo::Core::vkCreateInstance`调用，并在之后调用`TVulkanLoader::Instance()->LoadAll(this);`用于获取所有`Vulkan API`函数
  >* `TInstance::IsSupportVulkan()`中对于`vkCreateInstance`的相关调用，更改成使用`TVulkanLoader`调用
  >* `TInstance::GetVulkanInstanceVersion()`中对于`vkEnumerateInstanceVersion`的相关调用，更改成使用`TVulkanLoader`调用

* 2022/7/18 设计架构
  >
  >* `TLayerInfo`中使用`TVulkanLoader`调用相应函数
  >* `TExtensionInfo`中使用`TVulkanLoader`调用相应函数

* 2022/7/20 设计架构
  >
  >* `TVulkanLoader`加载`vkEnumeratePhysicalDevices`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceProperties`函数
  >* `TVulkanLoader`加载`vkEnumerateDeviceLayerProperties`函数
  >* `TVulkanLoader`加载`vkEnumerateDeviceExtensionProperties`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceQueueFamilyProperties`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceFeatures`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceMemoryProperties`函数
  >* `TPhysicalDevice::InternalCreate()`使用`Turbo::Core::vkEnumeratePhysicalDevices(...)`函数
  >* `TFormatInfo::GetSupportFormats`使用`TVulkanLoader`获取函数
  >* `TVulkanLoader`加载`vkCreateDevice`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceFormatProperties`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceImageFormatProperties`函数
  >* `TDevice::InternalCreate()`使用`TVulkanLoader`获取函数
  >* `TVulkanLoader`增加如下成员函数：
  >
  >```CXX
  >template <typename Function>
  >Function LoadDeviceFunction(TDevice *device, const char *name);
  >template <typename Function>
  >Function LoadDeviceFunction(VkDevice device, const char *name);
  >```
  >
  >* `TVulkanLoader`提供专门`TDevice/VkDevice`设备的特定实现函数版本获取
  >* `TVulkanLoader`加载`vkDestroyDevice`函数
  >* `TVulkanLoader`加载`vkGetPhysicalDeviceSparseImageFormatProperties`函数
  >* `TVulkanLoader`加载如下设备函数:
  >
  >```CXX
  >extern VULKAN_DEVICE_API PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
  >extern VULKAN_DEVICE_API PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
  >extern VULKAN_DEVICE_API PFN_vkAllocateMemory vkAllocateMemory;
  >extern VULKAN_DEVICE_API PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
  >extern VULKAN_DEVICE_API PFN_vkBindBufferMemory vkBindBufferMemory;
  >extern VULKAN_DEVICE_API PFN_vkBindImageMemory vkBindImageMemory;
  >extern VULKAN_DEVICE_API PFN_vkCmdBeginQuery vkCmdBeginQuery;
  >extern VULKAN_DEVICE_API PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
  >extern VULKAN_DEVICE_API PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
  >extern VULKAN_DEVICE_API PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
  >extern VULKAN_DEVICE_API PFN_vkCmdBindPipeline vkCmdBindPipeline;
  >extern VULKAN_DEVICE_API PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
  >extern VULKAN_DEVICE_API PFN_vkCmdBlitImage vkCmdBlitImage;
  >extern VULKAN_DEVICE_API PFN_vkCmdClearAttachments vkCmdClearAttachments;
  >extern VULKAN_DEVICE_API PFN_vkCmdClearColorImage vkCmdClearColorImage;
  >extern VULKAN_DEVICE_API PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
  >extern VULKAN_DEVICE_API PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
  >extern VULKAN_DEVICE_API PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
  >extern VULKAN_DEVICE_API PFN_vkCmdCopyImage vkCmdCopyImage;
  >extern VULKAN_DEVICE_API PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
  >extern VULKAN_DEVICE_API PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
  >extern VULKAN_DEVICE_API PFN_vkCmdDispatch vkCmdDispatch;
  >extern VULKAN_DEVICE_API PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
  >extern VULKAN_DEVICE_API PFN_vkCmdDraw vkCmdDraw;
  >extern VULKAN_DEVICE_API PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
  >extern VULKAN_DEVICE_API PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
  >extern VULKAN_DEVICE_API PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
  >extern VULKAN_DEVICE_API PFN_vkCmdEndQuery vkCmdEndQuery;
  >extern VULKAN_DEVICE_API PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
  >extern VULKAN_DEVICE_API PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
  >extern VULKAN_DEVICE_API PFN_vkCmdFillBuffer vkCmdFillBuffer;
  >extern VULKAN_DEVICE_API PFN_vkCmdNextSubpass vkCmdNextSubpass;
  >extern VULKAN_DEVICE_API PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
  >extern VULKAN_DEVICE_API PFN_vkCmdPushConstants vkCmdPushConstants;
  >extern VULKAN_DEVICE_API PFN_vkCmdResetEvent vkCmdResetEvent;
  >extern VULKAN_DEVICE_API PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
  >extern VULKAN_DEVICE_API PFN_vkCmdResolveImage vkCmdResolveImage;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetEvent vkCmdSetEvent;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetScissor vkCmdSetScissor;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
  >extern VULKAN_DEVICE_API PFN_vkCmdSetViewport vkCmdSetViewport;
  >extern VULKAN_DEVICE_API PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
  >extern VULKAN_DEVICE_API PFN_vkCmdWaitEvents vkCmdWaitEvents;
  >extern VULKAN_DEVICE_API PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
  >extern VULKAN_DEVICE_API PFN_vkCreateBuffer vkCreateBuffer;
  >extern VULKAN_DEVICE_API PFN_vkCreateBufferView vkCreateBufferView;
  >extern VULKAN_DEVICE_API PFN_vkCreateCommandPool vkCreateCommandPool;
  >extern VULKAN_DEVICE_API PFN_vkCreateComputePipelines vkCreateComputePipelines;
  >extern VULKAN_DEVICE_API PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
  >extern VULKAN_DEVICE_API PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
  >extern VULKAN_DEVICE_API PFN_vkCreateEvent vkCreateEvent;
  >extern VULKAN_DEVICE_API PFN_vkCreateFence vkCreateFence;
  >extern VULKAN_DEVICE_API PFN_vkCreateFramebuffer vkCreateFramebuffer;
  >extern VULKAN_DEVICE_API PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
  >extern VULKAN_DEVICE_API PFN_vkCreateImage vkCreateImage;
  >extern VULKAN_DEVICE_API PFN_vkCreateImageView vkCreateImageView;
  >extern VULKAN_DEVICE_API PFN_vkCreatePipelineCache vkCreatePipelineCache;
  >extern VULKAN_DEVICE_API PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
  >extern VULKAN_DEVICE_API PFN_vkCreateQueryPool vkCreateQueryPool;
  >extern VULKAN_DEVICE_API PFN_vkCreateRenderPass vkCreateRenderPass;
  >extern VULKAN_DEVICE_API PFN_vkCreateSampler vkCreateSampler;
  >extern VULKAN_DEVICE_API PFN_vkCreateSemaphore vkCreateSemaphore;
  >extern VULKAN_DEVICE_API PFN_vkCreateShaderModule vkCreateShaderModule;
  >extern VULKAN_DEVICE_API PFN_vkDestroyBuffer vkDestroyBuffer;
  >extern VULKAN_DEVICE_API PFN_vkDestroyBufferView vkDestroyBufferView;
  >extern VULKAN_DEVICE_API PFN_vkDestroyCommandPool vkDestroyCommandPool;
  >extern VULKAN_DEVICE_API PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
  >extern VULKAN_DEVICE_API PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
  >extern VULKAN_DEVICE_API PFN_vkDestroyDevice vkDestroyDevice;
  >extern VULKAN_DEVICE_API PFN_vkDestroyEvent vkDestroyEvent;
  >extern VULKAN_DEVICE_API PFN_vkDestroyFence vkDestroyFence;
  >extern VULKAN_DEVICE_API PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
  >extern VULKAN_DEVICE_API PFN_vkDestroyImage vkDestroyImage;
  >extern VULKAN_DEVICE_API PFN_vkDestroyImageView vkDestroyImageView;
  >extern VULKAN_DEVICE_API PFN_vkDestroyPipeline vkDestroyPipeline;
  >extern VULKAN_DEVICE_API PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
  >extern VULKAN_DEVICE_API PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
  >extern VULKAN_DEVICE_API PFN_vkDestroyQueryPool vkDestroyQueryPool;
  >extern VULKAN_DEVICE_API PFN_vkDestroyRenderPass vkDestroyRenderPass;
  >extern VULKAN_DEVICE_API PFN_vkDestroySampler vkDestroySampler;
  >extern VULKAN_DEVICE_API PFN_vkDestroySemaphore vkDestroySemaphore;
  >extern VULKAN_DEVICE_API PFN_vkDestroyShaderModule vkDestroyShaderModule;
  >extern VULKAN_DEVICE_API PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
  >extern VULKAN_DEVICE_API PFN_vkEndCommandBuffer vkEndCommandBuffer;
  >extern VULKAN_DEVICE_API PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
  >extern VULKAN_DEVICE_API PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
  >extern VULKAN_DEVICE_API PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
  >extern VULKAN_DEVICE_API PFN_vkFreeMemory vkFreeMemory;
  >extern VULKAN_DEVICE_API PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
  >extern VULKAN_DEVICE_API PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
  >extern VULKAN_DEVICE_API PFN_vkGetDeviceQueue vkGetDeviceQueue;
  >extern VULKAN_DEVICE_API PFN_vkGetEventStatus vkGetEventStatus;
  >extern VULKAN_DEVICE_API PFN_vkGetFenceStatus vkGetFenceStatus;
  >extern VULKAN_DEVICE_API PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
  >extern VULKAN_DEVICE_API PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
  >extern VULKAN_DEVICE_API PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
  >extern VULKAN_DEVICE_API PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
  >extern VULKAN_DEVICE_API PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
  >extern VULKAN_DEVICE_API PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
  >extern VULKAN_DEVICE_API PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
  >extern VULKAN_DEVICE_API PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
  >extern VULKAN_DEVICE_API PFN_vkMapMemory vkMapMemory;
  >extern VULKAN_DEVICE_API PFN_vkMergePipelineCaches vkMergePipelineCaches;
  >extern VULKAN_DEVICE_API PFN_vkQueueBindSparse vkQueueBindSparse;
  >extern VULKAN_DEVICE_API PFN_vkQueueSubmit vkQueueSubmit;
  >extern VULKAN_DEVICE_API PFN_vkQueueWaitIdle vkQueueWaitIdle;
  >extern VULKAN_DEVICE_API PFN_vkResetCommandBuffer vkResetCommandBuffer;
  >extern VULKAN_DEVICE_API PFN_vkResetCommandPool vkResetCommandPool;
  >extern VULKAN_DEVICE_API PFN_vkResetDescriptorPool vkResetDescriptorPool;
  >extern VULKAN_DEVICE_API PFN_vkResetEvent vkResetEvent;
  >extern VULKAN_DEVICE_API PFN_vkResetFences vkResetFences;
  >extern VULKAN_DEVICE_API PFN_vkSetEvent vkSetEvent;
  >extern VULKAN_DEVICE_API PFN_vkUnmapMemory vkUnmapMemory;
  >extern VULKAN_DEVICE_API PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
  >extern VULKAN_DEVICE_API PFN_vkWaitForFences vkWaitForFences;
  >```

* 2022/7/22 设计架构
  >
  >* `TVulkanLoader`中只提供`Vulkan_Core`的函数函数声明和获取，也就是说`TVulkanLoader.h`中只存在`Vulkan`核心函数声明和获取，有关`Vulkan`的属于扩展范围的函数，需要在相应的扩展模块使用`TVulkanLoader`自定义加载相应的函数
  >* `TVulkanLoader.h`中增加如下声明用来标识函数是`Vulkan`的核心函数还是扩展函数
  >
  >```CXX
  >#define VULKAN_CORE
  >#define VULKAN_EXTENSION
  >```

* 2022/7/23 设计架构
  >
  >* `TVulkanLoader`中增加`struct TDeviceFunctionTable`内部有设备相关函数声明，并用`TDeviceDriver`重命名
  >* `TVulkanLoader`中增加`TDeviceDriver LoadDeviceDriver(TDevice *device)`用于返回特定设备函数实现版本
  >* `TDevice`中增加`TDeviceDriver *deviceDriver;`用于创建设备函数表
  >* `TDevice::InternalCreate()`中增加对`TDeviceDriver *deviceDriver;`的初始化
  >* `TDevice`中增加`const TDeviceDriver *GetDeviceDriver();`函数，用于返回设备指定实现函数表（驱动）
  >* `~TDevice`中增加`vkDeviceWaitIdle(...)`调用使用`driver`替换调用
  >* 开始将`TVmaAllocator`中使用`TDeviceDriver`调用使适配

* 2022/7/24 设计架构
  >
  >* 开始将`TDeviceQueue`中使用`TDeviceDriver`调用使适配
  >* 开始将`TCommandBuffer`中使用`TDeviceDriver`调用使适配
  >* `TPipeline`中增加`TDevice* GetDevice()`成员函数
  >* 开始将`TCommandBufferPool`中使用`TDeviceDriver`调用使适配
  >* 开始将`TDescriptorPool`中使用`TDeviceDriver`调用使适配
  >* 开始将`TDescriptorSet`中使用`TDeviceDriver`调用使适配
  >* 开始将`TDescriptorSetLayout`中使用`TDeviceDriver`调用使适配
  >* 开始将`TFence`中使用`TDeviceDriver`调用使适配
  >* 开始将`TFramebuffer`中使用`TDeviceDriver`调用使适配
  >* 开始将`TGraphicsPipeline`中使用`TDeviceDriver`调用使适配
  >* 开始将`TImageView`中使用`TDeviceDriver`调用使适配
  >* 开始将`TPipelineLayout`中使用`TDeviceDriver`调用使适配
  >* 开始将`TRenderPass`中使用`TDeviceDriver`调用使适配
  >* 开始将`TSampler`中使用`TDeviceDriver`调用使适配
  >* 开始将`TSemaphore`中使用`TDeviceDriver`调用使适配
  >* 开始将`TShader`中使用`TDeviceDriver`调用使适配
  
* 2022/7/25 设计架构
  >
  >* `TDeviceQueue::InternalCreate()`中使用`TVulkanLoader`加载`vkQueuePresentKHR`函数
  >* `TDeviceQueue::Present(...)`中使用`TVulkanLoader`加载的`vkQueuePresentKHR`函数进行调用
  >* 将如下定义从`TVulkanLoader.h`中移动到`TCore.h`中：
  >
  >```CXX
  >#define VULKAN_GLOBAL_API
  >#define VULKAN_INSTANCE_API
  >#define VULKAN_DEVICE_API
  >#define VULKAN_CORE
  ># efine VULKAN_EXTENSION
  >```
  >
  >* 开始将`TSurface`中使用`TDeviceDriver`调用使适配

* 2022/7/27 设计架构
  >
  >* 开始将`TSwapchain`中使用`TDeviceDriver`调用使适配
  >* 有小伙伴提出在单独使用`Turbo`的`TCore`核心库的时候，需要手动链接`TCore`库的依赖库，经过检验确实如此，这不是`TCore`库发布的初衷，有关该问题的细节请参阅`./docs/issue.md::Turbo核心生成的TCore库...`，由于本人`CMake`并不是很精通，目前正在研究中...
  >* `TCore`中有关`Vulkan`函数的动态加载目前基本结束，接下来陆续撤走`CMake`中需要硬编码指定`vulkan-1.lib`目录的配置，计划将`vulkan/vulkan.h`头文件加入`TCore`中
  >* 将`Vulkan SDK 1.3.204.1`中的`Include`文件夹下的内容拷贝到了`./engine/core/include/vulkan`目录下
  >* 修改`TCore.h`中对于`#include <vulkan/vulkan.h>`的引用,修改成`#include "vulkan/vulkan.h"`
  >* 修改`TSurface.h`中对于`Vulkan`头文件的引用,修改成本地`vulkan`头文件
  >* 移除`TCore`的`CMake`中对于`Vulkan`目录配置的硬编码

* 2022/7/28 设计架构
  >
  >* 调整`Turbo`核心的`CMakeLists.txt`
  >* 移除`Turbo`核心的`vk_mem_alloc.h`,使用第三方库中的头文件
  >* 尝试解决`TCore`的依赖库问题
  >* 修改`PureIndexDraw`例子中的`IndexBuffer`大小的`bug`，感谢`会翔`提供的反馈

* 2022/7/30 设计架构
  >
  >* 调整`./engine/core/CMakeLists.txt`,尝试解决`TCore`的依赖库问题
  >* 调整`TShader.cpp`中对于`glslang`中`GlslangToSpv.h`头文件引用层级
  >* 调整`./main.cpp`中对于`vkGetImageSubresourceLayout(...)`的调用，改成`Turbo::Core::vkGetImageSubresourceLayout(...)`
  >* 调整`./main.cpp`中对于`vkDestroySurfaceKHR(...)`的调用，改成使用`TVulkanLoader`获取调用
  >* 将`./CMakeLists.txt`中对于`imgui_impl_glfw.cpp`和`imgui_impl_vulkan.cpp`的引用去掉，用不上
  >* 同理将`./samples/CMakeLists.txt`中对于`imgui_impl_glfw.cpp`和`imgui_impl_vulkan.cpp`的引用去掉，用不上
  >* `./samples`中的示例将适配`TVulkanLoader`
  >* **注：[VulkanMemoryAllocator]：If you fetch pointers to all Vulkan functions in a custom way**, e.g. using some loader like[Volk](https://github.com/zeux/volk)
  >   * Define `VMA_STATIC_VULKAN_FUNCTIONS` and `VMA_DYNAMIC_VULKAN_FUNCTIONS` to 0.
  >   * Pass these pointers via structure #VmaVulkanFunctions.
  >* 考虑是否将外部引入的`VkSurfaceKHR`中在`TSurface`析构时顺便销毁，目前外部引入的`VkSurfaceKHR`，需要在外部自己销毁
  >* `TVmaAllocator`目前对于`Vulkan API`的获取，使用`VulkanMemoryAllocator`内部自动获取。
  >   * If you want VMA to fetch pointers to Vulkan functions dynamically using vkGetInstanceProcAddr, vkGetDeviceProcAddr (this is the option presented in the example below):
  >     1. Define VMA_STATIC_VULKAN_FUNCTIONS to 0, VMA_DYNAMIC_VULKAN_FUNCTIONS to 1.
  >     2. Provide pointers to these two functions via VmaVulkanFunctions::vkGetInstanceProcAddr, VmaVulkanFunctions::vkGetDeviceProcAddr.
  >     3. The library will fetch pointers to all other functions it needs internally.
  >* 至此`Turbo`核心库`TCore`依赖库问题已解决，感谢`会翔`提供的问题反馈
