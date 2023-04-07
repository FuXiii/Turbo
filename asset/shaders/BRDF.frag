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

vec3 GetNormalFromMap(vec2 uv)
{
    vec3 tangentNormal = /*GammaToLiner*/ (texture(sampler2D(normalTexture, mySampler), uv, 0).rgb * 2 - 1);

    vec3 Q1 = dFdx(POSITION);
    vec3 Q2 = dFdy(POSITION);
    vec2 st1 = dFdx(uv);
    vec2 st2 = dFdy(uv);

    vec3 N = normalize(NORMAL);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float D_GGX(float NoH, float roughness)
{
    float a = NoH * roughness;
    float k = roughness / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}

float _D_GGX(float NoH, float a)
{
    float a2 = a * a;
    return a2 / (PI * pow(((NoH * NoH) * (a2 - 1.0) + 1.0), 2.0));
}

vec3 F_Schlick(float VoH, vec3 f0)
{
    return f0 + (vec3(1.0) - f0) * pow(1.0 - VoH, 5.0);
}

vec3 _F_Schlick(float VoH, vec3 f0)
{
    return f0 + (1.0 - f0) * pow(1.0 - VoH, 5.0);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float roughness)
{
    float a2 = roughness * roughness;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float _V_SmithGGXCorrelated(float NoV, float NoL, float a)
{
    float a2 = a * a;
    return 0.5 / (NoL * sqrt(a2 + (1.0 - a2) * NoV * NoV) + NoV * sqrt(a2 + (1.0 - a2) * NoL * NoL));
}

float Fd_Lambert()
{
    return 1.0 / PI;
}

void main()
{
    vec2 uv = UV * MyBuffer.value;
    vec3 cameraPos = vec3(MyBuffer.cameraPosX, MyBuffer.cameraPosY, MyBuffer.cameraPosZ);
    // vec3 normal_color = GammaToLiner(texture(sampler2D(normalTexture, mySampler), vec2(uv.x, 1 - uv.y), 0).rgb);
    vec3 normal_color = GammaToLiner(texture(sampler2D(normalTexture, mySampler), uv, 0).rgb);
    vec3 albedo_color = GammaToLiner(texture(sampler2D(albedoTexture, mySampler), uv, 0).rgb);
    vec3 roughness_color = GammaToLiner(texture(sampler2D(roughnessTexture, mySampler), uv, 0).rgb);

    vec3 l = -normalize(vec3(1, 1, 1));
    vec3 v = normalize(cameraPos - POSITION);
    // vec3 n = normalize(TBN * (normal_color * 2 - 1));
    vec3 n = GetNormalFromMap(uv);
    vec3 h = normalize(v + l);

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);
    float VoH = clamp(dot(v, h), 0.0, 1.0);

    float roughness = roughness_color.r;

    float metallic = 0;
    float a = roughness * roughness;
    vec3 albedo = albedo_color;

    vec3 diffuse = (1.0 - metallic) * albedo;
    vec3 f0 = vec3(0.04, 0.04, 0.04);

    // loat D = D_GGX(NoH, roughness_color.x);
    // loat V = V_SmithGGXCorrelated(NoV, NoL, roughness_color.x);
    // ec3 F = F_Schlick(LoH, vec3(0.04, 0.04, 0.04));

    float D = _D_GGX(NoH, a);
    float V = _V_SmithGGXCorrelated(NoV, NoL, a);
    vec3 F = _F_Schlick(VoH, f0);

    vec3 BRDF_specular = D * V * F;
    vec3 BRDF_diffuse = diffuse * Fd_Lambert() * (vec3(1.0) - F) * (1.0 - metallic);

    vec3 color = (BRDF_diffuse + BRDF_specular) * max(NoL, 0.01);
    color = LinearToGamma(color);
    outColor = vec4(color, 1.0);
}