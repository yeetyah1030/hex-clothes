#include "ColorProcessor.h"

// constructor
ColorProcessor::ColorProcessor() : m_dominantColor(ofColor::black) {}

// processing colour
void ColorProcessor::processColor(ofxCvColorImage& image,
    int threshold,
    float minArea,
    float maxArea,
    ofxCvContourFinder& contourFinder,
    ofxCvGrayscaleImage& grayscaleDiffImage) 
{
    // find dom colour
    m_dominantColor = findDominantColor(image);

    // get img properties
    const int numChannelsPerPixel = image.getPixels().getNumChannels();
    const int numChannels = image.getWidth() * image.getHeight() * numChannelsPerPixel;

    // process pixels
    for (int i = 0; i < numChannels; i += numChannelsPerPixel) {
        float pixel[3] = {
            image.getPixels()[i + 0],     // R
            image.getPixels()[i + 1],     // G
            image.getPixels()[i + 2]      // B
        };

        // convert to grayscale
        float luminance = 0.299f * pixel[0] + 0.587f * pixel[1] + 0.114f * pixel[2];

        // set pixel to white or black based on luminance threshold
        grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] =
            (luminance > threshold) ? 255 : 0;
    }
    grayscaleDiffImage.flagImageChanged();

    // find contours
    contourFinder.findContours(
        grayscaleDiffImage,
        minArea,
        maxArea,
        5, // default contour num
        false,
        true
    );
}

// finding dom colour
ofColor ColorProcessor::findDominantColor(ofxCvColorImage& image) {
    const int numChannelsPerPixel = image.getPixels().getNumChannels();
    const int numChannels = image.getWidth() * image.getHeight() * numChannelsPerPixel;

    // storing frequency of colors
    std::map<ofColor, int, bool(*)(const ofColor&, const ofColor&)> colorFrequency(
        [](const ofColor& a, const ofColor& b) {
            return a.getHex() < b.getHex();
        }
    );

    // keep a count of frequencies
    for (int i = 0; i < numChannels; i += numChannelsPerPixel) {
        ofColor color(
            image.getPixels()[i + 0], // R
            image.getPixels()[i + 1], // G
            image.getPixels()[i + 2]  // B
        );

        // increase frequency
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

    return dominantColor;
}

// dom colour getter
ofColor ColorProcessor::getDominantColor() const {
    return m_dominantColor;
}