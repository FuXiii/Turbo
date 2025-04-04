# 更新日志

## Memo

[备忘录](./docs/Memo.md)

## 2025/4/4

>* `engine\core\TDescriptor` 下 `TPushConstants` 增加使用 `offset`、`size`、`flags` 参数的 `Merge(...)` 函数。

## 2025/4/3

>* `engine\core\TShader` 下 `TLayout` 增加使用 `TSet`、`TBinding`、`TDescriptor`、`TBindings` 和 `TDescriptorSetLayout::TLayout` 参数的 `Merge(...)` 函数。
>* `engine\core\TShader` 下 `TLayout` 增加 `operator[]` 函数。
>* `engine\core\TDescriptorSetLayout` 下 `TLayout` 增加使用 `TBinding`、`TDescriptor`、`TBindings` 参数的 `Merge(...)` 函数。
>* `engine\core\TDescriptorSetLayout` 下 `TLayout` 增加 `operator[]` 函数。

## 2025/4/2

>* `engine\core\TShader` 下优化 `spirv_cross::CompilerGLSL` 的创建，直接使用`SPIR-V`代码构建，而不是使用拷贝之后的数据，提高效率。
>* `engine\core\TShader` 下优化，移除指针式内存分配，直接使用 `std::vector<uint32_t>` 存储和管理`SPIR-V`代码。提高效率。统一接口。

## 2025/4/1

>* `engine\core\TShader` 下 `TLayout` 增加 `TSets` 、 `TPushConstants` 版本的构造函数和 `Merge` 成员函数。

*注:目前`SPIRV-Reflect`对于`Specialization Constant`的支持还不是很完整。目前计划仍然使用 `SPIRV-Cross`进行解析*

## 2025/3/30

>* `engine\core\TDescriptor` 下 `TPushConstants` 增加 `bool Empty() const` 成员函数。
>* `engine\core\TDescriptorSetLayout` 下 `TDescriptorSetLayout::TLayout` 增加 `bool Empty() const` 成员函数。
>* `engine\core\TShader` 下 `TShader::TLayout` 增加 `bool Empty() const` 成员函数。
>* `engine\core\TDescriptorSetLayout` 下 `TDescriptorSetLayout` 增加使用 `TDescriptorSetLayout::TLayout` 的构函数。

## 2025/3/28

>* `engine\core` 下 `TDescriptor` 中优化实现 `ToString()` 成员函数。
>* `engine\core` 下 `TPushConstants` 中优化实现 `ToString()` 成员函数。
>* `engine\core` 下 `TDescriptorSetLayout::TLayout` 中优化实现 `ToString()` 成员函数。
>* `engine\core` 下 `TDescriptorSetLayout::TLayout` 中优化实现 `Merge(...)` 成员函数。
>* `engine\core` 下 `TShader::TLayout` 中实现 `Merge(...)` 成员函数。

## 2025/3/27

>* `engine\core` 下 `TDescriptor` 中 `TBinding` 声明转移至 `TDescriptorSetLayout::TLayout` 中。
>* `engine\core` 下 `TDescriptor` 中 `TBindings` 声明转移至 `TDescriptorSetLayout::TLayout` 中。
>* `engine\core` 下 `TDescriptor` 中 `Type` 枚举声明重命名为 `TType`。
>* `engine\core` 下 `TDescriptor` 中 `TSet` 声明转移至 `TShader::TLayout` 中。
>* `engine\core` 下 `TShader::TLayout` 中增加 `TPushConstants` 成员变量。
>* `engine\core` 下 `TDescriptor` 中实现 `TPushConstants` 功能。

## 2025/3/26

>* `engine\core` 下 `TDescriptor` 中新增 `TSet` 声明。
>* `engine\core` 下 `TDescriptor` 中新增 `TPushConstants` 声明。
>* `engine\core` 下 `TShader` 中新增 `Layout` 子类声明。

## 2025/3/25

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。
>* `samples` 下更新 `TurboAPIDesign_Descriptor` 示例用于测试设计代码。
>* `engine\core` 下 `TDescriptor` 中将 `TDescriptorType` 转移至 `TDescriptor` 类中声明，并重命名为 `Type`。
>* 将所有 `TDescriptorType` 修正为 `TDescriptor::Type` 声明。
>* `engine\core` 下 `TDescriptorSetLayout` 新增 `Layout` 子类声明。

## 2025/3/24

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。
>* `samples` 下更新 `TurboAPIDesign_Descriptor` 示例用于测试设计代码。

## 2025/3/22

>* `docs\Design\Design.md` 重命名为 `Descriptor` 设计 `Descriptor`。

## 2025/3/21

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。
>* `samples` 下更新 `TurboAPIDesign_Descriptor` 示例用于测试设计代码。

## 2025/3/20

>* `core\thirdparty` 下 `VulkanMemoryAllocator` 更新到最新版的 ``3.2.1`` 。
>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/19

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。
>* `samples` 下增加 `TurboAPIDesign_Descriptor` 示例用于测试设计代码。

## 2025/3/18

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/17

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/14

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/13

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/12

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/11

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/10

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/7

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/6

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/5

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/3/4

>* 更新 `docs\Design\Design.md` 设计 `Descriptor`。

## 2025/2/26

>* 更新 `docs\Memo.md` 研究 `描述符`。

## 2025/2/25

>* 更新 `README.en.md` 修正。
>* 更新 `docs\Memo.md` 研究 `描述符`。

## 2025/2/24

>* 更新 `README.en.md` 增加英文版的 `README`。
>* 更新 `docs\Memo.md` 研究 `描述符`。

## 2025/2/17

>* `samples` 下更新 ``SpirvReflectTest`` 示例。用外部参数传入着色器。

## 2025/2/14

>* 更新 `README.md` 。优化显示。

## 2025/2/13

>* `samples/FlowField` 下增加 ``ShowWIndByInstancedLines`` 示例。使用线条渲染全球风场。
>* 更新 `README.md` 。优化显示。

## 2025/2/12

>* `samples/utils` 下更新 ``Camera`` 示例。增加 ``View`` 和 ``Projection`` 的逆矩阵。
>* `samples/FlowField` 下更新 ``ShowWindByInstancedPoints`` 示例。增加地球模式显示。
>* `asset/shaders` 下增加 ``Test`` 文件夹，并创建 ``TexInVertexShader.vert`` 。测试顶点着色器中的纹理采样。

## 2025/2/11

>* `samples` 下更新 ``ShowWindByInstancedPoints`` 示例。用粒子的方式绘制风场。
>* `engine/core/thirdparty` 下新增 ``Vulkan-Headers`` 的 ``submodule`` 。
>* `samples` 下更新 ``SpirvReflectTest`` 尝试通过指令行传入待解析着色器文件。

## 2025/2/10

>* `samples/utils` 下增加 ``InfiniteCoordinateAxisPass`` 用于封装 ``无限坐标轴`` 渲染。
>* `samples/utils` 下增加 ``Camera`` 用于封装 ``相机`` 。
>* `core` 下 `TCommandBuffer` 增加 ``GetCurrentRenderPass()`` 函数。用于获取当前绑定的 `渲染管线` 。
>* `core` 下 `TBuffer` 增加 ``GetDevice()`` 函数。用于获取 ``缓存`` 的 ``逻辑设备`` 。
>* `samples` 下增加 ``InfiniteCoordinateAxisPassTest`` 示例。用于测试 ``InfiniteCoordinateAxisPass`` 。
>* `core` 下 `TPipeline` 更新 ``Valid()`` 判断逻辑。修正 ``Bug`` 。
>* `samples/FlowField` 下更新 ``ShowWindByPatch`` 示例。使用 ``InfiniteCoordinateAxisPass`` 。
>* `samples/FlowField` 下更新 ``ShowWindByPatch`` 示例。使用 ``GlobalWindPatchPass`` 封装关键代码 。

## 2025/2/9

>* `samples/FlowField` 下更新 ``ShowWindByPatch`` 示例。

## 2025/2/7

>* 适配 `VK_API_VERSION_VARIANT` 。
>* 适配 `VK_API_VERSION_MAJOR` 弃用 ``VK_VERSION_MAJOR``。
>* 适配 `VK_API_VERSION_MINOR` 弃用 ``VK_VERSION_MINOR``。
>* 适配 `VK_API_VERSION_PATCH` 弃用 ``VK_VERSION_PATCH``。
>* 更新 `TVersion` 适配 ``variant``。

## 2025/2/6

>* `.gitignore` 下增加对 ``.vscode/tasks.json`` 的版本追踪。
>* 将 `gizmo` 下的文件移动到 ``samples/utils`` 下。
>* `samples/FlowField` 下增加 ``ShowWindByPatch`` 示例。用面片的方式绘制风场。
>* `samples/FlowField` 下增加 ``ShowWindByInstancedPoints`` 示例。用粒子的方式绘制风场。
>* `engine/core` 下 ``TVulkanLoader`` 对于 ``设备函数`` 的获取，增加 ``Vulkan`` 实例版本的检查。
>* `engine/core` 下 ``TVersion`` 使用新版规定的 ``VK_MAKE_API_VERSION`` ，遗弃使用 ``VK_MAKE_VERSION`` 。

## 2025/1/29

>* `samples` 下更新 ``InfiniteCoordinateAxis`` 示例。

## 2025/1/28

>* 尝试将 ``Turbo`` 适配到 ``Linux`` 。
>* `samples` 下更新 ``VulkanTest`` 示例，适配 ``Linux`` 平台。输出更多 ``Vulkan`` 信息。
>* `samples` 下更新 ``InfiniteCoordinateAxis`` 示例，适配键鼠操作。

## 2025/1/27

>* 尝试将 ``Turbo`` 适配到 ``Linux`` 。

## 2025/1/24

>* `samples` 下更新 ``InfiniteCoordinateAxis`` 示例，用于尝试绘制一个 ``无限坐标系`` 。
>* `samples/utils` 下更新 ``ImGuiPass`` ，增加键盘输入支持。

## 2025/1/23

>* `engine\core\thirdparty` 下更新 ``ImGuiTest.cpp`` 示例，支持 ``鼠标滚轮`` 操作。
>* `samples` 下增加 ``utils`` 文件夹。用于存放 ``示例`` 常见通用程序代码。
>* `samples/utils` 下增加 ``ImGuiPass`` 接口，尝试将 ``ImGui`` 的代码封装到一个类中。
>* `samples` 下增加 ``ImGuiPassTest`` 示例，测试 ``ImGuiPass`` 接口。
>* `samples` 下增加 ``InfiniteCoordinateAxis`` 示例，用于尝试绘制一个 ``无限坐标系`` 。

## 2025/1/22

>* `engine\core\thirdparty` 下更新 ``SpirvReflectTest`` 示例，使其支持编译 ``HLSL`` 。研究 ``SPIRV-Reflect`` 使用。

## 2025/1/21

>* `engine\core\thirdparty` 下更新 ``SpirvReflectTest`` 实例。研究 ``SPIRV-Reflect`` 使用。

## 2025/1/20

>* `engine\core\thirdparty` 下更新 ``SpirvReflectTest`` 实例。研究 ``SPIRV-Reflect`` 使用。

## 2025/1/18

>* `engine\core\thirdparty` 下更新 ``SpirvReflectTest`` 实例。研究 ``SPIRV-Reflect`` 使用。

## 2025/1/16

>* `engine\core\thirdparty` 下更新 ``SpirvReflectTest`` 实例。研究 ``SPIRV-Reflect`` 使用。

## 2025/1/15

>* `engine\core\thirdparty` 下新增 ``SPIRV-Reflect`` 子模块。并将其集成到 ``TCore`` 核心中。
>* `samples` 下新增 ``SpirvReflectTest`` 示例。用于研究 ``SPIRV-Reflect`` 。

## 2025/1/14

>* `asset\shaders` 下新增 ``glslangTest.frag`` 。
>* `samples` 下更新 `GlslangTest` 示例。尝试解析 ``glslangTest.frag`` 。

## 2025/1/10

>* `asset\shaders` 下新增 ``glslangTest.vert`` 。
>* `samples` 下更新 `GlslangTest` 示例。尝试解析 ``glslangTest.vert`` 。

## 2025/1/9

>* `samples` 下增加 `ImGuiTest` 示例。用于渲染 ``ImGui`` 。
>* `samples` 下增加 `GlslangTest` 示例。用于研究 ``glslang`` 。

## 2025/1/8

>* `gizmo` 下增加 `ReadFile` 用于读取常见文本和二进制文件。
>* 更新 `global_wind.bin` 和与之相关的加载类`GlobalWind`，按照单精度浮点数进行读，加速读取速度。
>* 更新 `ShowWindByTex` ，按照图片纹理形式显示。

## 2025/1/7

>* `samples` 下所有示例增加 `TURBO_PROJECT_NAME` 宏。用于表示当前示例名称。
>* `TSurface` 的 `GetSurfaceFormats()` 函数重命名为 `GetSupportFormats()`。
>* `TCommandBuffer` 中移除 `TRefPtr<T>` 的冗余使用。
>* `TBarrier` 中移除 `TRefPtr<T>` 的冗余使用。
>* `TInfo` 及其子类中移除 `TRefPtr<T>` 的冗余使用。
>* `ShowWindByTex` 更新加载 ``global_wind.bin`` 到纹理中。
>* 增加 `SwapchainTest` 示例。用于测试 ``交换链`` 。

## 2025/1/6

>* `samples` 下增加 `VkFormatTest` 示例。用于研究测试 `VkFormat` 。
>* `TFormatInfo.cpp` 下移除 `std::vector<VkFormat> TAllFormats`。改用范围`format_ranges`遍历所有格式。
>* `samples` 下所有使用 `TCore` 头文件示例取消使用相对头文件路径。

## 2025/1/5

>* 尝试 `Linux` 平台编译。

## 2025/1/3

>* 修正 ``TCore`` 编译时依赖 ``glslang`` 库。
>* 所有示例都增加 ``TURBO_ASSET_ROOT`` 宏指向 `asset` 资源文件夹。自动化配置资源路径，不再需要手动配置了。
>* 增加 ``samples\FlowField`` 文件夹。用于存放流场相关示例。
>* ``FlowField`` 下增加 ``ShowWindByTex`` 示例。用于使用纹理显示 ``流场图``。
>* ``TInstance`` 增加引用版本构造。

## 2025/1/2

>* 修正 ``TInstance`` 中 ``GetPhysicalDevices()`` 返回数组引用的 ``Bug`` 。
>* 修正 ``std::vector`` 引用和非引用。查漏补缺。
>* ``TCommandBuffer``中增加设置单个 ``TViewport`` 和 ``TScissor`` 的 ``CmdSetViewport/Scissor(...)`` 函数版本。
>* 更新 ``CMakeLists.txt``中对于依赖库编译参数的设置。用户不再需要手动设置编译参数，``Turbo`` 自动配置了默认值。
>* 适配 ``glslang 15.1.0``
>* 适配 ``VulkanMemoryAllocator 3.2.0``

## 2024/12/31

>* 更新 ``TPipelineDescriptorSet`` 中增加对单一资源的绑定。
>* 更新 ``TRefPtr`` 中增加 ``PtrsToRefs<T>`` 函数。用于将指针型数组转成计数引用数组。
>* 修正 ``TPipelineDescriptorSet`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TPipelineLayout`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TDescriptorPool`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TFramebuffer`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TPipelineDescriptorSet`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TPipelineLayout`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TRenderingPipeline`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TRenderPass`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TSampler`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TSemaphore`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TShader`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TSurface`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TSwapchain`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TVmaAllocator`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TVulkanAllocator`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TVulkanLoader`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``sample`` 下示例，适配 ``TRefPtr`` 的冗余使用。

## 2024/12/30

