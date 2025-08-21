#version 450 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec4 TANGENT;

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 m;
    mat4 v;
    mat4 p;
}
Matrixs;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 uv;
layout(location = 3) out mat3 TBN;

void main()
{
    gl_Position = Matrixs.p * Matrixs.v * Matrixs.m * vec4(POSITION, 1.0);
    position = (Matrixs.m * vec4(POSITION, 1.0)).xyz;
    normal = normalize((Matrixs.m * vec4(NORMAL, 0.0)).xyz);
    vec3 tangent = normalize((Matrixs.m * vec4(TANGENT.xyz, 0.0)).xyz);
    vec3 bitangent = normalize(cross(normal, tangent));
    TBN = mat3(tangent, bitangent, normal);
    // TBN = mat3(tangent, normal, bitangent);
    // TBN = mat3(bitangent, normal, tangent);
    // TBN = mat3(bitangent, tangent, normal);
    // TBN = mat3(normal, tangent, bitangent);
    // TBN = mat3(normal, bitangent, tangent);
    uv = UV;
}