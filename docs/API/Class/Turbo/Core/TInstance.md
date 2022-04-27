# TInstance

class in Turbo::Core / Inherits from: [TObject](TObject.md) / header: TInstance.h

<br/>

## **Description**

该类作为 VKInstance 的封装类

<br/>

## **Properties**

Scope    | Type        								| Name                  		 |description
---------|------------------------------------------|--------------------------------|-------------
public   | friend class						 		| [TApplication](TApplication.md)|TInstance 的友元类为 [TApplication](TApplication.md)
private  | VkInstance  						 		| vkInstance    	   			 | Vulkan 中 VkInstance 的实例句柄
private  | const VkAllocationCallbacks*		 		| vkAllocationCallbacks 		 | 对象内存分配函数
private  | std::vector<[TLayer](TLayer.md)>  		| enabledLayers 				 | 创建实例时激活的验证层
private  | std::vector<[TExtension](TExtension.md)> | enabledExtensions 			 | 创建实例时激活的扩展
private  | std::vector<[TLayer](TLayer.md)> 		| supportLayers 				 | 实例支持的验证层
private  | std::vector<[TExtension](TExtension.md)> | supportExtensions 			 | 实例支持的扩展
private  | [TVersion](TVersion.md) 					| vulkanVersion 				 | 当前 Vulkan 实例的版本

<br/>

## **Function**

Scope  | Return      | Function              |description
-------|-------------|-----------------------|-------------
public  | [TException](TException.md)        | explicit TInstance( <br/>[TInstanceCreateInfo](TInstanceCreateInfo.md)* createInfo = nullptr,<br/> const VkAllocationCallbacks* pAllocator = nullptr<br/>)   |TInstance 构造函数
public  |         | explicit TInstance(<br/>const [TInstance](TInstance.md)& instance<br/>) = delete   |移除 TInstance 拷贝构造函数
public  |         | ~TInstance()   |TInstance 析构函数
public static | bool        | IsSupportVulkan( )    |是否支持 Vulkan
public static | [TVersion](TVersion.md)        | GetVulkanInstanceVersion()    |获取当前支持的 Vulkan 版本
public | VkInstance |GetVkInstance()|获取 VkInstance 句柄
public | [TVersion](TVersion.md) |GetVulkanVersion()|获取当前实例的 Vulkan 版本 
public | size_t |GetSupportLayerCount()|获取当前实例支持的验证层数量 
public | std::vector<[TLayer](TLayer.md)> |GetSupportLayers()|获取当前实例支持的验证层
public | bool |IsSupportLayer(<br/>std::string layerName<br/>)|通过验证层名称查看当前实例是否支持该验证层
public | bool |IsSupportLayer(<br/>[TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) layerType<br/>);|通过验证层枚举查看当前实例是否支持该验证层
public | size_t |GetSupportExtensionCount()|获取当前实例支持的扩展数量 
public | std::vector<[TExtension](TExtension.md)> |GetSupportExtensions()|获取当前实例支持的扩展
public | bool |IsSupportExtension(<br/>std::string extensionName<br/>)|通过验证层名称查看当前实例是否支持该验证层
public | bool |IsSupportExtension(<br/>[TExtensionType](TExtensionType.md) extensionType<br/>);|通过验证层枚举查看当前实例是否支持该验证层
public | size_t | GetEnabledLayersCount() |获取当前激活的验证层数量
public | std::vector<[TLayer](TLayer.md)> |GetEnabledLayers()|获取当前激活的验证层
public | size_t | GetEnabledExtensionCount() |获取当前激活的验证层数量
public | std::vector<[TExtension](TExtension.md)> |GetEnabledExtensions()|获取当前激活的验证层
public | std::string | virtual ToString() = 0|转成字符信息

[^_^]: # (哈哈我是注释，不会在浏览器中显示。)

[^_^]: # (API Block)

<br/>

## **Function Documentation**

<br/>

### public void explicit **TInstance**( <br/>[TInstanceCreateInfo](TInstanceCreateInfo.md)* createInfo = nullptr,<br/> const VkAllocationCallbacks* pAllocator = nullptr<br/>)   

---
构造函数

* <font color="#0000dd">createInfo</font> 构造参数。如果`createInfo`为`nullptr`的话，Turbo内部会构建一个不支持任何验证层和扩展并且版本为`Vulkan 1.0`的Vulkan实例。如果`createInfo`不为`nullptr`Turbo会按照`createInfo`构建Vulkan实例。

* <font color="#0000dd">pAllocator</font> 内存分配器。当`pAllocator`为`nullptr`时，Turbo内部调用Vulkan默认的分配器(Vulkan默认分配器为`nullptr`)。如果不为`nullptr`的话将会调用`pAllocator`指定的分配器。Turbo也提供了专用分配器[TAllocator](TAllocator.md)。

