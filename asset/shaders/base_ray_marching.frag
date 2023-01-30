#version 450
layout(push_constant) uniform my_push_constants_t
{
    float time;
    float resolutionX;
    float resolutionY;

    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;

    float lookForwardDirX;
    float lookForwardDirY;
    float lookForwardDirZ;
}my_push_constants;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

#define PI 3.1415926

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}

float sdTorus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(p.xz) - t.x, p.y);
    return length(q) - t.y;
}

float map(vec3 p) {
    return sdTorus(p, vec2(1, 0.2));
}

vec3 RayMarch(vec3 origin,vec3 dir)
{
    vec3 result=vec3(0,0,0);
    const int maxstep = 64;

    float t = 0; // current distance traveled along ray
    for (int i = 0; i < maxstep; ++i) {
        vec3 p = origin + dir * t; // World space position of sample
        float d = map(p);       // Sample of distance field (see map())

        // If the sample <= 0, we have hit something (see map()).
        if (d < 0.001) {
            result = vec3(0.5, 0.5, 0.5);
            break;
        }

        t += d;
    }

    return result;
}

void main()
{
    float iTime = my_push_constants.time;
    vec2 iResolution = vec2(my_push_constants.resolutionX, my_push_constants.resolutionY);
    vec2 fragCoord = vec2(uv.x * iResolution.x, uv.y * iResolution.y);
    vec3 cameraPos = vec3(my_push_constants.cameraPosX,my_push_constants.cameraPosY,my_push_constants.cameraPosZ);
    vec3 lookForwardDir = vec3(my_push_constants.lookForwardDirX,my_push_constants.lookForwardDirY,my_push_constants.lookForwardDirZ);

    vec3 forwardDir = vec3(my_push_constants.lookForwardDirX,my_push_constants.lookForwardDirY,my_push_constants.lookForwardDirZ);
    vec3 upDir=vec3(0,1,0);
    vec3 rightDir=cross(forwardDir,upDir);
    upDir=cross(rightDir,forwardDir);

    forwardDir=normalize(forwardDir);
    rightDir=normalize(rightDir);
    upDir=normalize(upDir);

    vec2 screen_ndc = vec2(uv.x,1-uv.y)*2-1;//Normalized Device Corrdinate:[-1,1]

    float near = 0.1;
    float aspect= iResolution.y/iResolution.x;//height/width
    float horizontalFov= PI/3.0;//60

    float screenHalfWidth=near*tan(horizontalFov/2.0);
    float screenHalfHeight=screenHalfWidth*aspect;

    vec3 pixel_pos = cameraPos+ forwardDir*near+rightDir*screen_ndc.x*screenHalfWidth+upDir*screen_ndc.y*screenHalfHeight;

    vec3 rayDir = pixel_pos-cameraPos;
    rayDir = normalize(rayDir);

    vec3 color = RayMarch(cameraPos,rayDir);
    outColor = vec4(color, 1.0);
}