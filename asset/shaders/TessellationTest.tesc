#version 450

layout(vertices = 3) out;

layout(set = 0, binding = 0) uniform MYBUFFER
{
    int inner0;
    int outer0;
    int outer1;
    int outer2;
}
MyBuffer;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelInner[0] = float(MyBuffer.inner0);

    gl_TessLevelOuter[0] = float(MyBuffer.outer0);
    gl_TessLevelOuter[1] = float(MyBuffer.outer1);
    gl_TessLevelOuter[2] = float(MyBuffer.outer2);
}