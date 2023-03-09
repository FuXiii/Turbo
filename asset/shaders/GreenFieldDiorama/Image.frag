#version 450
#extension GL_GOOGLE_include_directive : enable

layout(push_constant) uniform ShaderInputsStruct
{
    float time;
    float resolutionX;
    float resolutionY;
    float mouseX;
    float mouseY;
}
ShaderInputs;

layout(set = 0, binding = 0) uniform sampler2D iChannel0; // BufferA
layout(set = 0, binding = 1) uniform sampler2D iChannel1; // BufferB

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

#include "Common.glsl"

vec3 aces_tonemap(vec3 color)
{
    mat3 m1 = mat3(0.59719, 0.07600, 0.02840, 0.35458, 0.90834, 0.13383, 0.04823, 0.01566, 0.83777);
    mat3 m2 = mat3(1.60475, -0.10208, -0.00327, -0.53108, 1.10813, -0.07276, -0.07367, -0.00605, 1.07602);
    vec3 v = m1 * color;
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return pow(clamp(m2 * (a / b), 0.0, 1.0), vec3(1.0 / 2.2));
}

vec3 bloomTile(float lod, vec2 offset, vec2 uv)
{
    return textureLod(iChannel1, uv * exp2(-lod) + offset, 0.0).rgb;
}

vec3 getBloom(vec2 uv)
{
    vec3 blur = vec3(0.0);
    vec2 lOffsetFix = vec2(0.00025, 0.0005);
    blur = pow(bloomTile(2., vec2(0.0, 0.0) + lOffsetFix, uv), vec3(2.2)) + blur;
    blur = pow(bloomTile(3., vec2(0.3, 0.0) + lOffsetFix, uv), vec3(2.2)) * 1.3 + blur;
    blur = pow(bloomTile(4., vec2(0.0, 0.3) + lOffsetFix, uv), vec3(2.2)) * 1.6 + blur;
    blur = pow(bloomTile(5., vec2(0.1, 0.3) + lOffsetFix, uv), vec3(2.2)) * 1.9 + blur;
    blur = pow(bloomTile(6., vec2(0.2, 0.3) + lOffsetFix, uv), vec3(2.2)) * 2.2 + blur;

    return blur * BLOOM_RANGE;
}

void main()
{
    vec2 iResolution = vec2(ShaderInputs.resolutionX, ShaderInputs.resolutionY);
    vec2 fragCoord = vec2(uv.x * iResolution.x, uv.y * iResolution.y);
    vec2 uv = fragCoord.xy / iResolution.xy;

    vec4 col = texture(iChannel0, uv);
    // col.rgb += pow(getBloom(uv), vec3(2.2));
    col.rgb += getBloom(uv);
    col.rgb = aces_tonemap(col.rgb);

    outColor = vec4(col.rgb, 1.0);
    // fragColor = vec4(pow(col.aaa  * 0.1, vec3(2.0)), 1.0);
}