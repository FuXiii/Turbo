#version 450

layout(location = 0) out vec2 uv;

vec2 meta[4] = vec2[](
    vec2(-1, 1), 
    vec2(-1, -1), 
    vec2(1, 1), 
    vec2(1, -1)
);

void main()
{
    gl_Position = vec4(meta[gl_VertexIndex], 0.0, 1.0);
    uv = (meta[gl_VertexIndex] + vec2(1, 1)) * 0.5;
    uv.y = 1 - uv.y;
}
