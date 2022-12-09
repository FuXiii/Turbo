#pragma once
#ifndef TURBO_CORE_TRENDERPASS_H
#define TURBO_CORE_TRENDERPASS_H
#include "TAttachment.h"
#include "TVulkanHandle.h"
#include "TSubpass.h"

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
    T_VULKAN_HANDLE_PARENT TDevice *device = nullptr;
    T_VULKAN_HANDLE_HANDLE VkRenderPass vkRenderPass = VK_NULL_HANDLE;
    T_VULKAN_HANDLE_CHILDREN std::vector<TSubpass> subpasses;
    T_VULKAN_HANDLE_DATA std::vector<TAttachment> attachments;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TRenderPass(TDevice *device, std::vector<TAttachment> &attachments, std::vector<TSubpass> &subpasses);
    ~TRenderPass();

    VkRenderPass GetVkRenderPass();

    TDevice *GetDevice();

    const std::vector<TAttachment> &GetAttachments();

    const std::vector<TSubpass> &GetSubpasses();

    TSubpass GetSubpass(uint32_t subpass);

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TRENDERPASS_H