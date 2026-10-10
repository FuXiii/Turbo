# FrameGraph

一帧渲染的有向无环图

主要有两个方面：

* `Pass` 渲染流程，如何渲染。
* `Resource` 资源，数据读写。

有三个阶段：

1. `Setup` 初始化构建一个 `Pass` 并配置 `Resource` 的读写。
2. `Compile` 编译。用于排查哪些 `Pass` 和 `Resource` 没有被使用，将其从图中剔除，并规划底层资源分配等。
3. `Execute` 执行真正的渲染，录制渲染指令，推送到 `GPU` 执行。

代码类似：

```CXX
struct MyPassData
{
    int a;
    float b;
};

int main(int argc, char **argv)
{
    TFrameGraph fg;
    fg.AddPass<MyPassData>(
        "NyPass",
        //Setup
        [&](TFrameGraph::TBuilder &builder, MyPassData &data) {
            // builder.Create<MyTexture>();
        },
        //Execute
        [](const MyPassData &data, const Resources &resources){ 
            vkCmdBeginRendePass(...);
            ...
            vkCmdEndRendePass();           
        }
    );

    fg.Compile();
    fg.Execute();
    return 0;
}
```

`FrameGraph` 类似一个容器。

## ID 与 代理

* 一个 `Pass` 包含多个可读，可写 `Resource` 。
* 一个 `Resource` 也可以被多个 `Pass` 读写。

在 `FrameGraph` 的前端 `Pass` 和 `Resource` 就是一个唯一无符号整型的 `ID` 号(比较好的策略是使用数组索引)。

由于资源是在 `Compile` 阶段之后创建的，所以其之前的所有资源都仅仅是个 `虚资源` ，或一个 `占位符` ，真正的数据存储在 `代理` 中。

在 `FrameGraph` 的后端根据 `Pass` 和 `Resource` 的 `ID` 号，指向各种对应的 `代理` 。而代理存储着对应确切的底层数据。

```CXX
using Resource = std::uint32_t;
using Pass = std::uint32_t;

class ResourceProxy
{};

class PassProxy
{};

class FrameGraph
{
    std::unordered_map<Resource, ResourceProxy*> resourceMap;
    std::unordered_map<Pass, PassProxy*> passMap;

    Pass passID = 0;
    Pass resourceID = 0;

    Pass AddPass(...)
    {
        passID += 1;
        passMap[passID] = nullptr;//真正的创建将会延期到 compile 或 execute 阶段，此处仅仅是占位符
    }
};
```

## 资源 与 资源代理

每一个资源都有与之对应的资源代理，而用户创建的资源会有各式各样的，所以具体的资源代理应该是个模板类，用于将用户自定义资源引入 `FrameGraph` 中。

```CXX
template<tyname T>
class TResourceProxy:public ResourceProxy
{
    private:
        T* resource = nullptr;//真正的创建将会延期到 compile 或 execute 阶段，此处仅仅是占位符
};
```

由于资源创建是延迟创建，当真正需要创建对应资源时 `FrameGraph` 在默认情况下只会调用 `T`（用户自定义资源类）的默认无参构造函数，但这种处理逻辑很多时候并不有效，用户自定义的资源很有可能没有无参构造函数。所以需要一种途径告诉 `FrameGraph` 如何构建用户自定义资源。一般有两种途径：

1. 回调函数
2. 调用约定的用户自定义资源创建函数（算是回调函数的一种变种）

这里采用第 `2` 种，原因是简单直观，编译提示可读性较强。

自定义资源创建约定如下：

* `FrameGraph` 在创建用户自定义资源 `T` 时会调用 `static T* T::Create(const T::Descriptor&‌ descriptor‌)` 自定义资源类静态函数。

还有一种设计思路：创建和销毁符合 `RAII` 设计思路：

* `FrameGraph` 在创建用户自定义资源 `T` 时会调用 `new T(const T::Descriptor&‌ descriptor‌)` 自定义资源类构造函数（并在要回收时直接 `delete` 即可）。

