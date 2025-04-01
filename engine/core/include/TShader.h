#pragma once
#ifndef TURBO_CORE_TSHADER_H
#define TURBO_CORE_TSHADER_H
#include "TDescriptorSetLayout.h"
#include "TVulkanHandle.h"
#include <map>

namespace Turbo
{
namespace Core
{
class TDevice;

typedef enum class TShaderType
{
    VERTEX = 0,
    TESSELLATION_CONTROL = 1,
    TESSELLATION_EVALUATION = 2,
    GEOMETRY = 3,
    FRAGMENT = 4,
    COMPUTE = 5,
    TASK = 6,
    MESH = 7,
    RAY_GENERATION = 8,
    ANY_HIT = 9,
    CLOSEST_HIT = 10,
    MISS = 11,
    INTERSECTION = 12,
    CALLABLE = 13,
} TShaderType;

typedef enum class TShaderLanguage
{
    GLSL = 0,
    HLSL = 1,
    SPIR = 2
} TShaderLanguage;

std::vector<char> ReadSpirVFile(const std::string &file); // Just For Test

class TInterface : public Turbo::Core::TStructMember
{
  private:
    uint32_t location;

  public:
    TInterface(uint32_t location, TDescriptorDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name);
    ~TInterface();

  public:
    virtual std::string ToString() const override;
};

class TSpecializationConstant : public Turbo::Core::TInfo
{
  private:
    uint32_t id;
    std::string name;
    Turbo::Core::TDescriptorDataType descriptorDataType;
    uint32_t width;

  public:
    TSpecializationConstant() = default;
    TSpecializationConstant(uint32_t id, const std::string &name, Turbo::Core::TDescriptorDataType descriptorDataType, uint32_t width);
    ~TSpecializationConstant() = default;

    uint32_t GetConstantID() const;
    const std::string &GetName() const;
    Turbo::Core::TDescriptorDataType GetDescriptorDataType() const;
    uint32_t GetWidth() const;

  public:
    virtual std::string ToString() const override;
};

class TShader : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkShaderModule vkShaderModule = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN

    T_VULKAN_HANDLE_DATA size_t size = 0;
    T_VULKAN_HANDLE_DATA uint32_t *code = nullptr;
    T_VULKAN_HANDLE_DATA TShaderLanguage language;
    T_VULKAN_HANDLE_DATA TShaderType type;

  public:
    union TConstant {
        bool boolValue;
        int32_t intValue;
        uint32_t uintValue;
        float floatValue;
        double doubleValue;
    };

    struct TConstValue
    {
        Turbo::Core::TDescriptorDataType dataType = Turbo::Core::TDescriptorDataType::DESCRIPTOR_DATA_TYPE_UNKNOWN;
        TConstant value;
    };

    class TLayout
    {
      public:
        using TSet = std::size_t;
        using TSets = std::unordered_map<TSet, Turbo::Core::TDescriptorSetLayout::TLayout>;

      private:
        TShader::TLayout::TSets sets;
        TPushConstants pushConstants;

      public:
        TLayout() = default;
        TLayout(const TShader::TLayout::TSets &sets, const TPushConstants &pushConstants);
        TLayout(const TShader::TLayout::TSets &sets);
        TLayout(const TPushConstants &pushConstants);
        TLayout(TShader::TLayout::TSets &&sets, TPushConstants &&pushConstants);

        bool Empty() const;

        const TShader::TLayout::TSets &GetSets() const;
        const TPushConstants &GetPushConstants() const;

        void Merge(const TShader::TLayout::TSets &sets);
        void Merge(const TPushConstants &pushConstants);
        void Merge(const TLayout &layout);

        std::string ToString() const;
    };

  private:
    std::vector<TSpecializationConstant> specializationConstants;
    std::vector<TInterface> inputs;
    std::vector<TInterface> outputs;
    std::vector<TUniformBufferDescriptor *> uniformBufferDescriptors;
    std::vector<TStorageBufferDescriptor *> storageBufferDescriptors;
    std::vector<TCombinedImageSamplerDescriptor *> combinedImageSamplerDescriptors;
    std::vector<TSampledImageDescriptor *> sampledImageDescriptors;
    std::vector<TSamplerDescriptor *> samplerDescriptors;
    std::vector<TPushConstantDescriptor *> pushConstantDescriptors;
    std::vector<TInputAttachmentDescriptor *> inputAttachmentDescriptors;
    std::vector<TStorageImageDescriptor *> storageImageDescriptors;
    std::vector<TAccelerationStructureDescriptor *> accelerationStructureDescriptors;

