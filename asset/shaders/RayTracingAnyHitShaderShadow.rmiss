#version 460
#extension GL_EXT_ray_tracing : require

struct ShadowHitPayload
{
    bool isShadowed;
    uint seed;
};

layout(location = 1) rayPayloadInEXT ShadowHitPayload SHADOW_HIT_PAY_LOAD;

void main()
{
    SHADOW_HIT_PAY_LOAD.isShadowed = false;
}