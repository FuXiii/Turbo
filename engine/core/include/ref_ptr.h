#pragma once
#ifndef TURBO_CORE_REF_PTR_H
#define TURBO_CORE_REF_PTR_H
#include <assert.h>
#include <type_traits>

namespace Turbo
{
namespace Core
{
class TReferenced;

// template <typename T, typename std::enable_if<std::is_base_of<Turbo::Core::TReferenced, T>::value, bool>::type = true>
template <typename T>
class ref_ptr
{
  private:
    template <class Inherit>
    friend class ref_ptr;

  private:
    T *ptr = nullptr;

  public:
    ref_ptr()
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "ref_ptr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = nullptr;
    }

    ref_ptr(T *ptr)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "ref_ptr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    ref_ptr(const ref_ptr &other)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "ref_ptr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = other.ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    ref_ptr(ref_ptr &&other) noexcept
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "ref_ptr<T> must inherited from Turbo::Core::TReferenced");
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    template <typename Inherit>
    ref_ptr(const ref_ptr<Inherit> &other)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "ref_ptr<Inherit> must inherited from T");
        this->ptr = other.ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    ~ref_ptr()
    {
        if (this->ptr != nullptr)
        {
            this->ptr->UnReference();
        }
        this->ptr = nullptr;
    }

  public:
    ref_ptr &operator=(const ref_ptr &rp)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "ref_ptr<T> must inherited from Turbo::Core::TReferenced");

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
    ref_ptr &operator=(const ref_ptr<Inherit> &rp)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "ref_ptr<Inherit> must inherited from T");

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
    ref_ptr &operator=(ref_ptr<Inherit> &&rp)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "ref_ptr<Inherit> must inherited from T");

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

    ref_ptr &operator=(T *ptr)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "ref_ptr<T> must inherited from Turbo::Core::TReferenced");

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

    // bool operator==(const ref_ptr &rp) const;
    // bool operator==(const T *ptr) const;
    // friend bool operator==(const T *ptr, const ref_ptr &rp);

    // bool operator!=(const ref_ptr &rp) const;
    // bool operator!=(const T *ptr) const;
    // friend bool operator!=(const T *ptr, const ref_ptr &rp);
    // bool operator<(const ref_ptr &rp) const;

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
    void Swap(ref_ptr &rp)
    {
        T *temp = this->ptr;
        this->ptr = rp.ptr;
        rp.ptr = temp;
    }
};
} // namespace Core
} // namespace Turbo
#endif