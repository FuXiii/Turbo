#pragma once
#ifndef TURBO_CORE_REF_PTR_H
#define TURBO_CORE_REF_PTR_H
#include <type_traits>

namespace Turbo
{
namespace Core
{
class TReferenced;

template <typename T, typename std::enable_if<std::is_base_of<Turbo::Core::TReferenced, T>::value, bool>::type = true>
class ref_ptr
{
  private:
    T *ptr = nullptr;

  public:
    ref_ptr()
    {
        this->ptr = nullptr;
    }

    ref_ptr(T *ptr)
    {
        this->ptr = ptr;
        if (this->ptr)
        {
            this->ptr->Reference();
        }
    }

    ref_ptr(const ref_ptr &other)
    {
        this->ptr = other.ptr;
        if (this->ptr)
        {
            this->ptr->Reference();
        }
    }

    ref_ptr(ref_ptr &&other) noexcept
    {
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    template <typename Other>
    ref_ptr(const ref_ptr<Other> &other); // NOTE: Is this the same as [ref_ptr(const ref_ptr &other)] above

    ~ref_ptr();

  public:
    ref_ptr &operator=(const ref_ptr &rp);

    template <typename Other>
    ref_ptr &operator=(const ref_ptr<Other> &rp);

    template <typename Other>
    ref_ptr &operator=(ref_ptr<Other> &&rp);

    ref_ptr &operator=(T *ptr);

    bool operator==(const ref_ptr &rp) const;
    bool operator==(const T *ptr) const;
    friend bool operator==(const T *ptr, const ref_ptr &rp);

    bool operator!=(const ref_ptr &rp) const;
    bool operator!=(const T *ptr) const;
    friend bool operator!=(const T *ptr, const ref_ptr &rp);
    bool operator<(const ref_ptr &rp) const;

    operator T *() const;
    T &operator*() const;
    T *operator->() const;
    T *Get() const;
    bool operator!() const;
    bool Valid() const;
    T *Release();
    void Swap(ref_ptr &rp);
};
} // namespace Core
} // namespace Turbo

#endif