#version 450

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 view;
    mat4 projection;
}
matrixs;

layout(location = 0) out vec4 outColor;
layout(location = 1) out float outCoordinate;

#define FLOAT_INFINITE 3.40282347e+30

void main()
{
    vec4 color = vec4(0, 0, 0, 1);
    vec3 pos = vec3(0, 0, 0);

    float sign = 1;
    switch (gl_InstanceIndex)
    {
    case 0: {
        color = vec4(1, 0, 0, 1);
        pos = gl_VertexIndex == 0 ? vec3(0, 0, 0) : vec3(1, 0, 0) * FLOAT_INFINITE;
    }
    break;
    case 1: {
        color = vec4(0, 1, 0, 1);
        pos = gl_VertexIndex == 0 ? vec3(0, 0, 0) : vec3(0, 1, 0) * FLOAT_INFINITE;
    }
    break;
    case 2: {
        color = vec4(0, 0, 1, 1);
        pos = gl_VertexIndex == 0 ? vec3(0, 0, 0) : vec3(0, 0, 1) * FLOAT_INFINITE;
    }
    break;
    case 3: {
        color = vec4(1, 0, 0, 0.7);
        pos = gl_VertexIndex == 0 ? vec3(0, 0, 0) : vec3(-1, 0, 0) * FLOAT_INFINITE;
        sign = -1;
    }
    break;
    case 4: {
        color = vec4(0, 1, 0, 0.7);
        pos = gl_VertexIndex == 0 ? vec3(0, 0, 0) : vec3(0, -1, 0) * FLOAT_INFINITE;
        sign = -1;
    }
    break;
    case 5: {
        color = vec4(0, 0, 1, 0.7);
        pos = gl_VertexIndex == 0 ? vec3(0, 0, 0) : vec3(0, 0, -1) * FLOAT_INFINITE;
        sign = -1;
    }
    break;
    default: {
    }
    break;
    }

    outColor = color;

    pos == vec3(0, 0, 0) ? outCoordinate = 0 : outCoordinate = sign * FLOAT_INFINITE;

    gl_Position = matrixs.projection * matrixs.view * vec4(pos, 1.0);
}