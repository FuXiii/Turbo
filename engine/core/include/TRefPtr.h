#pragma once
#ifndef TURBO_CORE_TREFPTR_H
#define TURBO_CORE_TREFPTR_H
#include <assert.h>
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <initializer_list>
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
    template <typename Inherit>
    friend class TRefPtr;

  private:
    T *ptr = nullptr;

  public:
    TRefPtr()
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> which T must inherited from Turbo::Core::TReferenced");
        this->ptr = nullptr;
    }

    TRefPtr(const TRefPtr &other)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> which T must inherited from Turbo::Core::TReferenced");
        this->ptr = other.ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    TRefPtr(TRefPtr &&other) noexcept
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> which T must inherited from Turbo::Core::TReferenced");
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    TRefPtr(T *ptr)
    {
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> which T must inherited from Turbo::Core::TReferenced or nullptr");
        this->ptr = ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    template <typename Inherit>
    TRefPtr(const TRefPtr<Inherit> &other)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> which Inherit must inherited from T");
        this->ptr = other.ptr;
        if (this->ptr != nullptr)
        {
            this->ptr->Reference();
        }
    }

    template <typename Inherit>
    TRefPtr(const TRefPtr<Inherit> &&other)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> which Inherit must inherited from T");
        this->ptr = other.ptr;
        other.ptr = nullptr;
    }

    template <typename Inherit>
    TRefPtr(Inherit *ptr)
    {
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> which Inherit must inherited from T or nullptr");
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
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> which T must inherited from Turbo::Core::TReferenced");

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
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> which Inherit must inherited from T");

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
        static_assert(std::is_base_of<T, Inherit>::value, "TRefPtr<Inherit> which Inherit must inherited from T");

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
        static_assert(std::is_base_of<Turbo::Core::TReferenced, T>::value, "TRefPtr<T> which T must inherited from Turbo::Core::TReferenced or nullptr");

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

    template <typename Inherit>
    bool operator<(const TRefPtr<Inherit> &rp) const
    {
        return (this->ptr < rp.ptr);
    }

    // bool operator==(const TRefPtr<T> &rp) const
    //{
    //     return (this->ptr == rp.ptr);
    // }

    // template <typename Inherit>
    // bool operator==(const TRefPtr<Inherit> &rp) const
    //{
    //     return (this->ptr == rp.ptr);
    // }

    template <typename Inherit>
    bool operator!=(const TRefPtr<Inherit> &rp) const
    {
        return (this->ptr != rp.ptr);
    }

    template <typename Inherit>
    bool operator<(Inherit *ptr) const
    {
        return (this->ptr < ptr);
    }

    // bool operator==(T *ptr) const
    //{
    //     return (this->ptr == ptr);
    // }

    // bool operator==(const T *ptr) const
    //{
    //     return (this->ptr == ptr);
    // }

    template <typename Inherit>
    bool operator==(Inherit *ptr) const
    {
        return (this->ptr == ptr);
    }

    template <typename Inherit>
    bool operator!=(Inherit *ptr) const
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
        return (this->ptr != nullptr && this->ptr->Valid());
    }

    uint32_t ReferenceCount() const
    {
        if (this->Valid())
        {
            return this->ptr->GetReferenceCount();
        }

        return 0;
    }

    void Swap(TRefPtr &rp)
    {
        T *temp = this->ptr;
        this->ptr = rp.ptr;
        rp.ptr = temp;
    }

    T *Unbind()
    {
        T *temp_ptr = this->ptr;
        if (temp_ptr != nullptr)
        {
            temp_ptr->UnReferenceWithoutDelete();
        }
        this->ptr = nullptr;
        return temp_ptr;
    }

    // NOTE: It will force delete the memory it occupied. If you really know what are you doing, otherwise never call it yourself!
    // FIXME: It best to be private or remove
    // void Release()
    //{
    //    if (this->ptr != nullptr)
    //    {
    //        this->ptr->Release();
    //        this->ptr = nullptr;
    //    }
    //}

    explicit operator bool() const
    {
        return this->Valid();
    }
};

template <typename T, typename S>
inline TRefPtr<T> StaticCast(const TRefPtr<S> &refPtr)
{
    return static_cast<T *>(refPtr.Get());
}

template <typename T, typename S>
inline TRefPtr<T> DynamicCast(const TRefPtr<S> &refPtr)
{
    return dynamic_cast<T *>(refPtr.Get());
}

template <typename T, typename S>
inline TRefPtr<T> ConstCast(const TRefPtr<S> &refPtr)
{
    return const_cast<T *>(refPtr.Get());
}

template <typename T>
std::vector<T *> RefsToPtrs(const std::vector<Turbo::Core::TRefPtr<T>> &refs)
{
    std::vector<T *> ptrs;
    size_t size = refs.size();
    if (size != 0)
    {
        ptrs.resize(size, nullptr);
        for (size_t index = 0; index < size; index++)
        {
            ptrs[index] = refs[index];
        }
    }
    return ptrs;
}

template <typename T>
std::vector<Turbo::Core::TRefPtr<T>> PtrsToRefs(const std::vector<T *> &ptrs)
{
    std::vector<Turbo::Core::TRefPtr<T>> refs;
    size_t size = ptrs.size();
    if (size != 0)
    {
        refs.resize(size, nullptr);
        for (size_t index = 0; index < size; index++)
        {
            refs[index] = ptrs[index];
        }
    }
    return refs;
}

template <typename T>
std::vector<T *> RefsToPtrs(const std::initializer_list<Turbo::Core::TRefPtr<T>> &refs)
{
    std::vector<T *> ptrs;
    size_t size = refs.size();
    if (size != 0)
    {
        ptrs.resize(size, nullptr);

        size_t index = 0;
        for (auto &item : ptrs)
        {
            ptrs[index] = item;
            ++index;
        }
    }
    return ptrs;
}

template <typename T>
std::vector<Turbo::Core::TRefPtr<T>> PtrsToRefs(const std::initializer_list<T *> &ptrs)
{
    std::vector<Turbo::Core::TRefPtr<T>> refs;
    size_t size = ptrs.size();
    if (size != 0)
    {
        refs.resize(size, nullptr);

        size_t index = 0;
        for (auto &item : ptrs)
        {
            refs[index] = item;
            ++index;
        }
    }
    return refs;
}

} // namespace Core
} // namespace Turbo
#endif