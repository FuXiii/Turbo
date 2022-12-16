# Issue

## demo

- `fixed :` <font color=green>**[ ✓ ]year/month/day**</font>
- `error :` <font color=red>**[ 🞭 ]year/month/day**</font>
- `alarm :` <font color=yellow>**[ ! ]year/month/day**</font>
- `develope :` <font color=orange>**[ 🛠 ] now**</font>

### **文档！别忘写文档**

- 考虑`Turbo`中是否内置几个`TCommandBufferPool`用于`Turbo`引擎内部自己用（在`Turbo::Core::TDevice`被创建，内部生成`Turbo::Core::TDeviceQueue`之后，`Turbo::Core::TDevice`被销毁与其相关的`TCommandBufferPool`和其相关的内部资源也会一并销毁）。（该功能应该在`CommandBuffer`所有功能实现结束之后实现）

- <font color=green>**[ ✓ ]2022/5/6**</font> ~~<font color=yellow>**[ ! ]2022/5/6 基本搞清问题原因了，`TFormatInfo` 需要提供对于`VkFormatFeatureFlagBits`的支持，通过调用`vkGetPhysicalDeviceFormatProperties`获得。**</font> `Turbo::Core::TCommandBuffer::BeginRenderPass()中的ClearColor` 可能有问题（等到支持窗口渲染后再调，调试起来方便），而且现在写死了，需要开放对外接口(比如说放到`TAttachemnt`中声明)。~~

- 考虑将`TCommandBuffer::BeginRenderPass()`中的`ClearColor`得颜色数据放到`TAttachment`中，这样用户就可以自定义`ClearColore`了

- `Turbo::Core::TCommandBuffer::BeginRenderPass()中的ClearColor` 中有些函数形参传的是引用（特别是`std::vector<XXX>& xxx`），这将会导致很多没有必要的内存创建，尝试将引用改成指针。

- <font color=green>**[ ✓ ]2022/5/1**</font> ~~`TFormatInfo` 目前用起来比较麻烦，提供一种简便方法，比如隐示转换。~~

- `TFormatInfo`增加对于`vkGetPhysicalDeviceFormatProperties(...)`和`vkGetPhysicalDeviceImageFormatProperties(...)`功能的实现接口

- `Turbo::Core::TBuffer`的构造函数需要传入`VkBufferCreateFlags`，在`Turbo`中用起来比较奇怪，或者不向外提供该参数（目前搞不清楚用法，或者就这么放着到最后再说）

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TBuffer`的构造函数需要传入`VkDeviceSize`，在`Turbo`中另外定义`TDeviceSize`~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TBuffer`的构造函数需要传入`VkBufferUsageFlagBits`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- `Turbo::Core::TImage`的构造函数需要传入`VkImageCreateFlags`，在`Turbo`中用起来比较奇怪，或者不向外提供该参数（目前搞不清楚用法，或者就这么放着到最后再说）

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImage`的构造函数需要传入`VkImageType`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImage`的构造函数需要传入`VkSampleCountFlagBits`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImage`的构造函数需要传入`VkImageTiling`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImage`的构造函数需要传入`VkImageUsageFlags`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImage`的构造函数需要传入`VkImageLayout`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImageView`的构造函数需要传入`VkImageViewType`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TImageView`的构造函数需要传入`VkImageAspectFlags`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构~~

- `Turbo::Core::TShader`中对于`code`的二进制数据是一直存在于内存的，对于`Vulkan`标准来说，这部分二进制数据在`VkShaderModule`创建完之后是没有意义的，目前该部分数据，仅仅是用于`ToString()`时转成`Shader`可读代码输出，无其他有用性。

- `Turbo::Core::TShader`中对于`VkDescriptor`的支持，目前还缺少对于`storage image`, `subpass input(input attachemnt)`和`storage buffer`

- <font color=orange>**[ 🛠 ] now**</font> 实现 `subpass input(input attachemnt)`相关特性，完成延迟渲染相关功能

- <font color=green>**[ ✓ ]2022/5/14**</font> ~~`Turbo::Core::TShader`中对于`VkDescriptor`的支持，开始适配`texture`，`sampler`。~~

- <font color=green>**[ ✓ ]2022/5/14**</font> ~~`Turbo::Core::TDescriptorSet`中对于`Turbo::Core::TDescriptorSet::BindData()`函数只是用来测试的，只支持`VkWriteDescriptor`和`UniformBuffer`，其他的待实现~~

- <font color=green>**[ ✓ ]2022/5/14**</font> ~~`Turbo::Core::TDescriptorSet`中对于`Turbo::Core::TDescriptorSet::BindData()`函数只是用来测试的，只支持`VkWriteDescriptor`和`UniformBuffer`，其他的待实现~~

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TSubpass::Add[...]AttachmentReference`中对于`VkImageLayout`的传入和`TImageView`问题一样~~

