#version 450

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec4 TANGENT;

layout(location = 1) out vec3 normal;

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 m;
    mat4 v;
    mat4 p;
}
Matrixs;

void main()
{
    gl_Position = Matrixs.p * Matrixs.v * Matrixs.m * vec4(POSITION, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(Matrixs.v * Matrixs.m)));
    normal = normalize(vec3(vec4(normalMatrix * NORMAL, 0.0)));
}