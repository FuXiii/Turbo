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

### VkPipelineShaderStageCreateInfo* pStages

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

### VkPipelineVertexInputStateCreateInfo* pVertexInputState

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

> 如果图形管线激活使用 `VK_DYNAMIC_STATE_VERTEX_INPUT_EXT` 的话，那么 `vertex input attribute` 和 `vertex input binding` 将会通过 `vkCmdSetVertexInputEXT` 设置，并且 `VkGraphicsPipelineCreateInfo::pVertexInputState` 将会被忽略。

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

#### VertexInput API

`Vertex Attribute` 可以被多个 `VertexBinding`

```CXX
class VertexAtrribute
{
private:
    uint32_t location=0;
    //uint32_t binding=0;//NOTE: binding 可通过传入哪一个 VertexBinding 推导确定，所以该成员变量可不用手动设置。
    VkFormat format=0;
    uint32_t offset=0;

public:
    VertexAtrribute()=default;
    VertexAtrribute(uint32_t location, /*uint32_t binding,*/ VkFormat format, uint32_t offset);
};

class VertexBinding
{
private:
    //uint32_t binding=0;//NOTE: binding 可通过传入 VertexInput 推导确定，所以该成员变量可不用手动设置。
    uint32_t stride=0;
    VkVertexInputRate inputRate=VkVertexInputRate::VERTEX;

    std::vector<VertexAtrribute> vertexAtrributes;// NOTE: 存有多个 VertexAtrribute
    
public:
    VertexBinding()=default;
    VertexBinding(/*uint32_t binding, */uint32_t stride, VkVertexInputRate inputRate)；
};

class VertexInput
{
private:
    std::unordered_map<uint32_t/*NOTE: 对应 VkVertexInputAttributeDescription::binding*/, VertexBinding> VertexBindingMap;
};

struct Vertex
{
    vec3 vertex;
    vec2 uv;
    vec3 normal;
};

struct VertexOther
{
    vec4 color;
};

VertexAtrribute vertex_att(0, VK_FORMAT_R32G32B32, offsizeof(Vertex, vertex));
VertexAtrribute uv_att(1, VK_FORMAT_R32G32, offsizeof(Vertex, uv));
VertexAtrribute normal_att(2, VK_FORMAT_R32G32B32, offsizeof(Vertex, normal));
VertexAtrribute color_att(3, VK_FORMAT_R32G32B32A32, offsizeof(VertexOther, color));

VertexBinding vb(sizeof(Vertex),VkVertexInputRate::VERTEX);
vb << vertex_att;//底层应该调用 VertexBinding::AddAtrribute(...)
vb << uv_att;//底层应该调用 VertexBinding::AddAtrribute(...)
vb << normal_att;//底层应该调用 VertexBinding::AddAtrribute(...)

VertexBinding vb_other(sizeof(VertexOther),VkVertexInputRate::VERTEX);
vb_other << color_att;//底层应该调用 VertexBinding::AddAtrribute(...)

VertexInput vi;
vi.Add(0/*对应 VkVertexInputAttributeDescription::binding*/, vb);
vi.Add(1/*对应 VkVertexInputAttributeDescription::binding*/, vb_other);
//或
vi[0] = vb;
vi[1] = vb_other;
```

>* dynamic `VK_DYNAMIC_STATE_VERTEX_INPUT_EXT` :
>
>```CXX
>// Provided by VK_EXT_shader_object, VK_EXT_vertex_input_dynamic_state
>void vkCmdSetVertexInputEXT(
>    VkCommandBuffer                             commandBuffer,
>    uint32_t                                    vertexBindingDescriptionCount,
>    const VkVertexInputBindingDescription2EXT*  pVertexBindingDescriptions,
>    uint32_t                                    vertexAttributeDescriptionCount,
>    const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions);
>```

### VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState

```CXX
typedef struct VkPipelineInputAssemblyStateCreateInfo {
    VkStructureType                            sType;
    const void*                                pNext;
    VkPipelineInputAssemblyStateCreateFlags    flags;
    VkPrimitiveTopology                        topology;
    VkBool32                                   primitiveRestartEnable;
} VkPipelineInputAssemblyStateCreateInfo;
```

* `flags` 目前没有明确用途规定。
* `primitiveRestartEnable` 用于指代某一特殊顶点索引值是否解析成重新开始图元绘制。只有在索引绘制时该成员才被采用（vkCmdDrawIndexed，vkCmdDrawMultiIndexedEXT 和 vkCmdDrawIndexedIndirect）。特殊值:

