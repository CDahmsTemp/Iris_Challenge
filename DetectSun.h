// DetectSun.h

#ifndef DETECT_SUN
#define DETECT_SUN

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "Main.h"
#include "UtilityFunctions.h"

// function prototypes ////////////////////////////////////////////////////////////////////////////
void detectSun(cv::Mat &imgOriginal);

std::vector<std::vector<cv::Point> > processImageToConvexHullsLookingForSun(cv::Mat &image, std::string appendName);








#endif	// DETECT_SUN
