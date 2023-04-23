#include "TShader.h"
#include "TDevice.h"
#include "TException.h"
#include "TVulkanAllocator.h"
#include "TVulkanLoader.h"

#include <fstream>

#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>

#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <glslang/Include/BaseTypes.h>
#include <glslang/Public/ShaderLang.h>

Turbo::Core::TInterface::TInterface(uint32_t location, TDescriptorDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name) : Turbo::Core::TStructMember(dataType, width, offset, vecSize, columns, size, count, arrayStride, matrixStride, name)
{
    this->location = location;
}

Turbo::Core::TInterface::~TInterface()
{
}

std::string Turbo::Core::TInterface::ToString()
{
    return Turbo::Core::TStructMember::ToString();
}

std::vector<char> Turbo::Core::ReadSpirVFile(const std::string &file)
{
    std::vector<char> result;

    std::ifstream is(file, std::ifstream::binary);
    if (is)
    {
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        result.resize(length);
        is.read(result.data(), length);
    }

    return result;
}

EShLanguage TShaderTypeToGlslangEShLanguage(Turbo::Core::TShaderType type)
{
    switch (type)
    {
    case Turbo::Core::TShaderType::VERTEX:
        return EShLanguage::EShLangVertex;
        break;
    case Turbo::Core::TShaderType::TESSELLATION_CONTROL:
        return EShLanguage::EShLangTessControl;
        break;
    case Turbo::Core::TShaderType::TESSELLATION_EVALUATION:
        return EShLanguage::EShLangTessEvaluation;
        break;
    case Turbo::Core::TShaderType::GEOMETRY:
        return EShLanguage::EShLangGeometry;
        break;
    case Turbo::Core::TShaderType::FRAGMENT:
        return EShLanguage::EShLangFragment;
        break;
    case Turbo::Core::TShaderType::COMPUTE:
        return EShLanguage::EShLangCompute;
        break;
    }

    return EShLanguage::EShLangVertex;
}

glslang::EShSource TShaderTypeToGlalangEShSource(Turbo::Core::TShaderLanguage language)
{
    switch (language)
    {
    case Turbo::Core::TShaderLanguage::GLSL:
        return glslang::EShSource::EShSourceGlsl;
        break;
    case Turbo::Core::TShaderLanguage::HLSL:
        return glslang::EShSource::EShSourceHlsl;
        break;
    case Turbo::Core::TShaderLanguage::SPIR:
        return glslang::EShSource::EShSourceNone;
        break;
    }
    return glslang::EShSource::EShSourceGlsl;
}

Turbo::Core::TDescriptorDataType SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(spirv_cross::SPIRType::BaseType baseType)
{
    switch (baseType)
    {
    case spirv_cross::SPIRType::BaseType::Unknown: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Void: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_VOID;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Boolean: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN;
    }
    break;
    case spirv_cross::SPIRType::BaseType::SByte: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_SBYTE;
    }
    break;
    case spirv_cross::SPIRType::BaseType::UByte: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UBYTE;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Short: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_SHORT;
    }
    break;
    case spirv_cross::SPIRType::BaseType::UShort: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_USHORT;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Int: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT;
    }
    break;
    case spirv_cross::SPIRType::BaseType::UInt: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Int64: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT64;
    }
    break;
    case spirv_cross::SPIRType::BaseType::UInt64: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT64;
    }
    break;
    case spirv_cross::SPIRType::BaseType::AtomicCounter: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_ATOMIC_COUNTER;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Half: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_HALF;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Float: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Double: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Struct: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_STRUCT;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Image: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_IMAGE;
    }
    break;
    case spirv_cross::SPIRType::BaseType::SampledImage: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_SAMPLED_IMAGE;
    }
    break;
    case spirv_cross::SPIRType::BaseType::Sampler: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_SAMPLER;
    }
    break;
    case spirv_cross::SPIRType::BaseType::AccelerationStructure: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_ACCELERATION_STRUCTURE;
    }
    break;
    case spirv_cross::SPIRType::BaseType::RayQuery: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_RAYQUERY;
    }
    break;
    default: {
        return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN;
    }
    break;
    }

    return Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN;
}