> * VK_INDEX_TYPE_UINT32 -> 0xFFFFFFFF
> * VK_INDEX_TYPE_UINT16 -> 0xFFFF
> * VK_INDEX_TYPE_UINT8 -> 0xFF

正常情况下 `"list"` 图元不支持重开图元绘制，除非开启了如下 `feature`：

> * primitiveTopologyPatchListRestart (用于 VK_PRIMITIVE_TOPOLOGY_PATCH_LIST)
> * primitiveTopologyListRestart (用于所有其他的 list 拓扑)

* `topology` 代表图元的拓扑结构。

```CXX
typedef enum VkPrimitiveTopology {
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST = 0,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST = 1,
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP = 2,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 3,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP = 4,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN = 5,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY = 6,
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY = 7,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY = 8,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY = 9,
    VK_PRIMITIVE_TOPOLOGY_PATCH_LIST = 10,
} VkPrimitiveTopology;
```

>* dynamic `VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY` :
>
>```CXX
>// Provided by VK_VERSION_1_3
>void vkCmdSetPrimitiveTopology(
>    VkCommandBuffer                             commandBuffer,
>    VkPrimitiveTopology                         primitiveTopology);
>```
>
>* dynamic `VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE`

If the `VK_EXT_extended_dynamic_state3` extension is enabled, it can be `NULL` if the pipeline is created with both `VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE`, and `VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY` dynamic states set and `dynamicPrimitiveTopologyUnrestricted` is `VK_TRUE`.

* 如果包含`mesh`着色器的话 `pInputAssemblyState` 将会被忽略。

### VkPipelineTessellationStateCreateInfo* pTessellationState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineTessellationStateCreateInfo {
    VkStructureType                           sType;
    const void*                               pNext;
    VkPipelineTessellationStateCreateFlags    flags;
    uint32_t                                  patchControlPoints;
} VkPipelineTessellationStateCreateInfo;
```

* `flags` 目前没有明确用途规定。
* `patchControlPoints` 每一个 `patch` 的控制点数量。

>* dynamic `VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT`:
>
>```CXX
>// Provided by VK_EXT_extended_dynamic_state2, VK_EXT_shader_object
>void vkCmdSetPatchControlPointsEXT(
>    VkCommandBuffer                             commandBuffer,
>    uint32_t                                    patchControlPoints);
>```

### VkPipelineViewportStateCreateInfo* pViewportState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineViewportStateCreateInfo {
    VkStructureType                       sType;
    const void*                           pNext;
    VkPipelineViewportStateCreateFlags    flags;
    uint32_t                              viewportCount;
    const VkViewport*                     pViewports;
    uint32_t                              scissorCount;
    const VkRect2D*                       pScissors;
} VkPipelineViewportStateCreateInfo;
```

* 如果没有激活 `multiViewport` 特性的话 `viewportCount` 必须是 `1`
* 如果没有激活 `multiViewport` 特性的话 `scissorCount` 必须是 `1`
* If `scissorCount` and `viewportCount` are both not dynamic, then `scissorCount` and `viewportCount` must be identical

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkViewport {
    float    x;
    float    y;
    float    width;
    float    height;
    float    minDepth;
    float    maxDepth;
} VkViewport;
```

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkRect2D {
    VkOffset2D    offset;
    VkExtent2D    extent;
} VkRect2D;
```

>* dynamic `VK_DYNAMIC_STATE_VIEWPORT`:
>
>```CXX
>// Provided by VK_VERSION_1_0
>void vkCmdSetViewport(
>    VkCommandBuffer                             commandBuffer,
>    uint32_t                                    firstViewport,
>    uint32_t                                    viewportCount,
>    const VkViewport*                           pViewports);
>```

>* dynamic `VK_DYNAMIC_STATE_SCISSOR`:
>
>```CXX
>// Provided by VK_VERSION_1_0
>void vkCmdSetScissor(
>    VkCommandBuffer                             commandBuffer,
>    uint32_t                                    firstScissor,
>    uint32_t                                    scissorCount,
>    const VkRect2D*                             pScissors);
>```

* `multiViewport` feature

