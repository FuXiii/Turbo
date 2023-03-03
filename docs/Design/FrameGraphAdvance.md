# Turbo引擎使用FrameGraph驱动设计

最终会映射到`./docs/TurboDesign:Render`设计中  
*注：该文档有些图表是使用[mermaid](https://mermaid-js.github.io/mermaid/#/)进行书写的，`github`并不支持复杂的`mermaid`语法，导致在线查看该文档会渲染不出图表，本人是使用`VSCode`安装`Markdown Preview Mermaid Support`插件进行查看和书写的。*

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
  >* 创建`Surface、Swapchain和Context`章节
  >* 创建`Surface`章节
  >* 创建`Swapchain`章节

* 2022/12/9
  >
  >* 创建`离屏渲染流程`章节
  >* 创建`将RenderTarget结果拷贝给用户`章节
  >* 创建`用户自定义PassNode`章节
  >* `Image`资源派生中增加`CubeImage`资源类
  >* `资源的创建与销毁`章节中增加在使用`Render::TContext`创建`Redner::TImage`时与`Turbo::Core::TImage`资源类的对应说明
  >

* 2022/12/10
  >
  >* 将原先的`TImage`资源说明，创建成`资源`章节
  >* 更新`资源`资源章节中的`Descriptor`与`资源的创建与销毁`章节对应上
  >* 创建`Format格式`章节，用于规定创建`Image`和`Buffer`时的资源格式
  >* 创建`Turbo驱动初步`章节，用于将琐碎设计和之后的`Turbo驱动设计`做区分
  >

* 2022/12/11
  >
  >* 创建`资源的所有者端域`章节
  >* 更新`资源`章节，增加`端域`
  >

* 2022/12/12
  >
  >* 创建`Usage和Domain`章节

* 2022/12/13
  >
  >* 更新`Usage和Domain`章节
  >* 创建`资源拷贝传输`章节

* 2022/12/14
  >
  >* 更新`Usage和Domain`章节
  >* 更新`资源拷贝传输`章节

* 2022/12/15
  >
  >* 更新`Usage和Domain`章节。回读内存位标应为`HOST_ACCESS_RANDOM`，而不是`HOST_ACCESS_SEQUENTIAL_WRITE`
  >* 创建`Image的Format`章节

* 2022/12/16
  >
  >* 更新`Usage和Domain`章节。高频内存位标`条件`应为`TRANSFER_DST`，而不是`TRANSFER_SRC`
  >* 更新`资源`章节。增加`Vulkan`标准对于`3D纹理`资源的`layer`限值，由`Turbo`引擎维护
  >* 创建`Image的Format`章节
  >* 更新`Image的Format`章节
  
* 2022/12/17
  >
  >* 更新`Image的Format`章节

* 2022/12/18
  >
  >* 更新`Image的Format`章节
  >* 更新`资源`章节，添加`Depth`图片说明

* 2022/12/19
  >
  >* 更新`资源`章节，添加更新`Texture`和`3DImage`说明

* 2022/12/24
  >
  >* 更新`资源拷贝传输`章节

* 2022/12/25
  >
  >* 更新`资源拷贝传输`章节

* 2022/12/26
  >
  >* 更新`Context上下文`章节

* 2022/12/27
  >
  >* 更新`Context上下文`章节
  >* 更新`用户自定义PassNode`章节
  >* 创建`Mesh，Material和Drawable`章节

* 2022/12/28
  >
  >* 更新`用户自定义PassNode`章节
  >* 创建`Pipeline`章节
  >* 创建`RenderPass`章节
  >

* 2022/12/29
  >
  >* 更新`Pipeline`章节
  >* 创建`Pipeline的VertexBinding`章节
  >

* 2022/12/30
  >
  >* 更新`RenderPass`章节

* 2023/1/3
  >
  >* 更新`用户自定义PassNode`章节

* 2023/1/5
  >
  >* 将`RenderPass`章节更名为`Context::CmdBeginRenderPass`
  >* 创建`RenderPass`章节
  >* 创建`Subpass`章节

* 2023/1/6
  >
  >* 创建`Shader`章节

* 2023/1/9
  >
  >* 创建`指令推送`章节
  >* 创建`指令等待`章节

* 2023/1/10
  >
  >* 更新`指令等待`章节

* 2023/1/11
  >
  >* 创建`异步资源回收`章节

* 2023/1/18
  >
  >* 更新`Context::CmdBeginRenderPass`章节
  >* 更新`资源的创建与销毁`章节
  >* 更新`异步资源回收`章节

* 2023/1/22
  >
  >* `Context::CmdBeginRenderPass`章节下创建`RenderPass 创建`章节
  >* `Context::CmdBeginRenderPass`章节下创建`FrameBuffer 创建`章节
  >* `Context::CmdBeginRenderPass`章节下`FrameBuffer 创建`章节下创建`RenderPassPool`章节

* 2023/1/23
  >
  >* `Context::CmdBeginRenderPass`章节下增加`RenderPassProxy(RenderPass代理)`章节

* 2023/1/26
  >
  >* 创建`Turbo::Render::TRenderPass 转 Turbo::Core::TRenderPass`章节

* 2023/2/6
  >
  >* 创建`Image的Layout`章节

* 2023/2/9
  >
  >* 更新`FrameBuffer 创建`章节
  >* 更新`RenderPassProxy(RenderPass代理)`章节

* 2023/2/10
  >
  >* 更新`FrameBuffer 创建`章节
  >* 创建`Image的ImageView`章节

* 2023/2/13
  >
  >* 更新`FrameBuffer 创建`章节

* 2023/2/14
  >
  >* 更新`Context::CmdBeginRenderPass`章节

* 2023/2/16
  >
  >* 更新`Image的Layout`章节

* 2023/2/20
  >
  >* 更新`Pipeline的VertexBinding`章节

* 2023/2/21
  >
  >* 更新`Pipeline的VertexBinding`章节

* 2023/2/22
  >
  >* 更新`Pipeline的VertexBinding`章节
  >* 更新`资源`章节
  >* 创建`Image`章节
  >* 创建`Buffer`章节

* 2023/2/24
  >
  >* 更新`Format格式`章节
  >* 更新`Pipeline的VertexBinding`章节

* 2023/2/26
  >
  >* 创建`绑定Pipeline`章节
  >* 创建`绑定Pipeline的DescriptorSet`章节

* 2023/2/27
  >
  >* 更新`绑定Pipeline的DescriptorSet`章节
  >* 更新`绑定Pipeline`章节
  >* 创建`Pipeline创建`章节

* 2023/2/28
  >
  >* 创建`DrawCall`章节
  >* 创建`Dispatch`章节
  >* 更新`Buffer`章节中的`TIndexBuffer`

* 2023/3/1
  >
  >* `Buffer`章节中增加`TUniformBuffer`

---

# Turbo驱动初步

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

## 资源

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
>
>`ColorImage2D`派生有：
>
>* CubeImage

>`Buffer`派生有：
>
>* VertexBuffer
>* IndexBuffer

### Image

**注：按照`Vulkan`标准：[If imageType is VK_IMAGE_TYPE_3D, arrayLayers must be 1](https://registry.khronos.org/vulkan/specs/1.3/html/chap12.html#VUID-VkImageCreateInfo-imageType-00961)（如果创建三维纹理资源，layer必须是1）**

```CXX
typedef uint32_t TFlags;

enum TImageType
{
    1D,
    2D,
    3D
};

enum TFormat//这个枚举放到TFormat.h中作为通用枚举（Buffer也要用）
{...};

enum TUsageFlagsBits
{
    TRANSFER，//考虑是否由Turbo管理该TUsageFlagsBits::TRANSFER用例
    SAMPLED，
    STORAGE，
    COLOR_ATTACHMENT，
    DEPTH_STENCIL，
    INPUT_ATTACHMENT，
};
typedef TFlags TUsages;

enum TImageCreateFlagBits
{
    CUBE//用于天空盒
};
using TImageCreateFlags = uint32_t;

class Image{
    struct Image::Descriptor
    {
        TImageCreateFlags flags;//CUBE用于六面体纹理，多用于天空盒（详见CubeImage）
        TFormat format;
        uint32_t width;//1D轴，当（width≠0,height=0,depth=0）时，对应Turbo::Core::TImageType::1D
        uint32_t height;//2D轴，当（width≠0,height≠0,depth=0）时，对应Turbo::Core::TImageType::2D
        uint32_t depth;//3D轴，当（width≠0,height≠0,depth≠0）时，对应Turbo::Core::TImageType::3D
        uint32_t mipLevels;
        uint32_t layers;
        TImageUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };

void Create(const std::string &name, const Image::Descriptor &descriptor,void* allocator);
void Destroy();
};

class ColorImage: public Image
{
    struct ColorImage::Descriptor
    {
        TImageCreateFlags flags;
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t mipLevels;
        uint32_t layers;
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class ColorImage1D:public ColorImage
{
    struct ColorImage1D::Descriptor
    {
        //TImageCreateFlags flags; //flags值为0，Cube纹理需要六个二维纹理，一维不满足该条件
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;//width不能为0
        //uint32_t height;//该属性由Turbo维护，值为1
        //uint32_t depth; //该属性由Turbo维护，值为1
        uint32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1，TODO:考虑是否由Turbo维护
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class ColorImage2D: public ColorImage
{
    struct ColorImage2D::Descriptor
    {
        TImageCreateFlags flags; //TODO:考虑是否由Turbo维护，由于有了CubeImage类，这个flags对于ColorImage2D有点多余
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        //uint32_t depth; //该属性由Turbo维护，值为1
        uint32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1，TODO:考虑是否由Turbo维护
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class ColorImage3D: public ColorImage
{
    struct ColorImage3D::Descriptor
    {
        //TImageCreateFlags flags;//flags值为0，Cube纹理需要六个二维纹理，三维不满足该条件
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        uint32_t depth;//depth不能为0
        uint32_t mipLevels; //默认值为1
        //uint32_t layers; //默认值为1，由Turbo维护(根据Vulkan标准：If imageType is VK_IMAGE_TYPE_3D, arrayLayers must be 1)
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class CubeImage: public ColorImage2D
{
    struct CubeImage::Descriptor
    {
        //TImageCreateFlags flags;//该属性由Turbo维护，为CUBE
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;
        uint32_t height;
        //uint32_t depth; //该属性由Turbo维护，值为1
        uint32_t mipLevels; //默认值为1
        //uint32_t layers; //该属性由Turbo维护，默认值为6
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class DepthStencilImage: public Image
{
    struct DepthStencilImage::Descriptor
    {
        //TImageCreateFlags flags; //由Turbo维护，默认值为0
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持深度的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        uint32_t depth; //该属性由Turbo维护，值为1
        uint32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1，TODO:考虑是否由Turbo维护
        TUsages usages;//内部会自动附上TImageUsageBits::DEPTH_STENCIL_ATTACHMENT
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class DepthImage:public DepthStencilImage
{
    struct DepthImage::Descriptor
    {
        //TImageCreateFlags flags; //由Turbo维护，默认值为0
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持深度的格式)
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class DepthImage2D:public DepthImage
{
    struct DepthImage2D::Descriptor
    {
        //TImageCreateFlags flags; //由Turbo维护，默认值为0
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持深度的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        //uint32_t depth; //该属性由Turbo维护，值为1
        uint32_t mipLevels; //默认值为1
        uint32_t layers; //默认值为1
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
}
```

由于在开发中，二维/三维的单张颜色纹理和二维的单张深度纹理最为常用，所以`Turbo`应该提供如下`Texture`类

```CXX
class Texture2D: public ColorImage2D
{
    struct Texture2D::Descriptor
    {
        //TImageCreateFlags flags;//该属性由Turbo维护, flags值为0
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        //uint32_t depth;//该属性由Turbo维护，默认值为1
        uint32_t mipLevels;
        //uint32_t layers;//该属性由Turbo维护，默认值为1
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class Texture3D: public ColorImage3D
{
    struct Texture3D::Descriptor
    {
        //TImageCreateFlags flags;//该属性由Turbo维护, flags值为0
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持颜色的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        uint32_t depth;//depth不能为0
        uint32_t mipLevels;
        //uint32_t layers;//该属性由Turbo维护，默认值为1
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};

class Cubemap: public CubeImage
{
    struct Cubemap::Descriptor
    {
       ...
    };
};

class DepthTexture2D: public DepthImage2D
{
    struct DepthImage2D::Descriptor
    {
        //TImageCreateFlags flags; //由Turbo维护，默认值为0
        //TFormat format; //该属性由Turbo维护(Turbo会设置支持深度的格式)
        uint32_t width;//width不能为0
        uint32_t height;//height不能为0
        //uint32_t depth; //该属性由Turbo维护，值为1
        uint32_t mipLevels; //默认值为1
        //uint32_t layers; //由Turbo维护，默认值为1
        TUsages usages;
        TDomain domain;//详见[资源的所有者端域]章节
    };
};
```

### Buffer

与`Image`类似

```CXX
typedef enum TBufferUsageBits
{
    TRANSFER_SRC = 0x00000001,//考虑是否由Turbo管理该TUsageFlagsBits::TRANSFER_SRC用例
    TRANSFER_DST = 0x00000002,//考虑是否由Turbo管理该TUsageFlagsBits::TRANSFER_DST用例
    UNIFORM_TEXEL = 0x00000004,
    STORAGE_TEXEL = 0x00000008,
    UNIFORM = 0x00000010,
    STORAGE = 0x00000020,
    INDEX = 0x00000040,
    VERTEX = 0x00000080,
    INDIRECT = 0x00000100,
} TBufferUsageBits;
using TBufferUsages = uint32_t;

class TBuffer
{
  public:
    struct Descriptor
    {
        TBufferUsages usages;
        uint64_t size;
        TDomain domain;
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);
}
```

计划派生出如下子类：

#### VertexBuffer

```CXX
enum TVertexRate
{
    VERTEX,
    INSTANCE
};

using AttributeID=uint32_t;

class TVertexBuffer: public TBuffer
{
private:
    uint32_t stride;
    TVertexRate rate;
    std::vector<Attribute> attributes;

public:
    struct Descriptor
    {
        //TBufferUsages usages;//由Turbo管理，将会默认包括TBufferUsageBits::VERTEX
        uint64_t size;
        TDomain domain;

        uint32_t stride;
        TVertexRate rate = TVertexRate::VERTEX;//默认值为TVertexRate::VERTEX
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);

    AttributeID AddAttribute( TFormatType formatType, uint32_t offset);
};
```
#### IndexBuffer

```CXX
class TIndexBuffer: public TBuffer
{
public:
    enum class IndexType
    {
        UINT16,
        UINT32
    };

public:
    struct Descriptor
    {
        //TBufferUsages usages;//由Turbo管理，将会默认包括TBufferUsageBits::INDEX
        uint64_t size;
        TDomain domain;
        IndexType indexType;
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);
};
```

#### UniformBuffer

`UniformBuffer`常见于存储一个`Struct`中的数据

```CXX
class TUniformBuffer: public TBuffer
{
public:
    struct Descriptor
    {
        //TBufferUsages usages;//由Turbo管理，将会默认包括TBufferUsageBits::UNIFORM
        uint64_t size;
        TDomain domain;
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};
```

或者说提供一个模板类型的`UniformBuffer`（这样的弊端就是一个`UniformBuffer`只能对应一种`struct`，好处就是方便直接）:

```CXX
template<class T>
class TUniformBuffer: public TBuffer
{
    public:
    struct Descriptor
    {
        //TBufferUsages usages;//由Turbo管理，将会默认包括TBufferUsageBits::UNIFORM
        //uint64_t size;//由模板 sizeof(T)推出
        TDomain domain;
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
}
```

## Format格式

```CXX
namespace Turbo::Render
typedef enum class TFormat
{
    R8G8B8A8_SRGB,
    B8G8R8A8_SRGB,
    R8G8B8_SRGB,
    B8G8R8_SRGB,
    R8G8B8A8_UNORM,
    B8G8R8A8_UNORM,
    R8G8B8_UNORM,
    B8G8R8_UNORM,
    D32_SFLOAT,
    D16_UNORM,
    R32_SFLOAT,
    R32G32_SFLOAT,
    R32G32B32_SFLOAT,
    R32G32B32A32_SFLOAT,
}TFormat;
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

>* 对于`TContext::CreateImage(...)`与`Turbo::Core::TImage`参数对应
>
> ```CXX
>struct Turbo::Render::Image::Descriptor
>{
>    Turbo::Render::TFlag flag;//用于CubeImage
>    Turbo::Render::TFormat format;
>    uint32_t width;//1D轴，当（width≠0,height=0,depth=0）时，对应Turbo::Core::TImageType::1D
>    uint32_t height;//2D轴，当（width≠0,height≠0,depth=0）时，对应Turbo::Core::TImageType::2D
>    uint32_t depth;//3D轴，当（width≠0,height≠0,depth≠0）时，对应Turbo::Core::TImageType::3D
>    uitn32_t mipLevels;
>    uint32_t layers;
>    TUsages usages;
>};
>
>Turbo::Render::TContext::CreateImage(const Turbo::Render::Image::Descriptor& descriptor);
>Turbo::Core::TImage::TImage(
>       TDevice *device, //由TContext指定
>       VkImageCreateFlags imageFlags, //由Render层传入，一般Render::TCubeImage创建指定
>       TImageType type, //由Render层转换推出（根据Turbo::Render::Image::Descriptor的长宽高转换推出）
>       TFormatInfo format, //由Render层传入
>       uint32_t width, //由Render层传入
>       uint32_t height, //由Render层传入
>       uint32_t depth, //由Render层传入
>       uint32_t mipLevels, //由Render层传入
>       uint32_t arrayLayers, //由Render层传入
>       TSampleCountBits samples, //由Turbo维护
>       TImageTiling tiling, //由Turbo维护
>       TImageUsages usages, //由Render层传入
>       TMemoryFlags memoryFlags, //由Turbo维护
>       TImageLayout layout//由Turbo维护，默认值TImageLayout::UNDEFINED
>)
>```

目前`TResourceAllocator`分配和销毁`Image`和`Buffer`是直接分配，直接销毁的（这是短期目标），目前每一帧中用到就分配，结束就销毁，但是这并不是一种良构。正常来说分配的`Image`和`Buffer`最终应该在`Context`或者`ResourceAllocator`中存储并使用`Cache`或者`Pool`之类技术存储，这样也能与`异步资源回收`(见`异步资源回收`章节)互相配合。

*注：`异步资源回收`主要是针对`FrameGraph`中的资源进行回收，用户自定义的资源，比如采样纹理之类的还是要自己管理*

### 资源的所有者端域

所谓的资源所有者端位，其实是指资源是在`CPU`端创建，还是在`GPU`端创建。对于`CPU`和`GPU`的资源之间可以互相拷贝传输，所以需要在创建资源时设置资源的端域

有多种情况：

>1. `CPU`端到`GPU`端
>
>```mermaid
>graph TD;
>    CreateCPUDomainResource["创建CPU端资源"]
>    CreateGPUDomainResource["创建GPU端资源"]
>    SetDataIntoCPUDomainResource[将数据设置到CPU端资源中]
>    CopyCPUDomainResourceIntoGPUDomainResource[将CPU端资源数据拷贝到GPU端资源中]
>    ReleaseCPUDomainResource[释放CPU端资源]
>    
>    CreateCPUDomainResource-->CreateGPUDomainResource
>    CreateGPUDomainResource-->SetDataIntoCPUDomainResource
>    SetDataIntoCPUDomainResource-->CopyCPUDomainResourceIntoGPUDomainResource
>    CopyCPUDomainResourceIntoGPUDomainResource-->ReleaseCPUDomainResource
>```
>
>---
>
>2. `GPU`端到`CPU`端
>
>```mermaid
>graph TD;
>    CreateCPUDomainResource["创建CPU端资源"]
>    CreateGPUDomainResource["创建GPU端资源"]
>    SetDataIntoGPUDomainResource["通过运行CommandBuffer(Draw/Dispatch等)，将数据设置到GPU端资源中"]
>    CopyGPUDomainResourceIntoCPUDomainResource[将GPU端资源数据拷贝到CPU端资源中]
>    ReleaseGPUDomainResource[释放GPU端资源]
>    
>    CreateCPUDomainResource-->CreateGPUDomainResource
>    CreateGPUDomainResource-->SetDataIntoGPUDomainResource
>    SetDataIntoGPUDomainResource-->CopyGPUDomainResourceIntoCPUDomainResource
>    CopyGPUDomainResourceIntoCPUDomainResource-->ReleaseGPUDomainResource
>```
>
>---
>
>3. `CPU`端到`CPU`端
>
>```mermaid
>graph TD;
>    CreateCPUDomainResourceCPUa["创建CPU端资源CPUa"]
>    CreateCPUDomainResourceCPUb["创建CPU端资源CPUb"]
>    Copy["使用CommandBuffer或者memcpy在CPUa和CPUb之间拷贝数据"]
>    CreateCPUDomainResourceCPUa-->CreateCPUDomainResourceCPUb
>    CreateCPUDomainResourceCPUb-->Copy
>```
>
>---
>
>4. `GPU`端到`GPU`端
>
>```mermaid
>graph TD;
>    CreateGPUDomainResourceGPUa["创建GPU端资源GPUa"]
>    CreateGPUDomainResourceGPUb["创建GPU端资源GPUb"]
>    Copy["使用CommandBuffer在GPUa和GPUb之间拷贝数据"]
>    CreateGPUDomainResourceGPUa-->CreateGPUDomainResourceGPUb
>    CreateGPUDomainResourceGPUb-->Copy
>```
>
>5. `CPU`端与`GPU`端兼容  
>有时可以创建`CPU`端和`GPU`端共享的资源
>
```CXX
namespace Turbo::Render
typedef enum TDomainBits
{
    CPU=0x00000001,
    GPU=0x00000002
}TDomainBits;
using TDomain = uint32_t;
```

## Usage和Domain

>**考虑是否向用户开放`TRANSFER_SRC`和`TRANSFER_DST`？如果`Turbo`负责维护该属性应该会更符合设计思想，用户只需要考虑使用Domain[CPU,GPU]即可，会比较简单**

不同的`Usage`和`Domain`会影响`Turbo`底层对于具体资源的创建策略。  
常见的资源创建策略有一下几种：
>
>1. GPU独占资源（GPU）  
>表示该资源只有`GPU`可以访问。
>
>```mermaid
>graph TD;
>    direction TB
>    subgraph GPU
>        subgraph GPUImage["Image"]
>        direction TB
>           subgraph GPUImageDescriptor[Descriptor]
>                GPUImageDescriptorArgs["Usages:不限\nDomain:GPU"]
>           end
>           subgraph GPUImageCreateTImage["创建Core::TImage"]
>                GPUImageCreateTImageArgs["Tiling:OPTIMAL\nMemoryFlags:DEDICATED_MEMORY"]
>           end
>           GPUImageDescriptor--对应底层-->GPUImageCreateTImage
>        end
>        subgraph GPUBuffer["Buffer"]
>        direction TB
>           subgraph GPUBufferDescriptor[Descriptor]
>                GPUBufferDescriptorArgs["Usages:不限\nDomain:GPU"]
>           end
>           subgraph GPUBufferCreateTImage["创建Core::TBuffer"]
>                GPUBufferCreateTImageArgs["MemoryFlags:DEDICATED_MEMORY"]
>           end
>           GPUBufferDescriptor--对应底层-->GPUBufferCreateTImage
>        end
>    end
>```
>
>满足以下条件即为`GPU独占资源`：
>
> * `Domain`只有`GPU`位标  
>>
>>* 对应`Turbo::Core`底层资源内存分配为：`Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY`
>>* `Turbo::Core::Image`对应的构造参数：
>>   * Turbo::Core::TImageTiling::OPTIMAL
>>* `Turbo::Core::TBuffer`对应的构造参数：
>>   * 略

>2. 用于传输拷贝的暂存副本（CPU→GPU）  
>*注：暂存副本多为`Buffer`资源*  
>由于`GPU独占资源`只能使用`GPU`进行访问，有时需要将`CPU`端的数据赋值给`GPU`端，所以需要使用一个`CPU`端可写入并且可以拷贝到`GPU`端的资源，此种资源叫做`暂存副本`（`Staging`）。  
>
>```mermaid
>graph TD;
>    direction TB
>    subgraph CPU[CPU端资源]
>        subgraph CPUImage["Image"]
>        direction TB
>           subgraph CPUImageDescriptor[Descriptor]
>                CPUImageDescriptorArgs["Usages:TRANSFER_SRC+除了TRANSFER_DST所有\nDomain:CPU"]
>           end
>           subgraph CPUImageCreateTImage["创建Core::TImage"]
>                CPUImageCreateTImageArgs["Tiling:LINEAR（注意：Vulkan标准限值）\nMemoryFlags:HOST_ACCESS_SEQUENTIAL_WRITE"]
>           end
>           CPUImageDescriptor--对应底层-->CPUImageCreateTImage
>        end
>        subgraph CPUBuffer["Buffer"]
>        direction TB
>           subgraph CPUBufferDescriptor[Descriptor]
>                CPUBufferDescriptorArgs["Usages:TRANSFER_SRC+除了TRANSFER_DST所有\nDomain:CPU"]
>           end
>           subgraph CPUBufferCreateTImage["创建Core::TBuffer"]
>                CPUBufferCreateTImageArgs["MemoryFlags:HOST_ACCESS_SEQUENTIAL_WRITE"]
>           end
>           CPUBufferDescriptor--对应底层-->CPUBufferCreateTImage
>        end
>    end
>
>    CPUImage--"使用Map/Copy将数据赋值给CPU端Image"-->UseMapCopyDataIntoImage[刷新CPU端Image数据]
>    CPUBuffer--"使用Map/Copy将数据赋值给CPU端Image"-->UseMapCopyDataIntoBuffer[刷新CPU端Buffer数据]
>
>    subgraph CreateGPUOnlyResource["GPU独占资源"]
>        GPUOnlyImage["Image"]
>        GPUOnlyBuffer["Buffer"]
>    end
>
>    UseMapCopyDataIntoImage--"使用CopyCommand将CPU端数据拷贝至GPU端"-->GPUOnlyImage
>    UseMapCopyDataIntoImage--"使用CopyCommand将CPU端数据拷贝至GPU端"-->GPUOnlyBuffer
>
>    UseMapCopyDataIntoBuffer--"使用CopyCommand将CPU端数据拷贝至GPU端"-->GPUOnlyImage
>    UseMapCopyDataIntoBuffer--"使用CopyCommand将CPU端数据拷贝至GPU端"-->GPUOnlyBuffer
>```

>满足以下条件即为`暂存副本`：
>
> * `Usage`包含`TRANSFER_SRC`位标，不包括`TRANSFER_DST`位标，并且`Domain`只包含`CPU`位标
>
>>对应`Turbo::Core`底层资源内存分配为：`Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE`
>>
>>* `Turbo::Core::Image`对应的构造参数：
>>   * (一般都是创建暂存`Buffer`，之后`Command`拷贝到`OPTIMAL`和`DEDICATED_MEMORY`的`Image`中)
>>   * Turbo::Core::TImageTiling::~~OPTIMAL~~（目前有问题[VMA:OPTIMAL with HOST_ACCESS_SEQUENTIAL_WRITE](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/issues/305)）
>>     * 作为传输的数据源`usage`应该为`TRANSFER_SRC`
>>* `Turbo::Core::TBuffer`对应的构造参数：
>>   * ...

>3. 回读（CPU←GPU）  
>`回读`是将`GPU独占资源`回读拷贝至`CPU`端，简单来说是`暂存副本`（CPU→GPU）的逆向。  
>
>```mermaid
>graph TD;
>    direction TB
>    subgraph CPU[CPU端资源]
>        subgraph CPUImage["Image"]
>        direction TB
>           subgraph CPUImageDescriptor[Descriptor]
>                CPUImageDescriptorArgs["Usages:TRANSFER_DST+除了TRANSFER_SRC所有\nDomain:CPU"]
>           end
>           subgraph CPUImageCreateTImage["创建Core::TImage"]
>                CPUImageCreateTImageArgs["Tiling:LINEAR（注意：Vulkan标准限值）\nMemoryFlags:HOST_ACCESS_RANDOM"]
>           end
>           CPUImageDescriptor--对应底层-->CPUImageCreateTImage
>        end
>        subgraph CPUBuffer["Buffer"]
>        direction TB
>           subgraph CPUBufferDescriptor[Descriptor]
>                CPUBufferDescriptorArgs["Usages:TRANSFER_DST+除了TRANSFER_SRC所有\nDomain:CPU"]
>           end
>           subgraph CPUBufferCreateTImage["创建Core::TBuffer"]
>                CPUBufferCreateTImageArgs["MemoryFlags:HOST_ACCESS_RANDOM"]
>           end
>           CPUBufferDescriptor--对应底层-->CPUBufferCreateTImage
>        end
>    end
>
>    CPUImage--"使用Map获取数据指针"-->UseMapCopyDataIntoImage[CPU端获取数据]
>    CPUBuffer--"使用Map获取数据指针"-->UseMapCopyDataIntoBuffer[CPU端获取数据]
>
>    subgraph CreateGPUOnlyResource["GPU独占资源"]
>        GPUOnlyImage["Image"]
>        GPUOnlyBuffer["Buffer"]
>    end
>
>    GPUOnlyImage--"使用CopyCommand将GPU端数据拷贝至CPU端"-->CPUImage
>    GPUOnlyBuffer--"使用CopyCommand将GPU端数据拷贝至CPU端"-->CPUImage
>
>    GPUOnlyImage--"使用CopyCommand将GPU端数据拷贝至CPU端"-->CPUBuffer
>    GPUOnlyBuffer--"使用CopyCommand将GPU端数据拷贝至CPU端"-->CPUBuffer
>```
>
>满足以下条件即为`回读`：
>
> * `Usage`包含`TRANSFER_DST`位标，不包含`TRANSFER_SRC`位标，并且`Domain`只包含`CPU`位标
>
>>对应`Turbo::Core`底层资源内存分配为：`Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_RANDOM`
>>
>>* `Turbo::Core::Image`对应的构造参数：
>>   * 一种是是创建`CPU`端的`Buffer`，之后`Command`将`Image`拷贝到`Buffer`中，之后读`Buffer`。
>>   * 另一种是创建`CPU`端的`Image`，之后将`GPU`端的`Image`拷贝到`CPU`端`Image`中
>>   * Turbo::Core::TImageTiling::LINEAR（如果满足以下条件）  
in [Vulkan 标准 : VkImageCreateInfo](https://registry.khronos.org/vulkan/specs/1.3/html/chap12.html#VkImageCreateInfo)：  
Images created with `tiling` equal to `VK_IMAGE_TILING_LINEAR` have further restrictions on their limits and capabilities compared to images created with `tiling` equal to `VK_IMAGE_TILING_OPTIMAL`. Creation of images with tiling `VK_IMAGE_TILING_LINEAR` may not be supported unless other parameters meet all of the constraints:
>>     * `imageType` is `VK_IMAGE_TYPE_2D`
>>     * `format` is not a depth/stencil format
>>     * `mipLevels` is 1
>>     * `arrayLayers` is 1
>>     * `samples` is `VK_SAMPLE_COUNT_1_BIT`
>>     * `usage` only includes `VK_IMAGE_USAGE_TRANSFER_SRC_BIT` and/or `VK_IMAGE_USAGE_TRANSFER_DST_BIT`  
>>
>>   * Turbo::Core::TImageTiling::OPTIMAL(如果使用`Khronos KTX`标准进行纹理回读的话，满足此种情况)
>>* `Turbo::Core::TBuffer`对应的构造参数：
>>   * ...

**综上**:~~`暂存副本`和`回读`两者的底层`Core::TImage`创建相同~~(不同一个是`TRANSFER_SRC`一个是`TRANSFER_DST`)，可以按照`Domain::CPU`进行创建区分

>4. 高频传输(CPU⇄GPU)(多为CPU与GPU端共享资源)  
>`高频传输`一般用于`CPU`频繁的更改资源数据，`GPU`之后频繁的读此数据（多见于`uniform buffer`）  
>
>* 注：高频传输多为`Buffer`资源*  
>* 大体上和`暂存副本`区别不大，`Domain`为`CPU+GPU`，并且在分配`MemoryFlags`时多了一个`HOST_ACCESS_ALLOW_TRANSFER_INSTEAD`位标，并在`Copy`时，由`Turbo`负责正确拷贝
>
>```mermaid
>graph TD;
>    direction TB
>    subgraph CPUAndGPU[CPU与GPU端共享资源]
>        subgraph CPUImage["Image"]
>        direction TB
>           subgraph CPUImageDescriptor[Descriptor]
>                CPUImageDescriptorArgs["Usages:TRANSFER_DST\nDomain:CPU+GPU"]
>           end
>           subgraph CPUImageCreateTImage["创建Core::TImage"]
>                CPUImageCreateTImageArgs["Tiling:LINEAR（注意：Vulkan标准限值）\nMemoryFlags:HOST_ACCESS_SEQUENTIAL_WRITE+HOST_ACCESS_ALLOW_TRANSFER_INSTEAD"]
>           end
>           CPUImageDescriptor--对应底层-->CPUImageCreateTImage
>        end
>        subgraph CPUBuffer["Buffer"]
>        direction TB
>           subgraph CPUBufferDescriptor[Descriptor]
>                CPUBufferDescriptorArgs["Usages:TRANSFER_DST\nDomain:CPU+GPU"]
>           end
>           subgraph CPUBufferCreateTImage["创建Core::TBuffer"]
>                CPUBufferCreateTImageArgs["MemoryFlags:HOST_ACCESS_SEQUENTIAL_WRITE+HOST_ACCESS_ALLOW_TRANSFER_INSTEAD"]
>           end
>           CPUBufferDescriptor--对应底层-->CPUBufferCreateTImage
>        end
>    end
>    subgraph TurboCopy["Copy()"]
>    direction TB
>       IsSupportHOST_VISIBLE{{是否支持HOST_VISIBLE}}
>       Memcpy[memcpy]
>       UseStagingCase[使用暂存副本流程]
>       IsSupportHOST_VISIBLE--支持-->Memcpy
>       IsSupportHOST_VISIBLE--不支持-->UseStagingCase
>    end
>
>    CPUImage--"使用Copy将数据赋值给CPU端Image(由Turbo负责正确拷贝)"-->IsSupportHOST_VISIBLE
>    CPUBuffer--"使用Copy将数据赋值给CPU端Image(由Turbo负责正确拷贝)"-->IsSupportHOST_VISIBLE
>    Memcpy-->FinishCopy[赋值结束]
>    Memcpy-->FinishCopy
>    UseStagingCase-->FinishCopy
>    UseStagingCase-->FinishCopy
>```
>
>满足以下条件即为`高频传输`：
>
> * `Usage`包含`TRANSFER_DST`位标，并且`Domain`包含`CPU`和`GPU`位标
>
>>对应`Turbo::Core`底层资源内存分配为：`Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE`和`Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_ALLOW_TRANSFER_INSTEAD`
>>
>>此种情况在`Copy`时需要`Turbo`底层查看分配的内存是否支持`VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT`如果支持直接`memcpy`，如果不支持需要调用`暂存副本`流程（详情参考[VulkanMemoryAllocator::Advanced data uploading](`https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/usage_patterns.html`)）(这需要提供`Copy`接口函数统一调配)，
>>*如果简化的话可以直击使用`Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE`（有待测试）*
>>
>>* `Turbo::Core::Image`对应的构造参数（同`暂存副本`情况）：
>>   * ...
>>* `Turbo::Core::TBuffer`对应的构造参数：
>>   * ...

## 资源拷贝传输

资源的拷贝传输基本上有两种方式：

1. 创建`CPU`端资源，之后使用`memcpy(...)`直接将数据拷贝至资源中
2. 创建`GPU`端资源，之后使用`Commandbuffer`将数据拷贝至资源中

资源的拷贝传输包括：

1. `CPU`⇄`GPU`  
    1.1 `CPU`→`GPU`  
    * 使用`暂存副本`

    ```CXX
    void* some_data=...;
    Image/Buffer* cpu_resource=new Image/Buffer(TRANSFER_SRC, Domain::CPU);//暂存副本
    cpu_resource->Copy(some_data);//将数据存入暂存副本
    /*
    void Copy(some_data){
        memcpy(src: some_data, dst: this cpu resource);
    }
    */
    delete some_data;//可以删除原始数据了
    Image/Buffer* gpu_resource=new Image/Buffer(TRANSFER_DST, Domain::GPU);//GPU独占资源，内部位标
    gpu_resource->Copy(cpu_resource);//将暂存资源中的数据存入GPU独占资源中
    /*
    void Copy(cpu_resource){
        command_buffer->Copy(src: cpu_resource, dst: this gpu resource);
        device_queue->submit(command_buffer, fence);
        fence->WaitUntil();
    }
    */
    ```

    1.2 `CPU`←`GPU`
    * 使用`回读`

    ```CXX
    Image/Buffer* gpu_resource = ...;//某些已经有数据的GPU独占资源
    Image/Buffer* cpu_resource = new Image/Buffer(TRANSFER_DST, Domain::CPU);//回读副本
    cpu_resource->Copy(gpu_resource);//将GPU独占资源数据存入回读副本
    /*
    void Copy(gpu_resource){
        command_buffer->Copy(src: gpu_resource, dst: this cpu resource);
        device_queue->submit(command_buffer, fence);
        fence->WaitUntil();
    }
    */
    cpu_resource->Open();//开启资源
    void* data = cpu_resource->Data();//获取资源指针
    void* read_some_data = new ...(data[n...n+i]);//读数据
    cpu_resource->Close();//关闭资源
    ```

    1.3 `CPU`↔`GPU`
    * 使用`高频传输`

    ```CXX
    void* some_data =...;
    Image/Buffer* cpu_gpu_share_resource = new Image/Buffer(TRANSFER_DST, Domain::CPU + Domain::GPU);//CPU端和GPU端共享资源
    cpu_gpu_share_resource->Copy(some_data);//将数据存入共享资源
     /*
    void Copy(some_data){
       if(is_support_host_visible)
       {
            memcpy(src:some_data, dst:this cpu resource);
       }
       else
       {
            //TODO: 使用暂存副本流程
       }
    }
    */
    ```

2. `CPU`⇄`CPU`
    * 使用`memcpy(...)`

    ```CXX
    Image/Buffer* cpu_resource_a = ...;//某个已经存有数据的CPU端资源a
    Image/Buffer* cpu_resource_b = new Image/Buffer(TRANSFER_SRC, Domain::CPU);//CPU端资源b
    cpu_resource_b->Copy(cpu_resource_a);//将数据存入共享资源
     /*
    void Copy(cpu_resource_a){
       {
            memcpy(src:cpu_resource_a, dst:this cpu resource);
       }
    }
    */
    ```

3. `GPU`⇄`GPU`
    * 使用`CommandBuffer`

    ```CXX
    void* some_data=...;
    Image/Buffer* gpu_resource_a = ...;//某个已经存有数据的GPU端资源a
    Image/Buffer* gpu_resource_b = new Image/Buffer(TRANSFER_DST, Domain::CPU);//GPU端资源b
    gpu_resource_b->Copy(gpu_resource_a);//将数据存入暂存副本
    /*
    void Copy(gpu_resource_a){
        command_buffer->Copy(src: gpu_resource_a, dst: this gpu resource);
        device_queue->submit(command_buffer, fence);
        fence->WaitUntil();
    }
    */
    ```

资源拷贝传输大致可分为3种情况：

1. 以void*为代表的的数据资源
2. 以Buffer为代表的的数据资源
3. 以Image为代表的的数据资源

所以正常来说`void*`，`Buffer`，`Image`三者之间应该两两互相可拷贝传输

```mermaid
graph LR;
    VoidPtrSrc["void*"]
    BufferSrc["Buffer"]
    ImageSrc["Image"]

    VoidPtrDst["void*"]
    BufferDst["Buffer"]
    ImageDst["Image"]

    VoidPtrSrc<-..->VoidPtrDst
    VoidPtrSrc<---->BufferDst
    VoidPtrSrc<---->ImageDst

    BufferSrc<---->VoidPtrDst
    BufferSrc<---->BufferDst
    BufferSrc<---->ImageDst

    ImageSrc<---->VoidPtrDst
    ImageSrc<---->BufferDst
    ImageSrc<---->ImageDst
```

1. `void*`↔`void*`  
    此种情况属于程序自身内存拷贝，不属于`Turbo`负责的范畴

2. `void*`→`Buffer`

    ```CXX
    void* some_data;
    Buffer buffer;
    buffer.Copy(some_data,size);
    ```

    >`void Buffer::Copy(void* src,uint32_t size)`
    >
    >```mermaid
    >graph TD;
    >    IsCPUVisible{{资源能否在CPU端被访问}}
    >    UseMemcpy[使用memcpy进行数据拷贝]
    >    UseTempResource[使用临时资源进行数据拷贝]
    >
    >
    >    IsCPUVisible--能-->UseMemcpy
    >    IsCPUVisible--不能-->UseTempResource
    >```

3. `void*`→`Image`

    同`void*`→`Buffer`

    >`void TImage::Copy(void* src,uint32_t size)`

4. `Buffer`→`void*`

    有个前提是`Buffer`必须是`CPU`域的，也就是`CPU`可访问资源

    >`void* Buffer::Open()`

    >`void Buffer::Close()`

    ```CXX
    Buffer buffer;
    void* data_ptr = buffer.Open();
    //使用data_ptr将数据拷贝出来
    //...
    buffer.Close();
    ```

5. `Buffer`→`Buffer`

    ```mermaid
    graph TD;
        IsSrcAndDstCPUVisible{{目标Buffer和源Buffer能否都能在CPU端被访问}}
        UseMemcpy[使用memcpy进行数据拷贝]
        UseCommandBuffer[使用CommandBuffer进行数据拷贝]


        IsSrcAndDstCPUVisible--能-->UseMemcpy
        IsSrcAndDstCPUVisible--不能-->UseCommandBuffer
    ```

    >`void Buffer::Copy(const Buffer& buffer)`

6. `Buffer`→`Image`

    使用`CommandBuffer::CmdCopyBufferToImage(...)`

    >`void Image::Copy(const Buffer& buffer)`

7. `Image`→`void*`
    同 `Buffer`→`void*`

    >`void* Image::Open()`

    >`void Image::Close()`

8. `Image`→`Buffer`

    使用`CommandBuffer::CmdCopyImageToBuffer(...)`

    >`void Buffer::Copy(const Image& image)`

9. `Image`→`Image`

    使用`CommandBuffer::CmdCopyImage(...)`或`CommandBuffer::CmdBlitImage(...)`

    >`void Image::Copy(const Image& image)`

## Image的Format

图片(纹理)主要有一下几种数据：

1. 颜色（有时存储的颜色值并不是“颜色”信息，也可以是法线等信息）
2. 深度
3. 模板（用的不多）

由于不同设备对于不同格式的支持程度不大相同，所以在使用某种格式时需要先查看是否支持该格式。

* 对于`颜色`数据，目前有如下格式

```CXX
R8G8B8A8_SRGB
B8G8R8A8_SRGB
R8G8B8_SRGB
B8G8R8_SRGB
R8G8B8A8_UNORM
B8G8R8A8_UNORM
R8G8B8_UNORM
B8G8R8_UNORM
```

分配策略如下

```mermaid
graph TD;
    IsContainColorAttachmentOrInputAttahcmentUsage{{"Usage是否包含COLOR_ATTACHMENT || Usage是否包含INPUT_ATTACHMENT "}}

    IsSupportR8G8B8A8_SRGB{{"是否支持R8G8B8A8_SRGB(检测是否满足Usage标志位)"}}
    UseR8G8B8A8_SRGB[使用R8G8B8A8_SRGB]

    IsSupportB8G8R8A8_SRGB{{"是否支持B8G8R8A8_SRGB(检测是否满足Usage标志位)"}}
    UseB8G8R8A8_SRGB[使用B8G8R8A8_SRGB]

    IsSupportR8G8B8_SRGB{{"是否支持R8G8B8_SRGB(检测是否满足Usage标志位)"}}
    UseR8G8B8_SRGB[使用R8G8B8A8_SRGB]

    IsSupportB8G8R8_SRGB{{"是否支持B8G8R8_SRGB(检测是否满足Usage标志位)"}}
    UseB8G8R8_SRGB[使用B8G8R8_SRGB]

    IsSupportR8G8B8A8_UNORM{{"是否支持R8G8B8A8_UNORM(检测是否满足Usage标志位)"}}
    UseR8G8B8A8_UNORM[使用R8G8B8A8_UNORM]

    IsSupportB8G8R8A8_UNORM{{"是否支持B8G8R8A8_UNORM(检测是否满足Usage标志位)"}}
    UseB8G8R8A8_UNORM[使用B8G8R8A8_UNORM]

    IsSupportR8G8B8_UNORM{{"是否支持R8G8B8_UNORM(检测是否满足Usage标志位)"}}
    UseR8G8B8_UNORM[使用R8G8B8_UNORM]

    IsSupportB8G8R8_UNORM{{"是否支持B8G8R8_UNORM(检测是否满足Usage标志位)"}}
    UseB8G8R8_UNORM[使用B8G8R8_UNORM]

    InitTargetFormat[初始化目标格式为UNDEFINED]
    InitTargetFormat-->IsContainColorAttachmentOrInputAttahcmentUsage

    IsContainColorAttachmentOrInputAttahcmentUsage--是-->IsSupportR8G8B8A8_SRGB
    IsContainColorAttachmentOrInputAttahcmentUsage--否-->IsContainSampledUsage{{"Usage是否包含Sampled || Usage是否包含Storage"}}

    IsSupportR8G8B8A8_SRGB--是-->UseR8G8B8A8_SRGB
    IsSupportR8G8B8A8_SRGB--否-->IsSupportB8G8R8A8_SRGB

    IsSupportB8G8R8A8_SRGB--是-->UseB8G8R8A8_SRGB
    IsSupportB8G8R8A8_SRGB--否-->IsSupportR8G8B8_SRGB

    IsSupportR8G8B8_SRGB--是-->UseR8G8B8_SRGB
    IsSupportR8G8B8_SRGB--否-->IsSupportB8G8R8_SRGB

    IsSupportB8G8R8_SRGB--是-->UseB8G8R8_SRGB
    IsSupportB8G8R8_SRGB--否-->IsSupportR8G8B8A8_UNORM

    IsSupportR8G8B8A8_UNORM--是-->UseR8G8B8A8_UNORM
    IsSupportR8G8B8A8_UNORM--否-->IsSupportB8G8R8A8_UNORM

    IsSupportB8G8R8A8_UNORM--是-->UseB8G8R8A8_UNORM
    IsSupportB8G8R8A8_UNORM--否-->IsSupportR8G8B8_UNORM

    IsSupportR8G8B8_UNORM--是-->UseR8G8B8_UNORM
    IsSupportR8G8B8_UNORM--否-->IsSupportB8G8R8_UNORM

    IsSupportB8G8R8_UNORM--是-->UseB8G8R8_UNORM
    IsSupportB8G8R8_UNORM--否-->error["抛出不支持异常，没找到支持的格式"]

    IsContainSampledUsage--是-->IsSupportR8G8B8A8_UNORM
    IsContainSampledUsage--否-->TargetFormatStillUndefined[此时目标格式仍为UNDEFINED]

    TargetFormatStillUndefined--从头筛选-->IsSupportR8G8B8A8_SRGB
```

* 对于`深度`数据，目前有如下格式

```CXX
D32_SFLOAT,
D16_UNORM
```

分配策略如下

```mermaid
graph TD;
    InitTargetFormat[初始化目标格式为UNDEFINED]

    IsSupportD32_SFLOAT{{"是否支持D32_SFLOAT(检测是否满足Usage标志位)"}}
    UseD32_SFLOAT[使用D32_SFLOAT]

    IsSupportD16_UNORM{{"是否支持D16_UNORM(检测是否满足Usage标志位)"}}
    UseD16_UNORM[使用D16_UNORM]

    error["抛出不支持异常，没找到支持的格式"]


    InitTargetFormat-->IsSupportD32_SFLOAT

    IsSupportD32_SFLOAT--是-->UseD32_SFLOAT
    IsSupportD32_SFLOAT--否-->IsSupportD16_UNORM

    IsSupportD16_UNORM--是-->UseD16_UNORM
    IsSupportD16_UNORM--否-->error
```

* 对于`模板`数据，目前暂时不考虑

## Image的Layout

根据`Vulkan`标准，在创建`Image`时其对应的`initialLayout`，也就是初始化的布局必须是`VK_IMAGE_LAYOUT_UNDEFINED`或者`VK_IMAGE_LAYOUT_PREINITIALIZED`，在此`Turbo`是采用`VK_IMAGE_LAYOUT_UNDEFINED`，但是在之后的使用中（特别是`CommandBuffer`中）需要转换`Image`的`Layout`。在`Vulkan`标准[`12.4 Image Layout`](https://registry.khronos.org/vulkan/specs/1.3/html/chap12.html#resources-image-layouts)章节中，有如下描述：

>After initialization, applications need not use any layout other than the general layout, though this may produce suboptimal performance on some implementations.

对应译文：

>在`Image`初始化之后（创建之后），应用需要使用通用布局（`VK_IMAGE_LAYOUT_GENERAL`）而不是其他布局，尽管使用通用布局在某些平台实现中会导致次优化（性能没有专用布局那么优化）

也就是说按照`Vulkan`标准来说，使用通用布局是必然。根据`Filament`的源码，其内部也确实使用的是`VK_IMAGE_LAYOUT_GENERAL`布局，但在作为`着色器采样纹理`时使用了专用布局`VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL`。

```CXX
//in {filament_dir}/filament/backend/src/vulkan/VulkanUtility.cpp
VkImageLayout getDefaultImageLayout(TextureUsage usage) 
{
    //由于Filement有时需要采样深度纹理获得深度数据（比如SSAO），所以简单使用了VK_IMAGE_LAYOUT_GENERAL布局
    if (any(usage & TextureUsage::DEPTH_ATTACHMENT)) {
        return VK_IMAGE_LAYOUT_GENERAL;
    }

    //由于Filement有时需要将同一个纹理不同miplevel之间互相拷贝（比如Bloom），并且想要避免昂贵的布局转换操作，所以简单使用VK_IMAGE_LAYOUT_GENERAL布局
    if (any(usage & TextureUsage::COLOR_ATTACHMENT)) {
        return VK_IMAGE_LAYOUT_GENERAL;
    }

    // Finally, the layout for an immutable texture is optimal read-only.
    return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}
```

对于`Filament`一些额外说明:

* 对于将`CPU`资源拷贝到`GPU`时会将纹理布局转换到`VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL`布局，之后拷贝操作完再将布局转换回来
* 对于渲染最后的`Present`显示之前，会将图片的当前布局转换成`VK_IMAGE_LAYOUT_PRESENT_SRC_KHR`

*注：在`ImageView`中也有`Layout`，多个不同`ImageView`对于同一个`Image`其`ImageView`的`Layout`可以是多种多样的。这也就是在`Turbo`的设计中，一个`Image`可以对应多个`ImageView`*

如果是`Turbo`管理`Image`的`Layout`的话，最好是在`CommandBuffer`中记录指令时进行纹理布局管理（因为`Vulkan`是以`CommandBuffer`为核心的架构）。比如：

> `只往纹理中写数据`
>
>```mermaid
>graph LR;
>    Pass--写-->ColorTexture(["ColorTexture"])
>    Pass--写-->DepthTexture(["DepthTexture"])
>```
>
>对应的`Layout`可能为：
>
>* `ColorTexture`：`VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL`
>* `DepthTexture`：`VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL`

> `只读纹理`
>
>```mermaid
>graph LR;
>    ColorTexture(["ColorTexture"])--读-->Pass
>    DepthTexture(["DepthTexture"])--读-->Pass
>```
>
>对应的`Layout`可能为：
>
>* `ColorTexture`：`VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL`
>* `DepthTexture`：`VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL`

等等使用情况

*注：纹理的`Layput`有使用指令前的布局转换，有使用中的布局转换，也有使用后的布局转换*

理想的情况：

```CXX
Texture tex;//VK_IMAGE_LAYOUT_UNDEFINED

command_buffer.BeginRenderPass(render_pass);//此时知道RenderPass对应的FrameBuffer要用到的所有纹理，进行指令前的布局转换
command_buffer.BindPipelineDescriptorSet(..., tex);//进行指令中的布局转换
command_buffer.ClearColorImage(..., tex);//进行指令中的布局转换
command_buffer.ClearDepthStencilImage(..., tex);//进行指令中的布局转换
command_buffer.CopyBufferToImage(..., tex);//进行指令中的布局转换
command_buffer.CopyImageToBuffer(..., tex);//进行指令中的布局转换
command_buffer.CopyImage(..., tex);//进行指令中的布局转换
command_buffer.BlitImage(..., tex);//进行指令中的布局转换
...//等等
command_buffer.EndRenderPass();//进行指令后的布局转换
```

这需要每个纹理在记录特定`CommandBuffer`的指令前后进行`layout`转换（此种方法太麻烦，容易出问题）：

* 每个`CommandBuffer`都有一个表保存所有会用到的`Image`所对应的当前`Layout`，这个表用于记录`Image`的各种`Layout`转换
* 每个`CommandBuffer`有两个同步函数：`Flash`，`Wait`。在`Wait`时当`CommandBuffer`执行完成后将`Image`的`Layout`同步到`Image`中

还有一种相对简单的方式：在创建`Image`时`Context`中已经存在`CommandBuffer`了，在`Image`创建后会顺便将`Layout`转换指令加入`CommandBuffer`中（此种方法可能有局限性，比如：创建完并且转换指令已经推送到了`CommandBuffer`但是没提交就销毁了该`Image`就会导致`CommandBuffer`在提交时找不到`Image`，虽然这种情况出现是没有意义的）。

## Image的ImageView

在`Image::Create(...)`创建完`Turbo::Core::TImage`之后还没有结束，对于`Vulkan`渲染来说，`ImageView`是必须的。而一个`Image`可以对应多个`ImageView`，并且每个`ImageView`可以对应不同的`Subresource`(相当于`Image`的一个子集)

在每一个`Render::Image`中提供一个`GetImageView(...)`函数：

```mermaid
graph TD;
GetImageView["GetImageView(...)"]
FindImageView["在当前创建完成的多个ImageView集合中找兼容的ImageView，防止重复创建"]
IsFind{"是否找到了"}
CreateNewImageView["创建新的ImageView"]
ReturnImageView["返回已有的ImageView"]

GetImageView-->FindImageView
FindImageView-->IsFind

IsFind--否-->CreateNewImageView
IsFind--是-->ReturnImageView
```

创建一个`ImageView`需要如下参数：

```CXX
enum ImageViewType
{
    1D,
    2D,
    3D,
    CUBE,
    1D_ARRAY,
    2D_ARRAY,
    CUBE_ARRAY
};

enum
{
    COLOR_BIT,
    DEPTH_BIT,
    STENCIL_BIT,
};

//需要的参数如下：
//Turbo::Core::TImage* image;//该参数通过Turbo::Render::TImage中可获取
ImageViewType imageViewType;
//Format format;//该参数通过Turbo::Render::TImage中可获取
ImageViewAspect aspect;//该参数虽然对外公开，但是可以通过Turbo::Render::TImage和其子类推出来
uint32_t baseMipLevel;
uint32_t levelCount;
uint32_t baseArrayLayer;
uint32_t layerCount;
```

**考虑：这样设计是否为良构？`Turbo::Render::Image`的出现以及其子类`Turbo::Render::TTexture2D`、`Turbo::Render::TTexture3D`和`Turbo::Render::TDepthTexture2D`出现就是为了将繁杂的`Turbo::Core::TImage`和`Turbo::Core::TImageView`进行封装的，所以提供`GetImageView(...)`函数并不是一个好的选择。**

所以在此`Turbo`选择在`Turbo::Render::TImage`中将`Turbo::Core::TImage`和`Turbo::Core::TImageView`统一管理。

创建`ImageView`最好留给`Turbo::Render::TImage`的子类进行构建，`Turbo::Render::TImage`作为基类提供类似`protected: virtual Turbo::Core::TImageView * CreateImageView(....)`的接口函数，该接口函数会在`Turbo::Render::TImage::Create(...)`函数中创建完`Turbo::Core::TImage`之后调用，用于创建`Turbo::Core::TImageView`

## Context上下文

`Context`上下文中有整个`Turbo`的`Vulkan`环境，包括`Core::TInstance`、`Core::TPhysicalDevice`、`Core::TDevice`、`Core::TDeviceQueue`和各种`CommandBuffer`环境等

用户在构建上下文对象时，上下文的构造函数会去初始化环境。

在构造完`Context`之后，使用`Context`去构造`WorldRender/Render`进行后面渲染

`Context`需要提供`CreateImage(...)`，`DestroyImage(...)`，`CreateBuffer(...)`，`DestroyBuffer(...)`函数，用于创建和销毁资源

`Context`中应该有一个默认的`CommandBufferPool`，并提供`CommandBuffer* AllocateCommandBuffer()`和`void FreeCommandBuffer(CommandBuffer*)`函数

`Context`中应该有一个默认的`CommandBuffer`

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
>
> * 随即带来个问题：创建多大的`Surface`呢？

>2. 对于**用户指定了`Surface`**  
>如果用户指定了`Surface`，`Turbo`则会使用该`Surface`创建`Swapchain`，最终创建`ColorImage`用于存储最终的渲染结果（`RenderTarget`）

>3. 对于**用户更改了(重新指定了)`Surface`**  
>如果用户之前已经指定了`Surface`，并再次指定`Surface`，如果当前`Surface`和指定的`Surface`不相同需要等待之前多有相关工作结束，并重新构建相关资源。

### 方案二（采纳）√

`Turbo::Render`核心将使用离屏渲染，将渲染结果写入`RenderTarget`，如果用户绑定了`Surface`则将`RenderTarget`的渲染结果拷贝到`Surface`所对应的`Swapchain`所对应的`Image`中。

优点：

* 灵活，非常容易实现`GBufferPass`和`Post-ProcessPass`之类的功能
* 不依赖某一窗口，甚至是可以没有窗口
* 架构统一

缺点：

* 会多一次纹理图片颜色拷贝（无伤大雅，噗哈哈）

使用`CommandBuffer::CmdBlitImage(...)`可以很好的支持该工作

> 现在有个问题：如果采用方案二，离屏渲染的图片（`RenderTarget`）大小是多少呢？
>
>* 解决方案：需要用户自定义创建`Surface`（此`Surface`可以使虚的也可以是实的），并将创建好的`Surface`绑定给`Context`，之后`Context`根据`Surface`进行操作。
> 如此会有两种情况：
>   1. 用户没有指定`Surface`  
>       如果用户没有绑定任何`Surface`，`Context`将不会做任何事情，应为没有目标输出
>   2. 用户指定了`Surface`
>
>       ```mermaid
>        graph TD;
>            IsSurfaceSame{{当前Surface与用户指定的Surface是否相同}}--相同--->DoNothing[什么也不做];
>            IsSurfaceSame--不相同-->WaitAll[等待之前所有工作结束并回收资源];
>            WaitAll-->Refresh[使用用户指定的Surface进行新的构建];
>            DoNothing-->Frame[继续下一帧工作]
>            Refresh-->Frame[继续下一帧工作]
>        ```

## Surface

用户创建的`Surface`有两种

1. 虚拟`Surface`：对应着离屏渲染。所谓虚拟`Surface`是不跟任何窗口系统相关的虚拟表面（大白话是：不能显示在屏幕上，但可以获取渲染结果）
2. 真实`Surface`: 对应着与窗口系统相关的`Surface`(底层为`VkSurface`与`Turbo::Core::TSurface`对应)（大白话是：能显示在屏幕上，同时可以获取渲染结果）

这两种`Surface`都对应着`Turbo::Render::TSurface`，只不过是对应得构造函数不同罢了。

* 如果用户使用的是虚拟`Surface`，`Turbo`引擎将会在内部构建一套`ColorImage:RenderTarget`并在渲染结束后将渲染结果交给用户做后续工作
* 如果用户使用的是真实`Surface`，`Turbo`引擎将会在内部构建`Swapchain`等一系列工作，并将渲染结果展现在屏幕窗口上，同时用户也可以获取相应的渲染结果（同离屏渲染）
*注：现在`Turbo`并没有`Turbo::Windows`跨平台窗口层，而是交由用户自己制定需求，而大多数跨平台窗口层都提供返回`VkSurface`的接口，这也是`Turbo`支持跨平台窗口的原因，在未来也许会推出`Turbo::Windows`跨平台窗口层吧~？*

```CXX
namespace Turbo
{
    namespace Render
    {
        class TSurface
        {
            private:
            VkSurface vkSurface = VK_NULL_HANDLE;

            public:
            TSurface(uint32_t width,uint32_t height,uint32_t layer,uint32_t imageCount,...);//对应着[虚拟Surface]
            TSurface(VkSurface vkSurface);//对应着[真实Surface]
            //TSurface(const Turbo::TWindows& windows);//for未来~？
        }
    }
}
```

~~为了满足离屏渲染的需求，`Turbo::Core::TSurface`需要支持虚拟`Surface`~~
离屏渲染并不需要`Turbo::Core::TSurface`支持虚拟`Surface`，而是需要`RenderTarget`纹理

*注：以下代码已被遗弃，但可以做一个虚拟`Surface`内部数据参考*

```CXX
//虚拟Turbo::Render::TSurface对应的Turbo::Core::TSurface
Turbo::Core::TSurface属性
{
private:
    T_VULKAN_HANDLE_PARENT Turbo::Core::TDevice *device = Context.device;//上下文中创建的设备
    T_VULKAN_HANDLE_HANDLE VkSurfaceKHR vkSurfaceKHR = VK_NULL_HANDLE;//vkSurfaceKHR为空是虚Surface的标志

    bool isExternalHandle = true;//为true，单独为属性赋值

    std::vector<Turbo::Core::TQueueFamilyInfo> supportQueueFamilys;//空数组，size()为0

    uint32_t minImageCount;//同uint32_t Turbo::Render::TSurface::imageCount
    uint32_t maxImageCount;//同uint32_t Turbo::Render::TSurface::imageCount
    Turbo::Core::TExtent2D currentExtent;//同uint32_t Turbo::Render::TSurface::width 和 ~::height
    Turbo::Core::TExtent2D minImageExtent;//同uint32_t Turbo::Render::TSurface::width 和 ~::height
    Turbo::Core::TExtent2D maxImageExtent;//同uint32_t Turbo::Render::TSurface::width 和 ~::height
    uint32_t maxImageArrayLayers;//同uint32_t Turbo::Render::TSurface::layer

    Turbo::Extension::TSurfaceTransforms supportedTransforms;//TSurfaceTransformBits::TRANSFORM_IDENTITY_BIT
    Turbo::Extension::TSurfaceTransformBits currentTransform;//TSurfaceTransformBits::TRANSFORM_IDENTITY_BIT
    Turbo::Extension::TCompositeAlphas supportedCompositeAlpha;//TCompositeAlphaBits::ALPHA_OPAQUE_BIT
    Turbo::Core::TImageUsages supportedUsageFlags;//IMAGE_TRANSFER_SRC+IMAGE_TRANSFER_DST+IMAGE_COLOR_ATTACHMENT

    std::vector<Turbo::Extension::TSurfaceFormat> surfaceFormats;//{format=Turbo选择一个颜色格式R8G8B8A8,TColorSpace::colorSpaceType=TColorSpaceType::SRGB_NONLINEAR}
    std::vector<Turbo::Extension::TPresentMode> presentModes;//FIFO
}
```

## Swapchain

该类型由`Turbo`管理。对用户透明

~~为了满足离屏渲染的需求，`Turbo::Core::TSwapchain`需要支持虚拟`Surface`~~
同`Surface`情况

## 离屏渲染流程

`Turbo::Render`层的任务核心，其中`RenderTarget`是离屏渲染的目标纹理图片（内部对应一个`Image`）

```mermaid
 graph TD;
    UserCreateContext[用户创建Context上下文]
    UserCreateContext-->BeginFrame[开始当前帧]
    BeginFrame-->IsBindSurface{{是否已经绑定Surface}}
    IsBindSurface--未绑定-->DoNothingWithoutSurface[什么也不做]
    IsBindSurface--"已绑定(通过调用Context.BindSurface(...))"-->IsSurfaceSame[什么也不做]
    IsSurfaceSame{{当前Surface与用户指定的Surface是否相同}}--相同--->DoNothingForSameSurface[什么也不做]
    IsSurfaceSame--不相同-->WaitAll[等待之前所有工作结束并回收资源]
    subgraph RefreshSurface[使用用户指定的Surface进行新的构建]
        direction TB
        IsVirtualSurface{{是否是虚Surface}}
            subgraph CreateRenderTargetAccordingVirtualSurface[根据虚拟Surface创建RenderTarget]
                    direction TB
                    GetVirtualSurfaceConfig[获取用户的虚拟Surface的配置]
            end
        IsVirtualSurface--是-->CreateRenderTargetAccordingVirtualSurface
            subgraph CreateRenderTargetAccordingActualSurface[根据真实Surface创建RenderTarget]
                direction TB
                GetVkSurface[获取VkSurface] -->CreateSwapchain[根据VkSurface创建Swapchain]
                CreateSwapchain-->GetImage[获取Swapchain中的Image]
            end
        IsVirtualSurface--否-->CreateRenderTargetAccordingActualSurface
    end

    subgraph FrameGraph["FrameGraph"]
        direction TB
        CreateRenderTargetForFG[创建RenderTarget]-->UseRenderTarget
        subgraph UseRenderTarget["使用RenderTarget渲染（此处为FrameGraph的核心）"]
            direction TB
        end
        UseRenderTarget-->IsVirtualSurface1{{是否是虚Surface}}
        IsVirtualSurface1--是-->CopyResultReturntoUser[将RenderTarget结果拷贝给用户]
        IsVirtualSurface1--否-->CopyRenderTargetToSwapchainImage[将RenderTarget渲染结果拷贝 Swapchain的Image中]
        CopyRenderTargetToSwapchainImage-->ToUser[将渲染结果拷贝给用户]
        ToUser-->PresentSwapchainImage["显示到Surface上(PresentPass)"]
    end

    
    CreateRenderTargetAccordingVirtualSurface-->CreateRenderTargetForFG
    CreateRenderTargetAccordingActualSurface-->CreateRenderTargetForFG
    WaitAll-->IsVirtualSurface
    DoNothingForSameSurface-->CreateRenderTargetForFG
    CopyResultReturntoUser-->EndFrame[当前帧结束]
    PresentSwapchainImage-->EndFrame

    %%EndFrame-->BeginFrame
 ```

## 将`RenderTarget`结果拷贝给用户

 用户如何获取渲染的结果呢？

### 方案一 （弃用）×

 用户通过`Surface`获取渲染结果

### 方案二 （采纳）√

 用户通过自定义`PassNode`获取渲染结果

## 用户自定义`PassNode`

一个`PassNode`代表一个`GPU过程`，`GPU过程`主要有两个过程：

1. RenderPass：绘制过程
2. ComputePass：计算过程

而在`RenderPass`绘制过程中是可以绑定调用`ComputePass`的，所以`PassNode`主要是用于描述`绘制过程`。

而`PassNode`有两个阶段：`Setup`初始化阶段和`Execute`执行阶段

1. `Setup`初始化阶段  
该阶段主要是描述各`Subpass`，其中`Subpass`用于描述对各`Image`的读写情况，之后`Turbo`会根据相应配置创建相应的`RenderPass`，`FrameBuffer`等

```mermaid
graph TD;
    PassNode>"PassNode::Setup"]
    subgraph Subpass0[Subpass0]
        direction LR
        Depth0-.读.->Subpass
        Color0-.读.->Subpass
        Subpass--写-->Color1
        Subpass--写-->Color2
        Subpass--写-->Depth2

    end
    Subpass1["Subpass1"]
    Subpass2["Subpass2"]
    Subpassn["Subpass..."]
    
    PassNode-->Subpass0
    Subpass0-->Subpass1
    Subpass1-->Subpass2
    Subpass2-->Subpassn
```

```CXX
struct CustomPassData
{
    Resource colorTex;
    Resource normalTex;
    Resource depthTex;
}

//FrameGraph::PassNode::Setup
[&](TFrameGraph::TBuilder &builder, CustomPassData &data)
{
    data.colorTex = builder.Create<Texture2D>("color",{512,512,Usage::Color})
    data.normalTex = builder.Create<Texture2D>("normal",{512,512,Usage::Normal})
    data.depthTex = builder.Create<DepthTexture2D>("depth",{512,512,Usage::Depth})

    Subpass subpass0 = builder.CreateSubpass();    
    subpass0.Write(data.colorTex);
    subpass0.Write(data.depthTex);

    Subpass subpass1 = builder.CreateSubpass();
    subpass1.Read(data.colorTex);
    subpass1.Read(data.depthTex);
    subpass1.Write(data.normalTex);
}
```

2. `Execute`执行阶段  
执行阶段就是运行`Setup`阶段设置的各种`Subpass`，执行阶段会去创建`Pipeline`，绑定`CommandBuffer`等

```mermaid
graph TD;
    PassNode>"PassNode::Execute"]
            BeginRenderPass["BeginRenderPass"]
                subgraph Execute
                    direction TB
                    CmdBindPipeline[CmdBindPipeline]
                    CmdBindVertexBuffer[CmdBindVertexBuffer]
                    CmdDraw[CmdDraw]
                    CmdNextSubpass[CmdNextSubpass]
                    EtcExecute["..."]
                end

                subgraph MeshMaterial["Mesh/Material"]
                    direction TB
                    CmdBindMesh[CmdBindMesh]
                    CmdBindMaterial[CmdBindMaterial]
                    CmdMeshDraw[CmdMeshDraw]
                    EtcMeshMaterial["..."]
                end
                
            EndRenderPass["EndRenderPass"]

            PassNode-->BeginRenderPass
            BeginRenderPass--底层-->CmdBindPipeline
            CmdBindPipeline-->CmdBindVertexBuffer
            CmdBindVertexBuffer-->CmdDraw
            CmdDraw-->CmdNextSubpass
            CmdNextSubpass-->EtcExecute
            EtcExecute-->EndRenderPass

            BeginRenderPass-.高层.->CmdBindMesh
            CmdBindMesh-.->CmdBindMaterial
            CmdBindMaterial-.->CmdMeshDraw
            CmdMeshDraw-.->EtcMeshMaterial
            EtcMeshMaterial-.->EndRenderPass
