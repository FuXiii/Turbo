#pragma once
#ifndef TURBO_CORE_TFLAGS_H
#define TURBO_CORE_TFLAGS_H

#include <array>
#include <type_traits>
#include <string>
#include <bitset>
#include <ostream>

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
    TFlags() = default;

    TFlags(const T &flags)
    {
        std::memcpy(this->flags.data(), &flags, sizeof(T));
    }

    operator T() const
    {
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

    TFlags &operator|=(const TFlags &other)
    {
        for (std::size_t i = 0; i < this->flags.size(); i++)
        {
            this->flags[i] |= other.flags[i];
        }

        return *this;
    }

    TFlags operator|(const TFlags &other) const
    {
        TFlags temp_flags = *this;
        temp_flags |= other;
        return temp_flags;
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
};
} // namespace Core
} // namespace Turbo

template <typename T>
Turbo::Core::TFlags<T> operator|(const T &left, const T &right)
{
    Turbo::Core::TFlags<T> flags;
    flags |= left;
    flags |= right;
    return flags;
}

template <typename T>
Turbo::Core::TFlags<T> operator|(const T &left, const Turbo::Core::TFlags<T> &right)
{
    Turbo::Core::TFlags<T> flags;
    flags |= left;
    flags |= right;
    return flags;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Turbo::Core::TFlags<T> &flags)
{
    return os << flags.ToString();
}
#endif