### VkPipelineRasterizationStateCreateInfo* pRasterizationState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineRasterizationStateCreateInfo {
    VkStructureType                            sType;
    const void*                                pNext;
    VkPipelineRasterizationStateCreateFlags    flags;
    VkBool32                                   depthClampEnable;
    VkBool32                                   rasterizerDiscardEnable;
    VkPolygonMode                              polygonMode;
    VkCullModeFlags                            cullMode;
    VkFrontFace                                frontFace;
    VkBool32                                   depthBiasEnable;
    float                                      depthBiasConstantFactor;
    float                                      depthBiasClamp;
    float                                      depthBiasSlopeFactor;
    float                                      lineWidth;
} VkPipelineRasterizationStateCreateInfo;
```

* 如果激活 `depthClampEnable` 则深度值将会限制在 `VkViewport::minDepth` 和 `VkViewport::maxDepth` 之间。如果激活了 `depthClampControl` 特性，并且 `VkPipelineViewportDepthClampControlCreateInfoEXT::depthClampMode` 是 `VK_DEPTH_CLAMP_MODE_USER_DEFINED_RANGE_EXT`的话，则其范围为使用 `VkDepthClampRangeEXT` 限制：

```CXX
// Provided by VK_EXT_depth_clamp_control
typedef struct VkPipelineViewportDepthClampControlCreateInfoEXT {
    VkStructureType                sType;
    const void*                    pNext;
    VkDepthClampModeEXT            depthClampMode;
    const VkDepthClampRangeEXT*    pDepthClampRange;
} VkPipelineViewportDepthClampControlCreateInfoEXT;

// Provided by VK_EXT_depth_clamp_control
typedef enum VkDepthClampModeEXT {
    VK_DEPTH_CLAMP_MODE_VIEWPORT_RANGE_EXT = 0,
    VK_DEPTH_CLAMP_MODE_USER_DEFINED_RANGE_EXT = 1,
} VkDepthClampModeEXT;

// Provided by VK_EXT_depth_clamp_control
typedef struct VkDepthClampRangeEXT {
    float    minDepthClamp;
    float    maxDepthClamp;
} VkDepthClampRangeEXT;
```

如果 `VkPhysicalDeviceDepthClampZeroOneFeaturesKHR::depthClampZeroOne` 特性激活了的话：

* 如果深度附件的深度格式是浮点数的话，并且开启 `VK_EXT_depth_range_unrestricted` 扩展的话，则深度值不会发生改变。
* 否则深度值会被限制在[0, 1]之间。

```CXX
// Provided by VK_KHR_depth_clamp_zero_one
typedef struct VkPhysicalDeviceDepthClampZeroOneFeaturesKHR {
    VkStructureType    sType;
    void*              pNext;
    VkBool32           depthClampZeroOne;
} VkPhysicalDeviceDepthClampZeroOneFeaturesKHR;
```

否则

* 如果深度不在 `VkViewport::minDepth` 和 `VkViewport::maxDepth` 之间，则深度值是未定义的。
* 如果深度附件是浮点格式并且深度值不是 [0, 1] 之间，则深度值是未定义的。

* `rasterizerDiscardEnable` 用于控制图元是否在光栅化阶段之前立马剔除。但会执行光栅化之前的阶段。
* `polygonMode` 是三角形渲染模式。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkPolygonMode {
    VK_POLYGON_MODE_FILL = 0,
    VK_POLYGON_MODE_LINE = 1,
    VK_POLYGON_MODE_POINT = 2,
  // Provided by VK_NV_fill_rectangle
    VK_POLYGON_MODE_FILL_RECTANGLE_NV = 1000153000,
} VkPolygonMode;
```

* `cullMode` 指定要剔除的三角正反面。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkCullModeFlagBits {
    VK_CULL_MODE_NONE = 0,
    VK_CULL_MODE_FRONT_BIT = 0x00000001,
    VK_CULL_MODE_BACK_BIT = 0x00000002,
    VK_CULL_MODE_FRONT_AND_BACK = 0x00000003,
} VkCullModeFlagBits;
```

* `frontFace` 指定正面三角面的点顺序，用于确定正反面。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkFrontFace {
    VK_FRONT_FACE_COUNTER_CLOCKWISE = 0,//逆时针
    VK_FRONT_FACE_CLOCKWISE = 1,//顺时针
} VkFrontFace;
```

* `depthBiasEnable` 控制是否偏移片元深度值。
* `depthBiasConstantFactor` 用于控制每一个片元的深度值加上该固定值（实际上会作为系数与深度值相乘后相加）。
* `depthBiasClamp` 指定一个片元的最大或最小的深度偏移。当大于`0`取最小值，当小于`0`取取最大值，等于`0`时不做改变（具体可看深度偏移计算公式）。
* `depthBiasSlopeFactor` 用于指定深度偏移计算的斜率。
* `lineWidth` 线宽。

