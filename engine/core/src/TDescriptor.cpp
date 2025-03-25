#include "TDescriptor.h"
#include "TException.h"
#include "TShader.h"

Turbo::Core::TDescriptor::TDescriptor(TShader *shader, TDescriptor::Type type, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TInfo()
{
    this->shader = shader;
    this->type = type;
    this->dataType = dataType;
    this->count = count;
    this->set = set;
    this->binding = binding;
    this->name = name;
}

Turbo::Core::TDescriptor::TDescriptor(TDescriptor::Type type, uint32_t count)
{
    this->type = type;
    this->count = count;
}

Turbo::Core::TDescriptor::~TDescriptor()
{
}

const Turbo::Core::TDescriptor::Type &Turbo::Core::TDescriptor::GetType() const
{
    return this->type;
}

VkDescriptorType Turbo::Core::TDescriptor::GetVkDescriptorType() const
{
    return static_cast<VkDescriptorType>(this->type);
}

Turbo::Core::TDescriptorDataType Turbo::Core::TDescriptor::GetDataType() const
{
    return this->dataType;
}

const uint32_t &Turbo::Core::TDescriptor::GetCount() const
{
    return this->count;
}

uint32_t Turbo::Core::TDescriptor::GetSet() const
{
    return this->set;
}

uint32_t Turbo::Core::TDescriptor::GetBinding() const
{
    return this->binding;
}

const std::string &Turbo::Core::TDescriptor::GetName() const
{
    return this->name;
}

Turbo::Core::TShader *Turbo::Core::TDescriptor::GetShader()
{
    return this->shader;
}

bool Turbo::Core::TDescriptor::operator==(const TDescriptor &other) const
{
    return (this->type == other.type) && (this->count == other.count);
}

bool Turbo::Core::TDescriptor::operator!=(const TDescriptor &other) const
{
    return !((*this) == other);
}

std::string Turbo::Core::TDescriptor::ToString() const
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

std::string Turbo::Core::TStructMember::ToString() const
{
    return std::string();
}

Turbo::Core::TNaNDescriptor::TNaNDescriptor(uint32_t set) : Turbo::Core::TDescriptor(nullptr, TDescriptor::Type::SAMPLER, TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN, set, 0, 1, "NaN")
{
}

Turbo::Core::TNaNDescriptor::~TNaNDescriptor()
{
}

Turbo::Core::TUniformBufferDescriptor::TUniformBufferDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name, const std::vector<TStructMember> &members, uint32_t size) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::UNIFORM_BUFFER, dataType, set, binding, count, name)
{
    this->size = size;
    this->members = members;
}

uint32_t Turbo::Core::TUniformBufferDescriptor::GetSize() const
{
    return size;
}

Turbo::Core::TUniformBufferDescriptor::~TUniformBufferDescriptor()
{
}

Turbo::Core::TStorageBufferDescriptor::TStorageBufferDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::STORAGE_BUFFER, dataType, set, binding, count, name)
{
}

Turbo::Core::TStorageBufferDescriptor::~TStorageBufferDescriptor()
{
}

Turbo::Core::TCombinedImageSamplerDescriptor::TCombinedImageSamplerDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::COMBINED_IMAGE_SAMPLER, dataType, set, binding, count, name)
{
}

Turbo::Core::TCombinedImageSamplerDescriptor::~TCombinedImageSamplerDescriptor()
{
}

Turbo::Core::TSampledImageDescriptor::TSampledImageDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::SAMPLED_IMAGE, dataType, set, binding, count, name)
{
}

Turbo::Core::TSampledImageDescriptor::~TSampledImageDescriptor()
{
}

Turbo::Core::TSamplerDescriptor::TSamplerDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::SAMPLER, dataType, set, binding, count, name)
{
}

Turbo::Core::TSamplerDescriptor::~TSamplerDescriptor()
{
}

Turbo::Core::TPushConstantDescriptor::TPushConstantDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t count, const std::string &name, std::vector<TStructMember> &members, uint32_t size) : Turbo::Core::TUniformBufferDescriptor(shader, dataType, UINT32_MAX, UINT32_MAX, count, name, members, size)
{
}

Turbo::Core::TPushConstantDescriptor::~TPushConstantDescriptor()
{
}

Turbo::Core::TInputAttachmentDescriptor::TInputAttachmentDescriptor(uint32_t index, TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::INPUT_ATTACHMENT, dataType, set, binding, count, name)
{
    this->index = index;
}

Turbo::Core::TInputAttachmentDescriptor::~TInputAttachmentDescriptor()
{
}

uint32_t Turbo::Core::TInputAttachmentDescriptor::GetIndex() const
{
    return this->index;
}

Turbo::Core::TStorageImageDescriptor::TStorageImageDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::STORAGE_IMAGE, dataType, set, binding, count, name)
{
}

Turbo::Core::TStorageImageDescriptor::~TStorageImageDescriptor()
{
}

Turbo::Core::TAccelerationStructureDescriptor::TAccelerationStructureDescriptor(TShader *shader, TDescriptorDataType dataType, uint32_t set, uint32_t binding, uint32_t count, const std::string &name) : Turbo::Core::TDescriptor(shader, TDescriptor::Type::ACCELERATION_STRUCTURE, dataType, set, binding, count, name)
{
}

Turbo::Core::TAccelerationStructureDescriptor::~TAccelerationStructureDescriptor()
{
}