> 使用 `RAII` 或许是更好的选择

这样用户只需要在自定义类中实现约定好的函数， `FrameGraph` 就知道资源具体如何创建了。但这迎来了一个新问题：`T::Descriptor` 是什么？

### T::Descriptor

`T::Descriptor` 是用户自定义资源的创建参数类，该类也是用户自定义的：

```CXX
class MyCustomeResource
{
    public:
        class Descriptor//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;
        };

        static MyCustomeResource* Create(const MyCustomeResource::Descriptor& descriptor)
        {
            MyCustomeResource result=...;
            return result;
        }
};

//如果使用 RAII 思想设计
class MyCustomeResource
{
    public:
        class Descriptor//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;
        };

        MyCustomeResource(const MyCustomeResource::Descriptor& descriptor)
        {
            ...
        }
};
```

当 `Setup` 时需要注册资源的 `T::Descriptor` 参数值，这样 `FrameGraph` 才知道参数如何设置传递：

```CXX
struct MyPassData
{
    int a;
    float b;
};

int main(int argc, char **argv)
{
    TFrameGraph fg;
    fg.AddPass<MyPassData>(
        "NyPass",
        //Setup
        [&](TFrameGraph::TBuilder &builder, MyPassData &data) {

            MyCustomeResource::Descriptor descriptor = {};
            descriptor.a = 123.456f;
            descriptor.b = 91;

            builder.Create<MyCustomeResource>("MyCustomeResource", descriptor);
        },
        //Execute
        [](){ 
            vkCmdBeginRendePass(...);
            ...
            vkCmdEndRendePass();           
        }
    );

    fg.Compile();
    fg.Execute();
    return 0;
}
```

以上代码 `builder.Create<T>("XXX", T::Descriptor)` 就是向 `FrameGraph` 中注册一个用户自定义资源。

这样 `TResourceProxy` 就需要存储 `builder.Create<T>(...)` 时设置的 `T::Descriptor` ：

```CXX
template<tyname T>
class TResourceProxy:public ResourceProxy
{
    private:
        T::Descriptor descriptor;//存储用户自定义资源创建描述，用于延迟资源创建
        T* resource = nullptr;//真正的创建将会延期到 compile 或 execute 阶段，此处仅仅是占位符
};

resource = T::Create(descriptor);//这样在底层就可创建用户自定义资源
//如果使用 RAII 思想设计
resource = new T(descriptor);
```

这就要求 `T::Descriptor` 满足两个要求：

1. 存在默认构造函数
2. 存才拷贝赋值运算符

目前还有个问题：`TResourceProxy<T>` 中仅存储了 `T::Descriptor` 和一个等待延迟创建的 `resource`，其中 `T::Create(descriptor)` 在何处调用？

#### T::Create(descriptor)

`TResourceProxy<T>` 是继承自 `ResourceProxy` ,在这个父类中应该有一对创建和销毁资源的虚函数：

```CXX
class ResourceProxy
{
    public:
        virtual void Create() = 0;
        virtual void Destroy() = 0;
};
```

这样 `TResourceProxy<T>` 实现自己的创建和销毁函数即可：

```CXX
template<tyname T>
class TResourceProxy:public ResourceProxy
{
    private:
        T::Descriptor descriptor;//存储用户自定义资源创建描述，用于延迟资源创建
        T* resource = nullptr;//真正的创建将会延期到 compile 或 execute 阶段，此处仅仅是占位符
    public:
        void Create() override
        {
            this->resource = T::Create(descriptor);
            //如果使用 RAII 思想设计
            this->resource = new T(descriptor);
        }

        void Destroy() override
        {
            T::Destroy(this->resource);
            //如果使用 RAII 思想设计
            delete this->resource;
        }
};
```

销毁思路与创建类似。

由于资源完全可以自定义，所以很多常用资源都可以自定义，比如：

* Image
* ImageView
* RenderPass
* Framebuffer
* CommandBuffer

## Pass 与 资源

