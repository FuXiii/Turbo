#version 450

layout(set = 0, binding = 1) uniform texture2D myFlowFieldTex; //(lat, lon, u, v)
layout(set = 0, binding = 2) uniform sampler mySampler;

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

vec2 MapToColor(vec2 meta)
{
    return (normalize(meta) + 1) * 0.5;
}

void main()
{
    vec4 flow_field = texture(sampler2D(myFlowFieldTex, mySampler), inUV, 0);
    vec2 lat_lon = flow_field.xy;
    vec2 speed = flow_field.zw;
    vec4 out_color = vec4(MapToColor(speed), MapToColor(lat_lon));
    outColor = out_color;
}