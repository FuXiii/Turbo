#pragma once
#ifndef TURBO_CORE_TRENDERPASS_H
#define TURBO_CORE_TRENDERPASS_H
#include "TAttachment.h"
#include "TSubpass.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TFramebuffer;
class TCommandBuffer;

class TRenderPass : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TDevice> device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkRenderPass vkRenderPass = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TSubpass> subpasses;
    T_VULKAN_HANDLE_DATA std::vector<TAttachment> attachments;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TRenderPass(const TRefPtr<TDevice> &device, std::vector<TAttachment> &attachments, std::vector<TSubpass> &subpasses);

  protected:
    virtual ~TRenderPass();

  public:
    VkRenderPass GetVkRenderPass();

    TRefPtr<TDevice> GetDevice();

    const std::vector<TAttachment> &GetAttachments() const;

    const std::vector<TSubpass> &GetSubpasses() const;

    TSubpass GetSubpass(uint32_t subpass) const;

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TRENDERPASS_H