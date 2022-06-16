# TLayer
class in Turbo::Core / Inherits from: [TObject](TObject.md) / header: TLayer.h

<br/>

## **Description**

该类代表Vulkan中的验证层。可通过该类获取实例和设备支持的验证层相关信息。Turbo中每一个TLayer都代表一个单独的验证层。

<br/>

## **Properties**

Scope    | Type        | Name                  		  |description
---------|-------------|--------------------------------|-------------
public   | friend class| [TInstanceCreateInfo](TInstanceCreateInfo.md)|TLayer 的友元类为 [TInstanceCreateInfo](TInstanceCreateInfo.md)
public   | friend class| [TDeviceCreateInfo](TDeviceCreateInfo.md)|TLayer 的友元类为 [TDeviceCreateInfo](TDeviceCreateInfo.md)
private  | [TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype)  | layerType    |验证层枚举
private  | std::string| name|验证层名称
private  | TVersion  | specificationVersion|验证层Vulkan规范版本
private  | uint32_t  |implementationVersion |验证层实现版本
private  | std::string  | description|验证层描述
private  | std::vector<[TExtension](TExtension.md)>  |extensions |验证层支持的扩展

<br/>

## **Function**

Scope  		  | Return      						     | Function              |description
--------------|------------------------------------------|-----------------------|-------------
public  	  |											 | explicit TLayer() 	 	|TLayer 构造函数
public  	  |         								 | ~TLayer()   			 	|TLayer 析构函数
public static | size_t									 | GetInstanceLayerCount( ) |获取实例支持的验证层数量
public static | std::vector<[TLayer](TLayer.md)> 		 | GetInstanceLayers()    	|获取当前实例支持的验证层
public static | size_t 									 |GetPhysicalDeviceLayerCount(<br/>TPhysicalDevice* physicalDevice<br/>)|获取当前物理设备支持的验证层数量 
public static | std::vector<[TLayer](TLayer.md)>		 |GetPhysicalDeviceLayers(<br/>TPhysicalDevice* physicalDevice<br/>)|获取当前物理设备支持的验证层
public stctic | [TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) 			 |GetLayerTypeByLayerName(<br/>std::string layerName<br/>)|将验证层名称字符串转换成Turbo定义的验证层枚举类型
public static | std::string 							 |GetLayerNameByLayerType(<br/>[TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) layerType<br/>);|将验证层枚举类型转换成验证层名称字符串
public static | bool 									 |IsSupportInstanceLayer(<br/>[TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) layerType<br/>)|当前Vulkan是否支持指定验证层
public 		  | [TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) 			 |GetLayerType()				|获取当前验证层枚举类型
public 		  | std::string 							 |GetName()						|获取当前验证层名称字符串
public 		  | [TVersion](TVersion.md) 				 |GetSpecificationVersion()		|获取验证层Vulkan规范版本
public 		  | uint32_t 								 | GetImplementationVersion() 	|获取当前验证层的实现版本
public 		  | std::string 							 |GetDescription()				|获取当前验证层的描述
public 		  | size_t 									 | GetExtensionCount() 			|获取当前验证层所支持的扩展数量
public 		  | [TExtension](TExtension.md) 			 |GetExtension(uint32_t index)	|获取当前验证层特定索引上所支持的扩展
public 		  | std::vector<[TExtension](TExtension.md)> |GetExtensions()				|获取当前验证层所支持的扩展
public 		  | bool 									 | operator == (<br/>const [TLayer](TLayer.md)& layer<br/>)const|重载相等操作
public 		  | bool 									 | operator != (<br/>const [TLayer](TLayer.md)& layer<br/>)const|重载不相等操作
public 		  | std::string 							 | virtual ToString() override|转成字符信息

[^_^]: # (哈哈我是注释，不会在浏览器中显示。)

[^_^]: # (API Block)

<br/>

## **Function Documentation**

<br/>

### public void explicit **TLayer**()   

---
构造函数


<br/>

[^_^]: # (API Block)

### public **~TLayer**() 

---

析构函数

<br/>

[^_^]: # (API Block)

### public static size_t **GetInstanceLayerCount**( )

---

获取实例支持的验证层数量

#### Return
实例支持的验证层数量，如果返回`0`说明不支持任何验证层。

<br/>

[^_^]: # (API Block)

### public static std::vector<[TLayer](TLayer.md)> **GetInstanceLayers**()

---

获取实例支持的验证层

#### Return 

支持的验证层数组，如果不支持任何验证层将会返回没有任何元素的空数组

#### Note

<font color="#dddd00">每次调用该函数都会重新查询支持的验证层，重新生成信息数组，多次频繁的调用会影响性能。</font>

<br/>

[^_^]: # (API Block)

### public static size_t **GetPhysicalDeviceLayerCount**(<br/>TPhysicalDevice* physicalDevice<br/>)

---

获取当前物理设备支持的验证层数量

* <font color="#0000dd">physicalDevice</font> 物理设备。如果`physicalDevice`为`nullptr`的话，返回`0`。如果`physicalDevice`不为`nullptr`的话Turbo会按照`physicalDevice`去查询验证层数量。

