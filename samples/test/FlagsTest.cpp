enum class TestFlagBits
{
    _1 = 0b1,
    _10 = 0b10,
    _100 = 0b100,
    _1000 = 0b1000,
    _10000 = 0b10000,
    _100000 = 0b100000,
    _1000000 = 0b1000000,
};

#include <TFlags.h>

//#define Test_Flags_(TTT) \
//Turbo::Core::TFlags<TestFlagBits> operator|(TestFlagBits f1, const Turbo::Core::TFlags<TestFlagBits> &f2) noexcept \
//{return Turbo::Core::TFlags<TestFlagBits>(f1) | f2;}

int main()
{
    Turbo::Core::TFlags<TestFlagBits> flags = TestFlagBits::_10000;
    flags |= TestFlagBits::_10000;
    flags |= TestFlagBits::_1000;
    flags |= TestFlagBits::_100;

    std::cout << flags << std::endl;

    return 0;
}