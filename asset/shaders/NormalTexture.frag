#version 450
layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(location = 3) in mat3 TBN;

layout(set = 0, binding = 1) uniform texture2D normalTexture;
layout(set = 0, binding = 2) uniform texture2D albedoTexture;
layout(set = 0, binding = 3) uniform sampler mySampler;
layout(set = 0, binding = 4) uniform MYBUFFER
{
    float value;
    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;
}
MyBuffer;
layout(location = 0) out vec4 outColor;

void main()
{
    vec3 cameraPos = vec3(MyBuffer.cameraPosX, MyBuffer.cameraPosY, MyBuffer.cameraPosZ);
    vec3 l = normalize(vec3(1, 1, 1));
    vec3 v = normalize(POSITION - cameraPos);

    vec3 normal_color = texture(sampler2D(normalTexture, mySampler), UV, 0).rgb;
    vec3 albedo_color = texture(sampler2D(albedoTexture, mySampler), UV, 0).rgb;
    vec3 n = normalize(normal_color * 2 - 1);
    n = normalize(TBN * n);
    vec3 diffuse = albedo_color * max(dot(n, -l), 0);
    vec3 ambient = vec3(0.0, 0.0, 0.0);

    vec3 color = MyBuffer.value * (ambient + diffuse);
    color = pow(color * .4, vec3(1. / 2.2));
    outColor = vec4(color, 1.0);
}