```

```CXX
//FrameGraph::PassNode::Execute

//方案一(倾向于此方案)
[=](const CustomPassData &data, const TResources &resources, void *context) {
    Texture2D& color = resources.Get<Texture2D>(data.colorTex);
    Texture2D& normal = resources.Get<Texture2D>(data.normalTex);
    DepthTexture2D& depth = resources.Get<DepthTexture2D>(data.depth);

    //注：以下context->*仅为示意，大概率会随着设计的改变而变化
    Context* context = static_cast<Context*>(context);
    context->CmdBeginRenderPass(？？？TODO: 如何将RenderPass从PassNode中提出来？？？);
    context->CmdBindPipeline(pipeline);//来自Material：Pipeline需要现用，现创建
    context->CmdPushConstants(0, sizeof(alpha), &alpha);//来自Material
    context->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);//来自Material
    context->CmdBindVertexBuffers(vertex_buffers);//来自Mesh
    context->CmdSetViewport(frame_viewports);//来自Camera和Surface
    context->CmdSetScissor(frame_scissors);//来自Material
    context->CmdBindIndexBuffer(index_buffer);//来自Mesh
    context->CmdDrawIndexed(indices_count, 1, 0, 0, 0);//来自Drawable

    context->CmdNextSubpass();
    ...
    context->CmdEndRenderPass();
}

