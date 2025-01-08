#pragma once
#include <string>

class GlobalWind
{
  public:
    class Value
    {
      public:
        float lat = 0;
        float lon = 0;
        float u = 0;
        float v = 0;
    };

  private:
    std::ifstream *in = nullptr;
    size_t width = 0;
    size_t height = 0;

  public:
    GlobalWind(const std::string &url);
    ~GlobalWind();

    size_t Width();
    size_t Height();

    Value Get(size_t row, size_t column);

    size_t Load(void *ptr = nullptr);
};