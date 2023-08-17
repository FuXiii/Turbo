#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

struct HitPayload
{
    vec3 color;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;

void main()
{
    HIT_PAY_LOAD.color = vec3(0.2, 0.5, 0.5);
}
