#pragma once
#ifndef TIMAGE_H
#define TIMAGE_H
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

    void Create(const std::string &name, const Descriptor &descriptor);
    void Destroy();
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

    void Create(const std::string &name, const Descriptor &descriptor);
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

    void Create(const std::string &name, const Descriptor &descriptor);
};

void Test();
} // namespace Render
} // namespace Turbo
#endif // !TIMAGE_H