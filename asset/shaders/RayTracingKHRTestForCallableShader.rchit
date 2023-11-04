#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require // we need use [uint64_t] key word
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : require

struct HitPayload
{
    vec3 color;
};

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

struct CallablePayload
{
    vec3 color;
};

struct BottomLevelAccelerationStructureDeviceAddress
{
    uint64_t vertexDeviceAddress;
    uint64_t indexDeviceAddress;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;
layout(location = 0) callableDataEXT CallablePayload callablePayload;

layout(push_constant) uniform my_push_constants
{
    int callableSBTIndex;
}
my_push_constants_data;

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
    const vec3 world_normal = normalize(vec3(gl_WorldToObjectEXT * vec4(normal, 1.0)));

    const vec3 light_dir = -normalize(vec3(1, 1, 1));

    // Diffuse
    executeCallableEXT(my_push_constants_data.callableSBTIndex, 0);

    const vec3 diffuse_color = callablePayload.color;
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

    HIT_PAY_LOAD.color = diffuse + specular;
}