//方案二（需要高级特性）
[=](const CustomPassData &data, const TResources &resources, void *context) {
    Texture2D& color = resources.Get<Texture2D>(data.colorTex);
    Texture2D& normal = resources.Get<Texture2D>(data.normalTex);
    DepthTexture2D& depth = resources.Get<DepthTexture2D>(data.depth);

    Context* context = static_cast<Context*>(context);

    context->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);
    context->CmdSetViewport(frame_viewports);
    context->BindMaterial(material);
        //context->CmdBindPipeline(pipeline);//来自Material：Pipeline需要现用，现创建
        //context->CmdPushConstants(0, sizeof(alpha), &alpha);//来自Material
        //context->CmdBindPipelineDescriptorSet(pipeline_descriptor_set);//来自Material
        //context->CmdSetScissor(frame_scissors);//来自Material
    context->BindMesh(mesh);
        //context->CmdBindVertexBuffers(vertex_buffers);//来自Mesh
        //context->CmdBindIndexBuffer(index_buffer);//来自Mesh
    context->DrawMesh();
        //context->CmdDrawIndexed(indices_count, 1, 0, 0, 0);//来自Drawable

    context->CmdNextSubpass();
    ...
    context->CmdEndRenderPass();
}
```

由于`Pipeline`是在绑定时由`Turbo`动态管理创建，所以需要一个`Pipeline`描述，用于告诉`Context`绑定什么样的`Pipeline`

## Pipeline

`Pipeline`主要有两类

1. Graphic图形管线
2. Compute计算管线

其中计算管线比较简单，只需要指定计算着色器即可。

```CXX
class ComputePipeline
{
    ComputeShader* computeShader;
};

