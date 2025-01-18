#include <iostream>
#include <cassert>
#include <stdexcept>
#include <ReadFile.h>

// #include <spirv_reflect.h>
#include "spirv_reflect.h"

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Include/Types.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

#include <common.h>

std::string asset_root(TURBO_ASSET_ROOT);

enum class ShaderType
{
    VERTEX,
    TESS_CONTROL,
    TESS_EVALUATION,
    GEOMETRY,
    FRAGMENT,
    COMPUTE,
    RAY_GEN,
    INTERSECT,
    ANY_HIT,
    CLOSEST_HIT,
    MISS,
    CALLABLE,
    TASK,
    MESH
};

std::vector<uint32_t> GlslToSpirV(const std::string &glsl, ShaderType type)
{
    auto ShaderTypeToEShLanguage = [](ShaderType type) {
        switch (type)
        {
        case ShaderType::VERTEX: {
            return EShLanguage::EShLangVertex;
        }
        break;
        case ShaderType::TESS_CONTROL: {
            return EShLanguage::EShLangTessControl;
        }
        break;
        case ShaderType::TESS_EVALUATION: {
            return EShLanguage::EShLangTessEvaluation;
        }
        break;
        case ShaderType::GEOMETRY: {
            return EShLanguage::EShLangGeometry;
        }
        break;
        case ShaderType::FRAGMENT: {
            return EShLanguage::EShLangFragment;
        }
        break;
        case ShaderType::COMPUTE: {
            return EShLanguage::EShLangCompute;
        }
        break;
        case ShaderType::RAY_GEN: {
            return EShLanguage::EShLangRayGen;
        }
        break;
        case ShaderType::INTERSECT: {
            return EShLanguage::EShLangIntersect;
        }
        break;
        case ShaderType::ANY_HIT: {
            return EShLanguage::EShLangAnyHit;
        }
        break;
        case ShaderType::CLOSEST_HIT: {
            return EShLanguage::EShLangClosestHit;
        }
        break;
        case ShaderType::MISS: {
            return EShLanguage::EShLangMiss;
        }
        break;
        case ShaderType::CALLABLE: {
            return EShLanguage::EShLangCallable;
        }
        break;
        case ShaderType::TASK: {
            return EShLanguage::EShLangTask;
        }
        break;
        case ShaderType::MESH: {
            return EShLanguage::EShLangMesh;
        }
        break;
        default: {
            return EShLanguage::EShLangVertex;
        }
        break;
        }
    };

    EShLanguage language = ShaderTypeToEShLanguage(type);

    std::vector<uint32_t> result;

    if (!glsl.empty() && glslang::InitializeProcess())
    {
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

                throw std::runtime_error("Error : GLSL parse error!");
            }

            return shader;
        };

        glslang::TShader *shader = ParseShader(language, glsl);

        glslang::TProgram *program = new glslang::TProgram();
        program->addShader(shader);

        EShMessages message = static_cast<EShMessages>(EShMessages::EShMsgDefault | EShMessages::EShMsgSpvRules | EShMessages::EShMsgVulkanRules | EShMessages::EShMsgAST | EShMessages::EShMsgKeepUncalled);
        if (!program->link(message))
        {
            std::cout << program->getInfoLog() << std::endl;
            std::cout << program->getInfoDebugLog() << std::endl;

            throw std::runtime_error("Error : GLSL link error!");
        }

        // program->mapIO();

        {
            spv::SpvBuildLogger logger;
            glslang::SpvOptions spirv_options;

            glslang::GlslangToSpv(*program->getIntermediate(shader->getStage()), result, &logger, &spirv_options);
            for (auto logger_message : logger.getAllMessages())
            {
                std::cout << "logger_message: " << logger_message << std::endl;
            }
        }

        delete program;
        delete shader;

        glslang::FinalizeProcess();
    }

    return result;
}

