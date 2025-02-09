#version 450

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 view;
    mat4 projection;
}
matrixs;

layout(set = 0, binding = 1) uniform texture2D myFlowFieldTex; //(lat, lon, u, v)
layout(set = 0, binding = 2) uniform sampler mySampler;

layout(location = 0) out vec2 outUV;

void main()
{
    vec2 uv = vec2(0, 0);
    vec3 pos = vec3(0, 0, 0);

    ivec2 size = textureSize(sampler2D(myFlowFieldTex, mySampler), 0);

    switch (gl_VertexIndex)
    {
    case 0: {
        pos = vec3(0, 0, 0);
    }
    break;
    case 1: {
        pos = vec3(0, 1, 0);
    }
    break;
    case 2: {
        pos = vec3(1, 0, 0);
    }
    break;
    case 3: {
        pos = vec3(1, 1, 0);
    }
    break;
    default: {
    }
    break;
    }

    outUV = pos.xy;

    pos.xy *= size;

    gl_Position = matrixs.projection * matrixs.view * vec4(pos, 1.0);
}