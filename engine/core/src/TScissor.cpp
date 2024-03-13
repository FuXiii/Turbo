#include "TScissor.h"

Turbo::Core::TScissor::TScissor(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height) : Turbo::Core::TInfo()
{
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->width = width;
    this->height = height;
}

Turbo::Core::TScissor::~TScissor()
{
}

std::string Turbo::Core::TScissor::ToString() const
{
    return std::string();
}

int32_t Turbo::Core::TScissor::GetOffsetX() const
{
    return this->offsetX;
}

int32_t Turbo::Core::TScissor::GetOffsetY() const
{
    return this->offsetY;
}

uint32_t Turbo::Core::TScissor::GetWidth() const
{
    return this->width;
}

uint32_t Turbo::Core::TScissor::GetHeight() const
{
    return this->height;
}