#pragma once
#ifndef TURBO_CORE_TDESCRIPTORPOOL_H
#define TURBO_CORE_TDESCRIPTORPOOL_H
#include "TDescriptor.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{

class TDevice;
class TPipelineDescriptorSet;
class TPipelineLayout;

class TDescriptorSize : public Turbo::Core::TInfo
{
  private:
    TDescriptorType type;
    uint32_t count;

  public:
    TDescriptorSize(TDescriptorType type, uint32_t count);
    ~TDescriptorSize();

  public:
    TDescriptorType GetDescriptorType() const;
    uint32_t GetDescriptorCount() const;

  public:
    virtual std::string ToString() const override;
};

class TDescriptorPool : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TPipelineDescriptorSet>> pipelineDescriptorSets;

    uint32_t maxSetsCount;
    std::vector<TDescriptorSize> descriptorSizes;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TDescriptorPool(const TRefPtr<TDevice> &device, uint32_t maxSetsCount, std::vector<TDescriptorSize> &descriptorSizes);

  protected:
    virtual ~TDescriptorPool();

  public:
    TRefPtr<TPipelineDescriptorSet> &Allocate(const TRefPtr<TPipelineLayout> &pipelineLayout);
    void Free(TRefPtr<TPipelineDescriptorSet> &pipelineDescriptorSet);

    const TRefPtr<TDevice> &GetDevice();
    VkDescriptorPool GetVkDescriptorPool();

  public:
    virtual std::string ToString() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDESCRIPTORPOOL_H