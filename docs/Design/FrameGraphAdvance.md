# Turbo引擎使用FrameGraph驱动设计

最终会映射到`./docs/TurboDesign:Render`设计中

## 更新日志

* 2022/12/4
  >
  >* 结束琐碎设计文档化
  >* 创建`Turbo驱动设计`章节，用于开始`Turbo`引擎`Engine`层面与`FrameGraph`配合的设计

* 2022/12/5
  >
  >* 创建`资源的创建与销毁`章节
  >* 创建`Context上下文`章节
  >* 创建`WorldRender/Render 渲染器`章节

* 2022/12/7
  >
  >* 更新`资源的创建与销毁`章节
  >* 更新`Context上下文`章节

* 2022/12/8
  >
  >* 创建`Surface和Context`章节
---

来源于`docs/images`下的一些平日琐碎设计，该文档是琐碎设计的整理

> `Unity`中的一个材质就可以看成对应`fg`的一个`PassNode`，多个可渲染物体绑定同一个材质是，其实就是在当前的`Material`所对应的的那个`PassNode`内渲染绑定的`Mesh`，换句话说就是多个`DrawableObject`如果绑定了同一个`Material`，就在`Material`对应的`PassNode`下渲染多个`DrawableObject`

> 根据`Material`中的渲染配置生成对应的`RenderPass`、`Subpass`和`Pipeline`

随即引发了一些问题：

1. 这些材质应该放在`fg`的何处？，换句话说就是，材质对应的应该是哪个`PassNode`？这些`PassNode`的前后关系是啥？  
**答**：分两种情况：`GBuffer`的延迟渲染和`Forward`的前向渲染。还可能有特殊的自定义的`Material`可能会在后处理阶段等

2. 后处理材质放在何处？（属于问题1的范畴）

3. 比如`DepthPass`会去渲染所有物体，`ColorPass`也会去渲染所有物体，也就是说`DepthPass`中会有`Drawable`对`DepthMaterial`的绑定，`ColorPass`中会有`Drawable`对`ColorMaterial`的绑定。  
**答**：这时`Drawable`最好是引擎临时构建的，如果不是临时构建的话，在渲染前就要构建出很多`Drawable`对象，而很多对象只是材质不同，而`Mesh`都是相同的

---

> `Turbo`的目的是灵活性并不是强制用户使用`fg`，也就是说`fg`只是`Turbo`中可有可无的架构，为了达到这个目的`Turbo`需要细化`fg`中的步骤，将其抽出来作为单独的内容或是类，而`fg`只是使用这些`Turbo`引擎提供的功能类来将渲染流程驱动起来。比如如下结构流程：

```TXT
RenderPass
|_Pass0
|   |_inputs(对应PassNode的输入)
|   |_outputs(对应PassNode的输出)
|   |...(渲染时绑定)pipeline
|_Pass1
|   |_inputs(对应PassNode的输入)
|   |_outputs(对应PassNode的输出)
|   |...(渲染时绑定)pipeline
|_Pass...

Pipeline
|_vertex shader
|_...几何，细分等
|_fragment shader
|_各种pipeline配置
```

**如上对应示意的所有均为声明，Turbo引擎并不会构建真正的底层对象，只有运行时，渲染时会去构建相应的对象，以此来实现动态渲染**

> 首先明确的是`Material`的层级高于`fg`，也就是说`Material`用于最终声明对应的`PassNode`，`fg`不应该内嵌`Material`

```TXT
Mesh some_mesh;

Material color_material;
Material pbr_material;

MeshRender mesh_render0;
mesh_render0.mesh=some_mesh0;
mesh_render0.material=pbr_material;

MeshRender mesh_render1;
mesh_render1.mesh=some_mesh1;
mesh_render1.material=color_material;
```

转成对应的`fg`

```TXT
Material color_material => [Color PassNode]
Material pbr_material => [PBR PassNode]
```

现在解决两个问题：

