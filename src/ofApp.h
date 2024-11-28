#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxImGui.h"
#include "ImHelpers.h"

#include "CVC.h"
#include "ColorProcessor.h"
#include "ofColor.h"

class ofApp : public ofBaseApp {
public:
	ofVideoPlayer m_videoPlayer;
	ofVideoGrabber m_videoGrabber;
	bool m_videoResolutionConflict;
	bool m_camResolutionConflict;
	bool m_camPaused;

	ofxCvContourFinder m_contourFinder;

	// CV images
	ofxCvColorImage		m_colorImage;
	ofxCvGrayscaleImage	m_grayscaleDiffImage;

	// dom colours
	ofColor m_dominantColor;

	//GUI
	ofxImGui::Gui		m_gui;
	int					m_threshold;
	int					m_numContoursConsidered;
	float				m_minArea;
	float				m_maxArea;

	//app modes
	CVC::APP_MODE m_appMode;
	std::vector<std::string> m_appModes;

	ColorProcessor m_colorProcessor;
	ofFloatColor m_normalizedColor;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
};