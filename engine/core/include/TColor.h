#ifndef TCOLOR_H
#define TCOLOR_H
#include "TObject.h"

class TColor : public Turbo::Core::TObject
{
public:
	float r;
	float g;
	float b;
	// flaot a;

	static const TColor red;
	static const TColor green;
	static const TColor blue;
	static const TColor black;
	static const TColor white;

public:
	TColor();
	TColor(float value);
	TColor(float r, float g, float b);
	TColor(const TColor &color);
	~TColor();

public:
	inline TColor operator+(const TColor &color) const
	{
		return TColor(this->r + color.r, this->g + color.g, this->b + color.b);
	}

	inline TColor &operator+=(const TColor &color)
	{
		this->r += color.r;
		this->g += color.g;
		this->b += color.b;
		return (*this);
	}

	inline TColor operator-(const TColor &color) const
	{
		return TColor(this->r - color.r, this->g - color.g, this->b - color.b);
	}

	inline TColor &operator-=(const TColor &color)
	{
		this->r -= color.r;
		this->g -= color.g;
		this->b -= color.b;
		return (*this);
	}

	inline TColor operator*(const TColor &color) const
	{
		return TColor(this->r * color.r, this->g * color.g, this->b * color.b);
	}

	inline TColor &operator*=(const TColor &color)
	{
		this->r *= color.r;
		this->g *= color.g;
		this->b *= color.b;
		return (*this);
	}

	inline TColor operator/(const TColor &color) const
	{
		return TColor(this->r / color.r, this->g / color.g, this->b / color.b);
	}

	inline TColor operator/=(const TColor &color)
	{
		this->r /= color.r;
		this->g /= color.g;
		this->b /= color.b;
		return (*this);
	}

	inline bool operator==(const TColor &color) const
	{
		return this->r == color.r && this->g == color.g && this->b == color.b;
	}

	inline bool operator!=(const TColor &color) const
	{
		return this->r != color.r || this->g != color.g || this->b != color.b;
	}

public:
	virtual std::string ToString() override;
};

#endif // !TCOLOR_H