ComputeShader* compute_shader = new ...;
ComputePipeline compute_pipeline(compute_shader);

//FrameGraph::PassNode::Execute
context->BindPipeine(compute_pipeline);
context->Dispatch(...);
```

而对于图形管线，需要的相对较多了（大部分有默认值）：

```CXX
class GraphicsPipeline
{
    VertexShader* vertexShader;
    FragmentShader* fragmentShader;
    Topology topology;//POINT_LIST,LINE_LIST,LINE_STRIP,TRIANGLE_LIST等
    Polygon polygon;//FILL,LINE,POINT等
    Cull cull;//Front|Back
    FrontFace front;//COUNTER_CLOCKWISE,CLOCKWISE
    float lineWidth;
    bool depthTestEnable;
    bool depthWriteEnable;
    CompareOp depthCompareOp;//LESS_OR_EQUAL

    bool blendEnable;
    BlendFactor srcColorBlendFactor;
    BlendFactor dstColorBlendFactor;
    BlendOp colorBlendOp;
    BlendFactor srcAlphaBlendFactor;
    BlendFactor dstAlphaBlendFactor;
    BlendOp alphaBlendOp;

    ...等
    /*bool stencilTestEnable;
    StencilOp frontFailOp;
    StencilOp frontPassOp;
    StencilOp frontDepthFailOp;
    CompareOp frontCompareOp;*/
};

