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

    return result + " : " + this->message;
}

char const *Turbo::Core::TException::what() const throw()
{
    switch (this->result)
    {
    case Turbo::Core::TResult::MEMORY_MAP_FAILED:
        return "Memory map failed";
        break;
    case Turbo::Core::TResult::OUT_OF_RANGE:
        return "Out of range";
        break;
    case Turbo::Core::TResult::INVALID_PARAMETER:
        return "Invalid parameter";
        break;
    case Turbo::Core::TResult::INVALID_EXTERNAL_HANDLE:
        return "Invalid external handle";
        break;
    case Turbo::Core::TResult::DEVICE_LOST:
        return "Device lost";
        break;
    case Turbo::Core::TResult::TOO_MANY_OBJECTS:
        return "Too many objects";
        break;
    case Turbo::Core::TResult::EXTENSION_NOT_PRESENT:
        return "Have extension not support";
        break;
    case Turbo::Core::TResult::LAYER_NOT_PRESENT:
        return "Have layer not support";
        break;
    case Turbo::Core::TResult::INITIALIZATION_FAILED:
        return "Initialization failed";
        break;
    case Turbo::Core::TResult::OUT_OF_DEVICE_MEMORY:
        return "Out of device memory";
        break;
    case Turbo::Core::TResult::OUT_OF_HOST_MEMORY:
        return "Out of host memory";
        break;
    case Turbo::Core::TResult::UNSUPPORTED:
        return "Not support vulkan";
        break;
    case Turbo::Core::TResult::FAIL:
        return "Instance fail";
        break;
    case Turbo::Core::TResult::UNDEFINED:
        return "Undefined";
        break;
    default:
        return "Undefined";
        break;
    }

    return "Undefined";
}
