#include <chrono>
#include <iostream>
#include <render/include/TContext.h>
#include <thread>

int main()
{
    Turbo::Render::TContext context;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    return 0;
}