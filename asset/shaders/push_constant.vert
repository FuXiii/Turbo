#version 450 core

layout(push_constant) uniform my_push_constants_vertex
{
    float a;
    float b;
    float c;
    bool isVertexShader;
}
my_push_constants_data;

layout(set = 0, binding = 0) uniform bufferVals
{
    float value;
}
myBufferVals;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) out vec3 outColor;
layout(location = 3) out float outValue;

void main()
{
    gl_Position = vec4(pos, 1);
    outColor = color;
    if (my_push_constants_data.isVertexShader)
    {
        outColor = vec3(my_push_constants_data.a * color.r, my_push_constants_data.b * color.g, my_push_constants_data.c * color.b);
    }
    outValue = myBufferVals.value;
}