int main()
{
    std::cout << "Hello World" << std::endl;

    //auto spirv = GlslToSpirV(ReadTextFile(asset_root + "/shaders/imgui_meta.vert"), ShaderType::VERTEX);
    auto spirv = GlslToSpirV(ReadTextFile(asset_root + "/shaders/imgui_meta.frag"), ShaderType::FRAGMENT);

    SpvReflectShaderModule module;
    SpvReflectResult result = spvReflectCreateShaderModule(spirv.size() * sizeof(uint32_t), spirv.data(), &module);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    {
        std::cout << "<ShaderModule>" << std::endl;
        PrintModuleInfo(std::cout, module, "\t  - ");
        std::cout << "</ShaderModule>" << std::endl;
    }

    {
        uint32_t count = 0;
        result = spvReflectEnumerateDescriptorSets(&module, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectDescriptorSet *> sets(count);
        result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
        assert(result == SPV_REFLECT_RESULT_SUCCESS);

        std::cout << "<DescriptorSets>[" << count << "]" << std::endl;
        for (auto &set : sets)
        {
            std::cout << "\t{" << std::endl;
            PrintDescriptorSet(std::cout, *set, "\t  - ");
            std::cout << std::endl << "\t}" << std::endl;
        }
        std::cout << "</DescriptorSets>" << std::endl;
    }

    {
        uint32_t count = 0;
        result = spvReflectEnumerateDescriptorBindings(&module, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectDescriptorBinding *> bindings(count);
        result = spvReflectEnumerateDescriptorBindings(&module, &count, bindings.data());
        assert(result == SPV_REFLECT_RESULT_SUCCESS);

        std::cout << "<DescriptorBindings>[" << count << "]" << std::endl;
        for (auto &binding : bindings)
        {
            std::cout << "\t{" << std::endl;
            PrintDescriptorBinding(std::cout, *binding, "\t  - ");
            std::cout << std::endl << "\t}" << std::endl;
        }
        std::cout << "</DescriptorBindings>" << std::endl;
    }

    {
        uint32_t count = 0;
        result = spvReflectEnumerateInterfaceVariables(&module, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectInterfaceVariable *> interface_variables(count);
        result = spvReflectEnumerateInterfaceVariables(&module, &count, interface_variables.data());
        assert(result == SPV_REFLECT_RESULT_SUCCESS);

        std::cout << "<InterfaceVariables>[" << count << "]" << std::endl;
        for (auto &interface_variable : interface_variables)
        {
            std::cout << "\t{" << std::endl;
            PrintInterfaceVariable(std::cout, module.source_language, *interface_variable, "\t  - ");
            std::cout << std::endl << "\t}" << std::endl;
        }
        std::cout << "</InterfaceVariables>" << std::endl;
    }

    {
        uint32_t count = 0;
        result = spvReflectEnumerateInputVariables(&module, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectInterfaceVariable *> input_variables(count);
        result = spvReflectEnumerateInputVariables(&module, &count, input_variables.data());
        assert(result == SPV_REFLECT_RESULT_SUCCESS);

        std::cout << "<InputVariables>[" << count << "]" << std::endl;
        for (auto &input_variable : input_variables)
        {
            std::cout << "\t{" << std::endl;
            PrintInterfaceVariable(std::cout, module.source_language, *input_variable, "\t  - ");
            std::cout << std::endl << "\t}" << std::endl;
        }
        std::cout << "</InputVariables>" << std::endl;
    }

    {
        uint32_t count = 0;
        result = spvReflectEnumerateOutputVariables(&module, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectInterfaceVariable *> output_variables(count);
        result = spvReflectEnumerateOutputVariables(&module, &count, output_variables.data());

        std::cout << "<OutputVariables>[" << count << "]" << std::endl;
        for (auto &output_variable : output_variables)
        {
            std::cout << "\t{" << std::endl;
            PrintInterfaceVariable(std::cout, module.source_language, *output_variable, "\t  - ");
            std::cout << std::endl << "\t}" << std::endl;
        }
        std::cout << "</OutputVariables>" << std::endl;
    }

    {
        uint32_t count = 0;
        result = spvReflectEnumeratePushConstantBlocks(&module, &count, NULL);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        std::vector<SpvReflectBlockVariable *> push_constants(count);
        result = spvReflectEnumeratePushConstantBlocks(&module, &count, push_constants.data());
        assert(result == SPV_REFLECT_RESULT_SUCCESS);

        std::cout << "<PushConstants>[" << count << "]" << std::endl;
        for (auto &push_constant : push_constants)
        {
            std::cout << "\t{" << std::endl;
            {
                std::cout << "\t  - "
                          << "name : " << push_constant->name << std::endl;
                std::cout << "\t  - "
                          << "offset : " << push_constant->offset << std::endl;
                std::cout << "\t  - "
                          << "absolute_offset : " << push_constant->absolute_offset << std::endl;
                std::cout << "\t  - "
                          << "word_offset.offset : " << push_constant->word_offset.offset << std::endl;
                std::cout << "\t  - "
                          << "size : " << push_constant->size << std::endl;
                std::cout << "\t  - "
                          << "padded_size : " << push_constant->padded_size << std::endl;
            }
            std::cout << std::endl << "\t}" << std::endl;
        }
        std::cout << "</PushConstants>" << std::endl;
    }

    spvReflectDestroyShaderModule(&module);
    return 0;
}