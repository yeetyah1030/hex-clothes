#include "ofApp.h"

// SETUP --------------------------------------------------------------
void ofApp::setup() {
    // load resources
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

    // gui setup
    m_gui.setup();

    // app states setup
    m_appMode = CVC::APP_MODE::APP_VIDEO;
    m_appModes.push_back("APP_VIDEO");
    m_appModes.push_back("APP_CAM");

    // init dom colour
    m_dominantColor = ofColor(128, 128, 128);
}

// UPDATE --------------------------------------------------------------
void ofApp::update() {
    switch (m_appMode) {
    case CVC::APP_MODE::APP_VIDEO: {
        m_videoPlayer.update();

        if (m_videoPlayer.isFrameNew()) {
            processFrame(m_videoPlayer.getPixels(), m_videoResolutionConflict);
        }
        break;
    }

    case CVC::APP_MODE::APP_CAM: {
        if (!m_camPaused) {
            m_videoGrabber.update();

            if (m_videoGrabber.isFrameNew()) {
                processFrame(m_videoGrabber.getPixels(), m_camResolutionConflict);
            }
        }
        break;
    }
    }
}


// DRAW --------------------------------------------------------------
void ofApp::draw() {
    
    // draw videos
    ofSetColor(255, 255, 255, 0); // make it transparent
    m_grayscaleDiffImage.draw(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);
    ofSetColor(255, 255, 255, 255); // reset to opaque
    
    // center colorImage
    float x1 = (ofGetWidth() - m_colorImage.getWidth()) / 2;
    float y1 = (ofGetHeight() - m_colorImage.getHeight()) / 2.4;
    m_colorImage.draw(x1, y1);

    // draw contours
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

            // debug info
            /*m_contourFinder.blobs[i].draw();
            ofSetColor(ofColor::coral);
            ofDrawCircle(contourCenter.x, contourCenter.y, 5.0f);
            ofDrawBitmapString("Centre: " + ofToString(contourCenter.x) + "," + ofToString(contourCenter.y),
                m_contourFinder.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                contourCenter.y);
            ofDrawBitmapString("Area: " + ofToString(contourArea),
                m_contourFinder.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                contourCenter.y + 20.0f);*/
        }
    }
    
    // draw image in center
    ofSetColor(255, 255, 255);
    float x2 = (ofGetWidth() - m_cam01.getWidth()) / 2;
    float y2 = (ofGetHeight() - m_cam01.getHeight()) / 2;
    m_cam01.draw(x2, y2);
    
    ofPopMatrix();

    // draw gui
    m_gui.begin();
    {
        ImGui::Text("Color Detection");
        ImGui::SliderInt("Threshold", &m_threshold, 0, 255);
        ImGui::SliderInt("Num of Contours", &m_numContoursConsidered, 0, 30);
        ImGui::SliderFloat("Min. Area", &m_minArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));
        ImGui::SliderFloat("Max. Area", &m_maxArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));

        ImGui::Separator();

        ImGui::Text("\n Select an app state");
        static int currentListBoxIndex = 0;
        if (ofxImGui::VectorCombo("App Mode", &currentListBoxIndex, m_appModes)) {
            m_appMode = (CVC::APP_MODE)currentListBoxIndex;
        }
        
        // hex color display
        ImGui::Text("Hex Color: #%02X%02X%02X",
            (int)m_dominantColor.r,
            (int)m_dominantColor.g,
            (int)m_dominantColor.b);

        // dominant color display
        ImGui::Text("Dominant Color:");
        // use the normalized colour for ImGui
        ImGui::ColorEdit3("Dominant Color", (float*)&m_normalizedColor);

        // draw a rectangle with the dom color
        ofSetColor(m_dominantColor);
        ofDrawRectangle(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2 + 10,
            CVC::VIDEO_BORDER_SIZE + 10, 50, 50);
    }
    m_gui.end();
    
    // draw hex color text at top center of screen
    ofSetColor(255, 255, 255); // white text
    char hexColorText[20];
    snprintf(hexColorText, sizeof(hexColorText), "Hex Color: #%02X%02X%02X",
        (int)m_dominantColor.r,
        (int)m_dominantColor.g,
        (int)m_dominantColor.b);

    // calc horizontal center position
    float textX = (ofGetWidth() / 2.0) - (strlen(hexColorText) * 4);
    float textY = 30;
    ofDrawBitmapString(hexColorText, textX, textY);

    // reset color to dominant color after drawing text
    ofSetColor(m_dominantColor);
}

// FUNCTIONS --------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 32) { // spacebar
        switch (m_appMode) {
        case CVC::APP_MODE::APP_VIDEO: {
            m_videoPlayer.setPaused(!m_videoPlayer.isPaused());
        }
                                     break;

        case CVC::APP_MODE::APP_CAM: {
            m_camPaused = !m_camPaused;
        }
                                   break;
        }
    }
}

// processor helpers:
void ofApp::processFrame(ofPixels& pixels, bool resolutionConflict) {
    m_colorImage.setFromPixels(pixels);

    if (resolutionConflict) {
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

void ofApp::updateDominantColor() {
    if (m_contourFinder.nBlobs > 0) {
        m_dominantColor = m_colorProcessor.getAverageDominantColor();

        // Normalize to 0.0-1.0 range
        m_normalizedColor.r = m_dominantColor.r / 255.0f;
        m_normalizedColor.g = m_dominantColor.g / 255.0f;
        m_normalizedColor.b = m_dominantColor.b / 255.0f;
    }
}