void Turbo::Core::TShader::InternalCreate()
{
    VkShaderModuleCreateInfo vk_shader_module_create_info = {};
    vk_shader_module_create_info.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vk_shader_module_create_info.pNext = nullptr;
    vk_shader_module_create_info.flags = 0;
    vk_shader_module_create_info.codeSize = this->size;
    vk_shader_module_create_info.pCode = this->code;

    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    VkResult result = this->device->GetDeviceDriver()->vkCreateShaderModule(vk_device, &vk_shader_module_create_info, allocator, &this->vkShaderModule);
    if (result != VkResult::VK_SUCCESS)
    {
        throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TShader::InternalCreate");
    }
}

void Turbo::Core::TShader::InternalDestroy()
{
    VkDevice vk_device = this->device->GetVkDevice();
    VkAllocationCallbacks *allocator = Turbo::Core::TVulkanAllocator::Instance()->GetVkAllocationCallbacks();
    this->device->GetDeviceDriver()->vkDestroyShaderModule(vk_device, this->vkShaderModule, allocator);
}

void Turbo::Core::TShader::InternalParseSpirV()
{
    std::vector<uint32_t> spirv;
    spirv.resize(this->size / sizeof(uint32_t));
    uint32_t *spirv_data = spirv.data();
    memcpy(spirv_data, this->code, this->size);

    spirv_cross::CompilerGLSL glsl(std::move(spirv));

    spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();
    opts.vulkan_semantics = true;
    glsl.set_common_options(opts);

    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    for (spirv_cross::Resource &sampled_image_item : resources.sampled_images)
    {
        // VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        spirv_cross::ID id = sampled_image_item.id;
        spirv_cross::TypeID type_id = sampled_image_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype; // TShaderDataType
        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = sampled_image_item.name;

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.

        switch (type.image.dim)
        {
        case spv::Dim::Dim1D: {
        }
        break;
        case spv::Dim::Dim2D: {
        }
        break;
        case spv::Dim::Dim3D: {
        }
        break;
        case spv::Dim::DimCube: {
            // cubemap combined image
        }
        break;
        case spv::Dim::DimRect: {
        }
        break;
        case spv::Dim::DimBuffer: {
        }
        break;
        case spv::Dim::DimSubpassData: {
        }
        break;
        default: {
        }
        break;
        }

        TCombinedImageSamplerDescriptor *combined_image_sampler_descriptor = new TCombinedImageSamplerDescriptor(this, descriptor_data_type, set, binding, count, name);
        this->combinedImageSamplerDescriptors.push_back(combined_image_sampler_descriptor);
    }

    for (spirv_cross::Resource &separate_image_item : resources.separate_images)
    {
        spirv_cross::ID id = separate_image_item.id;
        spirv_cross::TypeID type_id = separate_image_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype;

        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = separate_image_item.name;

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.

        if (type.image.dim == spv::DimBuffer)
        {
            // VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
        }

        // VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
        TSampledImageDescriptor *sampled_image_descriptor = new TSampledImageDescriptor(this, descriptor_data_type, set, binding, count, name);
        this->sampledImageDescriptors.push_back(sampled_image_descriptor);
    }

    for (spirv_cross::Resource &storage_image_item : resources.storage_images)
    {
        spirv_cross::ID id = storage_image_item.id;
        spirv_cross::TypeID type_id = storage_image_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype;

        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = storage_image_item.name;

        if (type.image.dim == spv::DimBuffer)
        {
            // VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
        }

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }
        // VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
        TStorageImageDescriptor *storage_image_descriptor = new TStorageImageDescriptor(this, descriptor_data_type, set, binding, count, name);
        this->storageImageDescriptors.push_back(storage_image_descriptor);
    }

    for (spirv_cross::Resource &separate_sampler_item : resources.separate_samplers)
    {
        // VK_DESCRIPTOR_TYPE_SAMPLER
        spirv_cross::ID id = separate_sampler_item.id;
        spirv_cross::TypeID type_id = separate_sampler_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype;

        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = separate_sampler_item.name;

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.

        TSamplerDescriptor *sampler_descriptor = new TSamplerDescriptor(this, descriptor_data_type, set, binding, count, name);
        this->samplerDescriptors.push_back(sampler_descriptor);
    }

    for (spirv_cross::Resource &uniform_buffers_item : resources.uniform_buffers)
    {
        // VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER or VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
        spirv_cross::ID id = uniform_buffers_item.id;
        spirv_cross::TypeID type_id = uniform_buffers_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype; // TShaderDataType
        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = uniform_buffers_item.name;

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // Size
        size_t size = glsl.get_declared_struct_size(type);

        std::vector<TStructMember> struct_members;

        // Querying structs
        uint32_t member_totolar_size = 0;
        if (base_type == spirv_cross::SPIRType::BaseType::Struct)
        {
            uint32_t member_count = type.member_types.size();
            for (uint32_t member_index = 0; member_index < member_count; member_index++)
            {
                auto &member = type.member_types[member_index];

                spirv_cross::SPIRType member_type = glsl.get_type(member);
                spirv_cross::SPIRType::BaseType member_base_type = member_type.basetype;
                Turbo::Core::TDescriptorDataType member_descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(member_base_type);

                uint32_t member_vecsize = member_type.vecsize;
                uint32_t member_columns = member_type.columns;
                uint32_t member_width = member_type.width;

                std::string member_name = glsl.get_member_name(type.self, member_index);

                size_t member_offset = glsl.type_struct_member_offset(type, member_index);

                size_t member_size = glsl.get_declared_struct_member_size(type, member_index);

                size_t member_array_dimension = member_type.array.size(); // array dimension
                uint32_t member_array_count = 1;
                uint32_t member_array_stride = 0;
                if (member_array_dimension > 0)
                {
                    member_array_stride = glsl.type_struct_member_array_stride(type, member_index);
                    member_array_count = member_type.array[0]; // just for one dimension.
                }

                uint32_t member_matrix_stride = 0;
                if (member_type.columns > 1)
                {
                    // Get bytes stride between columns (if column major), for float4x4 -> 16 bytes.
                    member_matrix_stride = glsl.type_struct_member_matrix_stride(type, member_index);
                }

                TStructMember struct_member(member_descriptor_data_type, member_width, member_offset, member_vecsize, member_columns, member_size, member_array_count, member_array_stride, member_matrix_stride, member_name);
                struct_members.push_back(struct_member);
            }
        }

        TUniformBufferDescriptor *uniform_buffer_descriptor = new TUniformBufferDescriptor(this, descriptor_data_type, set, binding, count, name, struct_members, size);
        this->uniformBufferDescriptors.push_back(uniform_buffer_descriptor);
    }

    for (spirv_cross::Resource &push_constant_buffer_item : resources.push_constant_buffers)
    {
        // VkPushConstantRange in VkPipelineLayoutCreateInfo
        spirv_cross::ID id = push_constant_buffer_item.id;
        spirv_cross::TypeID type_id = push_constant_buffer_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype; // TShaderDataType
        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = push_constant_buffer_item.name;

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // Size
        size_t size = glsl.get_declared_struct_size(type);

        std::vector<TStructMember> struct_members;

        // Querying structs
        uint32_t member_totolar_size = 0;
        if (base_type == spirv_cross::SPIRType::BaseType::Struct)
        {
            uint32_t member_count = type.member_types.size();
            for (uint32_t member_index = 0; member_index < member_count; member_index++)
            {
                auto &member = type.member_types[member_index];

                spirv_cross::SPIRType member_type = glsl.get_type(member);
                spirv_cross::SPIRType::BaseType member_base_type = member_type.basetype;
                Turbo::Core::TDescriptorDataType member_descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(member_base_type);

                uint32_t member_vecsize = member_type.vecsize;
                uint32_t member_columns = member_type.columns;
                uint32_t member_width = member_type.width;

                std::string member_name = glsl.get_member_name(type.self, member_index);

                size_t member_offset = glsl.type_struct_member_offset(type, member_index);

                size_t member_size = glsl.get_declared_struct_member_size(type, member_index);

                size_t member_array_dimension = member_type.array.size(); // array dimension
                uint32_t member_array_count = 1;
                uint32_t member_array_stride = 0;
                if (member_array_dimension > 0)
                {
                    member_array_stride = glsl.type_struct_member_array_stride(type, member_index);
                    member_array_count = member_type.array[0]; // just for one dimension.
                }

                uint32_t member_matrix_stride = 0;
                if (member_type.columns > 1)
                {
                    // Get bytes stride between columns (if column major), for float4x4 -> 16 bytes.
                    member_matrix_stride = glsl.type_struct_member_matrix_stride(type, member_index);
                }

                TStructMember struct_member(member_descriptor_data_type, member_width, member_offset, member_vecsize, member_columns, member_size, member_array_count, member_array_stride, member_matrix_stride, member_name);
                struct_members.push_back(struct_member);
            }
        }

        TPushConstantDescriptor *push_constant_descriptor = new TPushConstantDescriptor(this, descriptor_data_type, count, name, struct_members, size);
        this->pushConstantDescriptors.push_back(push_constant_descriptor);
    }

    for (spirv_cross::Resource &subpass_input_item : resources.subpass_inputs)
    {
        // VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
        spirv_cross::ID id = subpass_input_item.id;
        spirv_cross::TypeID type_id = subpass_input_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype;

        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t set = glsl.get_decoration(id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl.get_decoration(id, spv::DecorationBinding);

        // name
        std::string name = subpass_input_item.name;

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.

        uint32_t attachment_index = glsl.get_decoration(id, spv::DecorationInputAttachmentIndex);

        TInputAttachmentDescriptor *input_attachment_descriptor = new TInputAttachmentDescriptor(attachment_index, this, descriptor_data_type, set, binding, count, name);
        this->inputAttachmentDescriptors.push_back(input_attachment_descriptor);
    }

    for (spirv_cross::Resource &storage_buffer_item : resources.storage_buffers)
    {
        // VK_DESCRIPTOR_TYPE_STORAGE_BUFFER or VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
        spirv_cross::ID id = storage_buffer_item.id;
        spirv_cross::TypeID type_id = storage_buffer_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype;
    }

    for (spirv_cross::Resource &stage_input_item : resources.stage_inputs)
    {
        // input
        spirv_cross::ID id = stage_input_item.id;
        spirv_cross::TypeID type_id = stage_input_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype; // TShaderDataType
        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t location = glsl.get_decoration(id, spv::DecorationLocation);

        // name
        std::string name = stage_input_item.name;

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.
        uint32_t width = type.width;

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // Size
        size_t size = width * vec_size / 8;
        if (base_type == spirv_cross::SPIRType::BaseType::Struct)
        {
            size = glsl.get_declared_struct_size(type);
        }

        Turbo::Core::TInterface in_interface(location, descriptor_data_type, width, 0, vec_size, colums, size, count, 0, 0, name);
        this->inputs.push_back(in_interface);
    }

    for (spirv_cross::Resource &stage_output_item : resources.stage_outputs)
    {
        // output
        spirv_cross::ID id = stage_output_item.id;
        spirv_cross::TypeID type_id = stage_output_item.type_id;

        spirv_cross::SPIRType type = glsl.get_type(type_id);
        spirv_cross::SPIRType::BaseType base_type = type.basetype; // TShaderDataType
        Turbo::Core::TDescriptorDataType descriptor_data_type = SpirvCrossSPIRTypeBaseTypeToTDescriptorDataType(base_type);

        // set and binding
        uint32_t location = glsl.get_decoration(id, spv::DecorationLocation);

        // name
        std::string name = stage_output_item.name;

        // vector and matrices
        uint32_t vec_size = type.vecsize; // size of vec
        uint32_t colums = type.columns;   // 1 column means it's a vector.
        uint32_t width = type.width;

        // Arrary
        size_t array_dimension = type.array.size(); // array dimension
        uint32_t count = 1;
        if (array_dimension > 0)
        {
            count = type.array[0]; // just for one dimension.
        }

        // Size
        size_t size = width * vec_size / 8;
        if (base_type == spirv_cross::SPIRType::BaseType::Struct)
        {
            size = glsl.get_declared_struct_size(type);
        }

        Turbo::Core::TInterface out_interface(location, descriptor_data_type, width, 0, vec_size, colums, size, count, 0, 0, name);
        this->outputs.push_back(out_interface);
    }
}