- **`Note:`** `Turbo::Core::TSubpass`中的`std::vector<VkAttachmentReference> ...`保留，用起来很方便

- 考虑`VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC`如何适配

- <font color=green>**[ ✓ ]2022/4/29**</font> ~~`Turbo::Core::TAttachment`构造函数中对于`Vulkan`声明的数据结构依赖过多，须自定义（`VkFormat`,`VkSampleCountFlagBits`,`VkAttachmentLoadOp`,`VkAttachmentStoreOp`等）~~

- <font color=green>**[ ✓ ]2022/4/30**</font> ~~对于`VkVertexInputBindingDescription`和`VkVertexInputAttributeDescription`考虑如何设计，使用起来方便~~

- <font color=green>**[ ✓ ]2022/5/3**</font> ~~`Turbo::Core::TRenderPass`构造函数的`TPipelineType type`转移到`TSubpass`中~~

- <font color=green>**[ ✓ ]2022/5/2**</font>  ~~`Turbo::Core::TPipeline`重度依赖`Vulkan`的数据结构~~

- `Turbo::Core::TPipeline`目前只实现了`Graphic`，计算管线`Compute`未实现

- <font color=green>**[ ✓ ]2022/5/1**</font> ~~`Turbo::Core::TPipeline`目前构造函数直接写入的是`Graphic`管线数据，该设计有问题，`Graphic`图形管线和计算管线`Compute`创建参数不同，相关功能和创建应该放到具体的子类中，`TPipeline`只提供虚基类~~

- <font color=green>**[ ✓ ]2022/5/27**</font> ~~`Turbo::Core::TPipeline`目前`...CreatInfo`不完整，有些数据是写死的，如：~~
  - ~~`VkPipelineMultisampleStateCreateInfo`~~
  - ~~`VkPipelineDepthStencilStateCreateInfo`~~
  - ~~`VkPipelineColorBlendAttachmentState`~~
  - ~~`VkPipelineColorBlendStateCreateInfo`~~
  - ~~`VkPipelineDynamicStateCreateInfo`~~

- 目前只实现了`TGraphicsPipeline`中所有的`VkPipelineShaderStageCreateInfo::pName`指代的`Shader`入口函数全是`main`,考虑是否提供自定义化接口(`glslang`好像提供相关入口函数获取功能)

- 目前只实现了`TGraphicsPipeline`中的`VkDynamicState`目前是写死的将`VK_DYNAMIC_STATE_VIEWPORT`、`VK_DYNAMIC_STATE_SCISSOR`和`VK_DYNAMIC_STATE_LINE_WIDTH`加入了动态数组中

- `Turbo::Core::TCommandBuffer`很多指令未实现，待实现

- `Second Command Buffer`目前`Turbo`不支持

- 尝试重载`TSubpass`的`operator >>`用于构成渲染链，比如：(也许放到FrameGraph层是个不错的选择)

```CXX
TSubpassChain subpass_chain_1 = subpass1 >> subpass2 >> subpass3;
TSubpassChain subpass_chain_2 = subpass4 >> subpass5 >> subpass6;

TRenderPass render_pass_1(subpass_chain_1);
TRenderPass render_pass_2(subpass_chain_2);
```

- <font color=green>**[ ✓ ]2022/5/4**</font> ~~实现`Barrier`~~

- <font color=green>**[ ✓ ]2022/5/5**</font> ~~实现`TCommandBuffer::FillBuffer()`~~

- 也许`TCommandBuffer`对于`Barrier`中的`TAccess`也许可以由内部根据`oldLayout`和`newLayout`来确定数值

- 需要提供`TBufferView`

- 需要提供`TDisplay`

- 需要修缮`TExtension`，尝试分成`TInstanceExtension`和`TPhysicalDeviceExtension`

- <font color=green>**[ ✓ ]2022/5/16**</font> ~~需要提供`TFilter`对应`VkFilter`，完善`TCommandBuffer::BlitImage(...)`~~

- 需要提供对应`KTX`文件的支持(非核心)

- 需要提供对应`gltf`文件的支持(非核心)

- <font color=green>**[ ✓ ]2022/5/10**</font> ~~实现索引绘制~~

