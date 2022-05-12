#pragma once
#ifndef TDESCRIPTORPOOL_H
#define TDESCRIPTORPOOL_H
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

    TDescriptorType GetDescriptorType();
    uint32_t GetDescriptorCount();

  public:
    virtual std::string ToString() override;
};

class TDescriptorPool : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN;

    uint32_t maxSetsCount;
    std::vector<TDescriptorSize> descriptorSizes;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    explicit TDescriptorPool(TDevice *device, uint32_t maxSetsCount, std::vector<TDescriptorSize> &descriptorSizes);
    ~TDescriptorPool();

    TPipelineDescriptorSet *Allocate(TPipelineLayout *pipelineLayout);
    void Free(TPipelineDescriptorSet *pipelineDescriptorSet);

    TDevice *GetDevice();
    VkDescriptorPool GetVkDescriptorPool();

  public:
    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TDESCRIPTORPOOL_H