Turbo::Core::TShader::TShader(TDevice *device, TShaderType type, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint)
{
    // glslang to spir-v
    if (device != nullptr)
    {
        this->device = device;
        this->type = type;
        this->language = language;
        this->entryPoint = entryPoint;

        bool is_initialize_process = glslang::InitializeProcess();
        if (!is_initialize_process)
        {
            throw Turbo::Core::TException(TResult::INITIALIZATION_FAILED, "Turbo::Core::TShader::TShader::glslang::InitializeProcess()");
        }

        TBuiltInResource resources = {};
        {
            resources.maxLights = 32;
            resources.maxClipPlanes = 6;
            resources.maxTextureUnits = 32;
            resources.maxTextureCoords = 32;
            resources.maxVertexAttribs = 64;
            resources.maxVertexUniformComponents = 4096;
            resources.maxVaryingFloats = 64;
            resources.maxVertexTextureImageUnits = 32;
            resources.maxCombinedTextureImageUnits = 80;
            resources.maxTextureImageUnits = 32;
            resources.maxFragmentUniformComponents = 4096;
            resources.maxDrawBuffers = 32;
            resources.maxVertexUniformVectors = 128;
            resources.maxVaryingVectors = 8;
            resources.maxFragmentUniformVectors = 16;
            resources.maxVertexOutputVectors = 16;
            resources.maxFragmentInputVectors = 15;
            resources.minProgramTexelOffset = -8;
            resources.maxProgramTexelOffset = 7;
            resources.maxClipDistances = 8;
            resources.maxComputeWorkGroupCountX = 65535;
            resources.maxComputeWorkGroupCountY = 65535;
            resources.maxComputeWorkGroupCountZ = 65535;
            resources.maxComputeWorkGroupSizeX = 1024;
            resources.maxComputeWorkGroupSizeY = 1024;
            resources.maxComputeWorkGroupSizeZ = 64;
            resources.maxComputeUniformComponents = 1024;
            resources.maxComputeTextureImageUnits = 16;
            resources.maxComputeImageUniforms = 8;
            resources.maxComputeAtomicCounters = 8;
            resources.maxComputeAtomicCounterBuffers = 1;
            resources.maxVaryingComponents = 60;
            resources.maxVertexOutputComponents = 64;
            resources.maxGeometryInputComponents = 64;
            resources.maxGeometryOutputComponents = 128;
            resources.maxFragmentInputComponents = 128;
            resources.maxImageUnits = 8;
            resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
            resources.maxCombinedShaderOutputResources = 8;
            resources.maxImageSamples = 0;
            resources.maxVertexImageUniforms = 0;
            resources.maxTessControlImageUniforms = 0;
            resources.maxTessEvaluationImageUniforms = 0;
            resources.maxGeometryImageUniforms = 0;
            resources.maxFragmentImageUniforms = 8;
            resources.maxCombinedImageUniforms = 8;
            resources.maxGeometryTextureImageUnits = 16;
            resources.maxGeometryOutputVertices = 256;
            resources.maxGeometryTotalOutputComponents = 1024;
            resources.maxGeometryUniformComponents = 1024;
            resources.maxGeometryVaryingComponents = 64;
            resources.maxTessControlInputComponents = 128;
            resources.maxTessControlOutputComponents = 128;
            resources.maxTessControlTextureImageUnits = 16;
            resources.maxTessControlUniformComponents = 1024;
            resources.maxTessControlTotalOutputComponents = 4096;
            resources.maxTessEvaluationInputComponents = 128;
            resources.maxTessEvaluationOutputComponents = 128;
            resources.maxTessEvaluationTextureImageUnits = 16;
            resources.maxTessEvaluationUniformComponents = 1024;
            resources.maxTessPatchComponents = 120;
            resources.maxPatchVertices = 32;
            resources.maxTessGenLevel = 64;
            resources.maxViewports = 16;
            resources.maxVertexAtomicCounters = 0;
            resources.maxTessControlAtomicCounters = 0;
            resources.maxTessEvaluationAtomicCounters = 0;
            resources.maxGeometryAtomicCounters = 0;
            resources.maxFragmentAtomicCounters = 8;
            resources.maxCombinedAtomicCounters = 8;
            resources.maxAtomicCounterBindings = 1;
            resources.maxVertexAtomicCounterBuffers = 0;
            resources.maxTessControlAtomicCounterBuffers = 0;
            resources.maxTessEvaluationAtomicCounterBuffers = 0;
            resources.maxGeometryAtomicCounterBuffers = 0;
            resources.maxFragmentAtomicCounterBuffers = 1;
            resources.maxCombinedAtomicCounterBuffers = 1;
            resources.maxAtomicCounterBufferSize = 16384;
            resources.maxTransformFeedbackBuffers = 4;
            resources.maxTransformFeedbackInterleavedComponents = 64;
            resources.maxCullDistances = 8;
            resources.maxCombinedClipAndCullDistances = 8;
            resources.maxSamples = 4;
            resources.maxMeshOutputVerticesNV = 256;
            resources.maxMeshOutputPrimitivesNV = 512;
            resources.maxMeshWorkGroupSizeX_NV = 32;
            resources.maxMeshWorkGroupSizeY_NV = 1;
            resources.maxMeshWorkGroupSizeZ_NV = 1;
            resources.maxTaskWorkGroupSizeX_NV = 32;
            resources.maxTaskWorkGroupSizeY_NV = 1;
            resources.maxTaskWorkGroupSizeZ_NV = 1;
            resources.maxMeshViewCountNV = 4;
            resources.limits.nonInductiveForLoops = 1;
            resources.limits.whileLoops = 1;
            resources.limits.doWhileLoops = 1;
            resources.limits.generalUniformIndexing = 1;
            resources.limits.generalAttributeMatrixVectorIndexing = 1;
            resources.limits.generalVaryingIndexing = 1;
            resources.limits.generalSamplerIndexing = 1;
            resources.limits.generalVariableIndexing = 1;
            resources.limits.generalConstantMatrixVectorIndexing = 1;
        }

        EShLanguage esh_language = TShaderTypeToGlslangEShLanguage(type);

        glslang::TShader shader_glslang(esh_language);

        glslang::EShSource esh_source = TShaderTypeToGlalangEShSource(language);

        const char *code_c_str = code.c_str();
        shader_glslang.setStrings(&code_c_str, 1);
        shader_glslang.setEnvInput(esh_source, esh_language, glslang::EShClient::EShClientVulkan, 100);

        glslang::EShTargetClientVersion esh_target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
        glslang::EShTargetLanguageVersion esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;
        Turbo::Core::TVersion vulkan_version = device->GetPhysicalDevice()->GetInstance()->GetVulkanVersion().GetValidVulkanVersion();
        if (vulkan_version == Turbo::Core::TVersion(1, 0, 0, 0))
        {
            esh_target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
            esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;
        }
        else if (vulkan_version == Turbo::Core::TVersion(1, 1, 0, 0))
        {
            esh_target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_1;
            esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_3;
        }
        else if (vulkan_version == Turbo::Core::TVersion(1, 2, 0, 0))
        {
            esh_target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_2;
            esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_5;
        }
        else if (vulkan_version == Turbo::Core::TVersion(1, 3, 0, 0))
        {
            esh_target_client_version = glslang::EShTargetClientVersion::EShTargetVulkan_1_3;
            esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_6;
        }
        if (device->IsEnabledExtension(Turbo::Core::TExtensionType::VK_KHR_SPIRV_1_4))
        {
            if ((uint32_t)(esh_target_language_version) < (uint32_t)(glslang::EShTargetLanguageVersion::EShTargetSpv_1_4))
            {
                esh_target_language_version = glslang::EShTargetLanguageVersion::EShTargetSpv_1_4;
            }
        }
        shader_glslang.setEnvClient(glslang::EShClient::EShClientVulkan, esh_target_client_version);
        shader_glslang.setEnvTarget(glslang::EShTargetLanguage::EShTargetSpv, esh_target_language_version);
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        DirStackFileIncluder dir_stack_file_includer = {};
        if (includePaths.size() > 0)
        {
            for (const std::string &include_path_item : includePaths)
            {
                dir_stack_file_includer.pushExternalLocalDirectory(include_path_item);
            }

            if (!shader_glslang.parse(&resources /*glslang::DefaultTBuiltInResource*/, 100, false, messages, dir_stack_file_includer))
            {
                std::string log_messgae(shader_glslang.getInfoLog());
                throw Turbo::Core::TException(TResult::SHADER_PARSE_FAILED, "Turbo::Core::TShader::TShader", log_messgae);
            }
        }
        else
        {
            if (!shader_glslang.parse(&resources /*glslang::DefaultTBuiltInResource*/, 100, false, messages))
            {
                std::string log_messgae(shader_glslang.getInfoLog());
                throw Turbo::Core::TException(TResult::SHADER_PARSE_FAILED, "Turbo::Core::TShader::TShader", log_messgae);
            }
        }

        glslang::TProgram program;
        program.addShader(&shader_glslang);
        if (!program.link(messages))
        {
            std::string log_messgae(program.getInfoLog());
            throw Turbo::Core::TException(TResult::SHADER_LINK_FAILED, "Turbo::Core::TShader::TShader", log_messgae);
        }

        std::vector<uint32_t> shader_spir;
        glslang::GlslangToSpv(*(program.getIntermediate(esh_language)), shader_spir);

        glslang::FinalizeProcess();

        this->size = shader_spir.size() * sizeof(uint32_t);
        this->code = (uint32_t *)malloc(size);
        memcpy(this->code, shader_spir.data(), this->size);

        this->InternalCreate();
        this->InternalParseSpirV();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TShader::TShader");
    }
}

