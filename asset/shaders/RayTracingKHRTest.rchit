#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) rayPayloadInEXT vec3 hitValue;
hitAttributeEXT vec3 attribs;

void main()
{
  hitValue = vec3(0.2, 0.5, 0.5);
}
