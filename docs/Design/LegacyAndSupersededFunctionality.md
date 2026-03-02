# 遗弃

`Vulkan` 发展的同时会有部分功能、特性和扩展等已过时，将会标注为 `过时` 和 `遗弃` 。

## VK_KHR_get_physical_device_properties2

`VK_KHR_get_physical_device_properties2` 在 `Vulkan 1.1` 引入到核心，引入了一些用于获取物理设备信息的新版本函数。

对于查询设备支持的设备特性应该使用 `vkGetPhysicalDeviceFeatures2` 而不是 `vkGetPhysicalDeviceFeatures` 。当要激活特性时应将 `VkPhysicalDeviceFeatures2` 传入 `VkDeviceCreateInfo::pNext` 中而不是使用 `VkDeviceCreateInfo::pEnabledFeatures` 。

## API variant

`VK_MAKE_VERSION` 或 `VK_VERSION_MAJOR` 已经被遗弃，使用 `VK_MAKE_API_VERSION` or `VK_API_VERSION_MAJOR`。

`VK_API_VERSION` 已经被移除，应使用形如 `VK_API_VERSION_1_0` 或者 `VK_MAKE_API_VERSION` 。

## Device Layers

`Device Layers` 被移除了，只剩 `Instance Layers` 了。

## Render Pass

`Vulkan 1.2` 和 `VK_KHR_create_renderpass2` 为 `Render Pass` 提供了新功能. 在 `Vulkan 1.0` 的基础上提供更强的扩展性。

`Vulkan 1.3` 和 `VK_KHR_dynamic_rendering` 不需要创建 `VkFramebuffer` 和 `VkRenderPass` 就可以进行渲染，但不支持 `Subpass`（多 `pass`），其只支持一个 `pass` 的情况。

`Vulkan 1.4` 和 `VK_KHR_dynamic_rendering_local_read` 提供了对 `Subpass` 的支持.需要开发者将其分散在多个动态渲染中。

注：`VK_QCOM_render_pass_shader_resolve` 还不能被动态渲染代替

如果不使用扩展的话，推荐开发者使用动态渲染的 `vkCmdBeginRendering` 和 `vkCmdEndRendering` 管理渲染.

## Sampler 和 Buffer View Objects

当使用 `descriptor heaps` 时， `sampler` 和 `buffer view objects` 将完全不必要创建，并且直接使用 `vkWriteSamplerDescriptorsEXT` 和 `vkWriteResourceDescriptorsEXT` 转成描述符，跳过了需要一起创建对象。此种情况下 `sampler` 可以通过 `shader bindings` 将采样器嵌入着色器中，且只需要 `VkSamplerCreateInfo` 而不需要创建采样器对象。

与之类似的，图片用于描述符也可以跳过 `Image View` 创建，但在使用 `Render Pass` 时 `Image View` 还是需要创建。

## 描述符管理，通过描述符堆

`Descriptor heaps` 提供一个完整替代方案用于管理着色器资源文件。

`Descriptor heaps` 使用类似 `VK_EXT_descriptor_buffer` 。

混合使用也是可以的，但不能同时使用，并且在某些平台会有性能消耗。
