#pragma once
#ifndef TDESCRIPTORSETLAYOUT_H
#define TDESCRIPTORSETLAYOUT_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TShader;
class TDescriptor;

class TDescriptorSetLayout : public Turbo::Core::TVulkanHandle
{
  public:
    friend class TShader;

  private:
    T_VULKAN_HANDLE_PARENT TShader *shader = nullptr;
    T_VULKAN_HANDLE_HANDLE VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;

    T_VULKAN_HANDLE_DATA std::vector<TDescriptor *> descriptors;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TDescriptorSetLayout(TShader *shader, std::vector<TDescriptor *> &descriptors);
    ~TDescriptorSetLayout();

    VkDescriptorSetLayout GetVkDescriptorSetLayout();

    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TDESCRIPTORSETLAYOUT_H