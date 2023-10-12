#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require // we need use [uint64_t] key word
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : require

#define KIND_SPHERE 0
#define KIND_CUBE 1

struct SPHERE
{
    vec3 center;
    float radius;
};

struct AABB
{
    vec3 minimum;
    vec3 maximum;
};

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

struct BottomLevelAccelerationStructureDeviceAddress
{
    uint64_t vertexDeviceAddress;
    uint64_t indexDeviceAddress;
    uint64_t aabbsDeviceAddress;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;

layout(buffer_reference, scalar) buffer Vertices
{
    Vertex v[];
};

layout(buffer_reference, scalar) buffer Indices
{
    ivec3 i[];
};

layout(buffer_reference, scalar) buffer AABBs
{
    AABB boxs[];
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

    // Vertices vertices = Vertices(blas_device_address.vertexDeviceAddress);
    // Indices indices = Indices(blas_device_address.indexDeviceAddress);
    AABBs aabbs = AABBs(blas_device_address.aabbsDeviceAddress);

    // ivec3 index = indices.i[gl_PrimitiveID];

    // Vertex v0 = vertices.v[index.x];
    // Vertex v1 = vertices.v[index.y];
    // Vertex v2 = vertices.v[index.z];

    // const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);

    // const vec3 position = v0.position * barycentrics.x + v1.position * barycentrics.y + v2.position * barycentrics.z;
    const vec3 world_position = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;

    AABB aabb = aabbs.boxs[gl_PrimitiveID];

    // Sphere data
    SPHERE sphere;
    sphere.center = (aabb.maximum + aabb.minimum) * 0.5;
    sphere.radius = ((aabb.maximum - aabb.minimum) * 0.5).x;

    vec3 diffuse_color = vec3(0, 1, 1);
    vec3 world_normal = normalize(world_position - sphere.center);
    if (gl_HitKindEXT == KIND_CUBE)
    {
        diffuse_color = vec3(1, 1, 0);

        vec3 absN = abs(world_normal);
        float maxC = max(max(absN.x, absN.y), absN.z);
        world_normal = (maxC == absN.x) ? vec3(sign(world_normal.x), 0, 0) : (maxC == absN.y) ? vec3(0, sign(world_normal.y), 0) : vec3(0, 0, sign(world_normal.z));
    }

    const vec3 light_dir = -normalize(vec3(1, 1, 1));

    // Diffuse
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

    HIT_PAY_LOAD.color = 0.3*diffuse + specular;
}
