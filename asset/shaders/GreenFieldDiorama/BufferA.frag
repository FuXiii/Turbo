#version 450
// main raymarching
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

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor; // BufferA

#include "Common.glsl"

mat3 setCamera(in vec3 ro, in vec3 ta, float cr)
{
    vec3 cw = normalize(ta - ro);
    vec3 cp = vec3(sin(cr), cos(cr), 0.0);
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = (cross(cu, cw));
    return mat3(cu, cv, cw);
}

void main()
{
    float iTime = ShaderInputs.time;
    vec2 iMouse = vec2(ShaderInputs.mouseX, ShaderInputs.mouseY);
    vec2 iResolution = vec2(ShaderInputs.resolutionX, ShaderInputs.resolutionY);
    vec2 fragCoord = vec2(uv.x * iResolution.x, uv.y * iResolution.y);
    // vec2 fragCoord = vec2(uv.x * iResolution.x, (1 - uv.y) * iResolution.y);

    gTime = ShaderInputs.time;

    vec2 mo = iMouse.xy / iResolution.xy;
    // mo.y += 0.2;
    float time = 23.5 + ShaderInputs.time * 1.5;
    // float time = 23.5;

    // camera
    vec3 ro = vec3(4.6 * cos(0.1 * time + 12.0 * mo.x), 1.2 + 3.0 * mo.y, 4.6 * sin(0.1 * time + 12.0 * mo.x));
    vec3 ta = vec3(0.0, 0.14, 0.0);
    // camera-to-world transformation
    mat3 ca = setCamera(ro, ta, 0.0);

    vec2 p = (-iResolution.xy + 2.0 * fragCoord) / iResolution.y;

    // ray direction
    vec3 rd = ca * normalize(vec3(p, 6.0));

    // ray differentials (NOT USED YET)
    vec2 px = (-iResolution.xy + 2.0 * (fragCoord.xy + vec2(1.0, 0.0))) / iResolution.y;
    vec2 py = (-iResolution.xy + 2.0 * (fragCoord.xy + vec2(0.0, 1.0))) / iResolution.y;
    vec3 rdx = ca * normalize(vec3(px, 2.0));
    vec3 rdy = ca * normalize(vec3(py, 2.0));

    // render
    vec4 col = render(p, ro, rd, rdx, rdy);

    outColor = col;
}