# Vulkan Info

`Turbo` 能够统一 `Vulkan` 的信息查询，在 `Vulkan` 的不同版本之间，增加和更新了很多 `Vulkan` 的 `instance` 和 `physical device` 信息。

`Turbo` 需要通过简单统一的接口进行信息获取。

为了高效方便，信息在初始化时全部获取存储一份副本，之后获取都从副本中获取（有待商榷）。

## 信息分类

* `Vulkan` 版本
* `Layer` （`Instance` 层）
* `Extension` (物理设备持的扩展)
* `Feature` 物理设备支持的扩展
* 物理设备其他信息 （比如设备名称等各种信息）

存储使用 `Vulkan` 原生 `API` 结构体存储即可，不需要再额外声明（太多了，额外自定义声明也没必要，有可能漏声明导致不一致）。

> 注: 所有操作需要在 `Vulkan Loader` 加载之后进行。
> `VulkanLoader` 是单例模式，所有信息获取接口都应该依赖该类。

## VulkanLoader 接口获取

The global commands are: `vkEnumerateInstanceVersion`, `vkEnumerateInstanceExtensionProperties`, `vkEnumerateInstanceLayerProperties`, and `vkCreateInstance`. Dispatchable commands are all other commands which are not global.

```CXX
// Provided by VK_VERSION_1_0
PFN_vkVoidFunction vkGetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName);
```

获取全局函数：

```CXX
vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceVersion");
vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");
vkGetInstanceProcAddr(NULL, "vkCreateInstance");
```

### Extending Physical Device Core Functionality

New core physical-device-level functionality can be used when both VkPhysicalDeviceProperties::apiVersion and VkApplicationInfo::apiVersion are greater than or equal to the version of Vulkan that added the new functionality. The Vulkan version supported by a physical device can be obtained by calling vkGetPhysicalDeviceProperties.

当 `VkPhysicalDeviceProperties::apiVersion` 和 `VkApplicationInfo::apiVersion` 版本大于等 `Vulkan` 发布该函数版本时，新的 `核心` 物理设备级函数可以使用。

物理设备支持的 `Vulkan` 版本可通过调用 `vkGetPhysicalDeviceProperties` 获取。

>注: 获取物理设备级的函数时需要一个创建好的 `VkInstance` 并通过 `vkGetInstanceProcAddr(instance, "XXX")` 获取。

### Extending Physical Device From Device Extensions

When the VK_KHR_get_physical_device_properties2 extension is enabled, or when both the instance and the physical-device versions are at least 1.1, physical-device-level functionality of a device extension can be used with a physical device if the corresponding extension is enumerated by vkEnumerateDeviceExtensionProperties for that physical device, even before a logical device has been created.

当支持 `VK_KHR_get_physical_device_properties2` 扩展或者 `instance` 和物理设备版本最起码是 `1.1` 的话，当物理设备级函数对应的设备扩展存在于 `vkEnumerateDeviceExtensionProperties` 列出的扩展中的话，设备扩展中的物理设备级函数也可以被获取并调用，即使（逻辑）设备还未被创建。

换句话说就是只要物理设备支持 `VK_KHR_get_physical_device_properties2` 扩展或者 `instance` 和物理设备版本最起码是 `1.1` 的话，不需要创建（逻辑）设备，就可以获取并调用设备扩展中声明的物理设备级函数。

To obtain a function pointer for a physical-device-level command from a device extension, an application can use vkGetInstanceProcAddr. This function pointer may point to dispatch code, which calls a different real implementation for different VkPhysicalDevice objects. Applications must not use a VkPhysicalDevice in any command added by an extension or core version that is not supported by that physical device.

从设备扩展中获取物理设备级函数，可通过 `vkGetInstanceProcAddr` 函数获取。获取的函数可能指向一个调度代码，针对不同的 `VkPhysicalDevice` 调用对应的底层实现。对于那行扩展或核心不支持的物理设备级函数，应用不应该使用。

Device extensions may define structures that can be added to the pNext chain of physical-device-level commands.

设备扩展可能对物理设备级函数定义新的结构体，并通过物理设备级函数的 `pNext` 链传入。

## Vulkan 版本

Vulkan 目前主要有三个版本

1. `Vulkan Loader`
2. `VkInstanceCreateInfo::pCreateInfo(VkApplicationInfo)::apiVersion`
3. `VkPhysicalDeviceProperties::apiVersion`

其中查询支持的 instance 级别的功能特性版本调用：

```CXX
// Provided by VK_VERSION_1_1
VkResult vkEnumerateInstanceVersion(uint32_t* pApiVersion);
```
