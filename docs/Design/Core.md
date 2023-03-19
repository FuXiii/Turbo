# 核心设计

## 更新日志

* 2023/3/19
  >
  >* 更新`Dynamic Rendering`章节
  >* 创建`Device Feature`章节

---

## Device Feature

随着`Vulkan`标准的发展，每次新版本的发布都会带来新的特性，这就需要`Turbo`维护自己的特性，提供维护自己的`TFeature`数据

经常需要使用的特性如下：

* `geometryShader`在`Vulkan1.0`
* `tessellationShader`在`Vulkan1.0`
* `sampleRateShading`在`Vulkan1.0`
* `depthClamp`在`Vulkan1.0`
* `depthBiasClamp`在`Vulkan1.0`
* `wideLines`在`Vulkan1.0`
* `fillModeNonSolid`在`Vulkan1.0`
* `timelineSemaphore`在`Vulkan1.2`
* `dynamicRendering`在`Vulkan1.3`

## Dynamic Rendering

对于`Vulkan`中的`Dynamic Rendering`主要如下函数

* `vkCmdBeginRenderingKHR`或者`vkCmdBeginRendering`
* `vkCmdEndRenderingKHR`或者`vkCmdEndRendering`

### Dynamic Rendering的Pipeline

对于`Dynamic Rendering`，不需要提前创建`Framebuffer`和`RenderPass`，对于`Framebuffer`和`RenderPass`的指定是在`vkCmdBeginRenderingKHR`构建时指定的，由于不需要`RenderPass`了，所以需要`Turbo`提供不需要`TRenderPass`和`Subpass`(`Vulkan`中对于`Dynamic Rendering`默认只支持一个`Subpass`的情况)的`GraphicsPipeline`版本。

对于提供不需要`TRenderPass`的`GraphicsPipeline`版本，目前有两种选择

1. 将当前`TGraphicsPipeline`提供一个没有`TRenderPass`和`Subpass`的版本，用于适配支持`Dynamic Rendering`的`Pipeline`(在此基础上可以考虑从`TGraphicsPipeline`派生一个`TRenderingPipeline`)

2. 从`TPipeline`派生出一个新的类`TRenderingPipeline`（内容和`TGraphicsPipeline`基本相同），专门用于适配支持`Dynamic Rendering`的`Pipeline`

先尝试采用方式`1`
