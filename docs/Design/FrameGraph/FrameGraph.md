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
            descriptor.a = &fg;

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

由于资源完全可以自定义，所有很多常用资源都可以自定义，比如：

* Image
* ImageView
* RenderPass
* Framebuffer
* CommandBuffer

## Pass 与 Pass代理
