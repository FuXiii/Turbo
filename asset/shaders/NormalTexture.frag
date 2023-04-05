#version 450
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;

layout(set = 0, binding = 1) uniform texture2D myTexture;
layout(set = 0, binding = 2) uniform sampler mySampler;
layout(set = 0, binding = 3) uniform MyBuffer
{
    float value;
    vec3 cameraPos;
}
MYBUFFER;
layout(location = 0) out vec4 outColor;

void main()
{
    vec3 texture_color = texture(sampler2D(myTexture, mySampler), UV, 0).rgb;
    outColor = MYBUFFER.value * vec4(texture_color, 1.0);
}