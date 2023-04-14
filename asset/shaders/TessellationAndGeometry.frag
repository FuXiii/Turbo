#version 450 core

layout(location = 2) in float NOISE;

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 color = vec3(1.0, 0.1686, 0.9568) + vec3(NOISE);
    outColor = vec4(color, 1.0);
}