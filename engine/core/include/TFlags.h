#pragma once
#ifndef TURBO_CORE_TFLAGS_H
#define TURBO_CORE_TFLAGS_H

#include <array>
#include <type_traits>
#include <string>
#include <bitset>
#include <ostream>

#include <iostream>

namespace Turbo
{
namespace Core
{
template <typename T /*, typename = std::enable_if_t<std::is_integral<T>::value || std::is_enum<T>::value>*/>
class TFlags
{
  private:
    std::array<char, sizeof(T)> flags = {0};

  public:
    typedef T Type;

  public:
    TFlags() = default;

    TFlags(const T &flags)
    {
        std::memcpy(this->flags.data(), &flags, sizeof(T));
    }

    template <typename Y>
    TFlags(const Y &flags)
    {
        std::memcpy(this->flags.data(), &flags, std::min(this->flags.size(), sizeof(Y)));
    }

    template <typename Y>
    TFlags(const TFlags<Y> &flags)
    {
        Y y = flags;
        std::memcpy(this->flags.data(), &y, std::min(this->flags.size(), flags.Size()));
    }

    operator T() const
    {
        std::cout << "operator T() const" << std::endl;
        T t;
        memset(&t, 0, sizeof(T));
        memcpy(&t, this->flags.data(), sizeof(T));
        return t;
    }

    std::size_t Size() const
    {
        return this->flags.size();
    }

    bool Has(const TFlags &flags) const
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            if ((this->flags[i] & flags.flags[i]) != flags.flags[i])
            {
                return false;
            }
        }
        return true;
    }

    TFlags &operator&=(const TFlags &other)
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            this->flags[i] &= other.flags[i];
        }

        return *this;
    }

    TFlags operator&(const TFlags &other) const
    {
        TFlags _flags = *this;
        _flags &= other;
        return _flags;
    }

    TFlags &operator|=(const TFlags &other)
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            this->flags[i] |= other.flags[i];
        }

        return *this;
    }

    // template <typename Y>
    // TFlags &operator|=(const TFlags<Y> &other)
    // {
    //     auto size = std::min(this->flags.size(), other.Size());
    //     for (std::size_t i = 0; i < size; i++)
    //     {
    //         this->flags[i] |= other.flags[i];
    //     }

    //     return *this;
    // }

    TFlags operator|(const TFlags &other) const
    {
        TFlags _flags = *this;
        _flags |= other;
        return _flags;
    }

    TFlags &operator^=(const TFlags &other)
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            this->flags[i] ^= other.flags[i];
        }

        return *this;
    }

    TFlags operator^(const TFlags &other) const
    {
        TFlags _flags = *this;
        _flags ^= other;
        return _flags;
    }

    TFlags operator~() const
    {
        TFlags _flags = *this;
        for (std::size_t i = 0; i < _flags.flags.size(); i++)
        {
            _flags.flags[i] = ~_flags.flags[i];
        }
        return _flags;
    }

    bool operator!() const
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            if (!this->flags[i])
            {
                return false;
            }
        }
        return true;
    }

    template <typename Y>
    bool operator==(const TFlags<Y> &other) const
    {
        auto size = std::min(this->flags.size(), other.Size());
        for (std::size_t i = 0; i < size; i++)
        {
            if (this->flags[i] != other.flags[i])
            {
                return false;
            }
        }
        return true;
    }

    template <typename Y>
    bool operator!=(const TFlags<Y> &other) const
    {
        return !(*this == other);
    }

    std::string ToString() const
    {
        std::string str;
        for (auto riter = this->flags.rbegin(); riter != this->flags.rend(); riter++)
        {
            str += std::bitset<8 * sizeof(char)>(*riter).to_string();
        }

        if (!str.empty())
        {
            std::size_t pos = 0;
            for (std::size_t i = 0; i < str.size(); i++)
            {
                if (str[i] != '0')
                {
                    break;
                }
                else
                {
                    pos = pos + 1;
                }
            }

            if (pos != 0)
            {
                str = str.substr(pos);
            }
        }

        return str;
    }

    uint32_t ToVkFlags() const
    {
        uint32_t vk_flags = 0;
        std::memcpy(&vk_flags, this->flags.data(), std::min(this->flags.size(), sizeof(uint32_t)));
        return vk_flags;
    }
};
} // namespace Core
} // namespace Turbo

// template <typename T>
// Turbo::Core::TFlags<T> operator|(const T &left, const Turbo::Core::TFlags<T> &right)
//{
//     Turbo::Core::TFlags<T> flags;
//     flags |= left;
//     flags |= right;
//     return flags;
// }

// FIXME: Turbo::Core::TFlags<T> operator| 操作符无限制会有问题。
// FIXME: 这样会导致所有的 xxx|xxx 运算最终全部转成 TFlags<T> ，这在某些枚举声明中会导致不支持的语法。
// FIXME: 一种有效的使用方式就是使用宏声明，主动声明确切的枚举类型支持 operator| 操作符。

// template <typename T>
// Turbo::Core::TFlags<T> operator|(const T &left, const T &right)
//{
//      Turbo::Core::TFlags<T> flags;
//      flags |= left;
//      flags |= right;
//      return flags;
// }
//
// template <typename T>
// Turbo::Core::TFlags<T> operator|(const T &left, const Turbo::Core::TFlags<T> &right)
//{
//   Turbo::Core::TFlags<T> flags;
//   flags |= left;
//   flags |= right;
//   return flags;
//}

//#define TURBO_DECLARE_EXPLICIT_FLAGS_TYPE_OR_OPERATOR(TFlagsType, TBitsType)\
//inline Turbo::Core::TFlags<TBitsType> operator|(const TBitsType &left, const TBitsType &right)\
//{\
//    Turbo::Core::TFlags<TBitsType> flags;\
//    flags |= left;\
//    flags |= right;\
//    return flags;\
//}\
//\
//inline Turbo::Core::TFlags<TBitsType> operator|(const TBitsType &left, const Turbo::Core::TFlags<TBitsType> &right)\
//{\
//    Turbo::Core::TFlags<TBitsType> flags;\
//    flags |= left;\
//    flags |= right;\
//    return flags;\
//}
//\
//inline Turbo::Core::TFlags<TBitsType> operator|(const Turbo::Core::TFlags<TBitsType> &left,const TBitsType & right)\
//{\
//    Turbo::Core::TFlags<TBitsType> flags;\
//    flags |= left;\
//    flags |= right;\
//    return flags;\
//}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Turbo::Core::TFlags<T> &flags)
{
    return os << flags.ToString();
}
#endif