1. 某个`MeshRender`何时进行真正的渲染绘制？  
**答**：引擎会收集所有绑定同一个`Material`的`MeshRender`的网格顶点信息(也就是`vertex buffer`之类的)，并将在对应的底层`PassNode`的`Execute`阶段渲染收集到的所有`MeshRender`  
*注*：为了快速得到`Material`都绑定了哪些`MeshRender`，有两种方案：
    1. `Material`内部维护一个数组，用于存放对应的`MeshRender`对象
    2. `MeshRender`中的`Material`只是`Material`的句柄，这样就可以快随找到对应的`Material`

---
> `Engine`与`FrameGraph`

```TXT
Engine中该有的类

Shader
|_Vertex Shader
|_Geometry Shader
|_细分着色器
|_Fragment Shader
|_光追标准着色器
|_...

Pipeline
|_Graphic Pipeline
|_Compute Pipeline
|_Raytracing Pieline

Image
|_Color Image
|_Depth Image
|_...

Subpass

RenderPass
```

对应代码结构示意：

```CXX
ColorImage some_color_image;
DepthImage some_depth_image;

VertexShader vertex_shader;
FragmentShader fragment_shader;

GraphicPipeline some_graphic_pipeline;
some_graphic_pipeline.SetVertexShader(vertex_shader);
some_graphic_pipeline.SetFragmentShader(fragment_shader);
some_graphic_pipeline.Set几何拓扑配置(...);
some_graphic_pipeline.Set透明融合配置(...);
some_graphic_pipeline.Set...配置(...);

Subpass subpass0;
subpass0.BindingColorImage(some_color_image);
subpass0.BindingDepthImage(some_depth_image);

Subpass subpass1;
subpass1.Binding...Image(...);

RenderPass render_pass;
render_pass.AddSubpass(subpass0);
render_pass.AddSubpass(subpass1);
```

`Engine`与`FrameGraph`对应示意

```TXT
RenderPass············PassNode
|_Subpass0············|_pass0
|_Subpass1············|_pass1
|_Subpassn············|_passn
              \||/
               \/
             Execute
             执行阶段
              \||/
               \/
 commandBuffer.BindingPipeline(...)
 commandBuffer.BindingVertexBuffer(...)
 commandBuffer.Draw(...)
```

---

> 渲染流程

```CXX
VertexShader vertex_shader;
FragmentShader fragment_shader;
ColorImage color_image;//或者叫RenderTarget更能体现其作用

GraphicPipeline some_graphic_pipeline;
some_graphic_pipeline.BindVertexShader(vertex_shader);
some_graphic_pipeline.BindFragmentShade(fragment_shader);

Mesh mesh;

//<对应FrameGraph::PassNode::Setup阶段>
Subpass subpass0;
subpass0.AddColorImage(color_image);

RenderPass render_pass;
render_pass.AddSubpass(subpass0);
//</对应FrameGraph::PassNode::Setup阶段>

//<对应FrameGraph::PassNode::Execute阶段>
CommandBuffer command_buffer;
command_buffer.BindRenderPass(render_pass);
command_buffer.BindPipeline(some_graphic_pipeline);
command_buffer.BindMesh(mesh);
command_buffer.Draw(...);
//</对应FrameGraph::PassNode::Execute阶段>

//注：PassNode的对应FrameBuffer需要引擎内部自动构建
//这就需要CommandBuffer也是Virtual虚的（因为按照Vulkan标准，CommandBuffer使用图形管线，必须提前构建好FrameBuffer）
```

**问**：真正的Pipeline和RenderPass何时创建？
**答**：如果使用`fg`的话，每一帧都会根据配置重新构建

**问**：如果不使用`fg`呢？

---
`FrameGraph`示意

```TXT
PassNode
    Setup
        //设置当前RenderPass的各种Subpass
        Subpass subpass0;
        subpass0.Read(someImage);
        subpass0.Write(someImage);

        Subpass subpass1;
        subpass1.Read(someImage);
        subpass1.Write(someImage);
    
    Execute
        1. 背后根据Setup阶段的配置创建RenderPass
        2. 背后根据RenderPass创建FrameBuffer
        3. 背后调用BeginRenderPass(图形管线)
        4. 调用各种渲染指令，例如:(BBBD)
        BindPipeline(...)
        BindVertexBuffer/BindIndexBuffer(...)
        BindPipelineData(...)
        Draw/DrawIndex(...)

        NextSubpass()
        BBBD
        ...

        NextSubpass()
        BBBD
        ...
```

