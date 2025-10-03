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