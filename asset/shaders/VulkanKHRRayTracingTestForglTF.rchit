#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require // we need use [uint64_t] key word
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : require
// #extension GL_GOOGLE_include_directive : enable

#define M_PI 3.14159265

struct HitPayload
{
    uint seed;
    uint depth;
    vec3 rayOrigin;
    vec3 rayDirection;
    vec3 weight;
    vec3 color;
};

struct VERTEX
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

struct MATERIAL
{
    vec3 pbrBaseColorFactor;
    vec3 emissiveFactor;
};

struct BottomLevelAccelerationStructureDeviceAddress
{
    uint64_t vertexDeviceAddress;
    uint64_t indexDeviceAddress;
    uint64_t materialDeviceAddress;
};

layout(buffer_reference, scalar) buffer Vertices
{
    VERTEX v[];
};

layout(buffer_reference, scalar) buffer Indices
{
    ivec3 i[];
};

layout(buffer_reference, scalar) buffer Material
{
    MATERIAL material;
};

struct PushConstant
{
    int accumulateFrame;
    float intensity;
    float roughness;
};

layout(set = 0, binding = 3, scalar) buffer BottomLevelAccelerationStructureDeviceAddress_
{
    BottomLevelAccelerationStructureDeviceAddress blas_device_address[];
}
BLAS_DEVICE_ADDRESS;

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;
layout(push_constant) uniform PushConstant_
{
    PushConstant pc;
};

hitAttributeEXT vec2 attribs;

// Return the tangent and binormal from the incoming normal
void createCoordinateSystem(in vec3 N, out vec3 Nt, out vec3 Nb)
{
    if (abs(N.x) > abs(N.y))
        Nt = vec3(N.z, 0, -N.x) / sqrt(N.x * N.x + N.z * N.z);
    else
        Nt = vec3(0, -N.z, N.y) / sqrt(N.y * N.y + N.z * N.z);
    Nb = cross(N, Nt);
}

uint lcg(inout uint prev)
{
    uint LCG_A = 1664525u;
    uint LCG_C = 1013904223u;
    prev = (LCG_A * prev + LCG_C);
    return prev & 0x00FFFFFF;
}

// // Generate a random float in [0, 1) given the previous RNG state
float rnd(inout uint prev)
{
    return (float(lcg(prev)) / float(0x01000000));
}

vec3 samplingHemisphere(inout uint seed, in vec3 x, in vec3 y, in vec3 z)
{
    float r1 = rnd(seed);
    float r2 = rnd(seed);
    float sq = sqrt(r1);

    vec3 direction = vec3(cos(2 * M_PI * r2) * sq, sin(2 * M_PI * r2) * sq, sqrt(1. - r1));
    direction = direction.x * x + direction.y * y + direction.z * z;

    return direction;
}

void main()
{
    BottomLevelAccelerationStructureDeviceAddress blas_device_address = BLAS_DEVICE_ADDRESS.blas_device_address[gl_InstanceCustomIndexEXT];

    Vertices vertices = Vertices(blas_device_address.vertexDeviceAddress);
    Indices indices = Indices(blas_device_address.indexDeviceAddress);
    Material _material = Material(blas_device_address.materialDeviceAddress);

    ivec3 index = indices.i[gl_PrimitiveID];

    VERTEX v0 = vertices.v[index.x];
    VERTEX v1 = vertices.v[index.y];
    VERTEX v2 = vertices.v[index.z];

    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y); // 最近命中点重心坐标

    const vec3 position = v0.position * barycentrics.x + v1.position * barycentrics.y + v2.position * barycentrics.z;
    const vec3 world_position = vec3(gl_ObjectToWorldEXT * vec4(position, 1.0));

    const vec3 normal = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;
    // const vec3 world_normal = normalize(vec3(normal * gl_WorldToObjectEXT));
    const vec3 world_normal = normalize(vec3(gl_WorldToObjectEXT * vec4(normal, 1.0)));

    vec3 tangent, bitangent;
    createCoordinateSystem(world_normal, tangent, bitangent);

    vec3 rayOrigin = world_position;
    vec3 rayDirection = mix(samplingHemisphere(HIT_PAY_LOAD.seed, tangent, bitangent, world_normal), reflect(HIT_PAY_LOAD.rayDirection, -world_normal), 1 - pc.roughness);
    // vec3 rayDirection = normalize();

    const float cos_theta = dot(rayDirection, world_normal);
    // Probability density function of samplingHemisphere choosing this rayDirection
    const float p = cos_theta / M_PI;

    vec3 albedo = _material.material.pbrBaseColorFactor;
    vec3 emissive = _material.material.emissiveFactor * pc.intensity;

    vec3 BRDF = albedo / M_PI;

    HIT_PAY_LOAD.rayOrigin = rayOrigin;
    HIT_PAY_LOAD.rayDirection = rayDirection;
    HIT_PAY_LOAD.color = emissive;
    HIT_PAY_LOAD.weight = BRDF * cos_theta / p;

    // HIT_PAY_LOAD.color = (world_normal + 1) * 0.5;
    // HIT_PAY_LOAD.color = diffuse;
}
