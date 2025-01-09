#include <iostream>

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Include/Types.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

int main()
{
    std::cout << "Hello World" << std::endl;
    if (glslang::InitializeProcess())
    {
        std::cout << "glslang::InitializeProcess() = true" << std::endl;
    }
    return 0;
}