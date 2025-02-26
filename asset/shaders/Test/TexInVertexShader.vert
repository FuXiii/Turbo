#version 450

layout(set = 0, binding = 1) uniform sampler2D mySampler2D;
layout(set = 0, binding = 2, rgba32f) uniform image2D myImage2D;

layout(set = 0, binding = 3) uniform texture2D myTexture2D;
layout(set = 0, binding = 4) uniform sampler mySampler;

void main()
{
    vec2 uv;
    ivec2 tex_coord;

    vec4 my_sampler_2d_texture_color = texture(mySampler2D, uv);
    vec4 my_image_2d_image_load_color = imageLoad(myImage2D, tex_coord);
    vec4 my_texture_2d_texture_color = texture(sampler2D(myTexture2D, mySampler), uv);
    vec4 my_texture_2d_texture_lod_color = textureLod(sampler2D(myTexture2D, mySampler), uv, 0);
    vec4 my_texture_2d_texel_fetch_color = texelFetch(sampler2D(myTexture2D, mySampler), tex_coord, 0);
    // vec4 my_texture_2d_texture_2d_color = texture2D(sampler2D(myTexture2D, mySampler), uv);

    gl_Position = vec4(0, 0, 0, 0);
}