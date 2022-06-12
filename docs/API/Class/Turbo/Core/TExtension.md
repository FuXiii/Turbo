# TExtension

<!--TExtensionType::/Enum/Turbo/Core/TExtensionType/-->

class in Turbo::Core / Inherits from: [TObject](TObject.md) / header: TExtension.h

<br/>

## **Description**

该类代表Vulkan中的扩展。可通过该类获取实例和设备支持的扩展相关信息。Turbo中每一个TExtension都代表一个单独的扩展。

<br/>

## **Properties**

Scope    | Type        | Name                  		  		|description
---------|-------------|------------------------------------|-------------
public   | friend class| [TLayer](TLayer.md)|TExtension 的友元类为 [TLayer](TLayer.md)
public   | friend class| [TInstanceCreateInfo](TInstanceCreateInfo.md)|TExtension 的友元类为 [TInstanceCreateInfo](TInstanceCreateInfo.md)
public   | friend class| [TDeviceCreateInfo](TDeviceCreateInfo.md)|TExtension 的友元类为 [TDeviceCreateInfo](TDeviceCreateInfo.md)
public   | friend class| [TPhysicalDeviceProxy](TPhysicalDeviceProxy.md)|TExtension 的友元类为 [TPhysicalDeviceProxy](TPhysicalDeviceProxy.md)
private  | [TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype)  | extensionType    |扩展枚举
private  | std::string| name|扩展名称
private  | uint32_t  |version |扩展版本

<br/>

## **Function**

Scope  		  | Return      						     | Function              |description
--------------|------------------------------------------|-----------------------|-------------
public  	  |											 | explicit TExtension() 	|TExtension 构造函数
public  	  |         								 | ~TExtension()   			|TExtension 析构函数
public static | size_t | GetInstanceExtensionCount(<br/>[TLayer](TLayer.md)* layer = nullptr<br/>) |获取实例支持的扩展数量
public static | std::vector<[TExtension](TExtension.md)> | GetInstanceExtensions(<br/>[TLayer](TLayer.md)* layer = nullptr<br/>)    	|获取当前实例支持的扩展
public static | size_t |GetPhysicalDeviceExtensionCount(<br/>[TPhysicalDevice](TPhyscialDevice.md)* physicalDevice, <br/>[TLayer](TLayer.md)* layer = nullptr<br/>)|获取当前物理设备支持的扩展数量 
public static | std::vector<[TExtension](TExtension.md)> |GetPhysicalDeviceExtensions(<br/>[TPhysicalDevice](TPhyscialDevice.md)* physicalDevice, <br/>[TLayer](TLayer.md)* layer = nullptr<br/>)|获取当前物理设备支持的扩展
public stctic | [TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype)|GetExtensionTypeByExtensionName(<br/>std::string extensionName<br/>)|将扩展名称字符串转换成Turbo定义的扩展类型枚举类型
public static | std::string |GetExtensionNameByExtensionType(<br/>[TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype) extensionType<br/>);|将扩展枚举类型转换成扩展名称字符串
public static | bool |IsSupportPhysicalDeviceExtension(<br/>[TPhysicalDevice](TPhysicalDevice.md)* physicalDevice,<br/>[TExtensionType](TExtensionType.md) extensionType)|当前Vulkan是否支持指定扩展
public | [TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype)|GetExtensionType()|获取当前扩展枚举类型
public | std::string |GetName() |获取当前扩展名称字符串
public | uint32_t |GetVersion() |获取扩展版本
public | bool | operator == (<br/>const [TExtension](TExtension.md)& extension<br/>)const|重载相等操作
public | bool | operator != (<br/>const [TExtension](TExtension.md)& extension<br/>)const|重载不相等操作
public | std::string | virtual ToString() override|转成字符信息

[^_^]: # (哈哈我是注释，不会在浏览器中显示。)

[^_^]: # (API Block)

<br/>

## **Function Documentation**

<br/>

### public void explicit **TExtension**()

---
构造函数


<br/>

[^_^]: # (API Block)

### public **~TExtension**() 

---

析构函数

<br/>

[^_^]: # (API Block)

### public static size_t **GetInstanceExtensionCount**(<br/>[TLayer](TLayer.md)* layer = nullptr<br/> )

---

获取实例扩展的数量

* <font color="#0000dd">layer</font> 验证层。如果`layer`为`nullptr`的话，该函数将会返回Vulkan实例支持的扩展数量，如果`layer`不为`nullptr`的话该函数将会返回该`layer`所支持的扩展数量。

#### Return
实例支持的扩展数量，如果返回`0`说明不支持任何扩展。

<br/>

[^_^]: # (API Block)

### public static std::vector<[TExtension](TExtension.md)> **GetInstanceExtensions**(<br/>[TLayer](TLayer.md)* layer = nullptr<br/>)

---

获取实例支持的验证层

#### Return 

支持的扩展数组，如果不支持任何扩展该函数将会返回没有任何元素的空数组

#### Note

<font color="#dddd00">每次调用该函数都会重新查询支持的扩展，重新生成信息数组，多次频繁的调用会影响性能。</font>

<br/>

[^_^]: # (API Block)

### public static size_t **GetPhysicalDeviceExtensionCount**(<br/>[TPhysicalDevice](TPhysicalDevice.md)* physicalDevice,<br/> [TLayer](TLayer.md)\* layer = nullptr<br/>)

---

获取当前物理设备支持的扩展数量

* <font color="#0000dd">physicalDevice</font> 物理设备。如果`physicalDevice`为`nullptr`的话，返回`0`。如果`physicalDevice`不为`nullptr`的话Turbo会按照`physicalDevice`去查询扩展数量。

