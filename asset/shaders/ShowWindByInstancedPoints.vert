#version 450

layout(push_constant) uniform myPushConstants
{
    float time;
    float deltaTime;
    float speed;
}
my_push_constants;

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 view;
    mat4 projection;
}
matrixs;

layout(set = 0, binding = 1, rgba32f) uniform image2D myFlowFieldTex;   //(lat, lon, u, v)
layout(set = 0, binding = 2, rgba32f) uniform image2D myParticalPosTex; //(x, y, z, ?)

layout(location = 0) out vec4 outColor;

float hash(float v)
{
    return abs(fract(sin(v) * 43758.5453));
}

void main()
{
    float time = my_push_constants.time;

    ivec2 partical_pos_tex_size = imageSize(myParticalPosTex);
    ivec2 flow_field_tex_size = imageSize(myFlowFieldTex);

    int row = gl_InstanceIndex / partical_pos_tex_size.x;
    int column = gl_InstanceIndex - row * partical_pos_tex_size.x;

    ivec2 tex_coord = ivec2(column, row);
    vec2 uv = vec2(row / float(partical_pos_tex_size.x), column / float(partical_pos_tex_size.y));

    outColor = vec4(1, 1, 1, 1);

    vec4 current_partical_pos = imageLoad(myParticalPosTex, tex_coord);
    {
        if (current_partical_pos.w < time)
        {
            float random_u = hash(dot(current_partical_pos.xy + vec2(901.2634280527, 458.97914471036), vec2(135.6379606213, 383.5357022848)));
            float random_v = hash(dot(current_partical_pos.yx + vec2(640.0754402848, 813.1273531719), vec2(375.4331612936, 198.0555356584)));

            vec2 start_partical_pos = (uv + vec2(random_u, random_v)) * 0.5;
            current_partical_pos = vec4(start_partical_pos, 0, current_partical_pos.w);
            current_partical_pos.w = time + (random_u + random_v) * 2;
        }

        vec4 current_flow = imageLoad(myFlowFieldTex, ivec2(clamp(current_partical_pos.xy, 0, 1) * flow_field_tex_size));
        vec2 geo = current_flow.xy; //(lat, lon)
        vec2 current_flow_dir = current_flow.zw;

        vec4 next_partical_pos = vec4(current_partical_pos.xy + current_flow_dir * 0.00003 * my_push_constants.speed, 0, current_partical_pos.w);

        imageStore(myParticalPosTex, tex_coord, next_partical_pos);
    }

    float z_offset = 0;
    gl_Position = matrixs.projection * matrixs.view * vec4(current_partical_pos.xy * flow_field_tex_size, -z_offset, 1.0);
}