#### Return

当前物理设备支持的验证层数量，如果为`0`说明当前物理设备不支持任何验证层

<br/>

[^_^]: # (API Block)

### public static std::vector<[TLayer](TVersion.md)> **GetPhysicalDeviceLayers**(<br/>TPhysicalDevice* physicalDevice<br/>)

---
获取当前物理设备支持的验证层

* <font color="#0000dd">physicalDevice</font> 物理设备。如果`physicalDevice`为`nullptr`的话，返回没有任何元素的空数组。如果`physicalDevice`不为`nullptr`的话Turbo会按照`physicalDevice`去查询物理设备支持的验证层。

#### Return
前物理设备支持的验证层数组，如果当前物理设备不支持任何验证层的话将会返回没有任何元素的空数组

#### Note
<font color="#dddd00">每次调用该函数都会重新查询支持的验证层，重新生成信息数组，多次频繁的调用会影响性能。</font>

<br/>

[^_^]: # (API Block)

### public stctic [TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) **GetLayerTypeByLayerName**(<br/>std::string layerName<br/>)

---

通过验证层名称获取验证层枚举类型

* <font color="#0000dd">layerName</font> 验证层名称。如果Turbo未找到对应的验证层枚举将会返回`TLayerType::UNDEFINED`,反之将会返回对应的枚举类型。

#### Return

验证层枚举类型

<br/>

[^_^]: # (API Block)

### public static std::string **GetLayerNameByLayerType**(<br/>[TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) layerType<br/>)

---

通过验证层枚举类型获取验证层名称

* <font color="#0000dd">layerType</font> 验证层枚举。如果Turbo未找到对应的验证层名称·将会返回没有任何字符的空字符串,反之将会返回对应的验证层名称字符串。

#### Return

验证层名称字符串

<br/>

[^_^]: # (API Block)

### public static bool **IsSupportInstanceLayer**(<br/>TLayerType layerType<br/>)

---

获取是否支持指定验证层。

* <font color="#0000dd">layerType</font> 验证类型枚举。

#### Return 

* <font color="#0000dd">true</font> 支持。
* <font color="#0000dd">false</font> 不支持。

#### Note

<font color="#dddd00">该函数内部会调用`GetInstanceLayers`函数，重新查询实例支持的验证层，多次频繁的调用会影响性能。</font>

<br/>

[^_^]: # (API Block)

### public [TLayerType](/Enum/Turbo/Core/TLayerType/#tlayertype) **GetLayerType**()
---

获取当前验证层类型，如果当前验证层未定义，将会返回`TLayerType::UNDEFINED`,在此情况下请调用`TLayer::GetName`函数获取验证层名称作为参考。


#### Return 

验证层类型

<br/>

[^_^]: # (API Block)

### public std::string **GetName**()

---

获取验证层名称

#### Return

验证层名称字符串

<br/>

[^_^]: # (API Block)

### public [TVersion](TVersion.md) **GetSpecificationVersion**()

---

获取验证层使用的Vulkan规范版本

#### Return

Vulkan规范版本

<br/>

[^_^]: # (API Block)

### public uint32_t **GetImplementationVersion**()

---

获取验证层实现版本

#### Return 

版本号

<br/>

[^_^]: # (API Block)

### public std::string **GetDescription**()

---

获取验证层的描述


#### Return 

描述字符串


<br/>

[^_^]: # (API Block)

### public size_t **GetExtensionCount**()

---

获取当前验证层支持的扩展的数量

#### Return

扩展数量

<br/>

[^_^]: # (API Block)

### public [TExtension](TExtension.md) **GetExtension**(<br/>uint32_t index<br/>)

---

获取特定索引的扩展

* <font color="#0000dd">index</font> 索引。`index`必须小于调用`GetExtensionCount`所返回的值，否则会返回`TException`异常，异常信息为`TResult::OUT_OF_RANGE`越界异常信息。

#### Return

激活的验证层数组

<br/>

<br/>

[^_^]: # (API Block)

### public std::vector<[TExtension](TExtension.md)> **GetExtensions**()

---

获取当前验证层支持的扩展

#### Return
扩展数组

<br/>

[^_^]: # (API Block)

### public bool **operator ==**(<br/>const TLayer& layer<br/>)const

---
重载比较操作符

* <font color="#0000dd">layer</font> 比较验证层。该函数在两个验证层`TLayer::layerType`相等时返回`true`,如果两个验证层`TLayer::layerType`不相等，比较两个验证层`TLayer::name`属性，如果一样则返回`ture`，否则返回`false`。

#### Return
* <font color="#0000dd">true</font> 两个`TLayer`之间相等
* <font color="#0000dd">false</font> 两个`TLayer`之间不相等

<br/>

[^_^]: # (API Block)

### public std::string virtual **ToString**() = 0

---

转成字符信息

#### Return
信息字符串。格式为：

`(##description) Vulkan version ##specificationVersion, layer version ##implementationVersion`

<br/>