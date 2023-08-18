#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

struct HitPayload
{
    vec3 color;
};

struct PushConstant
{
    bool isBarycentrics;
};

layout(location = 0) rayPayloadInEXT HitPayload HIT_PAY_LOAD;
layout(push_constant) uniform PushConstant_
{
    PushConstant pc;
};

hitAttributeEXT vec2 attribs;

void main()
{
    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y); // 最近命中点重心坐标

    if (pc.isBarycentrics)
    {
        HIT_PAY_LOAD.color = barycentrics;
    }
    else
    {
        HIT_PAY_LOAD.color = vec3(0.2, 0.5, 0.5);
    }
}
