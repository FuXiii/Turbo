#pragma once
#ifndef TSHADER_H
#define TSHADER_H
#include "TDescriptor.h"
#include "TDescriptorSetLayout.h"
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
    TInterface(uint32_t location, TShaderDataType dataType, uint32_t width, uint32_t offset, uint32_t vecSize, uint32_t columns, uint32_t size, uint32_t count, uint32_t arrayStride, uint32_t matrixStride, const std::string &name);
    ~TInterface();

  public:
    virtual std::string ToString() override;
};

class TShader : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkShaderModule vkShaderModule = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TDescriptorSetLayout *> descriptorSetLayouts;

    T_VULKAN_HANDLE_DATA size_t size = 0;
    T_VULKAN_HANDLE_DATA uint32_t *code = nullptr;
    T_VULKAN_HANDLE_DATA TShaderLanguage language;
    T_VULKAN_HANDLE_DATA TShaderType type;

  private:
    std::vector<TInterface> inputs;
    std::vector<TInterface> outputs;
    std::vector<TUniformBufferDescriptor *> uniformBufferDescriptors;

    std::string entryPoint;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;
    void InternalParseSpirV();

  public:
    TShader(TDevice *device, TShaderType type, TShaderLanguage language, const std::string &code, const std::string &entryPoint = "main");
    TShader(TDevice *device, TShaderType type, size_t size, uint32_t *code, const std::string &entryPoint = "main");

    ~TShader();

  public:
    TDevice *GetDevice();
    std::vector<TDescriptorSetLayout *> GetDescriptorSetLayouts();
    VkShaderStageFlags GetVkShaderStageFlags();
    VkShaderStageFlagBits GetVkShaderStageFlagBits();
    VkShaderModule GetVkShaderModule();

    const std::string &GetEntryPoint();

    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TSHADER_H