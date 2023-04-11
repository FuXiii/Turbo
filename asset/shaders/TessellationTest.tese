#version 450

layout(triangles, equal_spacing, cw) in;

layout(set = 0, binding = 1) uniform MATRIXS
{
    mat4 m;
    mat4 v;
    mat4 p;
}
Matrixs;

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;

    // vec4 pos1 = mix(p0, p1, gl_TessCoord.x);
    // vec4 pos = mix(p2, pos1, gl_TessCoord.y);
    vec4 pos = p0 * gl_TessCoord.x + p1 * gl_TessCoord.y + p2 * gl_TessCoord.z;

    gl_Position = Matrixs.p * Matrixs.v * Matrixs.m * pos;
}