**问**：Pipeline何时构建？  
**答**：现用先构建，或其他更好的方案

> `VertexBuffer`和`IndexBuffer`或许可以通过`Mesh`简化流程

---

**问**：目前`RenderPass`和`Subpass`基本上整理清楚了，但是`Pipeline`一直没有一个良好的架构，比如`Pipeline何时构建?  
**答**：由于`Vulkan`标准，在创建`Pipeline`是需要指定如下数据

1. RenderPass
2. Subpass
也就是说，`Pipeline`是特定于某一个`RenderPass`的某一个`Subpass`下的，换一句话说就是，`Pipeline`应该在`RenderPass::Begin`后`Subpass`执行时，`Pipeline`绑定中创建

**问**：如果`Pipeline`应该在`RenderPass::Begin`后`Subpass`执行时，`Pipeline`绑定中创建的话，会有个问题：

1. 当用户在同一个`RenderPass`下同一个`Subpass`下绑定多个相同的`Pipeline`
2. 当用户在同一个`RenderPass`下不同的`Subpass`下绑定多个相同的`Pipeline`
3. 当用户在不同`RenderPass`下不同的`Subpass`下绑定多个相同的`Pipeline`

会如何？

**答**：首先`Pipeline`只对某一`RenderPass`下的某一`Subpass`创建，这是一个`Pipeline`的最小活动范围，所以是个树状图。

```TXT
RenderPass
|_Subpass0
|   |_Pipeline 0...n
|_Subpass1
|   |_Pipeline 0...n
|_Subpass2
|   |_Pipeline 0...n
|_Subpass...
|   |_Pipeline 0...n
```

所以在绑定`Pipeline`时，引擎会根据当前的`RenderPass`下的当前`Subpass`中的`Pipeline`数组中找是否已经创建此`Pipeline`，如果已经创建就不用创建了，如果没创建就创建。

**问**：如何判断已经构建该`Pipeline0`？换句话说，如何判断两个`Pipeline`相等？  
**答**：目前能够想到的是主色器相同，配置相同，`RenderPass`相同，`Subpass`相同  
*注*：`Vulkan`标准中`Pipeline`有兼容性特性

---

**问**：如何创建`Pipeline`并保存在何处？如何管理？  
**答**：一个`Pipeline`属于某一个`RenderPass`下的某一个`Subpass`，一个`Subpass`可以绑定多个`Pipeline`，而对于`Vulkan`是基于`Command`驱动的，而不像`OpenGL`是基于`Context`。所以`Pipeline`正常来说生命周期应该和`CommandBuffer`相同，也就是说当`CommandBuffer`绑定`Pipeline`是引擎需要去查看当前`RenderPass`下的当前`Subpass`下有没有创建相应的`Pipeline`，如果
没有创建则创建，创建完直接使用。由于`Pipeline`每一帧都会重新创建，这样就可以满足动态改变`Pipeline`了。如果使用`fg`的话，`fg`，`Pipeline`，`CommandBuffer`的生命周期应该都是一样的，都是一帧的生命周期。

总结一下：

**如何创建`Pipeline`?**:
**答**：某一帧，某一`RenderPass`下某一`Subpass`下使用，`CommandBuffer`绑定时，引擎会使用当前`RenderPass`当前`Subpass`创建`Pipeline`并存入`CommandBuffer`中。

*注*：存入`CommandBuffer`可能不是最优解，可能搞一个类似一帧的`Context`之类会更加合理。

**保存在何处？**：  
**答**：搞一个类似一帧的`Context`之类会更加合理

**如何管理**：  
**答**：

* 增 ：搞一个类似一帧的`Context`之类会更加合理
* 删 ：在一帧`Context`结束时
* 改 ：用户动态改变`Pipeline`的配置，引擎根据用户配置生成真正的`Pipeline`
* 查 ：略 。目前没想到实际用途。

---

**问**：现在面临的问题是，如何判断某个被绑定的`RenderPass`的`Pipeline`已经被创建，而不需要重复创建。换句话说：如何判断两个`RenderPass`两`Pipeline`相同，相等？

---

用户端可能的用法（不使用`fg`）

```CXX
//创建一些有用的Image
vector<Images> some_images;

