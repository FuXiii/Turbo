#version 460
#extension GL_EXT_ray_tracing : require

layout(location = 0) rayPayloadInEXT vec3 HIT_VALUE;

void main()
{
    HIT_VALUE = vec3(0.0, 0.1, 0.3);
}