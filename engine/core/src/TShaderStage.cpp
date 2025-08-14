#include "TShaderStage.h"
#include <sstream>

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(const bool &value)
{
    this->type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN;
    VkBool32 data = value ? VK_TRUE : VK_FALSE;
    this->size = sizeof(VkBool32);
    this->value = malloc(this->size);
    memcpy(this->value, &data, this->size);
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(const int &value)
{
    this->type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT;
    this->size = sizeof(value);
    this->value = malloc(this->size);
    memcpy(this->value, &value, this->size);
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(const std::uint32_t &value)
{
    this->type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT;
    this->size = sizeof(value);
    this->value = malloc(this->size);
    memcpy(this->value, &value, this->size);
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(const float &value)
{
    this->type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT;
    this->size = sizeof(value);
    this->value = malloc(this->size);
    memcpy(this->value, &value, this->size);
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(const double &value)
{
    this->type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE;
    this->size = sizeof(value);
    this->value = malloc(this->size);
    memcpy(this->value, &value, this->size);
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(const TSpecializationConstant &other)
{
    if (other.type != Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN && other.size > 0 && other.value != nullptr)
    {
        this->type = other.type;
        this->size = other.size;
        this->value = malloc(this->size);
        memcpy(this->value, other.value, this->size);
    }
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::TSpecializationConstant(TSpecializationConstant &&other)
{
    std::swap(this->type, other.type);
    std::swap(this->size, other.size);
    std::swap(this->value, other.value);
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant &Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::operator=(const TSpecializationConstant &other)
{
    if (this != &other)
    {
        if (this->value != nullptr)
        {
            free(this->value);
            this->value = nullptr;
        }

        this->type = other.type;
        this->size = other.size;
        if (this->size > 0)
        {
            this->value = malloc(this->size);
            memcpy(this->value, other.value, this->size);
        }
    }

    return *this;
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant &Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::operator=(TSpecializationConstant &&other)
{
    // this->type = std::move(other.type);
    // this->size = std::move(other.size);
    // this->value = std::move(other.value);
    std::swap(this->type, other.type);
    std::swap(this->size, other.size);
    std::swap(this->value, other.value);
    return *this;
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::~TSpecializationConstant()
{
    if (this->value != nullptr)
    {
        free(this->value);
        this->value = nullptr;
        this->size = 0;
        this->type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN;
    }
}

Turbo::Core::TDescriptorDataType Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::GetType() const
{
    return this->type;
}

bool Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::GetBool() const
{
    bool result = false;
    if (this->Valid() && this->size >= sizeof(result) && this->type == Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN)
    {
        memcpy(&result, this->value, sizeof(result));
    }
    return result;
}

int Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::GetInt() const
{
    int result = 0;
    if (this->Valid() && this->size >= sizeof(result) && this->type == Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT)
    {
        memcpy(&result, this->value, sizeof(result));
    }
    return result;
}

std::uint32_t Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::GetUint32() const
{
    std::uint32_t result = 0;
    if (this->Valid() && this->size >= sizeof(result) && this->type == Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT)
    {
        memcpy(&result, this->value, sizeof(result));
    }
    return result;
}

float Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::GetFloat() const
{
    float result = 0.0f;
    if (this->Valid() && this->size >= sizeof(result) && this->type == Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT)
    {
        memcpy(&result, this->value, sizeof(result));
    }
    return result;
}

double Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::GetDouble() const
{
    double result = 0.0;
    if (this->Valid() && this->size >= sizeof(result) && this->type == Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE)
    {
        memcpy(&result, this->value, sizeof(result));
    }
    return result;
}

bool Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::Valid() const
{
    return this->type != Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN && this->size != 0 && this->value != nullptr;
}

std::string Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstant::ToString() const
{
    std::stringstream ss;
    ss << "{";
    {
        {
            ss << "\"type\":";
            switch (this->type)
            {
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN: {
                ss << "\"unknown\"";
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN: {
                ss << "\"boolean\"";
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT: {
                ss << "\"int\"";
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT: {
                ss << "\"uint\"";
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT: {
                ss << "\"float\"";
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE: {
                ss << "\"double\"";
            }
            break;
            default: {
                ss << "\"unknown\"";
            }
            }
        }
        ss << ",";
        {
            ss << "\"size\":" << static_cast<std::uint32_t>(this->size);
        }
        ss << ",";
        {
            ss << "\"value\":";
            switch (this->type)
            {
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN: {
                ss << "null";
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_BOOLEAN: {
                if (this->GetBool())
                {
                    ss << "true";
                }
                else
                {
                    ss << "false";
                }
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_INT: {
                ss << this->GetInt();
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UINT: {
                ss << this->GetUint32();
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_FLOAT: {
                ss << this->GetFloat();
            }
            break;
            case Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_DOUBLE: {
                ss << this->GetDouble();
            }
            break;
            default: {
                ss << "null";
            }
            }
        }
    }
    ss << "}";
    return ss.str();
}

void Turbo::Core::TShaderStage::TSpecializationConstants::Merge(const TShaderStage::TSpecializationConstants::TID &id, const bool &value)
{
    this->specializationConstants[id] = value;
}

void Turbo::Core::TShaderStage::TSpecializationConstants::Merge(const TShaderStage::TSpecializationConstants::TID &id, const int &value)
{
    this->specializationConstants[id] = value;
}

void Turbo::Core::TShaderStage::TSpecializationConstants::Merge(const TShaderStage::TSpecializationConstants::TID &id, const std::uint32_t &value)
{
    this->specializationConstants[id] = value;
}

void Turbo::Core::TShaderStage::TSpecializationConstants::Merge(const TShaderStage::TSpecializationConstants::TID &id, const float &value)
{
    this->specializationConstants[id] = value;
}

void Turbo::Core::TShaderStage::TSpecializationConstants::Merge(const TShaderStage::TSpecializationConstants::TID &id, const double &value)
{
    this->specializationConstants[id] = value;
}

bool Turbo::Core::TShaderStage::TSpecializationConstants::Empty() const
{
    return this->specializationConstants.empty();
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstantsMap::const_iterator Turbo::Core::TShaderStage::TSpecializationConstants::begin() const noexcept
{
    return this->specializationConstants.begin();
}

Turbo::Core::TShaderStage::TSpecializationConstants::TSpecializationConstantsMap::const_iterator Turbo::Core::TShaderStage::TSpecializationConstants::end() const noexcept
{
    return this->specializationConstants.end();
}

std::string Turbo::Core::TShaderStage::TSpecializationConstants::ToString() const
{
    std::stringstream ss;
    ss << "{";
    {
        if (!this->specializationConstants.empty())
        {
            std::vector<Turbo::Core::TShaderStage::TSpecializationConstants::TID> ordered_ids;
            {
                for (auto &item : this->specializationConstants)
                {
                    ordered_ids.push_back(item.first);
                }
                std::sort(ordered_ids.begin(), ordered_ids.end());
            }

            ss << "\"specialization constants\":";
            ss << "[";
            auto iter = ordered_ids.begin();
            while (iter != ordered_ids.end())
            {
                auto &specialization_constant = this->specializationConstants.at((*iter));
                ss << "{\"" << (*iter) << "\""
                   << ":" << specialization_constant.ToString() << "}";
                ++iter;
                if (iter != ordered_ids.end())
                {
                    ss << ",";
                }
            }
            ss << "]";
        }
    }
    ss << "}";
    return ss.str();
}

Turbo::Core::TShaderStage::TShaderStage(TShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants)
{
    this->shader = shader;
    this->specializationConstants = specializationConstants;
}

void Turbo::Core::TShaderStage::Merge(const TShaderStage::TSpecializationConstants::TID &id, const bool &value)
{
    this->specializationConstants.Merge(id, value);
}

void Turbo::Core::TShaderStage::Merge(const TShaderStage::TSpecializationConstants::TID &id, const int &value)
{
    this->specializationConstants.Merge(id, value);
}

void Turbo::Core::TShaderStage::Merge(const TShaderStage::TSpecializationConstants::TID &id, const std::uint32_t &value)
{
    this->specializationConstants.Merge(id, value);
}

void Turbo::Core::TShaderStage::Merge(const TShaderStage::TSpecializationConstants::TID &id, const float &value)
{
    this->specializationConstants.Merge(id, value);
}

void Turbo::Core::TShaderStage::Merge(const TShaderStage::TSpecializationConstants::TID &id, const double &value)
{
    this->specializationConstants.Merge(id, value);
}

const Turbo::Core::TShaderStage::TSpecializationConstants &Turbo::Core::TShaderStage::GetSpecializationConstants() const
{
    return this->specializationConstants;
}

const Turbo::Core::TShader *Turbo::Core::TShaderStage::GetShader() const
{
    return this->shader;
}

bool Turbo::Core::TShaderStage::Valid() const
{
    return this->shader.Valid();
}
