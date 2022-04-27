#pragma once
#ifndef TATTACHMENT_H
#define TATTACHMENT_H
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
class TImageView;

class TAttachment : public Turbo::Core::TInfo
{
  private:
    VkFormat format;
    VkSampleCountFlagBits samples;
    VkAttachmentLoadOp loadOp;
    VkAttachmentStoreOp storeOp;
    VkAttachmentLoadOp stencilLoadOp;
    VkAttachmentStoreOp stencilStoreOp;
    VkImageLayout initialLayout;
    VkImageLayout finalLayout;

  public:
    TAttachment(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout);
    ~TAttachment();

  public:
    VkFormat GetVkFormat();
    VkSampleCountFlagBits GetVkSampleCountFlagBits();
    VkAttachmentLoadOp GetLoadOp();
    VkAttachmentStoreOp GetStoreOp();
    VkAttachmentLoadOp GetStencilLoadOp();
    VkAttachmentStoreOp GetStencilStoreOp();
    VkImageLayout GetInitialLayout();
    VkImageLayout GetFinalLayout();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TATTACHMENT_H