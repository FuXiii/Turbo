#pragma once
#ifndef TURBO_CORE_TSHADERSTAGE_H
#define TURBO_CORE_TSHADERSTAGE_H
#include "TShader.h"

namespace Turbo
{
namespace Core
{
class TShaderStage : public Turbo::Core::TReferenced
{
  public:
    class TSpecializationConstants
    {
      public:
        class TSpecializationConstant
        {
          private:
            Turbo::Core::TDescriptorDataType type = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN;
            std::uint8_t size = 0;
            void *value = nullptr;

          public:
            TSpecializationConstant() = default;
            TSpecializationConstant(const bool &value);
            TSpecializationConstant(const int &value);
            TSpecializationConstant(const std::uint32_t &value);
            TSpecializationConstant(const float &value);
            TSpecializationConstant(const double &value);
            TSpecializationConstant(const TSpecializationConstant &other);
            TSpecializationConstant(TSpecializationConstant &&other);

            TSpecializationConstant &operator=(const TSpecializationConstant &other);
            TSpecializationConstant &operator=(TSpecializationConstant &&other);

            ~TSpecializationConstant();

            Turbo::Core::TDescriptorDataType GetType() const;

            std::uint8_t GetSize() const;

            bool GetBool() const;
            int GetInt() const;
            std::uint32_t GetUint32() const;
            float GetFloat() const;
            double GetDouble() const;

            bool Valid() const;
            std::string ToString() const;
        };

        using TID = std::uint32_t;

      public:
        using TSpecializationConstantsMap = std::unordered_map<TShaderStage::TSpecializationConstants::TID, TSpecializationConstant>;

      private:
        TSpecializationConstantsMap specializationConstants;

      public:
        void Merge(const TShaderStage::TSpecializationConstants::TID &id, const bool &value);
        void Merge(const TShaderStage::TSpecializationConstants::TID &id, const int &value);
        void Merge(const TShaderStage::TSpecializationConstants::TID &id, const std::uint32_t &value);
        void Merge(const TShaderStage::TSpecializationConstants::TID &id, const float &value);
        void Merge(const TShaderStage::TSpecializationConstants::TID &id, const double &value);

        bool Empty() const;
        std::size_t Size() const;
        bool Has(const TShaderStage::TSpecializationConstants::TID &id, TShaderStage::TSpecializationConstants::TSpecializationConstant *specializationConstant = nullptr) const;
        const TShaderStage::TSpecializationConstants::TSpecializationConstant &Get(const TShaderStage::TSpecializationConstants::TID &id) const;

        TSpecializationConstantsMap::const_iterator begin() const noexcept;
        TSpecializationConstantsMap::const_iterator end() const noexcept;

        std::string ToString() const;
    };

  private:
    Turbo::Core::TRefPtr<TShader> shader;
    TShaderStage::TSpecializationConstants specializationConstants;

  public:
    TShaderStage() = default;
    TShaderStage(TShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});

    void Merge(const TShaderStage::TSpecializationConstants::TID &id, const bool &value);
    void Merge(const TShaderStage::TSpecializationConstants::TID &id, const int &value);
    void Merge(const TShaderStage::TSpecializationConstants::TID &id, const std::uint32_t &value);
    void Merge(const TShaderStage::TSpecializationConstants::TID &id, const float &value);
    void Merge(const TShaderStage::TSpecializationConstants::TID &id, const double &value);

    const TShaderStage::TSpecializationConstants &GetSpecializationConstants() const;
    const Turbo::Core::TRefPtr<TShader> &GetShader() const;

    bool Valid() const;
};

class TComputeShaderStage : public TShaderStage
{
  public:
    TComputeShaderStage() = default;
    TComputeShaderStage(TComputeShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TVertexShaderStage : public TShaderStage
{
  public:
    TVertexShaderStage() = default;
    TVertexShaderStage(TVertexShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TTessellationControlShaderStage : public TShaderStage
{
  public:
    TTessellationControlShaderStage() = default;
    TTessellationControlShaderStage(TTessellationControlShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TTessellationEvaluationShaderStage : public TShaderStage
{
  public:
    TTessellationEvaluationShaderStage() = default;
    TTessellationEvaluationShaderStage(TTessellationEvaluationShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TGeometryShaderStage : public TShaderStage
{
  public:
    TGeometryShaderStage() = default;
    TGeometryShaderStage(TGeometryShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TFragmentShaderStage : public TShaderStage
{
  public:
    TFragmentShaderStage() = default;
    TFragmentShaderStage(TFragmentShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TTaskShaderStage : public TShaderStage
{
  public:
    TTaskShaderStage() = default;
    TTaskShaderStage(TTaskShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TMeshShaderStage : public TShaderStage
{
  public:
    TMeshShaderStage() = default;
    TMeshShaderStage(TMeshShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TRayGenerationShaderStage : public TShaderStage
{
  public:
    TRayGenerationShaderStage() = default;
    TRayGenerationShaderStage(TRayGenerationShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TAnyHitShaderStage : public TShaderStage
{
  public:
    TAnyHitShaderStage() = default;
    TAnyHitShaderStage(TAnyHitShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TClosestHitShaderStage : public TShaderStage
{
  public:
    TClosestHitShaderStage() = default;
    TClosestHitShaderStage(TClosestHitShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TMissShaderStage : public TShaderStage
{
  public:
    TMissShaderStage() = default;
    TMissShaderStage(TMissShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TIntersectionShaderStage : public TShaderStage
{
  public:
    TIntersectionShaderStage() = default;
    TIntersectionShaderStage(TIntersectionShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

class TCallableShaderStage : public TShaderStage
{
  public:
    TCallableShaderStage() = default;
    TCallableShaderStage(TCallableShader *shader, const TShaderStage::TSpecializationConstants &specializationConstants = {});
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSHADERSTAGE_H