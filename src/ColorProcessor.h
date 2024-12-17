#pragma once
#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvContourFinder.h"
#include "ofxCvGrayscaleImage.h"
#include <vector>
#include <map>

class ColorProcessor {
public:
    ColorProcessor();

    void processColor(ofxCvColorImage& image,
        int threshold,
        float minArea,
        float maxArea,
        ofxCvContourFinder& contourFinder,
        ofxCvGrayscaleImage& grayscaleDiffImage);

    ofColor findDominantColor(ofxCvColorImage& image);
    ofColor getDominantColor() const;
    ofColor getAverageDominantColor();

private:
    ofColor m_dominantColor;
    std::vector<ofColor> m_dominantColorHistory;
    const int MAX_COLOR_HISTORY = 10;
};