//声明一个Subpass
Subpass sp0;
sp0.AddColorImage(some_images[color_index]);
sp0.AddColorImage(some_images[depth_index]);

//声明一个Subpass
Subpass sp1;
sp0.AddInput(some_images[input1_index]);
sp0.AddColorImage(some_images[input2_index]);

//声明一个RenderPass
RenderPass rp;
rp.AddSubpass(sp0);
rp.AddSubpass(sp1);

//声明一些Pipeline
Pipeline pipeline0(拓扑类型，混合，深度...);
Pipeline pipeline1(拓扑类型，混合，深度...);
Pipeline pipeline...(拓扑类型，混合，深度...);

//创建CommandBuffer
CommandBuffeer cb;
cb.RenderPassBegin(rp);
cb.BindVertexBuffer(...);
cb.BindPipeline(some_pipeline);
cb.Draw(...)
cb.NextSubpass();
cb.[...];

//推送执行
DeviceQueue.Submit(cb);
```

---

现在要解决的问题是：  
**问**：如何断定两个`RenderPass`相等？如何断定两个`Pipeline`相等  
**原因**：因为用户绑定到`RenderPass`中的`Pipeline`只是个描述，引擎需要判断是否底层已经创建了符合此描述的对象。  
**答**：一帧的对应资源会存入`Context`中（这里的`Context`确切的说应该是一帧内拥有的所有资源和环境，包括所有的创建的`RenderPass`和`Pipeline`）

每一帧的`RenderPass`，`Subpass`和绑定的`Pipeline`都会变，所以一帧的资源函数需要存入`Context`中

结构示意：

```TXT
Context
|_RenderPass 0
|   |_Subpass 0------BindPipeline(....)
|   |_Subpass 1------BindPipeline(....)
|_RenderPass 1
|   |_Subpass 0------BindPipeline(....)
|   |_...
|_RenderPass ...
```

* 判断两个`RenderPass`相同。`RenderPass A``RenderPass B`  

```TXT
    所有对应的Subpass要兼容  
    如果B的Subpass数量小于等于A的数量，再判断  
        如果B的Subpass对应的写入写出与A的Subpass的相等
        否则两个RenderPass不相等
    否则两个RenderPass不相等
```

* 判断两个`Pipeline`相同(发生在`Pipeline`绑定时)

```TXT
如果两个Pipeline绑定到同一个RenderPass下的同一个Subpass，并且Pipeline配置相同，则两个Pipeline相同，反之则不相同
```

---

> `RenderPass`与资源(`Image`)之间的关系  

首先明确`RenderPass`并不会创建和销毁资源，`RenderPass`只绑定`Subpass`，`Subpass`中绑定`Image`资源，资源的创建和销毁应交由用户或`FrameGraph`驱动，资源的创建应该在`CommandBufer`绑定`RenderPass`之前完成，**销毁需要在合适的时候(异步的)**，在`CommandBuffer`绑定`RenderPass`时会基于`RenderPass`的`Subpass`对资源的引用情况去创建`FrameBuffer`，换句话说，`FrameBuffer`的生命周期与`RenderPass`同在，`FrameBuffer`应该存在`Context`中与`RenderPass`同在。当调用`CommandBuffer`的`BeginRenderPass()`时引擎会去找是否已经创建该`RenderPass`，如果已经创建，再去看对应的`FrameBuffer`有没有创建，如果没有创建就去创建，反之则不创建。

```TXT
                   没创建<------RenderPass创建了吗------>创建了
                     |                                   |
              RenderPass创建任务     创建了<------对应的FrameBuffer创建了吗？------>没创建
                                      |                                            |
                                     返回                                     创建对应的FrameBuffer     
```

---

> 首先像`Image`，`Buffer`这样的资源是不会采用构造函数去创建真正的资源的，而是调用对应资源类的`Create(...)`和`Destroy()`成员函数创建和销毁，所以资源只能通过`new`和`delete`创建和销毁对应的资源对象(注：这里不是资源，而是资源对象)，引擎架构中资源类中会存有真正的资源，而资源类的`Create(...)`和`Destroy()`成员函数则是真正去创建和销毁资源的函数。如果资源对象销毁了，而资源没有销毁，触发异常。

> 引擎架构中，资源类，`RenderPass`类，`Pipeline`类全都是先声明配置，后引擎会进行相应的创建和销毁，这样就可以实现动态改变资源，`RenderPass`和`Pipeline`了。其中`Shader`资源比较特殊，由于`Shader`的创建是个性能影响的元素，所以`Shader`资源的构造就是创建了相应的资源。

```CXX
VertexShader* vs=new ...(FilePath);
FragmentShader* fs=new ...(FilePath);

