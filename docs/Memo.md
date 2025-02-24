# 备忘录

## Important

`DescriptorSet` 和 `DescriptorSetLayout` 需要重构，相同的 `DescriptorSet/DescriptorSetLayout` 可以重复利用，而不需要重复创建。提高利用率。

## code

```CXX
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(binding_count);

    for (uint32_t binding_index; binding_index < binding_count; binding_index++)
    {
        TDescriptor *descriptor = this->descriptors[binding_index];
        if (descriptor != nullptr)
        {
            VkDescriptorSetLayoutBinding& vk_descriptor_set_layout_binding = bindings[binding_index];
            vk_descriptor_set_layout_binding.binding = descriptor->GetBinding();
            vk_descriptor_set_layout_binding.descriptorType = descriptor->GetVkDescriptorType();
            vk_descriptor_set_layout_binding.descriptorCount = descriptor->GetCount();
            vk_descriptor_set_layout_binding.stageFlags = this->shader->GetVkShaderStageFlags();
            vk_descriptor_set_layout_binding.pImmutableSamplers = nullptr;
        }
        else
        {
            throw Turbo::Core::TException(TResult::INVALID_PARAMETER);
        }
    }
```

```CXX
    std::vector<int> ints;
    std::vector<int>::iterator int_iterator = ints.begin();
    for (int &int_item : ints)
    {
        ints.erase(int_iterator);
        int_iterator++;
    }
```

## 容器对应 TRefPtr<T> 和 T* 类型的形参问题

```CXX
    void CmdBindVertexBuffers(const std::vector<TRefPtr<TBuffer>> &vertexBuffers = {});
    void CmdBindVertexBuffers(const std::vector<TBuffer *> &vertexBuffers = {});
    //FIX::对于如上 vector<TRefPtr<TBuffer>/TBuffer *> 形参，考虑使用 模板，这样就不需要声明两个函数了，只需要声明一个模板函数即可：
    template<typename T>
    void CmdBindVertexBuffers(const std::vector<T> &vertexBuffers = {});//需要对 T 进行限制
    //NOTE: 该方法编程方便，但可读性较差。
```

事实上 ``std::vector`` 都不是强制要求，只要：

* 传入的 ``容器`` 支持 ``for(:)`` 遍历
* ``容器`` 中存储的项为 ``TReference*`` 或 ``TRefPtr<T>``

``Turbo`` 的本质需求就是将用户传递进来的一串集合转成 ``Vulkan`` 要求的数组。

支持 ``for(:)`` 需要如下要求：

1. 数组类型（有 ``N`` 个元素）

* ``首表达式`` 为 ``数组头``
* ``尾表达式`` 为 ``数组头+N``

2. 类

* 有 ``首表达式`` （ ``begin()`` ）
* 有 ``尾表达式`` （ ``end()`` ）
* 有 ``首/尾表达式`` 支持 ``!=`` 操作
* 有 ``首/尾表达式`` 支持 ``++`` 操作

3. 其他

* ``首表达式`` ： ``begin(其他)``
* ``尾表达式`` ： ``end(其他)``

## 使用 TRefPtr<T>(ptr).Valid() 检查 ptr 的问题

```CXX
void SomeFun(T* ptr)
{
    //FIX::建议使用专门的检查函数，或构造函数
    if(!TRefPtr<T>(ptr).Valid())//该行结束可能会导致 ptr 进行 delete
    {
        return;
    }

    ...//Do valid
}

T* t = new T();
SomeFun(t);//SomeFun 结束之后 t 如果没人使用将会被 delete
```

## VkFormat

VK_FORMAT_MAX_ENUM = 0x7FFFFFFF
使用一个 uint32_t 就可以代表 VkFormat 中所有的格式

engine\core\src\TFormatInfo.cpp 下的 `std::vector<VkFormat> TAllFormats` 用于存储 `Vulkan` 所有的格式声明，
之后通过 `vkGetPhysicalDeviceFormatProperties(...)` 函数遍历每一个 `VkFormat` 来确定是否支持该格式。

