#include "TReferenced.h"
#include "TRefPtr.h"
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

    rt0 = rt;

    return 0;
}