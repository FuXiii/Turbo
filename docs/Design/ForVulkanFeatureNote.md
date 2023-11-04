# Vulkan Feature 开发流程

## 声明相应 Feature 信息

1. `TPhysicalDeviceInfo.h` 中 `TPhysicalDeviceInfo` 类中，增加声明相应的 `纯` `Vulkan` 标准特性 `VkPhysicalDevice{特性名称}Features` 结构体。
2. `TPhysicalDevice.h` 中 `TPhysicalDeviceFeatures` 类中，增加与上一条 `VkPhysicalDevice{特性名称}Features` 对应的`bool`成员变量。用于 `Turbo`。

## 存储相应 Feature 信息

1. `TPhysicalDevice.h` 中 `TPhysicalDevice` 类中 `EnumerateProperties` 成员函数中，获取 `TPhysicalDeviceInfo` 对于的特性值，并在该成员函数末尾存储。

## 获取相应 Feature 信息

1. `TPhysicalDevice.h` 中 `TPhysicalDevice` 类中 `GetDeviceFeatures` 成员函数中，使用获取的 `TPhysicalDeviceInfo` 对返回的 `TPhysicalDeviceFeatures` 赋值。

## 激活相应 Feature 信息

1. `TDevice.h` 中 `TDevice` 类中 `InternalCreate` 成员函数中，使用传入的 `TPhysicalDeviceFeatures` 对相应的 `Vulkan` 特性赋值激活。
