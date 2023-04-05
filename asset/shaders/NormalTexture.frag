#version 450
layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 TANGENT;

layout(set = 0, binding = 1) uniform texture2D myTexture;
layout(set = 0, binding = 2) uniform sampler mySampler;
layout(set = 0, binding = 3) uniform MYBUFFER
{
    float value;
    vec3 cameraPos;
}
MyBuffer;
layout(location = 0) out vec4 outColor;

void main()
{
    vec3 l = normalize(vec3(1, 1, 1));
    vec3 v = normalize(POSITION - MyBuffer.cameraPos);
    vec3 N = NORMAL;
    // vec3 T = normalize(cross(N, vec3(0, 1, 0)));
    vec3 T = TANGENT;
    //vec3 B = normalize(cross(N, T));
    vec3 B = normalize(cross(T, N));
    mat3 TBN = mat3(T, B, N);

    vec3 texture_color = texture(sampler2D(myTexture, mySampler), UV, 0).rgb;
    vec3 n = texture_color * 2 - 1;
    n = TBN * n;
    vec3 diffuse = vec3(1, 1, 1) * max(dot(n, -l), 0);
    vec3 ambient = vec3(0., 0., 0.);
    outColor = MyBuffer.value * vec4(ambient + diffuse, 1.0);
}