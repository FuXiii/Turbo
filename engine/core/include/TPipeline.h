#pragma once
#ifndef TURBO_CORE_TPIPELINE_H
#define TURBO_CORE_TPIPELINE_H
#include "TDescriptor.h"
#include "TFormatInfo.h"
#include "TPipelineCache.h"
#include "TScissor.h"
#include "TShader.h"
#include "TViewport.h"
#include "TVulkanHandle.h"
#include <map>

namespace Turbo
{
namespace Core
{
class TDevice;
class TShader;
class TVertexShader;
class TFragmentShader;
class TComputeShader;
class TTessellationControlShader;
class TTessellationEvaluationShader;
class TGeometryShader;
class TTaskShader;
class TMeshShader;
class TDescriptorSetLayout;
class TPipelineLayout;
class TPipelineCache;

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

// FIXME: It's better let TPipeline to be a Pure Virtual Class
// FIXME: Use PipelineLayout::Layout to create pipeline
class TPipeline : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE TRefPtr<TPipelineLayout> pipelineLayout;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TShader>> shaders;
    T_VULKAN_HANDLE_CHILDREN TRefPtr<TPipelineCache> pipelineCache;
    TPipelineType type;

  protected:
    T_VULKAN_HANDLE_HANDLE VkPipeline vkPipeline = VK_NULL_HANDLE;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    [[deprecated]] TPipeline(const TRefPtr<TDevice> &device, TPipelineType type, std::vector<TRefPtr<TShader>> &shaders, const TRefPtr<TPipelineCache> &pipelineCache = nullptr);
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TVertexShader> &vertexShader, const TRefPtr<TFragmentShader> &fragmentShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr); // for graphics Pipeline
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TVertexShader> &vertexShader, const TRefPtr<TTessellationControlShader> &tessellationControlShader, const TRefPtr<TTessellationEvaluationShader> &tessellationEvaluationShader, const TRefPtr<TFragmentShader> &fragmentShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr);
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TVertexShader> &vertexShader, const TRefPtr<TGeometryShader> &geometryShader, const TRefPtr<TFragmentShader> &fragmentShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr);
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TVertexShader> &vertexShader, const TRefPtr<TTessellationControlShader> &tessellationControlShader, const TRefPtr<TTessellationEvaluationShader> &tessellationEvaluationShader, const TRefPtr<TGeometryShader> &geometryShader, const TRefPtr<TFragmentShader> &fragmentShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr);
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TMeshShader> &meshShader, const TRefPtr<TFragmentShader> &fragmentShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr); // for mesh Pipeline
    TPipeline(TDevice *device, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr); // for graphics Pipeline
    TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr);
    TPipeline(TDevice *device, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr);
    TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr);
    TPipeline(TDevice *device, TMeshShader *meshShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr); // for mesh Pipeline

    // TPipeline(TDevice *device, TVertexShader* vertexShader,...其他着色器, TFragmentShader* fragmentShader);//for future graphics Pipeline
    // TPipeline(TDevice *device, ...光追标准着色器);//for future ray tracing Pipeline
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TComputeShader> &computeShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr); // for compute pipeline
    TPipeline(TDevice *device, TComputeShader *computeShader, TPipelineCache *pipelineCache = nullptr); // for compute pipeline

    

  protected:
    virtual ~TPipeline();

  public:
    TPipelineLayout *GetPipelineLayout();
    VkPipeline GetVkPipeline();

    TPipelineType GetType() const;
    std::vector<TShader *> GetShaders();

    TDevice *GetDevice();
    TPipelineCache *GetPipelineCache();

    // std::vector<TSpecializationConstant> GetSpecializationConstants() const;

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TPIPELINE_H