#pragma once
#ifndef TURBO_CORE_TPIPELINELAYOUT_H
#define TURBO_CORE_TPIPELINELAYOUT_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDescriptorSetLayout;
class TDevice;
class TPushConstantDescriptor;

class TPipelineLayout : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TDescriptorSetLayout *> descriptorSetLayouts;
    T_VULKAN_HANDLE_DATA std::vector<TPushConstantDescriptor *> pushConstantDescriptors;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipelineLayout(TDevice *device, std::vector<TDescriptorSetLayout *> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *>& pushConstantDescriptors);
    ~TPipelineLayout();

    const std::vector<TDescriptorSetLayout *> &GetDescriptorSetLayouts();
    const std::vector<TPushConstantDescriptor *> &GetPushConstantDescriptors();

    VkPipelineLayout GetVkPipelineLayout();

    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TPIPELINELAYOUT_H