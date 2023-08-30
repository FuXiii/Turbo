#version 460
#extension GL_EXT_ray_tracing : require

struct HitPayload
{
    vec3 color;
    uint seed;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;

void main()
{
    HIT_PAY_LOAD.color = vec3(0.0, 0.1, 0.3);
}