#version 450

layout(location = 0) in vec4 inColor;
layout(location = 1) in float inCoordinate;

layout(location = 0) out vec4 outColor;

void main()
{
    if (inCoordinate < 0 && (mod(floor(inCoordinate), 2) == 0))
    {
        discard;
    }
    outColor = inColor;
}