- <font color=green>**[ ✓ ]2022/5/11**</font> ~~实现纹理特性~~ 引入`KTX`规范`(KTX工具库目前好像不支持MinGW编译器)`

- <font color=green>**[ ✓ ]2022/5/12**</font>~~`Descriptor`设计有重大逻辑漏洞，需要重构~~

- `TSampler`的`VkCompareOp compareOp`特性目前固定为`VK_FALSE`,等统一设计深度测试那一部分再展开

- 当前`TSurface`只支持`Windows`平台，其他平台未实现，但留了接口

- 第三方库`glm`对于`MVP`矩阵异常的问题

- 当前`DescriptorSet`设计总感觉不合理，会冗余，应该搞个池子

- 使用加载动态库的方式加载`Vulkan`库

- `TCommandBuffer::CmdPushConstants(...)`中的`ShaderStages`进一步研究，现在写的有问题

- 梳理一下`TExtensionInfo`类，最好类似`TFormatInfo`的类思想

- `TSampler`的各项异性没开要和`TDevice`开启的`feature`配合

- 整理设计有关`Vulkan`中的`ImageSubresource`

- `TInstance`的`IsEnabledExtension(string)`函数中倒腾了两遍，没有必要，提供一个新的对应函数：`IsEnabledExtension(Turbo::Core::TExtensionType)`

- `TImage`需要对外提供`TImageLayout GetImageLayout()`来获取当前的图像布局,或者也许应该是`TCommandBuffer`中获取当前`TImage`的`TImageLayout`，因为图片布局在`TCommandBuffer`中随意变换的，只有在`TCommanBuffer`被推送运行后，图片布局才确定下来，此时可以用于刷新真正`TImage`中的图片布局，`TCommandBuffer`命令之间的图片布局变换只是变换`TImage`的临时图片布局

- <font color=green>**[ ✓ ]2022/5/3**</font>  ~~`Subpass Dependence`目前`Turbo`未实现~~

- <font color=green>**[ ✓ ]2022/5/2**</font> ~~实现`Turbo::Core::TCommandBuffer::NextSubpass()`函数~~

- <font color=green>**[ ✓ ]2022/5/2**</font>  ~~`Turbo`目前只支持单采样，多采样未捣鼓。对于多采样是`TSubpassPass`的功能，在`TSubpass`中指定多采样附件，根据`Vulkan`标准，规定`ResolveAttachment`中的多采样附件如果有的话，数量一定等于`ColorAttachment`(要不就都是多采样，要不就都是单采样),`TPipeline`中也有`VkPipelineMultisampleStateCreateInfo`~~

- <font color=green>**[ ✓ ]2022/5/8**</font> ~~**(该功能转移至`TSurface`中)** 提供`Swapchain`,有些窗口库创建完窗口直接返回`VkSurfaceKHR`,所以`TSwapchain`需要对外提供一个接口构造函数`TSwapchain(TDevice* device, VkSurfaceKHR)`用于接收外部创建好的`VkSurfaceKHR`~~

- <font color=green>**[ ✓ ]2022/7/23**</font> ~~`TVulkanLoader`中提供类似获取`struct DeviceFunctionTable`类型的函数，用于获取`VkDevice`特定的函数实现版本~~

- `TVulkanLoader`中`Destroy()`目前未被调用，应该在`TInstance`析构函数中调用，用于卸载`Vulkan`动态库

- <font color=green>**[ ✓ ]2022/7/27**</font> ~~`Turbo`中未完全覆盖`TVulkanLoader`获取的函数调用~~
  - <font color=green>**[ ✓ ]2022/7/25**</font> ~~`Turbo::Core::TDeviceQueue::Present(...)::vkQueuePresentKHR(...)`未覆盖~~
  - <font color=green>**[ ✓ ]2022/7/25**</font> ~~`TSurface`未覆盖~~
  - <font color=green>**[ ✓ ]2022/7/27**</font> ~~`TSwapchain`未覆盖~~

- <font color=green>**[ ✓ ]2022/7/30**</font> ~~`TCore`中`CMakeLists.txt`中有些库应该是`PRIVATE`~~

- `TCore`中`CMakeLists.txt`对于库的创建应该取消现有的设成静态库的配置，转成用户自定义生成静态库还是动态库

