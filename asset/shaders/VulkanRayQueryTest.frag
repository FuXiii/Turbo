#version 460 core
#extension GL_EXT_ray_tracing : enable
#extension GL_EXT_ray_query : enable

#define PI 3.1415926

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform MY_PUSH_CONSTANTS_DATA
{
    float _angle;
    float _length;
}
my_push_constants_data;

layout(set = 0, binding = 1) uniform accelerationStructureEXT topLevelAS;

void main()
{
    // outColor = vec4(1.0, 0.1686, 0.9568, 1.0);
    outColor = vec4(normal, 1.0);

    float _angle = my_push_constants_data._angle;
    float _length = my_push_constants_data._length;

    vec3 origin = position.xyz;
    vec3 direction = -normalize(vec3(1.414 * cos(_angle * PI / 180.0), 1.414 * sin(_angle * PI / 180.0), 1)); // vector to light
    //float tMin = 0.01f;
    float tMin = 0.01f;
    float tMax = _length;

    rayQueryEXT rayQuery;
    rayQueryInitializeEXT(rayQuery, topLevelAS, gl_RayFlagsTerminateOnFirstHitEXT, 0xFF, origin, tMin, direction, tMax);

    while (rayQueryProceedEXT(rayQuery))
    {
    }

    if (rayQueryGetIntersectionTypeEXT(rayQuery, true) != gl_RayQueryCommittedIntersectionNoneEXT)
    {
        outColor.xyz *= 0.1;
    }
}