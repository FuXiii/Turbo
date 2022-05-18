#pragma once
#ifndef TDESCRIPTOR_H
#define TDESCRIPTOR_H
#include "TInfo.h"

namespace Turbo
{
namespace Core
{

class TShader;

typedef enum class TDescriptorDataType
{
    DESCRIPTOR_DATA_TYPE_UNKNOWN,
    DESCRIPTOR_DATA_TYPE_VOID,
    DESCRIPTOR_DATA_TYPE_BOOLEAN,
    DESCRIPTOR_DATA_TYPE_SBYTE,
    DESCRIPTOR_DATA_TYPE_UBYTE,
    DESCRIPTOR_DATA_TYPE_SHORT,
    DESCRIPTOR_DATA_TYPE_USHORT,
    DESCRIPTOR_DATA_TYPE_ATOMIC_COUNTER,
    DESCRIPTOR_DATA_TYPE_INT,
    DESCRIPTOR_DATA_TYPE_UINT,
    DESCRIPTOR_DATA_TYPE_INT64,
    DESCRIPTOR_DATA_TYPE_UINT64,
    DESCRIPTOR_DATA_TYPE_HALF,
    DESCRIPTOR_DATA_TYPE_FLOAT,
    DESCRIPTOR_DATA_TYPE_DOUBLE,
    DESCRIPTOR_DATA_TYPE_STRUCT,
    DESCRIPTOR_DATA_TYPE_IMAGE,         // texture2d or Texture2D<T> :: VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
    DESCRIPTOR_DATA_TYPE_SAMPLED_IMAGE, // sampler2d :: VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    DESCRIPTOR_DATA_TYPE_SAMPLER,       // sampler or samplerState :: VK_DESCRIPTOR_TYPE_SAMPLER
    DESCRIPTOR_DATA_TYPE_ACCELERATION_STRUCTURE,
    DESCRIPTOR_DATA_TYPE_RAYQUERY,
} TDescriptorDataType;

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
    TShader *shader;

    TDescriptorType type;
    TDescriptorDataType dataType;
    uint32_t count;
    uint32_t set;
    uint32_t binding;
    std::string name;

  public:
    TDescriptor(TShader *shader, TDescriptorType type, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name);
    ~TDescriptor();

  public:
    TDescriptorType GetType();
    VkDescriptorType GetVkDescriptorType();
    TDescriptorDataType GetDataType();
    uint32_t GetCount();
    uint32_t GetSet();
    uint32_t GetBinding();
    const std::string &GetName();

    TShader *GetShader();

  public:
    virtual std::string ToString() override;
};

class TStructMember : public Turbo::Core::TInfo
{
  private:
    TDescriptorDataType dataType;
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
    TStructMember(TDescriptorDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name);
    TStructMember(const TStructMember &object);
    ~TStructMember();

    TStructMember &operator=(const TStructMember &object);

  public:
    virtual std::string ToString() override;
};

// unused sets in the layout, with zero descriptors.
class TNaNDescriptor : public TDescriptor
{
  public:
    TNaNDescriptor(uint32_t set);
    ~TNaNDescriptor();
};

// Equivalent to VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
class TUniformBufferDescriptor : public TDescriptor
{
  private:
    std::vector<TStructMember> members;

  public:
    TUniformBufferDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name, std::vector<TStructMember> &members);
    ~TUniformBufferDescriptor();
};

// Equivalent to VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
class TCombinedImageSamplerDescriptor : public TDescriptor
{
  private:
  public:
    TCombinedImageSamplerDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name);
    ~TCombinedImageSamplerDescriptor();
};

// Equivalent to VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
class TSampledImageDescriptor : public TDescriptor
{
  private:
  public:
    TSampledImageDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name);
    ~TSampledImageDescriptor();
};

// Equivalent to VkDescriptorSetLayoutBinding::descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER
class TSamplerDescriptor : public TDescriptor
{
  private:
  public:
    TSamplerDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name);
    ~TSamplerDescriptor();
};

} // namespace Core
} // namespace Turbo

#endif // !TDESCRIPTOR_H