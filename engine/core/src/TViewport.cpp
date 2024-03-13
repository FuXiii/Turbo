#include "TViewport.h"

Turbo::Core::TViewport::TViewport(float x, float y, float width, float height, float minDepth, float maxDepth) : Turbo::Core::TInfo()
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->minDepth = minDepth;
    this->maxDepth = maxDepth;
}

Turbo::Core::TViewport::~TViewport()
{
}

float Turbo::Core::TViewport::GetX() const
{
    return this->x;
}

float Turbo::Core::TViewport::GetY() const
{
    return this->y;
}

float Turbo::Core::TViewport::GetWidth() const
{
    return this->width;
}

float Turbo::Core::TViewport::GetHeight() const
{
    return this->height;
}

float Turbo::Core::TViewport::GetMinDepth() const
{
    return this->minDepth;
}

float Turbo::Core::TViewport::GetMaxDepth() const
{
    return this->maxDepth;
}

std::string Turbo::Core::TViewport::ToString() const
{
    return std::string();
}
