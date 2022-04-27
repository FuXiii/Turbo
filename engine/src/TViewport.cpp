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

float Turbo::Core::TViewport::GetX()
{
    return this->x;
}

float Turbo::Core::TViewport::GetY()
{
    return this->y;
}

float Turbo::Core::TViewport::GetWidth()
{
    return this->width;
}

float Turbo::Core::TViewport::GetHeight()
{
    return this->height;
}

float Turbo::Core::TViewport::GetMinDepth()
{
    return this->minDepth;
}

float Turbo::Core::TViewport::GetMaxDepth()
{
    return this->maxDepth;
}

std::string Turbo::Core::TViewport::ToString()
{
    return std::string();
}
