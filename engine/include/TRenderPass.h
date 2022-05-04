#pragma once
#ifndef TRENDERPASS_H
#define TRENDERPASS_H
#include "TAttachment.h"
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TSubpass;
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

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TRENDERPASS_H