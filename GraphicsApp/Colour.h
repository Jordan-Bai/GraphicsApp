#pragma once

struct Colour
{
	float r, g, b, a;
	
	Colour()
		: r(0), g(0), b(0), a(1)
	{
	}
	Colour(float red, float green, float blue, float alpha)
		: r(red), g(green), b(blue), a(alpha)
	{
	}
};

Colour LerpColours(Colour col1, Colour col2, float alpha);