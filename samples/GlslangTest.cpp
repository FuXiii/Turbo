#include <iostream>
#include <sstream>

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Include/Types.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

#include <SPIRV/disassemble.h>

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

        auto ParseShader = [](EShLanguage language, const std::string &code) {
            const char *const_code_c_str = code.c_str();

            glslang::TShader *shader = new glslang::TShader(language);
            shader->setStrings(&const_code_c_str, 1);
            shader->setEnvInput(glslang::EShSource::EShSourceGlsl, language, glslang::EShClient::EShClientVulkan, 100);
            shader->setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EShTargetClientVersion::EShTargetVulkan_1_0);
            shader->setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_0);
            EShMessages message = static_cast<EShMessages>(EShMessages::EShMsgDefault | EShMessages::EShMsgSpvRules | EShMessages::EShMsgVulkanRules | EShMessages::EShMsgAST | EShMessages::EShMsgKeepUncalled);
            if (!shader->parse(GetDefaultResources(), 100, true, message))
            {
                std::cout << shader->getInfoLog() << std::endl;
                std::cout << shader->getInfoDebugLog() << std::endl;
            }

            return shader;
        };

        //glslang::TShader *vertex_shader = ParseShader(EShLanguage::EShLangVertex, ReadTextFile(asset_root + "/shaders/glslangTest.vert"));
        //glslang::TShader *fragment_shader = ParseShader(EShLanguage::EShLangFragment, ReadTextFile(asset_root + "/shaders/glslangTest.frag"));
        glslang::TShader *vertex_shader = ParseShader(EShLanguage::EShLangVertex, ReadTextFile(asset_root + "/shaders/imgui_meta.vert"));
        glslang::TShader *fragment_shader = ParseShader(EShLanguage::EShLangFragment, ReadTextFile(asset_root + "/shaders/imgui_meta.frag"));

        glslang::TProgram *program = new glslang::TProgram();
        program->addShader(vertex_shader);
        program->addShader(fragment_shader);

        EShMessages message = static_cast<EShMessages>(EShMessages::EShMsgDefault | EShMessages::EShMsgSpvRules | EShMessages::EShMsgVulkanRules | EShMessages::EShMsgAST | EShMessages::EShMsgKeepUncalled);
        if (!program->link(message))
        {
            std::cout << program->getInfoLog() << std::endl;
            std::cout << program->getInfoDebugLog() << std::endl;
        }

        program->mapIO();

        {
            {
                spv::SpvBuildLogger logger;
                std::vector<uint32_t> spirv_binary;
                glslang::SpvOptions spirv_options;
                spirv_options.disableOptimizer = true;

                glslang::GlslangToSpv(*program->getIntermediate(vertex_shader->getStage()), spirv_binary, &logger, &spirv_options);

                {
                    std::ostringstream disassembly_stream;
                    spv::Disassemble(disassembly_stream, spirv_binary); // SPIR-V反序列化为可读格式

                    std::cout << "spirv_options.validate: " << spirv_options.validate << std::endl;
                    for (auto logger_message : logger.getAllMessages())
                    {
                        std::cout << "logger_message: " << logger_message << std::endl;
                    }
                    std::cout << "spv::Disassemble: \n" << disassembly_stream.str() << std::endl;
                }
            }

            {
                spv::SpvBuildLogger logger;
                std::vector<uint32_t> spirv_binary;
                glslang::SpvOptions spirv_options;
                spirv_options.disableOptimizer = true;

                glslang::GlslangToSpv(*program->getIntermediate(fragment_shader->getStage()), spirv_binary, &logger, &spirv_options);

                {
                    std::ostringstream disassembly_stream;
                    spv::Disassemble(disassembly_stream, spirv_binary); // SPIR-V反序列化为可读格式

                    std::cout << "spirv_options.validate: " << spirv_options.validate << std::endl;
                    for (auto logger_message : logger.getAllMessages())
                    {
                        std::cout << "logger_message: " << logger_message << std::endl;
                    }
                    std::cout << "spv::Disassemble: \n" << disassembly_stream.str() << std::endl;
                }
            }
        }

        delete program;
        delete fragment_shader;
        delete vertex_shader;

        glslang::FinalizeProcess();
    }
    return 0;
}