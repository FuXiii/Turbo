#include "TReferenced.h"
#include <iostream>

class ReferencedTest : public Turbo::Core::TReferenced
{
  public:
    ReferencedTest()
    {
        std::cout << "ReferencedTest()" << std::endl;
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

    return 0;
}