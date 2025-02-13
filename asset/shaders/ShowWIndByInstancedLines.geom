#version 450
#define POINTS_COUNT 10

layout(push_constant) uniform myPushConstants
{
    float time;
    float deltaTime;
    float speed;
    bool isEarth;
    float radius;
}
my_push_constants;

layout(set = 0, binding = 0) uniform MATRIXS
{
    mat4 view;
    mat4 projection;
    mat4 viewInverse;
    mat4 projectionInverse;
}
matrixs;

layout(set = 0, binding = 1) uniform sampler2D myFlowFieldTex;          //(lat, lon, u, v)
layout(set = 0, binding = 2, rgba32f) uniform image2D myParticalPosTex; //(x, y, z, ?)

layout(points) in;
layout(line_strip, max_vertices = POINTS_COUNT) out;

layout(location = 0) in vec4 inColor[1];
layout(location = 1) in vec3 inEyePos[1];
layout(location = 2) in vec4 inSamplePos[1];
layout(location = 3) in vec3 inParticalPos[1];

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec3 outEyePos;
layout(location = 2) out vec4 outSamplePos;
layout(location = 3) out vec3 outParticalPos;

#define ECCENTRICITY_SQUARED 0.0066943800042609
#define WGS84_RADIUS_EQUATOR my_push_constants.radius // 6378137.0

vec3 GeoToXYZ(vec2 geo, float height)
{
    float sin_latitude = sin((geo.x));
    float cos_latitude = cos((geo.x));

    float N = WGS84_RADIUS_EQUATOR / sqrt(1.0 - ECCENTRICITY_SQUARED * pow(sin_latitude, 2));

    float x = (N + height) * cos_latitude * cos((geo.y));
    float y = (N + height) * cos_latitude * sin((geo.y));
    float z = (N * (1 - ECCENTRICITY_SQUARED) + height) * sin_latitude;

    return vec3(x, z, y);
}

void main()
{
    outColor = inColor[0];
    outEyePos = inEyePos[0];
    outSamplePos = inSamplePos[0];
    outParticalPos = inParticalPos[0];
    gl_Position = matrixs.projection * matrixs.view * vec4(outParticalPos, 1.0);
    EmitVertex();

    {
        vec4 current_flow = texture(myFlowFieldTex, outSamplePos.xy);
        vec2 current_geo = current_flow.xy * 3.1415926 / 180.0; //(lat, lon)
        vec2 current_flow_dir = current_flow.zw;

        for (int i = 0; i < POINTS_COUNT; i++)
        {
            outSamplePos = vec4(outSamplePos.xy - current_flow_dir * 0.00003 * my_push_constants.speed, 0, outSamplePos.w);

            current_flow = texture(myFlowFieldTex, outSamplePos.xy);
            current_geo = current_flow.xy * 3.1415926 / 180.0; //(lat, lon)
            current_flow_dir = current_flow.zw;

            if (my_push_constants.isEarth)
            {
                outParticalPos = GeoToXYZ(current_geo, 1);
            }
            else
            {
                ivec2 flow_field_tex_size = textureSize(myFlowFieldTex, 0);
                outParticalPos = vec3(outSamplePos.xy * flow_field_tex_size, 0);
            }

            gl_Position = matrixs.projection * matrixs.view * vec4(outParticalPos, 1.0);
            outColor = inColor[0];
            outEyePos = inEyePos[0];

            EmitVertex();
        }
    }
}