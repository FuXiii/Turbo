#pragma once
#ifndef TURBO_CORE_TREFERENCED_H
#define TURBO_CORE_TREFERENCED_H
#include <cstdint>
#include <type_traits>

namespace Turbo
{
namespace Core
{
class TReferenced
{
  private:
    mutable uint32_t referenceCount = 0;

  private:
    void Release() const; // NOTE: It will force delete the memory it occupied. If you really know what you are doing now, otherwise never call it yourself!

  public:
    TReferenced();

    uint32_t Reference() const;
    uint32_t UnReference() const;
    uint32_t UnReferenceWithoutDelete() const;

    uint32_t GetReferenceCount() const;

    virtual bool Valid() const;

    template <typename... T>
    struct TCheckAllBaseOfReferenced;

    template <typename T>
    struct TCheckAllBaseOfReferenced<T>
    {
      public:
        static constexpr bool value = std::is_base_of<Turbo::Core::TReferenced, T>::value;
    };

    template <typename First, typename... Rest>
    struct TCheckAllBaseOfReferenced<First, Rest...>
    {
      public:
        static constexpr bool value = std::is_base_of<Turbo::Core::TReferenced, First>::value && TCheckAllBaseOfReferenced<Rest...>::value;
    };

    template <typename... T, std::enable_if_t<TCheckAllBaseOfReferenced<T...>::value, bool> = true>
    static bool Valid(const T *...ref)
    {
        const std::size_t count = sizeof...(ref);
        if (count != 0)
        {
            const TReferenced *refs[count] = {ref...};
            for (std::size_t index = 0; index < count; index++)
            {
                const TReferenced *ref_item = refs[index];
                if (ref_item == nullptr || !ref_item->Valid())
                {
                    return false;
                }
            }
        }
        return true;
    }
  protected:
    virtual ~TReferenced();
};
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TPHYSICALDEVICEINFO_H