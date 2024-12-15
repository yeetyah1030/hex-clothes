#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "CVC.h"

class UIManager
{
public:
	UIManager();

	void setup();
	void draw();
	void updateAppMode(CVC::APP_MODE& appMode);
	void updateTrackedColor(float trackedColor[4]);
	void getGuiParams(int& threshold, int& numContours, float& minArea, float& maxArea) const;

private:
	ofxImGui::Gui m_gui;
	int m_threshold; 
	int m_numContoursConsidered;
	float m_minArea;
	float m_maxArea;

	float m_trackedColor[4];

	std::vector<std::string> m_appModes;
	int m_currentAppModeIndex;
};