Turbo::Core::TShader::TShader(TDevice *device, TShaderType type, size_t size, uint32_t *code, const std::string &entryPoint)
{
    if (device != nullptr || size == 0 || code == nullptr)
    {
        this->device = device;
        this->type = type;
        this->language = TShaderLanguage::SPIR;
        this->size = size;
        this->code = (uint32_t *)malloc(size);
        this->entryPoint = entryPoint;
        memcpy(this->code, code, this->size);

        this->InternalCreate();
        this->InternalParseSpirV();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TShader::TShader");
    }
}

Turbo::Core::TShader::~TShader()
{
    this->InternalDestroy();

    for (TUniformBufferDescriptor *uniform_buffer_descriptor_item : this->uniformBufferDescriptors)
    {
        delete uniform_buffer_descriptor_item;
    }
    this->uniformBufferDescriptors.clear();

    for (TCombinedImageSamplerDescriptor *combined_image_sampler_descriptor_item : this->combinedImageSamplerDescriptors)
    {
        delete combined_image_sampler_descriptor_item;
    }
    this->combinedImageSamplerDescriptors.clear();

    for (TSampledImageDescriptor *sampled_image_descriptor_item : this->sampledImageDescriptors)
    {
        delete sampled_image_descriptor_item;
    }
    this->sampledImageDescriptors.clear();

    for (TSamplerDescriptor *sampler_descriptor_item : this->samplerDescriptors)
    {
        delete sampler_descriptor_item;
    }
    this->samplerDescriptors.clear();

    for (TPushConstantDescriptor *push_constant_descriptor_item : this->pushConstantDescriptors)
    {
        delete push_constant_descriptor_item;
    }
    this->pushConstantDescriptors.clear();

    for (TInputAttachmentDescriptor *input_attachment_descriptor_item : this->inputAttachmentDescriptors)
    {
        delete input_attachment_descriptor_item;
    }
    this->inputAttachmentDescriptors.clear();

    for (TStorageImageDescriptor *storage_image_descriptor_item : this->storageImageDescriptors)
    {
        delete storage_image_descriptor_item;
    }
    this->storageImageDescriptors.clear();

    free(this->code);
    this->code = nullptr;
    this->size = 0;
}

