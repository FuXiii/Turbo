#include <iostream>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <ReadFile.h>

// #include <spirv_reflect.h>
#include "spirv_reflect.h"

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Include/Types.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

#include <common.h>
#include <functional>
#include <filesystem>

std::string asset_root(TURBO_ASSET_ROOT);
std::string spirv_reflect_root(TURBO_SPIRV_REFLECT_ROOT);

using Version = std::pair<size_t, size_t>;

Version MakeVersion(size_t major = 1, size_t minor = 0)
{
    return std::make_pair(major, major);
}

enum class ShaderLanguage
{
    GLSL,
    HLSL
};

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

std::vector<uint32_t> ShaderToSpirV(const ShaderLanguage &language, const std::string &code, ShaderType type, const Version &vulkan = MakeVersion(), const Version &spirv = MakeVersion())
{
    auto check_vulkan_version = [&]() {
        if (vulkan.first == 1 && spirv.second < (glslang::EShTargetClientVersion::EShTargetClientVersionCount - 1))
        {
            return true;
        }

        return false;
    };

    auto check_spirv_version = [&]() {
        if (spirv.first == 1 && spirv.second < glslang::EShTargetLanguageVersion::EShTargetLanguageVersionCount)
        {
            return true;
        }

        return false;
    };

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

    auto LanguageToEShSource = [](const ShaderLanguage &language) {
        glslang::EShSource result = glslang::EShSource::EShSourceNone;
        switch (language)
        {
        case ShaderLanguage::GLSL: {
            result = glslang::EShSource::EShSourceGlsl;
        }
        break;
        case ShaderLanguage::HLSL: {
            result = glslang::EShSource::EShSourceHlsl;
        }
        break;
        }

        return result;
    };

    if (!check_vulkan_version())
    {
        throw std::runtime_error("Error : Vulkan version invalid!");
    }
    if (!check_spirv_version())
    {
        throw std::runtime_error("Error : SPIR-V version invalid!");
    }

    EShLanguage lang = ShaderTypeToEShLanguage(type);

    std::vector<uint32_t> result;

    if (!code.empty() && glslang::InitializeProcess())
    {
        auto ParseShader = [&](EShLanguage lang, const std::string &code) {
            const char *const_code_c_str = code.c_str();

            glslang::TShader *shader = new glslang::TShader(lang);
            shader->setStrings(&const_code_c_str, 1);
            shader->setEnvInput(LanguageToEShSource(language), lang, glslang::EShClient::EShClientVulkan, 100);
            // shader->setEnvClient(glslang::EShClient::EShClientVulkan, glslang::EShTargetClientVersion::EShTargetVulkan_1_0);
            shader->setEnvClient(glslang::EShClient::EShClientVulkan, static_cast<glslang::EShTargetClientVersion>(vulkan.first << 22 | vulkan.second << 12));
            // shader->setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, glslang::EShTargetLanguageVersion::EShTargetSpv_1_0);
            shader->setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, static_cast<glslang::EShTargetLanguageVersion>(spirv.first << 16 | spirv.second << 8));
            EShMessages message = static_cast<EShMessages>(EShMessages::EShMsgDefault | EShMessages::EShMsgSpvRules | EShMessages::EShMsgVulkanRules | EShMessages::EShMsgAST | EShMessages::EShMsgKeepUncalled);
            if (!shader->parse(GetDefaultResources(), 100, true, message))
            {
                std::cout << shader->getInfoLog() << std::endl;
                std::cout << shader->getInfoDebugLog() << std::endl;

                throw std::runtime_error("Error : GLSL parse error!");
            }

            return shader;
        };

        glslang::TShader *shader = ParseShader(lang, code);

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

int main(int argc, char **argv)
{
    std::cout << "argc: " << argc << std::endl;
    for (size_t index = 0; index < argc; index++)
    {
        std::cout << "argv[" << index << "]: " << argv[index] << std::endl;
    }

    if (argc >= 2)
    {
        auto dsa = (argv[1]);
        std::filesystem::path path = std::string(argv[1]);
        if (!path.empty() && std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
        {
            std::cout << "path.filename(): " << path.filename() << std::endl;
            std::cout << "path.extension(): " << path.extension() << std::endl;
        }
    }

    // auto spirv = ShaderToSpirV(ShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/imgui_meta.vert"), ShaderType::VERTEX);
    // auto spirv = ShaderToSpirV(ShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/imgui_meta.frag"), ShaderType::FRAGMENT, MakeVersion(1, 4), MakeVersion(1, 6));
    // auto spirv = ShaderToSpirV(ShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/volumetric_cloud.frag"), ShaderType::FRAGMENT);
    // auto spirv = ShaderToSpirV(ShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/BRDF.vert"), ShaderType::VERTEX);
    auto spirv = ShaderToSpirV(ShaderLanguage::GLSL, ReadTextFile(asset_root + "/shaders/BRDF.frag"), ShaderType::FRAGMENT);
    // auto spirv = ShaderToSpirV(ShaderLanguage::HLSL, ReadTextFile(spirv_reflect_root + "/examples/sample.hlsl"), ShaderType::VERTEX);

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

            {
                std::function<void(const SpvReflectBlockVariable *, const std::string &)> PrintAllBlock = [&](const SpvReflectBlockVariable *block, const std::string &ident) {
                    {
                        std::cout << ident << "= block->spirv_id: " << block->spirv_id << std::endl;
                        std::cout << ident << "= block->name: " << (block->name == nullptr ? "" : block->name) << std::endl;
                        std::cout << ident << "= block->offset: " << block->offset << std::endl;
                        std::cout << ident << "= block->absolute_offset: " << block->absolute_offset << std::endl;
                        std::cout << ident << "= block->size: " << block->size << std::endl;
                        std::cout << ident << "= block->padded_size: " << block->padded_size << std::endl;
                        std::cout << ident << "= block->decoration_flags: " << block->decoration_flags << std::endl;
                        {
                            std::cout << ident << "\t"
                                      << "= block->numeric.scalar.width: " << block->numeric.scalar.width << std::endl;
                            std::cout << ident << "\t"
                                      << "= block->numeric.scalar.signedness: " << block->numeric.scalar.signedness << std::endl;
                            std::cout << ident << "\t"
                                      << "= block->numeric.vector.component_count: " << block->numeric.vector.component_count << std::endl;
                            std::cout << ident << "\t"
                                      << "= block->numeric.matrix.column_count: " << block->numeric.matrix.column_count << std::endl;
                            std::cout << ident << "\t"
                                      << "= block->numeric.matrix.row_count: " << block->numeric.matrix.row_count << std::endl;
                            std::cout << ident << "\t"
                                      << "= block->numeric.matrix.stride: " << block->numeric.matrix.stride << std::endl;
                        }
                        std::cout << ident << "\t"
                                  << "*********************************" << std::endl;
                        {
                            std::cout << ident << "\t"
                                      << "= block->array.dims_count: " << block->array.dims_count << std::endl;
                            for (size_t i = 0; i < block->array.dims_count; i++)
                            {
                                std::cout << ident << "\t\t"
                                          << "= block->array.dims[" << i << "]: " << block->array.dims[i] << std::endl;
                            }
                            for (size_t i = 0; i < block->array.dims_count; i++)
                            {
                                std::cout << ident << "\t\t"
                                          << "= block->array.spec_constant_op_ids[" << i << "]: " << block->array.spec_constant_op_ids[i] << std::endl;
                            }
                            std::cout << ident << "\t"
                                      << "= block->array.stride: " << block->array.stride << std::endl;
                        }

                        std::cout << ident << "\t"
                                  << "= block->flags: " << block->flags << std::endl;
                        std::cout << ident << "\t"
                                  << "= block->word_offset.offset: " << block->word_offset.offset << std::endl;
                        std::cout << ident << "\t"
                                  << "= block->member_count: " << block->member_count << std::endl;

                        if (block->member_count > 0)
                        {
                            for (size_t index = 0; index < block->member_count; index++)
                            {
                                PrintAllBlock(block->members + index, ident + "\t\t");
                            }
                        }
                    }
                };

                std::cout << std::endl << "\t-------------------" << std::endl;
                std::cout << "\t  = binding->spirv_id: " << binding->spirv_id << std::endl;
                std::cout << "\t  = binding->input_attachment_index: " << binding->input_attachment_index << std::endl;
                {
                    std::cout << "\t\t  = binding->image.dim: " << binding->image.dim << std::endl;
                    std::cout << "\t\t  = binding->image.depth: " << binding->image.depth << std::endl;
                    std::cout << "\t\t  = binding->image.arrayed: " << binding->image.arrayed << std::endl;
                    std::cout << "\t\t  = binding->image.ms: " << binding->image.ms << std::endl;
                    std::cout << "\t\t  = binding->image.sampled: " << binding->image.sampled << std::endl;
                    std::cout << "\t\t  = binding->image.image_format: " << binding->image.image_format << std::endl;
                }
                std::cout << "\t\t*********************************" << std::endl;
                PrintAllBlock(&binding->block, "\t\t\t");
                std::cout << "\t  = binding->count: " << binding->count << std::endl;
                std::cout << "\t  = binding->type_description->type_name: " << (binding->type_description->type_name == nullptr ? "" : binding->type_description->type_name) << std::endl;

                std::cout << "\t+++++++++++++++++++" << std::endl;
            }
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
                          << "name : " << (push_constant->name == nullptr ? "" : push_constant->name) << std::endl;
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