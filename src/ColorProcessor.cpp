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

    // add colour to history
    m_dominantColorHistory.push_back(m_dominantColor);

    // keep only the last MAX_COLOR_HISTORY colors
    if (m_dominantColorHistory.size() > MAX_COLOR_HISTORY) {
        m_dominantColorHistory.erase(m_dominantColorHistory.begin());
    }

    // get img properties
    const int numChannelsPerPixel = image.getPixels().getNumChannels();
    const int numChannels = image.getWidth() * image.getHeight() * numChannelsPerPixel;

    // process pixels
    for (int i = 0; i < numChannels; i += numChannelsPerPixel) {
        float pixel[3] = {
            static_cast<float>(image.getPixels()[i + 0]),     // R
            static_cast<float>(image.getPixels()[i + 1]),     // G
            static_cast<float>(image.getPixels()[i + 2])      // B
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
    if (m_dominantColorHistory.empty()) {
        return m_dominantColor;
    }
    
    // Return the most recently added color
    return m_dominantColorHistory.back();
}

// new method to get average dominant color
ofColor ColorProcessor::getAverageDominantColor() {
    if (m_dominantColorHistory.empty()) {
        return ofColor::black;
    }

    float totalR = 0, totalG = 0, totalB = 0;
    for (const auto& color : m_dominantColorHistory) {
        totalR += color.r;
        totalG += color.g;
        totalB += color.b;
    }

    return ofColor(
        totalR / m_dominantColorHistory.size(),
        totalG / m_dominantColorHistory.size(),
        totalB / m_dominantColorHistory.size()
    );
}
