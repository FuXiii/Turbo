#pragma once
#ifndef TURBO_RENDER_TFORMAT_H
#define TURBO_RENDER_TFORMAT_H
namespace Turbo
{
namespace Render
{
typedef enum class TFormat
{
    UNDEFINED = 0,
    R8G8B8A8_SRGB = 43,
    B8G8R8A8_SRGB = 50,
    R8G8B8_SRGB = 29,
    B8G8R8_SRGB = 36,
    R8G8B8A8_UNORM = 37,
    B8G8R8A8_UNORM = 44,
    R8G8B8_UNORM = 23,
    B8G8R8_UNORM = 30,
    D32_SFLOAT = 126,
    D16_UNORM = 124
} TFormat;
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TFORMAT_H