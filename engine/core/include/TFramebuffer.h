#pragma once
#ifndef TURBO_CORE_TFRAMEBUFFER_H
#define TURBO_CORE_TFRAMEBUFFER_H
#include "TVulkanHandle.h"

namespace Turbo
{
namespace Core
{
class TRenderPass;
class TImageView;

class TFramebuffer : public Turbo::Core::TVulkanHandle
{
  private:
    T_VULKAN_HANDLE_PARENT TRefPtr<TRenderPass> renderPass;
    T_VULKAN_HANDLE_HANDLE VkFramebuffer vkFramebuffer = VK_NULL_HANDLE;
    std::vector<TRefPtr<TImageView>> attachments;

    uint32_t width;
    uint32_t height;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    // TFramebuffer(const TRefPtr<TRenderPass> &renderPass, std::vector<TRefPtr<TImageView>> &attachments);
    TFramebuffer(TRenderPass *renderPass, const std::vector<TRefPtr<TImageView>> &attachments);
    TFramebuffer(TRenderPass *renderPass, const std::vector<TImageView *> &attachments);

  protected:
    virtual ~TFramebuffer();

  public:
    VkFramebuffer GetVkFramebuffer();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    TRenderPass *GetRenderPass();

    std::vector<TImageView *> GetAttachments();

  public:
    virtual std::string ToString() const override;
    virtual bool Valid() const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TFRAMEBUFFER_H