// Main.cpp

#include "Main.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    cv::Mat imgOriginal;
    
    imgOriginal = cv::imread("image24.png");          // open image

    if (imgOriginal.empty()) {                                  // if unable to open image
        std::cout << "error: image not read from file\n\n";     // show error message on command line
        _getch();                                               // may have to modify this line if not using Windows
        return(0);                                              // and exit program
    }

    detectHorizon(imgOriginal);
    detectClouds(imgOriginal);

    std::vector<cv::Point> sunContour = detectSun(imgOriginal);       // try to find the sun

    // if a sun was found, draw the sun contour on the original image in purple and write that a sun was found on the image
    if (!sunContour.empty()) {
        drawContour(imgOriginal, sunContour, SCALAR_BLUE, 4);
        writeTextOnImage(imgOriginal, "Sun Found", SCALAR_BLUE);
    }

    int timeOfDay = detectTimeOfDay(imgOriginal);

    if (timeOfDay == DAYTIME) writeTextOnImage(imgOriginal, "Daytime", SCALAR_GREEN);
    else if (timeOfDay == DUSK) writeTextOnImage(imgOriginal, "Dusk", SCALAR_GREEN);
    else writeTextOnImage(imgOriginal, "Nighttime", SCALAR_GREEN);
    
    



    detectWeatherConditions(imgOriginal);
    detectSetting(imgOriginal);

    cv::imshow("imgOriginal", imgOriginal);

    cv::waitKey(0);                 // hold windows open until user presses a key

    return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void writeTextOnImage(cv::Mat &image, std::string text, cv::Scalar color) {
    static int textHeight = (int)((double)image.rows * 0.1);

    cv::Point ptLowerLeftTextOrigin((int)(image.cols * 0.75), textHeight);                // this will be the bottom left of the area that the text will be written to

    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;                      // choose a plain jane font
    double dblFontScale = (double)image.cols * 0.001;               // base font scale on width of image
    int intFontThickness = (int)std::round(dblFontScale * 2.0);     // base font thickness on font scale
    int intBaseline = 0;

    // call getTextSize
    cv::Size textSize = cv::getTextSize(text, intFontFace, dblFontScale, intFontThickness, &intBaseline);

    // write the text on the image
    cv::putText(image, text, ptLowerLeftTextOrigin, intFontFace, dblFontScale, color, intFontThickness);

    // increment static variable text height for the next time around
    textHeight = textHeight + (int)((double)image.rows * .065);
}























