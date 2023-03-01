#pragma once
#ifndef TURBO_RENDER_TBUFFER_H
#define TURBO_RENDER_TBUFFER_H
#include "TDomain.h"
#include "TFormat.h"
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

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
        TBufferUsages usages;
        uint64_t size;
        TDomain domain;
    };

    friend class TContext;

  private:
    void *allocator = nullptr;
    Turbo::Core::TBuffer *buffer = nullptr;
    Descriptor descriptor;

  public:
    TBuffer() = default;
    ~TBuffer() = default;

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);
    void Destroy(void *allocator);

    TBufferUsages GetUsages();
    uint64_t GetSize();
    TDomain GetDomain();

    void Copy(void *src, uint64_t size);
    void Copy(TBuffer *src, uint64_t srcOffset, uint64_t size);

    bool IsValid() const;
};

using TAttributeID = uint32_t;

class TVertexBuffer : public Turbo::Render::TBuffer
{
  public:
    typedef enum TRate
    {
        VERTEX = 0,
        INSTANCE = 1
    } TRate;

    class TAttribute
    {
      private:
        Turbo::Render::TFormat format = Turbo::Render::TFormat::UNDEFINED;
        uint32_t offset = std::numeric_limits<uint32_t>::max();

      public:
        TAttribute() = default;
        TAttribute(Turbo::Render::TFormat format, uint32_t offset);
        ~TAttribute() = default;

        Turbo::Render::TFormat GetFormat();
        uint32_t GetOffset();

        bool IsValid() const;
    };

    std::vector<TVertexBuffer::TAttribute> attributes;

    struct Descriptor
    {
        // TBufferUsages usages; //manage by Turbo
        uint64_t size;
        TDomain domain;
        uint32_t stride;
        TRate rate = TRate::VERTEX;
    };

  private:
    uint32_t stride = 0;
    TRate rate = TRate::VERTEX;

  public:
    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);

    TAttributeID AddAttribute(Turbo::Render::TFormat format, uint32_t offset);
    TAttribute GetAttribute(TAttributeID id) const;
    const std::vector<TVertexBuffer::TAttribute> &GetAttributes();

    uint32_t GetStride() const;
    TRate GetRate() const;
};

class TIndexBuffer : public Turbo::Render::TBuffer
{
  public:
    typedef enum TIndexType
    {
        UINT16 = 0,
        UINT32 = 1
    } TIndexType;

    struct Descriptor
    {
        // TBufferUsages usages; //manage by Turbo
        uint64_t size;
        TDomain domain;
    };

  private:
    TIndexType indexType = TIndexType::UINT32;

  public:
    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);

    void Copy(void *src, uint64_t size) = delete;
    void Copy(TBuffer *src, uint64_t srcOffset, uint64_t size) = delete;

    void Copy(const std::vector<uint16_t> &indexs);
    void Copy(const std::vector<uint32_t> &indexs);

    TIndexType GetIndexType() const;
};

template <typename T, std::enable_if_t<std::is_class<T>::value, bool> = true>
class TUniformBuffer : public Turbo::Render::TBuffer
{
  public:
    struct Descriptor
    {
        // TBufferUsages usages; //manage by Turbo
        // uint64_t size;//manage by T
        TDomain domain;
    };

  private:
  public:
    void Create(const std::string &name, const Descriptor &descriptor, void *allocator);

    void Copy(void *src, uint64_t size) = delete;
    void Copy(TBuffer *src, uint64_t srcOffset, uint64_t size) = delete;

    void Copy(const T &uniform);
};
} // namespace Render
} // namespace Turbo
#endif // !TURBO_RENDER_TBUFFER_H