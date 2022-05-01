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

typedef enum class TPipelineType
{
    Graphics,
    Compute
} TPipelineType;

class TPipeline : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TShader *> shaders;

    TPipelineType type;

  protected:
    T_VULKAN_HANDLE_HANDLE VkPipeline vkPipeline = VK_NULL_HANDLE;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipeline(TDevice *device, TPipelineType type, std::vector<TShader *> &shaders);
    ~TPipeline();

  public:
    VkPipelineLayout GetVkPipelineLayout();
    VkPipeline GetVkPipeline();

    TPipelineType GetType();
    std::vector<TShader *> GetShaders();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TPIPELINE_H