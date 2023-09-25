#include "TException.h"
#include <iostream>

Turbo::Core::TException::TException() : std::exception(), Turbo::Core::TObject()
{
    this->result = TResult::UNDEFINED;
}

Turbo::Core::TException::TException(TResult result, const std::string &message, const std::string &tip)
{
    this->result = result;
    this->message = message;
    this->tip = tip;
    if (!this->message.empty())
    {
        std::cout << "[Error]:" << this->message << "{" << this->tip << "}" << std::endl;
    }

    std::string what = std::string("[Error]:") + std::string("[") + Turbo::Core::TResultToString(this->result) + std::string("]") + this->message + std::string("{") + this->tip + std::string("}");
    exception(what.c_str());
}

Turbo::Core::TException::~TException()
{
}

Turbo::Core::TResult Turbo::Core::TException::GetResult()
{
    return this->result;
}

std::string Turbo::Core::TException::GetMessage()
{
    return this->message;
}

std::string Turbo::Core::TException::ToString()
{
    std::string result(this->what());
    return result;
}

// char const *Turbo::Core::TException::what() const throw()
// {

//     return "Undefined";
// }
