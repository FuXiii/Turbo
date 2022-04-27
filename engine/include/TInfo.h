#pragma once
#ifndef TINFO_H
#define TINFO_H
#include "TObject.h"

namespace Turbo
{
namespace Core
{
class TInfo : public TObject
{
  public:
    explicit TInfo();
    virtual ~TInfo();
};
} // namespace Core
} // namespace Turbo
#endif // !TINFO_H