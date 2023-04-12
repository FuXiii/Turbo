#version 450
layout(points) in;
layout(line_strip, max_vertices = 6) out;

layout(location = 1) in vec3 NORMAL[];

const float MAGNITUDE = 0.04;

layout(set = 0, binding = 1) uniform MATRIXS
{
    mat4 m;
    mat4 v;
    mat4 p;
}
Matrixs;

void GenerateLine(int index)
{
    gl_Position = Matrixs.p * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = Matrixs.p * (gl_in[index].gl_Position + vec4(NORMAL[index], 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    // GenerateLine(1);
    // GenerateLine(2);
}