Turbo::Core::TDevice *Turbo::Core::TShader::GetDevice()
{
    return this->device;
}

VkShaderStageFlags Turbo::Core::TShader::GetVkShaderStageFlags()
{
    switch (this->type)
    {
    case Turbo::Core::TShaderType::VERTEX: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
    }
    break;
    case Turbo::Core::TShaderType::TESSELLATION_CONTROL: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    break;
    case Turbo::Core::TShaderType::TESSELLATION_EVALUATION: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    break;
    case Turbo::Core::TShaderType::GEOMETRY: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    break;
    case Turbo::Core::TShaderType::FRAGMENT: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    break;
    case Turbo::Core::TShaderType::COMPUTE: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
    }
    break;
    }

    return VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

VkShaderStageFlagBits Turbo::Core::TShader::GetVkShaderStageFlagBits()
{
    switch (this->type)
    {
    case Turbo::Core::TShaderType::VERTEX: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
    }
    break;
    case Turbo::Core::TShaderType::TESSELLATION_CONTROL: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    break;
    case Turbo::Core::TShaderType::TESSELLATION_EVALUATION: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    break;
    case Turbo::Core::TShaderType::GEOMETRY: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    break;
    case Turbo::Core::TShaderType::FRAGMENT: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    break;
    case Turbo::Core::TShaderType::COMPUTE: {
        return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
    }
    break;
    }

    return VkShaderStageFlagBits::VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

