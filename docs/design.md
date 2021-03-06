## Engine Feature

---

> 说明: **粗体**为重点首要考虑的

- 跨平台，目标支持平台：**Windows**，**Linux**，Mac，Andriod，Harmony

- 引擎核心不会依赖于任何窗口(为方便跨平台)，基于Surface(VkSurface)提供对外窗口的适配
  - 对于VkSurface需要开启相关扩展

- 会提供一个基本的窗口层，用于创建窗口，但该层不属于引擎核心，将会基于SDL2开发

- 引擎将基于**Vulkan**开发，重点实现Vulkan 1.0/1.2，未来可能会适配其他图形API

- 基于 ~~ECS~~和**FrameGraph**技术实现
  - 对于ECS, Turbo核心不会依附于任何树状结构层次数据接口，ECS属于上层建筑

- 支持**多线程渲染**

## Render Feature

---

- 支持实时光线追踪

- 支持全局光照

- 支持PBR模型

- 支持 gltf 模型

- 支持 KTX 格式纹理

## Memory And Resource Feature

---

- Vulkan 的资源和内存通过 VulkanMemoryAllocator(VMA)实现

## API Feature

---

- ~~Log 使用 spdlog库实现~~(Log功能不应该是核心功能里的)
- ~~ECS 使用 entt库实现~~(ECS功能也不是核心里的，待开发到高层时再展开)

## Shader 文件扩展

- ".vert": "vert", // for a vertex shader
- ".vs": "vert", // for a vertex shader
- ".frag": "frag", // for a fragment shader
- ".fs": "frag", // for a fragment shader
- ".gs": "geom", // for a geometry shader
- ".geom": "geom", // for a geometry shader
- ".comp": "comp", // for a compute shader
- ".tesc": "tesc", // for a tessellation control shader
- ".tese": "tese", // for a tessellation evaluation shader
- ".rgen": "rgen", // for a ray generation shader
- ".rint": "rint", // for a ray intersection shader
- ".rahit": "rahit", // for a ray any hit shader
- ".rchit": "rchit", // for a ray closest shader
- ".rmiss": "rmiss", // for a ray miss shader
- ".rcall": "rcall", // for a ray callable shader
- ".mesh": "mesh", // for a mesh shader
- ".task": "task" // for a task shader

# FrameGraph

- 不依赖任何第三方，包括`Turbo`，按照独立的一个模块实现，类似于一个容器

- 单一头文件包括`FrameGraph`的所有声明和定义