其中的 `TAllFormats` 目前是写死的，不是很优雅，`Vulkan` 可能会随着版本的发布新增格式，可能需要随时更新适配 `TAllFormats` 中的值。
需要一种方式动态遍历 `VkFormat` 中的所有项，但 `C/C++` 并不支持直接遍历枚举，并且 `VkFormat` 中有重复值。

需要一个容器可以随时维护 `VkFormat` ，并支持遍历。

`vkconfig` 是通过解析 `vulkaninfo(vulkaninfoSDK)` 生成 `vulkaninfo.json` 文件，之后解析该文件。

`vulkaninfo` 通过 `vulkaninfo.hpp` 解析其中的 `format_ranges` 。

`vulkaninfo.hpp` 好像是通过 `vulkan` 的 `vk.xml` 生成的？

使用 `vulkaninfo.hpp` 中声明的 `format_ranges` 是个好主意。官方维护，值得信赖。(ﾟ∀。)

```CXX
struct FormatRange {
    // the Vulkan standard version that supports this format range, or 0 if non-standard
    APIVersion minimum_instance_version;

    // The name of the extension that supports this format range, or NULL if the range
    // is only part of the standard
    const char *extension_name;

    // The first and last supported formats within this range.
    VkFormat first_format;
    VkFormat last_format;
};

auto format_ranges = std::array{
    FormatRange{0, nullptr, static_cast<VkFormat>(0), static_cast<VkFormat>(184)},
    FormatRange{VK_API_VERSION_1_1, nullptr, static_cast<VkFormat>(1000156000), static_cast<VkFormat>(1000156033)},
    FormatRange{0, VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME, static_cast<VkFormat>(1000156000), static_cast<VkFormat>(1000156033)},
    FormatRange{VK_API_VERSION_1_3, nullptr, static_cast<VkFormat>(1000330000), static_cast<VkFormat>(1000330003)},
    FormatRange{0, VK_EXT_YCBCR_2PLANE_444_FORMATS_EXTENSION_NAME, static_cast<VkFormat>(1000330000), static_cast<VkFormat>(1000330003)},
    FormatRange{VK_API_VERSION_1_3, nullptr, static_cast<VkFormat>(1000340000), static_cast<VkFormat>(1000340001)},
    FormatRange{0, VK_EXT_4444_FORMATS_EXTENSION_NAME, static_cast<VkFormat>(1000340000), static_cast<VkFormat>(1000340001)},
    FormatRange{VK_API_VERSION_1_3, nullptr, static_cast<VkFormat>(1000066000), static_cast<VkFormat>(1000066013)},
    FormatRange{0, VK_EXT_TEXTURE_COMPRESSION_ASTC_HDR_EXTENSION_NAME, static_cast<VkFormat>(1000066000), static_cast<VkFormat>(1000066013)},
    FormatRange{VK_API_VERSION_1_4, nullptr, static_cast<VkFormat>(1000470000), static_cast<VkFormat>(1000470001)},
    FormatRange{0, VK_KHR_MAINTENANCE_5_EXTENSION_NAME, static_cast<VkFormat>(1000470000), static_cast<VkFormat>(1000470001)},
    FormatRange{0, VK_IMG_FORMAT_PVRTC_EXTENSION_NAME, static_cast<VkFormat>(1000054000), static_cast<VkFormat>(1000054007)},
    FormatRange{0, VK_NV_OPTICAL_FLOW_EXTENSION_NAME, static_cast<VkFormat>(1000464000), static_cast<VkFormat>(1000464000)},
};
```

```CXX
// Provided by VK_VERSION_1_0
void vkGetPhysicalDeviceFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties*                         pFormatProperties);
```

If no format feature flags are supported, the format itself is not supported, and images of that format cannot be created.

### Image Format

```CXX
// Provided by VK_VERSION_1_0
VkResult vkGetPhysicalDeviceImageFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkImageType                                 type,
    VkImageTiling                               tiling,
    VkImageUsageFlags                           usage,
    VkImageCreateFlags                          flags,
    VkImageFormatProperties*                    pImageFormatProperties);
```

The ``format``, ``type``, ``tiling``, ``usage``, and ``flags`` parameters correspond to parameters that would be consumed by ``vkCreateImage`` (as members of ``VkImageCreateInfo``).