//Pipeline 不应该使用new创建，应该使用临时声明，并使用`Context`真正的在引擎内部进行管理
Pipeline pipeline(vs,fg);

ColorImage* color = new ColorImage();
color->Create(...);

DepthImage* depth = new DepthImage();
depth->Create(...);

Subpass sp0;
sp0.AddColor(color);
sp0.SetDepth(depth);

//RenderPass不应该使用new创建，同Pipeline一样用`Context`真正的在引擎内部进行管理
RenderPass rp;
rp.AddSubpass(sp0);

CommnadBuffer* cb=new ...;
cb->BeginRenderPass(rp);
cb->BindPipeline(pipeline);
cb->.../DrawCall(...);
cb->EndRenderPass();
```

---

首先将`CommandBuffer`中的`RenderPass`和`Pipeline`转移出来，到一个`Context`中，`Context`中存有一帧中所有的数据，包括当前真的`RenderPass`，`pipeline`，`CommandBuffer`等

---
> 如果不使用`FrameGraph`，那么`Turbo`的流程应该是

1. 初始化：包括`Instance`，`Device`，`DeviceQueue`的创建
2. 创建各种资源：最常见的就是`Image`，还有就是`Shader`等
3. 使用创建的各种资源声明`Subpass`
4. 使用声明的`Subpass`组建声明`RenderPass`
5. 由`CommandBuffer`去`Begin()`和`End()`绑定的`RenderPass`。在绑定时需要传入一帧的上下文`Context`，用于查看当前上下文是否已经创建了此`RenderPass`，如果已经创建了就不用创建了直接用，反之要创建后再用
6. 声明`Pipeline`（注意是声明，不是创建，创建`Pipeline`是引擎`Context`的任务）
7. 由`CommandBuffer`去`BindingPipeline`绑定`Pipeline`，同绑定`RenderPass`一样需要传入`Context`用于判断是否创建了此`Pipeline`
8. 将`CommandBuffer`提交并运行

---

接着不使用`FrameGraph`的说明，`Turbo`流程，现在整理一下那些任务时`FrameGraph`能够帮助我们完成的。

先强调几点：贴图纹理，`Shader`之类的资源还是需要手动管理的，`Context`将会帮助我们管理`RenderPass`和`Pipeline`。

`FrameGraph`能够帮助我们完成的：

1. 帮助我们管理`FrameBuffer`相关`Image`及其生命周期
2. 帮助我们声明和管理`RenderPass`及其内部的`Subpass`（这点很重要）
3. 帮助我们调用一些可以提前确定的指令，比如`RenderPassBegin()`，`RenderPassEnd()`等
4. 帮助我们将指令提交到`DeviceQueue`中

---
资源回收同步点

方案示意如下：

假如`Swapchain`中有三张图，正常来说`Swapchain`中每一张图片都是一帧，都是一个`FrameGraph`。

```TXT
Image 0;
Image 1;
Image 2;
```

```TXT
同步点：Fence0--->|0|Present(第1帧)
  同步点：Fence1--->|1|Present(第2帧)
    同步点：Fence2--->|2|Present(第3帧)
      同步点：Fence0--->|0|Present(第4帧)
        同步点：Fence1--->|1|Present(第5帧)
          同步点：Fence2--->|2|Present(第6帧)
            ...以此类推
