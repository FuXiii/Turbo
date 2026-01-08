# Pipeline 设计

## Vulkan Spec

>VUID-VkPipelineShaderStageCreateInfo-stage-00704
>
>If the geometryShader feature is not enabled, stage must not be VK_SHADER_STAGE_GEOMETRY_BIT

>VUID-VkPipelineShaderStageCreateInfo-stage-00705
>
>If the tessellationShader feature is not enabled, stage must not be VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT or VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT

>VUID-VkPipelineShaderStageCreateInfo-stage-02091
>
>If the meshShaders feature is not enabled, stage must not be VK_SHADER_STAGE_MESH_BIT_EXT

>VUID-VkPipelineShaderStageCreateInfo-stage-02092
>
>If the taskShaders feature is not enabled, stage must not be VK_SHADER_STAGE_TASK_BIT_EXT

>VUID-VkPipelineShaderStageCreateInfo-clustercullingShader-07813
>
>If the clustercullingShader feature is not enabled, stage must not be VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI

## 需求

简化 `Pipeline` 的创建，使用 `<<` 操作符拼凑管线配置。

```CXX
NPipleine pipeline;
pipeline << vertex_shader;
pipeline << fragment_shader;
pipeline << vertex_attributes;
pipeline << ...;
```

在某些情况下，其中有些配置是可选的。

目前有以下类型的管线：

* compute 计算管线
* graphics 图形管线
* ray tracing 光追管线

其中图形管线有两种模式：

* primitive shading 管线（图元渲染管线）
* mesh shading 管线（网格渲染管线）

## 指定 DescriptorSetLayout

如果指定了描述符布局，则使用用户指定的（需要检查兼容性）。如果未指定，则使用指定的着色器默认推算出来的描述符布局。

## 创建 Compute 管线

```CXX
// Provided by VK_VERSION_1_0
VkResult vkCreateComputePipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkComputePipelineCreateInfo*          pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines);
```

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkComputePipelineCreateInfo {
    VkStructureType                    sType;
    const void*                        pNext;
    VkPipelineCreateFlags              flags;
    VkPipelineShaderStageCreateInfo    stage;
    VkPipelineLayout                   layout;
    VkPipeline                         basePipelineHandle;
    int32_t                            basePipelineIndex;
} VkComputePipelineCreateInfo;
```

>VUID-VkComputePipelineCreateInfo-stage-00701
>
>The `stage` member of `stage` must be VK_SHADER_STAGE_COMPUTE_BIT

```cxx
// Provided by VK_VERSION_1_0
typedef enum VkPipelineShaderStageCreateFlagBits {
  // Provided by VK_VERSION_1_3
    VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT = 0x00000001,
  // Provided by VK_VERSION_1_3
    VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT = 0x00000002,
  // Provided by VK_EXT_subgroup_size_control
    VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT_EXT = VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT,
  // Provided by VK_EXT_subgroup_size_control
    VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT_EXT = VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT,
} VkPipelineShaderStageCreateFlagBits;
```

`VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT`指示`SubgroupSize`在`shader stage`可能会改变。

## 创建 Graphics 管线

```CXX
// Provided by VK_VERSION_1_0
VkResult vkCreateGraphicsPipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkGraphicsPipelineCreateInfo*         pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines);
```

```cxx
// Provided by VK_VERSION_1_0
typedef struct VkGraphicsPipelineCreateInfo {
    VkStructureType                                  sType;
    const void*                                      pNext;
    VkPipelineCreateFlags                            flags;
    uint32_t                                         stageCount;
    const VkPipelineShaderStageCreateInfo*           pStages;
    const VkPipelineVertexInputStateCreateInfo*      pVertexInputState;
    const VkPipelineInputAssemblyStateCreateInfo*    pInputAssemblyState;
    const VkPipelineTessellationStateCreateInfo*     pTessellationState;
    const VkPipelineViewportStateCreateInfo*         pViewportState;
    const VkPipelineRasterizationStateCreateInfo*    pRasterizationState;
    const VkPipelineMultisampleStateCreateInfo*      pMultisampleState;
    const VkPipelineDepthStencilStateCreateInfo*     pDepthStencilState;
    const VkPipelineColorBlendStateCreateInfo*       pColorBlendState;
    const VkPipelineDynamicStateCreateInfo*          pDynamicState;
    VkPipelineLayout                                 layout;
    VkRenderPass                                     renderPass;
    uint32_t                                         subpass;
    VkPipeline                                       basePipelineHandle;
    int32_t                                          basePipelineIndex;
} VkGraphicsPipelineCreateInfo;
```

### pStages

```cxx
// Provided by VK_VERSION_1_0
typedef struct VkPipelineShaderStageCreateInfo {
    VkStructureType                     sType;
    const void*                         pNext;
    VkPipelineShaderStageCreateFlags    flags;
    VkShaderStageFlagBits               stage;
    VkShaderModule                      module;
    const char*                         pName;
    const VkSpecializationInfo*         pSpecializationInfo;
} VkPipelineShaderStageCreateInfo;
```

主要有如下成员：

* module
* pSpecializationInfo
* pName（可以使用`main`作为默认值）

其中 `pSpecializationInfo` 可以抽象成键值对（id-value）。

```cxx
TShaderStage shader_stage;
shader_stage << compute_shader;//指定着色器
shader_stage << std::make_paire(0, 1.0f);//指定 specialization
shader_stage << std::make_paire(1, 2);//指定 specialization
shader_stage << std::make_paire(2, 3.0);//指定 specialization
shader_stage << std::make_paire(3, true);//指定 specialization
```

### pVertexInputState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineVertexInputStateCreateInfo {
    VkStructureType                             sType;
    const void*                                 pNext;
    VkPipelineVertexInputStateCreateFlags       flags;
    uint32_t                                    vertexBindingDescriptionCount;
    const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
    uint32_t                                    vertexAttributeDescriptionCount;
    const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
} VkPipelineVertexInputStateCreateInfo;
```