If format is not a supported image format, or if the combination of ``format``, ``type``, ``tiling``, ``usage``, and ``flags`` is not supported for images, then ``vkGetPhysicalDeviceImageFormatProperties`` returns ``VK_ERROR_FORMAT_NOT_SUPPORTED``.

### TFormatType

声明维护 `TFormatType`（与 `VkFormat` 一致），但 `Vulkan` 可能在新版本中推出新的 `枚举项` ，也就是说，`Turbo`中的声明类型可能比`Vulkan`的少，没那么全。所以底层对于
`TFormatType`的使用按照一个`uint32_t`使用即可，这样如果用户要使用`Turbo`中没有的类型，只需要使用`类型强转`即可。

```CXX

TFormatType ft = (TFormatType)(TFormatType中未声明的有效VkFormat枚举值);

//在底层ft按照整数（VkFormat）使用
```

## Linux(Deepin V23)

wayland-client.h : sudo apt install libwayland-dev

## VulkanMmeoryAllocator

engine/core/thirdparty/VulkanMemoryAllocator/src/VmaUsage.h 第 100 行

### include <vulkan/vulkan_win32.h>

在 Linux 平台编译是个 `Bug`

## Extension

## TInfo

考虑是否继承自 TReferenced

## GLSL -> SPIR-V

解析有问题：

1. 解析信息不全。
2. 解析信息数据来源，尝试不再使用 ``SpirV-Cross`` 只使用 ``glslang`` 。
3. 使用 ``SpirV-Cross`` 提前信息可能有 ``bug``

会有问题： ``glslang`` 只支持编译 ``GLSL`` 和 ``HLSL`` ，而市面上的着色器前端语言标准层出不穷（GLSL HLSL  Slang WebGL HIP/CUDA SYCL MetalShadingLanguage 等）， ``Turbo`` 不可能支持每一个着色器前端语言，而现在所有前端语言几乎都支持编译为 ``SPIR-V`` 后端，原则上 ``Turbo`` 只需要支持 ``SPIR-V`` 的解析和使用即可支持所有的前端着色器语言。

现在的问题变成了使用 ``SPIRV-Cross/SPIRV-Reflect`` 序列化/反序列化出 ``描述符`` 信息。并进行相应的自动化适配。

目测 ``SPIRV-Reflect`` 比较轻量，应该好用。

### SPIRV-Reflect

```CXX
typedef struct SpvReflectDescriptorSet {
  uint32_t                          set;
  uint32_t                          binding_count;
  SpvReflectDescriptorBinding**     bindings;
} SpvReflectDescriptorSet;
```

```CXX
typedef struct SpvReflectDescriptorBinding {
  uint32_t                            spirv_id;//???
  const char*                         name;//变量声明名称
  uint32_t                            binding;//binding号
  uint32_t                            input_attachment_index;//???。输入附件的index
  uint32_t                            set;//set号
  SpvReflectDescriptorType            descriptor_type;//枚举。描述符类型，与Vulkan对应
  SpvReflectResourceType              resource_type;//枚举。资源类型
  SpvReflectImageTraits               image;//???。结构体。应该是描述image/sampler信息的
  SpvReflectBlockVariable             block;//???。结构体。应该是描述struct信息的
  SpvReflectBindingArrayTraits        array;//???。结构体。应该是描述声明变量的维度
  uint32_t                            count;//???
  uint32_t                            accessed;//???
  uint32_t                            uav_counter_id;//???
  struct SpvReflectDescriptorBinding* uav_counter_binding;//???
  uint32_t                            byte_address_buffer_offset_count;//???。缓存地址的比特偏移 数量
  uint32_t*                           byte_address_buffer_offsets;//???。缓存地址的比特偏移 数据

  SpvReflectTypeDescription*          type_description;//???。结构体。类型描述信息

  struct {
    uint32_t                          binding;
    uint32_t                          set;
  } word_offset;//???。set和binding号

  SpvReflectDecorationFlags           decoration_flags;//???。uint32_t。无符号整数。
  // Requires SPV_GOOGLE_user_type
  SpvReflectUserType                  user_type;//???。枚举。类型
} SpvReflectDescriptorBinding;
```