VkShaderModule Turbo::Core::TShader::GetVkShaderModule()
{
    return this->vkShaderModule;
}

const std::string &Turbo::Core::TShader::GetEntryPoint()
{
    return this->entryPoint;
}

const std::vector<Turbo::Core::TUniformBufferDescriptor *> &Turbo::Core::TShader::GetUniformBufferDescriptors()
{
    return this->uniformBufferDescriptors;
}

const std::vector<Turbo::Core::TCombinedImageSamplerDescriptor *> &Turbo::Core::TShader::GetCombinedImageSamplerDescriptors()
{
    return this->combinedImageSamplerDescriptors;
}

const std::vector<Turbo::Core::TSampledImageDescriptor *> &Turbo::Core::TShader::GetSampledImageDescriptors()
{
    return this->sampledImageDescriptors;
}

const std::vector<Turbo::Core::TSamplerDescriptor *> &Turbo::Core::TShader::GetSamplerDescriptors()
{
    return this->samplerDescriptors;
}

const std::vector<Turbo::Core::TPushConstantDescriptor *> &Turbo::Core::TShader::GetPushConstantDescriptors()
{
    return this->pushConstantDescriptors;
}

const std::vector<Turbo::Core::TInputAttachmentDescriptor *> &Turbo::Core::TShader::GetInputAttachmentDescriptors()
{
    return this->inputAttachmentDescriptors;
}

