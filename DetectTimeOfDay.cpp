// DetectTimeOfDay.cpp

#include "DetectTimeOfDay.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
int detectTimeOfDay(cv::Mat &imgOriginal) {

    // ToDo: this is a very crude time of day algorithm, when horizon line is found, probably should only look above that

    // find mean of grayscale image

    // declare grayscale image and convert original to grayscale
    cv::Mat imgGrayscale;
    cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);

    // find mean of grayscale image
    double grayscaleMean = findSingleChannelImageMean(imgGrayscale);
    std::cout << "\n" << "grayscaleMean = " << grayscaleMean << "\n";

    // next, find mean of value image

    // convert to HSV
    cv::Mat imgHsv;
    cv::cvtColor(imgOriginal, imgHsv, CV_BGR2HSV);

    // split into 3 HSV channels
    cv::Mat imgHsvChannels[3];
    cv::split(imgHsv, imgHsvChannels);

    // get value image
    cv::Mat imgValue;
    imgValue = imgHsvChannels[2];

    // find mean of value image
    double valueMean = findSingleChannelImageMean(imgValue);
    std::cout << "\n" << "valueMean = " << valueMean << "\n";

    // average the grayscale mean and value mean, then use this as the daytime / dusk / nighttime metric
    double averageBrightness = (grayscaleMean + valueMean) / 2.0;
    std::cout << "\n" << "averageBrightness = " << averageBrightness << "\n";

    if (valueMean < DUSK_THRESHOLD) {
        return NIGHTTIME;
    } else if (valueMean < DAYTIME_THRESHOLD) {
        return DUSK;
    } else {
        return DAYTIME;
    }

}