```CXX
typedef struct SpvReflectImageTraits {
  SpvDim                            dim;//枚举。维度
  uint32_t                          depth;//???。深度
  uint32_t                          arrayed;//???。数组
  uint32_t                          ms; // 0: single-sampled; 1: multisampled
  uint32_t                          sampled;//???
  SpvImageFormat                    image_format;//枚举。格式【枚举值与Vulkan的VkFormat不对应】
} SpvReflectImageTraits;
```

```CXX
typedef struct SpvReflectBlockVariable {
  uint32_t                          spirv_id;//???
  const char*                       name;//名称
  // For Push Constants, this is the lowest offset of all memebers
  uint32_t                          offset;           // Measured in bytes //???。偏移
  uint32_t                          absolute_offset;  // Measured in bytes //???。绝对偏移
  uint32_t                          size;             // Measured in bytes //???。大小
  uint32_t                          padded_size;      // Measured in bytes //???。填补大小
  SpvReflectDecorationFlags         decoration_flags;//???。整数。
  SpvReflectNumericTraits           numeric;//???。结构体。数字。好像分为 【标量|向量|矩阵】
  SpvReflectArrayTraits             array;//???。结构体。数组
  SpvReflectVariableFlags           flags;//???。整数。

  uint32_t                          member_count;//成员数量
  struct SpvReflectBlockVariable*   members;//结构体。描述成员。

  SpvReflectTypeDescription*        type_description;//???。结构体。类型描述信息

  struct {
    uint32_t                          offset;
  } word_offset;//???。偏移

} SpvReflectBlockVariable;
```

```CXX
typedef struct SpvReflectNumericTraits {
  struct Scalar {
    uint32_t                        width;
    uint32_t                        signedness;
  } scalar;//???。标量

  struct Vector {
    uint32_t                        component_count;
  } vector;//???。向量

  struct Matrix {
    uint32_t                        column_count;
    uint32_t                        row_count;
    uint32_t                        stride; // Measured in bytes
  } matrix;//???。矩阵
} SpvReflectNumericTraits;
```

```CXX
typedef struct SpvReflectArrayTraits {
  uint32_t                          dims_count;//???。维度数量
  // Each entry is either:
  // - specialization constant dimension
  // - OpTypeRuntimeArray
  // - the array length otherwise
  uint32_t                          dims[SPV_REFLECT_MAX_ARRAY_DIMS];//???。各维度分量大小
  // Stores Ids for dimensions that are specialization constants
  uint32_t                          spec_constant_op_ids[SPV_REFLECT_MAX_ARRAY_DIMS];//???。
  uint32_t                          stride; // Measured in bytes //???。跨度
} SpvReflectArrayTraits;
```

## Resource Descriptors

descriptor 是确切的数据结构，用于代表着色器资源，比如  buffer，buffer view，image view，sampler 或 combined image sampler。

一个 descriptor set 中有多个 descriptor

descriptor set 是在指令记录时用于绑定的

如果开启了 descriptorBuffer 特性（feature）的话，可以将多个 descriptor 放到 descriptor buffers 中，其也将在指令记录时进行绑定（与 descriptor set 类似）

着色器通过带有 绑定号 的变量声明，去相应的 descriptor set 的位置处定位资源。

也可以通过使用 Physical Storage Buffer Access 直接访问缓存的64位地址，而不通过 descriptor 。(而不通过 descriptor??? 通过 VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER 类型 descriptor)

### 描述符类型

#### Storage Image

VK_DESCRIPTOR_TYPE_STORAGE_IMAGE 通过一个 image view 与 image 资源相关联。

* 如果 image 的 format 支持 VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT，所有着色器阶段支持 加载（load）操作。
* 如果 image 的 format 支持 VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT，则 task，mesh 和 compute 着色器支持 存储（store）操作。
* 如果 image 的 format 支持 VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT，则 task，mesh 和 compute 着色器支持 原子（atomic ）操作。

