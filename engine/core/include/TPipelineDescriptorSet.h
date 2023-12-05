#pragma once
#ifndef TURBO_CORE_TPIPELINEDESCRIPTORSET_H
#define TURBO_CORE_TPIPELINEDESCRIPTORSET_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDescriptorPool;
class TDescriptorSet;
class TPipelineLayout;
class TBuffer;
class TImageView;
class TSampler;

class TPipelineDescriptorSet : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TPipelineLayout> pipelineLayout = nullptr;
    T_VULKAN_HANDLE_PARENT TRefPtr<TDescriptorPool> descriptorPool = nullptr;
    T_VULKAN_HANDLE_HANDLE std::vector<TRefPtr<TDescriptorSet>> descriptorSets;
    T_VULKAN_HANDLE_CHILDREN;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TPipelineDescriptorSet(const TRefPtr<TDescriptorPool> &descriptorPool, const TRefPtr<TPipelineLayout> &pipelineLayout);

  protected:
    virtual ~TPipelineDescriptorSet();

  public:
    const std::vector<TRefPtr<TDescriptorSet>> &GetDescriptorSet();

    // TODO: this BindData function just for Test
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TRefPtr<TBuffer>> &buffers);
    void BindData(uint32_t set, uint32_t binding, const TRefPtr<TBuffer> &buffer, uint32_t dstArrayElement = 0);
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<std::pair<TRefPtr<TImageView>, TRefPtr<TSampler>>> &combinedImageSamplers);
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TRefPtr<TImageView>> &imageViews);
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TRefPtr<TSampler>> &samplers);

    // FIXME: Need a ACCELERATION_STRUCTURE binding function
    /* FIXME: Just for Test*/ void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<VkAccelerationStructureKHR> &accelerationStructures);

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TPIPELINEDESCRIPTORSET_H