### VkPipelineMultisampleStateCreateInfo* pMultisampleState

```CXX
typedef uint32_t VkSampleMask;

// Provided by VK_VERSION_1_0
typedef struct VkPipelineMultisampleStateCreateInfo {
    VkStructureType                          sType;
    const void*                              pNext;
    VkPipelineMultisampleStateCreateFlags    flags;
    VkSampleCountFlagBits                    rasterizationSamples;
    VkBool32                                 sampleShadingEnable;
    float                                    minSampleShading;
    const VkSampleMask*                      pSampleMask;
    VkBool32                                 alphaToCoverageEnable;
    VkBool32                                 alphaToOneEnable;
} VkPipelineMultisampleStateCreateInfo;
```

* `flags` 目前没有明确用途规定。
* `rasterizationSamples` 用于指定光栅化阶段的多采样点数量。
* `sampleShadingEnable` 用于激活使用多采样着色。开启后片元着色器将会进行多采样。
* `minSampleShading` 如果`sampleShadingEnable`开启则用于设置多采样的最小分子。必须在 [0, 1] 之间。用于控制 [1, rasterizationSamples] 数量的采样点的。
* `pSampleMask` 指向数量为 `rasterizationSamples` 个 `VkSampleMask` 数组。用于采样遮罩测试（与片元的 coverage mask 进行测试）。
* `alphaToCoverageEnable` 用于指示透明通道是否用于覆盖计算。
* `alphaToOneEnable` 用于控制输出的透明通道是否为 `1`。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkSampleCountFlagBits {
    VK_SAMPLE_COUNT_1_BIT = 0x00000001,
    VK_SAMPLE_COUNT_2_BIT = 0x00000002,
    VK_SAMPLE_COUNT_4_BIT = 0x00000004,
    VK_SAMPLE_COUNT_8_BIT = 0x00000008,
    VK_SAMPLE_COUNT_16_BIT = 0x00000010,
    VK_SAMPLE_COUNT_32_BIT = 0x00000020,
    VK_SAMPLE_COUNT_64_BIT = 0x00000040,
} VkSampleCountFlagBits;
```

### VkPipelineDepthStencilStateCreateInfo* pDepthStencilState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineDepthStencilStateCreateInfo {
    VkStructureType                           sType;
    const void*                               pNext;
    VkPipelineDepthStencilStateCreateFlags    flags;
    VkBool32                                  depthTestEnable;
    VkBool32                                  depthWriteEnable;
    VkCompareOp                               depthCompareOp;
    VkBool32                                  depthBoundsTestEnable;
    VkBool32                                  stencilTestEnable;
    VkStencilOpState                          front;
    VkStencilOpState                          back;
    float                                     minDepthBounds;
    float                                     maxDepthBounds;
} VkPipelineDepthStencilStateCreateInfo;
```

* `flags` 目前没有明确用途规定。
* `depthTestEnable` 是否开启深度测试。
* `depthWriteEnable` 是否开启深度写入。当 `depthTestEnable` 开启后深度写入才有效。
* `depthCompareOp` 深度测试的比较操作。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkCompareOp {
    VK_COMPARE_OP_NEVER = 0,
    VK_COMPARE_OP_LESS = 1,
    VK_COMPARE_OP_EQUAL = 2,
    VK_COMPARE_OP_LESS_OR_EQUAL = 3,
    VK_COMPARE_OP_GREATER = 4,
    VK_COMPARE_OP_NOT_EQUAL = 5,
    VK_COMPARE_OP_GREATER_OR_EQUAL = 6,
    VK_COMPARE_OP_ALWAYS = 7,
} VkCompareOp;
```

* `depthBoundsTestEnable` 开启深度范围测试（深度值被限制在了 `minDepthBounds` 和 `maxDepthBounds` 之间，超过这个范围会被限制在 `0`）。
* `stencilTestEnable` 开启模板测试。
* `front` 和 `back` 用于配置模板测试。

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkStencilOpState {
    VkStencilOp    failOp;
    VkStencilOp    passOp;
    VkStencilOp    depthFailOp;
    VkCompareOp    compareOp;
    uint32_t       compareMask;
    uint32_t       writeMask;
    uint32_t       reference;
} VkStencilOpState;

```CXX
if((reference & compareMask) compareOp (stencilBufferValue & compareMask))
{
    //通过模板测试
    //写入的模板值根据 passOp 配置
}
else
{
    //未通过模板测试
    //写入的模板值根据 failOp 配置
}
```

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkStencilOp {
    VK_STENCIL_OP_KEEP = 0,
    VK_STENCIL_OP_ZERO = 1,
    VK_STENCIL_OP_REPLACE = 2,
    VK_STENCIL_OP_INCREMENT_AND_CLAMP = 3,
    VK_STENCIL_OP_DECREMENT_AND_CLAMP = 4,
    VK_STENCIL_OP_INVERT = 5,
    VK_STENCIL_OP_INCREMENT_AND_WRAP = 6,
    VK_STENCIL_OP_DECREMENT_AND_WRAP = 7,
} VkStencilOp;
```

