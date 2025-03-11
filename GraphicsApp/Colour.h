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

Colour LerpColours(Colour col1, Colour col2, float alpha)
{
	Colour nextColour;
	nextColour.r = (col1.r * (1 - alpha)) + (col2.r * alpha);
	nextColour.g = (col1.g * (1 - alpha)) + (col2.g * alpha);
	nextColour.b = (col1.b * (1 - alpha)) + (col2.b * alpha);
	nextColour.a = (col1.a * (1 - alpha)) + (col2.a * alpha);

	return nextColour;
}