#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "CVC.h"
#include "ofApp.h"
#include "ColourTheory.h"

class UIManager
{
public:
	UIManager();

	void setup();
	void update();
	void draw(ofApp* app);

private:
	ofxImGui::Gui m_gui;

	int m_threshold;
	int m_numContours;
	float m_minArea;
	float m_maxArea;

	ColourTheory m_colourTheory;



};