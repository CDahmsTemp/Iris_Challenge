// UtilityFunctions.h

#ifndef UTILITY_FUNCTIONS
#define UTILITY_FUNCTIONS

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "Main.h"
#include "UtilityFunctions.h"


// function prototypes ////////////////////////////////////////////////////////////////////////////
void drawContour(cv::Mat image, std::vector<cv::Point> contour, cv::Scalar color, int thickness);
void drawAndShowContour(cv::Size imageSize, std::vector<cv::Point> contour, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
cv::Point2f findContourCenterOfMass(std::vector<cv::Point> contour);
std::vector<cv::Point> getLargestContour(std::vector<std::vector<cv::Point> > contours);

std::vector<cv::Point> genPerfectCircleContour();
std::vector<std::vector<cv::Point> > getCircularContours(std::vector<std::vector<cv::Point> > contours, double minCircularity);




#endif	// UTILITY_FUNCTIONS