VertexShader* vertex_shader = new ...;
FragmentShader* fragment_shader = new ...;
GraphicsPipeline graphics_pipeline(vertex_shader, fragment_shader);
//FrameGraph::PassNode::Execute
context->BindPipeine(graphics_pipeline);
context->BindVertexBuffer(vertex_buffer);
context->BindIndexBuffer(index_buffer);
context->Draw(...);
```

~~`context`在绑定`pipeline`时并不会真的去创建`pipeline`，而只是更新`context`中当前`pipeline`状态，真正的的`Pipeline`创建在调用`DrawCall`绘制指令时，`Turbo`会根据当前状态进行相应对象的创建，归根结底，在`DrawCall`时创建`Pipeline`是因为：~~`Turbo::Core`中创建`TGraphicsPipeline`需要指定`std::vector<Turbo::Core::TVertexBinding>`，也就是顶点属性，而顶点属性正常来说应该在`Mesh`中，或者声明在`VertexBuffer`中(声明在`VertexBuffer`中，这是一个好主意)，并在绑定`VertexBuffer`时更新相关状态，总而言之：需要在创建`Pipeline`之前告诉`Turbo`绑定的顶点数据的属性，这样`Turbo`才能根据顶点属性创建`Pipeline`。由于其复杂性，新建一个`Pipeline的VertexBinding`章节单独讨论

### Pipeline的VertexBinding

>在`Vulkan`标准中对于`VkPipelineVertexInputStateCreateInfo::pVertexAttributeDescriptions`有这样的规定：
>
>`All elements of pVertexAttributeDescriptions must describe distinct attribute locations` [来源](https://registry.khronos.org/vulkan/specs/1.3/html/chap21.html#VUID-VkPipelineVertexInputStateCreateInfo-pVertexAttributeDescriptions-00617)
>
>翻译过来就是：绑定的`VkVertexInputAttributeDescription::location`不能有重复的

如前文所说，在创建`Pipeline`时需要指定`std::vector<Turbo::Core::TVertexBinding>`，而顶点属性现在存在于绑定的`VertexBuffer`中，其实还有一个地方：那就是`Shader`中，当用户创建完`VertexShader`后，`Turbo`其实在`VertexShader`中存有有着色器的`in`属性变量，也就是说`Turbo`知道该`Shader`需要什么样的`TVertexBinding`

按照`Shader`中的`in`变量声明来构建相应的`TVertexBinding`，应该算是一个好主意，但是会有一个问题：就是顶点着色器中的`in`声明需要与`VertexBuffer`对应上才行，而这种对应，`Turbo`并不能进行干预，只能用户自己写`Shader`时将`in`声明的变量与绑定的`VertexBuffer`相对应。换句话就是`Turbo`并不能干预用户如何实现`Shader`代码

`VertexBinding`主要用于描述如下：

1. `Binding`

    * `uint32_t binding`：绑定索引
    * `uint32_t stride`：单个数据长度
    * `TVertexRate rate`：相对`VERTEX`顶点（这个用的比较多），还是相对`INSTANCE`实例

2. `Attribute`（每个`Binding`所对应的的）

    * `uint32_t location`：对应着色器的接口`location`
    * `TFormatType formatType`：单个数据格式（相对于`Binding::stride`的长度）
    * `uint32_t offset`：偏移（相对于`Binding::stride`的长度）

![VertexInputBinding](./images/VertexInputBinding.png)

而这些属性主要位于`VertexBuffer`中，这也就是为什么`VertexBinding`与`VertexBuffer`一一对应

>`Vulkan`标准有如下规定：
>
>```CXX
>// Provided by VK_VERSION_1_0
>typedef struct VkPipelineVertexInputStateCreateInfo {
>    VkStructureType                             sType;
>    const void*                                 pNext;
>    VkPipelineVertexInputStateCreateFlags       flags;
>    uint32_t                                    vertexBindingDescriptionCount;
>    const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
>    uint32_t                                    vertexAttributeDescriptionCount;
>    const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
>} VkPipelineVertexInputStateCreateInfo;
>
>```
>
>* All elements of `pVertexBindingDescriptions` must describe distinct binding numbers  
>译：`pVertexBindingDescriptions`中所有成员不能有重复的`uint32_t VkVertexInputBindingDescription::binding`值
>* All elements of `pVertexAttributeDescriptions` must describe distinct attribute locations  
>译：`pVertexAttributeDescriptions`中所有成员不能有重复的`uint32_t VkVertexInputAttributeDescription::location`值

其中对应参数的确认阶段来源：

* `uint32_t Binding::binding`：在调用绑定顶点缓冲集时（`CmdBindVertexBuffers`），可以推出该值
* `uint32_t Binding::stride`：有两种方式
    1. 在创建`VertexBuffer`时指定当前顶点缓冲的`stride`值（采用此方式，作为`VertexBuffer`的一种属性，比较符合直觉）
    2. ~~在调用绑定顶点缓冲集时，设置该`stride`值~~(麻烦，在绑定时可能都不知道绑定的`VertexBuffer`中的数据格式)
* `TVertexRate Binding::rate`: 在创建`VertexBuffer`时指定（默认为`VERTEX`，如果是实例数据将会设置为`INSTANCE`）
* `uint32_t Attribute::location`: 在声明`Pipeline`时指定（也就是动态指定）
* `TFormatType Attribute::formatType`: 在创建`VertexBuffer`时指定（采用此方式，作为`VertexBuffer`的一种属性，比较符合直觉）
* `uint32_t Attribute::offset`: 在创建`VertexBuffer`时指定（采用此方式，作为`VertexBuffer`的一种属性，比较符合直觉）

综上则在创建`VertexBuffer`时，需要设置`VertexBuffer`的属性：

* 一个`uint32_t Binding::stride`
* 一个`TVertexRate Binding::rate`:*（注：由`Vulkan`标准可推出：一个`VertexBuffer`对于`VERTEX`和`INSTANCE`是一元的，也就是说一个`VertexBuffer`要么全是`VERTEX`，要么全是`INSTANCE`，不可能既是`VERTEX`也是`INSTANCE`）*
* 多个`Attribute`，每个`Attribute`包括：
  * `TFormatType Attribute::formatType`
  * `uint32_t Attribute::offset`

声明成伪代码：

```CXX
class Attribute
{
    TFormatType formatType;
    uint32_t offset;
};

