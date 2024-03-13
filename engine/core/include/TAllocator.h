#pragma once
#ifndef TURBO_CORE_TALLOCATOR_H
#define TURBO_CORE_TALLOCATOR_H
#include "TObject.h"
#include <type_traits>

namespace Turbo
{
namespace Core
{
class TAllocator : public TObject
{
  public:
    explicit TAllocator();

  protected:
    virtual ~TAllocator();

  public:
    static void *VKAPI_PTR Allocate(size_t size, size_t alignment);
    static void *VKAPI_PTR Reallocate(void *pOriginal, size_t size, size_t alignment);
    static void VKAPI_PTR Free(void *pMemory);

    template <typename T>
    static constexpr T AlignUp(T meta, size_t alignment) noexcept;

    virtual std::string ToString() const override;
};

template <typename T>
constexpr T Turbo::Core::TAllocator::AlignUp(T meta, size_t alignment) noexcept
{
    static_assert(std::is_integral<T>::value, "AlignUp T type: Integral required.");
    return T((meta + (T(alignment) - 1)) & ~T(alignment - 1));
}

} // namespace Core
} // namespace Turbo
#endif // !TURBO_CORE_TALLOCATOR_H