* 当 fragmentStoresAndAtomics 特性激活后，片元着色器也能像计算着色器那样存储和原子操作 storage image。
* 当 vertexPipelineStoresAndAtomics 特性激活后，vertex,tessellation 和 geometry 着色器也能像计算着色器那样存储和原子操作 storage image。

如果想要访问 storage image 中的数据，layout 必须是 VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR 或 VK_IMAGE_LAYOUT_GENERAL 。

#### Sampler

VK_DESCRIPTOR_TYPE_SAMPLER 与 sampler 对象相关联，用于 sampled image 采样 。

#### Sampled Image

VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE 通过一个 image view 与 image 资源相关联。用于 采样 操作。

着色器中使用一对 Sampler 和 Sampled Image 对图片进行采样

被采样的 image 的 layout 必须是如下之一：

* VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_GENERAL
* VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR
* VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
* VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR
* VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT

#### Combined Image Sampler

VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER 与一对 image 和 sampler 对象相关联，将 Sampler 和 Sampled Image 合成一个。

如果描述符应用一个采样器进行 Y′CBCR conversion 或者 subsampled image 操作，sampler 和 image 必须在同一个描述符中（Combined Image Sampler）。采样器进行的其他操作没有限制。

被采样的 image 的 layout 必须是如下之一：

* VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_GENERAL
* VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR
* VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
* VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR
* VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT

在一些平台 Combined Image Sampler 更高效

#### Uniform Texel Buffer

VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER 通过一个 buffer view 与 buffer 资源相关联。可进行 图片采样 操作。

Uniform texel buffers 使用一维数组存储像素数据，进而进行 图片采样。

* 如果 buffer view 的 format 支持 VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT，所有着色器阶段支持 加载（load）操作。

#### Storage Texel Buffer

VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER 通过一个 buffer view 与 buffer 资源相关联。可进行 图片加载、存储和原子 操作。

在 Uniform texel buffers 支持的操作基础上，支持对数据的写入。

* 如果 buffer view 的 format 支持 VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT，所有着色器阶段支持 加载（load）操作。
* 如果 buffer view 的 format 支持 VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT，则 task，mesh 和 compute 着色器支持 存储（store）操作。
* 如果 buffer view 的 format 支持 VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT，则 task，mesh 和 compute 着色器支持 原子（atomic ）操作。

* 当 fragmentStoresAndAtomics 特性激活后，片元着色器也能像计算着色器那样存储和原子操作 storage texel buffer。
* 当 vertexPipelineStoresAndAtomics 特性激活后，vertex,tessellation 和 geometry 着色器也能像计算着色器那样存储和原子操作 storage texel buffer。

#### Storage Buffer

VK_DESCRIPTOR_TYPE_STORAGE_BUFFER 通过与 buffer 资源直接相关联。可进行 加载、存储和原子 操作。用于描述一个结构体数据。

#### Uniform Buffer

VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER 通过与 buffer 资源直接相关联。可进行 加载 操作。用于描述一个结构体数据。

#### Dynamic Uniform Buffer

VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC 几乎与 Uniform Buffer 一样，与之唯一不同的是：指定了一个 buffer 的 offset 。

``` CXX
// Provided by VK_VERSION_1_0
void vkCmdBindDescriptorSets(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    uint32_t                                    dynamicOffsetCount,
    const uint32_t*                             pDynamicOffsets);
```

* dynamicOffsetCount
* pDynamicOffsets

用于配置 offset

#### Dynamic Storage Buffer

VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC 几乎与 Storage Buffer 一样，与之唯一不同的是：指定了一个 buffer 的 offset 。

与 Dynamic Uniform Buffer 一样，通过 vkCmdBindDescriptorSets(...) 配置 offset

#### Inline Uniform Block

VK_VERSION_1_3 或 VK_EXT_inline_uniform_block 特性

VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK 几乎与 Uniform Buffer 一样，与之唯一不同的是：直接从描述符自身内部进行存储，而不是与一个 buffer 进行关联。

常用于访问 一块小常量，将描述符和buffer解耦，而不需要复杂的间接操作。

与 push constants 相比，它允许在不同的渲染和调度指令间重用。

