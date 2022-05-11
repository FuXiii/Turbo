#pragma once
#ifndef TDESCRIPTOR_H
#define TDESCRIPTOR_H
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
typedef enum class TShaderDataType
{
    SHADER_DATA_TYPE_UNKNOWN,
    SHADER_DATA_TYPE_VOID,
    SHADER_DATA_TYPE_BOOLEAN,
    SHADER_DATA_TYPE_SBYTE,
    SHADER_DATA_TYPE_UBYTE,
    SHADER_DATA_TYPE_SHORT,
    SHADER_DATA_TYPE_USHORT,
    SHADER_DATA_TYPE_ATOMIC_COUNTER,
    SHADER_DATA_TYPE_INT,
    SHADER_DATA_TYPE_UINT,
    SHADER_DATA_TYPE_INT64,
    SHADER_DATA_TYPE_UINT64,
    SHADER_DATA_TYPE_HALF,
    SHADER_DATA_TYPE_FLOAT,
    SHADER_DATA_TYPE_DOUBLE,
    SHADER_DATA_TYPE_STRUCT,
    SHADER_DATA_TYPE_IMAGE,         // texture2d or Texture2D<T> :: VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
    SHADER_DATA_TYPE_SAMPLED_IMAGE, // sampler2d :: VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    SHADER_DATA_TYPE_SAMPLER,       // sampler or samplerState :: VK_DESCRIPTOR_TYPE_SAMPLER
    SHADER_DATA_TYPE_ACCELERATION_STRUCTURE,
    SHADER_DATA_TYPE_RAYQUERY,
} TShaderDataType;

typedef enum class TDescriptorType
{
    SAMPLER = 0,
    COMBINED_IMAGE_SAMPLER = 1,
    SAMPLED_IMAGE = 2,
    STORAGE_IMAGE = 3,
    UNIFORM_TEXEL_BUFFER = 4,
    STORAGE_TEXEL_BUFFER = 5,
    UNIFORM_BUFFER = 6,
    STORAGE_BUFFER = 7,
    UNIFORM_BUFFER_DYNAMIC = 8,
    STORAGE_BUFFER_DYNAMIC = 9,
    INPUT_ATTACHMENT = 10,
} TDescriptorType;

// Equivalent to VkDescriptorSetLayoutBinding
class TDescriptor : public Turbo::Core::TInfo
{
  private:
    TDescriptorType type;
    TShaderDataType dataType;
    uint32_t count;
    uint32_t set;
    uint32_t binding;
    std::string name;

  public:
    TDescriptor(TDescriptorType type, TShaderDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name);
    ~TDescriptor();

  public:
    TDescriptorType GetType();
    VkDescriptorType GetVkDescriptorType();
    TShaderDataType GetDataType();
    uint32_t GetCount();
    uint32_t GetSet();
    uint32_t GetBinding();
    const std::string &GetName();

  public:
    virtual std::string ToString() override;
};

class TStructMember : public Turbo::Core::TInfo
{
  private:
    TShaderDataType dataType;
    uint32_t width;
    uint32_t offset;
    uint32_t vecSize;
    uint32_t columns;
    uint32_t size;
    uint32_t count;
    uint32_t arrayStride;
    uint32_t matrixStride;
    std::string name;

  public:
    TStructMember(TShaderDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name);
    TStructMember(const TStructMember &object);
    ~TStructMember();

    TStructMember &operator=(const TStructMember &object);

  public:
    virtual std::string ToString() override;
};

// Equivalent to VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
class TUniformBufferDescriptor : public TDescriptor
{
  private:
    std::vector<TStructMember> members;

  public:
    TUniformBufferDescriptor(TShaderDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name, std::vector<TStructMember> &members);
    ~TUniformBufferDescriptor();
};

// Equivalent to VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
class TCombinedImageSamplerDescriptor : public TDescriptor
{
  private:
  public:
    TCombinedImageSamplerDescriptor(TShaderDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name);
    ~TCombinedImageSamplerDescriptor();
};
} // namespace Core
} // namespace Turbo

#endif // !TDESCRIPTOR_H