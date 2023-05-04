#version 450
layout(push_constant) uniform my_push_constants_t
{
    float time;
    float resolutionX;
    float resolutionY;
}
my_push_constants;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;
layout(set = 0, binding = 0) uniform texture2D myTexture;
layout(set = 0, binding = 1) uniform sampler mySampler;
float remap(float x, float a, float b, float c, float d)
{
    return (((x - a) / (b - a)) * (d - c)) + c;
}
void main()
{
    float iTime = my_push_constants.time;
    vec2 iResolution = vec2(my_push_constants.resolutionX, my_push_constants.resolutionY);
    vec2 fragCoord = vec2(uv.x * iResolution.x, uv.y * iResolution.y);
    vec4 texture_color = texture(sampler2D(myTexture, mySampler), uv, 0);
    vec3 color = vec3(texture_color.xyz);
    outColor = vec4(color, 1.0);
}