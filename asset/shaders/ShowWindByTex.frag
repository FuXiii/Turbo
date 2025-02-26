#version 450

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 out_color;

layout(set = 0, binding = 0) uniform texture2D myFlowFieldTex; //(lat, lon, u, v)
layout(set = 0, binding = 1) uniform sampler mySampler;

vec2 MapToColor(vec2 meta)
{
    return (normalize(meta) + 1) * 0.5;
}

void main()
{
    vec4 flow_field = texture(sampler2D(myFlowFieldTex, mySampler), uv, 0);
    vec2 lat_lon = flow_field.xy;
    vec2 speed = flow_field.zw;
    out_color = vec4(MapToColor(speed), MapToColor(lat_lon));
}
