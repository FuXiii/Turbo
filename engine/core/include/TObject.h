#pragma once
#ifndef TURBO_CORE_TOBJECT_H
#define TURBO_CORE_TOBJECT_H
#include "TCore.h"
#include "TReferenced.h"
#include <string.h>
#include <string>

namespace Turbo
{
namespace Core
{
class TObject : public TReferenced
{
  public:
    explicit TObject();

  protected:
    virtual ~TObject();

  public:
    virtual std::string ToString() const = 0;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TOBJECT_H