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
        [&](TFrameGraph::TBuilder &builder, MyPassData &data) {
            // builder.Create<MyTexture>();
        },
        [](){ 
            vkCmdBeginRendePass(...);
            ...
            vkCmdEndRendePass();           
        }
    );

    fg.Setup();
    fg.Compile();
    fg.Execute();
    return 0;
}
```

`FrameGraph` 类似一个容器。

## Node

`Pass` 是节点， `Resource` 也是节点
