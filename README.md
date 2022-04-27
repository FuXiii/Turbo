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

## Trifles

* 整理一下头文件，有点乱，去掉不必要的头文件

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
