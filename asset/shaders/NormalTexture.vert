#version 450

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(set = 0, binding = 0) uniform Matrixs
{
    mat4 m;
    mat4 v;
    mat4 p;
}
MATRIXS;
void main()
{
    gl_Position = MATRIXS.p * MATRIXS.v * MATRIXS.m * vec4(POSITION, 1.0);
}