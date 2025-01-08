#version 450

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 out_color;

layout(set = 0, binding = 0) uniform texture2D myFlowFieldTex; //(lat, lon, u, v)
layout(set = 0, binding = 1) uniform sampler mySampler;

void main()
{
    vec4 flow_field = texture(sampler2D(myFlowFieldTex, mySampler), uv, 0);
    vec2 speed = (normalize(vec2(flow_field.z, flow_field.w)) + 1) * 0.5;
    out_color = vec4(speed, 0, 1);
}
