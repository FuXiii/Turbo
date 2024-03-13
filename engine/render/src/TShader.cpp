#include "render/include/TShader.h"
#include "render/include/TContext.h"
#include <core/include/TException.h>
#include <core/include/TDevice.h>
#include <core/include/TShader.h>

Turbo::Render::TComputeShader::TComputeShader(Turbo::Render::TContext *context, TShader::TLanguage language, const std::string &code)
{
    if (context != nullptr)
    {
        Turbo::Core::TDevice *device = context->GetDevice();
        Turbo::Core::TShaderLanguage shader_language = Turbo::Core::TShaderLanguage::GLSL;

        switch (language)
        {
        case TShader::TLanguage::GLSL: {
            shader_language = Turbo::Core::TShaderLanguage::GLSL;
        }
        break;
        case TShader::TLanguage::HLSL: {
            shader_language = Turbo::Core::TShaderLanguage::HLSL;
        }
        break;
        }
        this->computeShader = new Turbo::Core::TComputeShader(device, shader_language, code);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TComputeShader::TComputeShader", "please ensure context is valid");
    }
}

Turbo::Render::TComputeShader::TComputeShader(TContext *context, size_t size, uint32_t *code)
{
    if (context != nullptr)
    {
        Turbo::Core::TDevice *device = context->GetDevice();
        this->computeShader = new Turbo::Core::TComputeShader(device, size, code);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TComputeShader::TComputeShader", "please ensure context is valid");
    }
}

Turbo::Render::TComputeShader::~TComputeShader()
{
    this->computeShader = nullptr;
}

Turbo::Core::TRefPtr<Turbo::Core::TComputeShader> Turbo::Render::TComputeShader::GetComputeShader()
{
    return this->computeShader;
}

Turbo::Render::TVertexShader::TVertexShader(Turbo::Render::TContext *context, TShader::TLanguage language, const std::string &code)
{
    if (context != nullptr)
    {
        Turbo::Core::TDevice *device = context->GetDevice();
        Turbo::Core::TShaderLanguage shader_language = Turbo::Core::TShaderLanguage::GLSL;

        switch (language)
        {
        case TShader::TLanguage::GLSL: {
            shader_language = Turbo::Core::TShaderLanguage::GLSL;
        }
        break;
        case TShader::TLanguage::HLSL: {
            shader_language = Turbo::Core::TShaderLanguage::HLSL;
        }
        break;
        }
        this->vertexShader = new Turbo::Core::TVertexShader(device, shader_language, code);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TVertexShader::TVertexShader", "please ensure context is valid");
    }
}

Turbo::Render::TVertexShader::TVertexShader(TContext *context, size_t size, uint32_t *code)
{
    if (context != nullptr)
    {
        Turbo::Core::TDevice *device = context->GetDevice();
        this->vertexShader = new Turbo::Core::TVertexShader(device, size, code);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TVertexShader::TVertexShader", "please ensure context is valid");
    }
}

Turbo::Render::TVertexShader::~TVertexShader()
{
    this->vertexShader = nullptr;
}

Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> Turbo::Render::TVertexShader::GetVertexShader()
{
    return this->vertexShader;
}

Turbo::Render::TFragmentShader::TFragmentShader(Turbo::Render::TContext *context, TShader::TLanguage language, const std::string &code)
{
    if (context != nullptr)
    {
        Turbo::Core::TDevice *device = context->GetDevice();
        Turbo::Core::TShaderLanguage shader_language = Turbo::Core::TShaderLanguage::GLSL;

        switch (language)
        {
        case TShader::TLanguage::GLSL: {
            shader_language = Turbo::Core::TShaderLanguage::GLSL;
        }
        break;
        case TShader::TLanguage::HLSL: {
            shader_language = Turbo::Core::TShaderLanguage::HLSL;
        }
        break;
        }
        this->fragmentShader = new Turbo::Core::TFragmentShader(device, shader_language, code);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TFragmentShader::TFragmentShader", "please ensure context is valid");
    }
}

Turbo::Render::TFragmentShader::TFragmentShader(TContext *context, size_t size, uint32_t *code)
{
    if (context != nullptr)
    {
        Turbo::Core::TDevice *device = context->GetDevice();
        this->fragmentShader = new Turbo::Core::TFragmentShader(device, size, code);
    }
    else
    {
        throw Turbo::Core::TException(Turbo::Core::TResult::INVALID_PARAMETER, "Turbo::Render::TFragmentShader::TFragmentShader", "please ensure context is valid");
    }
}

Turbo::Render::TFragmentShader::~TFragmentShader()
{
    fragmentShader = nullptr;
}

Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> Turbo::Render::TFragmentShader::GetFragmentShader()
{
    return this->fragmentShader;
}