const std::vector<Turbo::Core::TStorageImageDescriptor *> &Turbo::Core::TShader::GetStorageImageDescriptors()
{
    return this->storageImageDescriptors;
}

std::vector<Turbo::Core::TInterface> Turbo::Core::TShader::GetInputs()
{
    return this->inputs;
}

std::vector<Turbo::Core::TInterface> Turbo::Core::TShader::GetOutputs()
{
    return this->outputs;
}

Turbo::Core::TShaderType Turbo::Core::TShader::GetType()
{
    return this->type;
}

std::string Turbo::Core::TShader::ToString()
{
    std::vector<uint32_t> spirv;
    spirv.resize(this->size / sizeof(uint32_t));
    uint32_t *spirv_data = spirv.data();
    memcpy(spirv_data, this->code, this->size);

    spirv_cross::CompilerGLSL glsl(std::move(spirv));
    spirv_cross::CompilerGLSL::Options opts = glsl.get_common_options();
    opts.vulkan_semantics = true;
    glsl.set_common_options(opts);
    return glsl.compile();
}

Turbo::Core::TVertexShader::TVertexShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::VERTEX, language, code, includePaths, entryPoint)
{
}

Turbo::Core::TVertexShader::TVertexShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::VERTEX, size, code, entryPoint)
{
}

