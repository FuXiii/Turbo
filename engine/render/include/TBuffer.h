#pragma once
#ifndef TURBO_RENDER_TBUFFER_H
#define TURBO_RENDER_TBUFFER_H
#include "TDomain.h"
#include "TFormat.h"
#include <cstdint>
#include <string>

namespace Turbo
{
namespace Core
{
class TBuffer;
} // namespace Core
} // namespace Turbo

namespace Turbo
{
namespace Render
{
typedef enum TBufferUsageBits
{
    BUFFER_TRANSFER_SRC = 0x00000001,
    BUFFER_TRANSFER_DST = 0x00000002,
    BUFFER_UNIFORM_TEXEL = 0x00000004,
    BUFFER_STORAGE_TEXEL = 0x00000008,
    BUFFER_UNIFORM_BUFFER = 0x00000010,
    BUFFER_STORAGE_BUFFER = 0x00000020,
    BUFFER_INDEX_BUFFER = 0x00000040,
    BUFFER_VERTEX_BUFFER = 0x00000080,
    BUFFER_INDIRECT_BUFFER = 0x00000100,
} TBufferUsageBits;
using TBufferUsages = uint32_t;

class TBuffer
{
  public:
    struct Descriptor
    {
        TBufferUsageBits usages;
        uint64_t size;
        TDomain domain;
    };

  private:
    Turbo::Core::TBuffer *buffer = nullptr;
    Descriptor descriptor;

  public:
    TBuffer() = default;
    ~TBuffer() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);

    TDomain GetDomain();
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TBUFFER_H