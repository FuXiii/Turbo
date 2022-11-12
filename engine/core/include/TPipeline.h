#pragma once
#ifndef TPIPELINE_H
#define TPIPELINE_H
#include "TFormatInfo.h"
#include "TScissor.h"
#include "TViewport.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TShader;
class TVertexShader;
class TFragmentShader;
class TComputeShader;
class TDescriptorSetLayout;
class TPipelineLayout;

typedef enum class TPipelineType
{
    Graphics,
    Compute
} TPipelineType;

typedef enum TPipelineStageBits
{
    TOP_OF_PIPE_BIT = 0x00000001,
    DRAW_INDIRECT_BIT = 0x00000002,
    VERTEX_INPUT_BIT = 0x00000004,
    VERTEX_SHADER_BIT = 0x00000008,
    TESSELLATION_CONTROL_SHADER_BIT = 0x00000010,
    TESSELLATION_EVALUATION_SHADER_BIT = 0x00000020,
    GEOMETRY_SHADER_BIT = 0x00000040,
    FRAGMENT_SHADER_BIT = 0x00000080,
    EARLY_FRAGMENT_TESTS_BIT = 0x00000100,
    LATE_FRAGMENT_TESTS_BIT = 0x00000200,
    COLOR_ATTACHMENT_OUTPUT_BIT = 0x00000400,
    COMPUTE_SHADER_BIT = 0x00000800,
    TRANSFER_BIT = 0x00001000,
    BOTTOM_OF_PIPE_BIT = 0x00002000,
    HOST_BIT = 0x00004000,
    ALL_GRAPHICS_BIT = 0x00008000,
    ALL_COMMANDS_BIT = 0x00010000,
} TPipelineStageBits;
typedef VkFlags TPipelineStages;

class TPipeline : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE TPipelineLayout *pipelineLayout = nullptr;
    T_VULKAN_HANDLE_CHILDREN std::vector<TShader *> shaders;

    TPipelineType type;

  protected:
    T_VULKAN_HANDLE_HANDLE VkPipeline vkPipeline = VK_NULL_HANDLE;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    [[deprecated]]TPipeline(TDevice *device, TPipelineType type, std::vector<TShader *> &shaders);
    TPipeline(TDevice *device, TVertexShader* vertexShader,TFragmentShader* fragmentShader);//for graphics Pipeline
    //TPipeline(TDevice *device, TVertexShader* vertexShader,...其他着色器, TFragmentShader* fragmentShader);//for future graphics Pipeline
    //TPipeline(TDevice *device, ...光追标准着色器);//for future ray tracing Pipeline
    TPipeline(TDevice *device, TComputeShader *computeShader); // for compute pipeline
    ~TPipeline();

  public:
    TPipelineLayout *GetPipelineLayout();
    VkPipeline GetVkPipeline();

    TPipelineType GetType();
    std::vector<TShader *> GetShaders();

    TDevice *GetDevice();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TPIPELINE_H