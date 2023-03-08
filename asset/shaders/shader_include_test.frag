#version 450 core
#extension GL_GOOGLE_include_directive : enable
//#extension GL_ARB_shading_language_include : enable

#include "shader_include_test.glsl"

layout(location = 2) in vec3 inColor;
layout(location = 3) in float inValue;
layout(location = 0) out vec4 outColor;
void main()
{
    outColor = vec4(inColor, 1);
}