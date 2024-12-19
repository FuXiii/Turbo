#include <iostream>
#include <list>
#include <type_traits>

struct A_Test
{
};

template <typename T>
class TestUniformBuffer
{
  private:
    uint32_t testValue;

  public:
    struct Descriptor
    {
        uint32_t testValue;
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator)
    {
        this->testValue = descriptor.testValue;
        std::cout << "Size:" << sizeof(T) << std::endl;
    }

    uint32_t GetValue() const
    {
        return this->testValue;
    }
};

class TestContext
{
  public:
    template <typename T>
    void BindingTestUniformBuffer(const TestUniformBuffer<T> &tub)
    {
        std::cout << "Value:" << tub.GetValue() << std::endl;
    }
};

#include "TReferenced.h"
class TRefA : public Turbo::Core::TReferenced
{
  public:
    bool Valid() const override
    {
        return true;
    }
};
class TRefB : public Turbo::Core::TReferenced
{
  public:
    bool Valid() const override
    {
        return false;
    }
};
class TRefC : public TRefB
{
  public:
    bool Valid() const override
    {
        return true;
    }
};

// template <typename... T>
// struct check_all;
//
// template <typename T>
// struct check_all<T>
//{
//     // static constexpr bool value = std::is_same<T, int>::value;
//     static constexpr bool value = std::is_base_of<Turbo::Core::TReferenced, T>::value;
// };
//
// template <typename First, typename... Rest>
// struct check_all<First, Rest...>
//{
//     // static constexpr bool value = std::is_same<First, int>::value && check_all<Rest...>::value;
//     static constexpr bool value = std::is_base_of<Turbo::Core::TReferenced, First>::value && check_all<Rest...>::value;
// };
//
// template <typename... T, std::enable_if_t<check_all<T...>::value, bool> = true>
// void foo(T*... args)
//{
//     (std::cout << ... << args) << '\n';
// }
//
// int maian()
//{
//     TRefA *a = new TRefA;
//     TRefB *b = new TRefB;
//     TRefC *c = new TRefC;
//     foo(a, b, c); // 正确，所有参数都是int
//     //foo(1, 2, 3); // 正确，所有参数都是int
//     // foo(1, 2, "3"); // 错误，有一个参数不是int
//     return 0;
// }

int main()
{
    std::cout << "Hello World" << std::endl;

    bool is_class = std::is_class_v<A_Test>;
    std::cout << is_class << ":A_Test" << std::endl;

    struct B_Test
    {
        float test;
        uint16_t uint16_t_test;
        double double_test;
    };

    TestUniformBuffer<B_Test> test_buffer;
    test_buffer.Create("test uniform bffer", {123}, nullptr);

    TestContext tc;
    tc.BindingTestUniformBuffer(test_buffer);

    {
        TRefA *a = new TRefA;
        TRefB *b = new TRefB;
        TRefC *c = new TRefC;

        std::cout << "Valids(a, a, a) :" << Turbo::Core::TReferenced::Valid(a, a, a) << std::endl;
        std::cout << "Valids(a, b, c) :" << Turbo::Core::TReferenced::Valid(a, b, c) << std::endl;
        std::cout << "Valids(a, b, c, a, b, c) :" << Turbo::Core::TReferenced::Valid(a, b, c, a, b, c) << std::endl;
        std::cout << "Valids(a, c) :" << Turbo::Core::TReferenced::Valid(a, c) << std::endl;
        std::cout << "Valids(c) :" << Turbo::Core::TReferenced::Valid(c) << std::endl;
        // std::cout << "Valids() :" << Turbo::Core::TReferenced::Valids(nullptr) << std::endl;
    }
}