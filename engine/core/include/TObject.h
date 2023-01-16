#pragma once
#ifndef TURBO_CORE_TOBJECT_H
#define TURBO_CORE_TOBJECT_H
#include "TCore.h"
#include <string.h>
#include <string>

namespace Turbo
{
namespace Core
{
class TObject
{
  public:
    explicit TObject();
    virtual ~TObject();

  public:
    virtual std::string ToString() = 0;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TOBJECT_H