#version 450

layout(push_constant) uniform myPushConstants
{
    float time;
    float deltaTime;
    float speed;
    bool isEarth;
    float radius;
}
my_push_constants;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec3 inEyePos;
layout(location = 3) in vec3 inParticalPos;

layout(location = 0) out vec4 outColor;

void main()
{
    if (my_push_constants.isEarth)
    {
        float radius_2 = pow(my_push_constants.radius, 2);
        float origin_to_eye_2 = pow(inEyePos.x, 2) + pow(inEyePos.y, 2) + pow(inEyePos.z, 2);

        if (origin_to_eye_2 > radius_2)
        {
            float l_2 = origin_to_eye_2 - radius_2;

            vec3 partical_to_eye = inParticalPos - inEyePos;
            float partical_to_eye_l_2 = pow(partical_to_eye.x, 2) + pow(partical_to_eye.y, 2) + pow(partical_to_eye.z, 2);

            if (partical_to_eye_l_2 > l_2)
            {
                discard;
            }
        }
    }

    outColor = inColor;
}