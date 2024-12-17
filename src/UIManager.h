#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "CVC.h"
#include "ImHelpers.h"
#include "ColourTheory.h"

class UIManager
{
public:
	void setup();
	void draw(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, int& threshold, int& numContoursConsidered, float& minArea, float& maxArea, ofFloatColor& normalizedColor, ofColor& dominantColor, ofImage cam01);
	void drawGui(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, int& threshold, int& numContoursConsidered, float& minArea, float& maxArea, ofFloatColor& normalizedColor, ofColor& dominantColor);
	bool isAppModeChanged(CVC::APP_MODE& appMode, int& currentListBoxIndex, std::vector<std::string>& appModes);

	void updateColorTheory(const ofColor& baseColor);

private:
	ofxImGui::Gui m_gui;
	ColourTheory m_colorTheory;

	int m_selectedColorMode = CVC::ColorMode::COMPLEMENTARY;
};