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
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TRefPtr<TDescriptorSetLayout>> descriptorSetLayouts;
    T_VULKAN_HANDLE_DATA std::vector<TPushConstantDescriptor *> pushConstantDescriptors; // FIXME: check it

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TPipelineLayout(TDevice *device, const std::vector<TDescriptorSetLayout *> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors /*FIXME: check it*/);
    TPipelineLayout(TDevice *device, const std::vector<TRefPtr<TDescriptorSetLayout>> &descriptorSetLayouts, std::vector<TPushConstantDescriptor *> &pushConstantDescriptors /*FIXME: check it*/);

  protected:
    virtual ~TPipelineLayout();

  public:
    std::vector<TDescriptorSetLayout *> GetDescriptorSetLayouts();
    const std::vector<TPushConstantDescriptor *> &GetPushConstantDescriptors(); // FIXME: check it

    VkPipelineLayout GetVkPipelineLayout();

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TPIPELINELAYOUT_H