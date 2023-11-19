#pragma once
#ifndef TURBO_CORE_TINFO_H
#define TURBO_CORE_TINFO_H
#include "TObject.h"

namespace Turbo
{
namespace Core
{
class TInfo //: public TObject
{
  public:
    explicit TInfo();
    ~TInfo();

    virtual std::string ToString() = 0;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TINFO_H