class VertexBuffer: public Turbo::Render::TBuffer/*也许会作为Turbo::Render::TBuffer的一个子类*/
{
private:
    uint32_t stride;
    TVertexRate rate;
    std::map<std::string, Attribute> attributes;

public:
    VertexBuffer(uint32_t stride, TVertexRate rate = VERTEX);
    void AddAttribute(const std::string& attribute, TFormatType formatType, uint32_t offset);
};
```

需要在设置`GPU`指令时动态设置的的属性:

* `uint32_t Binding::binding`
* `uint32_t Attribute::location`：考虑如何对应：对应到哪个`VertexBuffer`的哪个`Attribute`上？参考如下伪代码

声明成伪代码：

```CXX
//假如Vertex Shader对于in属性声明如下
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;
layout (location = 4) in float weight;
layout (location = 5) in vec3 tangent;

//Turbo引擎端
typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

typedef struct vec2
{
    float x;
    float y;
} vec2;

typedef struct weight_tangent
{
    float weight;
    vec3 tangent;
} uv_tangent;

VertexBuffer position_buffer(/*stride*/sizeof(vec3));//rate默认为VERTEX
VertexBuffer normal_buffer(/*stride*/sizeof(vec3));//rate默认为VERTEX
VertexBuffer color_buffer(/*stride*/sizeof(vec3));//rate默认为VERTEX
VertexBuffer uv_buffer(/*stride*/sizeof(vec2));//rate默认为VERTEX
VertexBuffer weight_and_tangent_buffer(/*stride*/sizeof(weight_tangent));//rate默认为VERTEX

position_buffer.AddAttribute(/*attribute*/"POSITION", /*formatType*/R32G32B32, /*offset*/0);
normal_buffer.AddAttribute(/*attribute*/"NORMAL", /*formatType*/R32G32B32, /*offset*/0);
color_buffer.AddAttribute(/*attribute*/"COLOR", /*formatType*/R32G32B32, /*offset*/0);
uv_buffer.AddAttribute(/*attribute*/"UV", /*formatType*/R32G32, /*offset*/0);
weight_and_tangent_buffer.AddAttribute(/*attribute*/"WEIGHT", /*formatType*/R32, /*offset*/offsetof(weight_tangent, weight));
weight_and_tangent_buffer.AddAttribute(/*attribute*/"TANGENT", /*formatType*/R32G32B32, /*offset*/offsetof(weight_tangent, tangent));

//Turbo将根据绑定的顶点属性进行解析
command_buffer->BindVeretxAttribute(position_buffer, "POSITION", /*location*/0);
command_buffer->BindVeretxAttribute(normal_buffer, "NORMAL", /*location*/1);
command_buffer->BindVeretxAttribute(color_buffer, "COLOR", /*location*/2);
command_buffer->BindVeretxAttribute(uv_buffer, "UV",/*location*/3);
command_buffer->BindVeretxAttribute(weight_and_tangent_buffer, "WEIGHT",/*location*/4);
command_buffer->BindVeretxAttribute(weight_and_tangent_buffer, "TANGENT", /*location*/5);
```

如上的设计有弊端：

* 当场景中有大量需要绘制的模型时，光用于描述每一个模型的`VertexBuffer`中的`Attribute`那个字符串（`POSITION`、`NORMAL`等）就有很多，这会占据大量的没有必要的内存，所以将字符串去掉是很有必要的

优化的方式有两种：

1. 将用于`attribute`的字符串改为使用数字标时的`ID`号或索引号，这会大大减少内存使用

```CXX
VertexBuffer position_buffer(/*stride*/sizeof(vec3));//rate默认为VERTEX
VertexBuffer normal_buffer(/*stride*/sizeof(vec3));//rate默认为VERTEX
VertexBuffer color_buffer(/*stride*/sizeof(vec3));//rate默认为VERTEX
VertexBuffer uv_buffer(/*stride*/sizeof(vec2));//rate默认为VERTEX
VertexBuffer weight_and_tangent_buffer(/*stride*/sizeof(weight_tangent));//rate默认为VERTEX

uint32_t position_id = position_buffer.AddAttribute(/*formatType*/R32G32B32, /*offset*/0);
uint32_t normal_id = normal_buffer.AddAttribute(/*formatType*/R32G32B32, /*offset*/0);
uint32_t color_id = color_buffer.AddAttribute(/*formatType*/R32G32B32, /*offset*/0);
uint32_t uv_id = uv_buffer.AddAttribute(/*formatType*/R32G32, /*offset*/0);
uint32_t weight_id = weight_and_tangent_buffer.AddAttribute(/*formatType*/R32, /*offset*/offsetof(weight_tangent, weight));
uint32_t tangent_id = weight_and_tangent_buffer.AddAttribute(/*formatType*/R32G32B32, /*offset*/offsetof(weight_tangent, tangent));

//Turbo将根据绑定的顶点属性进行解析
command_buffer->BindVeretxAttribute(position_buffer, position_id, /*location*/0);
command_buffer->BindVeretxAttribute(normal_buffer, normal_id, /*location*/1);
command_buffer->BindVeretxAttribute(color_buffer,color_id, /*location*/2);
command_buffer->BindVeretxAttribute(uv_buffer, uv_id,/*location*/3);
command_buffer->BindVeretxAttribute(weight_and_tangent_buffer, weight_id,/*location*/4);
command_buffer->BindVeretxAttribute(weight_and_tangent_buffer, tangent_id, /*location*/5);
```

2. 完全抛弃`attribute`的字符串，改为完全动态绑定（此种方式类似`OpenGL`的`glVertexAttribPointer`函数）

```CXX
//OpenGL
void glVertexAttribPointer( GLuint index,
                            GLint size,
                            GLenum type,
                            GLboolean normalized,
                            GLsizei stride,
                            const GLvoid * pointer);