如果图形管线激活使用 `VK_DYNAMIC_STATE_VERTEX_INPUT_EXT` 的话，那么 `vertex input attribute` 和 `vertex input binding` 将会通过 `vkCmdSetVertexInputEXT` 设置，并且 `VkGraphicsPipelineCreateInfo::pVertexInputState` 将会被忽略。

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkVertexInputBindingDescription {
    uint32_t             binding;
    uint32_t             stride;
    VkVertexInputRate    inputRate;
} VkVertexInputBindingDescription;
```

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkVertexInputRate {
    VK_VERTEX_INPUT_RATE_VERTEX = 0,
    VK_VERTEX_INPUT_RATE_INSTANCE = 1,
} VkVertexInputRate;
```

渲染开始前需要通过 `vkCmdBindVertexBuffers` 绑定多个顶点缓存。

顶点着色器需要从一个或多个 `Buffer` 中获取顶点属性数据。

* 其中 `VkVertexInputBindingDescription::binding` 指的就是 `vkCmdBindVertexBuffers` 中绑定的第 `VkVertexInputBindingDescription::binding` 个 `Buffer` 。

* 其中 `VkVertexInputBindingDescription::stride` 指的就是对应绑定的 `Buffer` 中连续元素数据的长度。比如用户在 `Buffer` 中塞入多个格式为 `Vec3, Vec2, float` 的数据，则 `stride` 为这三个元素的长度之和。

* 其中 `VkVertexInputBindingDescription::inputRate` 用于指定缓存中的数据是给每个顶点用的还是给每个实例用的。

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkVertexInputAttributeDescription {
    uint32_t    location;
    uint32_t    binding;
    VkFormat    format;
    uint32_t    offset;
} VkVertexInputAttributeDescription;
```

每一次渲染(`DrawCall`)都需要指定顶点绑定数据：

```GLSL
#version 450 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec4 TANGENT;

...
```

* 其中 `VkVertexInputAttributeDescription::location` 指的就是着色器中对应的顶点数据位置(`location`)。
* 其中 `VkVertexInputAttributeDescription::binding` 与 `VkVertexInputBindingDescription::binding` 对应。

最多的 `location` 数量 `VkPhysicalDeviceLimits::maxVertexInputAttributes` 。

* 对于 `pVertexAttributeDescriptions` 中每个元素的 `binding` ， `pVertexBindingDescriptions` 中需要有与之对应的 `binding`
* `pVertexBindingDescriptions` 中的所有的元素的 `binding` 号是唯一的，不能重复。
* `pVertexAttributeDescriptions` 中的所有的元素的 `location` 号是唯一的，不能重复（ `binding` 是可以重复的）。

其中 `VkVertexInputAttributeDescription` 的 `location` , `format` 可以通过着色器推算出来（这有个问题，推算出来是个 `Vec3` 但 用户实际传递的参数是 `Vec2` 格式，这将会导致问题自动推算的结果与需要的不一致）。所以只有 `location` 是可以正确推算出来的。所以还需开放给用户自行配置。

其中 `VkVertexInputAttributeDescription::offset` 的偏移是相对于 `VkVertexInputBindingDescription::stride` 内的偏移，比如用户在 `Buffer` 中塞入多个格式为 `Vec3, Vec2, float` 的数据，则 `stride` 为这三个元素的长度之和。而对于 `Vec2` 的 `VkVertexInputAttributeDescription::offset` 为 `offset = sizeof(Vec3)` 。

一个 `Vertex input binding` 下可以有多个 `Vertex Attribute Description` 用于告诉 `GPU` 如何解析 `Vertex input binding` 对应 `Buffer` 中的数据。

这样的话绑定的 `Buffer` 既可以是单格式（比如全是 `Vec2` 或全是 `Vec3` 等），也可以是多格式（比如多个 `Vec3, Vec2, float` 等）。每个 `Buffer` 对应一个 `Vertex input binding` ，每个  `Vertex input binding` 可以配置多个 `Vertex Attribute Description` 用于确定如何解析 `Buffer` 中的数据。