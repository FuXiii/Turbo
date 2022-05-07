#pragma once
#ifndef TSURFACEFORMAT_H
#include "TColorSpace.h"
#include "TFormatInfo.h"
#include "TObject.h"

namespace Turbo
{
namespace Extension
{
class TSurface;
}
} // namespace Turbo

namespace Turbo
{
namespace Core
{
class TSurfaceFormat : public TObject
{
  public:
    friend class Turbo::Extension::TSurface;

  private:
    TFormatInfo format;
    TColorSpace colorSpace;

  public:
    explicit TSurfaceFormat();
    explicit TSurfaceFormat(TFormatInfo format, TColorSpace colorSpace);
    ~TSurfaceFormat();

  public:
    TFormatInfo GetFormat();
    TColorSpace GetColorSpace();

    virtual std::string ToString() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TSURFACEFORMAT_H