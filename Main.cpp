// Main.cpp

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows

// global constants ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 255.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);
const cv::Scalar SCALAR_PURPLE = cv::Scalar(128.0, 0.0, 128.0);

// function prototypes ////////////////////////////////////////////////////////////////////////////
void detectHorizon(cv::Mat &imgOriginal);
void detectClouds(cv::Mat &imgOriginal);
void detectSun(cv::Mat &imgOriginal);
void detectTimeOfDay(cv::Mat &imgOriginal);
void detectWeatherConditions(cv::Mat &imgOriginal);
void detectSetting(cv::Mat &imgOriginal);

std::vector<std::vector<cv::Point> > processImageToConvexHullsLookingForSun(cv::Mat &image, std::string appendName);
std::vector<cv::Point> genPerfectCircleContour();
std::vector<cv::Point> getLargestContour(std::vector<std::vector<cv::Point> > contours);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContour(cv::Size imageSize, std::vector<cv::Point> contour, std::string strImageName);
std::vector<std::vector<cv::Point> > getCircularContours(std::vector<std::vector<cv::Point> > contours);
cv::Point2f findContourCenterOfMass(std::vector<cv::Point> contour);

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
void detectSun(cv::Mat &imgOriginal) {

    cv::Mat imgHsv;
    cv::Mat imgHsvChannels[3];
    cv::Mat imgHue;
    cv::Mat imgSat;
    cv::Mat imgValue;

    // convert to HSV
    cv::cvtColor(imgOriginal, imgHsv, CV_BGR2HSV);       // convert to grayscale
    //cv::imshow("imgHsv", imgHsv);

    // split HSV image into Hue, Sat, and Value
    cv::split(imgHsv, imgHsvChannels);
    imgHue = imgHsvChannels[0];
    imgSat = imgHsvChannels[1];
    imgValue = imgHsvChannels[2];

    //cv::imshow("imgHue", imgHue);
    //cv::imshow("imgSat", imgSat);
    cv::imshow("imgValue", imgValue);

    cv::Mat imgInvertedHue = cv::Mat(imgOriginal.size(), CV_8UC3, SCALAR_BLACK);
    cv::Mat imgInvertedSat = cv::Mat(imgOriginal.size(), CV_8UC3, SCALAR_BLACK);
    cv::Mat imgInvertedValue = cv::Mat(imgOriginal.size(), CV_8UC3, SCALAR_BLACK);

    cv::bitwise_not(imgHue, imgInvertedHue);
    cv::bitwise_not(imgSat, imgInvertedSat);
    cv::bitwise_not(imgValue, imgInvertedValue);

    //cv::imshow("imgInvertedHue", imgInvertedHue);
    cv::imshow("imgInvertedSat", imgInvertedSat);
    //cv::imshow("imgInvertedValue", imgInvertedValue);

    // process the value and inverted sat images down to convex hulls
    std::vector<std::vector<cv::Point> > valueConvexHulls = processImageToConvexHullsLookingForSun(imgValue, " - Value");
    std::vector<std::vector<cv::Point> > invertedSatConvexHulls = processImageToConvexHullsLookingForSun(imgInvertedSat, " - Inverted Sat");

    // for both value and invertes sat images, get only semi-circular convex hulls
    std::vector<std::vector<cv::Point> > circularValueConvexHulls = getCircularContours(valueConvexHulls);
    drawAndShowContours(imgOriginal.size(), circularValueConvexHulls, "circularValueConvexHulls");

    std::vector<std::vector<cv::Point> > circularInvertedSatConvexHulls = getCircularContours(invertedSatConvexHulls);
    drawAndShowContours(imgOriginal.size(), circularInvertedSatConvexHulls, "circularInvertedSatConvexHulls");

    // for both value and invertes sat images, get the largest circular convex hull, note it is a bad assumption that the largest will always be the sun
    std::vector<cv::Point> valueSun = getLargestContour(circularValueConvexHulls);
    drawAndShowContour(imgOriginal.size(), valueSun, "valueSun");
    
    std::vector<cv::Point> invertedSatSun = getLargestContour(circularInvertedSatConvexHulls);
    drawAndShowContour(imgOriginal.size(), invertedSatSun, "invertedSatSun");

    // get the center of mass of both "suns"
    cv::Point2f invertedSatSunCenterOfMass = findContourCenterOfMass(invertedSatSun);
    cv::Point2f valueSunCenterOfMass = findContourCenterOfMass(valueSun);

    // if the center of mass of one sun is inside the other, and the other way around is also true, we've found the sun
    double pointWithinContour1 = cv::pointPolygonTest(valueSun, invertedSatSunCenterOfMass, false);
    double pointWithinContour2 = cv::pointPolygonTest(invertedSatSun, valueSunCenterOfMass, false);
    if (pointWithinContour1 > 0.0 && pointWithinContour2 > 0.0) {
        // suppose the smaller of the two overlapping contours is the outline of the sun (this is a bad assumption, fix this later)
        std::vector<cv::Point> smallerContour;
        if (cv::contourArea(valueSun) < cv::contourArea(invertedSatSun)) smallerContour = valueSun;
        else smallerContour = invertedSatSun;

        // draw the smaller contour on the original image in purple
        std::vector<std::vector<cv::Point> > contours;
        contours.push_back(smallerContour);
        cv::drawContours(imgOriginal, contours, -1, SCALAR_PURPLE, 4);        
    }
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
std::vector<std::vector<cv::Point> > getCircularContours(std::vector<std::vector<cv::Point> > contours) {
    const double MIN_CIRCULARITY = 0.9;
    std::vector<std::vector<cv::Point> > circularContours;      // this will be the return value

    // get a perfect circle contour so we can use matchShapes() next
    std::vector<cv::Point> perfectCircleContour = genPerfectCircleContour();

    for (auto &contour : contours) {
        double nonCirculatiry = cv::matchShapes(contour, perfectCircleContour, CV_CONTOURS_MATCH_I2, 0);
        double circularity = 1.0 - nonCirculatiry;
        if (circularity > MIN_CIRCULARITY) circularContours.push_back(contour);
    }
    return circularContours;
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
std::vector<std::vector<cv::Point> > processImageToConvexHullsLookingForSun(cv::Mat &image, std::string appendName) {
    
    // first threshold, use a very high threshold since we are looking for the sun (very bright)
    cv::Mat imgThresh;    
    cv::threshold(image, imgThresh, 250.0, 255.0, CV_THRESH_BINARY);
    //cv::imshow("imgThresh" + appendName, imgThresh);    cv::waitKey(0);

    // open image (erode, then dilate)
    cv::Mat imgThreshSmoothed = imgThresh.clone();
    cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::erode(imgThreshSmoothed, imgThreshSmoothed, structuringElement3x3);
    cv::dilate(imgThreshSmoothed, imgThreshSmoothed, structuringElement3x3);

    // smooth image (i.e. Gaussian blur)
    cv::GaussianBlur(imgThreshSmoothed, imgThreshSmoothed, cv::Size(3, 3), 0);

    // find and draw contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(imgThreshSmoothed.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat imgContours = cv::Mat(image.size(), CV_8UC3, SCALAR_BLACK);
    cv::drawContours(imgContours, contours, -1, SCALAR_WHITE);
    //cv::imshow("imgContours" + appendName, imgContours);    cv::waitKey(0);

    // find convex hulls
    std::vector<std::vector<cv::Point> > convexHulls(contours.size());
    for (unsigned int i = 0; i < contours.size(); i++) {
        cv::convexHull(contours[i], convexHulls[i]);
    }
    cv::Mat imgConvexHulls = cv::Mat(image.size(), CV_8UC3, SCALAR_BLACK);
    cv::drawContours(imgConvexHulls, convexHulls, -1, SCALAR_WHITE);
    //cv::imshow("imgConvexHulls" + appendName, imgConvexHulls);    cv::waitKey(0);
    
    return convexHulls;
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
void detectTimeOfDay(cv::Mat &imgOriginal) {

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void detectWeatherConditions(cv::Mat &imgOriginal) {

}


///////////////////////////////////////////////////////////////////////////////////////////////////
void detectSetting(cv::Mat &imgOriginal) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
    cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);
    cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);
    cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContour(cv::Size imageSize, std::vector<cv::Point> contour, std::string strImageName) {
    std::vector<std::vector<cv::Point> > contours;
    contours.push_back(contour);
    drawAndShowContours(imageSize, contours, strImageName);
}








