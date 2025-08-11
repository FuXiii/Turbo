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
#include "TPipelineLayout.h"
#include <initializer_list>

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

            bool GetBool() const;
            int GetInt() const;
            std::uint32_t GetUint32() const;
            float GetFloat() const;
            double GetDouble() const;

            bool Valid() const;
        };

        using ID = std::uint32_t;

      public:
        using TSpecializationConstantsMap = std::unordered_map<TPipeline::TSpecializationConstants::ID, TSpecializationConstant>;

      private:
        TSpecializationConstantsMap specializationConstants;

      public:
        void Merge(const TPipeline::TSpecializationConstants::ID &id, const bool &value);
        void Merge(const TPipeline::TSpecializationConstants::ID &id, const int &value);
        void Merge(const TPipeline::TSpecializationConstants::ID &id, const std::uint32_t &value);
        void Merge(const TPipeline::TSpecializationConstants::ID &id, const float &value);
        void Merge(const TPipeline::TSpecializationConstants::ID &id, const double &value);

        TSpecializationConstantsMap::const_iterator begin() const noexcept;
        TSpecializationConstantsMap::const_iterator end() const noexcept;
    };

  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE TRefPtr<TPipelineLayout> pipelineLayout; // NOTE: Create Internal
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
    [[deprecated]] TPipeline(TDevice *device, TVertexShader *vertexShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr); // for graphics Pipeline
    [[deprecated]] TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr);
    [[deprecated]] TPipeline(TDevice *device, TVertexShader *vertexShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr);
    [[deprecated]] TPipeline(TDevice *device, TVertexShader *vertexShader, TTessellationControlShader *tessellationControlShader, TTessellationEvaluationShader *tessellationEvaluationShader, TGeometryShader *geometryShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr);
    [[deprecated]] TPipeline(TDevice *device, TMeshShader *meshShader, TFragmentShader *fragmentShader, TPipelineCache *pipelineCache = nullptr); // for mesh Pipeline

    // TPipeline(TDevice *device, TVertexShader* vertexShader,...其他着色器, TFragmentShader* fragmentShader);//for future graphics Pipeline
    // TPipeline(TDevice *device, ...光追标准着色器);//for future ray tracing Pipeline
    // TPipeline(const TRefPtr<TDevice> &device, const TRefPtr<TComputeShader> &computeShader, const TRefPtr<TPipelineCache> &pipelineCache = nullptr); // for compute pipeline
    [[deprecated]] TPipeline(TDevice *device, TComputeShader *computeShader, TPipelineCache *pipelineCache = nullptr); // for compute pipeline

    TPipeline(TDevice *device, const TPipelineLayout::TLayout &layout, const std::initializer_list<TShader *> &shaders, TPipelineCache *pipelineCache = nullptr); // NOTE: new!

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