>* 修正 ``TDescriptorSetLayout`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TDevice`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TRefPtr`` 中增加 ``RefsToPtrs<T>(...)`` 函数。用于将 ``vector<TRefPtr<T>>`` 转成 ``vector<T*>`` 。
>* 修正 ``TDeviceQueue`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TFence`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TFormatInfo`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TFramebuffer`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TGraphicsPipeline`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TImage`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TImageView`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TInstance`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TLayerInfo`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TPhysicalDevice`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TPipelineCache`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TPipelineDescriptorSet`` 中对于 ``TRefPtr`` 的冗余使用。

## 2024/12/28

>* 修正 ``TCommandBuffer`` 取消使用 ``模板`` （可读性较差）。
>* 修正 ``TDescriptorSet`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TDescriptorPool`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TDescriptor`` 中对于 ``TRefPtr`` 的冗余使用。

## 2024/12/27

>* 修正 ``TCommandBuffer`` 中对于 ``vector<TRefPtr<T>/T*>`` 使用，使用 ``模板`` 代替明确类型，类型推断交给编译器。
>* 修正 ``TCommandBufferPool`` 中对于 ``TRefPtr`` 的冗余使用。
>* 修正 ``TComputePipeline`` 中对于 ``TRefPtr`` 的冗余使用。

## 2024/12/24

>* ``asset`` 文件夹下增加 ``global_wind.bin``，该文件内存有全球流场数据。
>* 增加 ``gizmo`` 文件夹，用于存放乱七八糟的一些工具测试之类的代码。
>* ``gizmo`` 下增加 ``GlobalWind.h/.cpp`` 文件，用于读取 ``global_wind.bin`` 文件。
>* ``sample`` 下增加 ``ReadGlobalWind`` 示例，用于测试读取 ``global_wind.bin`` 文件。

## 2024/12/19

>* ``TReferenced`` 下增加 ``static bool Valid(const TReferenced* ref)`` 静态函数。用于指针有效性检查。
>* 修正 ``TCommandBuffer`` 中对 ``TRefPtr`` 的冗余使用。

## 2024/12/18

>* 将形参和返回中的 ``TRefPtr`` 去除掉，正常不需要强制为 ``TRefPtr`` 略显冗余繁琐。只需要成员变量为 ``TRefPtr`` 即可进行自动化内存管理。
>* 修正 ``TBarrier`` 中对 ``TRefPtr`` 的冗余使用。
>* 修正 ``TBuffer`` 中对 ``TRefPtr`` 的冗余使用。

## 2024/12/17

>* 将 ``point_cloud`` 分支合并到 ``dev`` 分支
>* 将 ``README`` 中的 ``Log`` 日志转移到此 ``更新日志`` 中。
>* 增加对 ``sample`` 下 ``VolumeTest`` 示例的编译输出。

## 归档历史 Log

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
  > `TSurface`中增加`TSurface(Turbo::Core::TDevice *device, wl_display *display, wl_surface *surface)`构造函数，用于适配`wayland`
  > `TSurface`中`InternalCreate()`适配`Linux`的`wayland`
  > `TSurface`中`GetSurfaceSupportQueueFamilys()`适配`Linux`的`wayland`

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

* 2022/8/4 设计架构
  >
  >* `./samples`中增加`PBRTest.cpp`,用于`physically based rendering`与`BSDF`研究

* 2022/8/7 设计架构
  >
  >* `./samples`中`PBRTest.cpp`,对于`physically based rendering`与`BSDF`初步调试成功。看样子渲染结果应该是对的`（○｀ 3′○）`
  >
* 2022/8/10 设计架构
  >
  >* 开始`engine`层面的设计和实现，`engine`层的相关设计将会位于`./docs/TurboDesign.drawio::Engine`章节中，`engine`层的相关实现将会放入`./engine/include`和`./engine/src`中

* 2022/8/11 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中创建`纲要`
  >* 修正`./docs/TurboDesign.drawio::Engine`章节中`Subapss`的问题

* 2022/8/13 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中创建`概要设计`

* 2022/8/15 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中完善`概要设计`设计思路

* 2022/8/16 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中`概要设计`设计增加对于`FrameGraph`结合的设计思路

* 2022/8/17 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中`概要设计`设计增加对于`CommandBuffer相关`的设计思路
  >* `./docs/TurboDesign.drawio::Engine`章节中增加`详细设计`
  >* `Turbo`核心`TCommandBuffer`中新增`TSecondaryCommandBuffer`类，用于表示二级指令缓冲
  >* `Turbo`核心`TCommandBuffer`中新增`TCommandBufferBase`类，用于表示指令缓冲的基类，用于实现指令。该类为虚基类
  >* `Turbo`核心`TCommandBuffer`中`TCommandBufferBase`类，增加`TCommandBufferLevel GetLevel();`函数，用于获取指令缓冲等级
  >* `Turbo`核心`TCommandBuffer`中新增`TCommandBufferLevel`枚举，用于表示指令缓冲的的级别
  >* `Turbo`核心`TCommandBuffer`中将`TCommandBuffer`修改成继承自`TCommandBufferBase`
  >* `Turbo`核心修改`TCommandBufferPool`中将取消继承自`Turbo::Core::TPool<TCommandBuffer>`
  >* `Turbo`核心`TCommandBufferPool`中增加`void Free(TCommandBufferBase *commandBufferBase);`用于统一回收一级、二级指令缓冲
  >* `Turbo`核心`TCommandBufferPool`中增加`TSecondaryCommandBuffer *AllocateSecondary();`和`void Free(TSecondaryCommandBuffer *secondaryCommandBuffer);`用于创建和回收二级指令缓冲
  >* `Turbo`核心`TCommandBuffer`中应该实现`void CmdExecuteCommand(...);`函数，用于一级指令缓冲调度二级指令缓冲。今天太晚了，明天再弄。
  
* 2022/8/18 设计架构
  >
  >* `Turbo`核心`TCommandBuffer`中增加`TCommandBufferPool* GetCommandBufferPool();`函数
  >* `Turbo`核心`TCommandBuffer`中实现`void CmdExecuteCommand(...);`函数
  >* `Turbo`核心`TCommandBufferBase`中增加`VkCommandBufferInheritanceInfo *vkCommandBufferInheritanceInfo = nullptr`成员变量，用于二级指令缓冲的继承信息
  >* `Turbo`核心`TCommandBufferBase`中增加`uint32_t currentSubpass = 0;`成员变量，表示当前的`Subpass`
  >* `Turbo`核心`TCommandBufferBase`中增加`TFramebuffer *currentFramebuffer = nullptr;`成员变量，表示当前的`Framebuffer`
  >* `Turbo`核心`TSecondaryCommandBuffer`中删除`void Begin() = delete`成员函数调用，转而使用`void Begin(TRenderPass *renderPass, TFramebuffer *framebuffer, uint32_t subpass);`成员函数
  >* `Turbo`核心`TCommandBuffer.h`中增加`typedef enum class TSubpassContents`枚举，用于对应`VkSubpassContents`
  >* `Turbo`核心`TCommandBufferBase`中将`void CmdBeginRenderPass(...)`的形参增加一个`TSubpassContents`参数
  >* `Turbo`核心`TCommandBufferBase::Begin()`中将`vk_command_buffer_begin_info.flags`设置成`VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT`为适应二级指令缓冲
  >* `./samples`中增加`SecondaryCommandBufferTest`例子，用于测试二级指令缓冲

* 2022/8/18 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中`详细设计`设计,细化具体资源

* 2022/8/19 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中`系统设计`中增加文件加载层相关
  >* `./asset/models/`增加`material_sphere.gltf`文件
  >* `./samples/PBRTest.cpp`例子中将读取的模型改为`material_sphere.gltf`文件

* 2022/8/20 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中`详细设计`中增加`Material`设计思路
  >* `./samples`中增加`MultiDrawTest.cpp`文件，用于探究`Subpass`和`MutilDraw`之间的联系
  >* `./samples`中增加`SubpassTest.cpp`文件，用于探究`Subpass`和`MutilDraw`之间的联系

* 2022/8/21 设计架构
  >
  >* `./docs/TurboDesign.drawio::Engine`章节中`详细设计::Material`设计思路,有设计问题，进一步设计完善

* 2022/8/25 知会
  >
  >* 现在`Turbo::Engine`层面的设计多少都有些问题，比如：如果用户想要调配使用`FrameGraph`就需要对渲染有过硬的理解，目前看了[`Filament`](https://github.com/google/filament)，[`LegitEngine`](https://github.com/Raikiri/LegitEngine),[`FrameGraph-Example`](https://github.com/skaarj1989/FrameGraph-Example)等相关引擎对于`FrameGraph`亦或是`RenderGraph`的封装设计，这些引擎都会提供一套固定的`FrameGraph`，用于一套已经定义好了的渲染流程，这些流程多是`GBuffer`流程，用户提供`Renderable`结构数据，引擎将其塞入特定的`PassNode`中，这个塞入工作并不是用户自定义的，而是引擎完成的，这就会导致用户（对图形学不是很熟悉）需要按照引擎先前定好的流程渲染，渲染出有限的效果（大部分情况下这个有限的效果是够用的），除非用户对于图形学和`FrameGraph`有比较深刻的理解，则可自己使用`FrameGraph`配置自定义渲染流程。
  >* 从现在开始要全力备考了，再写代码我妈要打死我了，不管考得上，还是考不上，明年引擎都将继续，回见~

* 2022/9/4 设计架构
  >
  >* 感谢`会翔`提供的反馈。修复`TCommandBufferBase::CmdSetLineWidth(...)`时程序报错的问题，原因是`TGraphicsPipeline::InternalCreate()`在创建管线时`VkPipelineDynamicStateCreateInfo::dynamicStateCount`给个固定数值`2`,导致`VkDynamicState::VK_DYNAMIC_STATE_LINE_WIDTH`并不会加入动态状态中。

* 2022/10/16 设计架构
  >
  >* `./samples`中增加`InstancedDrawTest`示例，用于示例`实例化渲染`

* 2022/11/7 设计架构
  >
  >* `./samples`中增加`PerlinWorleyNoiseTest`示例，用于尝试生成`Perlin-Woley噪音`

* 2022/11/9 设计架构
  >
  >* `core`中创建`TComputerPipeline`类

* 2022/11/10 设计架构
  >
  >* `TurboDesign`中`Core`章节增加`Pipeline与Shader分支箭头（非常大的黄色箭头）`
  >* `TShader`中增加`TVertexShader`、`TFragmentShader`、`TComputeShader`并在`TShader.cpp`中实现（目前先实现这几个，剩下等有时间的）
  >* `TPipeline`中增加图形管线的构造函数`TPipeline(TDevice *device, TVertexShader* vertexShader,TFragmentShader* fragmentShader);`和计算管线的构造函数`TPipeline(TDevice *device, TComputeShader *computeShader);`。老的构造函数将会声明置成弃用函数。
  >* `TGraphicsPipeline`中增加使用`TVertxShader`和`TFragmentShader`的构造函数。老的构造函数将会声明置成弃用函数。
  >* 实现`TComputerPipeline`中并在其中调用`vkCreateComputePipelines`创建计算管线的`VkPipeline`
  >* `./samples`中增加`CineShaderLava`示例，在[`ShaderToy`](https://www.shadertoy.com/view/3sySRK)上看到的，感觉挺有意思，想试试使用`Turbo`渲染，就搬过来了。

* 2022/11/11 设计架构
  >
  >* `./samples`中增加`Octagrams`示例，在[`ShaderToy`](https://www.shadertoy.com/view/tlVGDt)上看到的，感觉挺有意思，就搬过来了。
  >* `./samples`中增加`ProteanClouds`示例，在[`ShaderToy`](https://www.shadertoy.com/view/3l23Rh)上看到的，感觉挺有意思，就搬过来了。
  >* 开始实现`Core`中`TCommandBufferBase`中的`void CmdDispatch(...)`函数，用于调用执行计算着色器的计算管线
  >* `./samples`中增加`ComputePipelineTest`测试示例，用于测试计算着色器和计算管线是否正确有效
  >* 引擎中目前没有`storage image`相关的解析，现进行实现。
  >* `TDescriptor`中增加`TStorageImageDescriptor`类，继承自`TDescriptor`,并实现
  >* `TShader`中增加`TStorageImageDescriptor`类的数组使用：声明`std::vector<TStorageImageDescriptor *> storageImageDescriptors`成员变量。
  >* `TShader`中增加`const std::vector<TStorageImageDescriptor *> &GetStorageImageDescriptors()`成员函数。
  >* `TShader`的`InternalParseSpirV()`成员函数中对`storage image`进行解析构建,并在`TShader`析构时销毁。
  >* `TPipeline`的`InternalCreate()`成员函数中对`storage image`进行解析添加。
  >* 在`TDescriptorSet`的`BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TImageView *> &imageViews)`成员函数中增加对`storage image`的支持。

* 2022/11/12 设计架构
  >
  >* 加了个`爱发电`赞助，感谢投喂。
  >* `./samples`中增加`Auroras`示例，在[`ShaderToy`](https://www.shadertoy.com/view/XtGGRt)上看到的，太漂亮了没忍住，又搬过来了。
  >* `./samples`中`ProteanClouds`示例中增加对应的鼠标位置数据

* 2022/11/13 设计架构
  >
  >* `./samples`中增加`ComputerPipelineGeneratePerlinWorley`示例。用于使用计算着色器生成128×128×128的3维`PerlinWorley噪音`，之后再使用图形管线显示。
  >* `./samples`中增加`ComputerPipelineGenerateWorley`示例。用于使用计算着色器生成32×32×32的3维`Worley噪音`，之后使用图形管线显示。
  >* `./samples`中增加`IspGlassball11Mix`示例。在[`ShaderToy`](https://www.shadertoy.com/view/NscXRj)上看到的，进行了一些魔改，没有进行降噪，原版代码好像是用的前一帧的数据进行降噪的，具体没咋看(主要是懒的写代码)。
  >* 接下来计划重点实现`Volumetric Cloud`，之前一直想尝试实现体积云，该体积云将会写较为详细的实现文档，敬请期待。

* 2022/11/14 设计架构
  >
  >* `./samples`中增加`VolumetricCloud`示例。用于尝试实现体积云
  >* `./asset/shaders`中增加`imgui.vert`顶点着色器文件
  >* `./asset/shaders`中增加`imgui.frag`片元着色器文件
  >* `./asset/shaders`中增加`perlin-worley.comp`计算着色器文件
  >* `./asset/shaders`中增加`worley.comp`计算着色器文件
  >* `./asset/shaders`中增加`post_processing.vert`顶点着色器文件
  >* `./asset/shaders`中增加`post_processing_voulmetric_cloud.frag`片元着色器文件
  >* `./docs`中增加`VolumetricCloud.md`用于书写体积云的实现说明文档
  >* `TVulkanLoader`中增加`Linux`平台的支持
  >* `Deepin`环境好像不支持`C++20`和`c++17`，现已退回到`c++11`版本
  >* 由于退回到`c++11`版本，`TFrameGraph`中的一些`c++`新特性将会退回老版本，目前`TFrameGraph`不成熟，有待完善或重构，目前不是非常重要。

* 2022/11/15 设计架构
  >
  >* 修改`TVulkanLoader`中增加`Linux`头文件`dlfcn.h`忘添加平台宏限制的Bug
  >* 修改`CppStandardTest.cpp`中代码，由于退回到了`C++11`
  >* `PerlinWorleyNoiseTest.cpp`中向着色器传递增加一个`float h`数据，用于控制噪音算法的赫斯特指数（Hurst Exponent）
  >* `README`中`赞助`栏目增加`体积云项目`
  >* 修改`TSwapchain.cpp`中`InternalCreate()`中`vk_swapchain_create_info_khr.oldSwapchain=nullptr`改成`vk_swapchain_create_info_khr.oldSwapchain=VK_NULL_HANDLE`，`Visual Studio 2017`中赋`nullptr`报错
  >* 修改`TFrameGraph.hpp`中`Execute`的相关声明，声明成`TExecute`,在使用`Visual Studio 2017`编译`Turbo`时会有报`error C2244 无法将函数定义与现有的声明匹配`，估计是名称和微软的库冲突了，换了名。`TFrameGraph`有时间要大修
  >* 修改所有示例中有关寻找是否支持`VK_LAYER_KHRONOS_VALIDATION`验证层相关代码。如果设备支持将该验证层加入`TInstance`的激活数组中，有个Bug是当电脑不支持时或`engine.GetInstance().GetSupportLayers()`返回数据有问题，将会造成不应该添加`Turbo::Core::TLayerInfo khronos_validation`(此时`khronos_validation`还是未赋值状态，内部类型为`TLayerType::UNDEFINED`)到最后还是会调用`enable_layer.push_back(khronos_validation)`增加这个内部类型为`TLayerType::UNDEFINED`的验证层，导致在创建`TInstance`时不识别该验证层，返回异常。
  >* 增加一个`Issue`:在`Windows11`下，英伟达驱动`472.91`，显卡型号`RTX3070`下`std::vector<Turbo::Core::TLayerInfo> Turbo::Core::TLayerInfo::GetInstanceLayers()`将会返回空数组，`Vulkan`运行时此时找不到支持的验证层。
  >* `./docs`中增加`images`文件夹，用于存放文档图片

* 2022/11/16 设计架构
  >
  >* `./doc/images`增加`Hurst_Exponent.gif`文件，用于体积云说明
  >* 更新`./docs/VolumetricCloud.md`文件
  >* `./docs/images`增加`base_cloud.png`文件和`detail_cloud.png`文件
  >* `./samples`增加`RayMarchingTest`示例，展示一个基本的光线步进示例，该示例未完成。
  >* `./asset/shaders`增加`base_ray_marching.frag`着色器，用于基本光线步进。

* 2022/11/17 设计架构
  >
  >* 修改`main.cpp`使其输出的`Turbo.exe`。在当运行时按下键盘`W`、`A`、`S`、`D`后可以移动相机，按住鼠标`右键`可以俯仰相机。
  >* 修改`main.cpp`中的`eye`变量名称，起错名了。

* 2022/11/18 设计架构
  >
  >* 完成`./sample`下的`RayMarchingTest`示例
  >* `./docs`下增加`compile_env.png`，此为`Windows`环境下编译`Turbo`的环境对照表。`KTX-Software`很早之前的[`Issue`](https://github.com/KhronosGroup/KTX-Software/pull/574)，一直存在，可能`GNU`标准的`clang/clang++`不是这么用的吧。
  >* 增加`README.txt`中第三方库的链接。`glm`有个历史遗留库，我下错好几次了。
  >* 更新`VolumetricCloud`文章

* 2022/11/19 设计架构
  >
  >* `./docs`增加`camera_coordinate.png`。用于`VolumetricCloud.md`文章说明
  >* `./docs`增加`camera_relative_pos.png`。用于`VolumetricCloud.md`文章说明
  >* `./docs`增加`camera_pixel_pos.png.png`。用于`VolumetricCloud.md`文章说明
  >* `./docs`增加`ndc.png`。用于`VolumetricCloud.md`文章说明
  >* `./docs`增加`base_ray_marching.gif`。用于`VolumetricCloud.md`文章说明
  >* 更新`VolumetricCloud`文章
  >* `./asset/shaders`增加`volumetric_cloud.frag`并对应修改`VolumetricCloud`示例。计划用于实现`体积云`，将会随着`VolumetricCloud.md`文章的进度完善

* 2022/11/20 设计架构
  >
  >* `./samples`增加`RayMarchingBoundingBoxTest`。用于`VolumetricCloud`的碰撞包围盒(为什么会有一圈圈的、类似摩尔纹的东西？光追噪音吗？)。
  >* `./asset/shaders`增加`ray_marching_bounding_box.frag`着色器。
  >* `RayMarchingTest`实例增加界面文字操作提示。

* 2022/11/21 设计架构
  >
  >* `./asset/shaders`中将`ray_marching_bounding_box.frag`的`BoudingBoxOrthogonalBasis GetBoundingBoxForwardDir(...)`函数修改成`GetBoundingBoxOrthogonalBasis(...)`，之前名字起错了
  >* `./asset/shaders`中通过修改`ray_marching_bounding_box.frag`的`bool IsPointInBoundingBox(vec3 point, BoundingBox boundingBox)`函数，增加一小点包围盒长宽高，用于补偿浮点数计算精度，进而改善了噪点，之前的精度太高了，导致生成了噪点

* 2022/11/22 设计架构
  >
  >* `./docs`增加`compiler_env_version.png`用于标识编译环境，`Windows`系统中，该环境在`LLVM`新版本下编译不了，需要导成`Visual Studio`能够顺利编译，`Linux`不变
  >* `./docs`增加`raymarching.png`。用于`VolumetricCloud.md`文章说明
  >* `./docs`增加`ray_surface_intersect.png`。用于`VolumetricCloud.md`文章说明
  >* 更新`VolumetricCloud`文章

* 2022/11/23 设计架构
  >
  >* 修改`TFrameGraph.hpp`中的`Execute`命名，在`Visual Studio`编译器下会冲突，改成`TExecute`，
  >* 修改`TFrameGraph.hpp`中`C++17`的`[[nodiscard]]`目前`Turbo`使用`C++11`
  >* `/.gitignore`中增加忽略`install/`文件夹

* 2022/11/24 设计架构
  >
  >* 更新`VolumetricCloud`文章
  >* 更改`./samples`下的`VolumetricCloud`，使其将`ComputerPipelineGeneratePerlinWorley`和`RayMarchingBoundingBoxTest`相结合

* 2022/11/25 设计架构
  >
  >* `./docs/images`增加`intersect_case_out_box_forward.png`用于`VolumetricCloud.md`文章说明
  >* `./docs/images`增加`intersect_case_in_box.png`用于`VolumetricCloud.md`文章说明
  >* `./docs/images`增加`intersect_case_out_box_backward.png`用于`VolumetricCloud.md`文章说明
  >* `./docs/images`增加`ray_marching_bounding_box.gif`用于`VolumetricCloud.md`文章说明
  >* 更新`VolumetricCloud`文章

* 2022/11/26 设计架构
  >
  >* `./docs/images`增加`bounding_box_sample_space.png`用于`VolumetricCloud.md`文章说明
  >* `./docs/images`增加`ray_marching_bounding_box_sample_space.gif`用于`VolumetricCloud.md`文章说明
  >* `./samples`下的`VolumetricCloud`实例中增加`delete my_computer_shader`。
  >* 更新`VolumetricCloud`文章
  >* `./samples`下增加`RayMarchingPerlinWorleyNoise`实例，用于展示光线步进中基于包围盒对柏林沃利噪音的采样。
  >* `./asset/shaders`下增加`ray_marching_perlin_worley_noise.frag`片元着色器，并使用于`RayMarchingPerlinWorleyNoise`实例中

* 2022/11/29 设计架构
  >
  >* `./samples`中`VolumetricCloud`中增加`power`、`absorption`、`outScattering`传输变量，用于体积云渲染
  >* `./samples`中`VolumetricCloud`中增加对高频沃利纹理的采样

* 2022/12/1 设计架构
  >
  >* 目前`VolumetricCloud`体积云渲染正在研究光照散射传输，研究周期可能会较长，先重构`FrameGraph`
  >* 对于`Deepin`环境下`C++17/20`的支持一直没有很好的解决方式，有可能`LLVM`环境没有配对，研究研究。
  >* 将`TFrameGraph`下的`*Agency`重命名成`*Proxy`

* 2022/12/2 设计架构
  >
  >* 将`TFrameGraph`下的`TURBO_NVALID_ID`重命名成`TURBO_INVALID_ID`
  >* 将`TFrameGraph`下的`Data`重命名成`TData`
  >* 将`TFrameGraph`下的`Setup`重命名成`TSetup`
  >* 修缮`TFrameGraph`
  >* `./docs`下增加`Desgin`文件夹
  >* `./docs/Desgin/images`下增加`fg1~18`文件，记录平日的`FrameGraph`的琐碎设计
  >* `./docs`下增加`linux_env`文件，记录`linux`上的依赖库（随便扯了张纸，有点脏，噗哈哈）
  >* `./docs/Desgin`下增加`FrameGraphAdvance.md`文件，用于整理位于`Turbo`教上层的`FrameGraph`和`Turbo`引擎衔接设计，相关设计源头位于`./docs/Desgin/images`下的一些琐碎设计

* 2022/12/4 设计架构
  >
  >* `./docs/Desgin`下`FrameGraphAdvance.md`基本完成，之后新的设计都会在此文档中增加
  >* 将`./docs/TurboDesign`中的`Engine`模块重命名成`Render`(考虑到将来可能会集成实现`ECS`)
  >* `./engine`下新建`render`文件夹，并在`render`文件夹下创建`include`和`src`文件夹
  >* `./engine/render`下创建`TImage`类

* 2022/12/5 设计架构
  >
  >* `FrameGraph`下对于资源的创建和销毁需要一个分配器或上下文
  >* `./docs/Desgin`下`FrameGraphAdvance.md`更新设计

* 2022/12/6 设计架构
  >
  >* `./engine/render`下新建`TContext`类，用于引擎上下文
  >* `./samples`下新建`RenderDesignTest`示例，用于`Turbo::Render`架构的测试

* 2022/12/7 设计架构
  >
  >* `./docs/Desgin`下`FrameGraphAdvance.md`更新设计
  >* `./engine/render`下`TContext`中增加`TInstance`初始化构建
  >* `./engine/render`下`TContext`中增加`TDevice`初始化构建

* 2022/12/8 设计架构
  >
  >* `./docs/Desgin`下`FrameGraphAdvance.md`更新设计

* 2022/12/9 设计架构
  >
  >* `./docs/Desgin`下`FrameGraphAdvance.md`更新设计
  >* `./engine/render`下新建`TSurface`类
  >* 重命名`Turbo`中所有的头文件定义宏，防止冲突

* 2022/12/10 设计架构
  >
  >* `./engine/render`下`TContext`增加`Turbo::Core::TImage *CreateImage(...)`函数
  >* `./engine/render`下`TImage`增加`typedef enum TImageCreateFlagBits`用于创建`TImage`的描述的位域标示
  >* `./engine/render`下`TImage`增加`using TImageCreateFlags = uint32_t`用于创建`TImage`的描述的位域标示集
  >* `./engine/render`下`TImage`的`Descriptor`增加`TImageCreateFlags flags`成员
  >* `./engine/render`下`TImage`增加`typedef enum TImageUsageBits`用于创建`TImage`的描述的`usage`位域标示
  >* `./engine/render`下`TImage`增加`using TImageUsages = uint32_t;`用于创建`TImage`的描述的`usage`位域标示集
  >* `./engine/render`下`TImage`的`Descriptor`增加`TImageUsages usages`成员
  >* 更新`./engine/render`下`TImage`及其子类的`Descriptor`成员
  >* 更新`./engine/render`下`TImage`其子类的`Create()`和`Destroy()`成员函数，增加`allocator`
  >* 更新`./engine/render`下`TContext`的`CreateImage()`函数
  >* `./engine/render`下新建`TResourceAllocator`类，用于资源创建的分配器

* 2022/12/11 设计架构
  >
  >* 更新`./docs/Desgin`下`FrameGraphAdvance.md`设计
  >* `./engine/render`下增加`TDomain.h`，用于定义资源分配的所有者端域
  >* `./engine/render`下`TImage`的`Descriptor`增加`TDomain domain`成员
  >* 更新`./engine/render`下`TContext`的`CreateImage()`函数

* 2022/12/12 设计架构
  >
  >* 更新`./docs/Desgin`下`FrameGraphAdvance.md`设计
  >* `./samples`下创建`VulkanTest`示例，用于纯`Vulkan`代码测试

* 2022/12/13 设计架构
  >
  >* 更新`./engine/render`下`TContext`的`CreateImage()`函数
  >* 更新`./engine/render`下`TImage`中`TImageUsageBits`枚举成员命名增加`..._ATTACHMENT`明确用意

* 2022/12/14 设计架构
  >
  >* 更新`./docs/Desgin`下`FrameGraphAdvance.md`设计
  >* 更新`readme`文档开头的`QQ`群链接，一开始链接放错了，放成爱发电赞助链接的交流群了

* 2022/12/15 设计架构
  >
  >* `./engine/core`下`TImage`内增加`void *vmaAllocationInfo`成员变量，用于存储`vma`内存分配信息，并在`构造函数中初始化，并赋值`，`析构`函数中释放
  >* `./engine/core`下`TImage`的`构造函数`中获取`VmaAllocationInfo`的`vma`内存分配信息
  >* `./engine/core`下`TBuffer`内增加`void *vmaAllocationInfo`成员变量，用于存储`vma`内存分配信息，并在`构造函数中初始化，并赋值`，`析构`函数中释放
  >* `./engine/core`下`TBuffer`的`构造函数`中获取`VmaAllocationInfo`的`vma`内存分配信息
  >* `./engine/core`下`TImage`中增加`TMemoryTypeInfo GetMemoryTypeInfo()`成员函数，用于获取底层数据的内存属性
  >* `./engine/core`下`TBuffer`中增加`TMemoryTypeInfo GetMemoryTypeInfo()`成员函数，用于获取底层数据的内存属性
  >* `./engine/render`下`TContext`中根据`FrameGraphAdvance.md`设计文档，更新实现`Turbo::Render::TContext::CreateImage(...)`成员函数
  >* `./engine/render`下`TDomainBits`中增加一个`BOTH`位标，用于表示`CPU+GPU`，方便使用

* 2022/12/16 设计架构
  >
  >* `./engine/render`下`TFormat`内增加`R8G8B8A8_SRGB`、`B8G8R8A8_UNORM`、`D16_UNORM`、`R8G8B8_SRGB`、`B8G8R8_SRGB`、`R8G8B8_UNORM`、`B8G8R8_UNORM`枚举成员
  >* `./engine/render`下`TContext`内增加`Turbo::Render::TContext::CreateImage(...)`增加对新增格式枚举成员的适配
  >* `./engine/render`下`TContext`的`Turbo::Render::TContext::CreateImage(...)`中对于`Turbo::Core::TImageType`的判断逻辑上有问题，修复
  >* `./engine/core`下`TCore`增加`typedef VkFormatProperties TFormatProperties`定义
  >* `./engine/core`下`TFormatInfo`增加`TFormatProperties formatProperties`成员变量
  >* `./engine/core`下`TFormatInfo`新增构造函数`TFormatInfo(TFormatType formatType = TFormatType::UNDEFINED, TFormatProperties formatProperties)`
  >* `./engine/core`下`TFormatInfo`构造函数`TFormatInfo(TFormatType formatType)`设置成遗弃函数
  >* `./engine/core`下`TFormatInfo`修改`Turbo::Core::TFormatInfo::GetSupportFormats()`函数适配`TFormatInfo`带有`TFormatProperties formatProperties`参数函数。
  >* `./engine/core`下`TPhysicalDeviceInfo`修改`std::vector<TFormatInfo> supportFormats`成员变量为`std::map<TFormatType, TFormatInfo> supportFormats`参数函数。
  >* `./engine/core`下`TPhysicalDevice`修改`void Turbo::Core::TPhysicalDevice::EnumerateFromat()`成员函数，为适配`std::map<TFormatType, TFormatInfo> TPhysicalDeviceInfo::supportFormats`参数函数。
  >* `./engine/core`下`TPhysicalDevice`修改`Turbo::Core::TPhysicalDevice::GetSupportFormats()`成员函数
  >* `./engine/core`下`TPhysicalDevice`修改`Turbo::Core::TPhysicalDevice::IsSupportFormat()`成员函数

* 2022/12/17 设计架构
  >
  >* `./engine/core`下`TPhysicalDevice`修改`Turbo::Core::TPhysicalDevice::IsSupportFormat()`成员函数
  >* `./engine/core`下`TFormatInfo`的`TFormatInfo(TFormatType formatType = TFormatType::UNDEFINED);`构造函数移除
  >* `./engine/core`下`TSwapchain`增加`TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatType formatType, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped)`构造函数，并实现
  >* `./engine/core`下`TSwapchain`增加`explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatType formatType, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped);`构造函数，并实现
  >* `./engine/core`下`TSwapchain`的`explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t width, uint32_t height, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, TSurfaceTransformBits transform, TCompositeAlphaBits compositeAlpha, TPresentMode presentMode, bool isClipped);`构造函数标记为遗弃
  >* `./engine/core`下`TSwapchain`的`explicit TSwapchain(TSurface *surface, uint32_t minImageCount, Turbo::Core::TFormatInfo format, uint32_t imageArrayLayers, Turbo::Core::TImageUsages usages, bool isClipped);`构造函数标记为遗弃
  >* `./engine/core`下`TPhysicalDevice`增加`TFormatInfo GetFormatInfo(TFormatType formatType);`函数，用于返回更加丰富的格式信息
  >* `./engine/core`下`TImageView`增加`explicit TImageView(TImage *image, TImageViewType viewType, TFormatType formatType, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)`构造函数
  >* `./engine/core`下`TImageView`下`explicit TImageView(TImage *image, TImageViewType viewType, TFormatInfo format, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)`构造函数标记为遗弃
  >* `./engine/core`下`TImage`下`[[deprecated]] explicit TImage(TDevice *device, VkImageCreateFlags imageFlags, TImageType type, TFormatInfo format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout = TImageLayout::UNDEFINED);`构造函数标记为遗弃
  >* `./engine/core`下`TImage`增加`explicit TImage(TDevice *device, VkImageCreateFlags imageFlags, TImageType type, TFormatType formatType, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevels, uint32_t arrayLayers, TSampleCountBits samples, TImageTiling tiling, TImageUsages usages, TMemoryFlags memoryFlags, TImageLayout layout = TImageLayout::UNDEFINED);`构造函数
  >* `./engine/core`下`TGraphicsPipeline`的`TVertexBinding`增加`void AddAttribute(uint32_t location, TFormatType formatType, uint32_t offset);`成员函数
  >* `./engine/core`下`TGraphicsPipeline`的`TVertexBinding`增加`void AddAttribute(uint32_t location, TFormatType formatType, uint32_t offset);`成员函数标记为遗弃
  >* `./engine/core`下`TGraphicsPipeline`的`TVertexAttribute`的`TFormatInfo format`成员变量更改为`TFormatType formatType`
  >* `./engine/core`下`TGraphicsPipeline`的`TVertexAttribute`的`TFormatInfo GetFormat();`成员函数更改为`TFormatType GetFormatType()`并实现
  >* `./engine/core`下`TGraphicsPipeline`的`TVertexAttribute`增加`TVertexAttribute(uint32_t location, TFormatType formatType, uint32_t offset);`构造函数
  >* `./engine/core`下`TGraphicsPipeline`的`TVertexAttribute`的`TVertexAttribute(uint32_t location, TFormatInfo format, uint32_t offset)`构造函数移除
  >* `./engine/core`下`TFormatInfo`中增加如下成员函数:
  >
  >   ```CXX
  >   bool IsSupportBuffer();
  >   bool IsSupportVertexBuffer();
  >   bool IsSupportLinearTiling();
  >   bool IsLinearTilingSupportSampledImage();
  >   bool IsLinearTilingSupportStorageImage();
  >   bool IsLinearTilingSupportStorageImageAtomic();
  >   bool IsLinearTilingSupportColorAttachment();
  >   bool IsLinearTilingSupportColorAttachmentBlend();
  >   bool IsLinearTilingSupportDepthStencilAttachment();
  >   bool IsLinearTilingSupportBlitSrc();
  >   bool IsLinearTilingSupportBlitDst();
  >   bool IsLinearTilingSupportSampledImageFilterLinear();
  >   bool IsLinearTilingSupportTransferSrc();
  >   bool IsLinearTilingSupportTransferDst();
  >   bool IsSupportOptimalTiling();
  >   bool IsOptimalTilingSupportSampledImage();
  >   bool IsOptimalTilingSupportStorageImage();
  >   bool IsOptimalTilingSupportStorageImageAtomic();
  >   bool IsOptimalTilingSupportColorAttachment();
  >   bool IsOptimalTilingSupportColorAttachmentBlend();
  >   bool IsOptimalTilingSupportDepthStencilAttachment();
  >   bool IsOptimalTilingSupportBlitSrc();
  >   bool IsOptimalTilingSupportBlitDst();
  >   bool IsOptimalTilingSupportSampledImageFilterLinear();
  >   bool IsOptimalTilingSupportTransferSrc();
  >   bool IsOptimalTilingSupportTransferDst();
  >   ```
  >
  >* 更新`./docs/Desgin`下`FrameGraphAdvance.md`设计

* 2022/12/18 设计架构
  >
  >* 更新`./docs/Desgin`下`FrameGraphAdvance.md`设计
  >* `./docs`下增加`vkspec1.3Core.pdf`文件，为`Vulkan1.3`官方标准文档，国内在线访问`Vulkan`官网真难受。
  >* `./engine/render`下`TFormat`中增加`UNDEFINED`枚举成员
  >* `./engine/render`下将`TFormat`中的枚举成员的值与`Vulkan`标准一一对应上
  >* `./engine/render`下将`TContext`下增加如下函数并实现
  >
  >   ```CXX
  >   Turbo::Core::TInstance *GetInstance();
  >   Turbo::Core::TPhysicalDevice*GetPhysicalDevice();
  >   Turbo::Core::TDevice *GetDevice();
  >   Turbo::Core::TDeviceQueue *GetDeviceQueue();
  >   ```
  >
  >* `./engine/render`下`TResourceAllocator`下增加`TContext* GetContext()`函数并实现
  >* `./engine/render`下`TColorImage`下`Turbo::Render::TColorImage::Create()`函数更新实现
  >* `./engine/render`下`TImage`下增加`class TDepthStencilImage`
  >* `./engine/render`下`TImage`下增加`class TDepthImage`
  >* `./engine/render`下`TImage`下增加`class TDepthImage2D`

* 2022/12/19 设计架构
  >
  >* 更新`./docs/Desgin`下`FrameGraphAdvance.md`设计
  >* `./engine/render`下`TImage`下增加`class TTexture2D`
  >* `./engine/render`下`TImage`下增加`class TColorImage3D`
  >* `./engine/render`下`TImage`下增加`class TTexture3D`
  >* `readme`增加平台支持标签
  >* `./docs/Desgin`下新增`WebGPU_Dawn.md`文档，用于记录[Google Dawn, a WebGPU implementation](https://dawn.googlesource.com/dawn/)学习记录

* 2022/12/20 设计架构
  >
  >* 🐑了，好难受。(눈_눈)
  >* `./engine/render`下`TImage`下增加`class TDepthTexture2D`
  >* `./engine/render`下`TImage`下增加`TFormat GetFormat()`成员函数
  >* `./engine/render`下`TImage`下增加`uint32_t GetWidth()`成员函数
  >* `./engine/render`下`TImage`下增加`uint32_t GteHeight()`成员函数
  >* `./engine/render`下`TImage`下增加`uint32_t GetDepth()`成员函数
  >* `./engine/render`下`TImage`下增加`uint32_t GetLayers()`成员函数
  >* `./engine/render`下`TImage`下增加`uint32_t GetMipLevels()`成员函数
  >* `./engine/render`下`TImage`下增加`TImageUsages GetUsage()`成员函数
  >* `./engine/render`下`TImage`下增加`TDomain GetDomain()`成员函数

* 2022/12/21 设计架构
  >
  >* `./engine/render`下增加`TBuffer`类
  >* `./engine/render`下`TContext`下增加`Turbo::Core::TBuffer *CreateBuffer(const TBuffer::Descriptor &descriptor)`成员函数
  >* `./engine/render`下`TContext`下增加`void DestroyBuffer(Turbo::Core::TBuffer *buffer)`成员函数
  >* `./engine/render`下`TResourceAllocator.h`下增加`Turbo::Core::TBuffer *CreateBuffer(const TBuffer::Descriptor &descriptor)`成员函数
  >* `./engine/render`下`ResourceAllocator.h`下增加`void DestroyBuffer(Turbo::Core::TBuffer *buffer)`成员函数

* 2022/12/23 设计架构
  >
  >* `./docs/Desgin`下将`WebGPU_Dawn.md`重命名为`WebGPU_CCPP.md`
  >* 更新`./docs/Desgin`下`WebGPU_CCPP`
  >* `./engine/render`下`TBuffer::Descriptor::usages`类型由原来的`TBufferUsageBits`改为`TBufferUsages`
  >* `./engine/render`下`TBuffer`中增加`TBufferUsages GetUsages()`成员函数
  >* `./engine/render`下`TBuffer`中增加`uint64_t GetSize()`成员函数
  >* `./engine/render`下`TBuffer`中增加`TDomain GetDomain()`成员函数
  >* `./engine/render`下`TImage`中`TImageUsages GetUsage()`成员函数更名为`TImageUsages GetUsages()`

* 2022/12/24 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`

