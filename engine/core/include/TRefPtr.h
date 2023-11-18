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

    TRefPtr(T *ptr)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
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
            T *tmp_ptr = this->ptr;
            this->ptr = rp.ptr;
            if (this->ptr != nullptr)
            {
                this->ptr->Reference();
            }

            if (tmp_ptr != nullptr)
            {
                tmp_ptr->UnReference();
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
            T *tmp_ptr = this->ptr;
            this->ptr = rp.ptr;
            if (this->ptr != nullptr)
            {
                this->ptr->Reference();
            }

            if (tmp_ptr != nullptr)
            {
                tmp_ptr->UnReference();
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
            T *tmp_ptr = this->ptr;
            this->ptr = ptr;
            if (this->ptr != nullptr)
            {
                this->ptr->Reference();
            }

            if (tmp_ptr != nullptr)
            {
                tmp_ptr->UnReference();
            }
        }

        return *this;
    }

    // bool operator==(const TRefPtr &rp) const;
    // bool operator==(const T *ptr) const;
    // friend bool operator==(const T *ptr, const TRefPtr &rp);

    // bool operator!=(const TRefPtr &rp) const;
    // bool operator!=(const T *ptr) const;
    // friend bool operator!=(const T *ptr, const TRefPtr &rp);
    // bool operator<(const TRefPtr &rp) const;

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
};
} // namespace Core
} // namespace Turbo
#endif