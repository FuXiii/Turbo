#pragma once
#ifndef TURBO_RENDER_TSAMPLER_H
#define TURBO_RENDER_TSAMPLER_H
#include <string>

namespace Turbo
{
namespace Core
{
class TSampler;
}
} // namespace Turbo

namespace Turbo
{
namespace Render
{
class TSampler
{
  public:
    typedef enum class TFilter
    {
        NEAREST = 0,
        LINEAR = 1,
    } TFilter;

    typedef enum class TMipmapMode
    {
        NEAREST = 0,
        LINEAR = 1,
    } TMipmapMode;

    typedef enum class TAddressMode
    {
        REPEAT = 0,
        MIRRORED_REPEAT = 1,
        CLAMP_TO_EDGE = 2,
        CLAMP_TO_BORDER = 3,
        MIRROR_CLAMP_TO_EDGE = 4,
    } TAddressMode;

  public:
    struct Descriptor
    {
        Turbo::Render::TSampler::TFilter min = Turbo::Render::TSampler::TFilter::LINEAR;
        Turbo::Render::TSampler::TFilter max = Turbo::Render::TSampler::TFilter::LINEAR;
        Turbo::Render::TSampler::TMipmapMode mipmap = Turbo::Render::TSampler::TMipmapMode::LINEAR;
        Turbo::Render::TSampler::TAddressMode U = Turbo::Render::TSampler::TAddressMode::REPEAT;
        Turbo::Render::TSampler::TAddressMode V = Turbo::Render::TSampler::TAddressMode::REPEAT;
        Turbo::Render::TSampler::TAddressMode W = Turbo::Render::TSampler::TAddressMode::REPEAT;
    };

    friend class TContext;

  private:
    void *allocator = nullptr;
    Turbo::Core::TSampler *sampler = nullptr;
    Descriptor descriptor;

  public:
    TSampler() = default;
    ~TSampler() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);

    bool IsValid() const;
};
} // namespace Render
} // namespace Turbo

#endif