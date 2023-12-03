#include "TBarrier.h"
#include "TException.h"

Turbo::Core::TBarrier::TBarrier() : Turbo::Core::TInfo()
{
}

Turbo::Core::TBarrier::~TBarrier()
{
}

Turbo::Core::TMemoryBarrier::TMemoryBarrier(TAccess srcAccess, TAccess dstAccess) : Turbo::Core::TBarrier()
{
    this->srcAccess = srcAccess;
    this->dstAccess = dstAccess;
}

Turbo::Core::TMemoryBarrier::~TMemoryBarrier()
{
}

Turbo::Core::TAccess Turbo::Core::TMemoryBarrier::GetSrcAccess() const
{
    return this->srcAccess;
}

Turbo::Core::TAccess Turbo::Core::TMemoryBarrier::GetDstAccess() const
{
    return this->dstAccess;
}

std::string Turbo::Core::TMemoryBarrier::ToString()
{
    return std::string();
}

Turbo::Core::TBufferMemoryBarrier::TBufferMemoryBarrier(TAccess srcAccess, TAccess dstAccess, const TRefPtr<TBuffer> &buffer, TDeviceSize offset, TDeviceSize size) : Turbo::Core::TMemoryBarrier(srcAccess, dstAccess)
{
    if (buffer.Valid())
    {
        this->buffer = buffer;
        this->offset = offset;
        this->size = size;
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TBufferMemoryBarrier::TBufferMemoryBarrier");
    }
}

Turbo::Core::TBufferMemoryBarrier::~TBufferMemoryBarrier()
{
}

Turbo::Core::TRefPtr<Turbo::Core::TBuffer> Turbo::Core::TBufferMemoryBarrier::GetBuffer()
{
    return this->buffer;
}

Turbo::Core::TDeviceSize Turbo::Core::TBufferMemoryBarrier::GetOffset() const
{
    return this->offset;
}

Turbo::Core::TDeviceSize Turbo::Core::TBufferMemoryBarrier::GetSize() const
{
    return this->size;
}

std::string Turbo::Core::TBufferMemoryBarrier::ToString()
{
    return std::string();
}

Turbo::Core::TImageMemoryBarrier::TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, const TRefPtr<TImage> &image, TImageLayout oldLayout, TImageLayout newLayout, TImageAspects aspects, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount) : Turbo::Core::TMemoryBarrier(srcAccess, dstAccess)
{
    if (image->Valid())
    {
        this->oldLayout = oldLayout;
        this->newLayout = newLayout;
        this->image = image;
        this->aspects = aspects;
        this->baseMipLevel = baseMipLevel;
        this->levelCount = levelCount;
        this->baseArrayLayer = baseArrayLayer;
        this->layerCount = layerCount;
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImageMemoryBarrier::TImageMemoryBarrier");
    }
}

Turbo::Core::TImageMemoryBarrier::TImageMemoryBarrier(TAccess srcAccess, TAccess dstAccess, const TRefPtr<TImageView> imageView, TImageLayout oldLayout, TImageLayout newLayout) : Turbo::Core::TMemoryBarrier(srcAccess, dstAccess)
{
    if (imageView.Valid())
    {
        this->oldLayout = oldLayout;
        this->newLayout = newLayout;
        this->image = imageView->GetImage();
        this->aspects = imageView->GetAspects();
        this->baseMipLevel = imageView->GetBaseMipLevel();
        this->levelCount = imageView->GetLevelCount();
        this->baseArrayLayer = imageView->GetBaseArrayLayer();
        this->layerCount = imageView->GetLayerCount();
    }
    else
    {
        throw Turbo::Core::TException(TResult::INVALID_PARAMETER, "Turbo::Core::TImageMemoryBarrier::TImageMemoryBarrier");
    }
}

Turbo::Core::TImageLayout Turbo::Core::TImageMemoryBarrier::GetOldLayout() const
{
    return this->oldLayout;
}

Turbo::Core::TImageLayout Turbo::Core::TImageMemoryBarrier::GetNewLayout() const
{
    return this->newLayout;
}

Turbo::Core::TRefPtr<Turbo::Core::TImage> Turbo::Core::TImageMemoryBarrier::GetImage()
{
    return this->image;
}

Turbo::Core::TImageAspects Turbo::Core::TImageMemoryBarrier::GetAspects() const
{
    return this->aspects;
}

uint32_t Turbo::Core::TImageMemoryBarrier::GetBaseMipLevel() const
{
    return this->baseMipLevel;
}

uint32_t Turbo::Core::TImageMemoryBarrier::GetLevelCount() const
{
    return this->levelCount;
}

uint32_t Turbo::Core::TImageMemoryBarrier::GetBaseArrayLayer() const
{
    return this->baseArrayLayer;
}

uint32_t Turbo::Core::TImageMemoryBarrier::GetLayerCount() const
{
    return this->layerCount;
}

std::string Turbo::Core::TImageMemoryBarrier::ToString()
{
    return std::string();
}