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
    T_VULKAN_HANDLE_PARENT TRenderPass *renderPass = nullptr;
    T_VULKAN_HANDLE_HANDLE VkFramebuffer vkFramebuffer = VK_NULL_HANDLE;
    std::vector<TImageView *> attachments;

    uint32_t width;
    uint32_t height;

  protected:
    virtual void InternalCreate() override;
    virtual void InternalDestroy() override;

  public:
    TFramebuffer(TRenderPass *renderPass, std::vector<TImageView *> &attachments);
    ~TFramebuffer();

    VkFramebuffer GetVkFramebuffer();

    uint32_t GetWidth();
    uint32_t GetHeight();

    TRenderPass *GetRenderPass();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TFRAMEBUFFER_H