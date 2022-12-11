#pragma once
#ifndef TURBO_RENDER_TFORMAT_H
#define TURBO_RENDER_TFORMAT_H
namespace Turbo
{
namespace Render
{
typedef enum class TFormat
{
    B8G8R8A8_SRGB,
    D32_SFLOAT,
    R8G8B8A8_UNORM,
    // R32G32B32_SFLOAT,
    //
} TFormat;
} // namespace Redner
} // namespace Turbo
#endif // !TURBO_RENDER_TFORMAT_H