#include "ofApp.h"

// SETUP --------------------------------------------------------------
void ofApp::setup() {
    // app setup
    ofSetWindowShape(CVC::APP_WINDOW_WIDTH, CVC::APP_WINDOW_HEIGHT);
    ofSetFrameRate(CVC::APP_DESIRED_FRAMERATE);

    // cam setup
    //m_videoGrabber.listDevices();     // find cameras on pc
    m_videoGrabber.setDeviceID(0);
    m_videoGrabber.setDesiredFrameRate(CVC::CAM_DESIRED_FRAMERATE);
    m_videoGrabber.initGrabber(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_camResolutionConflict = (((int)m_videoGrabber.getWidth() != CVC::VIDEO_WIDTH) ||
        ((int)m_videoGrabber.getHeight() != CVC::VIDEO_HEIGHT));

    // video setup
    m_videoPlayer.load(CVC::VIDEO_PATH_COLOR);
    m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
    m_videoPlayer.play();

    m_videoResolutionConflict = (((int)m_videoPlayer.getWidth() != CVC::VIDEO_WIDTH) ||
        ((int)m_videoPlayer.getHeight() != CVC::VIDEO_HEIGHT));

    // allocate CV images
    m_colorImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_grayscaleDiffImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);

    // init vars
    m_threshold = 128;
    m_numContoursConsidered = 5;
    m_minArea = 10.0f;
    m_maxArea = (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT);

    // gui
    m_gui.setup();

    // app states setup
    m_appMode = CVC::APP_MODE::APP_VIDEO;
    m_appModes.push_back("APP_VIDEO");
    m_appModes.push_back("APP_CAM");
}

// UPDATE --------------------------------------------------------------
void ofApp::update() {
    switch (m_appMode) {
    case CVC::APP_MODE::APP_VIDEO: {
        m_videoPlayer.update();

        // only process if it's a new frame
        if (m_videoPlayer.isFrameNew() == true) {
            m_colorImage.setFromPixels(m_videoPlayer.getPixels());

            // resize if there's a conflict
            if (m_videoResolutionConflict == true) {
                m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
            }

            processColor(m_colorImage);
        }
    }

    break;

    case CVC::APP_MODE::APP_CAM: {
        if (m_camPaused == false) {
            m_videoGrabber.update();

            if (m_videoGrabber.isFrameNew() == true) {
                m_colorImage.setFromPixels(m_videoGrabber.getPixels());
            }

            if (m_camResolutionConflict == true) {
                m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
            }

            processColor(m_colorImage);
        }

    }
    break;
    }
}

