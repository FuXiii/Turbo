#version 450
layout(push_constant) uniform my_push_constants_t
{
    float time;
    float resolutionX;
    float resolutionY;
    float slice;
    float coverage;
}
my_push_constants;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;
layout(set = 0, binding = 0) uniform texture3D myTexture;
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
    float slice = my_push_constants.slice;
    float coverage = my_push_constants.coverage;
    vec2 _uv = uv + 0.02 * iTime;
    vec4 fbm = texture(sampler3D(myTexture, mySampler), vec3(_uv, slice), 0);
    float perlinWorley = fbm.x;
    vec3 worleys = fbm.yzw;
    float worlyFbm = worleys.x * 0.625 + worleys.y * 0.125 + worleys.z * 0.25;
    float cloud = remap(perlinWorley, worlyFbm - 1., 1., 0., 1.);
    cloud = remap(cloud, 1 - coverage, 1., 0., 1.); // coverage
    vec3 color = vec3(cloud, cloud, cloud);
    outColor = vec4(color, 1.0);
}