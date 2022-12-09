#pragma once
#ifndef TURBO_CORE_TPOOL_H
#define TURBO_CORE_TPOOL_H
#include "TObject.h"
#include <stdint.h>
#include <vector>

namespace Turbo
{
namespace Core
{
template <typename T> class TPool : public Turbo::Core::TObject
{
  private:
    uint32_t count;

    std::vector<T *> pool;

  public:
    TPool(uint32_t count);
    ~TPool();

  public:
    template <typename... Args> T *Allocate(Args &&...args);
    void Free(T *object);

    const std::vector<T *> &GetPool();

    virtual std::string ToString() override;
};

template <typename T> Turbo::Core::TPool<T>::TPool(uint32_t count) : Turbo::Core::TObject()
{
    this->count = count;
}

template <typename T> Turbo::Core::TPool<T>::~TPool()
{
}

template <typename T> template <typename... Args> T *Turbo::Core::TPool<T>::Allocate(Args &&...args)
{
    if (this->pool.size() < this->count)
    {
        T *result = new T(args...);
        this->pool.push_back(result);

        return result;
    }

    return nullptr;
}

template <typename T> void Turbo::Core::TPool<T>::Free(T *object)
{
    uint32_t index = 0;
    bool is_found = false;
    for (T *item : this->pool)
    {
        if (item == object)
        {
            is_found = true;
            break;
        }
        index = index + 1;
    }

    if (is_found)
    {
        delete this->pool[index];
        this->pool.erase(this->pool.begin() + index);
    }
}

template <typename T> const std::vector<T *> &Turbo::Core::TPool<T>::GetPool()
{
    return this->pool;
}

template <typename T> std::string Turbo::Core::TPool<T>::ToString()
{
    return std::string();
}
} // namespace Core
} // namespace Turbo

#endif // !TURBO_CORE_TPOOL_H