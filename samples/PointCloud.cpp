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

        for (size_t elem_index = 0; elem_index < elements_count; elem_index++)
        {
            auto elem_name = elements[elem_index];
            if (equal_strings("vertex", elem_name))
            {
                int num_elems = 0;
                int num_props = 0;

                auto plist = ply_get_element_description(ply, elem_name, &num_elems, &num_props);
                std::cout << "elem_name:" << elem_name << std::endl;

                struct Vectex
                {
                    float x;
                    float y;
                    float z;
                    unsigned char r;
                    unsigned char g;
                    unsigned char b;
                    unsigned char a;
                };

                PlyProperty vert_props[] = {
                    {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vectex, x), 0, 0, 0, 0},
                    {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vectex, y), 0, 0, 0, 0},
                    {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vectex, z), 0, 0, 0, 0},
                    {"red", PLY_UCHAR, PLY_UCHAR, offsetof(Vectex, r), 0, 0, 0, 0},
                    {"green", PLY_UCHAR, PLY_UCHAR, offsetof(Vectex, g), 0, 0, 0, 0},
                    {"blue", PLY_UCHAR, PLY_UCHAR, offsetof(Vectex, b), 0, 0, 0, 0},
                    {"alpha", PLY_UCHAR, PLY_UCHAR, offsetof(Vectex, a), 0, 0, 0, 0},
                };

                ply_get_property(ply, elem_name, &vert_props[0]);
                ply_get_property(ply, elem_name, &vert_props[1]);
                ply_get_property(ply, elem_name, &vert_props[2]);
                ply_get_property(ply, elem_name, &vert_props[3]);
                ply_get_property(ply, elem_name, &vert_props[4]);
                ply_get_property(ply, elem_name, &vert_props[5]);
                ply_get_property(ply, elem_name, &vert_props[6]);

                for (size_t i = 0; i < num_elems; i++)
                {
                    Vectex vertex;
                    ply_get_element(ply, &vertex);

                    //Sif (vertex.b != 0)
                    {
                        std::cout << "i:" << i << std::endl;
                        std::cout << "\tx: " << vertex.x << std::endl;
                        std::cout << "\ty: " << vertex.y << std::endl;
                        std::cout << "\tz: " << vertex.z << std::endl;
                        std::cout << "\tr: " << (int)vertex.r << std::endl;
                        std::cout << "\tg: " << (int)vertex.g << std::endl;
                        std::cout << "\tb: " << (int)vertex.b << std::endl;
                        std::cout << "\ta: " << (int)vertex.a << std::endl;

                        std::cout << std::endl;
                    }
                }
            }
        }

        ply_close(ply);
    }
    else
    {
        std::cout << "ply == nullptr" << std::endl;
    }
    return 0;
}