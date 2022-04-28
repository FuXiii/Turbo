#pragma once
#ifndef TSUBPASS_H
#define TSUBPASS_H
#include "TImage.h"
#include "TInfo.h"

namespace Turbo
{
namespace Core
{

class TSubpass : public Turbo::Core::TInfo
{
  private:
    std::vector<VkAttachmentReference> *colors;
    std::vector<VkAttachmentReference> *resolves;
    std::vector<VkAttachmentReference> *inputs;
    std::vector<VkAttachmentReference> *preserves;
    VkAttachmentReference *depthStencil;

  public:
    TSubpass();
    TSubpass(const TSubpass &obj);
    ~TSubpass();

    void AddColorAttachmentReference(uint32_t attachment, TImageLayout layout);
    void AddResolveAttachmentReference(uint32_t attachment, TImageLayout layout);
    void AddInputAttachmentReference(uint32_t attachment, TImageLayout layout);
    void AddPreserveAttachmentReference(uint32_t attachment, TImageLayout layout);
    void SetDepthStencilAttachmentReference(uint32_t attachment, TImageLayout layout);
    void Clear();

    std::vector<VkAttachmentReference> *GetColorAttachmentReferences();
    std::vector<VkAttachmentReference> *GetResolveAttachmentReferences();
    std::vector<VkAttachmentReference> *GetInputAttachmentReferences();
    std::vector<VkAttachmentReference> *GetPreserveAttachmentReferences();
    VkAttachmentReference *GetDepthStencilAttachmentReference();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TSUBPASS_H