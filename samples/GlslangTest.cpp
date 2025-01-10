#include <iostream>

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Include/Types.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

#include <ReadFile.h>

std::string asset_root(TURBO_ASSET_ROOT);

int main()
{
    auto essl_version_str = glslang::GetEsslVersionString();
    auto glsl_version_str = glslang::GetGlslVersionString();

    std::cout << "essl_version_str : " << essl_version_str << std::endl;
    std::cout << "glsl_version_str : " << glsl_version_str << std::endl;

    if (glslang::InitializeProcess())
    {
        std::cout << "glslang::InitializeProcess() = true" << std::endl;

        auto vert_code = ReadTextFile(asset_root + "/shaders/glslangTest.vert");
        const char *code_c_str = vert_code.c_str();

        glslang::TShader shader(EShLanguage::EShLangVertex);
        shader.setStrings(&code_c_str, 1);
        shader.setEnvInput(glslang::EShSource::EShSourceGlsl, EShLanguage::EShLangVertex, glslang::EShClient::EShClientVulkan, 100);
        shader.setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EShTargetClientVersion::EShTargetVulkan_1_0);
        shader.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_0);
        EShMessages message = EShMessages(EShMessages::EShMsgDefault | EShMessages::EShMsgSpvRules | EShMessages::EShMsgVulkanRules);
        if (!shader.parse(GetDefaultResources(), 100, true, message))
        {
            std::cout << shader.getInfoLog() << std::endl;
        }

        glslang::FinalizeProcess();
    }
    return 0;
}