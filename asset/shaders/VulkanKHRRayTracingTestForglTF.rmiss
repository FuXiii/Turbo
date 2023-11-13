#version 460
#extension GL_EXT_ray_tracing : require

struct HitPayload
{
    uint seed;
    uint depth;
    vec3 rayOrigin;
    vec3 rayDirection;
    vec3 weight;
    vec3 color;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;

void main()
{
    if (HIT_PAY_LOAD.depth == 0)
        HIT_PAY_LOAD.color = vec3(0.3, 0.3, 0.3) * 0.8;
    else
        HIT_PAY_LOAD.color = vec3(0.01); // No contribution from environment
    HIT_PAY_LOAD.depth = 100;            // Ending trace
}