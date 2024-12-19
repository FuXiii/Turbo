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