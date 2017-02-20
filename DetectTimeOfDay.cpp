// DetectTimeOfDay.cpp

#include "DetectTimeOfDay.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void detectTimeOfDay(cv::Mat &imgOriginal) {

    // find mean of grayscale image

    // declare grayscale image and convert original to grayscale
    cv::Mat imgGrayscale;
    cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);

    // use cv::mean() to get the mean of 4 channels, then get the mean of only the first channel (only relevant channel for this grayscale image)
    cv::Scalar grayscaleMeanScalar = cv::mean(imgGrayscale);
    double grayscaleMean = grayscaleMeanScalar[0];

    // show result on standard out
    std::cout << "\n\n" << "grayscaleMean = " << grayscaleMean << "\n\n";

    // next, find mean of value image

    // convert to HSV
    cv::Mat imgHsv;
    cv::cvtColor(imgOriginal, imgHsv, CV_BGR2HSV);








    int dummy99 = 1 + 2;

}








