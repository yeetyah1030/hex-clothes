#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "CVC.h"
#include "ImHelpers.h"

class UIManager
{
public:
	void setup();
	void draw(CVC::APP_MODE& appMode, std::vector<std::string>& appModes, int& threshold, int& numContoursConsidered, float& minArea, float& maxArea, ofFloatColor& normalizedColor, ofColor& dominantColor);
	bool isAppModeChanged(CVC::APP_MODE& appMode, int& currentListBoxIndex, std::vector<std::string>& appModes);

private:
	ofxImGui::Gui m_gui;


};