* `minDepthBounds` 深度范围测试的最小值。
* `maxDepthBounds` 深度范围测试的最大值。

### VkPipelineColorBlendStateCreateInfo* pColorBlendState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineColorBlendStateCreateInfo {
    VkStructureType                               sType;
    const void*                                   pNext;
    VkPipelineColorBlendStateCreateFlags          flags;
    VkBool32                                      logicOpEnable;
    VkLogicOp                                     logicOp;
    uint32_t                                      attachmentCount;
    const VkPipelineColorBlendAttachmentState*    pAttachments;
    float                                         blendConstants[4];
} VkPipelineColorBlendStateCreateInfo;
```

* `logicOpEnable` 控制是否进行逻辑操作。
* `logicOp` 控制使用何种逻辑操作。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkLogicOp {
    VK_LOGIC_OP_CLEAR = 0,
    VK_LOGIC_OP_AND = 1,
    VK_LOGIC_OP_AND_REVERSE = 2,
    VK_LOGIC_OP_COPY = 3,
    VK_LOGIC_OP_AND_INVERTED = 4,
    VK_LOGIC_OP_NO_OP = 5,
    VK_LOGIC_OP_XOR = 6,
    VK_LOGIC_OP_OR = 7,
    VK_LOGIC_OP_NOR = 8,
    VK_LOGIC_OP_EQUIVALENT = 9,
    VK_LOGIC_OP_INVERT = 10,
    VK_LOGIC_OP_OR_REVERSE = 11,
    VK_LOGIC_OP_COPY_INVERTED = 12,
    VK_LOGIC_OP_OR_INVERTED = 13,
    VK_LOGIC_OP_NAND = 14,
    VK_LOGIC_OP_SET = 15,
} VkLogicOp;
```

* `attachmentCount` 指定 `pAttachments` 的数量。
* `pAttachments` 控制每一个颜色附件的颜色混合。

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineColorBlendAttachmentState {
    VkBool32                 blendEnable;
    VkBlendFactor            srcColorBlendFactor;
    VkBlendFactor            dstColorBlendFactor;
    VkBlendOp                colorBlendOp;
    VkBlendFactor            srcAlphaBlendFactor;
    VkBlendFactor            dstAlphaBlendFactor;
    VkBlendOp                alphaBlendOp;
    VkColorComponentFlags    colorWriteMask;
} VkPipelineColorBlendAttachmentState;
```

* `blendEnable` 是否开启混合测试。
* `srcColorBlendFactor` 源颜色混合系数。
* `dstColorBlendFactor` 目标颜色混合系数。
* `colorBlendOp` 颜色混合操作。
* `srcAlphaBlendFactor` 源透明混合系数。
* `dstAlphaBlendFactor` 目标透明混合系数。
* `alphaBlendOp` 透明混合操作。
* `colorWriteMask` 颜色写入遮罩。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkBlendFactor {
    VK_BLEND_FACTOR_ZERO = 0,
    VK_BLEND_FACTOR_ONE = 1,
    VK_BLEND_FACTOR_SRC_COLOR = 2,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR = 3,
    VK_BLEND_FACTOR_DST_COLOR = 4,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR = 5,
    VK_BLEND_FACTOR_SRC_ALPHA = 6,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA = 7,
    VK_BLEND_FACTOR_DST_ALPHA = 8,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA = 9,
    VK_BLEND_FACTOR_CONSTANT_COLOR = 10,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = 11,
    VK_BLEND_FACTOR_CONSTANT_ALPHA = 12,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA = 13,
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE = 14,
    VK_BLEND_FACTOR_SRC1_COLOR = 15,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR = 16,
    VK_BLEND_FACTOR_SRC1_ALPHA = 17,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA = 18,
} VkBlendFactor;
```

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkBlendOp {
    VK_BLEND_OP_ADD = 0,
    VK_BLEND_OP_SUBTRACT = 1,
    VK_BLEND_OP_REVERSE_SUBTRACT = 2,
    VK_BLEND_OP_MIN = 3,
    VK_BLEND_OP_MAX = 4,
} VkBlendOp;
```

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkColorComponentFlagBits {
    VK_COLOR_COMPONENT_R_BIT = 0x00000001,
    VK_COLOR_COMPONENT_G_BIT = 0x00000002,
    VK_COLOR_COMPONENT_B_BIT = 0x00000004,
    VK_COLOR_COMPONENT_A_BIT = 0x00000008,
} VkColorComponentFlagBits;
```