`pass` 可以有多个读写 `resource` 。

正常来说 如果一个 `pass` 没有写出 则该 `pass` 将会是一个无效 `pass` 正常来说会被从图中剔除。

但如果用户仅仅用该 `pass` 处理输入，比如说 `present`（显示）读取的资源，则该 `pass` 不应该被剔除。

所以 `pass` 需要一种途径 配置 是否需要从场景中剔除：

```cxx
void SetCull(bool)
```

如果用户将 `cull` 设置为 `false` 则该 `pass` 将不会被剔除。

有时用户需要创建自己的底层 `pass` 对象，此时可以将 `pass` 作为一种资源对象，但有一个问题：用户创建的底层 `pass` 如何获取 `fg` 创建的 `resource` ？在何时？何处获取？

如果按照资源来创建的话，`pass` 资源就需要在 `image` 资源创建完成后创建，并且 `pass` 创建回调中能够获取 `fg` 创建的图像资源。

所以资源创建需要有2个亮点特性：

1. 资源之间能够配置依赖
2. 创建资源时，能够获取到依赖的资源

### 资源依赖

如果用户创建 `VkRenderPass` 并不需要获取真正的资源，只需要知道目标资源的属性（比如 `fomat` ，`layout` ，`size` 等）即可创建`VkRenderPass` 。

但本 `fg` 不能设计为使用 `Vulkan`，该 `fg` 设计应该是图形接口无关的设计。

所以在资源创建设置依赖时，需要配置是否依赖底层创建的资源，如果依赖，则会在依赖资源在底层创建完成之后再创建该资源。

在配置资源依赖时，用户可以设置是否依赖底层的真正资源，如果依赖则在该资源创建完成之后创建，如果不依赖则只能获取到依赖资源的描述信息。

这样的话 `fg` 只负责资源的依赖和创建，具体的 `renderpass`，`subpass` 等，会留到 `execute` 阶段用户自定义调用（比如调用多个 `subpass` 渲染等）。

比如：

```txt
声明创建 读写 image
声明创建 renderpass
声明创建 pipeline
声明创建 Framebuffer

renderpass 依赖 image
pipeline 依赖 renderpass
Framebuffer 依赖 image 和 renderpass
```

之后 `fg` 会创建这些资源 在 `execute` 阶段通过录制 `GPU` 指令渲染即可。

### 配置与获取依赖

在 `setup` 阶段会创建（虚）资源，并将资源 `ID` 返回给调用端，使用该 `ID` 配置依赖：

```CXX
//setup
[&](TFrameGraph::TBuilder &builder, MyPassData &data) {

    MyCustomeResource::Descriptor descriptor = {};
    descriptor.a = 123.456f;
    descriptor.b = 91;

    auto cr_id = builder.Create<MyCustomeResource>("MyCustomeResource", descriptor);
    auto cr0_id = builder.Create<MyCustomeResource>("MyCustomeResource0", descriptor);
    auto cr1_id = builder.Create<MyCustomeResource>("MyCustomeResource1", descriptor);

    builder.DependsOn(cr_id, cr0_id);// cr_id 依赖 cr0_id
    builder.DependsOn(cr0_id, cr1_id);// cr0_id 依赖 cr1_id

    //或者更直接的接口
    cr_id.DependsOn(cr0_id);
    cr0_id.DependsOn(cr1_id);

    //这样的话创建循序将会是：cr1_id -> cr0_id -> cr_id
}
```

在资源创建时仅仅是传递了 `T::Descriptor` 参数类进去。目前有以下几种方式供用户获取依赖的资源：

1. 将资源依赖传递依托于 `T::Descriptor` 。
2. 再单独声明一个参数类，用于传递资源依赖。
3. 定义一个资源依赖接口 `class Depends`，之后 `T::Descriptor` 继承该依赖类。

如果采用 `3` 的话，这样用户只要自定义的描述继承自定义好的基类，就可以获取到相应资源了。

这样的话资源依赖配置直接使用定义好的基类即可：

