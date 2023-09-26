#pragma once
#ifndef TURBO_CORE_TException_H
#define TURBO_CORE_TException_H
#include "TObject.h"
#include <exception>

namespace Turbo
{
namespace Core
{
class TException : public std::exception, public Turbo::Core::TObject
{
  private:
    TResult result;
    std::string message;
    std::string tip;

    std::string whatStr;

  public:
    TException();
    TException(TResult result, const std::string &message = "", const std::string &tip = "");
    ~TException();

  public:
    TResult GetResult();
    std::string GetMessage();
    std::string GetTip();

    virtual std::string ToString() override;
    virtual const char *what() const throw() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TException_H