* `blendConstants` 固定颜色混合。比如使用 `VK_BLEND_FACTOR_CONSTANT_COLOR` 时就会用到 `blendConstants` 。其为 `4` 个 `float` 值，分别对应 `RGBA` 各分量。

### VkPipelineDynamicStateCreateInfo* pDynamicState

```CXX
// Provided by VK_VERSION_1_0
typedef struct VkPipelineDynamicStateCreateInfo {
    VkStructureType                      sType;
    const void*                          pNext;
    VkPipelineDynamicStateCreateFlags    flags;
    uint32_t                             dynamicStateCount;
    const VkDynamicState*                pDynamicStates;
} VkPipelineDynamicStateCreateInfo;
```

* `dynamicStateCount` 指示 `pDynamicStates` 中的元素数量。
* `pDynamicStates` 为 `VkDynamicState` 的数组（元素不能重复）。

```CXX
// Provided by VK_VERSION_1_0
typedef enum VkDynamicState {
    VK_DYNAMIC_STATE_VIEWPORT = 0,
    VK_DYNAMIC_STATE_SCISSOR = 1,
    VK_DYNAMIC_STATE_LINE_WIDTH = 2,
    VK_DYNAMIC_STATE_DEPTH_BIAS = 3,
    VK_DYNAMIC_STATE_BLEND_CONSTANTS = 4,
    VK_DYNAMIC_STATE_DEPTH_BOUNDS = 5,
    VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK = 6,
    VK_DYNAMIC_STATE_STENCIL_WRITE_MASK = 7,
    VK_DYNAMIC_STATE_STENCIL_REFERENCE = 8,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_CULL_MODE = 1000267000,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_FRONT_FACE = 1000267001,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY = 1000267002,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT = 1000267003,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT = 1000267004,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE = 1000267005,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE = 1000267006,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE = 1000267007,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_DEPTH_COMPARE_OP = 1000267008,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE = 1000267009,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE = 1000267010,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_STENCIL_OP = 1000267011,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE = 1000377001,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE = 1000377002,
  // Provided by VK_VERSION_1_3
    VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE = 1000377004,
  // Provided by VK_VERSION_1_4
    VK_DYNAMIC_STATE_LINE_STIPPLE = 1000259000,
} VkDynamicState;
```

* `VK_DYNAMIC_STATE_VIEWPORT` 将会忽略 `VkPipelineViewportStateCreateInf::pViewports` 并且必须在绘制命令之前调用 `vkCmdSetViewport` 。
* `VK_DYNAMIC_STATE_SCISSOR` 将会忽略 `VkPipelineViewportStateCreateInfo::pScissors` 并且必须在绘制命令之前调用 `vkCmdSetViewport` 。
* `VK_DYNAMIC_STATE_LINE_WIDTH` 将会忽略 `VkPipelineRasterizationStateCreateInfo::lineWidth` 并且必须在绘制命令之前调用 `vkCmdSetLineWidth` 。
* `VK_DYNAMIC_STATE_DEPTH_BIAS` 将会忽略 `VkPipelineRasterizationStateCreateInfo::depthBiasConstantFactor/depthBiasClamp/depthBiasSlopeFactor` 和其 `pNext` 下的 `VkDepthBiasRepresentationInfoEXT` ，并且必须在绘制命令之前调用 `vkCmdSetDepthBias`或者`vkCmdSetDepthBias2EXT` 。
* `VK_DYNAMIC_STATE_BLEND_CONSTANTS` 将会忽略 `VkPipelineColorBlendStateCreateInfo::blendConstants` 并且必须在绘制命令之前调用 `vkCmdSetBlendConstants`。
