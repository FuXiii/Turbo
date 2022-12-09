#pragma once
#ifndef TURBO_RENDER_TIMAGE_H
#define TURBO_RENDER_TIMAGE_H
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
class TImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t layers;
        //...
    };

  private:
    Turbo::Core::TImage *image = nullptr;
    Turbo::Core::TImageView *imageView = nullptr;

    Descriptor descriptor;

  public:
    TImage() = default;
    ~TImage() = default;

    void Create(const std::string &name, const Descriptor &descriptor /*TODO: we need a allocator/context*/);
    void Destroy(/*TODO: we need a allocator/context*/);
};

class TColorImage : public TImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t layers;
        //...
    };

  public:
    TColorImage() = default;
    ~TColorImage() = default;

    void Create(const std::string &name, const Descriptor &descriptor /*TODO: we need a allocator/context*/);
};

class TColorImage2D : public TColorImage
{
  public:
    struct Descriptor
    {
        uint32_t width;
        uint32_t height;
        uint32_t layers;
        //...
    };

  public:
    TColorImage2D() = default;
    ~TColorImage2D() = default;

    void Create(const std::string &name, const Descriptor &descriptor /*TODO: we need a allocator/context*/);
};

void Test();
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TIMAGE_H