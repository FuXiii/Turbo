#version 450 core

layout(push_constant) uniform my_push_constants_frag
{
    bool isVertexShader;
}
my_push_constants_data;

layout(location = 2) in vec3 inColor;
layout(location = 3) in float inValue;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(1, 1, 1, 1);
}