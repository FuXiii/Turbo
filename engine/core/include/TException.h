#pragma once
#ifndef TURBO_CORE_TException_H
#define TURBO_CORE_TException_H
#include "TObject.h"
#include <stdexcept>

namespace Turbo
{
namespace Core
{

// TODO: inherit std::runtime_error
class TException : public std::runtime_error //, public Turbo::Core::TObject
{
  private:
    TResult result;
    std::string message;
    std::string tip;

  public:
    TException();
    TException(TResult result, const std::string &message = "", const std::string &tip = "");
    ~TException();

  public:
    TResult GetResult() const;
    std::string GetMessage() const;
    std::string GetTip() const;

    virtual std::string ToString() const;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TException_H