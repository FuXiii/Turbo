# RenderPass

目前主要有 `RenderPass` 和 `动态渲染` 。从 `Vulkan 1.3` 版本开始 `动态渲染` 已并入核心，并且推荐使用 `动态渲染` ，将 `RenderPass` 归入到过时特性中。

大致两条思路：

1. 对 `RenderPass` 和 `动态渲染` 进行统一抽象，统一接口。这样只需要维护功能接口，具体由底层实现。
2. 并不改变现有接口，为`动态渲染`新开发单独的接口。

对于统一抽象和接口的方式，相当于在内部维护 `RenderPass` 和 `Framebuffer` 。该功能可行的前提是有完善的  `RenderPass` 和 `Framebuffer` 功能。所以先实现 `2` 再实现 `1` 。

在某些移动平台可能仅支持 `Vulkan 1.0`。所以老的 `RenderPass` 也不能放弃。
