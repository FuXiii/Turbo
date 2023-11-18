#include "TRefPtr.h"
#include "TReferenced.h"
#include <iostream>

class ReferencedTest : public Turbo::Core::TReferenced
{
  public:
    ReferencedTest()
    {
        std::cout << "ReferencedTest()" << std::endl;
    }

    void HelloWorld()
    {
        std::cout << "Hello World" << std::endl;
    }

  protected:
    ~ReferencedTest()
    {
        std::cout << "~ReferencedTest()" << std::endl;
    }
};

int main()
{
    std::cout << "Hello World" << std::endl;

    ReferencedTest *referenced_test = new ReferencedTest();
    std::cout << "Reference:" << referenced_test->Reference() << std::endl;
    std::cout << "Reference:" << referenced_test->Reference() << std::endl;
    std::cout << "Reference:" << referenced_test->Reference() << std::endl;
    std::cout << "UnReference:" << referenced_test->UnReference() << std::endl;
    std::cout << "UnReference:" << referenced_test->UnReference() << std::endl;
    std::cout << "UnReference:" << referenced_test->UnReference() << std::endl;

    Turbo::Core::TRefPtr<ReferencedTest> rt = new ReferencedTest();
    rt->HelloWorld();
    Turbo::Core::TRefPtr<ReferencedTest> rt0 = new ReferencedTest();
    rt0->HelloWorld();

    if (rt == rt0)
    {
        std::cout << "rt == rt0" << std::endl;
    }
    else
    {
        std::cout << "rt!= rt0" << std::endl;
    }

    rt0 = rt;

    if (rt == rt0)
    {
        std::cout << "rt == rt0" << std::endl;
    }
    else
    {
        std::cout << "rt!= rt0" << std::endl;
    }

    return 0;
}