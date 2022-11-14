#version 450
layout(binding = 0) uniform sampler2D fontSampler;
layout(location = 0) in vec2 inUV;
layout(location = 1) in vec4 inColor;
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outCustomColor;
void main()
{
    outColor = inColor * texture(fontSampler, inUV);
    outCustomColor = outColor;
}