* 2022/12/25 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./engine/render`下`TBuffer`中增加`void Copy(void *src, uint64_t size)`成员函数
  >* `./engine/render`下`TBuffer`中增加`void *allocator = nullptr`成员变量

* 2022/12/26 设计架构
  >
  >* 更新`./docs/Desgin`下`WebGPU_CCPP.md`
  >* `./engine/render`下`TContext`中增加`Turbo::Core::TCommandBufferPool *commandBufferPool = nullptr`成员变量
  >* `./engine/render`下`TContext`中增加`Turbo::Core::TCommandBuffer *AllocateCommandBuffer()`成员函数
  >* `./engine/render`下`TContext`中增加`void FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer)`成员函数
  >* `./engine/render`下`TResourceAllocator`中增加`Turbo::Core::TCommandBuffer *AllocateCommandBuffer()`成员函数
  >* `./engine/render`下`TResourceAllocator`中增加`void FreeCommandBuffer(Turbo::Core::TCommandBuffer *commandBuffer)`成员函数
  >* `./engine/render`下`TBuffer`中`void Copy(void *src, uint64_t size)`成员函数进行优化
  >* `./engine/render`下`TBuffer`中增加`void Copy(TBuffer *src, uint64_t srcOffset, uint64_t size)`成员函数

* 2022/12/27 设计架构
  >
  >* `./engine/render`下`TContext`中增加`Turbo::Core::TCommandBuffer *commandBuffer`成员变量
  >* `./engine/render`下`TContext`中增加`Turbo::Core::TCommandBuffer *GetCommandBuffer()`成员变量
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`

* 2022/12/28 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* 修改`./engine/core`下的`TSwapchain::InternalCreate()`中的`if (this->surface->GetMaxImageArrayLayers() > this->imageArrayLayers)`，更改成`if (this->surface->GetMaxImageArrayLayers() < this->imageArrayLayers)`原先是个`Bug`

* 2022/12/29 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`

* 2022/12/30 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./engine/framegraph`下`TFrameGraph`中增加`struct TRenderPass`成员，目前是实验性代码
  >* `./engine/render`下`TContext`中增加`void BeginRenderPass(Turbo::FrameGraph::TRenderPass &renderPass);`成员函数，目前是实验性代码
  >* `./docs/Design/`下新增`FrameGraph.md`，用于`FrameGraph`设计文档

* 2022/12/31 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraph.md`

* 2023/1/1 设计架构
  >
  >* `./engine/framegraph/include/TFrameGraph.hpp`中增加`class TSubpass`类
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`std::vector<TResource> writes`成员变量
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`std::vector<TResource> reads`成员变量
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`TResource Write(TResource resource)`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`TResource Read(TResource resource)`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TRenderPass`增加`std::vector<TSubpass> subpasses`成员变量
  >* `./engine/framegraph`中`TFrameGraph::TRenderPass`增加`void AddSubpass(const TSubpass &subpass)`成员变量

* 2023/1/2 设计架构
  >
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`std::vector<TResource> GetWrites()`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`std::vector<TResource> GetReads()`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`std::vector<TSubpass> GetSubpasses()`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TSubpass`增加`friend class TFrameGraph`友元类
  >* `./engine/framegraph`中`TFrameGraph::TRenderPass`增加`friend class TFrameGraph`友元类
  >* `./engine/framegraph`中`TFrameGraph::TBuilder`增加`class TSubpass`成员类，其作为`TFrameGraph::TSubpass`类的前端类或代理
  >* `./engine/framegraph`中`TFrameGraph::TBuilder`增加`TSubpass CreateSubpass()`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TBuilder::TSubpass`增加`TSubpass(TBuilder *builder, uint32_t index)`构造函数
  >* `./engine/framegraph`中`TFrameGraph::TBuilder::TSubpass`增加`TTBuilder *builder = nullptr`成员变量
  >* `./engine/framegraph`中`TFrameGraph::TBuilder::TSubpass`增加`uint32_t index`成员变量
  >* `./engine/framegraph`中`TFrameGraph::TBuilder::TSubpass`增加`TResource Write(TResource resource)`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TBuilder::TSubpass`增加`TResource Read(TResource resource)`成员函数
  >* `./engine/framegraph`中`TFrameGraph::TPassNode`增加`TRenderPass renderPass`成员变量
  >* `./engine/framegraph`中`TFrameGraph::TPassNode`增加`TRenderPass GetRenderPass()`成员函数
  >* `./engine/framegraph`中`TFrameGraph`增加`uint32_t TURBO_INVALID_SUBPASS_INDEX`全局变量
  >* 更新`./samples`中`rameGraphSample`增加`int test3()`对于新`FrameGraph`标准的测试代码

* 2023/1/3 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraph.md`
  >* `./engine/framegraph`中`TFrameGraph::TBuilder`下的`TResource Read(TResource resource)`成员函数访问域从`public`改成`private`
  >* `./engine/framegraph`中`TFrameGraph::TBuilder`下的`TResource Write(TResource resource)`成员函数访问域从`public`改成`private`
  >* 更新`./samples`中`rameGraphSample`示例代码，适配到新的`FrameGraph`标准并移除原先的`test2`，并将`test3`改名成`test2`
  >* `./engine/framegraph`中`TFrameGraph::TRenderPass`移除`uint32_t testValue`成员变量
  >* `./engine/samples`中`RenderDesignTest`移除`TFrameGraph::TRenderPass::testValue`有关代码
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./asset/models/`下增加`fuxiii-2022.gltf`模型
  >* `./engine/framegraph`中`TFrameGraph`增加`std::string ToMermaid()`成员函数

* 2023/1/4 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraph.md`
  >* `./engine/framegraph`中`TFrameGraph`更新`std::string ToMermaid()`成员函数
  >* `./engine/render`下增加`TRenderPass.h`，`TRenderPass.cpp`文件
  >* `./engine/render`下增加`TRenderPass`类
  >* `./asset/models`下增加`SciFiHelmet`模型

* 2023/1/5 设计架构
  >
  >* `./engine/render`下的`RenderPass`中增加`TSubpass`类
  >* `./engine/render`下的`TSubpass`中增加`std::vector<Turbo::Render::TColorImage> colors`成员变量
  >* `./engine/render`下的`TSubpass`中增加`std::vector<Turbo::Render::TImage> inputs`成员变量
  >* `./engine/render`下的`TSubpass`中增加`Turbo::Render::TDepthStencilImage depthStencil`成员变量
  >* `./engine/render`下的`TSubpass`中增加`TSubpass &AddColorAttachment(const Turbo::Render::TColorImage &colorImage)`成员函数
  >* `./engine/render`下的`TSubpass`中增加`const std::vector<Turbo::Render::TImage>& GetInputAttachments()`成员函数
  >* `./engine/render`下的`TSubpass`中增加`Turbo::Render::TDepthStencilImage GetDepthStencilAttachment()`成员函数
  >* `./engine/render`下的`TRenderPass`中去除实验性代码
  >* `./engine/render`下的`TRenderPass`中增加`std::vector<TSubpass> subpasses`成员变量
  >* `./engine/render`下的`TRenderPass`中增加`TRenderPass &AddSubpass(const Turbo::Render::TSubpass &subpass)`成员函数
  >* `./engine/render`下的`TRenderPass`中增加`const std::vector<Turbo::Render::TSubpass> &GetSubpasses()`成员函数
  >* `./engine/render`下的`TContext`中增加`void BeginRenderPass(Turbo::Render::TRenderPass &renderPass)`成员函数

* 2023/1/6 设计架构
  >
  >* `./engine/render`下的`TImage`中增加对于`Get*()`成员函数的`const`修饰
  >* `./engine/render`下的`TImage`中增加`bool IsValid()`成员函数
  >* `./engine/render`下的`TSubpass`中增加`Attachment`时增加对于传入`Image`参数的有效性判断（`Turbo::Render::TImage::IsValid()`），有效则加入，无效则直接返回异常
  >* `./engine/render`下增加`TPipeline.h`和`TPipeline.cpp`
  >* `./engine/render`下`TPipeline`中增加`class TPipeline`
  >* `./engine/render`下`TPipeline`中增加`class TComputePipeline`
  >* `./engine/render`下`TPipeline`中增加`class TGraphicsPipeline`
  >* `./engine/render`下增加`TShader.h`和`TShader.cpp`
  >* `./engine/render`下`TShader`中增加`class TShader`
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./engine/render`下`TShader`中增加`class TVertexShader`
  >* `./engine/render`下`TShader`中增加`typedef enum TLanguage`成员枚举
  >* `./engine/render`下`TVertexShader`中增加`TVertexShader(TContext *context, TShader::TLanguage language, const std::string &code)`构造函数
  >* `./engine/render`下`TVertexShader`中增加`TVertexShader(TContext *context, size_t size, uint32_t *code)`构造函数
  >* `./engine/render`下`TShader`中增加`class TFragmentShader`
  >* `./engine/render`下`TFragmentShader`中增加`TFragmentShader(TContext *context, TShader::TLanguage language, const std::string &code)`构造函数
  >* `./engine/render`下`TFragmentShader`中增加`TFragmentShader(TContext *context, size_t size, uint32_t *code)`构造函数
  >* `./engine/render`下`TShader`中增加`class TComputeShader`
  >* `./engine/render`下`TComputeShader`中增加`TComputeShader(TContext *context, TShader::TLanguage language, const std::string &code)`构造函数
  >* `./engine/render`下`TComputeShader`中增加`TComputeShader(TContext *context, size_t size, uint32_t *code)`构造函数
  >* `Turbo::Core`层与`Turbo::Render`有同名头文件，会有冲突，使用`Turbo::Core`的头文件使用`core/include/...`，使用`Turbo::Render`的头文件使用`render/include/...`，同时修改`CMakeList.txt`中的相关设置
  >* `./engine/render`下`TShader`中增加如下，使得`Shader`必须通过`new`创建:
  >
  > ```CXX
  > TShader(TShader const &) = delete;
  > TShader(TShader &&) = delete;
  > TShader &operator=(TShader const &) = delete;
  > TShader &operator=(TShader &&) = delete;
  >  ```
  >
  >* `./engine/render`下`TComputePipeline`中增加`TComputePipeline(Turbo::Render::TComputeShader *computeShader)`构造函数
  >* `./engine/render`下`TComputePipeline`中增加`Turbo::Render::TComputeShader* computeShader`成员变量

* 2023/1/7 设计架构
  >
  >* `./engine/render`下`TComputePipeline`中增加`TComputePipeline &SetComputeShader(Turbo::Render::TComputeShader *computeShader)`成员函数
  >* `./engine/render`下`TComputePipeline`中移除`TComputePipeline(Turbo::Render::TComputeShader *computeShader)`构造函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`Turbo::Render::TVertexShader *vertexShader`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`Turbo::Render::TFragmentShader *fragmentShader`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum TTopology`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`TTopology topology`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetTopology(TTopology topology)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum TCullBits`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`using TCull = uint32_t`成员声明
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool primitiveRestartEnable`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool depthClampEnable`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool rasterizerDiscardEnable`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TPolygon polygon`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TCull cull`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetPrimitiveRestartEnable(bool primitiveRestartEnable);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthClampEnable(bool depthClampEnable);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetRasterizerDiscardEnable(bool rasterizerDiscardEnable);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetPolygon(TPolygon polygon);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetCull(TCull cull);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum TFront`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetFront(TFront front)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool depthBiasEnable`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`float depthBiasConstantFactor`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`float depthBiasClamp`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`float depthBiasSlopeFactor`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`float lineWidth`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthBiasEnable(bool depthBiasEnable);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthBiasConstantFactor(float depthBiasConstantFactor);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthBiasClamp(float depthBiasClamp);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthBiasSlopeFactor(float depthBiasSlopeFactor);`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetLineWidth(float lineWidth);`成员函数

