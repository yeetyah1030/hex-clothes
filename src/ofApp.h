#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxImGui.h"
#include "ImHelpers.h"

#include "CVC.h"
#include "ColorProcessor.h"
#include "ofColor.h"
#include "UIManager.h"

class ofApp : public ofBaseApp
{
public:
    // assets
	ofImage m_background;
    ofImage m_cam01;
    
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

	UIManager m_uiManager;

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
    
    // processor helpers
    void processFrame(ofPixels& pixels, bool resolutionConflict);
    void updateDominantColor();
    
    // drawing helpers
    void drawGrayscaleDiffImage();
    void drawCenteredColorImage();
    void drawContours();
    void drawHexColorText();
};
