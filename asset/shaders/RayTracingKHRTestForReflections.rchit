#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require // we need use [uint64_t] key word
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : require

struct HitPayload
{
    int depth;
    vec3 attenuation;
    vec3 color;
};

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

struct BottomLevelAccelerationStructureDeviceAddress
{
    uint64_t vertexDeviceAddress;
    uint64_t indexDeviceAddress;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;
layout(location = 1) rayPayloadEXT bool isShadowed;

layout(set = 0, binding = 0) uniform accelerationStructureEXT TOP_LEVEL_AS;

layout(buffer_reference, scalar) buffer Vertices
{
    Vertex v[];
};

layout(buffer_reference, scalar) buffer Indices
{
    ivec3 i[];
};

layout(set = 0, binding = 3, scalar) buffer BottomLevelAccelerationStructureDeviceAddress_
{
    BottomLevelAccelerationStructureDeviceAddress blas_device_address[];
}
BLAS_DEVICE_ADDRESS;

struct PushConstant
{
    int depth;
};

layout(push_constant) uniform PushConstant_
{
    PushConstant pc;
};

hitAttributeEXT vec2 attribs;

void main()
{
    BottomLevelAccelerationStructureDeviceAddress blas_device_address = BLAS_DEVICE_ADDRESS.blas_device_address[gl_InstanceCustomIndexEXT];

    Vertices vertices = Vertices(blas_device_address.vertexDeviceAddress);
    Indices indices = Indices(blas_device_address.indexDeviceAddress);

    ivec3 index = indices.i[gl_PrimitiveID];

    Vertex v0 = vertices.v[index.x];
    Vertex v1 = vertices.v[index.y];
    Vertex v2 = vertices.v[index.z];

    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);

    const vec3 position = v0.position * barycentrics.x + v1.position * barycentrics.y + v2.position * barycentrics.z;
    const vec3 world_position = vec3(gl_ObjectToWorldEXT * vec4(position, 1.0));

    const vec3 normal = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;
    const vec3 world_normal = normalize(vec3(normal * gl_WorldToObjectEXT));

    const vec3 light_dir = -normalize(vec3(1, 1, 1));

    // Diffuse
    const vec3 diffuse_color = vec3(1, 1, 1);
    float normalDotLight = max(dot(world_normal, light_dir), 0);
    vec3 diffuse = diffuse_color * normalDotLight;

    // Specular
    const float PI = 3.1415925;
    float shininess = 28.0;
    float energy_conservation = (2 + shininess) / (2 * PI);
    vec3 view_dir = normalize(-gl_WorldRayDirectionEXT);
    vec3 reflect_dir = normalize(reflect(-light_dir, world_normal));
    float specular_value = energy_conservation * pow(max(dot(view_dir, reflect_dir), 0), shininess);
    vec3 specular = vec3(specular_value);

    float attenuation = 1;

    // 只有光源照射到的表面处进行阴影追踪
    if (dot(world_normal, light_dir) > 0)
    {
        float t_min = 0.001;
        float t_max = 10000.0;
        vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT; // gl_HitTEXT is an alias of gl_RayTmaxEXT
        vec3 direction = light_dir;
        uint ray_flags = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
        isShadowed = true;
        traceRayEXT(TOP_LEVEL_AS,  // acceleration structure
                    ray_flags,     // rayFlags
                    0xFF,          // cullMask
                    0,             // sbtRecordOffset
                    0,             // sbtRecordStride
                    1,             // missIndex
                    origin.xyz,    // ray origin
                    t_min,         // ray min range
                    direction.xyz, // ray direction
                    t_max,         // ray max range
                    1              // payload (location = 1)
        );

        if (isShadowed)
        {
            attenuation = 0.3;
        }
    }

    bool is_reflection = true;
    const float attenuation_coefficient = 0.95;
    if (is_reflection && HIT_PAY_LOAD.depth < pc.depth)
    {
        vec3 origin = world_position;
        vec3 rayDir = reflect(gl_WorldRayDirectionEXT, world_normal);
        HIT_PAY_LOAD.attenuation *= attenuation_coefficient;

        HIT_PAY_LOAD.depth++;
        traceRayEXT(TOP_LEVEL_AS,       // acceleration structure
                    gl_RayFlagsNoneEXT, // rayFlags
                    0xFF,               // cullMask
                    0,                  // sbtRecordOffset
                    0,                  // sbtRecordStride
                    0,                  // missIndex
                    origin,             // ray origin
                    0.1,                // ray min range
                    rayDir,             // ray direction
                    100000.0,           // ray max range
                    0                   // payload (location = 0)
        );
        HIT_PAY_LOAD.depth--;
    }

    HIT_PAY_LOAD.color += (attenuation * (diffuse + specular)) * HIT_PAY_LOAD.attenuation;
}
