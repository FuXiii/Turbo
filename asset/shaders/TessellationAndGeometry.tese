#version 450

layout(triangles, equal_spacing, cw) in;

layout(location = 1) in vec3 NORMAL[];
layout(location = 2) in float NOISE[];

layout(location = 1) out vec3 normal;
layout(location = 2) out float noise;

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;

    vec4 _pos = p0 * gl_TessCoord.x + p1 * gl_TessCoord.y + p2 * gl_TessCoord.z;
    vec3 _normal = NORMAL[0] * gl_TessCoord.x + NORMAL[1] * gl_TessCoord.y + NORMAL[2] * gl_TessCoord.z;
    float _noise = NOISE[0] * gl_TessCoord.x + NOISE[1] * gl_TessCoord.y + NOISE[2] * gl_TessCoord.z;

    gl_Position = _pos;
    normal = _normal;
    noise = _noise;
}