#version 450 core
layout (set = 0, binding = 0) uniform bufferVals {
    mat4 mvp;
} myBufferVals;
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;
layout (location = 0) out vec4 outColor;
void main() {
   outColor = inColor;
   gl_Position =pos;
}