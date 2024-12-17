#include "ofApp.h"

// SETUP --------------------------------------------------------------
void ofApp::setup()
{
    // load resources
    m_background.load("background.jpg");
    m_cam01.load("cam01.png");
    
    // app setup
    ofSetWindowShape(CVC::APP_WINDOW_WIDTH, CVC::APP_WINDOW_HEIGHT);
    ofSetFrameRate(CVC::APP_DESIRED_FRAMERATE);

    // cam setup
    m_videoGrabber.setDeviceID(0);
    m_videoGrabber.setDesiredFrameRate(CVC::CAM_DESIRED_FRAMERATE);
    m_videoGrabber.initGrabber(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_camResolutionConflict = (((int)m_videoGrabber.getWidth() != CVC::VIDEO_WIDTH) || ((int)m_videoGrabber.getHeight() != CVC::VIDEO_HEIGHT));

    // video setup
    m_videoPlayer.load(CVC::VIDEO_PATH_COLOR);
    m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
    m_videoPlayer.play();

    m_videoResolutionConflict = (((int)m_videoPlayer.getWidth() != CVC::VIDEO_WIDTH) || ((int)m_videoPlayer.getHeight() != CVC::VIDEO_HEIGHT));

    // allocate CV images
    m_colorImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_grayscaleDiffImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);

    // init gui vars
    m_threshold = 128;
    m_numContoursConsidered = 5;
    m_minArea = 10.0f;
    m_maxArea = (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT);

    // app states setup
    m_appMode = CVC::APP_MODE::APP_VIDEO;
    m_appModes.push_back("APP_VIDEO");
    m_appModes.push_back("APP_CAM");

    // init dom colour
    m_dominantColor = ofColor(128, 128, 128);

    // UI Manager setup
    m_uiManager.setup();
}

// UPDATE --------------------------------------------------------------
void ofApp::update() 
{
    switch (m_appMode)
    {
        case CVC::APP_MODE::APP_VIDEO:
        {
            m_videoPlayer.update();

            if (m_videoPlayer.isFrameNew())
            {
                processFrame(m_videoPlayer.getPixels(), m_videoResolutionConflict);
            }
            break;
        }

        case CVC::APP_MODE::APP_CAM:
        {
            if (!m_camPaused)
            {
                m_videoGrabber.update();

                if (m_videoGrabber.isFrameNew())
                {
                    processFrame(m_videoGrabber.getPixels(), m_camResolutionConflict);
                }
            }
            break;
        }
    }
}

// DRAW --------------------------------------------------------------
void ofApp::draw() 
{
    ofSetColor(255, 255, 255);
    m_background.draw(0, 0, ofGetWidth(), ofGetHeight());

    // draw videos
    drawGrayscaleDiffImage();
    drawCenteredColorImage();
    
    // draw contours
    drawContours();

    // draw ui
    m_uiManager.draw(m_appMode, m_appModes, m_normalizedColor, m_dominantColor, m_cam01);
    
    drawHexColorText();
}

// FUNCTIONS --------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
    if (key == 32) 
    { // spacebar
        switch (m_appMode)
        {
            case CVC::APP_MODE::APP_VIDEO:
            {
                m_videoPlayer.setPaused(!m_videoPlayer.isPaused());
            }
            break;

            case CVC::APP_MODE::APP_CAM:
            {
                m_camPaused = !m_camPaused;
            }
            break;
        }
    }
}

// processor helpers:
void ofApp::processFrame(ofPixels& pixels, bool resolutionConflict)
{
    m_colorImage.setFromPixels(pixels);

    if (resolutionConflict) 
    {
        m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    }

    m_colorProcessor.processColor(
        m_colorImage,
        m_threshold,
        m_minArea,
        m_maxArea,
        m_contourFinder,
        m_grayscaleDiffImage
    );

    updateDominantColor();
}

void ofApp::updateDominantColor()
{
    if (m_contourFinder.nBlobs > 0) 
    {
        m_dominantColor = m_colorProcessor.getAverageDominantColor();

        // Normalize to 0.0-1.0 range
        m_normalizedColor.r = m_dominantColor.r / 255.0f;
        m_normalizedColor.g = m_dominantColor.g / 255.0f;
        m_normalizedColor.b = m_dominantColor.b / 255.0f;
    }
}

// drawing helpers
void ofApp::drawGrayscaleDiffImage() 
{
    ofSetColor(255, 255, 255, 0); // make it transparent
    m_grayscaleDiffImage.draw(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);
    ofSetColor(255, 255, 255, 255); // reset to opaque
}

void ofApp::drawCenteredColorImage() 
{
    float x = (ofGetWidth() - m_colorImage.getWidth()) / 2;
    float y = (ofGetHeight() - m_colorImage.getHeight()) / 2.4;
    m_colorImage.draw(x, y);
}

void ofApp::drawContours() 
{
    static ofVec2f contourCenter;
    static float contourArea;

    ofPushMatrix();
    {
        ofTranslate(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);
        // loop through blobs and show debug info
        for (int i = 0; i < m_contourFinder.nBlobs; i++) {
            contourCenter.set(m_contourFinder.blobs[i].boundingRect.getCenter().x,
                m_contourFinder.blobs[i].boundingRect.getCenter().y);
            contourArea = m_contourFinder.blobs[i].area;
        }
    }
    ofPopMatrix();
}

void ofApp::drawHexColorText() 
{
    // Set color to white for text
    ofSetColor(255, 255, 255); // white text

    // Prepare the hex color string
    char hexColorText[50];
    snprintf(hexColorText, sizeof(hexColorText), "Most Frequent Colour: #%02X%02X%02X",
        (int)m_dominantColor.r,
        (int)m_dominantColor.g,
        (int)m_dominantColor.b);

    // Calculate the horizontal center position
    float textX = (ofGetWidth() / 2.0) - (strlen(hexColorText) * 4);
    float textY = 30;  // Vertical position at the top

    // Draw the text on screen
    ofDrawBitmapString(hexColorText, textX, textY);

    // Reset color to the dominant color after drawing text
    ofSetColor(m_dominantColor);
}




