#pragma once
#ifndef TURBO_CORE_TDESCRIPTORSETLAYOUT_H
#define TURBO_CORE_DESCRIPTORSETLAYOUT_H
#include "TDescriptor.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;

class TDescriptorSetLayout : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TDescriptor *> descriptors;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TDescriptorSetLayout(const TRefPtr<TDevice> &device, std::vector<TDescriptor *> &descriptors);

  protected:
    virtual ~TDescriptorSetLayout();

  public:
    uint32_t GetSet() const;
    VkDescriptorSetLayout GetVkDescriptorSetLayout();
    TDescriptorType GetDescriptorType(uint32_t binding) const;

    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TDESCRIPTORSETLAYOUT_H