Turbo::Core::TFragmentShader::TFragmentShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::FRAGMENT, language, code, includePaths, entryPoint)
{
}

Turbo::Core::TFragmentShader::TFragmentShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::FRAGMENT, size, code, entryPoint)
{
}

Turbo::Core::TComputeShader::TComputeShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::COMPUTE, language, code, includePaths, entryPoint)
{
}

Turbo::Core::TComputeShader::TComputeShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::COMPUTE, size, code, entryPoint)
{
}

Turbo::Core::TTessellationControlShader::TTessellationControlShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::TESSELLATION_CONTROL, language, code, includePaths, entryPoint)
{
}

Turbo::Core::TTessellationControlShader::TTessellationControlShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::TESSELLATION_CONTROL, size, code, entryPoint)
{
}

Turbo::Core::TTessellationEvaluationShader::TTessellationEvaluationShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::TESSELLATION_EVALUATION, language, code, includePaths, entryPoint)
{
}

Turbo::Core::TTessellationEvaluationShader::TTessellationEvaluationShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::TESSELLATION_EVALUATION, size, code, entryPoint)
{
}

Turbo::Core::TGeometryShader::TGeometryShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::GEOMETRY, language, code, includePaths, entryPoint)
{
}

Turbo::Core::TGeometryShader::TGeometryShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint) : Turbo::Core::TShader(device, TShaderType::GEOMETRY, size, code, entryPoint)
{
}