* 2023/1/8 设计架构
  >
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum class TCompareOp`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum class TStencilOp`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum class TLogicOp`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum class TBlendFactor`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`typedef enum class TBlendOp`成员枚举
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool depthTestEnable = true`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool depthWriteEnable = true`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TCompareOp depthCompareOp = TCompareOp::LESS_OR_EQUAL`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool depthBoundsTestEnable = false`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool stencilTestEnable = false`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TStencilOp frontFailOp = TStencilOp::KEEP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TStencilOp frontPassOp = TStencilOp::KEEP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TStencilOp frontDepthFailOp = TStencilOp::KEEP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TCompareOp frontCompareOp = TCompareOp::ALWAYS`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t frontCompareMask = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t frontWriteMask = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t frontReference = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TStencilOp backFailOp = TStencilOp::KEEP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TStencilOp backPassOp = TStencilOp::KEEP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TStencilOp backDepthFailOp = TStencilOp::KEEP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TCompareOp backCompareOp = TCompareOp::ALWAYS`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t backCompareMask = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t backWriteMask = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t backReference = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`float minDepthBounds = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`float maxDepthBounds = 0`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool logicOpEnable = false`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TLogicOp logicOp = TLogicOp::NO_OP`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`bool blendEnable = false`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TBlendFactor srcColorBlendFactor = TBlendFactor::ZERO`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TBlendFactor dstColorBlendFactor = TBlendFactor::ZERO`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TBlendOp colorBlendOp = TBlendOp::ADD`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TBlendFactor srcAlphaBlendFactor = TBlendFactor::ZERO`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TBlendFactor dstAlphaBlendFactor = TBlendFactor::ZERO`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TBlendOp alphaBlendOp = TBlendOp::ADD`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`uint32_t patchControlPoints`成员变量
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetPatchControlPoints(uint32_t patchControlPoints)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthTestEnable(bool depthTestEnable)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthWriteEnable(bool depthWriteEnable)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthCompareOp(TCompareOp depthCompareOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDepthBoundsTestEnable(bool depthBoundsTestEnable)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilTestEnable(bool stencilTestEnable)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontFailOp(TStencilOp frontFailOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontPassOp(TStencilOp frontPassOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontDepthFailOp(TStencilOp frontDepthFailOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontCompareOp(TCompareOp frontCompareOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontCompareMask(uint32_t frontCompareMask)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontWriteMask(uint32_t frontWriteMask)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilFrontReference(uint32_t frontReference)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackFailOp(TStencilOp backFailOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackPassOp(TStencilOp backPassOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackDepthFailOp(TStencilOp backDepthFailOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackCompareOp(TCompareOp backCompareOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackCompareMask(uint32_t backCompareMask)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackWriteMask(uint32_t backWriteMask)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetStencilBackReference(uint32_t backReference)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetMinDepthBounds(float minDepthBounds)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetMaxDepthBounds(float maxDepthBounds)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetLogicOpEnable(bool logicOpEnable)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetLogicOp(TLogicOp logicOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetBlendEnable(bool blendEnable)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetSrcColorBlendFactor(TBlendFactor srcColorBlendFactor)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDstColorBlendFactor(TBlendFactor dstColorBlendFactor)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetColorBlendOp(TBlendOp colorBlendOp)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetSrcAlphaBlendFactor(TBlendFactor srcAlphaBlendFactor)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetDstAlphaBlendFactor(TBlendFactor dstAlphaBlendFactor)`成员函数
  >* `./engine/render`下`TGraphicsPipeline`中增加`TGraphicsPipeline &SetAlphaBlendOp(TBlendOp alphaBlendOp)`成员函数
  >* `./docs/Issue.md`中增加`TGraphicsPipeline`对于`VkPipelineTessellationStateCreateInfo`不支持的待解决问题
  
* 2023/1/9 设计架构
  >
  >* `./engine/render`下`TContext`中增加`void BindPipeline(const TComputePipeline *computePipeline)`成员函数
  >* `./engine/render`下`TContext`中增加`void BindPipeline(const TGraphicsPipeline *graphicsPipeline)`成员函数
  >* `./README.md`下增加`Modular`章节，用于描述`Turbo`架构中的各个模块
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`

* 2023/1/10 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./engine/core`下`TFence.h`中增加`class TFences`类
  >* `./engine/core`下`TFences`中增加`std::map<TDevice *, std::vector<TFence *>> fenceMap`成员变量
  >* `./engine/core`下`TFences`中增加`void Add(TFence *fence)`成员函数
  >* `./engine/core`下`TFences`中增加`bool Wait(uint64_t timeout)`成员函数
  >* `./engine/core`下`TFence`中增加`TDevice *GetDevice()`成员函数
  >* `./engine/render`下`TContext`中增加`void Flush()`成员函数
  >* `./engine/render`下`TContext`中增加`bool Wait(uint64_t timeout)`成员函数
  >* `./engine/render`下`TContext.h`中增加`typedef struct TCommandBuffer`结构体
  >* `./engine/render`下`TContext`中`Turbo::Core::TCommandBuffer *commandBuffer`成员变量改为`Turbo::Render::TCommandBuffer currentCommandBuffer`
  >* `./engine/render`下`TContext`中`Turbo::Core::TCommandBuffer *GetCommandBuffer()`成员变量改为`Turbo::Render::TCommandBuffer GetCommandBuffer()`
  >* `./engine/render`下`TContext`中增加`std::vector<Turbo::Render::TCommandBuffer> commandBuffers`成员变量
  >* `./docs/`下增加`FAQ.md`用于记录常见问题

* 2023/1/11 设计架构
  >
  >* `./engine/framegraph`下`TVirtualResourceProxy`中`virtual void Create()`成员函数增加`void *allocator`函数参数
  >* `./engine/framegraph`下`TVirtualResourceProxy`中`virtual void Destroy()`成员函数增加`void *allocator`参数
  >* `./engine/framegraph`下`TResourceProxy`中`virtual void Create()`成员函数增加`void *allocator`函数参数
  >* `./engine/framegraph`下`TResourceProxy`中`virtual void Destroy()`成员函数增加`void *allocator`参数
  >* `./engine/framegraph`下`TFrameGraph`中`void Execute(void *context = nullptr)`成员函数增加`void *allocator`参数
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* 更新`./docs/Design/`下的`FrameGraph.md`

* 2023/1/12 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraph.md`
  >* `./engine/framegraph`下`TSubpass`中增加`std::vector<TResource> inputs`成员变量
  >* `./engine/framegraph`下`TSubpass`中增加`void Input(TResource resource)`成员函数
  >* `./engine/framegraph`下`TSubpass`中增加`std::vector<TResource> GetInputs()`成员函数
  >* `./engine/framegraph`下`TResource`中增加`bool isInput`成员变量
  >* `./engine/framegraph`下`TFrameGraph::TBuilder`中增加`TResource Input(TResource resource)`成员函数
  >* `./engine/framegraph`下`TFrameGraph::TBuilder::TSubpass::Read(TResource resource)`中增加`resource.isInput = false`设置代码
  >* `./engine/framegraph`下`TFrameGraph::TBuilder::TSubpass::Input(TResource resource)`中增加`resource.isInput = true`设置代码
  >* `./engine/framegraph`下`Turbo::FrameGraph::TPassNode::IsRead(TResource resource)`下增加重复性`isInput`检测
  >* `./engine/framegraph`下`Turbo::FrameGraph::TFrameGraph::ToMermaid()`下增加对`input`资源的解析
  >* `./engine/framegraph`下`Turbo::FrameGraph::TFrameGraph::ToMermaid()`下`struct linkStyle`下增加`enum Type`成员枚举，并增加`Type type`成员变量，移除`bool isWrite`成员变量
  >* `./engine/framegraph`下`Turbo::FrameGraph::TPassNode::AddRead(...)`中移除`assert(!this->IsCreate(resource) && !this->IsWrite(resource))`判断
  >* 更新`./samples`下`FrameGraphSample`示例

* 2023/1/13 设计架构
  >
  >* 更新`./samples`下的`VulkanTest.cpp`

* 2023/1/14 设计架构
  >
  >* `./engine/core`下`Turbo::Core::TGraphicsPipeline`下增加兼容`VkPipelineCache`的构造函数，目前为测试研发

* 2023/1/15 设计架构
  >
  >* `./engine/core`下增加`TPipelineCache.h`和`TPipelineCache.cpp`
  >* `./engine/core`下`TPipelineCache`中增加`class Turbo::Core::TPipelineCache`
  >* `./engine/core`下`TPipelineCache`中增加`TDevice *device`成员变量
  >* `./engine/core`下`TPipelineCache`中增加`VkPipelineCache vkPipelineCache`成员变量
  >* `./engine/core`下`TPipelineCache`中增加`size_t size`成员变量
  >* `./engine/core`下`TPipelineCache`中增加`void *data`成员变量
  >* `./engine/core`下`TPipelineCache`中增加`void InternalCreate()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`void InternalDestroy()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`TPipelineCache(TDevice *device)`构造函数
  >* `./engine/core`下`TPipelineCache`中增加`TPipelineCache(TDevice *device, size_t size, void *data)`构造函数
  >* `./engine/core`下`TPipelineCache`中增加`~TPipelineCache()`析构函数
  >* `./engine/core`下`TPipelineCache`中增加`VkPipelineCache GetVkPipelineCache()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`size_t GetSize()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`TResult GetData(size_t size, void *dst)`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`std::string ToString()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`uint32_t GetHeaderSize()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`enum class TPipelineCacheHeaderVersion`枚举
  >* `./engine/core`下`TPipelineCache`中增加`TPipelineCacheHeaderVersion GetHeaderVersion()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`TVendorInfo GetVendor()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`uint32_t GetDeviceID()`成员函数
  >* `./engine/core`下`TPipelineCache`中增加`std::vector<uint8_t> GetUUID()`成员函数

* 2023/1/16 设计架构
  >
  >* `./engine/core`下`TPipeline.h`中构造函数，增加对`TPipelineCache`的使用
  >* `./engine/core`下`TPipeline.h`中增加`TPipelineCache *pipelineCache`成员变量
  >* `./engine/core`下`TPipeline.h`中增加`TPipelineCache *GetPipelineCache()`成员函数
  >* `./engine/core`下`TGraphicsPipeline.h`中移除`VkPipelineCache vkPipelineCache`的相关参数和成员，改为使用`TPipelineCache`
  >* `./engine/core`下`TComputePipeline`中增加`TComputePipeline(TPipelineCache *pipelineCache, TComputeShader *computeShader)`构造函数
  >* `./engine/core`下`TComputePipeline`中更新`TComputePipeline::InternalCreate()`成员函数，适配`TPipelineCache`来创建`TComputePipeline`
  >* 更新`./samples`下的`VulkanTest.cpp`
  >* 移除`./engine/core`下`TPipelineCache.cpp`中莫名其妙的头文件，应该是`clangd`的自动头文件补全搞的
  >* 移除`./engine/core`下`TPipelineCache.cpp`中`#include <basetsd.h>`头文件，应该是`clangd`的自动头文件补全搞的
  >* 移除`./engine/core`下`TFrameGraph.cpp`中`#include <basetsd.h>`头文件，应该是`clangd`的自动头文件补全搞的
  >* 移除`./samples`下`VulkanTest.cpp`中移除`#include <vcruntime.h>`和`#include <vcruntime_string.h>`头文件，应该是`clangd`的自动头文件补全搞的
  >* `./engine/core`下`TObject.h`中增加`#include <string.h>`头文件`memcpy`，在此头文件声明

* 2023/1/17 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraph.md`
  
* 2023/1/18 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* 更新`./.gitmodules`下的`thirdparty/glm`库链接
  >* 更新`./docs`下的`FAQ.md`
  >* `./engine/framegraph`下`TResources`中增加`Turbo::FrameGraph::TRenderPass GetRenderPass() const`成员函数
  >* 更新`./samples`下的`RenderDesignTest.cpp`

* 2023/1/19 设计架构
  >
  >* `./engine/framegraph`下`TRenderPass`中增加`Turbo::FrameGraph::TSubpass GetSubpass(size_t index)`成员函数
  >* `./engine/framegraph`下`TSubpass`中增加`TResource GetWrite(size_t index)`成员函数
  >* `./engine/framegraph`下`TSubpass`中增加`TResource GetRead(size_t index)`成员函数
  >* `./engine/framegraph`下`TSubpass`中增加`TResource GetInput(size_t index)`成员函数
  >* `./engine/framegraph`下`TNodeHandle`中增加`bool IsValid()`成员函数
  >* `./engine/render`下`TImage`中`uint32_t GteHeight() const`成员函数，更改成`uint32_t GetHeight() const`。之前拼写错误

* 2023/1/21 设计架构
  >
  >* `./engine/core`下`TBuffer`中`Map`函数中有,当想要映射内存时，目前使用的是用户创建`TBuffer`时，是否指定了`HOST_ACCESS_RANDOM`或者`HOST_ACCESS_SEQUENTIAL_WRITE`标志位，如果设置了该标志位说明内存可以进行映射，但是某些移动设备（手机或笔记本等平台端）上的内存是`CPU`与`GPU`共存、共享的。换句话说当你在上层创建一个`GPU`端的内存(此时用户指定的是`DEDICATED_MEMORY`)，在底层也许是`CPU`和`GPU`共享的内存，所以此时根据用户是否指定了`HOST_ACCESS_RANDOM`和`HOST_ACCESS_SEQUENTIAL_WRITE`标志位来判断是否可以`Map`是不合理的，应该查看分配的内存是否有在`host`端能够访问，能访问将返回映射的内存指针，反之则返回`nullptr`,但是当前`VulkanMemoryAllocator`中对于内存分配，如果没有使用`Host`端的相关标志位（`HOST_ACCESS_RANDOM`或者`HOST_ACCESS_SEQUENTIAL_WRITE`），在`VulkanMemoryAllocator`中都被认为是不能进行内存映射的，所以需要在`Turbo::Render::TBuffer::Copy(...)`中增加额外判断，判断内存是否支持映射
  >* `./engine/core`下`TBuffer`中增加`bool IsMappable()`成员函数
  >* `./engine/core`下`TBuffer`中`Copy`函数中增加对`this->buffer->IsMappable()`的判断
  >* `./engine/core`下`TImage`中增加`bool IsMappable()`成员函数

* 2023/1/22 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./engine/render`下`TContext`中增加`class TRenderPassPool`类
  >* `./engine/render`下`TContext`中`class TRenderPassPool`下增加`class TRenderPassProxy`类

* 2023/1/23 设计架构
  >
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`
  >* `./engine/render`下`TRenderPassPool`中`TRenderPassProxy`增加`void Create(Turbo::Render::TRenderPass &renderPass)`成员函数
  >* `./engine/render`下`TRenderPassPool`中`TRenderPassProxy`增加`void Destroy()`成员函数
  >* `./engine/render`下`TRenderPassPool`中`TRenderPassProxy`增加`friend class TRenderPassPool`友元类
  >* `./engine/render`下`TRenderPassPool`中`Find(...)`函数更改成`TRenderPassProxy Find(Turbo::Render::TRenderPass &renderPass)`
  >* `./engine/render`下`TRenderPassPool`中增加`TRenderPassProxy Allocate(Turbo::Render::TRenderPass &renderPass)`成员函数
  >* `./engine/render`下`TRenderPassPool`中增加`void Free(Turbo::Render::TRenderPass &renderPass)`成员函数
  >* `./engine/render`下`TRenderPassPool`中`TRenderPassProxy`增加`bool IsValid()`成员函数
  
* 2023/1/26 设计架构
  >
  >* `./engine/render`下`TRenderPassPool`更新完善`Find(...)`函数算法
  >* `./engine/render`下`TImage.h`中增加`typedef enum TSampleCountBits`
  >* `./engine/render`下`TImage.h`中增加`using TSampleCounts = uint32_t`
  >* `./engine/render`下`TImage`中增加`TSampleCountBits GetSampleCountBits() const`成员函数，目前只支持`Turbo::Render::TSampleCountBits::SAMPLE_1_BIT`
  >* `./engine/render`下`TContext.h`中`TRenderPassPool`构造函数增加`TContext *context`参数
  >* `./engine/render`下`TContext.h`中`TRenderPassPool::TRenderPassProxy`中`Create`函数增加`TContext *context`参数
  >* `./engine/render`下`TContext.h`中`TRenderPassPool::TRenderPassProxy`中更新完善`Create`函数
  >* 更新`./docs/Design/`下的`FrameGraphAdvance.md`

* 2023/1/27 设计架构
  >
  >* `./engine/render`下`TImage`中增加`friend class TRenderPassPool`友元类
  >* `./engine/render`下`TContext.h`中`TRenderPassPool`中`TRenderPassProxy`中，更新完善`Create(...)`函数
  >* `./engine/render`下`TContext.h`中`TRenderPassPool`中，更新完善`~TRenderPassPool()`析构函数
  >* 更新`./samples`下`RenderDesignTest`示例

* 2023/1/28 设计架构
  >
  >* 更新`./samples`下`VolumetricCloud`示例中的对于`./asset/shaders/volumetric_cloud.frag`中对于高频细节纹理使用的算法

* 2023/1/31 设计架构
  >
  >* 有关`./samples`下`VolumetricCloud`示例中对于体积云光照渲染，总是渲染不出满意的效果。国外常见的体积云渲染的论文、文章之类的也零零散散的看了，大部分的文章套路就是，先把一长串光照传输方程列出来，讲讲朗伯-比尔定律，讲讲`Phase`相函数（米氏散射，H-G散射，瑞利散射，几何散射等等），讲讲内散射、外散射、吸收以及消亡之间的关系，略过了大量的细节，到后文的相关实现，更是略之又略（比如寒霜引擎的那片文章，散射、外散射、吸收以及消亡系数如何计算并没有说明，有的文章直接拿采样密度作为消亡系数，等等很多问题），而且不同文章都有各自的实现（特别是计算采样点到太阳之间的光照，一个人一个写法），很难获得一个统一的、较明确的实现原理和过程（说白了就是，文章的实现和理论有时并不是一一对应的），这就会导致看的越多越迷惑（只能说大致相同，落到细节疑问百出），所以我尝试将`Production Volume Rendering SIGGRAPH 2017 Course`文章（该文章较理论化，原文放到了`./docs`下`volumePaper.pdf`）翻译到`./docs/VolumetricCloud.md`文档中（光照章节），希望在边学边翻译的过程中能解决之前的疑问吧
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/1 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/2 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/3 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/4 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/6 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/2/7 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/8 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/9 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/2/10 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档
  >* `./engine/render`下`TImage.h`中`TImage`中增加`virtual void CreateImageView(Turbo::Core::TImage *image)`函数
  >* `./engine/render`下`TImage.h`中`TImage::Create(...)`中增加对`virtual Turbo::Core::TImageView * CreateImageView(Turbo::Core::TImage *image)`成员函数的调用
  >* `./engine/render`下`TTexture2D`中增加`virtual Turbo::Core::TImageView * CreateImageView(Turbo::Core::TImage *image)`函数
  >* `./engine/render`下`TTexture3D`中增加`virtual Turbo::Core::TImageView * CreateImageView(Turbo::Core::TImage *image)`函数
  >* `./engine/render`下`TDepthTexture2D`中增加`virtual Turbo::Core::TImageView * CreateImageView(Turbo::Core::TImage *image)`函数
  >* `./engine/render`下`TImage`的`Destroy`成员函数中进行`ImageView`的资源回收
  >* `./engine/render`下`TImage`的`IsValid()`成员函数中增加对`ImageView`的判断
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/11 设计架构
  >
  >* `./engine/render`下`TRenderPass.h`中`TRenderPass`中增加`Turbo::Core::TRenderPass *renderPass = nullptr`成员变量
  >* `./engine/render`下`TRenderPass.h`中`TRenderPass`中增加`friend class TRenderPassPool;`友元类
  >* `./engine/render`下`TContext`中将`TRenderPassProxy`移除
  >* `./engine/render`下`TContext`中将`std::vector<TRenderPassProxy> renderPassProxies`更改成`std::vector<TRenderPass> renderPasses`
  >* `./engine/render`下`TContext`中将`TRenderPassProxy`中的`void Create(Turbo::Render::TRenderPass &renderPass, Turbo::Render::TContext *context)`和`void Destroy()`移动到`TRenderPassPool`中，并重命名为`void CreateRenderPass(Turbo::Render::TRenderPass &renderPass, Turbo::Render::TContext *context)`和`void DestroyRenderPass(Turbo::Render::TRenderPass &renderPass)`
  >* `./engine/render`下`TContext`中将`TRenderPassPool`中移除`void DestroyRenderPass(Turbo::Render::TRenderPass &renderPass)`成员函数
  >* `./engine/render`下`TContext`中将`TRenderPassProxy Find(Turbo::Render::TRenderPass &renderPass)`更改成`bool Find(Turbo::Render::TRenderPass &renderPass)`
  >* `./engine/render`下`TContext`中将`TRenderPassProxy Allocate(Turbo::Render::TRenderPass &renderPass)`更改成`bool Allocate(Turbo::Render::TRenderPass &renderPass)`
  >* `./engine/render`下`TRenderPass`中增加`bool IsValid()`函数

* 2023/2/13 设计架构
  >
  >* `./engine/render`下`TContext.h`中增加`TFrameBufferPool`类
  >* `./engine/render`下`TContext.h`中`TRenderPassPool`中的成员函数`void CreateRenderPass(Turbo::Render::TRenderPass &renderPass, Turbo::Render::TContext *context)`修改成`void CreateRenderPass(Turbo::Render::TRenderPass &renderPass)`，其中的`Turbo::Render::TContext *context`在`TRenderPassPool`中存在对应的成员变量
  >* `./engine/render`下`TContext.h`中`TRenderPassPool`中的成员变量`std::vector<TRenderPass> renderPasses;`修改成`std::vector<TRenderPass> renderPasses;`
  >* `./engine/render`下`TRenderPass.h`中`TRenderPass`中增加`Turbo::Core::TFramebuffer *framebuffer`成员变量

* 2023/2/14 设计架构
  >
  >* 修改`./engine/core`下`TPhysicalDevice`中`GetMaxImageExtent()`中增加当格式不被支持时的判断
  >* 修改`./engine/core`下`TPhysicalDevice`中`GetMaxImageMipLevels()`中增加当格式不被支持时的判断
  >* 修改`./engine/core`下`TPhysicalDevice`中`GetMaxImageArrayLayers()`中增加当格式不被支持时的判断
  >* 修改`./engine/core`下`TPhysicalDevice`中`GetSupportImageSampleCounts()`中增加当格式不被支持时的判断
  >* 修改`./engine/core`下`TPhysicalDevice`中`GetMaxImageResourceSize()`中增加当格式不被支持时的判断
  >* `./engine/core`下`TPhysicalDevice`中增加`bool IsFormatSupportImage(...)`成员函数
  >* `./engine/core`下`TImage`中`InternalCreate()`中增加对于格式是否支持的判断
  >* `./engine/render`下`TRenderPass`中增加`friend class TFramebufferPool`使得`TFramebufferPool`在分配之后将结果刷新到`TRenderPass`中
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档
  >* `./engine/core`下`TFramebuffer`中增加`std::vector<TImageView *> GetAttachments()`成员函数
  >* `./engine/render`下`TRenderPass`下增加`std::vector<Turbo::Render::TImage> GetAttachments();`成员函数
  >* `./engine/render`下`TImage`下增加`bool operator ==(const TImage& image)`成员函数
  >* `./engine/render`下`TImage`下增加`bool operator !=(const TImage& image)`成员函数
  >* `./engine/render`下`TImage`下增加`friend class TFramebufferPool`友元类
  >* `./engine/render`下`TContext.h`下的`TFramebufferPool`基本完成

* 2023/2/15 设计架构
  >
  >* `./engine/render`下`TRenderPass`中`IsValid()`成员函数，增加对于`Turbo::Core::TFramebuffer *framebuffer`成员变量的判断
  >* `./engine/render`下`TRenderPassPool`中增加`TFramebufferPool *framebufferPool;`成员变量，增加对于`Turbo::Core::TFramebuffer`的分配
  >* `./engine/render`下`TRenderPassPool`中增加`Allocate(...)`成员函数，增加对于`Turbo::Core::TFramebuffer`的分配
  >* `./engine/render`下`TContext`中增加`TRenderPassPool *renderPassPool`成员变量，并在构造函数和析构函数中进行创建和销毁
  >* `./engine/render`下`TRenderPass`中增加`friend class TContext;`友元类
  >* `./engine/render`下`TContext`中更新完善`BeginRenderPass(...)`成员函数
  >* `./samples`下增加`PushConstantTest`例子，用于测试`Vulkan`的`Push Constant`
  >* `./engine/render`下`TContext`中增加`void EndRenderPass();`成员函数
  >* `./engine/render`下`TRenderPass`中增加`bool IsEmpty() const`成员函数
  >* `./engine/render`下`TSubpass`中增加`bool IsEmpty() const`成员函数

* 2023/2/16 设计架构
  >
  >* `./engine/render`下`TContext`中`BeginRenderPass(...)`成员函数增加`bool`返回结果
  >* `./engine/render`下`TImage`中`Create()`成员函数中增加`Layout`从`UNDEFINED`转成`GENERAL`的测试代码
  >* `./engine/render`下`TContext`中增加`void ClearTexture(Turbo::Render::TTexture2D &texture2D, float r = 0, float g = 0, float b = 0, float a = 0)`成员函数
  >* `./engine/render`下`TImage`中增加`friend class TContext`友元类
  >* `./engine/render`下`TContext`构造函数中,修改对于`Window`系统中`VK_KHR_WIN32_SURFACES`扩展对应错误的`Bug`

* 2023/2/17 设计架构
  >
  >* `./engine/render`下`TContext`中增加`Turbo::Core::TImage *GetTextureImage(Turbo::Render::TTexture2D texture2d)`成员函数，该函数仅用于测试，请谨慎调用
  >* `./samples`下增加`RenderAndFrameGraph`例子，用于测试使用`FrameGraph`来驱动`Turbo`进行绘制。
  >* `./samples`下`VolumetricCloud`例子，中用于体积云的着色器文件对应目录被误改了，修正

* 2023/2/18 设计架构
  >
  >* `./samples`下增加`ClearColorTest`例子，用于测试`Vulkan`清空颜色测试

* 2023/2/19 设计架构
  >
  >* 更新`./docs`下`VolumetricCloud.md`文档

* 2023/2/20 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/2/21 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/2/22 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档
  >* `./engine/render`下`TBuffer.h`中增加`TVertexBuffer`类
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`typedef enum TRate`枚举
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`class TAttribute`类
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`std::vector<TVertexBuffer::TAttribute> attributes`成员变量
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`struct Descriptor`成员结构
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`uint32_t stride`成员变量
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`TRate rate`成员变量
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`void Create(const std::string &name, const Descriptor &descriptor, void *allocator)`成员函数
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`TAttributeID AddAttribute(Turbo::Render::TFormat format, uint32_t offset)`成员函数
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`TAttribute GetAttribute(TAttributeID id)const`成员函数

* 2023/2/23 设计架构
  >
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`uint32_t GetStride() const`成员函数
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`类中声明`TRate GetRate()const`成员函数

* 2023/2/24 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档
  >* `./engine/render`下`TFormat.h`中增加`R32_SFLOAT`成员枚举
  >* `./engine/render`下`TFormat.h`中增加`R32G32_SFLOAT`成员枚举
  >* `./engine/render`下`TFormat.h`中增加`R32G32B32_SFLOAT`成员枚举
  >* `./engine/render`下`TFormat.h`中增加`R32G32B32A32_SFLOAT`成员枚举
  >* `./engine/render`下`TContext.h`中`TContext`增加`void BindVeretxAttribute(const Turbo::Render::TVertexBuffer &vertexBuffer, Turbo::Render::TAttributeID attributeID, uint32_t location)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`增加`std::vector<Turbo::Core::TVertexBinding *> vertexBindings`成员变量，用于管理和暂存绑定的顶点信息
  >* `./engine/render`下`TBuffer.h`中`TBuffer`增加`bool IsValid() const`成员函数
  >* `./engine/render`下`TBuffer.h`中`TBuffer`增加`friend class TContext;`友元类
  >* `./engine/render`下`TContext.h`中`TContext`增加`std::vector<Turbo::Core::TBuffer *> vertexBuffers`成员变量，用于管理和暂存绑定的顶点缓冲
  >* `./engine/render`下`TBuffer.h`中`TVertexBuffer`中`TAttribute`增加`bool IsValid() const;`成员函数
  >* 修改`./engine/core`下`TVertexBinding`中`AddAttribute(...)`成员函数的算法（防止重复的`location`，如果有重复的`location`，将对应的属性覆盖刷新）
  >* `./engine/core`下`TVertexAttribute`中增加`void SetLocation(uint32_t location)`成员函数
  >* `./engine/core`下`TVertexAttribute`中增加`void SetFormatType(TFormatType formatType)`成员函数
  >* `./engine/core`下`TVertexAttribute`中增加`void SetOffset(uint32_t offset)`成员函数

* 2023/2/25 设计架构
  >
  >* 修正`./samples`下`RenderAndFrameGraph.cpp`示例中，由于`Layout`转换`Bug`导致什么也不显示。

* 2023/2/26 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/2/27 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/2/28 设计架构
  >
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档
  >* `./engine/render`下`TBuffer.h`中新增`TIndexBuffer`类，继承自`Turbo::Render::TBuffer`。用于代表索引缓冲

* 2023/3/1 设计架构
  >
  >* `./engine/render`下`TBuffer.h`中`TIndexBuffer`类中增加`void Copy(const std::vector<uint32_t>& indexs)`成员函数
  >* `./engine/render`下`TBuffer.h`中`TIndexBuffer`类中增加`void Copy(const std::vector<uint16_t>& indexs)`成员函数
  >* `./engine/render`下`TBuffer.h`中增加`TUniformBuffer`类

* 2023/3/2 设计架构
  >
  >* `./engine/render`下`TBuffer.h`中`TUniformBuffer`类增加`void Create(const std::string &name, const Descriptor &descriptor, void *allocator)`成员函数
  >* `./engine/render`下`TBuffer.h`中`TUniformBuffer`类增加`void Copy(const T &uniform)`成员函数

* 2023/3/3 设计架构
  >
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`void BindDescriptor(uint32_t set, uint32_t binding, const std::vector<Turbo::Render::TTexture2D> &texture2Ds)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`void BindDescriptor(uint32_t set, uint32_t binding, const Turbo::Render::TTexture2D &texture2D)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`void BindDescriptor(uint32_t set, uint32_t binding, const std::vector<Turbo::Render::TTexture3D> &texture3Ds)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`void BindDescriptor(uint32_t set, uint32_t binding, const Turbo::Render::TTexture3D &texture3D)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`template <typename T>void BindDescriptor(uint32_t set, uint32_t binding, const std::vector<Turbo::Render::TUniformBuffer<T>> &uniformBuffers)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`template <typename T>void BindDescriptor(uint32_t set, uint32_t binding, const Turbo::Render::TUniformBuffer<T> &uniformBuffer)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`using TSetID = uint32_t`成员声明
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`using TBindingID = uint32_t`成员声明
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`typedef enum class TDescriptorMapType`成员枚举
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`typedef struct TDescriptorID`成员结构体声明
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`std::map<TDescriptorID, std::vector<Turbo::Core::TBuffer *>, TDescriptorID> uniformBufferMap`成员变量
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`std::map<TDescriptorID, std::vector<std::pair<Turbo::Core::TImageView *, Turbo::Core::TSampler *>>, TDescriptorID> combinedImageSamplerMap`成员变量
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`std::map<TDescriptorID, std::vector<Turbo::Core::TImageView *>, TDescriptorID> sampledImageMap`成员变量
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`std::map<TDescriptorID, std::vector<Turbo::Core::TSampler *>, TDescriptorID> samplerMap`成员变量
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`std::map<TSetID, std::map<TBindingID, TDescriptorMapType>> descriptorMap`成员变量

* 2023/3/5 设计架构
  >
  >* `./engine/render`下`TContext.h`中`TContext`类中，更新实现`BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TTexture2D &texture2D)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中，更新实现`BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TTexture3D> &texture3Ds)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中，更新实现`BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TTexture3D &texture3D)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中，更新实现`template <typename T>void BindDescriptor(TSetID set, TBindingID binding, const std::vector<Turbo::Render::TUniformBuffer<T>> &uniformBuffers)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中，更新实现`template <typename T>void BindDescriptor(TSetID set, TBindingID binding, const Turbo::Render::TUniformBuffer<T> &uniformBuffer)`成员函数
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档
  >* `./engine/render`下增加`TSampler.h`和`TSampler.cpp`用于表示`Sampler`
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`typedef enum class TFilter`成员枚举
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`typedef enum class TMipmapMode`成员枚举
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`typedef enum class TAddressMode`成员枚举
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`Turbo::Core::TSampler *sampler = nullptr`成员变量
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`struct Descriptor`成员声明
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`friend class TContext`友元类
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`void *allocator = nullptr`成员变量
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`Turbo::Core::TSampler *sampler = nullptr`成员变量
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`Descriptor descriptor`成员变量
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`void Create(const std::string &name, const Descriptor &descriptor, void *allocator)`成员函数
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`void Destroy(void *allocator)`成员函数
  >* `./engine/render`下`TSampler.h`中`TSampler`类中增加`bool IsValid() const`成员函数

* 2023/3/6 设计架构
  >
  >* `./engine/render`下`TResourceAllocator.h`中`TResourceAllocator`类中，增加`Turbo::Core::TSampler *CreateSampler(const Turbo::Render::TSampler::Descriptor &descriptor)`成员函数
  >* `./engine/render`下`TResourceAllocator.h`中`TResourceAllocator`类中，增加`void DestroySampler(Turbo::Core::TSampler *sampler)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中，增加`Turbo::Core::TSampler *CreateSampler(const Turbo::Render::TSampler::Descriptor &descriptor)`成员函数
  >* `./engine/render`下`TContext.h`中`TContext`类中，增加`void DestroySampler(Turbo::Core::TSampler *sampler)`成员函数
    >* `./engine/render`下`TSampler.h`中`TSampler`类中更新`void Create(const std::string &name, const Descriptor &descriptor, void *allocator)`成员函数
  >* `./engine/render`下`TSampler.h`中`TSampler`类中更新`void Destroy(void *allocator)`成员函数
  >* 更新`./docs/Design`下`FrameGraphAdvance.md`文档

* 2023/3/7 设计架构
  >
  >* `./engine/render`下`TContext.h`中增加`TGraphicsPipelinePool`类
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`std::map<Turbo::Core::TRenderPass *, std::map<uint32_t /*subpass*/, std::vector<Turbo::Core::TGraphicsPipeline *>>> graphicsPipelineMap`成员变量
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`bool Allocate(Turbo::Render::TRenderPass &renderPass, uint32_t subpass, Turbo::Render::TGraphicsPipeline &graphicsPipeline)`成员函数
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`bool Find(Turbo::Render::TRenderPass &renderPass, uint32_t subpass, Turbo::Render::TGraphicsPipeline &graphicsPipeline)`成员函数
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`void CreateGraphicsPipeline(Turbo::Render::TRenderPass &renderPass, uint32_t subpass, Turbo::Render::TGraphicsPipeline &graphicsPipeline)`成员函数
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`void Free(Turbo::Render::TGraphicsPipeline &graphicsPipeline)`成员函数
  >* `./engine/render`下`TRenderPass.h`中`TRenderPass`类中增加`friend class TGraphicsPipelinePool`友元类
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`friend class TGraphicsPipelinePool`友元类
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`Turbo::Core::TGraphicsPipeline *graphicsPipeline = nullptr`成员变量
  
* 2023/3/8 设计架构
  >
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`TContext *context`成员变量
  >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`TGraphicsPipelinePool(TContext *context)`构造函数
  >* `./engine/render`下`TContext.h`中`TContext`类中增加`std::vector<Turbo::Core::TVertexBinding *> GetVertexBindings()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`Turbo::Render::TVertexShader *GetVertexShader()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`Turbo::Render::TFragmentShader *GetFragmentShader()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TTopology GetTopology()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetPrimitiveRestartEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetPatchControlPoints()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetDepthClampEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetRasterizerDiscardEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TPolygon GetPolygon()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TCull GetCull()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TFront GetFront()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetDepthBiasEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`float GetDepthBiasConstantFactor()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`float GetDepthBiasClamp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`float GetDepthBiasSlopeFactor()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`float GetLineWidth()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetDepthTestEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetDepthWriteEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TCompareOp GetDepthCompareOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetDepthBoundsTestEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetStencilTestEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TStencilOp GetStencilFrontFailOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TStencilOp GetStencilFrontPassOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TStencilOp GetStencilFrontDepthFailOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TCompareOp GetStencilFrontCompareOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetStencilFrontCompareMask()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetStencilFrontWriteMask()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetStencilFrontReference()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TStencilOp GetStencilBackFailOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TStencilOp GetStencilBackPassOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TStencilOp GetStencilBackDepthFailOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TCompareOp GetStencilBackCompareOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetStencilBackCompareMask()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetStencilBackWriteMask()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`uint32_t GetStencilBackReference()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`float GetMinDepthBounds()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`float GetMaxDepthBounds()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetLogicOpEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TLogicOp GetLogicOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`bool GetBlendEnable()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TBlendFactor GetSrcColorBlendFactor()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TBlendFactor GetDstColorBlendFactor()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TBlendOp GetColorBlendOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TBlendFactor GetSrcAlphaBlendFactor()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TBlendFactor GetDstAlphaBlendFactor()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TBlendOp GetAlphaBlendOp()`成员函数
  >* `./engine/render`下`TPipeline.h`中`TGraphicsPipeline`类中增加`TGraphicsPipeline`友元类
  >* `./engine/render`下`TShader.h`中`TComputeShader`类中增加`Turbo::Core::TComputeShader * GetComputeShader()`成员函数
  >* `./engine/render`下`TShader.h`中`TVertexShader`类中增加`Turbo::Core::TVertexShader * GetVertexShader()`成员函数
  >* `./engine/render`下`TShader.h`中`TFragmentShader`类中增加`Turbo::Core::TFragmentShader * GetFragmentShader()`成员函数
    >* `./engine/render`下`TContext.h`中`TContext`类中增加`void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)`成员函数
    >* `./engine/render`下`TContext.h`中`TGraphicsPipelinePool`类中增加`void GC()`成员函数
    >* `./engine/render`下`TContext.h`中`TFramebufferPool`类中增加`void GC()`成员函数
    >* `./engine/render`下`TContext.h`中`TRenderPassPool`类中增加`void GC()`成员函数
    >* `./engine/render`下`TContext.h`中增加`void GC()`成员函数
    >* `./engine/render`下`TContext.h`中增加`TGraphicsPipelinePool *graphicsPipelinePool`成员变量
    >* `./engine/render`下`TContext.h`中增加`Turbo::Render::TGraphicsPipeline currentGraphicsPipeline`成员变量
    >* `./engine/render`下`TContext.h`中增加`Turbo::Render::TRenderPass currentRenderPass`成员变量
    >* `./engine/render`下`TContext.h`中增加`uint32_t currentSubpass`成员变量

* 2023/3/9 设计架构
  >
  >* `./engine/core`下`TShader.h`中`TShader`类中增加对于`DirStackFileIncluder`的使用，使得`Turbo`支持解析带有`#include`的着色器代码
  >* `./engine/core`下`TShader.h`中`TVertexShader`类构造函数增加`const std::vector<std::string>& includePaths`形参，用于将着色器`#include`文件路径传入进行解析
  >* `./engine/core`下`TShader.h`中`TFragmentShader`类构造函数增加`const std::vector<std::string>& includePaths`形参，用于将着色器`#include`文件路径传入进行解析
  >* `./engine/core`下`TShader.h`中`TComputeShader`类构造函数增加`const std::vector<std::string>& includePaths`形参，用于将着色器`#include`文件路径传入进行解析
  >* `./samples`下增加`ShaderIncludeTest`示例，用于测试着色器的`#include`特性

* 2023/3/10 设计架构
  >
  >* `./samples`下增加`GreenFieldDiorama`示例，用于测试多通道，颜色附件，纹理采样，着色器`#include`等

* 2023/3/13 设计架构
  >
  >* 修正`./engine/render`下`TRenderPass`类中`GetAttachments()`对于深度模板纹理的信息收集`Bug`
  >* 修正`./engine/render`下`TContext`类中`BindVeretxAttribute()`对于顶点绑定索引的计算错误`Bug`
  >* 修正`./engine/render`下`TRenderPassPool`类中`Find()`对于深度模板附件未对空指针进行检查`Bug`
  >* `./samples`下增加`PureHelloTriangleAndFrameGraph`示例，用于测试基于`FrameGraph`驱动`Turbo`绘制一个纯三角形

* 2023/3/14 设计架构
  >
  >* 修正`./samples`下`PureHelloTriangleAndFrameGraph`示例，在改变窗口大小时导致的`VkImageView`无效的`Bug`，该`Bug`的原因是一帧结束，并且当窗口大小改变时没有及时回收`FrameBuffer`的资源

* 2023/3/15 设计架构
  >
  >* `./docs/Design`下增加`Core.md`文档。用于记录`Turbo`核心的相关设计
  >* 尝试解决`内存泄漏`的`Bug`

* 2023/3/16 设计架构
  >
  >* 尝试解决`内存泄漏`的`Bug`
  >* 经过多次测试和探索，`内存泄漏`位于`vkCreatePipelineLayout`和`vkDestroyPipelineLayout`函数内部，而这一部分属于显卡驱动范畴，大概率是`GPU`驱动`Bug`

* 2023/3/17 设计架构
  >
  >* 确定`vkCreatePipelineLayout`和`vkDestroyPipelineLayout`发生的内存泄漏是`NVIDIA GPU`驱动`Bug`，`Game Ready Driver`驱动版本为`531.29`发布日期为`2023/03/14`
  >* 修正添加`./engine/core`下`TDescriptorSetLayout`类中`~TDescriptorSetLayout()`成员函数对于`Turbo::Core::TNaNDescriptor`资源内存的释放回收。

* 2023/3/19 设计架构
  >
  >* `./samples`下增加`VulkanDynamicRenderingTest`示例，用于测试`Vulkan`的`Dynamic Rendering`特性
  >* `./engine/core`下`TExtensionInfo`中`TExtensionType`增加`VK_KHR_DYNAMIC_RENDERING`扩展
  >* `./engine/core`下`TExtensionInfo`中`TAllExtensionNames`增加`VK_KHR_dynamic_rendering`名称
  >* 更新`./docs/Design/Core.md`文档

* 2023/3/20 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./engine/core`下的`TPhysicalDeviceInfo`类中的`TPhysicalDeviceFeatures`更改成`VkPhysicalDeviceFeatures`
  >* `./engine/core`下的`TPhysicalDevice`类中的`TPhysicalDeviceFeatures GetDeviceFeatures()`更改成`VkPhysicalDeviceFeatures GetDeviceFeatures()`
  >* `./engine/core`下的`TCore.h`中移除`typedef VkPhysicalDeviceFeatures TPhysicalDeviceFeatures`声明
  >* `./engine/core`下的`TPhysicalDevice.h`中增加`class TPhysicalDeviceFeatures`类型，内部增加如下声明：
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool geometryShader`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool tessellationShader`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool sampleRateShading`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool depthClamp`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool depthBiasClamp`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool wideLines`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool fillModeNonSolid`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool timelineSemaphore`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool dynamicRendering`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`virtual std::string ToString() override`成员函数声明

* 2023/3/21 设计架构
  >
  >* `./engine/core`下的`TPhysicalDeviceInfo.h`中的`TPhysicalDeviceInfo`类中增加`VkPhysicalDeviceVulkan11Features vulkan11Feature`成员变量
  >* `./engine/core`下的`TPhysicalDeviceInfo.h`中的`TPhysicalDeviceInfo`类中增加`VkPhysicalDeviceVulkan12Features vulkan12Feature`成员变量
  >* `./engine/core`下的`TPhysicalDeviceInfo.h`中的`TPhysicalDeviceInfo`类中增加`VkPhysicalDeviceVulkan13Features vulkan13Feature`成员变量
  >* `./engine/core`下的`TVulkanLoader.h`中增加`extern VULKAN_GLOBAL_API VULKAN_CORE PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2`声明
  >* `./engine/core`下的`TVulkanLoader.h`中`TVulkanLoader`类中`LoadAllInstanceFunctions(TInstance *instance)`增加对于`Turbo::Core::vkGetPhysicalDeviceFeatures2`函数的获取
  >* `./engine/core`下的`TPhysicalDevice.h`中`TPhysicalDevice`类中`EnumerateProperties()`成员函数中增加对于`VkPhysicalDeviceVulkan11Features`，`VkPhysicalDeviceVulkan12Features`和`VkPhysicalDeviceVulkan13Features`数据的获取

* 2023/3/22 设计架构
  >
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool samplerAnisotropy`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`增加`bool logicOp`成员变量声明
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDevice`移除`VkPhysicalDeviceFeatures GetDeviceFeatures()`成员函数
  >* `./engine/core`下的`TPhysicalDevice.h`中`class TPhysicalDevice`增加`TPhysicalDeviceFeatures GetDeviceFeatures()`成员函数
  >* `./engine/core`下的`TDevice.h`中`class TDevice`中移除`TDevice(TPhysicalDevice *, std::vector<TLayerInfo> *, std::vector<TExtensionInfo> *, VkPhysicalDeviceFeatures *)`构造函数
  >* `./engine/core`下的`TDevice.h`中`class TDevice`中移除`VkPhysicalDeviceFeatures enabledFeatures`成员变量
  >* `./engine/core`下的`TDevice.h`中`class TDevice`中增加`TPhysicalDeviceFeatures enabledFeatures`成员变量
  >* `./engine/core`下的`TDevice.h`中`class TDevice`中移除`VkPhysicalDeviceFeatures GetEnableDeviceFeatures()`成员函数
  >* `./engine/core`下的`TDevice.h`中`class TDevice`中增加`TPhysicalDeviceFeatures GetEnableDeviceFeatures()`成员函数
  >* `./engine/core`下的`TDevice.h`中`class TDevice`中`InternalCreate()`成员函数中增加对于`Vulkan1.1`，`Vulkan1.2`和`Vulkan1.3`的`Feature`激活
  >* `./engine/core`下的`TVulkanLoader.h`中`class TVulkanLoader`中增加对于`vkCmdBeginRendering`和`vkCmdEndRendering`两个函数的获取
  >* 更新`./docs/Design/Core.md`文档

* 2023/3/25 设计架构
  >
  >* `./engine/core`下的新建`TRenderingPipeline.h`和`TRenderingPipeline.cpp`
  >* `./engine/core`下`TRenderingPipeline.h`中增加声明`TRenderingPipeline`类
  >* `./engine/core`下`TRenderingPipeline.h`中增加声明`TRenderingAttachments`类
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`std::vector<TFormatType> colorAttachmentFormats`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`TFormatType depthAttachmentFormat`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`TFormatType stencilAttachmentFormat`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`void AddColorAttachmentFormat(TFormatType formatType)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`void SetDepthAttachmentFormat(TFormatType formatType)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`void SetStencilAttachmentFormat(TFormatType formatType)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`const std::vector<TFormatType> &GetColorAttachmentFormats() const`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`const TFormatType &GetDepthAttachmentFormat() const`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingAttachments`类中增加`const TFormatType &GetStencilAttachmentFormat() const`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加``成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TRenderingAttachments renderingAttachments`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TTopologyType topology`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool primitiveRestartEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`std::vector<TVertexBinding> vertexBindings`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool depthClampEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool rasterizerDiscardEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TPolygonMode polygonMode`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TCullModes cullMode`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TFrontFace frontFace`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool depthBiasEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float depthBiasConstantFactor`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float depthBiasClamp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float depthBiasSlopeFactor`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float lineWidth`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool multisampleEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TSampleCountBits sample`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool depthTestEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool depthWriteEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TCompareOp depthCompareOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool depthBoundsTestEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool stencilTestEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TStencilOp frontFailOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TStencilOp frontPassOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TStencilOp frontDepthFailOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TCompareOp frontCompareOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`uint32_t frontCompareMask`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`uint32_t frontWriteMask`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`uint32_t frontReference`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TStencilOp backFailOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TStencilOp backPassOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TStencilOp backDepthFailOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TCompareOp backCompareOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`uint32_t backCompareMask`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`uint32_t backWriteMask`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`uint32_t backReference`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float minDepthBounds`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float maxDepthBounds`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool logicOpEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TLogicOp logicOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`bool blendEnable`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TBlendFactor srcColorBlendFactor`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TBlendFactor dstColorBlendFactor`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TBlendOp colorBlendOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TBlendFactor srcAlphaBlendFactor`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TBlendFactor dstAlphaBlendFactor`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`TBlendOp alphaBlendOp`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float constantR`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float constantG`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float constantB`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`float constantA`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`virtual void InternalCreate() override`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`Turbo::Core::TRenderingPipeline::TRenderingPipeline(const TRenderingAttachments &renderingAttachments, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TTopologyType topology, bool primitiveRestartEnable, bool depthClampEnable, bool rasterizerDiscardEnable, TPolygonMode polygonMode, TCullModes cullMode, TFrontFace frontFace, bool depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, bool multisampleEnable, TSampleCountBits sample, bool depthTestEnable, bool depthWriteEnable, TCompareOp depthCompareOp, bool depthBoundsTestEnable, bool stencilTestEnable, TStencilOp frontFailOp, TStencilOp frontPassOp, TStencilOp frontDepthFailOp, TCompareOp frontCompareOp, uint32_t frontCompareMask, uint32_t frontWriteMask, uint32_t frontReference, TStencilOp backFailOp, TStencilOp backPassOp, TStencilOp backDepthFailOp, TCompareOp backCompareOp, uint32_t backCompareMask, uint32_t backWriteMask, uint32_t backReference, float minDepthBounds, float maxDepthBounds, bool logicOpEnable, TLogicOp logicOp, bool blendEnable, TBlendFactor srcColorBlendFactor, TBlendFactor dstColorBlendFactor, TBlendOp colorBlendOp, TBlendFactor srcAlphaBlendFactor, TBlendFactor dstAlphaBlendFactor, TBlendOp alphaBlendOp, float constantR, float constantG, float constantB, float constantA) : Turbo::Core::TPipeline(vertexShader->GetDevice(), vertexShader, fragmentShader)`成员函数

* 2023/3/26 设计架构
  >
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`virtual void InternalDestroy() override`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`TRenderingPipeline`类中增加`~TRenderingPipeline()`析构函数

* 2023/3/27 设计架构
  >
  >* 完成`./sample`下的`VulkanDynamicRenderingTest`示例

* 2023/3/29 设计架构
  >
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`重命名为`class TAttachmentsFormat`
  >* `./engine/core`下`TRenderingPipeline.h`中增加`class TRenderingAttachments`
  >* `./engine/core`下`TCommandBuffer.h`中`class TCommandBufferBase`增加`void CmdBeginRendering(const TRenderingAttachments &renderingAttachment)`成员函数
  >* `./engine/core`下`TCommandBuffer.h`中`class TCommandBufferBase`增加`void CmdEndRendering()`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中增加`typedef enum TResolveModeBits`枚举
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`struct TRenderingAttachment`结构体声明
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`std::vector<TRenderingAttachment> colorAttachments`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`TRenderingAttachment depthAttachment`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`TRenderingAttachment stencilAttachment`成员变量
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`void AddColorAttachment(TImageView *imageView, TImageLayout layout, TImageView *resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`void AddColorAttachment(TImageView *imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`void SetDepthAttachment(TImageView *imageView, TImageLayout layout, TImageView *resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`void SetDepthAttachment(TImageView *imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`void SetStencilAttachment(TImageView *imageView, TImageLayout layout, TImageView *resolveImageView, TImageLayout resolveLayout, TResolveModeBits resolveModeBits, TLoadOp loadOp, TStoreOp storeOp)`成员函数
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`void SetStencilAttachment(TImageView *imageView, TImageLayout layout, TLoadOp loadOp, TStoreOp storeOp)`成员函数
  >* 更新`./docs/Design/Core.md`文档

* 2023/3/30 设计架构
  >
  >* `./engine/core`下`TRenderingPipeline.h`中`class TRenderingAttachments`中增加`friend class TCommandBufferBase`友元类声明，使得在`TCommandBufferBase`中记录指令时能够访问到对应数据
  >* `./engine/core`下`TCommandBuffer.h`中`class TCommandBufferBase`完善实现`void CmdBeginRendering(const TRenderingAttachments &renderingAttachment)`成员函数
  >* `./engine/core`下`TCommandBuffer.h`中`class TCommandBufferBase`完善实现`void CmdEndRendering()`成员函数
  >* `./engine/core`下`TCommandBuffer.h`中`class TCommandBufferBase`中`void CmdBeginRendering(const TRenderingAttachments &renderingAttachment)`成员函数增加形参为`void CmdBeginRendering(const TRenderingAttachments &renderingAttachment, uint32_t offsetX, uint32_t offsetY, uint32_t width, uint32_t height)`
  >* `./samples`中增加`DynamicRenderingTest`例子，用于测试`Turbo`封装的`Vulkan1.3`中`Dynamic Rendering`特性

* 2023/4/3 设计架构
  >
  >* `./engine/core`下`TSurface`构造函数中增加对于`Surface`扩展是否已经激活的判断
  >* `./engine/core`下`TSwapchain`构造函数中增加对于`Swapchain`扩展是否已经激活的判断

* 2023/4/4 设计架构
  >
  >* `./asset/images`下增加`RockCliffLayered`文件夹，用于存储`RockCliffLayered`的`PBR`纹理
  >* `./samples`中增加`NormalTexture`例子，用于尝试使用法线纹理获得切线空间的法线

* 2023/4/5 设计架构
  >
  >* `./samples`中更新完善`NormalTexture`例子
  >* `./samples`中增加`BRDF`例子，用于尝试实现常见的`PBR`纹理流程渲染

* 2023/4/6 设计架构
  >
  >* `./samples`中修改`NormalTexture`例子中`my_buffer`的`TBuffer`的大小值，之前是`sizeof(float)`应该为`sizeof(my_buffer_data)`此为一个`Bug`，现修正。

* 2023/4/7 设计架构
  >
  >* `./samples`中更新完善`BRDF`例子

* 2023/4/8 设计架构
  >
  >* `./samples`中增加`CPPMultithreading`例子，用于研究`C++`的多线程特性

* 2023/4/10 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档，研究`Vulkan`中的细分着色器。
  >* `./samples`中增加`TessellationTest`例子，用于研究测试`Vulkan`的细分特性
  >* `./engine/core`下`TShader.h`中增加`class TTessellationControlShader`类
  >* `./engine/core`下`TShader.h`中增加`class TTessellationEvaluationShader`类
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, uint32_t patchControlPoints, ...)`构造函数
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`uint32_t patchControlPoints`成员变量
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`中增加`TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)`构造函数
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`的`InternalCreate()`成员函数中增加对`VkPipelineTessellationStateCreateInfo`的设置

* 2023/4/11 设计架构
  >
  >* `./asset/shaders`中增加`TessellationTest.vert`细分示例使用的顶点着色器
  >* `./asset/shaders`中增加`TessellationTest.tesc`细分示例使用的细分控制着色器
  >* `./asset/shaders`中增加`TessellationTest.tese`细分示例使用的细分评估着色器
  >* `./asset/shaders`中增加`TessellationTest.frag`细分示例使用的片元着色器
  >* `./samples`中完成`TessellationTest`例子

* 2023/4/12 设计架构
  >
  >* `./engine/core`下`TShader.h`中增加`class TGeometryShader`类
  >* `./samples`中增加`GeometryShaderTest`例子，用于测试研究`Vulkan`中的几何着色器特性
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, ...)`支持几何着色器的构造函数
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`中增加`TPipeline(TDevice *device, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)`支持几何着色器的构造函数
  >* `./asset/shaders`中增加`GeometryTest.vert`几何着色器示例使用的顶点着色器
  >* `./asset/shaders`中增加`GeometryTest.geom`几何着色器示例使用的几何着色器
  >* `./asset/shaders`中增加`GeometryTest.frag`几何着色器示例使用的片元着色器
  >* `./engine/core`下`TDescriptorSetLayout.h`中`class TDescriptorSetLayout`中`InternalCreate()`成员函数中增加对于重复`Binding`描述符号的检查筛选，并且合并不同着色器使用相同描述符的`ShaderStageFlag`

* 2023/4/14 设计架构
  >
  >* `./samples`中增加`TessellationAndGeometry`例子，用于测试研究`Vulkan`中的细分着色器和几何着色器配合使用的特性
  >* `./asset/shaders`中增加`TessellationAndGeometry.vert`细分和几何着色器示例使用的顶点着色器
  >* `./asset/shaders`中增加`TessellationAndGeometry.tesc`细分和几何着色器示例使用的细分控制着色器
  >* `./asset/shaders`中增加`TessellationAndGeometry.tese`细分和几何着色器示例使用的细分评估着色器
  >* `./asset/shaders`中增加`TessellationAndGeometry.geom`细分和几何着色器示例使用的几何着色器
  >* `./asset/shaders`中增加`TessellationAndGeometry.frag`细分和几何着色器示例使用的片元着色器
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, uint32_t patchControlPoints, ...)`支持细分和几何着色器的构造函数
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`中增加`TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr)`支持细分和几何着色器的构造函数

* 2023/4/16 设计架构
  >
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TPipelineCache *pipelineCache, TRenderPass *renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, ...)`支持管线缓存的细分着色器的图形管线构造函数
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TPipelineCache *pipelineCache, TRenderPass*renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TGeometryShader*geometryShader, TFragmentShader *fragmentShader, ...)`支持管线缓存的几何着色器的图形管线构造函数
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TPipelineCache *pipelineCache, TRenderPass*renderPass, uint32_t subpass, std::vector<TVertexBinding> &vertexBindings, TVertexShader *vertexShader, TTessellationControlShader*tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader*geometryShader, TFragmentShader *fragmentShader, ...)`支持管线缓存的细分和几何着色器的图形管线构造函数
  >* 更新`./docs/Design/Core.md`文档，研究`Mesh Shader`。
  >* `./samples`中增加`VulkanFeatureTest`例子，用于研究`Vulkan`特性

* 2023/4/17 设计架构
  >
  >* `./samples`中移除`VulkanFeatureTest`例子，改用`VulkanTest`进行`Vulkan`接口测试
  >* 更新`./engine/core/include`下的`Vulkan`头文件，当前的`VK_HEADER_VERSION`为`204`有点老，更新到`247`，并且增加`Vulkan`的`vk_video`头文件夹

* 2023/4/18 设计架构
  >
  >* 更新`./engine/core`下的`TVulkanLoader.h`中`TVulkanLoader`类中的`GetVulkanVersion()`函数，将其更改为静态函数，并更新内部算法
  >* 更新`./engine/core`下的`TInstance.h`中`TInstance`类中的`IsSupportVulkan()`函数
  >* 更新`./engine/core`下的`TInstance.h`中`TInstance`类中的`GetVulkanInstanceVersion()`函数
  >* 更新`./docs/Design/Core.md`文档

* 2023/4/19 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./engine/core`下`TCore.h`中增加`#define VULKAN_PHYSICAL_DEVICE_API`声明
  >* `./engine/core`下`TVulkanLoader.h`中增加`struct TPhysicalDeviceFunctionTable`声明
  >* `./engine/core`下`TVulkanLoader.h`中增加`using TPhysicalDeviceDriver = TPhysicalDeviceFunctionTable`声明
  >* `./engine/core`下`TPhysicalDevice.h`中增加`struct TPhysicalDeviceFunctionTable`声明
  >* `./engine/core`下`TPhysicalDevice.h`中增加`using TPhysicalDeviceDriver = TPhysicalDeviceFunctionTable`声明
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDevice`类中增加`TPhysicalDeviceDriver *physicalDeviceDriver`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDevice`类中增加`const TPhysicalDeviceDriver* GetPhysicalDeviceDriver()`成员函数
  >* `./engine/core`下`TVulkanLoader.h`中`TVulkanLoader`类中增加`TPhysicalDeviceDriver LoadPhysicalDeviceDriver(TPhysicalDevice *physicalDevice)`成员函数
  >* 更新`./engine/core`下`TPhysicalDevice.h`中`TPhysicalDevice`类中`InternalCreate()`增加`TPhysicalDeviceDriver* physicalDeviceDriver`成员变量的构建和初始化
  >* 更新`./engine/core`下`TPhysicalDevice.h`中`TPhysicalDevice`类中`InternalDestroy()`增加`TPhysicalDeviceDriver* physicalDeviceDriver`成员变量回收释放
  >* 更新`./engine/core`下`TPhysicalDevice.h`中`TPhysicalDevice`类中使用`TPhysicalDeviceDriver* physicalDeviceDriver`进行`Vulkan`函数调用
  >* `./engine/core`下`TVulkanLoader.h`中移除全局的`PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion`声明
  >* `./engine/core`下`TVulkanLoader.h`中移除全局的`PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2`声明
  >* `./engine/core`下`TVulkanLoader.h`中移除全局的`PFN_vkCmdBeginRendering vkCmdBeginRendering`声明
  >* `./engine/core`下`TVulkanLoader.h`中移除全局的`PFN_vkCmdEndRendering vkCmdEndRendering`声明

* 2023/4/20 设计架构
  >
  >* `./engine/core`下`TPhysicalDeviceInfo`类中增加`VkPhysicalDeviceMeshShaderFeaturesEXT meshShaderFeaturesEXT`和`VkPhysicalDeviceMeshShaderFeaturesNV meshShaderFeaturesNV`的成员变量
  >* `./engine/core`下`TPhysicalDevice`类中`EnumerateProperties()`成员函数中增加对`VkPhysicalDeviceMeshShaderFeaturesEXT`和`VkPhysicalDeviceMeshShaderFeaturesNV`的特性获取和赋值
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool taskShaderNV`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool meshShaderNV`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool taskShaderEXT`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool meshShaderEXT`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool multiviewMeshShaderEXT`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool primitiveFragmentShadingRateMeshShaderEXT`成员变量
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool meshShaderQueriesEXT`成员变量
  >* `./engine/core`下`TPhysicalDevice`类中`GetDeviceFeatures()`中增加对`Mesh Shader`特性的赋值
  >* `./samples`中增加`MeshShaderTest`例子，用于研究`Mesh Shader`
  >* `./engine/core`下`TDevice`类中`InternalCreate()`中增加对`Mesh Shader`特性的赋值
  
* 2023/4/21 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./engine/core`下`TExtensionInfo.h`中`TExtensionType`枚举中增加`VK_EXT_MESH_SHADER`枚举量
  >* `./engine/core`下`TExtensionInfo.cpp`中`TAllExtensionNames`数组中增加`VK_EXT_mesh_shader`字符串
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加`PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT`成员变量，并在`TDeviceDriver::LoadDeviceDriver(...)`中对应获取
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加`PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT`成员变量，并在`TDeviceDriver::LoadDeviceDriver(...)`中对应获取
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加`PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT`成员变量，并在`TDeviceDriver::LoadDeviceDriver(...)`中对应获取
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加`PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV`成员变量，并在`TDeviceDriver::LoadDeviceDriver(...)`中对应获取
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加`PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV`成员变量，并在`TDeviceDriver::LoadDeviceDriver(...)`中对应获取
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加`PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV`成员变量，并在`TDeviceDriver::LoadDeviceDriver(...)`中对应获取
  >* `./engine/core`下`TCommandBuffer.h`中`TCommandBufferBase`类中增加`void CmdDrawMeshTasksEXT(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)`成员函数
  >* `./engine/core`下`TCommandBuffer.h`中`TCommandBufferBase`类中增加`void CmdDrawMeshTasksNV(uint32_t taskCount, uint32_t firstTask)`成员函数

* 2023/4/22 设计架构
  >
  >* `./engine/core`下`TDevice.h`中`TDevice`类中增加`void InspectExtensionAndVersionDependencies()`成员函数用于检查扩展依赖
  >* `./engine/core`下`TDevice.h`中`TDevice`类中`InternalCreate()`成员函数中增加对于设备扩展依赖的检查
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDevice`类中增加`TExtensionInfo GetExtensionByType(TExtensionType extensionType)`成员函数

* 2023/4/23 设计架构
  >
  >* `./engine/core`下`TDevice.h`中`TDevice`类中更新`void InspectExtensionAndVersionDependencies()`成员函数用于检查扩展依赖
  >* `./engine/core`下`TShader.h`中`TShader`类中更新`TShader(...)`构造函数中对于`glslang::EShTargetClientVersion`和`glslang::EShTargetLanguageVersion`的设置，如果想使用`Mesh Shader`特性，`Spir-V`的版本需要大于等于`1.4`
  >* `./engine/core`下`TShader.h`中`TShaderType`枚举中增加`TASK`枚举值，用于`Task Shader`
  >* `./engine/core`下`TShader.h`中`TShaderType`枚举中增加`MESH`枚举值，用于`Mesh Shader`
  >* `./engine/core`下`TShader.cpp`中`TShaderTypeToGlslangEShLanguage(...)`增加对于`Turbo::Core::TShaderType::TASK`到`EShLanguage::EShLangTaskNV`的转换
  >* `./engine/core`下`TShader.cpp`中`TShaderTypeToGlslangEShLanguage(...)`增加对于`Turbo::Core::TShaderType::MESH`到`EShLanguage::EShLangMeshNV`的转换
  >* `./engine/core`下`TShader.h`中`TShader`类中`GetVkShaderStageFlags()`成员函数中增加对于`Turbo::Core::TShaderType::TASK`到`VK_SHADER_STAGE_TASK_BIT_EXT`的转换
  >* `./engine/core`下`TShader.h`中`TShader`类中`GetVkShaderStageFlags()`成员函数中增加对于`Turbo::Core::TShaderType::MESH`到`VK_SHADER_STAGE_MESH_BIT_EXT`的转换
  >* `./engine/core`下`TShader.h`中`TShader`类中`GetVkShaderStageFlagBits()`成员函数中增加对于`Turbo::Core::TShaderType::TASK`到`VK_SHADER_STAGE_TASK_BIT_EXT`的转换
  >* `./engine/core`下`TShader.h`中`TShader`类中`GetVkShaderStageFlagBits()`成员函数中增加对于`Turbo::Core::TShaderType::MESH`到`VK_SHADER_STAGE_MESH_BIT_EXT`的转换
  >* `./engine/core`下`TShader.h`中增加`TTaskShader`类，用于表示`Task Shader`
  >* `./engine/core`下`TShader.h`中增加`TMeshShader`类，用于表示`Mesh Shader`
  >* `./asset/shaders`中增加`MeshShaderTest.mesh`的网格着色器文件
  >* `./asset/shaders`中增加`MeshShaderTest.frag`的片元着色器文件

* 2023/4/24 设计架构
  >
  >* `./engine/core/thirdparty`下的`glslang`库改成`git`的`submodule`进行管理，随着`Vulkan`的版本升级，`glslang`也会跟随更新使得符合`Vulkan`的新标准（这样`Mesh Shader`就支持解析相应`GLSL`扩展了）
  >* `./engine/core`下的`TShader.cpp`中增加`glslang/Public/ShaderLang.h`头文件，用于使用`glslang`中`GetDefaultResource()`函数
  >* `./engine/core`下的`CMakeLists.txt`中增加`glslang-default-resource-limits`库包含
  >* `./engine/core`下的`TShader.h`中`TShader`类的构造函数中从`hader_glslang.parse(&resources, ...)`修改为`shader_glslang.parse(GetDefaultResources(), ...)`
  >* `./engine/core`下的`TShader.h`中`TShader`类的构造函数中移除`TBuiltInResource resources`的声明和相关使用
  >* `./engine/core`下的`TShader.cpp`中`TShaderTypeToGlslangEShLanguage(...)`中返回的`EShLangTaskNV`更改为`EShLangTask`
  >* `./engine/core`下的`TShader.cpp`中`TShaderTypeToGlslangEShLanguage(...)`中返回的`EShLangMeshNV`更改为`EShLangMesh`
  >* 更新`./docs/Design/Core.md`文档

* 2023/4/25 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`中增加`TGraphicsPipeline(TRenderPass *renderPass, uint32_t subpass, TMeshShader *meshShader, TFragmentShader *fragmentShader, ...)`支持`Mesh Shader`的图形管线构造函数
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`中增加`TPipeline(TDevice *device, TMeshShader *meshShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache)`支持`Mesh Shader`的管线基类构造函数

* 2023/4/26 设计架构
  >
  >* 更新`./docs/FAQ.md`文档
  >* `./samples`中增加`SpecializationConstantsTest`例子，用于研究着色器中的常量设定
  >* `./engine/core`下`TShader.h`中`class TShader`中`InternalParseSpirV()`成员函数中增加对于`Specialization Constants`特化常量的解析

* 2023/4/27 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./asset/shaders`中增加`SpecializationConstantsTest.comp`的计算着色器文件，用于测试特化常量

* 2023/4/28 设计架构
  >
  >* `./engine/core`下`TShader.h`中增加`class TSpecializationConstant`用于描述特化常量
  >* `./engine/core`下`TShader.h`中`class TShader`中增加`std::vector<TSpecializationConstant> specializationConstants`成员变量
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`uint32_t id`成员变量
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`std::string name`成员变量
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`Turbo::Core::TDescriptorDataType descriptorDataType`成员变量
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`uint32_t width`成员变量
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`TSpecializationConstant(uint32_t id, const std::string &name, Turbo::Core::TDescriptorDataType descriptorDataType, uint32_t width)`构造函数
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`uint32_t GetConstantID()`成员函数
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`const std::string &GetName()`成员函数
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`Turbo::Core::TDescriptorDataType GetDescriptorDataType()`成员函数
  >* `./engine/core`下`TShader.h`中`class TSpecializationConstant`类中增加`uint32_t GetWidth()`成员函数
  >* `./engine/core`下`TShader.h`中`class TShader`中`InternalParseSpirV()`成员函数中完善对于`Specialization Constants`特化常量的解析
  >* `./engine/core`下`TShader.h`中`class TShader`中增加`const std::vector<TSpecializationConstant> &GetSpecializationConstants()`成员函数
  >* `./engine/core`下`TPipeline.h`中增加`class TSpecializations`类
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`union TConstant`成员`union`类型
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`struct TConstValue`成员类型
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`std::map<uint32_t, TConstValue> specializationMap`成员变量
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`std::map<uint32_t, TConstValue> specializationMap`成员变量
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`void SetConstant(uint32_t id, bool value)`成员函数
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`void SetConstant(uint32_t id, int32_t value)`成员函数
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`void SetConstant(uint32_t id, uint32_t value)`成员函数
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`void SetConstant(uint32_t id, float value)`成员函数
  >* `./engine/core`下`TPipeline.h`中`class TSpecializations`类中增加`void SetConstant(uint32_t id, double value)`成员函数
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中增加`std::vector<TSpecializationConstant> specializationConstants`成员变量
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中增加`TSpecializations specializations`成员变量
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中所有的构造函数增加`TSpecializations *specializations`形参
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中更新`InternalCreate()`成员函数，统计特化常量
  >* `./engine/core`下`TComputePipeline.h`中`class TComputePipeline`类中增加`TComputePipeline(TSpecializations *specializations, TComputeShader *computeShader)`构造函数
  >* `./engine/core`下`TComputePipeline.h`中`class TComputePipeline`类中增加`TComputePipeline(TPipelineCache *pipelineCache, TSpecializations *specializations, TComputeShader *computeShader)`构造函数

* 2023/4/29 设计架构
  >
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中移除`std::vector<TSpecializationConstant> specializationConstants`成员变量
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中增加`std::vector<TSpecializationConstant> GetSpecializationConstants()`成员函数
  
* 2023/5/2 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./engine/core`下`TPipeline.h`中移除`TSpecializations`类和相关声明
  >* `./engine/core`下将`TSpecializations`中的相关声明转移至`TShader.h`中的`TShader`类中
  >* `./engine/core`下`TPipeline.h`中`class TPipeline`类中移除`std::vector<TSpecializationConstant> GetSpecializationConstants()`成员函数

* 2023/5/4 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档
  >* `./engine/core`下`TComputePipeline.h`中`class TComputePipeline`类中`InternalCreate()`成员函数中增加对`VkSpecializationInfo`的解析
  >* `./asset/shaders`中增加`post_show_texture.frag`片元着色器文件，用于在后处理中采样显示纹理
  >* 更新`./sample`下的`SpecializationConstantsTest`示例

* 2023/5/5 设计架构
  >
  >* `./engine/core`下`TComputePipeline.h`中`class TComputePipeline`类中`InternalCreate()`成员函数中更新对`VkSpecializationInfo`的解析，之前`constant_width`计算有误
  >* 更新`./sample`下的`SpecializationConstantsTest`示例
  >* `./engine/core`下`TGraphicsPipeline.h`中`class TGraphicsPipeline`类中`InternalCreate()`成员函数中增加对`VkSpecializationInfo`的解析
  >* `./asset/shaders`中增加`SpecializationConstantsTest.vert`顶点着色器文件，用于测试图形管线特化常量
  >* `./asset/shaders`中增加`SpecializationConstantsTest.frag`片元着色器文件，用于测试图形管线特化常量

* 2023/5/6 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档。开始研究`Vulkan`硬件实时光追

* 2023/5/7 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档

* 2023/5/8 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档

* 2023/5/9 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档

* 2023/5/10 设计架构
  >
  >* 更新`./docs/Design/Core.md`文档

* 2023/6/5 设计架构
  >
  >* `./sample`下增加`VulkanKHRRayTracingTest`示例，用于研究`Vulkan KHR`标准硬件实时光追。相关研究文档请查阅[Vulkan KHR 光线追踪标准](https://fuxiii.github.io/Essentials.of.Vulkan/InformalEssay/KHRRayTracing.html)。
  >* `./engine/core`下`TExtensionInfo.h`下的`TExtensionType`枚举中增加`VK_KHR_ACCELERATION_STRUCTURE`枚举量。并且在对应的`TExtensionInfo.cpp`中的`TAllExtensionNames`中增加`VK_KHR_acceleration_structure`名称。

* 2023/6/6 设计架构
  >
  >* `./engine/core`下`TPhysicalDeviceInfo.h`下的`TPhysicalDeviceInfo`类中增加`VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeaturesKHR`成员变量，用于存储加速结构特性信息。
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDevice`类中`EnumerateProperties`函数中增加对于`VkPhysicalDeviceAccelerationStructureFeaturesKHR`加速结构特性信息的获取和赋值。
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDeviceFeatures`类中增加`bool accelerationStructure`、`bool accelerationStructureIndirectBuild`、`bool accelerationStructureHostCommands`和`bool descriptorBindingAccelerationStructureUpdateAfterBind`成员。用于获取和激活加速结构。
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDevice`类中`GetDeviceFeatures()`函数中，增加对于`bool accelerationStructure`、`bool accelerationStructureIndirectBuild`、`bool accelerationStructureHostCommands`和`bool descriptorBindingAccelerationStructureUpdateAfterBind`成员的赋值和获取。
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中`InternalCreate()`函数中，增加对于`VkPhysicalDeviceAccelerationStructureFeaturesKHR`特性的激活判断赋值。

* 2023/6/7 设计架构
  >
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中`InternalCreate()`增加扩展指针链对于`VkPhysicalDeviceAccelerationStructureFeaturesKHR`特性的激活。
  >* 更新`./samples`下的`VulkanKHRRayTracingTest`示例

* 2023/6/12 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中增加对`VK_KHR_acceleration_structure`相关函数声明。
  >* `./engine/core`下`TVulkanLoader.h`下的`TVulkanLoader`类中`LoadDeviceDriver`成员函数增加对`VK_KHR_acceleration_structure`相关函数的获取。

* 2023/6/13 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下的`TVulkanLoader`类中`LoadDeviceDriver`成员函数增加对获取扩展函数之前判断是否支持相应扩展的判断。

* 2023/6/15 设计架构
  >
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDevice`类中`IsSupportExtension`成员函数，将扩展名再次转换成扩展类型枚举进行判断去掉，没必要且重复。
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中`IsEnabledExtension`成员函数，将扩展名再次转换成扩展类型枚举进行判断去掉，没必要且重复。

* 2023/6/20 设计架构
  >
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中更新`InspectExtensionAndVersionDependencies()`成员函数为`InspectExtensionAndVersionDependencies(TExtensionInfo& extensionInfo)`。应该使用递归的方式检查依赖。
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中更新`InternalCreate()`成员函数。检查依赖。
  >* `./engine/core`下`TBuffer.h`下的`TBufferUsageBits`枚举中增加`BUFFER_SHADER_DEVICE_ADDRESS`、`BUFFER_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY`、`BUFFER_ACCELERATION_STRUCTURE_STORAGE`和`BUFFER_SHADER_BINDING_TABLE`枚举成员枚举。
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中增加对`VK_KHR_buffer_device_address`相关函数声明。
  >* `./engine/core`下`TVulkanLoader.h`下的`TVulkanLoader`类中更新`LoadDeviceDriver`成员函数。增加对`VK_KHR_buffer_device_address`相关函数获取。

* 2023/6/21 设计架构
  >
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中更新`InspectExtensionAndVersionDependencies()`成员函数中增加`VK_KHR_MAINTENANCE2`扩展依赖检查。
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中更新`InspectExtensionAndVersionDependencies()`成员函数中移除对`VK_KHR_DEVICE_GROUP_CREATION`和`VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES2`扩展依赖检查，这两个依赖属于`instance`依赖，不应该出现在`device`扩展中。

* 2023/6/25 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_KHR_get_physical_device_properties2`扩展的函数。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_KHR_external_memory_capabilities`扩展的函数。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_KHR_external_fence_capabilities`扩展的函数。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_KHR_external_semaphore_capabilities`扩展的函数。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_EXT_tooling_info`扩展的函数。

* 2023/6/27 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_EXT_buffer_device_address`扩展的函数。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_KHR_external_semaphore_capabilities`提升至核心得函数。
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中`InspectExtensionAndVersionDependencies()`成员函数中增加对`VK_EXT_BUFFER_DEVICE_ADDRESS`扩展依赖检查。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中增加`VK_KHR_device_group`扩展的函数。
  >* `./engine/core`下`TVulkanLoader.h`下增加`TInstanceFunctionTable`结构体并声明`TInstanceDriver`为`TInstanceFunctionTable`别名。
  >* `./engine/core`下`TVulkanLoader.h`下`TInstanceFunctionTable`结构体中增加`VK_KHR_device_group_creation`扩展的函数。

* 2023/6/29 设计架构
  >
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDeviceFeatures`类中增加`bool bufferDeviceAddress`成员。
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDeviceFeatures`类中增加`bool bufferDeviceAddressCaptureReplay`成员。
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDeviceFeatures`类中增加`bool bufferDeviceAddressMultiDevice`成员。
  >* `./engine/core`下`TPhysicalDevice.h`下的`EnumerateProperties()`成员函数中增加对`VK_KHR_buffer_device_address`特性的获取。
  >* `./engine/core`下`TPhysicalDeviceInfo.h`下的`TPhysicalDeviceInfo`类中增加对`VkPhysicalDeviceBufferDeviceAddressFeaturesKHR physicalDeviceBufferDeviceAddressFeaturesKHR`成员变量。
  >* `./engine/core`下`TPhysicalDevice.h`下的`TPhysicalDevice`类中`GetDeviceFeatures()`成员函数中增加对`VK_KHR_buffer_device_address`特性的获取。

* 2023/6/30 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下的`TInstanceFunctionTable`结构体中，增加`Vulkan 1.0`的函数。
  >* `./engine/core`下`TVulkanLoader.h`下的`TVulkanLoader`类中，增加`LoadInstanceDriver`成员函数。用于获取`instance`函数。

* 2023/7/1 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下增加`vkEnumerateInstanceVersion`的`Vulkan`全局函数，并在`TVulkanLoader`构造函数中获取。
  >* `./engine/core`下`TVulkanLoader.h`下移除所有非`Vulkan`全局函数。
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类的`Load`函数增加`PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr`形参。
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类中移除`LoadDeviceFunction(TInstance *instance, const char *name)`成员函数。
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类中移除`LoadDeviceFunction(VkInstance instance, const char *name)`成员函数。
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类中`LoadDeviceFunction(VkDevice device, const char *name)`成员函数增加`PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr`形参
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类中移除`void LoadAllInstanceFunctions(TInstance *instance)`成员函数
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类中移除`void LoadAllDeviceFunctions(TInstance *instance)`成员函数
  >* `./engine/core`下`TVulkanLoader.h`下`TVulkanLoader`类中移除`void LoadAll(TInstance *instance)`成员函数
  >* `./engine/core`下`TInstance.h`下`TInstance`类中增加`TInstanceDriver *instanceDriver = nullptr`成员变量，并在`InternalCreate`成员函数中分配获取，在`InternalDestroy`中回收
  >* `./engine/core`下`TInstance.h`下`TInstance`类中增加`const TInstanceDriver *GetInstanceDriver()`成员函数
  >* `./engine/core`下`TDevice.h`下`TDevice`类中`InternalCreate`中`vkCreateDevice`函数使用`Turbo::Core::TPhysicalDeviceDriver`中的`vkCreateDevice`创建
  >* `./engine/core`下`TPhysicalDevice.h`下`TPhysicalDevice`类中`InternalCreate`中`vkEnumeratePhysicalDevices`函数使用`Turbo::Core::GetInstanceDriver`中的`vkEnumeratePhysicalDevices`函数
  >* `./engine/core`下`TVmaAllocator.h`下`TVmaAllocator`类中`InternalCreate`中`vkGetDeviceProcAddr`函数使用`Turbo::Core::TInstanceDriver`中的`vkGetDeviceProcAddr`函数
  >* `./docs`下`FAQ.md`下`Could Not find Vulkan (missing: VULKAN_LIBRARY VULKAN_INCLUDE_DIR)`更新最新确切的解决方法。
  >* `./engine/core`下`TVulkanLoader.h`下的`TInstanceFunctionTable`结构体中，增加`PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups`的函数并获取。
  >* `./engine/core`下`TVulkanLoader.h`下的`TPhysicalDeviceFunctionTable`结构体中`VK_EXT_tooling_info`先关函数转移至`TDeviceFunctionTable`中并获取。
  >* `./engine/core`下`TInstance.h`下的`TInstance`类中增加`void InspectExtensionAndVersionDependencies(TExtensionType extensionType)`成员函数。使用递归的方式检查依赖。并在`InternalCreate()`进行调用。
  >* `./engine/core`下`TInstance.h`下的`TInstance`类中增加`TExtensionInfo GetExtensionByType(TExtensionType extensionType)`成员函数

* 2023/7/2 设计架构
  >
  >* `./engine/core`下`TPhysicalDevice.h`下`TPhysicalDevice`类中`EnumerateProperties()`成员函数中，增加对`vkGetPhysicalDeviceFeatures2KHR`函数的判断和使用。
  >* `./engine/core`下`TDevice.h`下`TDevice`类中`InspectExtensionAndVersionDependencies()`成员函数中，移除对于扩展的`Vulkan`版本限制，对于扩展不应该使用版本限制。
  >* `./engine/core`下`TInstance.h`下`TInstance`类中`InspectExtensionAndVersionDependencies()`成员函数中，移除对于扩展的`Vulkan`版本限制，对于扩展不应该使用版本限制。

* 2023/7/3 设计架构
  >
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkCmdDispatchBase vkCmdDispatchBase`成员函数并获取。
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask`成员函数并获取。
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures`成员函数并获取。
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR`成员函数并获取。
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupport`成员函数并获取。
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR`成员函数并获取
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR`成员函数并获取
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR`成员函数并获取
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR`成员函数并获取
  >* `./engine/core`下`TVulkanLoader.h`下的`TDeviceFunctionTable`结构体中，增加`PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR`成员函数并获取
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中`InternalCreate`成员函数中，修正遍历`enabledExtensions`时调用`InspectExtensionAndVersionDependencies`导致的遍历器失效`Bug`
  >* `./engine/core`下`TInstance.h`下的`TInstance`类中`InternalCreate`成员函数中，修正遍历`enabledExtensions`时调用`InspectExtensionAndVersionDependencies`导致的遍历器失效`Bug`
  >* `./engine/core`下`TVmaAllocator.h`下的`TVmaAllocator`类中`InternalCreate`成员函数中，增加对于`VmaAllocatorCreateInfo::flags`的使用（如果要使用`buffer device address`的特性，需要开启`VmaAllocatorCreateFlagBits::VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT`标志位）
  >* `./engine/core`下`TDevice.h`下的`TDevice`类中`InternalCreate`成员函数中，增加对于`VkPhysicalDeviceBufferDeviceAddressFeaturesKHR`的使用激活设置
  >* `./docs/Design`下增加`ForVulkanFeatureNote.md`文档用于记录开发`Vulkan`特性的流程
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的设备内存地址获取

* 2023/7/4 设计架构
  >
  >* `./engine/core`下`TVmaAllocator.h`下的`TVmaAllocator`类中`InternalCreate`成员函数中，对于`VmaAllocatorCreateInfo::flags`的使用（在 `Vulkan 1.2` 标准之后被升级为核心标准）
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的加速结构

* 2023/7/5 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的加速结构，成功创建一个底层加速结构

* 2023/7/6 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索压缩实时光追的加速结构

* 2023/7/10 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的顶层加速结构

* 2023/7/11 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的顶层加速结构

* 2023/7/12 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的顶层加速结构
  >* `./engine/core`下`thirdparty`中`SPIRV-Cross`和`VulkanMemoryAllocator`转成`submodules`
  >* `./engine/core`下更新`CMakeLists.txt`适配新版的`VulkanMemoryAllocator`
  >* `README`增加`Clone`章节

* 2023/7/13 设计架构
  >
  >* `./engine/core`下`TDescriptor.h`中`TDescriptorType`枚举增加`ACCELERATION_STRUCTURE`用于加速结构描述符类型
  >* `./engine/core`下`TDescriptor.h`中增加`TAccelerationStructureDescriptor`类。用于加速结构描述符
  >* `./engine/core`下`TShader.h`中`TShader`类中增加`std::vector<TAccelerationStructureDescriptor *> accelerationStructureDescriptors`成员变量。用于存储加速结构描述符
  >* `./engine/core`下`TShader.h`中`TShader`类中增加`std::vector<TAccelerationStructureDescriptor *> accelerationStructureDescriptor`成员变量。用于存储加速结构描述符
  >* `./engine/core`下`TShader.h`中`TShader`类中`InternalParseSpirV()`成员函数中增加对`TAccelerationStructureDescriptor`的加速结构描述符的创建
  >* `./engine/core`下`TShader.h`中`TShader`类中`~TShader`析构函数中增加对`TAccelerationStructureDescriptor`的加速结构描述符的销毁
  >* `./asset/shaders`下增加`RayTraceTest.rgen`光线生成着色器，用于测试光线生成着色器。
  >* `./engine/core`下`TShader.h`中`TShaderType`枚举中增加`RAY_GENERATION`、`ANY_HIT`、`CLOSEST_HIT`、`MISS`、`INTERSECTION`、`CALLABLE`成员枚举量，用于光线追踪着色器
  >* `./engine/core`下`TShader.h`中`TShader`类中更新`GetVkShaderStageFlagBits()`成员函数，适配光线追踪着色器
  >* `./engine/core`下`TShader.cpp`中更新`TShaderTypeToGlslangEShLanguage()`全局函数，适配光线追踪着色器
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索实时光追的着色器和相关描述符
  >* `./engine/core`下`TShader.h`中增加`TRayGenerationShader`类。用于描述光线生成着色器。

* 2023/7/14 设计架构
  >
  >* `./engine/core`下`TDescriptorPool.h`中`TDescriptorPool`类中增加对`TDescriptorType::ACCELERATION_STRUCTURE`的适配
  >* `./engine/core`下`TShader.h`中`TShader`类中增加`const std::vector<TAccelerationStructureDescriptor *> &GetAccelerationStructureDescriptors()`的成员函数
  >* `./engine/core`下`TPipeline.h`中`TPipeline`类中增加对`TDescriptorType::ACCELERATION_STRUCTURE`的适配
  >* `./engine/core`下`TPipelineDescriptorSet.h`中`TPipelineDescriptorSet`类中增加`void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<VkAccelerationStructureKHR> &accelerationStructures)`的临时测试函数，用于绑定加速结构
  >* `./engine/core`下`TDescriptorSet.h`中`TDescriptorSet`类中增加`void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<VkAccelerationStructureKHR> &accelerationStructures)`的临时测试函数，用于绑定加速结构

* 2023/7/19 设计架构
  >
  >* `./samples`下增加`PureCCppWebGPUTest`项目文件夹，用于测试`C/C++`的`WebGPU`项目编译至浏览器的网页端渲染。
  >* 更新`README.md`下的`State`段落，简单记录当前开发状态。
  >* 更新`./docs/Design/WebGPU_CCPP.md`文档，增加对`PureCCppWebGPUTest`项目的说明。

* 2023/7/20 设计架构
  >
  >* `./samples`下`PureCCppWebGPUTest`项目文件夹增加`compile_flags.txt`，用于设置`clangd`的`Emscripten`的头文件目录。

* 2023/7/22 设计架构
  >
  >* `./samples`下新增`PureCCppWebGlslangAndSpirVCrossTest`项目。用于存放将`GLSL`或`HLSL`或`Spir-V`着色器代码通过`Web`网页端，在线将代码进行互相编译和转换项目。

* 2023/7/24 设计架构
  >
  >* `./samples`下新增`PureCCppWebGlfwTest`项目。用于存放`glfw`的`Web`网页端测试工程。
  >* `./samples`下新增`PureCCppWebImGuiTest`项目。用于存放`imgui`的`Web`网页端测试工程。

* 2023/7/25 设计架构
  >
  >* `./samples`下新增`PureCCppWebShaderCompiler`项目。用于存放在线`GLSL`、`HLSL`和`SPIR-V`网页端着色器转化编译器项目。
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。提供在线`GLSL`、`HLSL`转`SPIR-V`功能。

* 2023/7/26 设计架构
  >
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。增大显示大小，之前的可视范围太小了。
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。增加`Ctrl+V`粘贴代码快捷键的支持。

* 2023/7/27 设计架构
  >
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。增加反编译 ``SPIR-V`` 的功能。

* 2023/7/28 设计架构
  >
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。增加编译 ``SPIR-V`` 的功能。

* 2023/7/29 设计架构
  >
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。增加编译 `GLSL`、 `HLSL`、 `MSL`、 `C++`和`Reflection`的功能。

* 2023/7/31 设计架构
  >
  >* `./samples`下更新`PureCCppWebShaderCompiler`项目。修改`Bug`和一些错误。

* 2023/8/1 设计架构
  >
  >* `./samples`下新增`PureCCppMiniWebGPUTest`项目。一个使用`C/C++`书写的`WebGPU`的小例子。
  >* `./samples`下更新`PureCCppMiniWebGPUTest`项目。测试`WebGPU`的`C`接口。

* 2023/8/2 设计架构
  >
  >* `./samples`下更新`PureCCppMiniWebGPUTest`项目。测试`WebGPU`的`C`接口。

* 2023/8/3 设计架构
  >
  >* `./samples`下更新`PureCCppMiniWebGPUTest`项目。测试`WebGPU`的`C`接口的`WGPUCommandEncoder`和`WGPUCommandBuffer`。

* 2023/8/7 设计架构
  >
  >* `./engine/core`下新增`TRayTracingPipeline.h`和`TRayTracingPipeline.cpp`用于声明定义`TRayTracingPipeline`类
  >* `./engine/core`下`TShader.h`中增加`TAnyHitShader`类。用于描述任意命中着色器。
  >* `./engine/core`下`TShader.h`中增加`TClosestHitShader`类。用于描述最近命中着色器。
  >* `./engine/core`下`TShader.h`中增加`TMissShader`类。用于描述未命中着色器。
  >* `./engine/core`下`TShader.h`中增加`TIntersectionShader`类。用于描述相交着色器。
  >* `./engine/core`下`TShader.h`中增加`TCallableShader`类。用于描述可调用着色器。
  >* `./engine/core`下`TShader.h`中`TShader`类中增加`std::vector<uint32_t> GetSpirV()`成员函数。用于获取着色器的`Spir-V`的数据。
  >* `./asset/shaders`下新增`RayTracingKHRTest.rmiss`。表示光追测试程序使用的未命中着色器。
  >* `./asset/shaders`下`RayTraceTest.rgen`更改命名为`RayTracingKHRTest.rgen`。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`探索光追管线和着色器

* 2023/8/9 设计架构
  >
  >* `./samples`下更新`SubpassTest.cpp`。修正内部小三角形随时间变化大小。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。可控制视点方便查看渲染效果。

* 2023/8/10 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。尝试创建光追管线。
  >* `./engine/core`下`TExtensionInfo.cpp`中`TAllExtensionNames`增加`VK_KHR_ray_tracing_pipeline`元素值。用于光追管线扩展。
  >* `./engine/core`下`TExtensionInfo.h`中`TExtensionType`增加`VK_KHR_RAY_TRACING_PIPELINE`枚举值。用于与`VK_KHR_ray_tracing_pipeline`元素光追管线扩展元素对应。
  >* `./engine/core`下`TDevice.cpp`中`InspectExtensionAndVersionDependencies`中增加对`VK_KHR_RAY_TRACING_PIPELINE`扩展依赖的检查。
  >* `./engine/core`下`TVulkanLoader.h`中`TDeviceFunctionTable`结构体中增加对`VK_KHR_ray_tracing_pipeline`扩展函数的声明
  >* `./engine/core`下`TVulkanLoader.cpp`中`LoadDeviceDriver`函数中增加对`VK_KHR_ray_tracing_pipeline`扩展函数的加载获取
  >* `.docs/Design`下增加`ForVulkanExtensionNote.md`文档文件，用于记录`Vulkan Extension 开发流程`

* 2023/8/11 设计架构
  >
  >* `./engine/core`下`TPhysicalDeviceInfo.h`中`TPhysicalDeviceInfo`类中增加`VkPhysicalDeviceRayTracingPipelineFeaturesKHR physicalDeviceRayTracingPipelineFeaturesKHR`成员变量。用于存储光追管线特性信息。
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool rayTracingPipeline`成员变量。用于存储光追管线特性信息。
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool rayTracingPipelineShaderGroupHandleCaptureReplay`成员变量。用于存储光追管线特性信息。
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool rayTracingPipelineShaderGroupHandleCaptureReplayMixed`成员变量。用于存储光追管线特性信息。
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool rayTracingPipelineTraceRaysIndirect`成员变量。用于存储光追管线特性信息。
  >* `./engine/core`下`TPhysicalDevice.h`中`TPhysicalDeviceFeatures`类中增加`bool rayTraversalPrimitiveCulling`成员变量。用于存储光追管线特性信息。
  >* `./engine/core`下`TPhysicalDevice`类中`EnumerateProperties`成员函数中增加对`VkPhysicalDeviceRayTracingPipelineFeaturesKHR`特性的获取。
  >* `./engine/core`下`TPhysicalDevice`类中`GetDeviceFeatures`成员函数中增加对`VkPhysicalDeviceRayTracingPipelineFeaturesKHR`特性的设置。
  >* `./engine/core`下`TDevice`类中`InternalCreate`成员函数中增加对`VkPhysicalDeviceRayTracingPipelineFeaturesKHR`特性的设置。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。尝试创建光追管线和着色器绑定表。

* 2023/8/12 设计架构
  >
  >* `./engine/core`下`TDevice`类中`GetBestGraphicsQueue`成员函数对`score`的设置有`Bug`，修正。
  >* `./engine/core`下`TDevice`类中`GetBestComputeQueue`成员函数对`score`的设置有`Bug`，修正。
  >* `./engine/core`下`TDevice`类中`GetBestTransferQueue`成员函数对`score`的设置有`Bug`，修正。
  >* `./engine/core`下`TDevice`类中`GetBestSparseBindingQueue`成员函数对`score`的设置有`Bug`，修正。
  >* `./engine/core`下`TDevice`类中`GetBestProtectedQueue`成员函数对`score`的设置有`Bug`，修正。

* 2023/8/14 设计架构
  >
  >* `./engine/core`下`TVulkanAllocator`类中`AlignUp`静态成员函数移至`TAllocator`类中。并增加`std::is_integral`的判断。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。尝试创建光追管线和着色器绑定表。

* 2023/8/15 设计架构
  >
  >* `./engine/core`下`TAllocator`类中`AlignUp`静态成员函数修改计算错误的`Bug`。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。尝试创建光追管线和着色器绑定表。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。尝试创建用于光追的图片。

* 2023/8/16 设计架构
  >
  >* `./asset/shaders`下`RayTracingKHRTest.rgen`着色器中`set`和`binding`写反了，修正。
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。尝试创建用于光追的描述符集并进行`vkCmdTraceRaysKHR`光追指令。

* 2023/8/17 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTest.cpp`。修正一些小`Bug`和着色器矩阵不匹配的问题。现在可以输出正确光追渲染结果了。

* 2023/8/18 设计架构
  >
  >* `./samples`下增加`VulkanKHRRayTracingTestForLighting`示例。用于尝试使用硬件实时光追渲染光照。
  >* `./asset/shaders`下增加`RayTracingKHRTestForLighting.rchit`着色器。用于`VulkanKHRRayTracingTestForLighting`示例。
  >* `./asset/shaders`下增加`RayTracingKHRTestForLighting.rgen`着色器。用于`VulkanKHRRayTracingTestForLighting`示例。
  >* `./asset/shaders`下增加`RayTracingKHRTestForLighting.miss`着色器。用于`VulkanKHRRayTracingTestForLighting`示例。
  >* `./samples`下更新`VulkanKHRRayTracingTest`示例。将模型摆正。
  >* `./samples`下优化`VulkanKHRRayTracingTest`示例。丰富渲染。

* 2023/8/19 设计架构
  >
  >* `./samples`下更新`PureCCppMiniWebGPUTest`示例。研究 `WebGPU`。

* 2023/8/20 设计架构
  >
  >* `./samples`下新增`PureMiniWebGPUTest`示例。研究纯`C`语言版`WebGPU`。

* 2023/8/21 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForLighting`示例。研究光照计算。

* 2023/8/22 设计架构
  >
  >* `./samples`下新增`VulkanKHRRayTracingTestForLightingShadow`示例。研究光照计算中的阴影。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadow.rgen`着色器。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadow.rchit`着色器。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadow.rmiss`着色器。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadowShadow.rmiss`着色器。

* 2023/8/23 设计架构
  >
  >* `./samples`下新增`VulkanKHRRayTracingTestForLightingShadowWithTexture`示例。研究光追光照计算中对纹理采样。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadowWithTexture.rgen`着色器。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadowWithTexture.rchit`着色器。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadowWithTexture.rmiss`着色器。
  >* `./asset/shaders`下新增`RayTracingKHRTestForLightingShadowShadowWithTexture.rmiss`着色器。

* 2023/8/28 设计架构
  >
  >* `./samples`下新增`VulkanKHRRayTracingJitterCamera`示例。通过抖动采样点并多帧累积采样结果进行抗锯齿。
  >* `./asset/shaders`下新增`random.glsl`文件，用于生成随机数。
  >* `./asset/shaders`下新增`RayTracingKHRJitterCamera.rgen`文件，用于`VulkanKHRRayTracingJitterCamera`示例。
  >* `./asset/shaders`下新增`RayTracingKHRJitterCamera.rmiss`文件，用于`VulkanKHRRayTracingJitterCamera`示例。
  >* `./asset/shaders`下新增`RayTracingKHRJitterCameraShadow.rmiss`文件，用于`VulkanKHRRayTracingJitterCamera`示例。
  >* `./asset/shaders`下新增`RayTracingKHRJitterCamera.rchit`文件，用于`VulkanKHRRayTracingJitterCamera`示例。

* 2023/8/29 设计架构
  >
  >* `./samples`下新增`VulkanKHRRayTracingAnyHitShader`示例。用于研究实时光追中的任意命中着色器。
  >* `./asset/shaders`下新增`RayTracingAnyHitShader.rgen`文件，用于`VulkanKHRRayTracingAnyHitShader`示例。
  >* `./asset/shaders`下新增`RayTracingAnyHitShader.rmiss`文件，用于`VulkanKHRRayTracingAnyHitShader`示例。
  >* `./asset/shaders`下新增`RayTracingAnyHitShaderShadow.rmiss`文件，用于`VulkanKHRRayTracingAnyHitShader`示例。
  >* `./asset/shaders`下新增`RayTracingAnyHitShader.rchit`文件，用于`VulkanKHRRayTracingAnyHitShader`示例。

* 2023/8/30 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingAnyHitShader`示例。用于研究实时光追中的任意命中着色器。
  >* `./asset/shaders`下新增`RayTracingAnyHitShader.rahit`文件（任意命中着色器），用于`VulkanKHRRayTracingAnyHitShader`示例。

* 2023/8/31 设计架构
  >
  >* `./asset/shaders`下新增`RayTracingAnyHitShaderShadow.rahit`文件（任意命中着色器），用于`VulkanKHRRayTracingAnyHitShader`示例中阴影负载 `1` 的任意命中。
  >* `./asset/shaders`下更新`RayTracingAnyHitShaderShadow.rmiss`。将阴影负载增加随机种子`seed`成员。
  >* `./asset/shaders`下更新`RayTracingAnyHitShader.rchit`。将阴影负载增加随机种子`seed`成员。
  >* `./samples`下更新`VulkanKHRRayTracingAnyHitShader`示例。增加对阴影负载的任意命中着色器的适配。
  >* `./samples`下更新`BRDF`示例。使用非`Y`轴向上的材质球，并将背景清空成浅色。
  >* `./samples`下更新`NormalTexture`示例。使用非`Y`轴向上的材质球，并将背景清空成浅色。
  >* `./samples`下更新`PBRTest`示例。`roughness`参数默认设置为`0.5`。

* 2023/9/4 设计架构
  >
  >* `./samples`下新增`VulkanKHRRayTracingTestForInstances`示例。用于光追实例化（多实体）。

* 2023/9/6 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForInstances`示例。用于光追实例化（多实体）。

* 2023/9/8 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForReflections`示例。用于光追反射。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflections.rgen`文件，用于`VulkanKHRRayTracingTestForReflections`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflections.rmiss`文件，用于`VulkanKHRRayTracingTestForReflections`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflectionsShadow.rmiss`文件，用于`VulkanKHRRayTracingTestForReflections`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflections.rchit`文件，用于`VulkanKHRRayTracingTestForReflections`示例。

* 2023/9/11 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForReflections_WithoutLimited`示例。用于不受`VkPhysicalDeviceRayTracingPipelinePropertiesKHR::maxRayRecursionDepth`限制的光追反射。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflections_WithoutLimited.rgen`文件，用于`VulkanKHRRayTracingTestForReflections_WithoutLimited`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflections_WithoutLimited.rmiss`文件，用于`VulkanKHRRayTracingTestForReflections_WithoutLimited`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflectionShadows_WithoutLimited.rmiss`文件，用于`VulkanKHRRayTracingTestForReflections_WithoutLimited`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForReflections_WithoutLimited.rchit`文件，用于`VulkanKHRRayTracingTestForReflections_WithoutLimited`示例。

* 2023/9/19 设计架构
  >
  >* `./samples`下新增`VulkanKHRRayTracingTestForMultiClosestHits`示例。用于研究实时光追中的多个最近命中着色器。

* 2023/9/20 设计架构
  >
  >* `engine\core\include`下`TCore.h`新增对于`VK_USE_PLATFORM_OHOS`或`OHOS_PLATFORM`的开源鸿蒙系统的`TURBO_PLATFORM_OPEN_HARMONY`宏定义的。
  >* `engine\core\include`下`TCore.h`新增对于`vulkan_ohos.h`的开源鸿蒙系统的头文件加入。
  >* `engine\core\src`下`TVulkanLoader.cpp`新增对于的开源鸿蒙系统`TURBO_PLATFORM_OPEN_HARMONY`宏适配。

* 2023/9/21 设计架构
  >
  >* `engine\core\include`下`TVulkanLoader.h`将外部全局变量`PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr`移动到`TVulkanLoader`类中作为成员变量。
  >* `engine\core\include`下`TVulkanLoader.h`将外部全局变量`PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion`移动到`TVulkanLoader`类中作为成员变量。
  >* `engine\core\include`下`TVulkanLoader.h`将外部全局变量`PFN_vkCreateInstance vkCreateInstance`移动到`TVulkanLoader`类中作为成员变量。
  >* `engine\core\include`下`TVulkanLoader.h`将外部全局变量`PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties`移动到`TVulkanLoader`类中作为成员变量。
  >* `engine\core\include`下`TVulkanLoader.h`将外部全局变量`PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties`移动到`TVulkanLoader`类中作为成员变量。
  >* `engine\core\include`下`TVulkanLoader.h`中的`TVulkanLoader`类`Turbo::Core::TVulkanLoader::Load`使用相应的成员变量获取函数。
  >* `engine\core\src`下`TVulkanLoader.cpp`中的`TVulkanLoader`类构造函数获取初始化相应的成员变量。
  >* `engine\core\include`下`TVulkanLoader.h`增加`struct TGlobalFunctionTable`结构体，用于将`Vulkan`全局函数打包。
  >* `engine\core\include`下`TVulkanLoader.h`增加`TGlobalDriver`重命名`TGlobalFunctionTable`。
  >* `engine\core`下`TVulkanLoader`类增加`TGlobalDriver LoadGlobalDriver()`成员函数，并实现。
  >* `engine\core`下`TVmaAllocator`类的构造函数中使用`TVulkanLoader`获取`vkGetInstanceProcAddr`接口。
  >* `engine\core`下`TExtensionInfo`类的`GetInstanceExtensionCount`函数中使用`TVulkanLoader`中的`LoadGlobalDriver`接口获取`vkEnumerateInstanceExtensionProperties`接口。
  >* `engine\core`下`TExtensionInfo`类的`GetInstanceExtensions`函数中使用`TVulkanLoader`中的`LoadGlobalDriver`接口获取`vkEnumerateInstanceExtensionProperties`接口。
  >* `engine\core`下`TInstance`类的`InternalCreate`函数中使用`TVulkanLoader`中的`LoadGlobalDriver`接口获取`vkCreateInstance`接口。
  >* `engine\core`下`TLayerInfo`类的`GetInstanceLayerCount`函数中使用`TVulkanLoader`中的`LoadGlobalDriver`接口获取`vkEnumerateInstanceLayerProperties`接口。
  >* `engine\core`下`CMakeLists.txt`中增加`TCORE_SHARED_LIBS`选项，用于编译输出`TCore`的动态库。

* 2023/9/24 设计架构
  >
  >* `./samples`下增加`AndroidVulkanTest`，用于研究`Adnroid`的`Vulkan`，并尝试将`Turbo`适配到`Android`平台。
  >* `./engine/core/src`下`TVulkanLoader.cpp`增加对于`TURBO_PLATFORM_ANDROID`宏适配，用于适配到`Android`平台。
  >* `./engine/core`下`TSurface`增加对于`TURBO_PLATFORM_ANDROID`宏适配，用于适配到`Android`平台。
  >* `./engine/core`下`TSurface`类中，适配`Android`平台，增加`ANativeWindow *nativeWindow`成员变量。
  >* `./engine/core`下`TSurface`类中，适配`Android`平台，增加`PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR`成员变量。
  >* `./engine/core/src`下实现`TSurface`类中`TSurface(Turbo::Core::TDevice *device, ANativeWindow *window)`构造函数。适配`Android`平台。
  >* `./engine/core/src`下更新`TSurface`类中`InternalCreate`函数。适配`Android`平台。
  >* `./engine/core/src`下更新`TSurface`类中`GetSurfaceSupportQueueFamilys()`函数。适配`Android`平台。
  >* `./engine/core/include`下更新`TPlatform.h`中的`TPlatformType`平台枚举值声明。会与安卓平台特定的``ADNROID``宏冲突。
  >* `./engine/core/src`下更新`TVulkanLoader.cpp`中的`TVulkanLoader::GetVulkanVersion()`函数。适配`Android`平台。
  >* `./engine/core/src`下更新`TPlatform.cpp`中修正`return32;`为`return 32;`的`Bug`。

* 2023/9/25 设计架构
  >
  >* `engine/core`下更新`TSwapchain`的构造函数。修改之前强制判断并使用`TCompositeAlphaBits::ALPHA_OPAQUE_BIT`透明配置，该配置在`Android`平台上不适用。在`Android`平台上会返回`TCompositeAlphaBits::ALPHA_INHERIT_BIT`，也就是透明度不由`Vulkan`控制，而是`Android`平台自己控制。
  >* `engine/core`下更新`TSwapchain`的`InternalCreate()`函数。增加对于`TCompositeAlphaBits::ALPHA_OPAQUE_BIT`的判断，防止在`Android`平台上导致不应该的异常抛出。
  >* `engine/core`下更新`TException`。统一错误消息格式，使用标准异常消息输出。
  >* `engine/core`下更新`TCore`下增加`std::string TResultToString(TResult result)`函数。用于帮助将结果枚举转成字符串。
  >* `engine/core`下更新`TSurface`下`InternalCreate()`。将`VkAndroidSurfaceCreateInfoKHR`下的`sType`设置为`VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR`，之前为`VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR`是个`Bug`
  >* `engine/core`下更新`TException`，增加一个`std::string whatStr`成员变量。移除使用`exception(char const* const _Message)`构造函数（该函数非标准函数）。

* 2023/9/26 设计架构
  >
  >* `engine/core`下更新`TException`，增加`std::string GetTip()`成员函数。移用于获取提示信息。
  >* `engine/core`下更新`TVersion`的版本大小比较算法，之前的比较有逻辑`Bug`。
  >* `engine/core`下增加`AndroidPureHelloTriangle`文件夹，用于存放`Turbo`在`Android`上绘制三角形的示例程序。

* 2023/9/27 设计架构
  >
  >* `./samples`下增加`VulkanKHRRayTracingTestForAnimationTLAS`示例。用于研究光追中顶层加速结构的更新。

* 2023/9/28 设计架构
  >
  >* `./samples`下增加`VulkanKHRRayTracingTestForAnimationBLAS`示例。用于研究光追中底层加速结构的更新。
  >* `./asset/shaders`下增加`RayTracingKHRTestForAnimationBLAS.comp`计算着色器。用于研究光追中底层加速结构更新中使用计算管线更新顶点数据。
  >* `./engine/core`下`TDescriptorSet`类中增加`void BindData(uint32_t binding, TBuffer *buffer, uint32_t dstArrayElement)`成员函数。用于简单绑定单个缓存。
  >* `./engine/core`下`TPipelineDescriptorSet`类中增加`void BindData(uint32_t set, uint32_t binding, TBuffer *buffer, uint32_t dstArrayElement)`成员函数。用于简单绑定单个缓存。
  >* `./engine/core`下`TDescriptorSet`类中`BindData(..., std::vector<TBuffer *> &buffers)`中增加对`STORAGE_BUFFER`描述符类型判断。
  >* 不支持`VK_DESCRIPTOR_TYPE_STORAGE_BUFFER`描述符类型（相关代码未实现）。需要填补这一部分空缺。
  >* `./engine/core`下`TDescriptor.h`中增加`TStorageBufferDescriptor`类声明。用于表述`VK_DESCRIPTOR_TYPE_STORAGE_BUFFER`类型的描述符
  >* `./engine/core`下`TShader`类中增加`std::vector<TStorageBufferDescriptor *> storageBufferDescriptors`成员变量。用于存储`VK_DESCRIPTOR_TYPE_STORAGE_BUFFER`类型的描述符。
  >* `./engine/core`下`TShader`类析构中增加对`std::vector<TStorageBufferDescriptor *> storageBufferDescriptors`成员的内存释放。
  >* `./engine/core`下`TShader`类中增加`const std::vector<TStorageBufferDescriptor *> &GetStorageBufferDescriptors()`成员函数。用于获取着色器中的对应描述符信息。
  >* `./engine/core`下`TPipeline`类中`InternalCreate()`成员函数中增加对于`std::vector<TStorageBufferDescriptor *>`描述符的处理。
  
* 2023/10/11 设计架构
  >
  >* `./samples`下增加`VulkanKHRRayTracingTestForIntersectionShader`示例。用于研究光追中相交着色器的使用。
  >* `./asset/shaders`下新增`RayTracingKHRTestForIntersection.rgen`文件，用于`VulkanKHRRayTracingTestForIntersectionShader`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForIntersection.rchit`文件，用于`VulkanKHRRayTracingTestForIntersectionShader`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForIntersection.rmiss`文件，用于`VulkanKHRRayTracingTestForIntersectionShader`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForIntersection.rint`文件，用于`VulkanKHRRayTracingTestForIntersectionShader`示例。

* 2023/10/29 设计架构
  >
  >* 配了一台`3060Ti`显卡的电脑，光追研究继续。
  >* `./engine/render`下`TBuffer.h`中`TUniformBuffer`移除`std::enable_if_t`（`llvm-mingw`编译器不支持，估计是`C++`标准配置不对）。
  >* `./thirdparty`下更新`KTX-Software`到最新`main`分支。
  >* `./engine/render`下`TContext`中移除对于`TDescriptorID`的初始化列表写法使用最原始的结构体赋值法。（`llvm-mingw`好像不支持初始化列表写法，目前不知道如何设置编译器支持初始化列表写法，估计是`C++`标准配置不对）。
  >* `./engine/render`下`TContext`中将`std::is_class_v<A_Test>`改为`std::is_class<A_Test>::value`。
  >* `./samples`下`VulkanTest`中将所有`std::exception(const std::string&)`改为`Turbo::Core::TException`。
  >* `./samples`下`VulkanAllocatorTest`中将所有`std::exception(const std::string&)`改为`Turbo::Core::TException`。

* 2023/10/31 设计架构
  >
  >* `./engine/render`下`TBuffer.h`中`TUniformBuffer`移除`std::enable_if_t`（`C++14`特性），使用`std::enable_if`替换（`C++11`特性）。

* 2023/11/4 设计架构
  >
  >* `./samples`下增加`VulkanKHRRayTracingTestForCallableShader`示例。用于研究可调用着色器的使用。
  >* `./asset/shaders`下新增`RayTracingKHRTestForCallableShader.rchit`文件，用于`VulkanKHRRayTracingTestForCallableShader`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForCallableShader_R.rchit`文件，用于`VulkanKHRRayTracingTestForCallableShader`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForCallableShader_G.rchit`文件，用于`VulkanKHRRayTracingTestForCallableShader`示例。
  >* `./asset/shaders`下新增`RayTracingKHRTestForCallableShader_B.rchit`文件，用于`VulkanKHRRayTracingTestForCallableShader`示例。
  >* `./engine/core`下`TPhysicalDeviceInfo.h`中类`TPhysicalDeviceInfo`中新增`VkPhysicalDeviceRayQueryFeaturesKHR physicalDeviceRayQueryFeaturesKHR`成员。
  >* `./engine/core`下`TPhysicalDevice.h`中类`TPhysicalDeviceFeatures`中新增`bool rayQuery = false`成员。
  >* `./engine/core`下`TPhysicalDevice.h`中类`TPhysicalDevice`中更新`EnumerateProperties()`成员函数，获取并保存`VkPhysicalDeviceRayQueryFeaturesKHR`相关数据。
  >* `./engine/core`下`TPhysicalDevice.h`中类`TPhysicalDevice`中更新`GetDeviceFeatures()`成员函数，获取`VkPhysicalDeviceRayQueryFeaturesKHR`相关数据。
  >* `./engine/core`下`TDevice.h`中类`TDevice`中更新`InternalCreate()`成员函数，更新对`VkPhysicalDeviceRayQueryFeaturesKHR`相关数据的激活设置。

* 2023/11/5 设计架构
  >
  >* `./samples`下增加`VulkanRayQueryTest`示例。用于研究`VK_KHR_ray_query`扩展的使用。
  >* `./engine/core`下`TExtensionInfo.cpp`中数组`TAllExtensionNames`中增加`VK_KHR_ray_query`项。
  >* `./engine/core`下`TExtensionInfo.h`中枚举`TExtensionType`中增加`VK_KHR_RAY_QUERY`项。
  >* `./engine/core`下`TDevice.cpp`中，类`TDevice`的成员函数`InspectExtensionAndVersionDependencies()`中增加对`VK_KHR_RAY_QUERY`扩展依赖项的检查。

* 2023/11/6 设计架构
  >
  >* `./samples`下更新`VulkanRayQueryTest`示例。用于研究`VK_KHR_ray_query`扩展的使用。

* 2023/11/7 设计架构
  >
  >* `./samples`下更新`VulkanRayQueryTest`示例。用于研究`VK_KHR_ray_query`扩展的使用。
  >* `./asset/shaders`下增加`VulkanRayQueryTest.vert`示例。用于研究`VK_KHR_ray_query`扩展的顶点着色器。
  >* `./asset/shaders`下增加`VulkanRayQueryTest.frag`示例。用于研究`VK_KHR_ray_query`扩展的片元着色器。
  >* `./engine/core`下`TDescriptor.cpp`中，类`TDescriptor`的成员函数`GetVkDescriptorType()`中增加对`ACCELERATION_STRUCTURE`加速结构描述符类型（`VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR`）的适配。

* 2023/11/8 设计架构
  >
  >* `./asset`下增加`CornellBox`的`glTF`模型。

* 2023/11/9 设计架构
  >
  >* `./samples`下增加`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。

* 2023/11/10 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。

* 2023/11/11 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。

* 2023/11/12 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。`glTF`场景解析和基本渲染完成。

* 2023/11/13 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。完成对于光追结果的采样积累。
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。完成对于材质信息的采样渲染。

* 2023/11/14 设计架构
  >
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。修正法线错误 `Bug`。
  >* `./engine/core`下更新`TDevice`类中`InspectExtensionAndVersionDependencies()`成员函数中增加对`VK_KHR_shader_clock`扩展的依赖检查。
  >* `./engine/core`下更新`TPhysicalDeviceInfo.h`中`class TPhysicalDeviceInfo`中增加`VkPhysicalDeviceShaderClockFeaturesKHR physicalDeviceShaderClockFeaturesKHR`成员。
  >* `./engine/core`下更新`TPhysicalDevice.h`中`class TPhysicalDeviceFeatures`中增加`bool shaderSubgroupClock`和`bool shaderDeviceClock`成员。
  >* `./engine/core`下更新`TPhysicalDevice`类中`EnumerateProperties()`成员函数中增加对`VkPhysicalDeviceShaderClockFeaturesKHR`特性的获取。
  >* `./engine/core`下更新`TPhysicalDevice`类中`GetDeviceFeatures()`成员函数中增加对`VkPhysicalDeviceShaderClockFeaturesKHR`特性的赋值。
  >* `./engine/core`下更新`TDevice`类中`InternalCreate()`成员函数中增加对`VkPhysicalDeviceShaderClockFeaturesKHR`特性的激活。
  >* `./samples`下更新`VulkanKHRRayTracingTestForGLTF`示例。用于研究对于`glTF`模型文件的光线追踪渲染。增加对于`VK_KHR_shader_clock`扩展和相关特性的支持。

* 2023/11/15 设计架构
  >
  >* `./engine/core`下新增`TReferenced`类。用于计数引用内存回收机制。

* 2023/11/16 设计架构
  >
  >* `./engine/core`下更新`TReferenced`类中的`UnReference()`成员函数。修正`delete`返回对象内部变量的`Bug`。
  >* `./samples`下增加`ReferencedTest`示例。用于计数引用内存回收机制测试。
  >* `./engine/core`下新增`ref_ptr`类。用于计数引用内存回收机制。

* 2023/11/17 设计架构
  >
  >* `./engine/core`下更新`ref_ptr`类。用于计数引用内存回收机制。

* 2023/11/18 设计架构
  >
  >* `./engine/core`下将`ref_ptr`重命名为`TRefPtr`(统一命名规则)。用于计数引用内存回收机制。
  >* `./engine/core`下更新`TRefPtr`。用于计数引用内存回收机制。

* 2023/11/19 设计架构
  >
  >* `Turbo`使用`Turbo::Core::TRefPtr`和`Turbo::Core::TReferenced`进行重构。
  >* `./engine/core`下将`Turbo::Core::TObject`继承自`Turbo::Core::TReferenced`。
  >* `./engine/core`下将`Turbo::Core::TInfo`暂时取消继承自`Turbo::Core::TObject`。
  >* `Turbo`中将所有继承自`Turbo::Core::TObject`的子类的析构函数全部转成`protected`权限。
  >* `./engine/core`下将`TBarrier.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TBuffer.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TCommandBuffer.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TCommandBufferPool.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。

* 2023/11/20 设计架构
  >
  >* `./engine/core`下将`TComputePipeline.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TDescriptor.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TDescriptorPool.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TDescriptorSet.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TDescriptorSetLayout.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TDevice.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TDeviceQueue.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。

* 2023/11/21 设计架构
  >
  >* `./engine/core`下将`TEngine.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TFence.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TFramebuffer.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TGraphicsPipeline.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TImage.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。

* 2023/11/22 设计架构
  >
  >* `./engine/core`下将`TImageView.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TInstance.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TPhysicalDevice.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TPipeline.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TPipelineCache.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TPipelineDescriptorSet.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TPipelineLayout.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TRenderingPipeline.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TRenderPass.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TSampler.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TSemaphore.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TShader.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。

* 2023/11/23 设计架构
  >
  >* `./engine/core`下将`TSurface.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TSwapchain.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TVmaAllocator.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TVulkanAllocator.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下将`TVulkanLoader.h`及其`cpp`中的所有计数引用子类使用`Turbo::Core::TRefPtr`维护。
  >* `./engine/core`下更新`TRefPtr`中使用`class`声明的模板关键字，使用`typename`替代。
  >* `./engine/core`下更新`TReferenced`中增加`virtual bool Valid() const`成员虚函数。用于判定该引用是否有效（用于自定义判定有效性）。
  >* `./engine/core`下更新`TRefPtr`中更新`bool Valid() const`成员函数。增加对该指针自身对象的有效性判断（用于自定义判定有效性）。
  >* `./engine/core`下更新`TBarrier.cpp`中更新`TBufferMemoryBarrier`构造函数的`buffer`参数使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TBarrier.cpp`中更新`TImageMemoryBarrier`构造函数的`image`参数使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TBarrier.cpp`中更新`TImageMemoryBarrier`构造函数的`imageView`参数使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TCommandBuffer.cpp`中更新`TCommandBufferBase`构造函数的`commandBufferPool`参数使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TCommandBuffer.cpp`中更新`TCommandBufferBase`的`CmdBindPipelineDescriptorSet`成员函数使用`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TCommandBufferPool.cpp`中更新`TCommandBufferPool`的`Free(const TRefPtr<TCommandBufferBase> &commandBufferBase)`成员函数移除对`delete`的使用。
  >* `./engine/core`下更新`TCommandBufferPool.cpp`中更新`~TCommandBufferPool`析构函数，使用`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TCommandBufferPool.cpp`中更新`Free(const TRefPtr<TCommandBuffer> &commandBuffer)`成员函数，使用`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TCommandBufferPool.cpp`中更新`Free(const TRefPtr<TSecondaryCommandBuffer> &secondaryCommandBuffer)`成员函数，使用`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TCommandBufferPool.cpp`中更新`TCommandBufferPool`构造函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TCommandBuffer.cpp`中更新`CmdBindDescriptorSets`成员函数中，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TCommandBuffer.cpp`中更新`TCommandBufferBase`的`CmdBeginRendering`成员函数中，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TCommandBuffer.cpp`中更新`TCommandBufferBase`的`CmdUpdateBuffer`成员函数中，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TCommandBuffer.cpp`中更新`TCommandBufferBase`的`CmdPushConstants`成员函数中，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDescriptorPool.cpp`中更新`TDescriptorPool`的构造函数中，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDescriptorPool.cpp`中更新`TDescriptorPool`的`Free`成员函数，回收资源。
  >* `./engine/core`下更新`TCommandBufferPool.cpp`中更新`TCommandBufferPool`的`Free`成员函数，回收资源。

* 2023/11/24 设计架构
  >
  >* `./engine/core`下的`TDescriptorPool`类内增加`std::vector<TRefPtr<TPipelineDescriptorSet>> pipelineDescriptorSets`成员。
  >* `./engine/core`下的`TDescriptorPool`类内更新`Allocate(...)`成员函数。内部使用容器存储。
  >* `./engine/core`下的`TDescriptorPool`类内更新`Free(...)`成员函数。内部判断是否为该池分配。
  >* `./engine/core`下的`TDescriptorPool`类内更新`~TDescriptorPool()`析构函数。回收所有分配的描述符。
  >* `./engine/core`下更新`TDescriptorSet.cpp`中更新`TDescriptorSet`的构造函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDescriptorSet.cpp`中更新`TDescriptorSet`的`BindData`成员函数，使用`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TDescriptorSetLayout.cpp`中`TDescriptorSetLayout`的`InternalCreate`成员函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDescriptorSetLayout.cpp`中`TDescriptorSetLayout`的构造函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDevice.cpp`中`TDevice`的`AddChildHandle`成员函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDevice.cpp`中`TDevice`的`InternalCreate`成员函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDevice.cpp`中`TDevice`的`InternalDestroy`成员函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDevice.cpp`中`TDevice`的构造函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDevice.cpp`中`TDevice`的析构函数，移除对于`delete`的显式调用。
  >* `./engine/core`下更新`TDevice.cpp`中`TDevice`的`GetDeviceQueueCountByQueueFamily`成员函数，使用`Turbo::Core::TRefPtr<T>`。
  >* `./engine/core`下更新`TDeviceQueue`中的`Submit`成员函数。将指针变成引用。并在内部适配`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TDeviceQueue`。增加只使用`Fence`的`Submit`成员函数。并在内部适配`Turbo::Core::TRefPtr`。
  >* `./engine/core`下更新`TDeviceQueue.cpp`中`TDeviceQueue`的`Present`成员函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TDeviceQueue.cpp`中`TDeviceQueue`的`AddChildHandle`成员函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TEngine.cpp`中`TEngine`的`VerificationInitVulkan`成员函数，移除对于`delete`的显式调用。
  >* `./engine/core`下更新`TEngine.cpp`中`TEngine`的析构函数，移除对于`delete`的显式调用。
  >* `./engine/core`下更新`TEngine.cpp`中`TEngine`的`GetInstance`函数，使用`Turbo::Core::TRefPtr`适配。
  >* `./engine/core`下更新`TException`。继承自`std::runtime_error`。并移除不必要的成员变量和函数。
  >* `./engine/core`下更新`TFence.cpp`中`TFence`的构造函数，使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下更新`TFence.cpp`中`TFence`的`Wait`成员函数，使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下更新`TCommandBufferBase`中`CmdBindDescriptorSets`成员函数，`std::vector`形参使用`const`进行适配。
  >* `./engine/core`下更新`TCommandBufferBase`中`CmdBindVertexBuffers`成员函数，`std::vector`形参使用`const`进行适配。
  >* `./engine/core`下更新`TCommandBufferBase`中`CmdSetViewport`成员函数，`std::vector`形参使用`const`进行适配。
  >* `./engine/core`下更新`TCommandBufferBase`中`CmdSetScissor`成员函数，`std::vector`形参使用`const`进行适配。
  >* `./engine/core`下更新`TCommandBufferBase`中`CmdPipelineBarrier`成员函数，`std::vector`形参使用`const`进行适配。
  >* `./engine/core`下更新`TAttachment`中所有信息获取函数后增加`const`。
  >* `./engine/core`下更新`TMemoryBarrier`中所有信息获取函数后增加`const`。
  >* `./engine/core`下更新`TBufferMemoryBarrier`中所有信息获取函数后增加`const`。
  >* `./engine/core`下更新`TImageMemoryBarrier`中所有信息获取函数后增加`const`。
  >* `./engine/core`下更新`TReferenced`中增加`void Release() const`成员函数。用于内存强制回收。
  >* `./engine/core`下更新`TRefPtr<T>`中增加`void Release()`成员函数。用于内存强制回收。
  >* `./engine/core`下更新`TCommandBufferPool`中`Allocate`成员函数。优化调用。
  >* `./engine/core`下更新`TCommandBufferPool`中`AllocateSecondary`成员函数。优化调用。
  >* `./engine/core`下更新`TCommandBufferPool`中`Free(TRefPtr<TCommandBuffer> &)`成员函数。修正导致错误递归`Bug`。
  >* `./engine/core`下更新`TCommandBufferPool`中`Free(TRefPtr<TSecondaryCommandBuffer> &)`成员函数。修正导致错误递归`Bug`。
  >* `./engine/core`下更新`TCommandBufferPool`中`Free(TRefPtr<TCommandBufferBase> &)`成员函数。优化调用，并强制回收内存。

* 2023/11/27 设计架构
  >
  >* `./engine/core`下的`TCommandBufferPool`类内更新`Free(TRefPtr<TCommandBufferBase> &)`成员函数。移除对于`TRefPtr::Release()`的调用。
  >* `./engine/core`下的`TDescriptorPool`类内更新`Free(TRefPtr<TPipelineDescriptorSet> &)`成员函数。
  >* `./engine/core`下的`TReferenced`类内增加`uint32_t GetReferenceCount() const`成员函数。
  >* `./engine/core`下的`TReferenced`类内更新`uint32_t referenceCount`成员从`protected`转成`private`。
  >* `./engine/core`下的`TRefPtr`类内增加`uint32_t ReferenceCount() const`成员函数。
  >* `./engine/core`下的`TFences`类内更新`Add(const TRefPtr<TFence> &)`成员函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TFormatInfo`类内更新`GetSupportFormats(TPhysicalDevice *)`成员函数为`GetSupportFormats(const TRefPtr<TPhysicalDevice> &)`。
  >* `./engine/core`下的`TFormatInfo`类内更新`IsSupportFormat(TPhysicalDevice *, TFormatType)`成员函数为`IsSupportFormat(const TRefPtr<TPhysicalDevice> &, TFormatType)`。
  >* `./engine/core`下的`TFormatInfo`类内更新`GetSupportFormats(const TRefPtr<TPhysicalDevice> &)`成员函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TFormatInfo`类内更新`IsSupportFormat(const TRefPtr<TPhysicalDevice> &, TFormatType)`成员函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TFramebuffer`类内更新构造函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TGraphicsPipeline`类内更新`InternalCreate()`函数。使用`Turbo::Core::TRefPtr`。
  >* `./engine/core`下的`TGraphicsPipeline`类内更新`InternalCreate()`函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TGraphicsPipeline`类内更新构造函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TImage`类内更新构造函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TImageView`类内更新构造函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TInstance`类内更新`IsHaveHandle`成员函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TInstance`类内更新`AddChildHandle`成员函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TLayerInfo`类内更新`GetPhysicalDeviceLayerCount(TPhysicalDevice *)`成员函数。修改为`GetPhysicalDeviceLayerCount(const TRefPtr<TPhysicalDevice> &)`。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TLayerInfo`类内更新`GetPhysicalDeviceLayers(TPhysicalDevice *physicalDevice)`成员函数。修改为`GetPhysicalDeviceLayers(const TRefPtr<TPhysicalDevice> &)`
  >* `./engine/core`下的`TPhysicalDevice`类内更新`AddChildHandle`成员函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TPhysicalDevice`类内更新构造函数。使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。

* 2023/11/30 设计架构
  >
  >* `./engine/core`下的`TPipeline`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TPipeline`类内更新`InternalCreate()`成员函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TPipeline`类内更新`InternalDestroy()`成员函数。移除对于`pipelineLayout`内存回收。
  >* `./engine/core`下的`TPipelineCache`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TPipelineDescriptorSet`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TPipelineDescriptorSet`类内更新`InternalCreate()`成员函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TPipelineDescriptorSet`类内更新`InternalDestroy()`成员函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TPipelineDescriptorSet`类内更新`BindData()`成员函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TPipelineLayout`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TPipelineDescriptorSet`类内更新析构函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TRenderingPipeline`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TRenderingPipeline`类内更新`InternalCreate`函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TRenderPass`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TSampler`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TSemaphore`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TShader`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TShader`类内更新构造函数，修正之前有效性判断的代码错误。
  >* `./engine/core`下的`TSurface`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TSurface`类内更新`InternalCreate`函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TSurface`类内更新`InternalDestroy`函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TSwapchain`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TSwapchain`类内更新`InternalCreate`函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TSwapchain`类内更新构造函数。使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TVersion.cpp`修正其继承自`TInfo`。
  >* `./engine/core`下的`TVmaAllocator`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TVulkanAllocator`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TVulkanAllocator`类内使用`Turbo::Core::TRefPtr`进行适配。
  >* `./engine/core`下的`TVulkanAllocator`类内`Destory()`静态成员函数内移除对`delete`的使用。
  >* `./engine/core`下的`TRefPtr.h`中增加`cstdint`头文件。
  >* `./engine/core`下的`TVulkanLoader`类内使用`Turbo::Core::TRefPtr::Valid()`进行有效性判断。
  >* `./engine/core`下的`TVulkanLoader`类内使用`Turbo::Core::TRefPtr`进行适配。

* 2023/12/1 设计架构
  >
  >* `./.gitignore`下增加对`thirdparty`第三方库的追踪忽略。
  >* `./engine/framegraph`下的`TFrameGraph.hpp`内的`TResourceProxy<T>`增加`T resource`的限制描述备注。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TResourceProxy<T>`内的`uint32_t id`成员修改为`ID id`。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TRenderPass`内的`AddSubpass()`成员函数。函数形参增加默认值。并返回索引值
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`Turbo::FrameGraph::TFrameGraph::TBuilder`内的`CreateSubpass()`成员函数。去掉不必要的临时变量。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TSubpass`内增加`bool IsWrite(TResource resource)`成员函数。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TSubpass`内增加`bool IsRead(TResource resource)`成员函数。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TSubpass`内增加`bool IsInput(TResource resource)`成员函数。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TSubpass`内的`void Write(TResource resource)`成员函数。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TSubpass`内的`void Read(TResource resource)`成员函数。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TSubpass`内的`void Input(TResource resource)`成员函数。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TFrameGraph`内的`Compile()`成员函数。增加对计数引用数为`0`的判断。
  >* `./engine/framegraph`下更新`TFrameGraph.hpp`内的`TFrameGraph`内的`Execute()`成员函数。资源只在`PassNode`有效的情况下创建和销毁。
  >* `./engine/framegraph`下`TFrameGraph.hpp`内的`TFrameGraph`内增加`std::string ToHtml()`成员函数。方便输出查看。

* 2023/12/2 设计架构
  >
  >* `./engine/render`下的`TBuffer`中增加`core/include/TRefPtr.h`头文件的引入。
  >* `./engine/render`下的`TBuffer`类中将`Turbo::Core::TBuffer *buffer`成员改成`Turbo::Core::TRefPtr<Turbo::Core::TBuffer> buffer`。

* 2023/12/3 设计架构
  >
  >* `./engine/core`下更新`TBuffer.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TCommandBuffer.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TCommandBufferPool.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TDescriptor.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TDescriptorPool.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TDescriptorSet.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TDescriptorSetLayout.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TDevice.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TDeviceQueue.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TException.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TExtensionInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TFormatInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TFramebuffer.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TGraphicsPipeline.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TImage.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TImageView.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TInstance.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TLayerInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TMemoryHeapInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TMemoryTypeInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TPhysicalDevice.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TPipeline.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TPipelineCache.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TQueueFamilyInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TRenderPass.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TScissor.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TSemaphore.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TShader.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TSubpass.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TSurface.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TSwapchain.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TVendorInfo.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TVersion.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TViewport.h`中对所有的信息获取函数后增加`const`。并更新相应`cpp`文件。
  >* `./engine/core`下更新`TCommandBufferBase`中`CmdPipelineBarrier`成员函数。为了调用`const`形参中的非`const`函数而进行了拷贝。（这可能是没有必要的拷贝）
  >* `./engine/core`下更新`TBarrier.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TCommandBuffer.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TCommandBufferPool.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TDescriptor.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TDescriptorPool.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TDevice.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TDeviceQueue.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TEngine.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TFence.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TFramebuffer.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TImage.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TImageView.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TInstance.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TPhysicalDevice.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TPipeline.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TRenderPass.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TShader.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TSurface.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。
  >* `./engine/core`下更新`TSwapchain.h`所有的`TRefPtr<T>`返回函数，都使用`const TRefPtr<T>&`格式返回（减少不必要的拷贝构造）。

* 2023/12/4 设计架构
  >
  >* `./engine/core`下更新`TRefPtr.h`。增加`StaticCast<T,S>`静态强制转换函数。
  >* `./engine/core`下更新`TRefPtr.h`。增加`DynamicCast<T,S>`动态强制转换函数。
  >* `./engine/core`下更新`TRefPtr.h`。增加`ConstCast<T,S>`常量强制转换函数。
  >* `./engine/core`下更新`TCommandBufferPool`。将原先的`std::vector<TRefPtr<TCommandBufferBase>> commandBuffers`成员分成两个成员`std::vector<TRefPtr<TCommandBuffer>> commandBuffers`和`std::vector<TRefPtr<TSecondaryCommandBuffer>> secondaryCommandBuffers`。
  >* `./engine/core`下更新`TCommandBufferPool`。`Allocate()`和`AllocateSecondary()`成员函数返回引用。
  >* `./engine/core`下更新`TCommandBufferPool`的`Free(TRefPtr<TCommandBufferBase> &)`成员函数。适配一级和二级的指令缓存的释放。
  >* `./engine/core`下更新`TCommandBufferPool`的`InternalDestroy`成员函数。适配二级的指令缓存数组的释放。
  >* `./engine/core`下更新`TCommandBufferPool`的析构函数。将指令缓存数组的清空转移至`InternalDestroy`成员函数中。
  >* `./docs`下更新`Issue.md`。记录一些待解决的问题。

* 2023/12/5 设计架构
  >
  >* `./engine/core`下将所有的`ToString()`函数后面都加上`const`声明。
  >* `./engine/core`下将`TReferenced`中的`Release()`公有成员函数转移至私有。
  >* `./engine/core`下`TBuffer`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TCommandBufferBase`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TCommandBufferPool`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TDescriptorPool`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TDescriptorSet`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TDevice`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TDeviceQueue`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TEngine`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TFence`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TFences`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TFramebuffer`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TImage`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TImageView`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TInstance`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TPhysicalDevice`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TPipeline`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TPipelineCache`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TPipelineDescriptorSet`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TPipelineLayout`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TRenderPass`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TSampler`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TSemaphore`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TSurface`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TSwapchain`中增加`Valid()`函数的重写。
  >* `./engine/core`下`TVmaAllocator`中增加`Valid()`函数的重写。

* 2023/12/6 设计架构
  >
  >* `./engine/core`下`TFormatInfo`中移除对于`Turbo::Core::TDevice`的提前声明和头文件引入。
  >* `./engine/core`下移除所有类中对`TRefPtr<T>`成员赋`nullptr`初始值。使其通过默认的构造函数进行初始化。
  >* `./engine/core`下`TDescriptor.cpp`开头增加对于`TShader.h`头文件的包含。
  >* `./engine/core`下`TPhysicalDevice`中`GetSupportFormats() const`成员函数中将`this`的`const TPhysicalDevice *`指针强制转换成`TPhysicalDevice *`。用于获取支持的格式。
  >* `./engine/core`下`TPipelineDescriptorSet.cpp`开头增加对于`TDescriptorSetLayout.h`头文件的包含。
  >* `./engine/core`下`TPipelineDescriptorSet.cpp`开头增加对于`TDescriptorPool.h`头文件的包含。
  >* `./engine/core`下`TPipelineDescriptorSet.cpp`开头增加对于`TBuffer.h`头文件的包含。
  >* `./engine/core`下`TRenderingPipeline.cpp`开头增加对于`TImageView.h`头文件的包含。
  >* `./engine/core`下`TBarrier.h`中`TImageMemoryBarrier`的析构函数声明为`default`。
  >* `./engine/core`下`TDescriptorPool.h`中`TDescriptorSize`的析构函数声明为`default`。

* 2023/12/7 设计架构
  >
  >* `./engine/core`下`main.cpp`进行`TRefPtr<T>`计数引用适配。
  >* `./engine/core`下`TDeviceQueue`中的`bool Submit(std::vector<TRefPtr<TSemaphore>> &, std::vector<TRefPtr<TSemaphore>> &,...)`更改成`bool Submit(const std::vector<TRefPtr<TSemaphore>> &, const std::vector<TRefPtr<TSemaphore>> &,...)`。形参为`const`的版本。
  >* `./engine/core`下`TInstance`中`InternalCreate()`成员函数，对于`this->physicalDevices`数组遍历时出现了未知异常。尝试解决该`Bug`。
  >* `./engine/core`下`TRefPtr`中移除`void Release()`成员函数。
  >* `./engine/core`下`TReferenced`中增加`uint32_t UnReferenceWithoutDelete() const`成员函数。用于仅解除计数引用而不触发销毁判断。
  >* `./engine/core`下`TRefPtr`中增加`T *Unbind()`成员函数。用于仅解除计数引用而不触发销毁判断。
  >* `./engine/core`下`TInstance`中`InternalCreate()`成员函数，在内部会调用`TVulkanLoader::Instance()->LoadInstanceDriver(this)`而`this`当前的计数引用是`0`，当执行完该函数后计数引用发现自身引用数为`0`，将会触发`自销毁（delete this）`这将导致不必要的异常`Bug`。修正。
  >* `./engine/core`下`TPhysicalDevice`中`InternalCreate()`成员函数，在内部某些函数会使用`TRefPtr`引用`this`，当前的计数引用是`0`，当执行完该函数后计数引用发现自身引用数为`0`，将会触发`自销毁（delete this）`这将导致不必要的异常`Bug`。修正。
  >* `./engine/core`下`TDevice`中`InternalCreate()`成员函数，在内部某些函数会使用`TRefPtr`引用`this`，当前的计数引用是`0`，当执行完该函数后计数引用发现自身引用数为`0`，将会触发`自销毁（delete this）`这将导致不必要的异常`Bug`。修正。
  >* `./engine/core`下创建`TDevice`和`TDeviceQueue`时内部有非常恶心的逻辑前后互调用和重建，调整这一部分使其清晰明了。（历史遗留问题，当时脑子抽了写的啥玩意？？？）
  >* `./engine/core`下更新`TDevice`下的`InternalCreate()`函数。内部构建已知队列方式。而不是使用之前动态创建队列再反补回头重构设备方式（太麻烦，也没必要）。
  >* `./engine/core`下更新`TDevice`下的`std::vector<TRefPtr<TDeviceQueue>> deviceQueues`成员变量修改为`std::map<TQueueFamilyIndex, std::vector<TRefPtr<TDeviceQueue>>>`类型。
  >* `./engine/core`下更新`TInstance`下的`InternalCreate()`成员函数中移除对于`TPhysicalDevice::InternalCreate()`的调用。
  >* `./engine/core`下更新`TPhysicalDevice`下的`InternalCreate()`成员函数中移除对于`TDevice::InternalCreate()`的调用。
  >* `./engine/core`下更新`TDevice`下的`InternalCreate()`成员函数中移除对于`TDeviceQueue::InternalCreate()`的调用。
  >* `./engine/core`下更新`TDevice`下的构造函数中对于`TDeviceQueue`的创建。
  >* `./engine/core`下更新`TDeviceQueue`下的构造函数。移除对于重构`TDevice`的策略。
  >* `./engine/core`下更新`TPhysicalDevice`下移除`std::map<TQueueFamilyInfo, uint32_t> availableQueueCountMap`成员变量。
  >* `./engine/core`下更新`TPhysicalDevice`下移除`void AvailableQueueCountMinusOneByQueueFamilyIndex(uint32_t queueFamilyIndex)`成员函数。
  >* `./engine/core`下更新`TPhysicalDevice`下移除`void AvailableQueueCountPlussOneByQueueFamilyIndex(uint32_t queueFamilyIndex)`成员函数。
  >* `./engine/core`下更新`TPhysicalDevice`下移除`void ResetQueueCountMap()`成员函数。
  >* `./engine/core`下更新`TPhysicalDevice`下`GetAvailableQueueCount`成员函数。
  >* `./engine/core`下更新`TDeviceQueue`下的`InternalCreate()`成员函数中移除对于`TCommandBufferPool::InternalCreate()`的调用。
  >* `./engine/core`下更新`TDevice`下的`AddChildHandle(..)`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`InternalDestroy(..)`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`InternalDestroy(..)`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`GetDeviceQueueCountByQueueFamily(..)`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下移除`GetDeviceQueueCountByQueueFamily`成员函数。
  >* `./engine/core`下更新`TDevice`下移除`GetDeviceQueueFamilyInfos`成员函数。
  >* `./engine/core`下更新`TDevice`下的`GetBestGraphicsQueue`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`GetBestComputeQueue`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`GetBestTransferQueue`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`GetBestSparseBindingQueue`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`TDevice`下的`GetBestProtectedQueue`成员函数。适配`deviceQueues`的成员变量。
  >* `./engine/core`下更新`main.cpp`下结尾处提出对于销毁`VkSurfaceKHR`修改意见。

* 2023/12/11 设计架构
  >
  >* `./docs`下`TurboDesign.drawio`增加`JobSystem`板块。用于研究`JobSystem`。
  >* `./docs`下`TurboDesign.drawio`的`JobSystem`板块增加`WorkStealingDequeue`的架构图。用于研究`JobSystem`。
  >* `./engine/core`下`TSurface`中增加`const VkAllocationCallbacks *externalVkAllocationCallbacks`成员变量。用于适配外部`VkSurfaceKHR`句柄。
  >* `./engine/core`下`TSurface`中`TSurface(const Turbo::Core::TRefPtr<Turbo::Core::TDevice> &, VkSurfaceKHR)`构造函数中增加`const VkAllocationCallbacks *pAllocator`形参。并修改相应`cpp`中的实现。用于适配外部`VkSurfaceKHR`句柄。
  >* `./engine/core`下`TSurface`更新`InternalDestroy()`成员函数。用于适配外部`VkSurfaceKHR`句柄。
  >* `./engine/core`下`main.cpp`更新对于`glfw`创建的外部`VkSurfaceKHR`句柄的适配。

* 2023/12/13 设计架构
  >
  >* `./engine/render`下更新`TBuffer`中的`buffer`使用默认构造初始化。
  >* `./engine/render`下更新`TBuffer`中的`Destroy`成员函数。释放所有成员变量。
  >* `./engine/render`下更新`TBuffer`中的`Copy`成员函数。将所有`Turbo::Core`下的计数引用对象使用`Turbo::Core::TRefPtr`承接。
  >* `./engine/render`下更新`TImage`中的`image`和`imageView`成员。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TImage`及其子类中的`CreateImageView`成员函数。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TImage`中的`Destroy`成员函数。释放所有成员变量。
  >* `./engine/render`下更新`TShader`。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TSampler`。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TRenderPass`。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TGraphicsPipeline`。使用`Turbo::Core::TRefPtr<T>`适配。

* 2023/12/14 设计架构
  >
  >* `./engine/render`下更新`TResourceAllocator`。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TContext`。使用`Turbo::Core::TRefPtr<T>`适配。
  >* `./engine/render`下更新`TContext`下的`Turbo::Render::TCommandBuffer`从原先的`typedef struct`转成`class`声明。
  >* `./engine/render`下更新`TBuffer.h`下引入`Core`的`TBuffer.h`的头文件。通过编译。
  >* `./engine/render`下更新`TBuffer.cpp`下引入`Core`的`TDevice.h`的头文件。通过编译。
  >* `./engine/render`下更新`TContext.h`下引入`Core`的`TFence.h`的头文件。通过编译。
  >* `./engine/render`下更新`TImage.h`下引入`Core`的`TImage.h`的头文件。通过编译。
  >* `./engine/render`下更新`TImage.h`下引入`Core`的`TImageView.h`的头文件。通过编译。
  >* `./engine/render`下更新`TImage.cpp`下引入`Core`的`TImage.h`的头文件。通过编译。
  >* `./engine/render`下更新`TResourceAllocator.cpp`下引入`Core`的`TCommandBuffer.h`的头文件。通过编译。
  >* `./engine/render`下更新`TShader.cpp`下引入`Core`的`TDevice.h`的头文件。通过编译。
  >* `./engine/render`目前有些头文件有冗余。
  >* `./engine/render`使用`Turbo::Core::TRefPtr<T>`初步适配完成。

* 2023/12/18 设计架构
  >
  >* `./thirdparty`下更新`KTX-Software`同步到`main`分支。
  >* `./thirdparty`下`KTX-Software`中用到了`std::make_unique`。基本上为`C++14`以上特性。为此将`Turbo`从`C++11`提升至`C++17`。

* 2024/2/26
  >
  >* 更新 `HelloTriangle` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `TReferenced` 优化代码调用。

* 2024/2/28
  >
  >* 更新 `PureHelloTriangle` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/2/29
  >
  >* 更新 `PureCombinedImageSampler` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/2
  >
  >* 更新 `PureSeparateImageSampler` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `PureIndexDraw` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `TurboTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/3
  >
  >* 更新 `PBRTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/4
  >
  >* 更新 `SecondaryCommandBufferTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/5
  >
  >* 更新 `MultiDrawTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `SubpassTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `LineAndLineWidth` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/6
  >
  >* 更新 `InstancedDrawTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `PerlinWorleyNoiseTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `CineShaderLava` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/7
  >
  >* 更新 `Octagrams` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `ProteanClouds` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `ComputePipelineTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `Auroras` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/8
  >
  >* 更新 `ComputerPipelineGeneratePerlinWorley` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `ComputerPipelineGenerateWorley` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `IspGlassball11Mix` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VolumetricCloud` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `RayMarchingTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `RayMarchingBoundingBoxTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `RayMarchingPerlinWorleyNoise` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/9
  >
  >* 暂时移除 `RenderDesignTest` 示例。``Render`` 库重构中。
  >* 更新 `PushConstantTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 暂时移除 `RenderAndFrameGraph` 示例。``Render`` 库重构中。
  >* 更新 `ClearColorTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `ShaderIncludeTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `GreenFieldDiorama` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 暂时移除 `PureHelloTriangleAndFrameGraph` 示例。``Render`` 库重构中。
  >* 更新 `VulkanDynamicRenderingTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `DynamicRenderingTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `NormalTexture` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/10
  >
  >* 更新 `BRDF` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `TessellationTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `GeometryShaderTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `TessellationAndGeometry` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `MeshShaderTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `SpecializationConstantsTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/11
  >
  >* 更新 `VulkanKHRRayTracingTestForLighting` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForLightingShadow` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/12
  >
  >* 更新 `VulkanKHRRayTracingTestForLightingShadowWithTexture` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingJitterCamera` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingAnyHitShader` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForInstances` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForReflections` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForReflections_WithoutLimited` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/13
  >
  >* 更新 `VulkanKHRRayTracingTestForMultiClosestHits` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForAnimationTLAS` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForAnimationBLAS` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForIntersectionShader` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForCallableShader` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanRayQueryTest` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。
  >* 更新 `VulkanKHRRayTracingTestForGLTF` 示例。使用 `Turbo::Core::TRefPtr<T>` 适配。

* 2024/3/17
  >
  >* 更新 ``TAllocator`` ，优化代码结构。尝试实现自定义内存分配器。

* 2024/3/18
  >
  >* 更新 ``TAllocator`` ，优化自定义内存分配器代码结构。尝试增加内存头，简化结构管理。并转移至 ``Turbo::Core`` 命名空间。
  >* 更新 ``VulkanTest`` 示例，使用 ``TVulkanAllocator`` 。使得 ``Linux`` 通过编译。
  >* 更新 ``VulkanAllocatorTest`` 示例，使用 ``TVulkanAllocator`` 。使得 ``Linux`` 通过编译。
  >* 更新 ``KTX-Software`` 到主分支。

* 2024/3/21
  >
  >* 更新 ``TBarrier.h`` 适配各函数从指针自动内部转 ``Turbo::Core::TRefPtr<T>`` 。

* 2024/6/27
  >
  >* ``thirdparty`` 下增加 ``ply`` 文件夹，用于存储 ``ply`` 文件读写源码。
  >* ``samples`` 下增加 ``PointCloud.cpp`` 示例，用于点云。

* 2024/6/30
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。读取并解析 ``ply`` 点云文件。

* 2024/7/1
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。将点云数据写入纹理中存储。

* 2024/7/4
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。

* 2024/7/6
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。

* 2024/7/12
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。修正视角。

* 2024/7/14
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。使其支持千万级别的点云数据。

* 2024/7/15
  >
  >* 更新 ``samples`` 下 ``PointCloud.cpp`` 示例。移除重复点云文件的读取。

* 2024/7/17
  >
  >* 更新 ``CMakeLists.txt`` 增加 ``Windows`` 平台下 ``WIN32`` 宏的声明。在某些情况下 `clang` 编译器不能自动定义该宏。

* 2024/7/18
  >
  >* 更新 ``TImage.cpp`` 中 ``InternalDestroy()`` 成员函数对于外部 ``VkImage`` 在回收时不需要调用 ``vmaDestroyImage`` 回收图片
  >* 修正 ``TSurface.cpp`` 中对于外部 ``VkSurfaceKHR`` 在回收时没有获取 ``vkDestroySurfaceKHR`` 函数的 ``bug``

* 2024/8/29
  >
  >* 更新 ``.gitignore`` 忽略同步 ``glslang`` 、 ``SPIRV-Cross`` 、 ``VulkanMemoryAllocator`` 、 。
