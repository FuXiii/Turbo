#pragma once
#ifndef TURBO_RENDER_TIMAGE_H
#define TURBO_RENDER_TIMAGE_H
#include "TDomain.h"
#include "TFormat.h"
#include <cstdint>
#include <string>

namespace Turbo
{
namespace Core
{
class TImage;
class TImageView;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Render
{
typedef enum TImageCreateFlagBits
{
    CUBE = 0x00000001,
} TImageCreateFlagBits;
using TImageCreateFlags = uint32_t;

typedef enum TImageUsageBits
{
    TRANSFER_SRC = 0x00000001,
    TRANSFER_DST = 0x00000002,
    SAMPLED = 0x00000004,
    STORAGE = 0x00000008,
    COLOR_ATTACHMENT = 0x00000010,
    DEPTH_STENCIL_ATTACHMENT = 0x00000020,
    TRANSIENT_ATTACHMENT = 0x00000040,
    INPUT_ATTACHMENT = 0x00000080,
} TImageUsageBits;
using TImageUsages = uint32_t;

typedef enum TSampleCountBits
{
    SAMPLE_1_BIT = 0x00000001,
    SAMPLE_2_BIT = 0x00000002,
    SAMPLE_4_BIT = 0x00000004,
    SAMPLE_8_BIT = 0x00000008,
    SAMPLE_16_BIT = 0x00000010,
    SAMPLE_32_BIT = 0x00000020,
    SAMPLE_64_BIT = 0x00000040,
} TSampleCountBits;
using TSampleCounts = uint32_t;

class TImage
{
  public:
    struct Descriptor
    {
        TImageCreateFlags flags;
        TFormat format;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t layers;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

    friend class TRenderPassPool;

  private:
    Turbo::Core::TImage *image = nullptr;
    Turbo::Core::TImageView *imageView = nullptr;

    Descriptor descriptor;

  protected:
    virtual void CreateImageView();

  public:
    TImage() = default;
    ~TImage() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);

    TFormat GetFormat() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    uint32_t GetDepth() const;
    uint32_t GetLayers() const;
    uint32_t GetMipLevels() const;
    TImageUsages GetUsages() const;
    TDomain GetDomain() const;
    TSampleCountBits GetSampleCountBits() const;
    bool IsValid() const;
};

class TColorImage : public TImage
{
  public:
    struct Descriptor
    {
        TImageCreateFlags flags;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t layers;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  public:
    TColorImage() = default;
    ~TColorImage() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TColorImage2D : public TColorImage
{
  public:
    struct Descriptor
    {
        TImageCreateFlags flags;
        uint32_t width;
        uint32_t height;
        uint32_t layers;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  public:
    TColorImage2D() = default;
    ~TColorImage2D() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TColorImage3D : public TColorImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  public:
    TColorImage3D() = default;
    ~TColorImage3D() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TDepthStencilImage : public TImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t layers;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  public:
    TDepthStencilImage() = default;
    ~TDepthStencilImage() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TDepthImage : public TDepthStencilImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t layers;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  public:
    TDepthImage() = default;
    ~TDepthImage() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TDepthImage2D : public TDepthImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t layers;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  public:
    TDepthImage2D() = default;
    ~TDepthImage2D() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TTexture2D : public TColorImage2D
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  protected:
    void CreateImageView() override;

  public:
    TTexture2D() = default;
    ~TTexture2D() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TTexture3D : public TColorImage3D
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  protected:
    void CreateImageView() override;

  public:
    TTexture3D() = default;
    ~TTexture3D() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

class TDepthTexture2D : public TDepthImage2D
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t mipLevels;
        TImageUsages usages;
        TDomain domain;
    };

  protected:
    void CreateImageView() override;

  public:
    TDepthTexture2D() = default;
    ~TDepthTexture2D() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
};

} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TIMAGE_H