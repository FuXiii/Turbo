#pragma once
#ifndef TURBO_CORE_TBARRIER_H
#define TURBO_CORE_TBARRIER_H
#include "TImage.h"
#include "TImageView.h"
#include "TInfo.h"
#include "TBuffer.h"

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
    TAccess GetSrcAccess() const;
    TAccess GetDstAccess() const;

    virtual std::string ToString() const override;
};

class TBufferMemoryBarrier : public Turbo::Core::TMemoryBarrier
{
  private:
    TRefPtr<TBuffer> buffer; // TBuffer *buffer;
    TDeviceSize offset;
    TDeviceSize size;

  public:
    //[[deprecated]] TBufferMemoryBarrier(TAccess srcAccess, TAccess dstAccess, const TRefPtr<TBuffer> &buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE);
    TBufferMemoryBarrier(TAccess srcAccess, TAccess dstAccess, TBuffer *buffer, TDeviceSize offset = 0, TDeviceSize size = VK_WHOLE_SIZE);
    ~TBufferMemoryBarrier();

  public:
    // /*deprecated*/ const TRefPtr<TBuffer> &GetBuffer();
    TBuffer *GetBuffer();
    TDeviceSize GetOffset() const;
    TDeviceSize GetSize() const;

    virtual std::string ToString() const override;
};

class TImageMemoryBarrier : public Turbo::Core::TMemoryBarrier
{
  private:
    TImageLayout oldLayout;
    TImageLayout newLayout;
    TRefPtr<TImage> image; // TImage *image;
    TImageAspects aspects;
    uint32_t baseMipLevel;
    uint32_t levelCount;
    uint32_t baseArrayLayer;
    uint32_t layerCount;

  public:
    //[[deprecated]] TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, const TRefPtr<TImage> &image, TImageLayout oldLayout, TImageLayout newLayout, TImageAspects aspects, uint32_t baseMipLevel = 0, uint32_t levelCount = VK_REMAINING_MIP_LEVELS, uint32_t baseArrayLayer = 0, uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS);
    TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, TImage *image, TImageLayout oldLayout, TImageLayout newLayout, TImageAspects aspects, uint32_t baseMipLevel = 0, uint32_t levelCount = VK_REMAINING_MIP_LEVELS, uint32_t baseArrayLayer = 0, uint32_t layerCount = VK_REMAINING_ARRAY_LAYERS);
    //[[deprecated]] TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, const TRefPtr<TImageView> imageView, TImageLayout oldLayout, TImageLayout newLayout);
    TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, TImageView *imageView, TImageLayout oldLayout, TImageLayout newLayout);
    ~TImageMemoryBarrier() = default;

  public:
    TImageLayout GetOldLayout() const;
    TImageLayout GetNewLayout() const;
    // [[deprecated]] const TRefPtr<TImage> &GetImage();
    TImage *GetImage();
    TImageAspects GetAspects() const;
    uint32_t GetBaseMipLevel() const;
    uint32_t GetLevelCount() const;
    uint32_t GetBaseArrayLayer() const;
    uint32_t GetLayerCount() const;

    virtual std::string ToString() const override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TBARRIER_H