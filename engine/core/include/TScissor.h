#pragma once
#ifndef TURBO_CORE_TSCISSOR_H
#define TURBO_CORE_TSCISSOR_H
#include "TInfo.h"

namespace Turbo
{
namespace Core
{
class TDevice;
class TPipeline;

class TScissor : public Turbo::Core::TInfo
{
  private:
    int32_t offsetX;
    int32_t offsetY;
    uint32_t width;
    uint32_t height;

  public:
    TScissor(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height);
    ~TScissor();

  public:
    int32_t GetOffsetX();
    int32_t GetOffsetY();
    uint32_t GetWidth();
    uint32_t GetHeight();

  public:
    virtual std::string ToString() override;
};

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TSCISSOR_H