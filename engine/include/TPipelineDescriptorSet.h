#pragma once
#ifndef TPIPELINEDESCRIPTORSET_H
#define TPIPELINEDESCRIPTORSET_H
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
    T_VULKAN_HANDLE_PARENT TPipelineLayout *pipelineLayout = nullptr;
    T_VULKAN_HANDLE_PARENT TDescriptorPool *descriptorPool = nullptr;
    T_VULKAN_HANDLE_HANDLE std::vector<TDescriptorSet *> descriptorSets;
    T_VULKAN_HANDLE_CHILDREN;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TPipelineDescriptorSet(TDescriptorPool *descriptorPool, TPipelineLayout *pipelineLayout);
    ~TPipelineDescriptorSet();

    const std::vector<TDescriptorSet *> &GetDescriptorSet();

    // TODO: this BindData function just for Test
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TBuffer *> &buffers);
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<std::pair<TImageView *, TSampler *>> &combinedImageSamplers);
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TImageView *> &imageViews);
    void BindData(uint32_t set, uint32_t binding, uint32_t dstArrayElement, std::vector<TSampler *> &sampler);

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TPIPELINEDESCRIPTORSET_H