---------------------------------------------->time
```

**解释**：每一帧都会等待上次用到自己(图片0,1,2)的那一帧结束后再使用自己，每一帧等待上次结束是回收资源的好时候，可能的行为：唤醒资源回收线程等。

---

# Turbo驱动设计

主要有两种资源`Image`和`Buffer`，每个资源内部都有一个`Descriptor`的结构体，用于创建时描述该资源。  
>`Image`派生有：
>
>* ColorImage
>* DepthImage
>* StencilImage
>* DepthStencilImage
>
>`ColorImage`派生有：
>
>* ColorImage1D（这个可能用处不多）
>* ColorImage2D
>* ColorImage3D
>
>`DepthImage`派生有：
>
>* DepthImage2D (二维的Depth纹理比较常用)
>
>`StencilImage`派生有：
>
>* StencilImage2D
>
>`DepthStencilImage`派生有：
>
>* DepthStencilImage2D

>`Buffer`派生有：
>
>* VertexBuffer
>* IndexBuffer

```CXX
typedef uint32_t TFlags;

enum TImageType
{
    1D,
    2D,
    3D
};

enum TFormat
{...};

enum TUsageFlagsBits
{
    TRANSFER，
    SAMPLED，
    STORAGE，
    COLOR_ATTACHMENT，
    DEPTH_STENCIL，
    INPUT_ATTACHMENT，
};
typedef TFlags TUsages;

enum TCreateFlagsBits
{
    CUBE//用于天空盒
};

class Image{
    struct Image::Descriptor
    {
        TImageType type;
        TFormat format;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uitn32_t mipLevels;
        uint32_t layers;
        //SampleCount //该属性由Turbo维护，Turbo提供顶层开启多采样功能接口
        TUsages usages;
        //ShaderMode //该属性由Turbo维护
        //initialLayout //该属性由Turbo维护
    };

void Create(const std::string &name, const Image::Descriptor &descriptor,void* allocator);
void Destroy();
};

class ColorImage: public Image
{
    struct ColorImage::Descriptor
    {
        TImageType type;
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uitn32_t mipLevels;
        uint32_t layers;
        //SampleCount //该属性由Turbo维护，Turbo提供顶层开启多采样功能接口
        TUsages usages;
        //ShaderMode //该属性由Turbo维护
        //initialLayout //该属性由Turbo维护
    };
};

class ColorImage2D: public Image
{
    struct ColorImage2D::Descriptor
    {
        //TImageType type; //该属性由Turbo维护
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;
        uint32_t height;
        //uint32_t depth; //该属性由Turbo维护，值为1
        uitn32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1
        //SampleCount //该属性由Turbo维护，Turbo提供顶层开启多采样功能接口
        TUsages usages;
        //ShaderMode //该属性由Turbo维护
        //initialLayout //该属性由Turbo维护
    };
};

class ColorImage3D: public ColorImage
{
    struct ColorImage3D::Descriptor
    {
        //TImageType type; //该属性由Turbo维护
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uitn32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1
        //SampleCount //该属性由Turbo维护，Turbo提供顶层开启多采样功能接口
        TUsages usages;
        //ShaderMode //该属性由Turbo维护
        //initialLayout //该属性由Turbo维护
    };
};
```

## 资源的创建与销毁

资源的创建与销毁需要一个资源分配器，而该资源分配器因该由`Context`上下文来创建

资源分配器命名为`TResourceAllocator`，其构造函数参数：`context`。

资源分配器只要分配两种资源：

1. Image
2. Buffer

资源分配器只分配基础资源，比如`class Image`类的资源，而不会分配其派生的子类(因为子类都是派生自`Image`)，`class Buffer`资源类似。

方案一（弃用）×
>
>* 由于创建`Turbo::Render::Image`需要返回`Turbo::Core::TImage`和`Turbo::Core::TImageView`两个类，所以`TResourceAllocator`在创建`Image`时需要返回`std::pair<Turbo::Core::TImage*, Turbo::Core::TImageView*>`。
>* 对于销毁`Turbo::Render::Image`，需要传入`Turbo::Core::TImage`和`Turbo::Core::TImageView`两个类，其中`Turbo`在销毁时查看`Turbo::Core::TImageView`是否能与`Turbo::Core::TImage`对应上，能对上就删除，对不上直接返回异常。

方案二（采纳）√
>
>* 由于`Turbo::Render::TContext`来创建和销毁`Turbo::Core::TImage`并由`Turbo::Render::TImage`来创建和销毁`Turbo::Core::TImageView`(其原因是`Turbo::Core::TImageView`其实可以动态的改变，当将某`Image`解释成`Color Image`，对应生成支持`Color`的`ImageView`即可，当想解释成`Depth`时，重新建立支持`Depth`的`ImageView`即可，灵活管理，方便扩展)

```CXX
class TResourceAllocator
{
    public:
    TResourceAllocator(TContext* context);

