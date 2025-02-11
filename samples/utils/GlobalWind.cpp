#include "GlobalWind.h"
#include <fstream>

GlobalWind::GlobalWind(const std::string &url)
{
    this->in = new std::ifstream(url, std::ios::binary | std::ios::in | std::ios::ate);
    if (this->in != nullptr && this->in->is_open())
    {
        size_t end = this->in->tellg();
        this->in->seekg(end - sizeof(size_t) * 2);
        this->in->read(reinterpret_cast<char *>(&(this->width)), sizeof(size_t));
        this->in->read(reinterpret_cast<char *>(&(this->height)), sizeof(size_t));
    }
    else
    {
        // TODO: throw exception
    }
}

GlobalWind::~GlobalWind()
{
    if (this->in != nullptr)
    {
        delete this->in;
        this->in = nullptr;
    }
}

size_t GlobalWind::Width()
{
    return this->width;
}

size_t GlobalWind::Height()
{
    return this->height;
}

GlobalWind::Value GlobalWind::Get(size_t row, size_t column)
{
    GlobalWind::Value value;

    auto GetOffset = [&](size_t row, size_t column) { return (row * width + column) * sizeof(float) * 4; };

    this->in->seekg(GetOffset(row, column));

    this->in->read(reinterpret_cast<char *>(&value.lat), sizeof(value.lat));
    this->in->read(reinterpret_cast<char *>(&value.lon), sizeof(value.lon));
    this->in->read(reinterpret_cast<char *>(&value.u), sizeof(value.u));
    this->in->read(reinterpret_cast<char *>(&value.v), sizeof(value.v));

    return value;
}

size_t GlobalWind::Load(void *ptr)
{
    this->in->seekg(-2 * sizeof(size_t), std::ios::end);
    size_t result = this->in->tellg();

    this->in->seekg(0);
    if (ptr != nullptr)
    {
        this->in->read(reinterpret_cast<char *>(ptr), result);
    }

    return result;
}