    std::string entryPoint;

    std::map<uint32_t, TConstValue> specializationMap;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;
    void InternalParseSpirV();

  public:
    TShader(TDevice *device, TShaderType type, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TShader(TDevice *device, TShaderType type, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TShader();

  public:
    TDevice *GetDevice();
    VkShaderStageFlags GetVkShaderStageFlags() const;
    VkShaderStageFlagBits GetVkShaderStageFlagBits() const;
    VkShaderModule GetVkShaderModule();

    const std::string &GetEntryPoint() const;

    const std::vector<TUniformBufferDescriptor *> &GetUniformBufferDescriptors() const;
    const std::vector<TStorageBufferDescriptor *> &GetStorageBufferDescriptors() const;
    const std::vector<TCombinedImageSamplerDescriptor *> &GetCombinedImageSamplerDescriptors() const;
    const std::vector<TSampledImageDescriptor *> &GetSampledImageDescriptors() const;
    const std::vector<TSamplerDescriptor *> &GetSamplerDescriptors() const;
    const std::vector<TPushConstantDescriptor *> &GetPushConstantDescriptors() const;
    const std::vector<TInputAttachmentDescriptor *> &GetInputAttachmentDescriptors() const;
    const std::vector<TStorageImageDescriptor *> &GetStorageImageDescriptors() const;
    const std::vector<TAccelerationStructureDescriptor *> &GetAccelerationStructureDescriptors() const;

    std::vector<TInterface> GetInputs() const;
    std::vector<TInterface> GetOutputs() const;

    const std::vector<TSpecializationConstant> &GetSpecializationConstants() const;

    TShaderType GetType() const;

    //<specialization constants>
    void SetConstant(uint32_t id, bool value);
    void SetConstant(uint32_t id, int32_t value);
    void SetConstant(uint32_t id, uint32_t value);
    void SetConstant(uint32_t id, float value);
    void SetConstant(uint32_t id, double value);
    const std::map<uint32_t, TConstValue> &GetSpecializations() const;
    //</specialization constants>

    std::vector<uint32_t> GetSpirV() const;

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

class TVertexShader : public Turbo::Core::TShader
{
  public:
    TVertexShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TVertexShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TVertexShader() = default;
};

class TFragmentShader : public Turbo::Core::TShader
{
  public:
    TFragmentShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TFragmentShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TFragmentShader() = default;
};

class TComputeShader : public Turbo::Core::TShader
{
  public:
    TComputeShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TComputeShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TComputeShader() = default;
};

class TTessellationControlShader : public Turbo::Core::TShader
{
  public:
    TTessellationControlShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TTessellationControlShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TTessellationControlShader() = default;
};

class TTessellationEvaluationShader : public Turbo::Core::TShader
{
  public:
    TTessellationEvaluationShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TTessellationEvaluationShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TTessellationEvaluationShader() = default;
};

class TGeometryShader : public Turbo::Core::TShader
{
  public:
    TGeometryShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TGeometryShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TGeometryShader() = default;
};

class TTaskShader : public Turbo::Core::TShader
{
  public:
    TTaskShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TTaskShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TTaskShader() = default;
};

class TMeshShader : public Turbo::Core::TShader
{
  public:
    TMeshShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TMeshShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TMeshShader() = default;
};

class TRayGenerationShader : public Turbo::Core::TShader
{
  public:
    TRayGenerationShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TRayGenerationShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TRayGenerationShader() = default;
};

class TAnyHitShader : public Turbo::Core::TShader
{
  public:
    TAnyHitShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TAnyHitShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TAnyHitShader() = default;
};

class TClosestHitShader : public Turbo::Core::TShader
{
  public:
    TClosestHitShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TClosestHitShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TClosestHitShader() = default;
};

class TMissShader : public Turbo::Core::TShader
{
  public:
    TMissShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TMissShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TMissShader() = default;
};

class TIntersectionShader : public Turbo::Core::TShader
{
  public:
    TIntersectionShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TIntersectionShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TIntersectionShader() = default;
};

class TCallableShader : public Turbo::Core::TShader
{
  public:
    TCallableShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TCallableShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");

  protected:
    virtual ~TCallableShader() = default;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSHADER_H