#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "CVC.h"
#include "ImHelpers.h"
#include "ColorTheory.h"

// This class is responsible for handling most UI elements
class UIManager
{
public:
	void setup();
	void draw(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, ofFloatColor& normalizedColor, ofColor& dominantColor, ofImage cam01);
	void drawGui(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, ofFloatColor& normalizedColor, ofColor& dominantColor);
	bool isAppModeChanged(CVC::APP_MODE& appMode, int& currentListBoxIndex, std::vector<std::string>& appModes);

	void updateColorTheory(const ofColor& baseColor);

private:
	ofxImGui::Gui m_gui;
	ColorTheory m_colorTheory;
	

	int m_selectedColorMode = CVC::ColorMode::COMPLEMENTARY;
};