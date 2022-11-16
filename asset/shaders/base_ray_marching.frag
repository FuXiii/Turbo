#version 450
layout(push_constant) uniform my_push_constants_t
{
    float time;
    float resolutionX;
    float resolutionY;
}my_push_constants;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

void main()
{
    float iTime = my_push_constants.time;
    vec2 iResolution = vec2(my_push_constants.resolutionX, my_push_constants.resolutionY);
    vec2 fragCoord = vec2(uv.x * iResolution.x, uv.y * iResolution.y);
    vec3 color = vec3(1, 0, 0);
    outColor = vec4(color, 1.0);
}