#### Return
如果未创建成功将会抛出[TException](TException.md)异常，反之则不会抛出异常。


<br/>

[^_^]: # (API Block)

### public explicit **TInstance**(<br/>const [TInstance](TInstance.md)& instance<br/>) = delete

---

移除拷贝构造函数。请用指针或者引用获取。该设计防止错误销毁Vulkan实例

<br/>

[^_^]: # (API Block)

### public **~TInstance**() 

---

如果内部成功创建Vulkan实例，该函数会将Vulkan实例销毁，所以请妥善管理[TInstance](TInstance.md)对象

<br/>

[^_^]: # (API Block)

### public static bool **IsSupportVulkan**( )

---

是否支持Vulkan。

#### Return
* <font color="#0000dd">true</font> 支持Vulkan
* <font color="#0000dd">false</font> 不支持Vulkan

<br/>

[^_^]: # (API Block)

### public static [TVersion](TVersion.md) **GetVulkanInstanceVersion**()

---

获取当前支持的最高Vulkan版本。注意Turbo中除了[IsSupportVulkan](#public-static-bool-issupportvulkan)函数会检查是否支持Vulkan外，其他所有函数均默认当前环境支持Vulkan

#### Return 

当前最高Vulkan版本

<br/>

[^_^]: # (API Block)

### public VkInstance **GetVkInstance**()

---

获取Vulkan实例VkInstance句柄

#### Return

VkInstance实例句柄

<br/>

[^_^]: # (API Block)

### public [TVersion](TVersion.md) **GetVulkanVersion**()

---

获取当前实例的Vulkan版本

#### Return
当前实例的Vulkan版本

<br/>

[^_^]: # (API Block)

### public size_t **GetSupportLayerCount**()

---

获取当前支持的验证层数量

#### Return

支持的验证层数量

<br/>

[^_^]: # (API Block)

### public std::vector<[TLayer](TLayer.md)> **GetSupportLayers**()

---

获取当前支持的验证层

#### Return

当前支持的验证层数组

<br/>

[^_^]: # (API Block)

### public bool **IsSupportLayer**(<br/>std::string layerName<br/>)

---

是否支持指定验证层

* <font color="#0000dd">layerName</font> 验证层的名称字符串。

#### Return 

* <font color="#0000dd">true</font> 支持。
* <font color="#0000dd">false</font> 不支持。

<br/>

[^_^]: # (API Block)

### public bool **IsSupportLayer**(<br/>[TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) layerType<br/>)
---

是否支持指定验证层

* <font color="#0000dd">layerType</font> 验证层的名称枚举。

#### Return 

* <font color="#0000dd">true</font> 支持。
* <font color="#0000dd">false</font> 不支持。

<br/>

[^_^]: # (API Block)

### public size_t **GetSupportExtensionCount**()

---

获取支持的扩展数量

#### Return

支持的扩展数量

<br/>

[^_^]: # (API Block)

### public std::vector<[TExtension](TExtension.md)> **GetSupportExtensions**()

---

获取支持的扩展

#### Return

支持的扩展数组

<br/>

[^_^]: # (API Block)

### public bool **IsSupportExtension**(<br/>std::string extensionName<br/>)

---

是否支持指定扩展

* <font color="#0000dd">extensionName</font> 扩展名称字符串 

#### Return 

* <font color="#0000dd">true</font> 支持。
* <font color="#0000dd">false</font> 不支持。

<br/>

[^_^]: # (API Block)

### public bool **IsSupportExtension**(<br/>[TExtensionType](TExtensionType.md) extensionType<br/>)

---

是否支持指定扩展

* <font color="#0000dd">extensionType</font> 扩展名称类型枚举 

#### Return 

* <font color="#0000dd">true</font> 支持。
* <font color="#0000dd">false</font> 不支持。


<br/>

[^_^]: # (API Block)

### public size_t **GetEnabledLayersCount**()

---

获取当前激活的验证层的数量

#### Return

激活的验证层的数量

<br/>

[^_^]: # (API Block)

### public std::vector<[TLayer](TLayer.md)> **GetEnabledLayers**()

---

获取当前激活的验证层

#### Return

激活的验证层数组

<br/>

<br/>

[^_^]: # (API Block)

### public size_t **GetEnabledExtensionCount**()

---

获取当前激活扩展的数量

#### Return
当前激活扩展的数量

<br/>

[^_^]: # (API Block)

### public std::vector<[TExtension](TExtension.md)> **GetEnabledExtensions**()

---


获取当前激活扩展

#### Return
当前激活扩展数组

<br/>

[^_^]: # (API Block)

### [public](#tinstance) std::string virtual **ToString**() = 0

---

转成字符信息

#### Return
信息字符串

<br/>