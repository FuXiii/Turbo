#version 450 core
layout(set = 0, binding = 0) uniform bufferVals
{
    float value;
}
myBufferVals;
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) out vec3 outColor;
layout(location = 3) out float outValue;
void main()
{
    gl_Position = vec4(pos, 1);
    outColor = color;
    outValue = myBufferVals.value;
}