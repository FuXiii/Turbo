#version 450

layout(set = 1, binding = 0) uniform texture2D myTexture;
layout(set = 1, binding = 1) uniform sampler mySampler;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(1, 0, 0, 1);
}