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

class TSpecializations : public Turbo::Core::TInfo
{
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

  private:
    std::map<uint32_t, TConstValue> specializationMap;

  public:
    TSpecializations() = default;
    ~TSpecializations() = default;

    void SetConstant(uint32_t id, bool value);
    void SetConstant(uint32_t id, int32_t value);
    void SetConstant(uint32_t id, uint32_t value);
    void SetConstant(uint32_t id, float value);
    void SetConstant(uint32_t id, double value);

    const std::map<uint32_t, TConstValue> &GetSpecializations() const;

  public:
    virtual std::string ToString() override;
};

class TPipeline : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE TPipelineLayout *pipelineLayout = nullptr;
    T_VULKAN_HANDLE_CHILDREN std::vector<TShader *> shaders;
    T_VULKAN_HANDLE_CHILDREN TPipelineCache *pipelineCache = nullptr;
    TPipelineType type;

  protected:
    T_VULKAN_HANDLE_HANDLE VkPipeline vkPipeline = VK_NULL_HANDLE;

    std::vector<TSpecializationConstant> specializationConstants;
    TSpecializations specializations;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    [[deprecated]] TPipeline(TDevice *device, TPipelineType type, std::vector<TShader *> &shaders, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr);
    TPipeline(TDevice *device, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr); // for graphics Pipeline
    TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr);
    TPipeline(TDevice *device, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr);
    TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr);
    TPipeline(TDevice *device, TMeshShader *meshShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr); // for graphics Pipeline

    // TPipeline(TDevice *device, TVertexShader* vertexShader,...其他着色器, TFragmentShader* fragmentShader);//for future graphics Pipeline
    // TPipeline(TDevice *device, ...光追标准着色器);//for future ray tracing Pipeline
    TPipeline(TDevice *device, TComputeShader *computeShader, TPipelineCache *pipelineCache = nullptr, TSpecializations *specializations = nullptr); // for compute pipeline
    ~TPipeline();

  public:
    TPipelineLayout *GetPipelineLayout();
    VkPipeline GetVkPipeline();

    TPipelineType GetType();
    std::vector<TShader *> GetShaders();

    TDevice *GetDevice();
    TPipelineCache *GetPipelineCache();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TPIPELINE_H