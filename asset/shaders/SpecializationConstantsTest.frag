#version 450

layout(constant_id = 0) const bool bool_const_vlaue = false;
layout(constant_id = 1) const int int_const_vlaue = 10;
layout(constant_id = 2) const uint uint_const_vlaue = 20;
layout(constant_id = 3) const float float_const_vlaue = 30.0;
layout(constant_id = 11) const double double_const_vlaue = 40.0;

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
    if (bool_const_vlaue && int_const_vlaue == 20 && uint_const_vlaue == 30 && float_const_vlaue == 40 && double_const_vlaue == 50)
    {
        if (uv.x > 0.5)
        {
            color = vec3(1, 1, 0);
        }

        if (uv.y > 0.5)
        {
            color = vec3(0, 1, 1);
        }
    }
    else if (!bool_const_vlaue && int_const_vlaue == 10 && uint_const_vlaue == 20 && float_const_vlaue == 30 && double_const_vlaue == 40)
    {
        if (uv.x > 0.5)
        {
            color = vec3(1, 0, 1);
        }
    }
    else
    {
        
    }

    outColor = vec4(color, 1.0);
}