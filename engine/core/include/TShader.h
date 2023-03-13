#pragma once
#ifndef TURBO_CORE_TSHADER_H
#define TURBO_CORE_TSHADER_H
#include "TDescriptor.h"
#include "TVulkanHandle.h"

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
    COMPUTE = 5
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
    virtual std::string ToString() override;
};

class TShader : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkShaderModule vkShaderModule = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN

    T_VULKAN_HANDLE_DATA size_t size = 0;
    T_VULKAN_HANDLE_DATA uint32_t *code = nullptr;
    T_VULKAN_HANDLE_DATA TShaderLanguage language;
    T_VULKAN_HANDLE_DATA TShaderType type;

  private:
    std::vector<TInterface> inputs;
    std::vector<TInterface> outputs;
    std::vector<TUniformBufferDescriptor *> uniformBufferDescriptors;
    std::vector<TCombinedImageSamplerDescriptor *> combinedImageSamplerDescriptors;
    std::vector<TSampledImageDescriptor *> sampledImageDescriptors;
    std::vector<TSamplerDescriptor *> samplerDescriptors;
    std::vector<TPushConstantDescriptor *> pushConstantDescriptors;
    std::vector<TInputAttachmentDescriptor *> inputAttachmentDescriptors;
    std::vector<TStorageImageDescriptor *> storageImageDescriptors;

    std::string entryPoint;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;
    void InternalParseSpirV();

  public:
    TShader(TDevice *device, TShaderType type, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TShader(TDevice *device, TShaderType type, size_t size, uint32_t *code, const std::string &entryPoint = "main");

    ~TShader();

  public:
    TDevice *GetDevice();
    VkShaderStageFlags GetVkShaderStageFlags();
    VkShaderStageFlagBits GetVkShaderStageFlagBits();
    VkShaderModule GetVkShaderModule();

    const std::string &GetEntryPoint();

    const std::vector<TUniformBufferDescriptor *> &GetUniformBufferDescriptors();
    const std::vector<TCombinedImageSamplerDescriptor *> &GetCombinedImageSamplerDescriptors();
    const std::vector<TSampledImageDescriptor *> &GetSampledImageDescriptors();
    const std::vector<TSamplerDescriptor *> &GetSamplerDescriptors();
    const std::vector<TPushConstantDescriptor *> &GetPushConstantDescriptors();
    const std::vector<TInputAttachmentDescriptor *> &GetInputAttachmentDescriptors();
    const std::vector<TStorageImageDescriptor *> &GetStorageImageDescriptors();

    std::vector<TInterface> GetInputs();
    std::vector<TInterface> GetOutputs();

    TShaderType GetType();

    virtual std::string ToString() override;
};

class TVertexShader : public Turbo::Core::TShader
{
  public:
    TVertexShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TVertexShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");
};

class TFragmentShader : public Turbo::Core::TShader
{
  public:
    TFragmentShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TFragmentShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");
};

class TComputeShader : public Turbo::Core::TShader
{
  public:
    TComputeShader(TDevice *device, TShaderLanguage language, const std::string &code, const std::vector<std::string> &includePaths = std::vector<std::string>(), const std::string &entryPoint = "main");
    TComputeShader(TDevice *device, size_t size, uint32_t *code, const std::string &entryPoint = "main");
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSHADER_H