* <font color="#0000dd">layer</font> 验证层。如果`physicalDevice`不为`nullptr`并且`layer`不为`nullptr`的话，该函数将会返回当前`physicalDevice`中的`layer`(如果支持该验证层)中的扩展数量。反之如果`physicalDevice`不为`nullptr`而`layer`为`nullptr`的话，将会返回`physicalDevice`支持的扩展数量。

#### Return

当前物理设备支持的验证层数量，如果为`0`说明当前物理设备不支持任何验证层

<br/>

[^_^]: # (API Block)

### public static std::vector<[TExtension](TExtension.md)> **GetPhysicalDeviceExtensions**(<br/>TPhysicalDevice* physicalDevice,<br/>TLayer* layer = nullptr<br/>)

---
获取当前物理设备支持的扩展

* <font color="#0000dd">physicalDevice</font> 物理设备。如果`physicalDevice`为`nullptr`的话，返回`0`。如果`physicalDevice`不为`nullptr`的话Turbo会按照`physicalDevice`去查询扩展。

* <font color="#0000dd">layer</font> 验证层。如果`physicalDevice`不为`nullptr`并且`layer`不为`nullptr`的话，该函数将会返回当前`physicalDevice`中的`layer`(如果支持该验证层)中的扩展。反之如果`physicalDevice`不为`nullptr`而`layer`为`nullptr`的话，将会返回`physicalDevice`支持的扩展。

#### Return
物理设备支持的扩展数组，如果当前物理设备不支持任何扩展的话将会返回没有任何元素的空数组

#### Note
<font color="#dddd00">每次调用该函数都会重新查询支持的验证层，重新生成信息数组，多次频繁的调用会影响性能。</font>

<br/>

[^_^]: # (API Block)

### public stctic [TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype) **GetExtensionTypeByExtensionName**(<br/>std::string extensionName<br/>)

---

通过验证层名称获取验证层枚举类型

* <font color="#0000dd">extensionName</font> 验证层名称。如果Turbo未找到对应的验证层枚举将会返回`TLayerType::UNDEFINED`,反之将会返回对应的枚举类型。

#### Return

扩展枚举类型

<br/>

[^_^]: # (API Block)

### public static std::string **GetExtensionNameByExtensionType**(<br/>[TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype) extensionType<br/>)

---

通过扩展枚举类型获取扩展名称

* <font color="#0000dd">extensionType</font> 扩展枚举。如果Turbo未找到对应的扩展名称·将会返回没有任何字符的空字符串,反之将会返回对应的扩展名称字符串。

#### Return

扩展名称字符串

<br/>

[^_^]: # (API Block)

### public static bool **IsSupportPhysicalDeviceExtension**(<br/>[TPhysicalDevice](TPhysicalDevice.md)* physicalDevice,<br/> [TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype) extensionType<br/>)

---

获取指定物理设备是否支持指定扩展。

* <font color="#0000dd">physicalDevice</font> 物理设备。如果为`nullptr`则直接返回`false`,如果不为`nullptr`的话，则在`physicalDevice`上进行扩展查询。
* <font color="#0000dd">extensionType</font> 要查询的扩展型枚举。

#### Return 

* <font color="#0000dd">true</font> 支持。
* <font color="#0000dd">false</font> 不支持。

#### Note

<font color="#dddd00">该函数内部会调用`TExtension::GetPhysicalDeviceExtensions`函数，重新查询实例支持的扩展，多次频繁的调用会影响性能。</font>

<br/>

[^_^]: # (API Block)

### public [TExtensionType](/Enum/Turbo/Core/TExtensionType/#textensiontype) **GetExtensionType**()
---

获取当前扩展类型，如果当前扩展未定义，将会返回`TLayerType::UNDEFINED`,在此情况下请调用`TExtension::GetName`函数获取扩展名称作为参考。


#### Return 

扩展类型

<br/>

[^_^]: # (API Block)

### public std::string **GetName**()

---

获取扩展名称

#### Return

扩展名称字符串

<br/>

[^_^]: # (API Block)

### public uint32_t **GetVersion**()

---

获取扩展的版本

#### Return

扩展的版本

<br/>

[^_^]: # (API Block)

### public bool **operator ==**(<br/>const [TExtension](TExtension.md)& extension<br/>)const

---
重载比较操作符

* <font color="#0000dd">extension</font> 比较扩展。该函数在两个扩展`TExtension::extensionType`相等时返回`true`,如果两个扩展`TExtension::extensionType`不相等，比较两个验证层`TExtension::name`属性，如果一样则返回`ture`，否则返回`false`。

#### Return
* <font color="#0000dd">true</font> 两个`TExtension`之间相等
* <font color="#0000dd">false</font> 两个`TExtension`之间不相等

<br/>

[^_^]: # (API Block)

### public bool **operator !=**(<br/>const [TExtension](TExtension.md)& extension<br/>)const

---
重载比较操作符

* <font color="#0000dd">extension</font> 比较扩展。该函数在两个扩展`TExtension::extensionType`不相等时返回`true`,否则返回`false`。

#### Return
* <font color="#0000dd">true</font> 两个`TExtension`之间不相等
* <font color="#0000dd">false</font> 两个`TExtension`之间相等

<br/>

[^_^]: # (API Block)

### public std::string virtual **ToString**() = 0

---

转成字符信息

#### Return
信息字符串。格式为：

`(##ExtensionName) : extension revision  ##ExtensionVersion`

<br/>