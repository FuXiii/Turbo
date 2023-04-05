#version 450

layout(set = 0, binding = 1) uniform texture2D myTexture;
layout(set = 0, binding = 2) uniform sampler mySampler;
layout(set = 0, binding = 3) uniform MyBuffer
{
    float value;
    vec3 cameraPos;
}
MYBUFFER;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(1, 0, 0, 1);
}