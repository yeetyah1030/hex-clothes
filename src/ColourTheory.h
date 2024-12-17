#pragma once

#include "ofMain.h"
#include "CVC.h"

// This class is responsible for analyzing a single input colour based 
// on colour theory principles (complementary, analogous, triadic) and producing results for display
class ColourTheory
{
public:
	void drawAll();
	void calculateAllColours(const ofColor& colour);
	void displayComplementaryResult(int x, int y);
	void displayAnalogousResult(int x, int y);
	void displayTriadicResult(int x, int y);
	std::string getFeedback(CVC::ColorMode colorMode);

private:
	ofColor complementaryRGB;
	std::vector<ofColor> analogousRGB;
	std::vector<ofColor> triadicRGB;

	// Complementary colours are calculated by inverting the RGB values
	void calculateComplementary(const ofColor& colour);

	// Analogous colours are determined by shifting the hue of the base colour by
	// ±30 degrees in the HSV colour space
	void calculateAnalogous(const ofColor& colour);

	// Triadic colours are calculated by shifting the hue by 
	// ±120 degrees in the HSV colour space
	void calculateTriadic(const ofColor& colour);

	std::string toHex(const ofColor& color);
	void displayColour(int x, int y, ofColor colour);
};