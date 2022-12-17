#pragma once
#ifndef TURBO_RENDER_TFORMAT_H
#define TURBO_RENDER_TFORMAT_H
namespace Turbo
{
namespace Render
{
typedef enum class TFormat
{
    R8G8B8A8_SRGB,
    B8G8R8A8_SRGB,
    R8G8B8_SRGB,
    B8G8R8_SRGB,
    R8G8B8A8_UNORM,
    B8G8R8A8_UNORM,
    R8G8B8_UNORM,
    B8G8R8_UNORM,
    D32_SFLOAT,
    D16_UNORM
} TFormat;
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TFORMAT_H