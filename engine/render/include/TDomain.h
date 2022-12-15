#pragma once
#ifndef TURBO_RENDER_TDOMAIN_H
#define TURBO_RENDER_TDOMAIN_H
#include <stdint.h>

namespace Turbo
{
namespace Render
{
typedef enum TDomainBits
{
    CPU = 0x00000001,
    GPU = 0x00000002,
    BOTH = CPU | GPU
} TDomainBits;
using TDomain = uint32_t;
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TDOMAIN_H