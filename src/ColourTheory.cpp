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

void ColourTheory::displayColour(int x, int y, ofColor colour)
{
	// These x and y values can be changed later on
	ofSetColor(colour);
	ofDrawRectangle(x, y, 50, 50);

	ofSetColor(255);
	std::string hexCode = ofToHex(colour.r) + ofToHex(colour.g) + ofToHex(colour.b);
	ofDrawBitmapString("#" + hexCode, x, y, 65);
}

void ColourTheory::calculateAllColours(const ofColor& colour)
{
	calculateComplementary(colour);
	calculateAnalogous(colour);
	calculateTriadic(colour);
}

void ColourTheory::drawAll()
{
	// These x and y values can be changed later on
	displayColour(0, 60, complementaryRGB);

	displayColour(0, 140, analogousRGB[0]);
	displayColour(80, 140, analogousRGB[1]);

	displayColour(0, 200, triadicRGB[0]);
	displayColour(80, 200, triadicRGB[1]);
}
