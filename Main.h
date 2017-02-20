// Main.h

#ifndef MY_MAIN         // used MY_MAIN for this include guard rather than MAIN just in case some compilers or environments #define MAIN already
#define MY_MAIN

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "DetectHorizon.h"
#include "DetectClouds.h"
#include "DetectSun.h"
#include "DetectTimeOfDay.h"
#include "DetectWeatherConditions.h"
#include "DetectSetting.h"

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows

// global constants ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 255.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);
const cv::Scalar SCALAR_PURPLE = cv::Scalar(128.0, 0.0, 128.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);

const double DUSK_THRESHOLD = 20.0;
const double DAYTIME_THRESHOLD = 130.0;

enum TimeOfDayEnum { NIGHTTIME, DUSK, DAYTIME };

// function prototypes ////////////////////////////////////////////////////////////////////////////
void writeTextOnImage(cv::Mat &image, std::string text, cv::Scalar color);




# endif	// MAIN