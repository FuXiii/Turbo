#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require // we need use [uint64_t] key word
#extension GL_EXT_buffer_reference2 : require
#extension GL_EXT_scalar_block_layout : enable

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
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;

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

hitAttributeEXT vec2 attribs;

layout(shaderRecordEXT) buffer ShaderRecord_
{
    vec4 shaderRecord;
};

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

    const vec3 normal = v0.normal * barycentrics.x + v1.normal * barycentrics.y + v2.normal * barycentrics.z;

    HIT_PAY_LOAD.color = (shaderRecord.rgb + normal) * 0.5;
}