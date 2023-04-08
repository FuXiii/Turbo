#include <iostream>
#include <thread>

void Task()
{
    uint32_t index = 0;
    uint32_t count = 1000000;
    do
    {
        //std::cout << "id" << std::this_thread::get_id() << std::endl;
        index++;
    } while (index < count);
}

int main()
{
    std::cout << "Hello World" << std::endl;

    std::thread thread_0(Task);
    std::thread thread_1(Task);
    std::thread thread_2(Task);
    std::thread thread_3(Task);

    thread_0.join();
    thread_1.join();
    thread_2.join();
    thread_3.join();

    return 0;
}