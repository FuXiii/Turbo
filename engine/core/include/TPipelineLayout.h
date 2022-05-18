#pragma once
#ifndef TPIPELINELAYOUT_H
#define TPIPELINELAYOUT_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDescriptorSetLayout;
class TDevice;

class TPipelineLayout : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TDescriptorSetLayout *> descriptorSetLayouts;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipelineLayout(TDevice *device, std::vector<TDescriptorSetLayout *> &descriptorSetLayouts);
    ~TPipelineLayout();

    const std::vector<TDescriptorSetLayout *> &GetDescriptorSetLayout();

    VkPipelineLayout GetVkPipelineLayout();

    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TPIPELINELAYOUT_H