    Turbo::Core::TImage* CreateImage(const Turbo::Render::TImage::Descriptor& des)
    {
        //返回使用context创建的图片资源
        return context->CreateImage(des.width,des.height,des.depth,...);
    }

    void DestroyImage(Turbo::Core::TImage* image)
    {
        context->DestroyImage(image,imageView);
    }
}
```

`Buffer`同`Image`

## Context上下文

`Context`上下文中有整个`Turbo`的`Vulkan`环境，包括`Core::TInstance`、`Core::TPhysicalDevice`、`Core::TDevice`、`Core::TDeviceQueue`和各种`CommandBuffer`环境等

用户在构建上下文对象时，上下文的构造函数会去初始化环境。

在构造完`Context`之后，使用`Context`去构造`WorldRender/Render`进行后面渲染

`Context`需要提供`CreateImage(...)`，`DestroyImage(...)`，`CreateBuffer(...)`，`DestroyBuffer(...)`函数，用于创建和销毁资源

## WorldRender/Render 渲染器

用户在使用`Context`创建完`WorldRender/Render`后调用`WorldRender/Render::DrawFrame(...)`，其中`DrawFrame(...)`函数会去构建一帧的`FrameGraph`并进行一帧的渲染

## Surface、Swapchain和Context

### 方案一（弃用）×

在创建完`Context`后`Turbo`的基本环境已近构建，如果想要将渲染画面展示在屏幕上，还需要用户传入从窗口获得的`Surface`。

目前有以下几种情况：

1. 用户没有指定`Surface`（离屏渲染）
2. 用户指定了`Surface`
3. 用户更改了(重新指定了)`Surface`

>1. 对于**用户没有指定`Surface`**  
>如果用户没有指定`Surface`，`Turbo`则会在内部创建一个虚的`Surface`，并使用虚`Surface`创建虚`Swapchain`，最终创建`ColorImage`用于存储最终的渲染结果（`RenderTarget`）  
>   * 随即带来个问题：创建多大的`Surface`呢？

>2. 对于**用户指定了`Surface`**  
>如果用户指定了`Surface`，`Turbo`则会使用该`Surface`创建`Swapchain`，最终创建`ColorImage`用于存储最终的渲染结果（`RenderTarget`）

>3. 对于**用户更改了(重新指定了)`Surface`**  
>如果用户之前已经指定了`Surface`，并再次指定`Surface`，如果当前`Surface`和指定的`Surface`不相同需要等待之前多有相关工作结束，并重新构建相关资源。

### 方案二（采纳）√

`Turbo`核心将使用离屏渲染，将渲染结果写入`RenderTarget`，如果用户绑定了`Surface`则将`RenderTarget`的渲染结果拷贝到`Surface`所对应的`Swapchain`所对应的`Image`中。

使用`CommandBuffer::CmdBlitImage(...)`可以很好的支持该工作

>* 现在有个问题：如果采用方案二，离线渲染的图片（`RenderTarget`）大小是多少呢？
>* 解决方案：需要用户自定义创建`Surface`（此`Surface`可以使虚的也可以是实的），并将创建好的`Surface`绑定给`Context`，之后`Context`根据`Surface`进行操作。
> 如此会有两种情况：
>   1. 用户没有指定`Surface`  
>       如果用户没有绑定任何`Surface`，`Context`将不会做任何事情，应为没有目标输出
>   2. 用户指定了`Surface`
>       ```mermaid
>        graph TD;
>            IsSurfaceSame{{当前Surface与用户指定的Surface是否相同}}--相同--->DoNothing[什么也不做];
>            IsSurfaceSame--不相同-->WaitAll[等待之前所有工作结束并回收资源];
>            WaitAll-->Refresh[使用用户指定的Surface进行新的构建];
>            DoNothing-->Frame[继续下一帧工作]
>            Refresh-->Frame[继续下一帧工作]
>        ```
---
`mermaid`图测试
```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```