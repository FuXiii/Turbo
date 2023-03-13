#version 450 core
#extension GL_GOOGLE_include_directive : enable
// #extension GL_ARB_shading_language_include : enable

#include "shader_include_test.glsl"

layout(location = 2) in vec3 inColor;
layout(location = 3) in float inValue;
layout(location = 0) out vec4 outColor;
void main()
{
    float time = inValue;
    float threshold = (sin(time) + 1) / 2;

    float step = 1.0 / 4.0;
    outColor = vec4(inColor, 1);

    if (threshold > (0 * step) && threshold < (1 * step))
    {
        outColor = vec4(inColor, 1);
    }

    if (threshold > (1 * step) && threshold < (2 * step))
    {
        outColor = Red();
    }

    if (threshold > (2 * step) && threshold < (3 * step))
    {
        outColor = Green();
    }

    if (threshold > (3 * step) && threshold < (4 * step))
    {
        outColor = Blue();
    }
}