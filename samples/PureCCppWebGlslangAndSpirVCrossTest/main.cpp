
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <glslang/Include/BaseTypes.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <iostream>
//#include <string>
#include <vector>

#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>

std::vector<uint32_t> ShaderCodeToSpirV(glslang::EShSource language, EShLanguage shaderType, const std::string &code)
{
    std::vector<uint32_t> shader_spir;

    bool is_initialize_process = glslang::InitializeProcess();
    if (!is_initialize_process)
    {
        return shader_spir;
    }

    EShLanguage esh_language = shaderType;

    glslang::TShader shader_glslang(esh_language);

    glslang::EShSource esh_source = language;

    const char *code_c_str = code.c_str();
    shader_glslang.setStrings(&code_c_str, 1);
    shader_glslang.setEnvInput(esh_source, esh_language, glslang::EShClient::EShClientVulkan, 100);

    glslang::EShTargetClientVersion esh_target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
    glslang::EShTargetLanguageVersion esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;

    shader_glslang.setEnvClient(glslang::EShClient::EShClientVulkan, esh_target_client_version);
    shader_glslang.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, esh_target_language_version);
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    if (!shader_glslang.parse(GetDefaultResources(), 100, false, messages))
    {
        std::string log_messgae(shader_glslang.getInfoLog());
        std::cout << "Parse Error:" << log_messgae << std::endl;
        glslang::FinalizeProcess();
        return shader_spir;
    }

    glslang::TProgram program;
    program.addShader(&shader_glslang);
    if (!program.link(messages))
    {
        std::string log_messgae(program.getInfoLog());
        std::cout << "Link Error:" << log_messgae << std::endl;
        glslang::FinalizeProcess();
        return shader_spir;
    }

    glslang::GlslangToSpv(*(program.getIntermediate(esh_language)), shader_spir);

    glslang::FinalizeProcess();

    return shader_spir;
}

#include <glslang/Include/glslang_c_interface.h>

// Required for use of glslang_default_resource
#include <glslang/Public/resource_limits_c.h>

typedef struct SpirVBinary
{
    uint32_t *words; // SPIR-V words
    int size;        // number of words in SPIR-V binary
} SpirVBinary;

SpirVBinary compileShaderToSPIRV_Vulkan(glslang_stage_t stage, const char *shaderSource, const char *fileName)
{
    const glslang_input_t input = {
        .language = GLSLANG_SOURCE_GLSL,
        .stage = stage,
        .client = GLSLANG_CLIENT_VULKAN,
        .client_version = GLSLANG_TARGET_VULKAN_1_0,
        .target_language = GLSLANG_TARGET_SPV,
        .target_language_version = GLSLANG_TARGET_SPV_1_0,
        .code = shaderSource,
        .default_version = 100,
        .default_profile = GLSLANG_NO_PROFILE,
        .force_default_version_and_profile = false,
        .forward_compatible = false,
        .messages = GLSLANG_MSG_DEFAULT_BIT,
        .resource = glslang_default_resource(),
    };

    glslang_shader_t *shader = glslang_shader_create(&input);

    SpirVBinary bin = {
        .words = NULL,
        .size = 0,
    };
    if (!glslang_shader_preprocess(shader, &input))
    {
        printf("GLSL preprocessing failed %s\n", fileName);
        printf("%s\n", glslang_shader_get_info_log(shader));
        printf("%s\n", glslang_shader_get_info_debug_log(shader));
        printf("%s\n", input.code);
        glslang_shader_delete(shader);
        return bin;
    }

    if (!glslang_shader_parse(shader, &input))
    {
        printf("GLSL parsing failed %s\n", fileName);
        printf("%s\n", glslang_shader_get_info_log(shader));
        printf("%s\n", glslang_shader_get_info_debug_log(shader));
        printf("%s\n", glslang_shader_get_preprocessed_code(shader));
        glslang_shader_delete(shader);
        return bin;
    }

    glslang_program_t *program = glslang_program_create();
    glslang_program_add_shader(program, shader);

    if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
    {
        printf("GLSL linking failed %s\n", fileName);
        printf("%s\n", glslang_program_get_info_log(program));
        printf("%s\n", glslang_program_get_info_debug_log(program));
        glslang_program_delete(program);
        glslang_shader_delete(shader);
        return bin;
    }

    glslang_program_SPIRV_generate(program, stage);

    bin.size = glslang_program_SPIRV_get_size(program);
    bin.words = (uint32_t *)malloc(bin.size * sizeof(uint32_t));
    glslang_program_SPIRV_get(program, bin.words);

    const char *spirv_messages = glslang_program_SPIRV_get_messages(program);
    if (spirv_messages)
        printf("(%s) %s\b", fileName, spirv_messages);

    glslang_program_delete(program);
    glslang_shader_delete(shader);

    return bin;
}

const std::string VERTEX_SHADER_CODE =

    "#version 450 core\n"
    "layout(location = 0) in vec3 inPos;\n"
    "layout(location = 1) in vec3 inColor;\n"
    "layout(location = 0) out vec3 outColor;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(inPos, 1);\n"
    "    outColor = inColor;\n"
    "}";

int main()
{
    std::vector<uint32_t> sipriv_code =
        ShaderCodeToSpirV(glslang::EShSource::EShSourceGlsl, EShLanguage::EShLangVertex, VERTEX_SHADER_CODE);
    std::cout << "sipriv_code size:" << sipriv_code.size() << std::endl;

    std::string spirv_code_bin_str;
    for (uint32_t code_bin : sipriv_code)
    {
        spirv_code_bin_str += std::to_string(code_bin);
    }
    std::cout << spirv_code_bin_str << std::endl;

    spirv_cross::CompilerGLSL glsl(sipriv_code);
    spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();
    opts.vulkan_semantics = true;
    glsl.set_common_options(opts);

    std::string shader_code = glsl.compile();

    std::cout << "shader_code:" << std::endl;
    std::cout << shader_code << std::endl;

    return 0;
}