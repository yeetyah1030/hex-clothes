#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ColorProcessor {
public:
    // constructor
    ColorProcessor();

    // process colour & return results
    void processColor(ofxCvColorImage& image,
        int threshold,
        float minArea,
        float maxArea,
        ofxCvContourFinder& contourFinder,
        ofxCvGrayscaleImage& grayscaleDiffImage);

    // getters
    ofColor getDominantColor() const;

private:
    // finding dom colour
    ofColor findDominantColor(ofxCvColorImage& image);

    // storing dom colour
    ofColor m_dominantColor;
};