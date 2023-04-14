#version 450

layout(location = 1) in vec3 NORMAL[];
layout(location = 2) in float NOISE[];

layout(set = 0, binding = 1) uniform MYBUFFER
{
    int inner0;
    int outer0;
    int outer1;
    int outer2;
    float scale;
    float time;
    float value;
}
MyBuffer;

layout(vertices = 3) out;
layout(location = 1) out vec3 normal[3];
layout(location = 2) out float noise[3];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    normal[gl_InvocationID] = NORMAL[gl_InvocationID];
    noise[gl_InvocationID] = NOISE[gl_InvocationID];

    gl_TessLevelInner[0] = float(MyBuffer.inner0);

    gl_TessLevelOuter[0] = float(MyBuffer.outer0);
    gl_TessLevelOuter[1] = float(MyBuffer.outer1);
    gl_TessLevelOuter[2] = float(MyBuffer.outer2);
}