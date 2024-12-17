#include "ColorTheory.h"

void ColorTheory::calculateComplementary(const ofColor& color)
{
	complementaryRGB.set(255 - color.r, 255 - color.g, 255 - color.b);
}

void ColorTheory::calculateAnalogous(const ofColor& color)
{
	analogousRGB.clear();

	// Extract HSB values from the colour (hue, saturation, brightness)
	float hue, saturation, brightness; 
	color.getHsb(hue, saturation, brightness);

	ofColor color1, color2;
	color1.setHsb(fmod(hue + 30, 255), saturation, brightness);
	color2.setHsb(fmod(hue - 30 + 255, 255), saturation, brightness);

	analogousRGB.push_back(color1);
	analogousRGB.push_back(color2);
}

void ColorTheory::calculateTriadic(const ofColor& color)
{
	triadicRGB.clear();

	float hue, saturation, brightness;
	color.getHsb(hue, saturation, brightness);

	ofColor color1, color2;
	color1.setHsb(fmod(hue + 120, 255), saturation, brightness);
	color2.setHsb(fmod(hue - 120 + 255, 255), saturation, brightness);

	triadicRGB.push_back(color1);
	triadicRGB.push_back(color2);
}

std::string ColorTheory::toHex(const ofColor& color)
{
	return "#" + ofToHex(color.r) + ofToHex(color.g) + ofToHex(color.b);
}

void ColorTheory::displayColor(int x, int y, ofColor color)
{
	// These x and y values can be changed later on
	ofSetColor(color);
	ofDrawRectangle(x, y, 50, 50);

	ofSetColor(255);
	ofDrawBitmapString(toHex(color), x + 35, y, 65);
}

void ColorTheory::calculateAllColors(const ofColor& color)
{
	calculateComplementary(color);
	calculateAnalogous(color);
	calculateTriadic(color);
}

void ColorTheory::displayComplementaryResult(int x, int y)
{
	displayColor(x, y, complementaryRGB);
}

void ColorTheory::displayAnalogousResult(int x, int y)
{
	displayColor(x, y, analogousRGB[0]);
	displayColor(x + 60, y, analogousRGB[1]);
}

void ColorTheory::displayTriadicResult(int x, int y)
{
	displayColor(x, y, triadicRGB[0]);
	displayColor(x + 60, y, triadicRGB[1]);
}

std::string ColorTheory::getFeedback(CVC::ColorMode colorMode)
{
	switch (colorMode)
	{
	case CVC::COMPLEMENTARY:
		return "Based on what you're wearing,\nthe color " + toHex(complementaryRGB) + "\nwill complement your outfit!";
	case CVC::ANALOGOUS:
		return "The colors " + toHex(analogousRGB[0]) + " and " + toHex(analogousRGB[1]) + "\ngo well together!";
	case CVC::TRIADIC:
		return "The colors " + toHex(triadicRGB[0]) + " and " + toHex(triadicRGB[1]) + "\nprovide a balanced tradic scheme!";
	default:
		return "No feedback available.";
	}
}

void ColorTheory::drawAll()
{
	// These x and y values can be changed later on
	displayColor(20, 320, complementaryRGB);

	displayColor(20, 400, analogousRGB[0]);
	displayColor(80, 400, analogousRGB[1]);

	displayColor(20, 480, triadicRGB[0]);
	displayColor(80, 480, triadicRGB[1]);
}