// DRAW --------------------------------------------------------------
void ofApp::draw() {
    // draw videos
    ofSetColor(255, 255, 255);
    m_grayscaleDiffImage.draw(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);
    m_colorImage.draw(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2.0f, CVC::VIDEO_BORDER_SIZE);

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

            // draw contour
            m_contourFinder.blobs[i].draw();

            // draw center
            ofSetColor(ofColor::coral);
            ofDrawCircle(contourCenter.x, contourCenter.y, 5.0f);

            // draw text
            ofDrawBitmapString("Centre: " + ofToString(contourCenter.x) + "," + ofToString(contourCenter.y),
                m_contourFinder.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                contourCenter.y);
            ofDrawBitmapString("Area: " + ofToString(contourArea),
                m_contourFinder.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                contourCenter.y + 20.0f);
        }
    }
    ofPopMatrix();

    // draw gui
    m_gui.begin();
    {
        ImGui::Text("Hex Clothes");
        ImGui::SliderInt("Threshold", &m_threshold, 0, 255);
        ImGui::SliderInt("Num of Contours", &m_numContoursConsidered, 0, 30);
        ImGui::SliderFloat("Min. Area", &m_minArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));
        ImGui::SliderFloat("Max. Area", &m_maxArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));

        ImGui::Separator();

        ImGui::Text("\n select an app state");
        static int currentListBoxIndex = 0;
        if (ofxImGui::VectorCombo("App Mode", &currentListBoxIndex, m_appModes)) {
            m_appMode = (CVC::APP_MODE)currentListBoxIndex;
        }

        // text display and draw most dom colour
        ImGui::Text("Dominant Color:");
        ImGui::ColorEdit3("Dominant Color", (float*)&m_dominantColor);
        ofSetColor(m_dominantColor);
        ofDrawRectangle(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2 + 10,CVC::VIDEO_BORDER_SIZE + 10, 50, 50);


        ImGui::ColorEdit3("Selected Color", (float*)m_trackedColor);
        ImGui::Text("\n Instructions: \ get the hex codes of your clothes!");
    }
    m_gui.end();
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


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    if (button == OF_MOUSE_BUTTON_RIGHT) {
        // select the colour from the image within bounded video region

        ofRectangle videoRect = ofRectangle(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2,
            CVC::VIDEO_BORDER_SIZE,
            CVC::VIDEO_WIDTH,
            CVC::VIDEO_HEIGHT);

        // clamping screenspace to image space
        int convertX = ofClamp(x, videoRect.getMinX(), videoRect.getMaxX());
        int convertY = ofClamp(y, videoRect.getMinY(), videoRect.getMaxY());

        // mapping screenspace to image space (by shift coordinate origin
        convertX -= videoRect.getMinX();
        convertY -= videoRect.getMinY();

        //get the color using x + y * width function
        const int index = (convertX + convertY * m_colorImage.getWidth()) * m_colorImage.getPixels().getNumChannels();
        m_trackedColor[0] = m_colorImage.getPixels()[index + 0] / 255.0f;
        m_trackedColor[1] = m_colorImage.getPixels()[index + 1] / 255.0f;
        m_trackedColor[2] = m_colorImage.getPixels()[index + 2] / 255.0f;
    }
}

//--------------------------------------------------------------
void ofApp::processColor(ofxCvColorImage& image) {
    const int numChannelsPerPixel = image.getPixels().getNumChannels();
    const int numChannels = CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT * numChannelsPerPixel;

    // storing frequency of colours & the frequency count
    map<ofColor, int, bool(*)(const ofColor&, const ofColor&)> colorFrequency([](const ofColor& a, const ofColor& b) {
        return a.getHex() < b.getHex();
        });

    float pixel[3] = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < numChannels; i += numChannelsPerPixel) {
        ofColor color(
            image.getPixels()[i + 0], // R
            image.getPixels()[i + 1], // G
            image.getPixels()[i + 2]  // B
        );

        // increase frequency of colour in the map
        colorFrequency[color]++;
    }

    // find most frequent colour
    int maxFrequency = 0;
    ofColor dominantColor;

    for (const auto& pair : colorFrequency) {
        if (pair.second > maxFrequency) {
            maxFrequency = pair.second;
            dominantColor = pair.first;
        }
    }

    m_dominantColor = dominantColor;

    // update greyscale
    for (int i = 0; i < numChannels; i += numChannelsPerPixel) {
        pixel[0] = image.getPixels()[i + 0]; // R
        pixel[1] = image.getPixels()[i + 1]; // G
        pixel[2] = image.getPixels()[i + 2]; // B

        if ((abs(pixel[0] - m_trackedColor[0] * 255.0f) < m_threshold) &&
            (abs(pixel[1] - m_trackedColor[1] * 255.0f) < m_threshold) &&
            (abs(pixel[2] - m_trackedColor[2] * 255.0f) < m_threshold)) {
            m_grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] = 255;
        }
        else {
            m_grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] = 0;
        }
    }

    m_grayscaleDiffImage.flagImageChanged();

    // find contours/blobs
    m_contourFinder.findContours(m_grayscaleDiffImage, m_minArea, m_maxArea, m_numContoursConsidered, false, true);
}