```

```CXX
VertexBuffer position_buffer();
VertexBuffer normal_buffer();//rate默认为VERTEX
VertexBuffer color_buffer();//rate默认为VERTEX
VertexBuffer uv_buffer();//rate默认为VERTEX
VertexBuffer weight_and_tangent_buffer();//rate默认为VERTEX

command_buffer->BindVeretxAttribute(position_buffer, /*stride*/sizeof(vec3), /*formatType*/R32G32B32, /*offset*/0, /*rate*/VERTEX, /*location*/0);//for position
command_buffer->BindVeretxAttribute(normal_buffer, /*stride*/sizeof(vec3), /*formatType*/R32G32B32, /*offset*/0, /*rate*/VERTEX, /*location*/1);//for normal
command_buffer->BindVeretxAttribute(color_buffer, /*stride*/sizeof(vec3),/*formatType*/R32G32B32, /*offset*/0, /*rate*/VERTEX, /*location*/2);//for color
command_buffer->BindVeretxAttribute(uv_buffer, /*stride*/sizeof(vec2), /*formatType*/R32G32, /*offset*/0, /*rate*/VERTEX, /*location*/3);//for uv
command_buffer->BindVeretxAttribute(weight_and_tangent_buffer, /*stride*/sizeof(weight_tangent), /*formatType*/R32, /*offset*/offsetof(weight_tangent, weight), /*rate*/VERTEX, /*location*/4);//for weight
command_buffer->BindVeretxAttribute(weight_and_tangent_buffe, /*stride*/sizeof(weight_tangent), /*formatType*/R32G32B32,/*offset*/offsetof(weight_tangent, tangent), /*rate*/VERTEX, /*location*/5);//for tangent
```

`Turbo`考虑优化方式`1`和`2`都提供相应的接口

在调用`BindVeretxAttribute`时`Turbo`不会真的将`VertexBuffer`塞入`CommandBuffer`，只有在绑定了`pipeline`并且调用了绘制指令时才会去真正的构建`Pipeline`和将`VertexBuffer`塞入`CommandBuffer`中。

*注：根据`Vulkan`标准来看在`DrawCall`指令之前需要绑定所有绘制相关的数据（包括渲染管线和顶点缓存数组），此时`DrawCall`便是是收集统计的好时机*

### 绑定Pipeline的DescriptorSet

对于`Pipeline`能够正确执行，还需要将对应的各种`layout (set = M, binding = N) uniform UNIFORM_TYPE UNIFORM_NAME[T]`属性进行绑定，`Turbo`将会提供`BindDescriptor(uint32_t set, uint32_t binding, std::vector<各种uinform资源类型>)`接口进行对应资源绑定

在`Turbo`中将所有绑定的资源都视为可一次性绑定多个资源，也就是

```CXX
//shader
layout (set = M, binding = N) uniform UNIFORM_TYPE UNIFORM_NAME[T]

//host
context.BindDescriptor(uint32_t set, uint32_t binding, std::vector<各种uinform资源类型>)
```

就算绑定的资源只有一个，也视为`size`为`1`的数组进行设置（这与`Turbo`的`Core`层相一致）

在`Turbo`的`Core`层，需要创建完`Pipeline`才能进行`Descriptor`相关资源数据的绑定，所以在调用`BindDescriptor(...)`时，内部是使用了一个树状结构先将用户绑定的`Descriptor`进行缓存。

目前`Turbo`应该提供如下基本`Descriptor`绑定

* `Buffer`: 对应`GLSL`中`uniform Buffer{...}Buffer;`。最终存储为`std::vector<TBuffer *>`
* `std::pair<TImage, TSampler>`: 对应`GLSL`中`uniform sampler2D xxx;`。最终存储为`std::vector<std::pair<TImageView *, TSampler *>>`
* `TImage`: 对应`GLSL`中`uniform texture2D xxx;`。最终存储为`std::vector<TImageView *>`
* `Sampler`: 对应`GLSL`中`uniform sampler xxx;`。最终存储为`std::vector<TSampler *>`

问题来了，对于不同的类型`Turbo`如何进行不同类型的`Descriptor`缓存呢？(泛型编程可能是一个解决方法，但是有点麻烦，不直观)

一种比较直接的方式就是使用`Map`直接记录对应`Descriptor`数组：

```CXX
//Turbo的内部维护结构
struct DescriptorID
{
    uint32_t set = MAX;
    uint32_t binding = MAX;
}

std::map<DescriptorID, std::vector<TBuffer *>> bufferMap;
std::map<DescriptorID, std::vector<std::pair<TImageView *, TSampler *>>> imageCombineWithSamplerMap;
std::map<DescriptorID, std::vector<TImageView *>> sampledImageMap;
std::map<DescriptorID, std::vector<TSampler *>> samplerMap;

//对应的Context对外接口
BindDescriptor(uint32_t set, uint32_t binding, std::vector<TBuffer>);//在bufferMap中进行缓存
BindDescriptor(uint32_t set, uint32_t binding, std::vector<std::pair<TImage, TSampler>>);//在bimageCombineWithSamplerMap中进行缓存
BindDescriptor(uint32_t set, uint32_t binding, std::vector<TImage>);//在sampledImageMap中进行缓存
BindDescriptor(uint32_t set, uint32_t binding, std::vector<Sampler>);//在samplerMap中进行缓存
```

这有一个问题：用户在一次`DrawCall`时重复性绑定，如下

```CXX
BindDescriptor(0, 0, some_buffers);//在bufferMap中进行缓存
BindDescriptor(0, 0, some_images);//在sampledImageMap中进行缓存
```

用户在同一个`set`和`binding`下绑定了不同类型的`Descriptor`，虽然此行为在`Vulkan`标准中不被允许，但在`Turbo`中将会进行覆盖操作，这就需要`Turbo`记录某某`set`和`binding`下的`Descriptor`是否已经被绑在了哪个类型的缓存里，如果已经绑定了，将原来的绑定解除，进行新的绑定，反之则直接绑定。这需要`Turbo`内部提供一个数据结构用于记录某某`set`和`binding`被绑定到了哪个缓冲中。

可能的结构为：

```CXX
enum DescriptorType
{
    UNIFROM_BUFFER,//表示相应数据缓存在bufferMap中
    COMBINED_IMAGE_SAMPLER,//表示相应数据缓存在imageCombineWithSamplerMap中
    SAMPLED_IMAGE,//表示相应数据缓存在sampledImageMap中
    SAMPLER,//表示相应数据缓存在samplerMap中
};

class BindingMap
{
    std::map<uint32_t binding, DescriptorType> bindings;
};

class SetMap
{
    std::map<uint32_t set, BindingMap> sets;
};
```

这样`Turbo`的判断流程大致如下：

```mermaid
graph TD;

Input["传入set号, binding号, std::vector<各种uinform资源类型>"]
IsSetMapHasSet{"SetMap中是否存有传入的Set号"}

IsBindingMapHasBinding{"BindingMap中是否存有传入的Binding号"}
AddNewSetAndBinding["增加新的Set，Binding映射（SetMap中增加新项）。并将std::vector<各种uinform资源类型>存入相应缓存"]

subgraph RefreshSetAndBindgMap["更新Set和Binding对应的缓存"]
    RemoveDescriptorBinding["将对应的描述符数据从相应缓存中移除"]
    AddDescriptorBinding["将对应的描述符数据添加到相应缓存中"]
    UpdateSetBindingMap["更新Set和Binding的相应记录"]

    RemoveDescriptorBinding-->AddDescriptorBinding
    AddDescriptorBinding-->UpdateSetBindingMap
end

AddNewBinding["增加新的Binding（BindingMap增加新项目）。并将std::vector<各种uinform资源类型>存入相应缓存 "]

Input-->IsSetMapHasSet
IsSetMapHasSet--是-->IsBindingMapHasBinding
IsSetMapHasSet--否-->AddNewSetAndBinding

IsBindingMapHasBinding--是-->RefreshSetAndBindgMap
IsBindingMapHasBinding--否-->AddNewBinding
```

### 绑定Pipeline

`Turbo`的`Context`中分别存有用于计算管线和图形管线的管线缓存

```CXX
class Context
{
    TComputePipeline compute_pipeline;
    TGraphicsPipeline graphics_pipeline;

    std::vector<Turbo::Core::TPipeline*> pipeline;//所有创建的管线
}
```

当用户绑定管线，其实就是更新相应的`TComputePipeline compute_pipeline`和`TGraphicsPipeline graphics_pipeline`

### Pipeline创建

`ComputePipeline`创建需要如下数据:

* `ComputerShader`：来源于绑定的`ComputePipeline`中

`GraphicsPipeline`创建需要如下数据:

* `RenderPass`：来源于`BeginRenderPass(...)`中
* `subpass`：来源于`NextSubpass()`中
* `std::vector<TVertexBinding>`：来源于绑定的`BindVeretxAttribute`中
* `std::vector<TShader *> &shaders`：来源于绑定的`GraphicsPipeline`中
* `各种渲染配置（包括Topology，PolygonMode，CullModes等等）`：来源于绑定的`GraphicsPipeline`中

## Subpass

`Subpass`实际上是多个`Image`的集合，包括如下三种`Image`集

1. `Input`
2. `Color`
3. `DepthStencil`（确切说`DepthStencil`不是`Image`集，应该是单一的，只能有一张`DepthStencil`纹理）

```CXX
//in Turbo::Render
class Subpass
{
    private:
        std::vector<ColorImage> colors;
        std::vector<Image> inputs;
        DepthStencilImage depthStencil;

    public:
        Subpass& AddColorAttachment(const ColorImage& colorImage);
        Subpass& AddInputAttachment(const Image& image);
        Subpass& SetDepthStencilAttachment(const DepthStencilImage& depthStencilImage);

        const std::vector<ColorImage>& GetColorAttachments();
        const std::vector<Image>& GetInputAttachments();
        const DepthStencilImage GetDepthStencilAttachment();
};
```

## RenderPass

`RenderPass`实际上是多个`Subpass`的集合

```CXX
//in Turbo::Render
class RenderPass
{
    private:
        std::vector<Subpass> subpasses;

    public:
        RenderPass& AddSubpass(const Subpass& subpass);
        const std::vector<Subpass>& GetSubpasses();
};
```

## Context::CmdBeginRenderPass

在`FrameGraph::PassNode::Execute`阶段绑定`RenderPass`时

```CXX
context->CmdBeginRenderPass(render_pass)
```

`Turbo`主要做两件事：

1. 创建相应的`RenderPass`
2. 创建相应的`FrameBuffer`

`FrameBuffer`中可以很轻松的得到其中的`Attachment`集合（可以获取对应的`ImageView`），而`RenderPass`需要从各个`Subpass`中统计出来，所以为了方便，`RenderPass`需要提供`std::vector<Turbo::Render::TImage> GetAttachemts()`函数。

对于`RenderPass`和`FrameBuffer`的创建，需要根据`FrameGraph::PassNode::Setup`阶段中声明的各种`Subpass`来创建。

现在有个问题：如何从`PassNode`中将`RenderPass`配置提取出来，并传给`Context`，说白了就是传给`CmdBeginRenderPass(RenderPass)`函数的`RenderPass`参数的实参如何构建？接口如何设计？

可能的方式：

```CXX
[=](const PresentPassData &data, const TResources &resources, void *context) 
{
    FrameGraph::RenderPass fg_render_pass = resources.GetRenderPass();

    //std::vector<FrameGraph::Subpass> subpasses = fg_render_pass.GetSubpasses();
    //FrameGraph::Attachment attachment = subpasses[x].GetAttachment();
    //Texture2D &color_texture = resources.Get<Texture2D>(data.colorTexture);
    //Texture2D &color_texture = attachment.Get<Texture2D>(data.colorTexture);
    //...

    Render::RenderPass render_pass = Fg_RenderPass_To_Render_RnederPass(fg_render_pass);

    context->CmdBeginRenderPass(render_pass);
    ...
    context->CmdEndRenderPass();
}
```

`FrameGraph::PassNode`中增加`FrameGraph::RenderPass FrameGraph::PassNode::GetRenderPass()`，这样就可以获得`FrameGraph::PassNode::Setup`阶段所配置的`RenderPass`，并拿着该配置创建`Context`需要的`RenderPass`。缺点就是有点麻烦，需要定义两头差不多的`RenderPass`数据结构。优化的方式就是直接将`FrameGraph::RenderPass`作为`Context::CmdBeginRenderPass`的实参。

```CXX
//Turbo::Render
class Context
{
    //void BeginRenderPass(Turbo::FrameGraph::TRenderPass &renderPass);//会有问题，见下文
    void BeginRenderPass(Turbo::Render::TRenderPass &renderPass);
}
```

对于`void Context::BeginRenderPass(Turbo::FrameGraph::TRenderPass &renderPass)`，由于`Turbo::FrameGraph::TRenderPass`下`Turbo::FrameGraph::TSubpass`下绑定的各个资源都是资源句柄`id`，对于该资源句柄对应得资源究竟是什么，只有开发用户知道，而这对于`Turbo`来说并不知道，所以更不用说在`void Context::BeginRenderPass(Turbo::FrameGraph::TRenderPass &renderPass)`中解析出对应资源句柄`id`对应得究竟是什么类型的资源。

`Turbo`对于`RenderPass`中的各种`Subpass`所绑定的`Attachment`，`Turbo`会进行统计，目的是创建与之对应的`FrameBuffer`，所以在此规定一下统计顺序，创建的`FrameBuffer`顺序也与之对应：

1. `Color`附件集
2. `Input`附件集
3. `DepthStencil`附件集

### RenderPass 创建

当调用`Context::BeginRenderPass(...)`后，`Turbo`会去查找是否有已经创建完的`RenderPass`,尝试重复利用已有的`RenderPass`，如果没有找到的话，则创建相应的`RenderPass`。

由于需要查询当前`Context`是否存在相互兼容的`RenderPass`已做到重复利用已有的`RenderPass`，此时需要`Context`中存在一个用于存储`RenderPass`的集合（`Set`）或池子(`Pool`,这个不错)

#### RenderPassPool

`Context`下有多个`RenderPass`，存储在`RenderPassPool`中。

`RenderPassPool`应该有如下功能：

* `bool Find(RenderPass& renderPass)`：查询相应的兼容的`RenderPass`，如果存在则返回`true`,并将结果写入到`RenderPass& renderPass`中，如果没找到则返回`false`，在内部会去调用`std::find(...)`函数，所以`class RenderPass`需要实现对于`==`符号重载
* `RenderPass& Allocate(RenderPass& renderPass)`:先使用`Find(renderPass)`查询一下是否有该`RenderPass`,如果有直接返回相应的`RenderPass`，反之则分配一个`RenderPass`并存入池子中，之后返回创建的`RenderPass`
* `void Free(RenderPass& renderPass)`：销毁相应的`RenderPass`

对于`RenderPassPool`中存储的`RenderPass`应该关心如下事物（核心思路是适配`Turbo::Core::TRenderPass`）：

* 对应的`Turbo::Core::TRenderPass`
* 从`Turbo::Render::TRenderPass`中的各种`Subpass`
* 从`Turbo::Render::TRenderPass`中的各种`Subpass`中指定的`attachment`相关的

### RenderPassProxy(RenderPass代理)

由于需要管理`RenderPass`和`FrameBuffer`，所以提供了`RenderPassProxy`类，用于整合管理`RenderPass`和`FrameBuffer`，并由`RenderPassPool`创建和销毁

> `新的想法`
>
>最近想到一种比较符合直觉的模式：刷新模式（我自己起的名字）
>
>由于之前使用`RenderPassPool`来创建`RenderPassProxy`类，`RenderPassProxy`类下面有真正的`RenderPass`，中间多一个`RenderPassProxy`来进行转换总感觉怪怪的，所以想到了如下方式：
>
>```CXX
>/*
>Turbo::Render::TRenderPass
>{
>    friend class TRenderPassPool;//使得TRenderPassPool可以去刷新renderPass成员变量
>
>    private:
>        Turbo::Core::TRenderPass* renderPass=nullptr;//注意：此成员变量为private，为待刷新的数据
>}
>*/
>Turbo::Render::TRenderPass render_pass;//此时其成员变量renderPass（真正的RenderPass现在为空）
>
>Turbo::Render::TRenderPassPool render_pass_pool;
>
>bool result = render_pass_pool.Allocate(render_pass);//此时进行Turbo::Render::TRenderPass::renderPass成员变量的刷新
>if(result)
>{
>    //创建RenerPass成功
>    //render_pass的成员变量renderPass将会被TRenderPassPool附上有效值
>    {
>        //伪代码
>        Turbo::Core::TRenderPass* core_render_pass = render_pass.renderPass;
>        if(core_render_pass != nullptr)
>        {
>           ...
>        }
>    }
>    
>}
>else
>{
>    //创建RenerPass失败
>    //render_pass的成员变量renderPass仍然为空
>}
>```

在创建完`Turbo::Core::TRenderPass`之后，可以通过`const std::vector<TAttachment> & Turbo::Core::TRenderPass::GetAttachments()`函数获取相应的

### Turbo::Render::TRenderPass 转 Turbo::Core::TRenderPass

通过用户指定的`Turbo::Render::TRenderPass`在`RenderPassPool`中创建`Turbo::Core::TRenderPass`，对于`Turbo`来说其主要任务有两个

1. 收集各个`Subpass`中的`Attachment`，集成到统一的一个`Attachment`数组中
2. 计算各个`Subpass`中的`Attachment`相对于`Attachment`数组中的偏移等（`AttachmentReference`）

由于会有重复的`Attachment`(如何判断两个`Attachment`相等：通过查看`Attachment`底层`Image`是否为同一个)，需要剔除重复的`Attachment`项目

### FrameBuffer 创建

一个`RenderPass`下可以绑定多个`FrameBuffer`，在使用`RenderPassPool`得到了一个有效的`RenderPass`后，`Turbo`需要根据用于传入的`RenderPass`配置来查看当前的`RenderPassPool`返回的`RenderPass`是否有支持当前的`FrameBuffer`。对应得流程如下：

```mermaid
graph TD;
InputRenderPass["外部输入的RenderPass（配置）"]
AnValidRenderPass["RenderPassPool返回的一个有效的RenderPass（RenderPassProxy）"]
FindFrameBuffer{"寻找是否有兼容的FrameBuffer"}

