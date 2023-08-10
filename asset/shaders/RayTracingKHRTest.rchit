#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) rayPayloadInEXT vec3 HIT_VALUE;

void main()
{
    HIT_VALUE = vec3(0.2, 0.5, 0.5);
}
