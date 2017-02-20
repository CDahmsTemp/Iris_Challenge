// Main.cpp

#include "Main.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    cv::Mat imgOriginal;
    
    imgOriginal = cv::imread("image2.png");          // open image

    if (imgOriginal.empty()) {                                  // if unable to open image
        std::cout << "error: image not read from file\n\n";     // show error message on command line
        _getch();                                               // may have to modify this line if not using Windows
        return(0);                                              // and exit program
    }

    detectHorizon(imgOriginal);
    detectClouds(imgOriginal);


    std::vector<cv::Point> sunContour;
    
    detectSun(imgOriginal);



    detectTimeOfDay(imgOriginal);
    detectWeatherConditions(imgOriginal);
    detectSetting(imgOriginal);

    cv::imshow("imgOriginal", imgOriginal);

    cv::waitKey(0);                 // hold windows open until user presses a key

    return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void detectHorizon(cv::Mat &imgOriginal) {

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void detectClouds(cv::Mat &imgOriginal) {

}



///////////////////////////////////////////////////////////////////////////////////////////////////
void detectTimeOfDay(cv::Mat &imgOriginal) {

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void detectWeatherConditions(cv::Mat &imgOriginal) {

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void detectSetting(cv::Mat &imgOriginal) {

}










