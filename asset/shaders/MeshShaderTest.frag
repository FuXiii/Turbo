#version 450 core
layout(location = 0) in vec3 COLOR;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(COLOR, 1.0);
}