#pragma once
#ifndef TDESCRIPTORSETLAYOUT_H
#define TDESCRIPTORSETLAYOUT_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDescriptor;
class TDevice;

class TDescriptorSetLayout : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TDescriptor *> descriptors;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TDescriptorSetLayout(TDevice *device, std::vector<TDescriptor *> &descriptors);
    ~TDescriptorSetLayout();

    uint32_t GetSet();
    VkDescriptorSetLayout GetVkDescriptorSetLayout();

    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TDESCRIPTORSETLAYOUT_H