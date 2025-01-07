#pragma once
#include <string>

class GlobalWind
{
  public:
    class Value
    {
      public:
        double lat = 0;
        double lon = 0;
        double u = 0;
        double v = 0;
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

    void Load(size_t& size, void *data);
};