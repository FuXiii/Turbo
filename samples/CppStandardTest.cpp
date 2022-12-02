#include <iostream>
#include <utility>

class AAA
{
  private:
    int value1 = 0;
    float value2 = 0;

  public:
    AAA()
    {
        std::cout << "AAA()" << std::endl;
    }

    ~AAA()
    {
    }

    AAA(int value1, float value2)
    {
        this->value1 = value1;
        this->value2 = value2;
        std::cout << "AAA(int,float)" << std::endl;
    }

    AAA(const AAA &obj)
    {
        if (this != &obj)
        {
            this->value1 = obj.value1;
            this->value2 = obj.value2;
        }
        std::cout << "AAA(const AAA &)" << std::endl;
    }

    AAA(AAA &&obj) noexcept
    {
        if (this != &obj)
        {
            this->value1 = obj.value1;
            this->value2 = obj.value2;

            obj.value1 = 0;
            obj.value2 = 0;
        }
        std::cout << "AAA(AAA &&)" << std::endl;
    }

    AAA &operator=(const AAA &obj)
    {
        if (this != &obj)
        {
            this->value1 = obj.value1;
            this->value2 = obj.value2;
        }
        std::cout << "AAA &operator=(const AAA &)" << std::endl;
        return *this;
    }

    AAA &operator=(AAA &&obj) noexcept
    {
        if (this != &obj)
        {
            std::swap(*this, obj);
        }
        std::cout << "AAA &operator=(AAA &&)" << std::endl;
        return *this;
    }

    int GetValue1()
    {
        return this->value1;
    }

    float GetValue2()
    {
        return this->value2;
    }
};

int main()
{
    AAA aaa;

    AAA aaa1(1, 2);

    AAA aaa2(aaa1);

    aaa = aaa1;
    std::cout << "value1:" << aaa.GetValue1() << " value2:" << aaa.GetValue2() << std::endl;

    AAA aaa3(4, 5);
    aaa = aaa3;

    std::cout << "value1:" << aaa.GetValue1() << " value2:" << aaa.GetValue2() << std::endl;

    AAA aaa4(std::forward<AAA>(AAA(7, 8)));

    std::cout << "value1:" << aaa4.GetValue1() << " value2:" << aaa4.GetValue2() << std::endl;
    std::cout << "end" << std::endl;

    std::cout << __cplusplus << std::endl;

    int *nullptr_point = nullptr;
    delete nullptr_point;

    return 0;
}