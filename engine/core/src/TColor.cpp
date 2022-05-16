#include "TColor.h"

const TColor TColor::red = TColor(1.0, 0.0, 0.0);
const TColor TColor::green = TColor(0.0, 1.0, 0.0);
const TColor TColor::blue = TColor(0.0, 0.0, 1.0);
const TColor TColor::black = TColor(0.0, 0.0, 0.0);
const TColor TColor::white = TColor(1.0, 1.0, 1.0);

TColor::TColor() : TObject()
{
	this->r = 0.0f;
	this->g = 0.0f;
	this->b = 0.0f;
}

TColor::TColor(float value) : TObject()
{
	this->r = value;
	this->g = value;
	this->b = value;
}

TColor::TColor(float r, float g, float b) : TObject()
{
	this->r = r;
	this->g = g;
	this->b = b;
}

TColor::TColor(const TColor &color) : TObject()
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
}

TColor::~TColor()
{
}

std::string TColor::ToString()
{
	return std::string();
}