- <font color=green>**[ ✓ ]2022/7/30**</font> ~~`Turbo`核心生成的`TCore`库,照常理应该在`Visual Studio`中链接时不需要再次链接核心的依赖库，但是目前测试，在`Visual Studio`还是需要链接比如`vma`,`glslang`等核心依赖库才能链接到`TCore`库中否则报未定义相关函数实现的错误，初步怀疑是`CMake`的`target_link_libraries`中的`PUBLIC`和`PRIVATE`的区别，但是改成`PRIVATE`好像并不能解决以上问题。如果使用`ar.exe`之类的库合并工具需要写自定义`CMake`指令，`Visual Studio`的库合并工具是官方的`lib.exe`，但是，但是使用库合并工具应该不是最好的解决方案。研究中...~~

- `Turbo`中目前还有很多跨平台的内容没有填补，正常没有实现的需要抛出`throw Turbo::Core::TException(Turbo::Core::TResult::UNIMPLEMENTED,...)`异常,帮助跨平台开发提示

- `TVulkanLoader`需要适配`Linux`系统

- 考虑是否将外部引入的`VkSurfaceKHR`中在`TSurface`析构时顺便销毁，目前外部引入的`VkSurfaceKHR`，需要在外部自己销毁

- `Turbo`引擎中对于`Vulkan`的扩展支持不全，新版的`Vulkan`扩展比如`1.3`的一些新扩展没有，有时间添加

- `Turbo`的`Core`中有个`TEngine`类，该类提供的功能顶多算一个辅助信息获取，没有这个类也是可以的，有时间将其剔出去

- `Turbo`的`Core`中`TCommandBuffer`类中，`vkBeginCommandBuffer`中对于二级指令缓冲需要指定`VkCommandBufferUsageFlags`。`vkCmdBeginRenderPass(...)`中对于二级指令缓冲需要指定`VkSubpassContents::VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS`

- `void Turbo::Core::TPipelineLayout::InternalCreate()`中对于`VkPushConstantRange`的数据`offset`的解析可能有问题

- 创建`Pipeline`时可以在`VkPipelineShaderStageCreateInfo`指定`VkSpecializationInfo`数据，目前没有支持该功能，需要支持。

- 创建`TComputePipeline`和`TGraphicsPipeline`时，指定的都是`TShader`，可能会构成困扰，将`TShader`派生出`TVertexShader`、`TFragmentShader`、`TComputeShader`等。

- 优化`TComputePipeline`、`TGraphicsPipeline`和`TShader`之间的结构关系，现在的有点杂乱敷衍

- 派生了`TShader`的一些子类，父类中的一些应该属于子类的属性应该挪到子类中。

- 在`Windows11`下，英伟达驱动`472.91`，显卡型号`RTX3070`下`std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TLayerInfo::GetInstanceLayers()`将会返回空数组，`Vulkan`运行时此时找不到支持的验证层。

- 在`RayMarchingBoundingBoxTest`中使用纯数学，光线向量与包围盒求交，会有一圈圈的纹路，不知为啥，光追噪音？还是代码理论哪里出现了问题？

- 在`engine\framegraph\include\TFrameGraph.hpp`中类`template <typename T> class TResourceProxy`中的成员变量`T resource;`，这样声明可能会有问题，用户不一定会指定带有无参默认构造函数的类。这里改成指针可能会更好。

- 在`engine\framegraph\include\TFrameGraph.hpp`中`Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::Write(TResource resource)`中，如果此时`resource`在当前`pass`中还没有创建，应该看一下`Fg`中是否有该资源节点:`this->frameGraph.GetResourceNode(resource)`，如果`this->frameGraph.GetResourceNode(resource)`中也没找到该资源节点的话，说明资源未被创建，应有的策略是直接终止程序，而终止程序是在`this->frameGraph.GetResourceNode(resource)`中使用`assert(this->IsValid(resource))`，这在`Release`模式下会失效。正常应该返回异常（考虑是否使用`Turbo::Core`中的异常）

- 在`engine\framegraph\include\TFrameGraph.hpp`中`Turbo::FrameGraph::TResource Turbo::FrameGraph::TFrameGraph::TBuilder::Read(TResource resource)`中，同上一条`...::Write(TResource resource)`中相同的问题

- 在`engine\framegraph\include\TFrameGraph.hpp`中`void Turbo::FrameGraph::TFrameGraph::Execute(void *context)`中，在运行结束时会去销毁对应得资源文件，正常此时应该将资源标记为待回收，并进行异步回收。

- 由于`TFrameGraph`中的资源回收属于异步回收（见上一条目），则需要一个异步回收器，在结束一帧的工作后进行异步回收。

- `./engine/core`下`TBuffer`和`TImage`有个公共类应该更加合理

- 修改`./engine/core`下`TBuffer`和`TImage`对于`TFormatInfo`参数的构造函数，更改成使用`TFormatType formatType`参数构造
