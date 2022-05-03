# Issue

## demo

- `fixed :` <font color=green>**[ ✓ ]year/month/day**</font>
- `error :` <font color=red>**[ 🞭 ]year/month/day**</font>
- `alarm :` <font color=yellow>**[ ! ]year/month/day**</font>
- `develope :` <font color=orange>**[ 🛠 ] now**</font>

### **文档！别忘写文档**

- 考虑`Turbo`中是否内置几个`TCommandBufferPool`用于`Turbo`引擎内部自己用（在`Turbo::Core::TDevice`被创建，内部生成`Turbo::Core::TDeviceQueue`之后，`Turbo::Core::TDevice`被销毁与其相关的`TCommandBufferPool`和其相关的内部资源也会一并销毁）。

- `Turbo::Core::TCommandBuffer::BeginRenderPass()中的ClearColor` 可能有问题（等到支持窗口渲染后再调，调试起来方便），而且现在写死了，需要开放对外接口(比如说放到`TAttachemnt`中声明)。

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

- `Turbo::Core::TShader`中对于`VkDescriptor`的支持，只支持`UniformBuffer`，其他类型接口留了，没实现。

- `Turbo::Core::TDescriptorSet`中对于`Turbo::Core::TDescriptorSet::BindData()`函数只是用来测试的，只支持`VkWriteDescriptor`和`UniformBuffer`，其他的待实现

- <font color=green>**[ ✓ ]2022/4/28**</font> ~~`Turbo::Core::TSubpass::Add[...]AttachmentReference`中对于`VkImageLayout`的传入和`TImageView`问题一样~~

- **`Note:`** `Turbo::Core::TSubpass`中的`std::vector<VkAttachmentReference> ...`保留，用起来很方便

- <font color=green>**[ ✓ ]2022/4/29**</font> ~~`Turbo::Core::TAttachment`构造函数中对于`Vulkan`声明的数据结构依赖过多，须自定义（`VkFormat`,`VkSampleCountFlagBits`,`VkAttachmentLoadOp`,`VkAttachmentStoreOp`等）~~

- <font color=green>**[ ✓ ]2022/4/30**</font> ~~对于`VkVertexInputBindingDescription`和`VkVertexInputAttributeDescription`考虑如何设计，使用起来方便~~

- `Turbo::Core::TRenderPass`构造函数的`TPipelineType type`转移到`TSubpass`中

- <font color=green>**[ ✓ ]2022/5/2**</font>  ~~`Turbo::Core::TPipeline`重度依赖`Vulkan`的数据结构~~

- `Turbo::Core::TPipeline`目前只实现了`Graphic`，计算管线`Compute`未实现

- <font color=green>**[ ✓ ]2022/5/1**</font> ~~`Turbo::Core::TPipeline`目前构造函数直接写入的是`Graphic`管线数据，该设计有问题，`Graphic`图形管线和计算管线`Compute`创建参数不同，相关功能和创建应该放到具体的子类中，`TPipeline`只提供虚基类~~

- 计算管线`Compute`未实现

- `Turbo::Core::TPipeline`目前`...CreatInfo`不完整，有些数据是写死的，如：
  - ~~`VkPipelineMultisampleStateCreateInfo`~~
  - `VkPipelineDepthStencilStateCreateInfo`
  - `VkPipelineColorBlendAttachmentState`
  - `VkPipelineColorBlendStateCreateInfo`
  - `VkPipelineDynamicStateCreateInfo`

- 目前只实现了`TGraphicsPipeline`中所有的`VkPipelineShaderStageCreateInfo::pName`指代的`Shader`入口函数全是`main`,考虑是否提供自定义化接口(`glslang`好像提供相关入口函数获取功能)

- 目前只实现了`TGraphicsPipeline`中的`VkDynamicState`目前是写死的将`VK_DYNAMIC_STATE_VIEWPORT`和`VK_DYNAMIC_STATE_SCISSOR`加入了动态数组中

- `Turbo::Core::TCommandBuffer`很多指令未实现，待实现

- `Second Command Buffer`目前`Turbo`不支持

- <font color=orange>**[ 🛠 ] now**</font>  `Subpass Dependence`目前`Turbo`未实现

- <font color=green>**[ ✓ ]2022/5/2**</font> ~~实现`Turbo::Core::TCommandBuffer::NextSubpass()`函数~~

- <font color=green>**[ ✓ ]2022/5/2**</font>  ~~`Turbo`目前只支持单采样，多采样未捣鼓。对于多采样是`TSubpassPass`的功能，在`TSubpass`中指定多采样附件，根据`Vulkan`标准，规定`ResolveAttachment`中的多采样附件如果有的话，数量一定等于`ColorAttachment`(要不就都是多采样，要不就都是单采样),`TPipeline`中也有`VkPipelineMultisampleStateCreateInfo`~~
