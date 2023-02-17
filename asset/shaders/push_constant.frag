#version 450 core

layout(push_constant) uniform my_push_constants_frag
{
    float a;
    float b;
    float c;
    bool isVertexShader;
}
my_push_constants_data;

layout(location = 2) in vec3 inColor;
layout(location = 3) in float inValue;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = inValue * vec4(inColor, 1);

    if (!my_push_constants_data.isVertexShader)
    {
        outColor = my_push_constants_data.a * vec4(inColor, 1);
    }
}