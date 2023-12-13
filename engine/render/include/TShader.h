#pragma once
#ifndef TURBO_RENDER_TSHADER_H
#define TURBO_RENDER_TSHADER_H
#include <string>

#include <core/include/TRefPtr.h>

namespace Turbo
{
namespace Core
{
class TComputeShader;
class TVertexShader;
class TFragmentShader;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Render
{

class TContext;

class TShader
{
  public:
    typedef enum TLanguage
    {
        GLSL = 0,
        HLSL = 1,
    } TLanguage;

  public:
    TShader() = default;
    ~TShader() = default;

  public:
    // disallow copy and assignment
    TShader(TShader const &) = delete;
    TShader(TShader &&) = delete;
    TShader &operator=(TShader const &) = delete;
    TShader &operator=(TShader &&) = delete;
};

class TComputeShader : public TShader
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TComputeShader> computeShader;

  public:
    TComputeShader(TContext *context, TShader::TLanguage language, const std::string &code);
    TComputeShader(TContext *context, size_t size, uint32_t *code);
    ~TComputeShader();

    Turbo::Core::TRefPtr<Turbo::Core::TComputeShader> GetComputeShader();
};

class TVertexShader : public TShader
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> vertexShader;

  public:
    TVertexShader(TContext *context, TShader::TLanguage language, const std::string &code);
    TVertexShader(TContext *context, size_t size, uint32_t *code);
    ~TVertexShader();

    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> GetVertexShader();
};

class TFragmentShader : public TShader
{
  private:
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> fragmentShader;

  public:
    TFragmentShader(TContext *context, TShader::TLanguage language, const std::string &code);
    TFragmentShader(TContext *context, size_t size, uint32_t *code);
    ~TFragmentShader();

    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> GetFragmentShader();
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H