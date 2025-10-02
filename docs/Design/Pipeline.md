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

## 创建 Compute 管线

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