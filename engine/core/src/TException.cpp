#include "TException.h"
#include <iostream>

Turbo::Core::TException::TException() : std::runtime_error("UNDEFINED") //, Turbo::Core::TObject()
{
    this->result = TResult::UNDEFINED;
}

Turbo::Core::TException::TException(TResult result, const std::string &message, const std::string &tip) : std::runtime_error(std::string("[Error]:") + std::string("[") + Turbo::Core::TResultToString(result) + std::string("]") + message + std::string("{") + tip + std::string("}"))
{
    this->result = result;
    this->message = message;
    this->tip = tip;
    if (!this->message.empty())
    {
        std::cout << "[Error]:" << this->message << "{" << this->tip << "}" << std::endl;
    }
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

std::string Turbo::Core::TException::GetTip()
{
    return this->tip;
}

std::string Turbo::Core::TException::ToString()
{
    std::string result(this->what());
    return result;
}