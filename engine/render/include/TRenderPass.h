#pragma once
#include <cstdint>
#include <stdint.h>
#ifndef TURBO_RENDER_TRENDERPASS_H
#define TURBO_RENDER_TRENDERPASS_H
#include <stdint.h>

namespace Turbo
{
namespace Render
{
class TRenderPass
{
  private:
    uint32_t testValue = 0;
    bool testIsSomething = false;

  public:
    TRenderPass() = default;
    ~TRenderPass() = default;

    TRenderPass &SetTestValue(uint32_t value);
    TRenderPass &SetTestIsSomething(bool isSomething);
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TRENDERPASS_H