#include <GlobalWind.h>
#include <iostream>

int main()
{
    GlobalWind gw("../../asset/global_wind.bin");
    auto width = gw.Width();
    auto height = gw.Height();

    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;

    for (size_t row = 0; row < height; row++)
    {
        for (size_t column = 0; column < width; column++)
        {
            auto value = gw.Get(row, column);
            std::cout << "(";
            std::cout << value.lat << ", ";
            std::cout << value.lon << ", ";
            std::cout << value.u << ", ";
            std::cout << value.v;
            std::cout << ") ";
            std::cout << std::endl;
        }
    }
}