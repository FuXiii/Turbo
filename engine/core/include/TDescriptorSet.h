#pragma once
#ifndef TURBO_CORE_TDESCRIPTORSET_H
#define TURBO_CORE_TDESCRIPTORSET_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDescriptorPool;
class TDescriptorSetLayout;
class TBuffer;
class TImageView;
class TSampler;

class TDescriptorSet : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDescriptorPool *descriptorPool = nullptr;
    T_VULKAN_HANDLE_PARENT TDescriptorSetLayout *descriptorSetLayout = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TDescriptorSet(TDescriptorPool *descriptorPool, TDescriptorSetLayout *descriptorSetLayout);
    ~TDescriptorSet();

    VkDescriptorSet GetVkDescriptorSet();

    uint32_t GetSet();

    // TODO: this BindData function just for Test
    void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TBuffer *> &buffers);
    void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<std::pair<TImageView *, TSampler *>> &combinedImageSamplers);
    void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TImageView *> &imageViews);
    void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<TSampler *> &sampler);

    // FIXME: Need a ACCELERATION_STRUCTURE binding function
    /* FIXME: Just for Test*/ void BindData(uint32_t binding, uint32_t dstArrayElement, std::vector<VkAccelerationStructureKHR> &accelerationStructures);

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDESCRIPTORSET_H