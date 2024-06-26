#include <iostream>
#include <ply.h>

int main()
{
    std::cout << "Hello World" << std::endl;

    int elements_count = 0;
    char **elements;
    int file_type;
    float version;

    PlyFile *ply = ply_open_for_reading(const_cast<char *>(std::string("../../asset/models/points.ply").c_str()), &elements_count, &elements, &file_type, &version);
    if (ply != nullptr)
    {
        std::cout << "ply != nullptr" << std::endl;

        ply_close(ply);
    }
    else
    {
        std::cout << "ply == nullptr" << std::endl;
    }
    return 0;
}