CreateFrameBuffer["创建FrameBuffer"]
ReturnFrameBuffer["返回FrameBuffer"]

InputRenderPass-->AnValidRenderPass
AnValidRenderPass-->FindFrameBuffer
FindFrameBuffer--没找到-->CreateFrameBuffer
FindFrameBuffer--找到了-->ReturnFrameBuffer
CreateFrameBuffer-->ReturnFrameBuffer
```

现在有一个问题：

`FrameBuffer`中对应的`Image`是每一帧都在做变化的，而且一次渲染只能绑定一个`FrameBuffer`，所以`FrameBuffer`的生命周期只在一帧中。

也就是说在创建完`RenderPass`之后，需要去类似`FrameBufferPool`结构中去找是否有兼容的`FrameBuffer`，如果有的话直接返回，没有的话新创建一个，之后渲染完一帧后回收销毁`FrameBufferPool`中的所有的`FrameBuffer`

*注：根据`Vulkan`标准，创建`FrameBuffer`时，必须指定`RenderPass`（标准规定，是与之兼容的`RenderPass`）。所以在创建`FrameBuffer`前必须创建`RenderPass`*

同`RenderPassPool`创建`RenderPass`类似，`FrameBufferPool`也需要传入`RenderPass`，并将创建结果刷新到`RenderPass`中，这需要`RenderPass`有一个存储刷新`FrameBuffer`的成员变量

## DrawCall

最经典的`DrawCall`就是`vkCmdDraw`和`vkCmdDrawIndexed`，对于`Turbo`为`Draw(...)`和`DrawIndexed(...)`

此时对于调用`DrawCall`，`Turbo`会收集之前用户设置的数据，并对相关资源进行`创建`，`记录`相关指令，最后`清空`相关资源。

对于`创建`：

* 创建`GraphicsPipeline`
  * 收集`RenderPass`：来源于`BeginRenderPass(...)`中
  * 收集`subpass`：来源于`NextSubpass()`
  * 收集`std::vector<TVertexBinding>`：来源于`BindVeretxAttribute(...)`
  * 收集`std::vector<TShader *> &shaders`：来源于`BindPipeine(...)`所绑定的`GraphicsPipeline`中
  * 收集`Pipeline属性（Topology，Polygon，CullModes等）`：来源于`BindPipeine(...)`所绑定的`GraphicsPipeline`中

    *注：一帧结束后销毁*

* 创建`TPipelineDescriptorSet`
  * 通过`TDescriptorPool`的`Allocate(TPipelineLayout*)`函数
  * 传入的`TPipelineLayout`从之前创建的`Pipeline`获得
  * 通过`TPipelineDescriptorSet`的`BindData`将数据绑定进去，数据来源于`BindDescriptor(...)`

    *注：一帧结束后回收*

对于`记录`：

* 记录`CmdBindPipeline(...)`指令，来源于之前创建的`GraphicsPipeline`
* 记录`CmdBindPipelineDescriptorSet(...)`指令，来源于之前创建的`TPipelineDescriptorSet`
* 记录`CmdBindVertexBuffers(...)`指令，来源于之前调用`BindVeretxAttribute`中的绑定
* 记录`DrawCall`指令(`CmdDraw`和`CmdDrawIndexed`)，来源于用户对于`DrawCall`的调用

对于`清空`：

* 清空由于`BindVeretxAttribute(...)`记录的`VertexBuffer`和`std::vector<TVertexBinding>`
* 清空由于`BindDescriptor(...)`记录的`Descriptor`和相关记录结构

*注：如果用户在绑定完`VeretxAttribute`和`Descriptor`之后没有再调用相关绑定函数接口，`Turbo`调用`DrawCall`将不会再收集更新相关指令，因为相关指令没有更新的必要，默认使用之前的绑定结果。（如果新的渲染与之前的绑定的数据不兼容的话将会因不符合`Vulkan`标准而出问题）*

## Dispatch

## Shader

`Shader`（着色器），是组成`Pipeline`的重要组件，常见的`Shader`如下：

1. `VertexShader`（顶点着色器）
2. `GeometryShader`（几何着色器）
3. `TessellationShader`（细分着色器）
4. `FragmentShader`(片元着色器)

光追标准着色器(`NVIDIA`标准或`Khronos`标准)：

5. `RayGenerationShader`
6. `IntersectionShader`
7. `AnyHitShader`
8. `ClosetHitShader`
9. `MissShader`

`NVIDIA`标准：

10. `TaskShader`
11. `MeshShader`

## 指令推送

指令推送就是将记录了一堆指令的`CommandBuffer`推送到`GPU`进行执行，所以`Context`中需要提供`Flush()`函数

```mermaid
graph TD;
Flush["Context::Flush(...)函数调用"]
CreateFence["创建Fence"]
SubmmitCurrentCommandBuffer["推送当前CommandBuffer"]
NoWaitAndPushIntoFenceSet["将Fence加入到【待同步Fence集合】中(将来的某一时刻进行同步等待)"]
CurrentCommandBufferPushIntoCommandBufferSet["将当前CommandBuffer加入到【待同步CommandBuffer集合】中"]
ReCreateCommandBuffer["重新创建一个CommandBuffer,并作为当前CommandBuffer使用"]
Return["返回"]

Flush-->CreateFence
CreateFence-->SubmmitCurrentCommandBuffer
SubmmitCurrentCommandBuffer-->NoWaitAndPushIntoFenceSet

NoWaitAndPushIntoFenceSet-->CurrentCommandBufferPushIntoCommandBufferSet
CurrentCommandBufferPushIntoCommandBufferSet-->ReCreateCommandBuffer
ReCreateCommandBuffer-->Return
```

*注：`Context::Flush(...)`函数调用的最佳时机多位于`Renderer::BeginFrame()`或`Renderer::EndFrame()`时（目前`Turbo`还没有`Renderer`的相关设计）*

## 指令等待

指令推送到`GPU`执行后，需要在未来的某一时刻进行等待`GPU`指令执行完成，所以`Context`需要提供`bool Wait(uint64_t timeout)`函数

```mermaid
graph TD;
Wait["Context::Wait(...)函数调用"]
WaitAllFenceTimeoutFenceSet["等待【待同步Fence集合】中的Fence，timeout时长"]
IsHasTimeout{"是否有超时"}
YesHasTimeout["说明【待同步CommandBuffer集合】中有的CommandBuffer在timeout时间段内没有完成"]
NoHasTimeout["说明所有的CommandBuffer都完成了"]

RemoveAndDestroyCommandBuffers["销毁并移除所有【待同步CommandBuffer集合】中的CommandBuffer"]
RemoveAndDestroyFences["销毁并移除所有【待同步Fence集合】中的Fence"]

RemoveFinishedCommandBufferAndDestroy["移除【待同步CommandBuffer集合】中已完成的CommandBuffer并销毁"]
RemoveFinishedFenceAndDestroy["移除【待同步Fence集合】中已完成的Fence并销毁"]

ReturnFalse["返回False，标时超时，规定时间内有任务没完成"]
ReturnTrue["返回True，规定时间内所有任务完成"]

Wait-->WaitAllFenceTimeoutFenceSet
WaitAllFenceTimeoutFenceSet-->IsHasTimeout
IsHasTimeout--有超时-->YesHasTimeout
IsHasTimeout--无超时-->NoHasTimeout

NoHasTimeout-->RemoveAndDestroyCommandBuffers
RemoveAndDestroyCommandBuffers-->RemoveAndDestroyFences
RemoveAndDestroyFences-->ReturnTrue

YesHasTimeout-->RemoveFinishedCommandBufferAndDestroy
RemoveFinishedCommandBufferAndDestroy-->RemoveFinishedFenceAndDestroy
RemoveFinishedFenceAndDestroy-->ReturnFalse
```

*注：判断`是否有超时`时需要`Turbo::Core`层提供同时等待多个`Fence`的接口，目前未提供该接口，只能一个`Fence`一个`Fence`的等，需要扩展`Turbo::Core`接口，比如提供`class Turbo::Core::TFences`*

*注：`Context::Wait(...)`函数调用的最佳时机多为获取`Swapchain`的`Image`时（当`Swapchain`的`Image`上次被使用后，这次又再一次被使用，这时需要等待上一次使用结束，才能进行这一次的使用）*

## 异步资源回收

由于`FrameGraph`在`void Execute(...)`时的流程如下

```mermaid
graph TD;
    CreateResource["创建PassNode要用的资源数据"]
    PassNodeExecute["运行PassNode的Execute回调"]
    DestroyResource["销毁不需要的资源数据"]

    CreateResource-->PassNodeExecute
    PassNodeExecute-->DestroyResource
    DestroyResource--下一个PassNode-->CreateResource
```

这里在`销毁不需要的资源数据`时，此时可能用户并没有调用`Flush`，换句话说就是，用户此时可能并没有将指令推送到`GPU`，如果此时销毁了相应的资源，之后再推送指令，由于资源已经销毁了，所以在`销毁不需要的资源数据`时并不能真的去销毁资源，而需要将资源保存下来，等待未来的某一合适的时候销毁。

`Filament`引擎在`销毁不需要的资源数据`时，是将资源保存到了一个`Cache`中，如下：

```CXX
//Filament engine code
void ResourceAllocator::destroyTexture(TextureHandle h) noexcept {
    if constexpr (mEnabled) {//目前mEnabled永远为true
        // find the texture in the in-use list (it must be there!)
        auto it = mInUseTextures.find(h);
        assert_invariant(it != mInUseTextures.end());

        // move it to the cache
        const TextureKey key = it->second;
        uint32_t size = key.getSize();

        //将资源转移到缓存中
        mTextureCache.emplace(key, TextureCachePayload{ h, mAge, size });
        mCacheSize += size;

        // remove it from the in-use list
        mInUseTextures.erase(it);
    } else {
        mBackend.destroyTexture(h);
    }
}
```

在`Filament`中`mTextureCache`是作为`ResourceAllocator`成员变量来缓存资源文件的，这是一个不错的选择。

目前能想到的资源回收有两个方面：

1. 每个资源有个最长生命周期，超过生命周期将自动回收（这个可能需要想一下`Turbo::Core::Image`、`Turbo::Core::TImageView`和`Turbo::Render::TImage`之间如何配合，配合不好可能会出问题）
2. `ResourceAllocator`提供一个`Gc()`函数，用于定期回收资源

## Mesh，Material和Drawable

`Turbo`上层想要抽象出`Mesh`，`Material`和`Drawable`，其中：

* `Mesh`代表三维模型的各种顶点信息，包括：
  * 顶点位置数据
  * 顶点法线数据
  * 顶点索引数据
  * 顶点UV数据
  * 等等

* `Material`代表渲染流程，也就是代表着：`RenderPass`中的配置和渲染指令`Command`

* （非必要）`Drawable`或者叫`MeshDrawable`，是`Mesh`和`Material`的配对

---
`mermaid`图测试

```mermaid
graph TD;
    A-->B;
    A-->C;
    B-->D;
    C-->D;
```
