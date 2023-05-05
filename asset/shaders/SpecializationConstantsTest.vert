#version 450

layout(constant_id = 0) const bool bool_const_vlaue = true;
layout(constant_id = 1) const int int_const_vlaue = 1;
layout(constant_id = 2) const uint uint_const_vlaue = 2;
layout(constant_id = 3) const float float_const_vlaue = 3.0;
layout(constant_id = 10) const double double_const_vlaue = 4.0;

out gl_PerVertex
{
    vec4 gl_Position;
};
layout(location = 0) out vec2 uv;
void main()
{
    uv = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    vec3 pos = vec3(uv * 2.0f - 1.0f, 0.0f);

    if (!bool_const_vlaue && int_const_vlaue == 2 && uint_const_vlaue == 3 && float_const_vlaue == 4 && double_const_vlaue == 5)
    {
        gl_Position = vec4(pos * 0.3, 1.0f);
    }
    else if (bool_const_vlaue && int_const_vlaue == 1 && uint_const_vlaue == 2 && float_const_vlaue == 3 && double_const_vlaue == 4)
    {
        gl_Position = vec4(pos * 0.6, 1.0f);
    }
    else
    {
        gl_Position = vec4(pos, 1.0f);
    }
}