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
    TDescriptor::TType type;
    uint32_t count;

  public:
    TDescriptorSize(TDescriptor::TType type, uint32_t count);
    ~TDescriptorSize() = default;

  public:
    TDescriptor::TType GetDescriptorType() const;
    uint32_t GetDescriptorCount() const;

  public:
    virtual std::string ToString() const override;
};

class TDescriptorPool : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device;
    T_VULKAN_HANDLE_HANDLE VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TRefPtr<TPipelineDescriptorSet>> pipelineDescriptorSets;

    uint32_t maxSetsCount;
    std::vector<TDescriptorSize> descriptorSizes;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    // TDescriptorPool(const TRefPtr<TDevice> &device, uint32_t maxSetsCount, std::vector<TDescriptorSize> &descriptorSizes);
    TDescriptorPool(TDevice *device, uint32_t maxSetsCount, const std::vector<TDescriptorSize> &descriptorSizes);

  protected:
    virtual ~TDescriptorPool();

  public:
    TPipelineDescriptorSet *Allocate(TPipelineLayout *pipelineLayout);
    void Free(TPipelineDescriptorSet *pipelineDescriptorSet);

    TDevice *GetDevice();
    VkDescriptorPool GetVkDescriptorPool();

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDESCRIPTORPOOL_H