#pragma once
#ifndef TURBO_RENDER_TSHADER_H
#define TURBO_RENDER_TSHADER_H
#include <string>

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
};

class TComputeShader : public TShader
{
  private:
    Turbo::Core::TComputeShader *computeShader = nullptr;

  public:
    TComputeShader() = default;
    TComputeShader(TContext *context, TShader::TLanguage language, const std::string &code);
    TComputeShader(TContext *context, size_t size, uint32_t *code);
    ~TComputeShader();
};

class TVertexShader : public TShader
{
  private:
    Turbo::Core::TVertexShader *vertexShader = nullptr;

  public:
    TVertexShader() = default;
    TVertexShader(TContext *context, TShader::TLanguage language, const std::string &code);
    TVertexShader(TContext *context, size_t size, uint32_t *code);
    ~TVertexShader();
};

class TFragmentShader : public TShader
{
  private:
    Turbo::Core::TFragmentShader *fragmentShader = nullptr;

  public:
    TFragmentShader() = default;
    TFragmentShader(TContext *context, TShader::TLanguage language, const std::string &code);
    TFragmentShader(TContext *context, size_t size, uint32_t *code);
    ~TFragmentShader();
};
} // namespace Render
} // namespace Turbo

#endif // !TURBO_RENDER_TRENDERPASS_H