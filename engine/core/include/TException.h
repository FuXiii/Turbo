#pragma once
#ifndef TException_H
#define TException_H
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

  public:
    TException();
    TException(TResult result, const std::string &message = "", const std::string &tip = "");
    ~TException();

  public:
    TResult GetResult();
    std::string GetMessage();

    virtual std::string ToString() override;
    virtual const char *what() const throw() override;
};
} // namespace Core
} // namespace Turbo
#endif // !TException_H