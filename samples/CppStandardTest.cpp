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

int main()
{
    //std::cout << "Hello World" << std::endl;
//
    //bool is_class = std::is_class_v<A_Test>;
    //std::cout << is_class << ":A_Test" << std::endl;
//
    //struct B_Test
    //{
    //    float test;
    //    uint16_t uint16_t_test;
    //    double double_test;
    //};
//
    //TestUniformBuffer<B_Test> test_buffer;
    //test_buffer.Create("test uniform bffer", {123}, nullptr);
//
    //TestContext tc;
    //tc.BindingTestUniformBuffer(test_buffer);
}