#version 450 core
#extension GL_EXT_buffer_reference : enable

layout(set = 1, binding = 4) uniform texture2D overlapTexture2D;
layout(set = 3, binding = 4) uniform sampler otherSampler;

layout(r32ui, set = 3, binding = 0) uniform uimage2D image0_0;
layout(buffer_reference) buffer T1;
layout(set = 3, binding = 1, buffer_reference) buffer T1 {
   layout(offset = 0) int a[2]; // stride = 4 for std430, 16 for std140
   layout(offset = 32) int b;
   layout(offset = 48) T1  c[2]; // stride = 8 for std430, 16 for std140
   layout(offset = 80) T1  d;
} x;

layout(push_constant) uniform my_push_constants_frag
{
    bool isVertexShader;
}
my_push_constants_data;

//layout (push_constant, std430) uniform Block { int identity[32]; } pc;

layout(location = 2) in vec3 inColor;
layout(location = 3) in float inValue;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(1, 1, 1, 1);
}