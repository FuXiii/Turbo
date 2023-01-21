#include <iostream>
#include <list>

int password;

bool Post(int password)
{
    if (password == 987654321)
    {
        return true;
    }
    return false;
}

int main()
{
    int _password = 0;
    bool result = Post(_password);

    while (!result)
    {
        _password = _password + 1;

        // 得到了密码
        result = Post(_password);
    }

    std::cout << _password << std::endl;
}