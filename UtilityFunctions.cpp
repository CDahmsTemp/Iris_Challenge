// UtilityFunctions.cpp

#include "UtilityFunctions.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContour(cv::Size imageSize, std::vector<cv::Point> contour, std::string strImageName) {
    std::vector<std::vector<cv::Point> > contours;
    contours.push_back(contour);
    drawAndShowContours(imageSize, contours, strImageName);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);
    cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
cv::Point2f findContourCenterOfMass(std::vector<cv::Point> contour) {
    // find the contour moments
    cv::Moments moments = cv::moments(contour);

    // using the moments, find the center of mass
    double xCenter = moments.m10 / moments.m00;
    double yCenter = moments.m01 / moments.m00;

    return(cv::Point2f(xCenter, yCenter));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<cv::Point> getLargestContour(std::vector<std::vector<cv::Point> > contours) {
    int indexOfLargestContour = 0;
    int areaOfLargestContour = 0;

    // loop through every contour
    for (unsigned int i = 0; i < contours.size(); i++) {
        // if the current contour index is bigger that the previous largest contour . . .
        if (cv::contourArea(contours[i]) > areaOfLargestContour) {
            // update the index of the largest contour and the area of the largest contour so far
            indexOfLargestContour = i;
            areaOfLargestContour = cv::contourArea(contours[i]);
        }
    }
    return contours[indexOfLargestContour];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<cv::Point> genPerfectCircleContour() {

    const int PERFECT_CIRCLE_IMAGE_WIDTH = 500;
    const int PERFECT_CIRCLE_IMAGE_HEIGHT = 500;
    const int PERFECT_CIRCLE_X_CENTER = 250;
    const int PERFECT_CIRCLE_Y_CENTER = 250;
    const int PERFECT_CIRCLE_RADIUS = 100;

    // make a blank image, then draw a perfect circle
    cv::Mat imgPerfectCircle = cv::Mat(cv::Size(PERFECT_CIRCLE_IMAGE_WIDTH, PERFECT_CIRCLE_IMAGE_HEIGHT), CV_8UC1, SCALAR_BLACK);
    cv::circle(imgPerfectCircle, cv::Point(PERFECT_CIRCLE_X_CENTER, PERFECT_CIRCLE_Y_CENTER), PERFECT_CIRCLE_RADIUS, SCALAR_WHITE, -1);
    //cv::imshow("imgPerfectCircle", imgPerfectCircle);

    // threshold the image with the perfect circle
    cv::Mat imgPerfectCircleThresh;
    cv::threshold(imgPerfectCircle, imgPerfectCircleThresh, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);
    //cv::imshow("imgPerfectCircleThresh", imgPerfectCircleThresh);

    // find contours on the perfect circle thresh image
    std::vector<std::vector<cv::Point> > perfectCircleContours;
    cv::findContours(imgPerfectCircleThresh.clone(), perfectCircleContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // we should have a contour that is a perfect circle now, we can draw it to check
    cv::Mat imgPerfectCircleContour = cv::Mat(cv::Size(PERFECT_CIRCLE_IMAGE_WIDTH, PERFECT_CIRCLE_IMAGE_HEIGHT), CV_8UC1, SCALAR_BLACK);
    cv::drawContours(imgPerfectCircleContour, perfectCircleContours, -1, SCALAR_WHITE, -1);
    //cv::imshow("imgPerfectCircleContour", imgPerfectCircleContour);

    return perfectCircleContours[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::vector<cv::Point> > getCircularContours(std::vector<std::vector<cv::Point> > contours, double minCircularity) {    
    std::vector<std::vector<cv::Point> > circularContours;      // this will be the return value

    std::vector<cv::Point> perfectCircleContour = genPerfectCircleContour();

    for (auto &contour : contours) {
        double nonCirculatiry = cv::matchShapes(contour, perfectCircleContour, CV_CONTOURS_MATCH_I2, 0);
        double circularity = 1.0 - nonCirculatiry;
        if (circularity > minCircularity) circularContours.push_back(contour);
    }
    return circularContours;
}



