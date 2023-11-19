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

class ReferencedTestContainer : public Turbo::Core::TReferenced
{
  private:
    Turbo::Core::TRefPtr<ReferencedTest> referencedTest;

  public:
    ReferencedTestContainer(const Turbo::Core::TRefPtr<ReferencedTest> &referencedTest)
    {
        std::cout << "ReferencedTestContainer()" << std::endl;
        if (referencedTest != nullptr)
        {
            this->referencedTest = referencedTest;
        }
    }

    void HelloWorld()
    {
        this->referencedTest->HelloWorld();
    }

    Turbo::Core::TRefPtr<ReferencedTest> Get()
    {
        return this->referencedTest;
    }

  protected:
    ~ReferencedTestContainer()
    {
        std::cout << "~ReferencedTestContainer()" << std::endl;
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

    Turbo::Core::TRefPtr<ReferencedTestContainer> referenced_test_container = new ReferencedTestContainer(rt);
    referenced_test_container->HelloWorld();

    Turbo::Core::TRefPtr<ReferencedTestContainer> referenced_test_container0 = new ReferencedTestContainer(new ReferencedTest());
    referenced_test_container0->HelloWorld();

    return 0;
}