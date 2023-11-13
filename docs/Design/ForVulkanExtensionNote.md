# Vulkan Extension 开发流程

## TAllExtensionNames 声明相应 Extesnion 名称

1. `TExtensionInfo.cpp`中`TAllExtensionNames`数组中增加对应新扩展的名称（需要确认一下是否之前已经声明过）

## TExtensionType 枚举中声明相应 Extesnion 的枚举

枚举所对应枚举值必须与`TAllExtensionNames`对应的扩展名索引相对应

## TDevice 中的 InspectExtensionAndVersionDependencies 函数对 Extesnion 的依赖进行检查

在`TDevice::InspectExtensionAndVersionDependencies`中检查新扩展的依赖

## TVulkanLoader 中声明和加载对应的扩展函数

### 函数声明

声明扩展的函数

### 函数加载

加载扩展的函数

## 对相应的 Feature 进行适配

这将进入 `Vulkan Feature 开发流程` 文档说明
