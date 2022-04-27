#include "TDescriptor.h"

Turbo::Core::TDescriptor::TDescriptor(TDescriptorType type, TShaderDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TInfo()
{
    this->type = type;
    this->dataType = dataType;
    this->count = count;
    this->set = set;
    this->binding = binding;
    this->name = name;
}

Turbo::Core::TDescriptor::~TDescriptor()
{
}

Turbo::Core::TDescriptorType Turbo::Core::TDescriptor::GetType()
{
    return this->type;
}

VkDescriptorType Turbo::Core::TDescriptor::GetVkDescriptorType()
{
    switch (this->type)
    {
    case Turbo::Core::TDescriptorType::SAMPLER: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
    }
    break;
    case Turbo::Core::TDescriptorType::COMBINED_IMAGE_SAMPLER: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    }
    break;
    case Turbo::Core::TDescriptorType::SAMPLED_IMAGE: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    }
    break;
    case Turbo::Core::TDescriptorType::STORAGE_IMAGE: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    }
    break;
    case Turbo::Core::TDescriptorType::UNIFORM_TEXEL_BUFFER: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    }
    break;
    case Turbo::Core::TDescriptorType::STORAGE_TEXEL_BUFFER: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    }
    break;
    case Turbo::Core::TDescriptorType::UNIFORM_BUFFER: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }
    break;
    case Turbo::Core::TDescriptorType::STORAGE_BUFFER: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }
    break;
    case Turbo::Core::TDescriptorType::UNIFORM_BUFFER_DYNAMIC: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    }
    break;
    case Turbo::Core::TDescriptorType::STORAGE_BUFFER_DYNAMIC: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    }
    break;
    case Turbo::Core::TDescriptorType::INPUT_ATTACHMENT: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    }
    break;
    }

    return VkDescriptorType::VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

Turbo::Core::TShaderDataType Turbo::Core::TDescriptor::GetDataType()
{
    return this->dataType;
}

uint32_t Turbo::Core::TDescriptor::GetCount()
{
    return this->count;
}

uint32_t Turbo::Core::TDescriptor::GetSet()
{
    return this->set;
}

uint32_t Turbo::Core::TDescriptor::GetBinding()
{
    return this->binding;
}

const std::string &Turbo::Core::TDescriptor::GetName()
{
    return this->name;
}

std::string Turbo::Core::TDescriptor::ToString()
{
    return std::string();
}

Turbo::Core::TStructMember::TStructMember(TShaderDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name) : Turbo::Core::TInfo()
{
    this->dataType = dataType;
    this->width = width;
    this->offset = offset;
    this->vecSize = vecSize;
    this->columns = columns;
    this->size = size;
    this->count = count;
    this->arrayStride = arrayStride;
    this->matrixStride = matrixStride;
    this->name = name;
}

Turbo::Core::TStructMember::TStructMember(const TStructMember &object)
{
    if (this != &object)
    {
        this->dataType = object.dataType;
        this->width = object.width;
        this->offset = object.offset;
        this->vecSize = object.vecSize;
        this->columns = object.columns;
        this->size = object.size;
        this->count = object.count;
        this->arrayStride = object.arrayStride;
        this->matrixStride = object.matrixStride;
        this->name = object.name;
    }
}

Turbo::Core::TStructMember::~TStructMember()
{
}

Turbo::Core::TStructMember &Turbo::Core::TStructMember::operator=(const TStructMember &object)
{
    if (this != &object)
    {
        this->dataType = object.dataType;
        this->width = object.width;
        this->offset = object.offset;
        this->vecSize = object.vecSize;
        this->columns = object.columns;
        this->size = object.size;
        this->count = object.count;
        this->arrayStride = object.arrayStride;
        this->matrixStride = object.matrixStride;
        this->name = object.name;
    }

    return *this;
}

std::string Turbo::Core::TStructMember::ToString()
{
    return std::string();
}

Turbo::Core::TUniformBufferDescriptor::TUniformBufferDescriptor(TShaderDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name, std::vector<TStructMember> &members) : Turbo::Core::TDescriptor(TDescriptorType::UNIFORM_BUFFER, dataType, set, binding, count, name)
{
    this->members = members;
}

Turbo::Core::TUniformBufferDescriptor::~TUniformBufferDescriptor()
{
}
