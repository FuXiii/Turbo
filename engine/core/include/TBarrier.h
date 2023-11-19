#pragma once
#ifndef TURBO_CORE_TBARRIER_H
#define TURBO_CORE_TBARRIER_H
#include "TImage.h"
#include "TImageView.h"
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
class TBuffer;

class TBarrier : public Turbo::Core::TInfo
{
  public:
    TBarrier();
    ~TBarrier();
};

class TMemoryBarrier : public Turbo::Core::TBarrier
{
  private:
    TAccess srcAccess;
    TAccess dstAccess;

  public:
    TMemoryBarrier(TAccess srcAccess, TAccess dstAccess);
    ~TMemoryBarrier();

  public:
    TAccess GetSrcAccess();
    TAccess GetDstAccess();

    virtual std::string ToString() override;
};

class TBufferMemoryBarrier : public Turbo::Core::TMemoryBarrier
{
  private:
    TBuffer *buffer;
    TDeviceSize offset;
    TDeviceSize size;

  public:
    TBufferMemoryBarrier(TAccess srcAccess, TAccess dstAccess, TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE);
    ~TBufferMemoryBarrier();

  public:
    TBuffer *GetBuffer();
    TDeviceSize GetOffset();
    TDeviceSize GetSize();

    virtual std::string ToString() override;
};

class TImageMemoryBarrier : public Turbo::Core::TMemoryBarrier
{
  private:
    TImageLayout oldLayout;
    TImageLayout newLayout;
    TImage *image;
    TImageAspects aspects;
    uint32_t baseMipLevel;
    uint32_t levelCount;
    uint32_t baseArrayLayer;
    uint32_t layerCount;

  public:
    TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, TImage *image, TImageLayout oldLayout, TImageLayout newLayout, TImageAspects aspects, uint32_t baseMipLevel = 0, uint32_t levelCount = VK_REMAINING_MIP_LEVELS, uint32_t baseArrayLayer = 0, uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS);
    TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, TImageView *view, TImageLayout oldLayout, TImageLayout newLayout);
    ~TImageMemoryBarrier();

  public:
    TImageLayout GetOldLayout();
    TImageLayout GetNewLayout();
    TImage *GetImage();
    TImageAspects GetAspects();
    uint32_t GetBaseMipLevel();
    uint32_t GetLevelCount();
    uint32_t GetBaseArrayLayer();
    uint32_t GetLayerCount();

    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TBARRIER_H