# Issue

* ### **文档！别忘写文档**

* 考虑`Turbo`中是否内置几个`TCommandBufferPool`用于`Turbo`引擎内部自己用（在`Turbo::Core::TDevice`被创建，内部生成`Turbo::Core::TDeviceQueue`之后，`Turbo::Core::TDevice`被销毁与其相关的`TCommandBufferPool`和其相关的内部资源也会一并销毁）。

* `Turbo::Core::TCommandBuffer::BeginRenderPass()中的ClearColor` 可能有问题（等到支持窗口渲染后再调，调试起来方便），而且现在写死了，需要开放对外接口(比如说放到`TAttachemnt`中声明)

* `Turbo::Core::TBuffer`的构造函数需要传入`VkBufferCreateFlags`，在`Turbo`中用起来比较奇怪，或者不向外提供该参数（目前搞不清楚用法，或者就这么放着到最后再说）

* `Turbo::Core::TBuffer`的构造函数需要传入`VkDeviceSize`，在`Turbo`中另外定义`TDeviceSize`

* `Turbo::Core::TBuffer`的构造函数需要传入`VkBufferUsageFlagBits`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImage`的构造函数需要传入`VkImageCreateFlags`，在`Turbo`中用起来比较奇怪，或者不向外提供该参数（目前搞不清楚用法，或者就这么放着到最后再说）

* `Turbo::Core::TImage`的构造函数需要传入`VkImageType`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImage`的构造函数需要传入`VkSampleCountFlagBits`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImage`的构造函数需要传入`VkImageTiling`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImage`的构造函数需要传入`VkImageUsageFlags`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImage`的构造函数需要传入`VkImageLayout`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImageView`的构造函数需要传入`VkImageViewType`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TImageView`的构造函数需要传入`VkImageAspectFlags`，在`Turbo`中用起来比较奇怪，构造自己的相关数据结构

* `Turbo::Core::TShader`中对于`code`的二进制数据是一直存在于内存的，对于`Vulkan`标准来说，这部分二进制数据在`VkShaderModule`创建完之后是没有意义的，目前该部分数据，仅仅是用于`ToString()`时转成`Shader`可读代码输出，无其他有用性。

* `Turbo::Core::TShader`中对于`VkDescriptor`的支持，只支持`UniformBuffer`，其他类型接口留了，没实现。

* `Turbo::Core::TDescriptorSet`中对于`Turbo::Core::TDescriptorSet::BindData()`函数只是用来测试的，只支持`VkWriteDescriptor`和`UniformBuffer`，其他的待实现

* `Turbo::Core::TSubpass::Add[...]AttachmentReference`中对于`VkImageLayout`的传入和`TImageView`问题一样

* `Turbo::Core::TSubpass`中的`std::vector<VkAttachmentReference> ...`保留，用起来很方便

* `Turbo::Core::TAttachment`构造函数中对于`Vulkan`声明的数据结构依赖过多，须自定义（`VkFormat`,`VkSampleCountFlagBits`,`VkAttachmentLoadOp`,`VkAttachmentStoreOp`等）

* 对于`VkVertexInputBindingDescription`和`VkVertexInputAttributeDescription`考虑如何设计，使用起来方便

* `Turbo::Core::TRenderPass`构造函数的`TPipelineType type`转移到`TSubpass`中

* `Turbo::Core::TPipeline`重度依赖`Vulkan`的数据结构
