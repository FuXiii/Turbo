#include <iostream>
#include <list>
#include <type_traits>

struct A_Test
{
};

template <class T>
class TestUniformBuffer
{
  public:
    struct Descriptor
    {
        uint32_t testValue;
    };

    void Create(const std::string &name, const Descriptor &descriptor, void *allocator)
    {
        std::cout << "Size:" << sizeof(T) << std::endl;
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
    test_buffer.Create(
        "test uniform bffer",
        {
            123
        },
        nullptr);
}