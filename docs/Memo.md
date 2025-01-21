# 备忘录

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
  SpvReflectImageTraits               image;//结构体。???。应该是描述image/sampler信息的
  SpvReflectBlockVariable             block;//结构体。???。应该是描述struct信息的
  SpvReflectBindingArrayTraits        array;//结构体。???。应该是描述声明变量的维度
  uint32_t                            count;//???
  uint32_t                            accessed;//???
  uint32_t                            uav_counter_id;//???
  struct SpvReflectDescriptorBinding* uav_counter_binding;//???
  uint32_t                            byte_address_buffer_offset_count;//???。缓存地址的比特偏移 数量
  uint32_t*                           byte_address_buffer_offsets;//???。缓存地址的比特偏移 数据

  SpvReflectTypeDescription*          type_description;//结构体。???。缓存地址的比特偏移 数据

  struct {
    uint32_t                          binding;
    uint32_t                          set;
  } word_offset;//???。set和binding号

  SpvReflectDecorationFlags           decoration_flags;//uint32_t。无符号整数。???
  // Requires SPV_GOOGLE_user_type
  SpvReflectUserType                  user_type;//枚举。???。类型
} SpvReflectDescriptorBinding;
```