```CXX
class DescriptorBase
{
public:
    void DependsOn(size_t id);
    template<typename T> T* Resource<T>(size_t id);
};
```

这样的话配置资源依赖就可以写成如下：

```CXX
class MyCustomeResource
{
public:
    class Descriptor: public fg::DescriptorBase{};
};

[&](TFrameGraph::TBuilder &builder, MyPassData &data) {

    MyCustomeResource::Descriptor descriptor = {};
    descriptor.a = 123.456f;
    descriptor.b = 91;

    auto cr1_id = builder.Create<MyCustomeResource>("MyCustomeResource1", descriptor);

    descriptor.DependsOn(cr1_id);
    auto cr0_id = builder.Create<MyCustomeResource>("MyCustomeResource0", descriptor);

    descriptor.DependsOn(cr0_id);
    auto cr_id = builder.Create<MyCustomeResource>("MyCustomeResource", descriptor);

    //这样的话创建循序将会是：cr1_id -> cr0_id -> cr_id
}
```

这会导致一个问题：依赖必须从后往前声明，要不 `Create` 传入的依赖可能还未声明出来。

所以还是在调用资源的创建函数（或构造函数）将依赖资源传递进去：

```CXX
class MyCustomeResource
{
    public:
        class Descriptor//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;
        };

        static MyCustomeResource* Create(const MyCustomeResource::Descriptor& descriptor, const fg::Resources& depends)
        {
            MyCustomeResource result = ...;

            auto resource = depends.Get<XXX>(id);
            for(auto&item:depends)
            {
                ...
            }

            return result;
        }
};

//如果使用 RAII 思想设计
class MyCustomeResource
{
    public:
        class Descriptor//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;
        };

        MyCustomeResource(const MyCustomeResource::Descriptor& descriptor, const fg::Resources& depends)
        {
            ...
        }
};
```

这会引发一个问题：`depends.Get<XXX>(id)` 中的 `id` 从何处得知？

可以将 `cr_id.DependsOn(cr0_id)` , `T::Descriptor` 和 `T::Descriptor` 基类相互结合：

还是使用  `cr_id.DependsOn(cr0_id)` 配置资源依赖，通过 `GetDescriptor<T>(id)` 获取对应 `id` 资源的描述数据，再将依赖的资源 `id` 记录在描述信息中，这样在资源真正创建时就知道资源与 `id` 的对应关系。

```CXX
class MyCustomeResource
{
    public:
        class Descriptor//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;

                size_t dependID = 0;
        };

        static MyCustomeResource* Create(const MyCustomeResource::Descriptor& descriptor, const fg::Resources& depends)
        {
            MyCustomeResource result = ...;

            auto resource = depends.Get<XXX>(descriptor.dependID);
            
            return result;
        }
};

//如果使用 RAII 思想设计
class MyCustomeResource
{
    public:
        class Descriptor//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;

                size_t dependID = 0;
        };

        MyCustomeResource(const MyCustomeResource::Descriptor& descriptor, const fg::Resources& depends)
        {
            ...
        }
};

//setup
[&](TFrameGraph::TBuilder &builder, MyPassData &data) {

    MyCustomeResource::Descriptor descriptor = {};
    descriptor.a = 123.456f;
    descriptor.b = 91;

    auto cr_id = builder.Create<MyCustomeResource>("MyCustomeResource", descriptor);
    auto cr0_id = builder.Create<MyCustomeResource>("MyCustomeResource0", descriptor);
    auto cr1_id = builder.Create<MyCustomeResource>("MyCustomeResource1", descriptor);

    builder.DependsOn(cr_id, cr0_id);// cr_id 依赖 cr0_id
    builder.DependsOn(cr0_id, cr1_id);// cr0_id 依赖 cr1_id

    //或者更直接的接口
    cr_id.DependsOn(cr0_id);
    cr0_id.DependsOn(cr1_id);

    //这样的话创建循序将会是：cr1_id -> cr0_id -> cr_id

    auto& target_descriptor = builder.GetDescriptor<MyCustomeResource::Descriptor>(cr_id);
    target_descriptor.dependID = cr0_id;

    auto& target_descriptor0 = builder.GetDescriptor<MyCustomeResource::Descriptor>(cr0_id);
    target_descriptor0.dependID = cr1_id;
}
```

