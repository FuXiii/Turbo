#pragma once
#ifndef TURBO_CORE_TREFPTR_H
#define TURBO_CORE_TREFPTR_H
#include <assert.h>
#include <type_traits>

namespace Turbo
{
namespace Core
{
class TReferenced;

// template <typename T, typename std::enable_if<std::is_base_of<Turbo::Core::TReferenced, T>::value, bool>::type = true>
template <typename T>
class TRefPtr
{
  private:
    template <class Inherit>
    friend class TRefPtr;

  private:
    T *ptr = nullptr;

  public:
    TRefPtr()
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = nullptr;
    }

    TRefPtr(const TRefPtr &other)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = other.ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    TRefPtr(TRefPtr &&other) noexcept
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    TRefPtr(T *ptr)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    template <typename Inherit>
    TRefPtr(const TRefPtr<Inherit> &other)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> must inherited from T");
        this->ptr = other.ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    template <typename Inherit>
    TRefPtr(const TRefPtr<Inherit> &&other)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> must inherited from T");
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    template <typename Inherit>
    TRefPtr(Inherit *ptr)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> must inherited from T");
        this->ptr = ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    ~TRefPtr()
    {
        if (this->ptr != nullptr)
        {
            this->ptr->UnReference();
        }
        this->ptr = nullptr;
    }

  public:
    TRefPtr &operator=(const TRefPtr &rp)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");

        if (this->ptr != rp.ptr)
        {
            if (this->ptr != nullptr)
            {
                this->ptr->UnReference();
            }
            this->ptr = rp.ptr;
            if (this->ptr != nullptr)
            {
                this->ptr->Reference();
            }
        }

        return *this;
    }

    template <typename Inherit>
    TRefPtr &operator=(const TRefPtr<Inherit> &rp)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> must inherited from T");

        if (this->ptr != rp.ptr)
        {
            if (this->ptr != nullptr)
            {
                this->ptr->UnReference();
            }
            this->ptr = rp.ptr;
            if (this->ptr != nullptr)
            {
                this->ptr->Reference();
            }

            
        }

        return *this;
    }

    template <typename Inherit>
    TRefPtr &operator=(TRefPtr<Inherit> &&rp)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> must inherited from T");

        if (this->ptr != rp.ptr)
        {
            if (this->ptr != nullptr)
            {
                this->ptr->UnReference();
            }
            this->ptr = rp.ptr;
            rp.ptr = nullptr;
        }

        return *this;
    }

    TRefPtr &operator=(T *ptr)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");

        if (this->ptr != ptr)
        {
            if (this->ptr != nullptr)
            {
                this->ptr->UnReference();
            }
            this->ptr = ptr;
            if (this->ptr != nullptr)
            {
                this->ptr->Reference();
            }
        }

        return *this;
    }

    template <class Inherit>
    bool operator<(const TRefPtr<Inherit> &rp) const
    {
        return (this->ptr < rp.ptr);
    }

    template <class Inherit>
    bool operator==(const TRefPtr<Inherit> &rp) const
    {
        return (this->ptr == rp.ptr);
    }

    template <class Inherit>
    bool operator!=(const TRefPtr<Inherit> &rp) const
    {
        return (this->ptr != rp.ptr);
    }

    template <class Inherit>
    bool operator<(const Inherit *ptr) const
    {
        return (this->ptr < ptr);
    }

    template <class Inherit>
    bool operator==(const Inherit *ptr) const
    {
        return (this->ptr == ptr);
    }

    template <class Inherit>
    bool operator!=(const Inherit *ptr) const
    {
        return (this->ptr != ptr);
    }

    operator T *() const
    {
        return this->ptr;
    }

    T &operator*() const
    {
        return *this->ptr;
    }

    T *operator->() const
    {
        return this->ptr;
    }

    T *Get() const
    {
        return this->ptr;
    }

    bool operator!() const
    {
        return this->ptr == nullptr;
    }

    bool Valid() const
    {
        return this->ptr != nullptr;
    }

    // T *Release();
    void Swap(TRefPtr &rp)
    {
        T *temp = this->ptr;
        this->ptr = rp.ptr;
        rp.ptr = temp;
    }

    explicit operator bool() const
    {
        return this->Valid();
    }
};
} // namespace Core
} // namespace Turbo
#endif