#version 450
#define PI 3.141592653589
layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 UV;
layout(location = 3) in mat3 TBN;

layout(set = 0, binding = 1) uniform texture2D normalTexture;
layout(set = 0, binding = 2) uniform texture2D albedoTexture;
layout(set = 0, binding = 3) uniform texture2D roughnessTexture;
layout(set = 0, binding = 4) uniform sampler mySampler;
layout(set = 0, binding = 5) uniform MYBUFFER
{
    float value;
    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;
    float roughness;
}
MyBuffer;
layout(location = 0) out vec4 outColor;

#define MEDIUMP_FLT_MAX 65504.0
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)

float D_GGX(const vec3 n, const vec3 h, float roughness)
{
    vec3 NxH = cross(n, h);
    float a = dot(n, h) * roughness;
    float k = roughness / (dot(NxH, NxH) + a * a);
    float d = k * k * (1.0 / PI);
    return saturateMediump(d);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float roughness)
{
    float a2 = roughness * roughness;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);
}

float V_SmithGGXCorrelatedFast(float NoV, float NoL, float roughness)
{
    float a = roughness;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5 / (GGXV + GGXL);
}

vec3 F_Schlick(float VoH, vec3 f0, vec3 f90)
{
    return f0 + (f90 - f0) * pow(1.0 - VoH, 5.0);
}

float Fd_Lambert()
{
    return 1.0 / PI;
}

vec3 Fd_Burley(float NoV, float NoL, float LoH, float roughness)
{
    float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
    vec3 lightScatter = F_Schlick(NoL, vec3(1.0, 1.0, 1.0), vec3(f90, f90, f90));
    vec3 viewScatter = F_Schlick(NoV, vec3(1.0, 1.0, 1.0), vec3(f90, f90, f90));
    return lightScatter * viewScatter * (1.0 / PI);
}

void main()
{
    vec2 uv = UV * MyBuffer.value * 3;
    vec3 cameraPos = vec3(MyBuffer.cameraPosX, MyBuffer.cameraPosY, MyBuffer.cameraPosZ);
    vec3 normal_color = texture(sampler2D(normalTexture, mySampler), vec2(uv.x, 1 - uv.y), 0).rgb;
    vec3 albedo_color = texture(sampler2D(albedoTexture, mySampler), uv, 0).rgb;
    vec3 roughness_color = texture(sampler2D(roughnessTexture, mySampler), uv, 0).rgb;

    vec3 l = normalize(vec3(-1, -1, -1));
    vec3 v = normalize(POSITION - cameraPos);
    vec3 n = normalize(TBN * (normal_color * 2 - 1));
    // vec3 n = NORMAL;
    vec3 h = normalize(-v + l);

    float D = D_GGX(n, h, /*MyBuffer.roughness*/ roughness_color.x);
    // float V = V_SmithGGXCorrelatedFast(dot(n, -v), dot(n, l), /*MyBuffer.roughness*/roughness_color.x);
    float V = V_SmithGGXCorrelated(dot(n, -v), dot(n, l), /*MyBuffer.roughness*/ roughness_color.x);
    vec3 F = F_Schlick(dot(-v, h), vec3(0.56, 0.57, 0.58), vec3(1, 1, 1));

    vec3 BRDF_specular = D * V * F;

    vec3 diffuse = albedo_color * Fd_Burley(dot(n, -v), dot(n, l), dot(h, l), /*MyBuffer.roughness*/ roughness_color.x);
    vec3 ambient = vec3(0.0, 0.0, 0.0);

    vec3 color = ambient + diffuse + BRDF_specular;
    color = pow(color * .4, vec3(1. / 2.2));
    outColor = vec4(color, 1.0);
}