#pragma once
#ifndef TOBJECT_H
#define TOBJECT_H
#include "TCore.h"
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
#endif // !TOBJECT_H