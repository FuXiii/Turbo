# FrameGraph设计

*注：在书写该文档之前已经存在一部分设计，位于`./docs/TurboDesign.drawio:FrameGraph`中。由于`Markdown`文件书写起来比较方便，所以使用该文档继续书写设计*

*注：该文档与`./docs/Design/FrameGraphAdvance.md`文档配合使用，`FrameGraphAdvance.md`文档是书写`Turbo`引擎如何基于`FrameGraph`驱动起来的设计文档*

## 更新日志

* 2022/12/30
  >
  >* 创建`FrameGraph`文档
  >* 创建`PassNode与RenderPass`章节
  >* 创建`FrameGraph::Subpass`章节
  >* 创建`FrameGraph::RenderPass`章节

* 2022/12/31
  >
  >* 更新`FrameGraph::Subpass`章节
  >* 更新`FrameGraph::RenderPass`章节

## PassNode与RenderPass

在`PassNode::Setup`阶段需要配置当前`PassNode`的各种`Subpass`，之后`Turbo`引擎会根据用户的配置创建`RenderPass`和`FrameBuffer`

```CXX
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

用户每调用一次`TFrameGraph::TBuilder::CreateSubpass()`就是声明一个`Subpass`，并且创建一个`Subpass`并添加进`PassNode`所代表的`RenderPass`中，而`Subpass`中有对应资源的读写配置

`Subpass`对应资源测操作有：

1. 读，`Subpass::Read(Resource)`
2. 写，`Subpass::Write(Resource)`

> `FrameGraph::RenderPass`转`Render::RenderPass`
>
> * `Subpass::Read(Resource)`，对应于`Vulkan`底层的`InputAttachment`
> * `Subpass::Write(Resource)`，对应于`Vulkan`底层的`ColorAttachment`或 `DepthStencilAttachment`，具体需要看是什么资源
>
> 对于当`Subpass::Write(Resource)`资源为`DepthStencil`纹理时，会有个问题，按照`Vulkan`标准每个`Subpass`只能绑定一个`DepthTexture`，而`Turbo`并不会制止用户往多个`DepthTexture`中写入，这会与`Vulkan`标准冲突，一种解决方案是当写入多个`DepthStencil`纹理时，只有最后一个深度模板纹理有效，`Turbo`输出警告信息

## FrameGraph::Subpass

由于原先是使用`TFrameGraph::TBuilder::Write(...)`和`TFrameGraph::TBuilder::Read(...)`函数，现由于资源的读写由`Subpass`负责，则`TFrameGraph::TBuilder`对于资源的读写改成私有，通过友元`Subpass`调用`TFrameGraph::TBuilder`对于资源的读写即可，所以`Subpass`中需要存有`TFrameGraph::TBuilder`引用

而对于资源的读写，同样要注册到`PassNode`对应的`RenderPass`中，所以

* `Subpass::Write()`的同时将向其中的`RenderPass`下对应的`Subpass`中注册资源
* `Subpass::Read()`的同时将向其中的`RenderPass`下对应的`Subpass`中注册资源

*考虑:是否将`Subpass::Write(...)`和`Subpass::Read(...)`设计成私有，并成为`TBuilder`的友元，这样只有在`PassNode::Setup`阶段可以调用`Subpass::Write(...)`和`Subpass::Read(...)`，如果设计成友元，其他私有成员也可以访问到了，也是个问题*

```CXX
//FrameGraph
class Subpass
{
    private:
        TBuilder& builder;
        RenderPass& renderPass;

        uint32_t subpass;//当前subpass在RenderPass中的index
        std::vector<Resource> writes;
        std::vector<Resource> reads;

    public:
        TSubpass();
        TSubpass(TBuilder& builder);

        Resource Write(Resource);
        Resource Read(Resource);
}

Resource Subpass::Write(Resource resource)
{
   Resource write_resource = builder.Write(resource);
   renderPass.Subpasses[subpass].writes.push_back(write_resource);

   return write_resource;
}

Resource Subpass::Read(Resource resource)
{
   Resource read_resource = builder.Read(resource);
   renderPass.Subpasses[subpass].reads.push_back(read_resource);

   return read_resource;
}
```

## FrameGraph::RenderPass

`Subpass`对于资源的读写，其实就是将对应的读写注册到`RenderPass`中，而一个`PassNode`代表一个`RenderPass`，所以一个`PassNode`中就应该存有一个`RenderPass`信息。

```CXX
class PassNode
{
    private:
        RenderPass renderPass;
};
```

而一个`RenderPass`下有多个`Subpass`

```CXX
class RenderPass
{
    private:
        std::vector<Subpass> subpasses;

    public:
        RenderPass();
       
        void AddSubpass(Subpass& subpass);
};
```
