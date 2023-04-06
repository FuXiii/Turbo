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

float D_GGX(float NoH, float roughness)
{
    float a = NoH * roughness;
    float k = roughness / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}

vec3 F_Schlick(float VoH, vec3 f0)
{
    return f0 + (vec3(1.0) - f0) * pow(1.0 - VoH, 5.0);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float roughness)
{
    float a2 = roughness * roughness;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float Fd_Lambert()
{
    return 1.0 / PI;
}

vec3 GammaToLiner(vec3 color)
{
    vec3 line_color = color;
    line_color.r <= 0.04045 ? line_color.r = color.r / 12.92 : line_color.r = pow((color.r + 0.055) / 1.055, 2.4);
    line_color.g <= 0.04045 ? line_color.g = color.g / 12.92 : line_color.g = pow((color.g + 0.055) / 1.055, 2.4);
    line_color.b <= 0.04045 ? line_color.b = color.b / 12.92 : line_color.b = pow((color.b + 0.055) / 1.055, 2.4);
    return line_color;
}

vec3 LinearToGamma(vec3 color)
{
    color = color / (color + vec3(1.0));
    return pow(color * .4, vec3(1. / 2.2));
}

void main()
{
    vec2 uv = UV * MyBuffer.value;
    vec3 cameraPos = vec3(MyBuffer.cameraPosX, MyBuffer.cameraPosY, MyBuffer.cameraPosZ);
    // vec3 normal_color = GammaToLiner(texture(sampler2D(normalTexture, mySampler), vec2(uv.x, 1 - uv.y), 0).rgb);
    vec3 normal_color = GammaToLiner(texture(sampler2D(normalTexture, mySampler), uv, 0).rgb);
    vec3 albedo_color = GammaToLiner(texture(sampler2D(albedoTexture, mySampler), uv, 0).rgb);
    vec3 roughness_color = GammaToLiner(texture(sampler2D(roughnessTexture, mySampler), uv, 0).rgb);

    vec3 l = normalize(vec3(1, 1, 1));
    vec3 v = normalize(cameraPos - POSITION);
    vec3 n = normalize(TBN * (normal_color * 2 - 1));
    vec3 h = normalize(v + l);

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    float VoH = clamp(dot(v, h), 0.0, 1.0);

    float D = D_GGX(NoH, roughness_color.x);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness_color.x);
    vec3 F = F_Schlick(LoH, vec3(0.04, 0.04, 0.04));

    vec3 BRDF_specular = D * V * F;
    vec3 BRDF_diffuse = albedo_color * Fd_Lambert();

    vec3 color = BRDF_diffuse + BRDF_specular;
    color = LinearToGamma(color);
    outColor = vec4(color, 1.0);
}