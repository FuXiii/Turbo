#pragma once
#ifndef TATTACHMENT_H
#define TATTACHMENT_H
#include "TImage.h"
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
class TImageView;

typedef enum TLoadOp
{
    LOAD_OP_LOAD = 0,
    LOAD_OP_CLEAR = 1,
    LOAD_OP_DONT_CARE = 2,
} TLoadOp;

typedef enum TStoreOp
{
    STORE_OP_STORE = 0,
    STORE_OP_DONT_CARE = 1,
    STORE_OP_NONE = 1000301000,
} TStoreOp;

class TAttachment : public Turbo::Core::TInfo
{
  private:
    VkFormat format;
    TSampleCountBits samples;
    TLoadOp loadOp;
    TStoreOp storeOp;
    TLoadOp stencilLoadOp;
    TStoreOp stencilStoreOp;
    TImageLayout initialLayout;
    TImageLayout finalLayout;

  public:
    TAttachment(VkFormat format, TSampleCountBits samples, TLoadOp loadOp, TStoreOp storeOp, TLoadOp stencilLoadOp, TStoreOp stencilStoreOp, TImageLayout initialLayout, TImageLayout finalLayout);
    ~TAttachment();

  public:
    VkFormat GetVkFormat();
    TSampleCountBits GetVkSampleCountFlagBits();
    TLoadOp GetLoadOp();
    TStoreOp GetStoreOp();
    TLoadOp GetStencilLoadOp();
    TStoreOp GetStencilStoreOp();
    TImageLayout GetInitialLayout();
    TImageLayout GetFinalLayout();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TATTACHMENT_H