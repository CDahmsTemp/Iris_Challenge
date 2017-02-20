// DetectTimeOfDay.cpp

#include "DetectTimeOfDay.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
TimeOfDayEnum detectTimeOfDay(cv::Mat &imgOriginal) {

    // find mean of grayscale image

    // declare grayscale image and convert original to grayscale
    cv::Mat imgGrayscale;
    cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);

    // find mean of grayscale image
    double grayscaleMean = findSingleChannelImageMean(imgGrayscale);

    // show result on standard out
    std::cout << "\n\n" << "grayscaleMean = " << grayscaleMean << "\n\n";

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

    // show result on standard out
    std::cout << "\n\n" << "valueMean = " << valueMean << "\n\n";

    // average the grayscale mean and value mean, then use this as the daytime / dusk / nighttime metric
    double averageBrightness = (grayscaleMean + valueMean) / 2.0;

    if (averageBrightness < DUSK_THRESHOLD) {
        return NIGHTTIME;
    } else if (averageBrightness < DAYTIME_THRESHOLD) {
        return DUSK;
    } else {
        return DAYTIME;
    }



}








