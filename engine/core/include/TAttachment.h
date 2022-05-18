#pragma once
#ifndef TATTACHMENT_H
#define TATTACHMENT_H
#include "TFormatInfo.h"
#include "TImage.h"
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
class TImageView;

typedef enum class TLoadOp
{
    LOAD = 0,
    CLEAR = 1,
    DONT_CARE = 2,
} TLoadOp;

typedef enum class TStoreOp
{
    STORE = 0,
    DONT_CARE = 1,
    NONE = 1000301000,
} TStoreOp;

class TAttachment : public Turbo::Core::TInfo
{
  private:
    TFormatInfo format;
    TSampleCountBits samples;
    TLoadOp loadOp;
    TStoreOp storeOp;
    TLoadOp stencilLoadOp;
    TStoreOp stencilStoreOp;
    TImageLayout initialLayout;
    TImageLayout finalLayout;

  public:
    TAttachment(TFormatInfo format, TSampleCountBits samples, TLoadOp loadOp, TStoreOp storeOp, TLoadOp stencilLoadOp, TStoreOp stencilStoreOp, TImageLayout initialLayout, TImageLayout finalLayout);
    ~TAttachment();

  public:
    TFormatInfo GetFormat();
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