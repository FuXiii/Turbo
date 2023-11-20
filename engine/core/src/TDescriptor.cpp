#include "TDescriptor.h"
#include "TException.h"

Turbo::Core::TDescriptor::TDescriptor(const TRefPtr<TShader> &shader, TDescriptorType type, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TInfo()
{
    this->shader = shader;
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
    case Turbo::Core::TDescriptorType::ACCELERATION_STRUCTURE: {
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    }
    break;
    }

    return VkDescriptorType::VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

Turbo::Core::TDescriptorDataType Turbo::Core::TDescriptor::GetDataType()
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

Turbo::Core::TRefPtr<Turbo::Core::TShader> Turbo::Core::TDescriptor::GetShader()
{
    return this->shader;
}

std::string Turbo::Core::TDescriptor::ToString()
{
    return std::string();
}

Turbo::Core::TStructMember::TStructMember(TDescriptorDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name) : Turbo::Core::TInfo()
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

Turbo::Core::TNaNDescriptor::TNaNDescriptor(uint32_t set) : Turbo::Core::TDescriptor(nullptr, TDescriptorType::SAMPLER, TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN, set, 0, 1, "NaN")
{
}

Turbo::Core::TNaNDescriptor::~TNaNDescriptor()
{
}

Turbo::Core::TUniformBufferDescriptor::TUniformBufferDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name, std::vector<TStructMember> &members, uint32_t size) : Turbo::Core::TDescriptor(shader, TDescriptorType::UNIFORM_BUFFER, dataType, set, binding, count, name)
{
    this->size = size;
    this->members = members;
}

uint32_t Turbo::Core::TUniformBufferDescriptor::GetSize()
{
    return size;
}

Turbo::Core::TUniformBufferDescriptor::~TUniformBufferDescriptor()
{
}

Turbo::Core::TStorageBufferDescriptor::TStorageBufferDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::STORAGE_BUFFER, dataType, set, binding, count, name)
{
}

Turbo::Core::TStorageBufferDescriptor::~TStorageBufferDescriptor()
{
}

Turbo::Core::TCombinedImageSamplerDescriptor::TCombinedImageSamplerDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::COMBINED_IMAGE_SAMPLER, dataType, set, binding, count, name)
{
}

Turbo::Core::TCombinedImageSamplerDescriptor::~TCombinedImageSamplerDescriptor()
{
}

Turbo::Core::TSampledImageDescriptor::TSampledImageDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::SAMPLED_IMAGE, dataType, set, binding, count, name)
{
}

Turbo::Core::TSampledImageDescriptor::~TSampledImageDescriptor()
{
}

Turbo::Core::TSamplerDescriptor::TSamplerDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::SAMPLER, dataType, set, binding, count, name)
{
}

Turbo::Core::TSamplerDescriptor::~TSamplerDescriptor()
{
}

Turbo::Core::TPushConstantDescriptor::TPushConstantDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t count, const std::string &name, std::vector<TStructMember> &members, uint32_t size) : Turbo::Core::TUniformBufferDescriptor(shader, dataType, UINT32_MAX, UINT32_MAX, count, name, members, size)
{
}

Turbo::Core::TPushConstantDescriptor::~TPushConstantDescriptor()
{
}

Turbo::Core::TInputAttachmentDescriptor::TInputAttachmentDescriptor(uint32_t index, const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::INPUT_ATTACHMENT, dataType, set, binding, count, name)
{
    this->index = index;
}

Turbo::Core::TInputAttachmentDescriptor::~TInputAttachmentDescriptor()
{
}

uint32_t Turbo::Core::TInputAttachmentDescriptor::GetIndex()
{
    return this->index;
}

Turbo::Core::TStorageImageDescriptor::TStorageImageDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::STORAGE_IMAGE, dataType, set, binding, count, name)
{
}

Turbo::Core::TStorageImageDescriptor::~TStorageImageDescriptor()
{
}

Turbo::Core::TAccelerationStructureDescriptor::TAccelerationStructureDescriptor(const TRefPtr<TShader> &shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptorType::ACCELERATION_STRUCTURE, dataType, set, binding, count, name)
{
}

Turbo::Core::TAccelerationStructureDescriptor::~TAccelerationStructureDescriptor()
{
}
