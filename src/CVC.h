#pragma once

#include "ofMain.h"

namespace CVC {
    static const int APP_WINDOW_WIDTH = 820;
    static const int APP_WINDOW_HEIGHT = 800;
    static const int APP_DESIRED_FRAMERATE = 60;
    static const int CAM_DESIRED_FRAMERATE = 30;

    static const std::string VIDEO_PATH_COLOR = "tupperware.mov";
    static const int VIDEO_WIDTH = 250;
    static const int VIDEO_HEIGHT = 280;

    static const int VIDEO_BORDER_SIZE = 10;

    typedef enum TYPE {
        APP_VIDEO,
        APP_CAM
    } APP_MODE;

    enum ColorMode
    {
        COMPLEMENTARY = 0,
        ANALOGOUS,
        TRIADIC
    };
};