这会引发一个问题：`cr_id.DependsOn(cr0_id)` 已经指定了依赖资源 `id` 而 `target_descriptor.dependsID = cr0_id` 又存了一份感觉重复。感觉有点麻烦。

虽然稍微有些繁琐，但这应该是最灵活的方式了。可以简化代码将配置依赖和赋值一起做：

```CXX
//setup
[&](TFrameGraph::TBuilder &builder, MyPassData &data) {

    MyCustomeResource::Descriptor descriptor = {};
    descriptor.a = 123.456f;
    descriptor.b = 91;

    auto cr_id = builder.Create<MyCustomeResource>("MyCustomeResource", descriptor);
    auto cr0_id = builder.Create<MyCustomeResource>("MyCustomeResource0", descriptor);
    auto cr1_id = builder.Create<MyCustomeResource>("MyCustomeResource1", descriptor);

    auto& target_descriptor = builder.GetDescriptor<MyCustomeResource::Descriptor>(cr_id);
    auto& target_descriptor0 = builder.GetDescriptor<MyCustomeResource::Descriptor>(cr0_id);

    target_descriptor.dependID =  builder.DependsOn(cr_id, cr0_id);// cr_id 依赖 cr0_id
    target_descriptor0.dependID = builder.DependsOn(cr0_id, cr1_id);// cr0_id 依赖 cr1_id

    //或者更直接的接口

    target_descriptor.dependID = cr_id.DependsOn(cr0_id);//返回 cr0_id
    target_descriptor0.dependID = cr0_id.DependsOn(cr1_id);//返回 cr1_id

    //这样的话创建循序将会是：cr1_id -> cr0_id -> cr_id
}
```

在资源真正被创建时需要通过 `const fg::Resources& depends` 参数获取依赖的资源，为了简单该参数可合并到 `描述符基类` 中，这样只需要一个用户自定义继承自  `描述符基类` 的描述符类即可，省了再次创建一个新参数。

```CXX
//如果使用 RAII 思想设计
class MyCustomeResource
{
    public:
        class Descriptor:public fg::DescriptorBase//或者使用 struct
        {
            public:
                float a = 0;
                char b = 0;
                void* c = nullptr;

                size_t dependID = 0;
        };

        MyCustomeResource(const MyCustomeResource::Descriptor& descriptor)
        {
            auto resource = descriptor.GetResource<XXX>(descriptor.dependID);
        }
};
```

这样 `fg` 负责在调用真正的资源创建前在对应的资源描述符中准备好依赖的资源。

### 资源依赖存储

资源的依赖资源仅将资源 `ID` 进行存储即可。

```CXX
class ResourceProxy
{
public:
    std::vector<ResourceID> depends;//元素不重复容器
};

a.DependsOn(b);//本质上找到 a 的资源代理 a'，将 b 保存到 a' 的 `ResourceProxy::depends` 中。
```

依赖可能导致链式依赖，比如 `a->b->c->d` 依赖链，`fg` 会自动查询依赖的资源是否已创建，未创建将会自动创建。

## Pass 与 Pass代理

`fg` 中的 `Pass` 在底层代理主要需要保存如下信息：

* 创建的资源（元素不重复容器，只需要存储 `ID`）
* 读取的资源（元素不重复容器，只需要存储 `ID`）
* 写入的资源（元素不重复容器，只需要存储 `ID`）
* Pass 资源清单数据（用户自定义结构体或类，一般是个结构体）
* setup 函数（注：这个函数一般不用存储，在 `fg.AddPass<XXX>(...)` 的内部直接调用了，不需要存储做延迟调用）
* execute 函数

### Pass 代理 与资源

一个 `Pass`
