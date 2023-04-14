#version 450

#define LINE

#if defined(LINE)
layout(points) in;
layout(line_strip, max_vertices = 2) out;
#else
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
#endif

layout(location = 1) in vec3 NORMAL[];
layout(location = 2) in float NOISE[];

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 m;
    mat4 v;
    mat4 p;
}
Matrixs;

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

layout(location = 2) out float noise;

void GenerateLine(int index)
{
    gl_Position = Matrixs.p * gl_in[index].gl_Position;
    noise = NOISE[index];
    EmitVertex();
    gl_Position = Matrixs.p * (gl_in[index].gl_Position + vec4(NORMAL[index], 0.0) * MyBuffer.scale * NOISE[index]);
    noise = NOISE[index];
    EmitVertex();
    EndPrimitive();
}

void GenerateTriangle(int index)
{
    gl_Position = Matrixs.p * (gl_in[index].gl_Position + vec4(NORMAL[index], 0.0) * MyBuffer.scale * NOISE[index]);
    noise = NOISE[index];
    EmitVertex();

    gl_Position = Matrixs.p * (gl_in[index + 1].gl_Position + vec4(NORMAL[index + 1], 0.0) * MyBuffer.scale * NOISE[index + 1]);
    noise = NOISE[index + 1];
    EmitVertex();

    gl_Position = Matrixs.p * (gl_in[index + 2].gl_Position + vec4(NORMAL[index + 2], 0.0) * MyBuffer.scale * NOISE[index + 2]);
    noise = NOISE[index + 2];
    EmitVertex();
    EndPrimitive();
}

void main()
{
#if defined(LINE)
    GenerateLine(0);
#else
    GenerateTriangle(0);
#endif
}