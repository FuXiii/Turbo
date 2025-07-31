#pragma once
#ifndef TURBO_CORE_TDESCRIPTORSET_H
#define TURBO_CORE_TDESCRIPTORSET_H
#include "TVulkanHandle.h"
#include "TDescriptorSetLayout.h"
#include "TBuffer.h"

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
    T_VULKAN_HANDLE_PARENT TRefPtr<TDescriptorPool> descriptorPool;
    T_VULKAN_HANDLE_PARENT TRefPtr<TDescriptorSetLayout> descriptorSetLayout;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TDescriptorSet(TDescriptorPool *descriptorPool, TDescriptorSetLayout *descriptorSetLayout);

  protected:
    virtual ~TDescriptorSet();

  public:
    VkDescriptorSet GetVkDescriptorSet();

    // [[deprecated]] uint32_t GetSet() const;

    // TODO: this BindData function just for Test
    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TBuffer *> &buffers);
    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TBuffer>> &buffers);
    void BindData(uint32_t binding, TBuffer *buffer, uint32_t dstArrayElement = 0);

    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<std::pair<TImageView *, TSampler *>> &combinedImageSamplers);
    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<std::pair<TRefPtr<TImageView>, TRefPtr<TSampler>>> &combinedImageSamplers);
    void BindData(uint32_t binding, const std::pair<TImageView *, TSampler *> &combinedImageSampler, uint32_t dstArrayElement = 0);

    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TImageView *> &imageViews);
    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TImageView>> &imageViews);
    void BindData(uint32_t binding, TImageView *imageView, uint32_t dstArrayElement = 0);

    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TSampler *> &samplers);
    void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<TRefPtr<TSampler>> &samplers);
    void BindData(uint32_t binding, TSampler *sampler, uint32_t dstArrayElement = 0);

    // FIXME: Need a ACCELERATION_STRUCTURE binding function
    /* FIXME: Just for Test*/ void BindData(uint32_t binding, uint32_t dstArrayElement, const std::vector<VkAccelerationStructureKHR> &accelerationStructures);

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDESCRIPTORSET_H