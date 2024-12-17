#include "ColourTheory.h"

void ColourTheory::calculateComplementary(const ofColor& colour)
{
	complementaryRGB.set(255 - colour.r, 255 - colour.g, 255 - colour.b);
}

void ColourTheory::calculateAnalogous(const ofColor& colour)
{
	analogousRGB.clear();

	// Extract HSB values from the colour (hue, saturation, brightness)
	float hue, saturation, brightness; 
	colour.getHsb(hue, saturation, brightness);

	ofColor colour1, colour2;
	colour1.setHsb(fmod(hue + 30, 255), saturation, brightness);
	colour2.setHsb(fmod(hue - 30 + 255, 255), saturation, brightness);

	analogousRGB.push_back(colour1);
	analogousRGB.push_back(colour2);
}

void ColourTheory::calculateTriadic(const ofColor& colour)
{
	triadicRGB.clear();

	float hue, saturation, brightness;
	colour.getHsb(hue, saturation, brightness);

	ofColor colour1, colour2;
	colour1.setHsb(fmod(hue + 120, 255), saturation, brightness);
	colour2.setHsb(fmod(hue - 120 + 255, 255), saturation, brightness);

	triadicRGB.push_back(colour1);
	triadicRGB.push_back(colour2);
}

std::string ColourTheory::toHex(const ofColor& color)
{
	return "#" + ofToHex(color.r) + ofToHex(color.g) + ofToHex(color.b);
}

void ColourTheory::displayColour(int x, int y, ofColor colour)
{
	// These x and y values can be changed later on
	ofSetColor(colour);
	ofDrawRectangle(x, y, 50, 50);

	ofSetColor(255);
	ofDrawBitmapString(toHex(colour), x + 35, y, 65);
}

void ColourTheory::calculateAllColours(const ofColor& colour)
{
	calculateComplementary(colour);
	calculateAnalogous(colour);
	calculateTriadic(colour);
}

void ColourTheory::displayComplementaryResult(int x, int y)
{
	displayColour(x, y, complementaryRGB);
}

void ColourTheory::displayAnalogousResult(int x, int y)
{
	displayColour(x, y, analogousRGB[0]);
	displayColour(x + 60, y, analogousRGB[1]);
}

void ColourTheory::displayTriadicResult(int x, int y)
{
	displayColour(x, y, triadicRGB[0]);
	displayColour(x + 60, y, triadicRGB[1]);
}

std::string ColourTheory::getFeedback(CVC::ColorMode colorMode)
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

void ColourTheory::drawAll()
{
	// These x and y values can be changed later on
	displayColour(20, 320, complementaryRGB);

	displayColour(20, 400, analogousRGB[0]);
	displayColour(80, 400, analogousRGB[1]);

	displayColour(20, 480, triadicRGB[0]);
	displayColour(80, 480, triadicRGB[1]);
}