inline uniform block 不能整合到一个数组中，相反在资源绑定时确定其大小（单位：字节）。

```CXX
// Provided by VK_VERSION_1_3
typedef struct VkDescriptorPoolInlineUniformBlockCreateInfo {
    VkStructureType    sType;
    const void*        pNext;
    uint32_t           maxInlineUniformBlockBindings;
} VkDescriptorPoolInlineUniformBlockCreateInfo;

// Provided by VK_VERSION_1_3
typedef struct VkWriteDescriptorSetInlineUniformBlock {
    VkStructureType    sType;
    const void*        pNext;
    uint32_t           dataSize;
    const void*        pData;
} VkWriteDescriptorSetInlineUniformBlock;
```

#### Sample Weight Image

VK_QCOM_image_processing 特性

VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM 通过一个 image view 与 image 资源相关联。可用于 weight image sampling 。

image view 必须使用 VkImageViewSampleWeightCreateInfoQCOM 创建

着色器可以结合 weight 参数， sampled image 参数和一个 sampler 进行 weight image sampling 操作。

* 如果 image view 的 format 支持 VK_FORMAT_FEATURE_2_WEIGHT_IMAGE_BIT_QCOM，所有着色器阶段支持 weight image sampling 操作。
* 如果 image view 的 format 支持 VK_FORMAT_FEATURE_2_WEIGHT_SAMPLED_IMAGE_BIT_QCOM，所有着色器阶段支持 sampled image view 操作。

如果想要访问 sample weight image 中的数据，layout 必须是 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL 或 VK_IMAGE_LAYOUT_GENERAL 。

#### Block Matching Image

VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM 通过一个 image view 与 image 资源相关联。可用于 block matching 。

着色器可以结合 target image 参数， reference image 参数和一个 sampler 进行 block matching 操作。

* 如果 target view 和 reference view 的 format 支持 VK_FORMAT_FEATURE_2_BLOCK_MATCHING_BIT_QCOM，所有着色器阶段支持 block matching 操作。

如果想要访问 block matching image 中的数据，layout 必须是 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL 或 VK_IMAGE_LAYOUT_GENERAL 。

#### Input Attachment

VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT 通过一个 image view 与 image 资源相关联。可用于片元着色器中帧缓存坐标（framebuffer local）加载（load）操作。

* 如果 image 的 format 支持 VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT 或 VK_FORMAT_FEATURE_2_LINEAR_COLOR_ATTACHMENT_BIT_NV 或 VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT，image 就支持 input attachments 操作。

image 的 layout 必须是如下之一：

* VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_GENERAL
* VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR
* VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
* VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
* VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR
* VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT
* VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ

#### Acceleration Structure

VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR 或 VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV 与一个几何体相关联。用不用光线追踪。着色器对该描述符只读。

#### Mutable

VK_DESCRIPTOR_TYPE_MUTABLE_EXT 表示该描述符可以变化成 VkMutableDescriptorTypeListEXT::pDescriptorTypes 列出的任何一个描述符类型（指定于 VkMutableDescriptorTypeListEXT::pDescriptorTypes 的 pNext）。

在任意一个时刻，该描述符都有明确的类型。不仅可以变成 VK_DESCRIPTOR_TYPE_MUTABLE_EXT 类型，也可以变成 pDescriptorTypes 其中的一个类型。

在描述符被更新时，描述符的类型可以进行改变。当一个描述符用于描述符集中的绑定，确切的描述符类型会被确定（不是 VK_DESCRIPTOR_TYPE_MUTABLE_EXT 类型）

当确切的描述符类型为 VK_DESCRIPTOR_TYPE_MUTABLE_EXT 时，被认为时未定义描述符，如果着色器希望的描述符类型不是绑定的相应类型，对应的描述符类型也是未定义的。

*题外话：也许可以用于占位符*

查询哪些描述符类型支持 VK_DESCRIPTOR_TYPE_MUTABLE_EXT ，可以通过 vkGetDescriptorSetLayoutSupport 使用 VK_DESCRIPTOR_TYPE_MUTABLE_EXT 绑定，也可以传入一系列描述符类型进行查询。
