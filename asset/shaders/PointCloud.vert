#version 450

layout(set = 0, binding = 0) uniform MVP_MATRIXS
{
    mat4 model;
    mat4 view;
    mat4 project;
};
layout(set = 0, binding = 1, rgba32f) uniform image2D POINTS_POISITION_TEX;
layout(set = 0, binding = 2, rgba32f) uniform image2D POINTS_COLOR_TEX;

layout(location = 0) out vec3 v_color;

void main()
{
    int tex_width = 512;
    int row = gl_InstanceIndex / tex_width;
    int column = gl_InstanceIndex - row * tex_width;
    ivec2 tex_coord = ivec2(column, row);
    // ivec2 tex_coord = ivec2(row, column);

    vec3 point_pos = imageLoad(POINTS_POISITION_TEX, tex_coord).xyz;
    vec4 point_color = imageLoad(POINTS_COLOR_TEX, tex_coord);

    v_color = point_color.xyz;
    gl_Position = project * view * model * vec4(point_pos, 1.0);
    // gl_Position = project * view * model * vec4(vec3(row, column, 0), 1.0);
    gl_PointSize = 1.0;
}