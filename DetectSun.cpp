// DetectSun.cpp

#include "DetectSun.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<cv::Point> detectSun(cv::Mat &imgOriginal) {

    cv::Mat imgHsv;
    cv::Mat imgHsvChannels[3];
    cv::Mat imgHue;
    cv::Mat imgSat;
    cv::Mat imgValue;

    // convert to HSV
    cv::cvtColor(imgOriginal, imgHsv, CV_BGR2HSV);       // convert to grayscale
                                                         
    // split HSV image into Hue, Sat, and Value
    cv::split(imgHsv, imgHsvChannels);
    imgHue = imgHsvChannels[0];
    imgSat = imgHsvChannels[1];
    imgValue = imgHsvChannels[2];

    //cv::imshow("imgHue", imgHue);
    //cv::imshow("imgSat", imgSat);
    cv::imshow("imgValue", imgValue);

    // invert Hue, Sat, and Value
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

    // for both value and inverted sat images, get only semi-circular convex hulls
    std::vector<std::vector<cv::Point> > circularValueConvexHulls = getCircularContours(valueConvexHulls, 0.5);
    drawAndShowContours(imgOriginal.size(), circularValueConvexHulls, "circularValueConvexHulls");

    std::vector<std::vector<cv::Point> > circularInvertedSatConvexHulls = getCircularContours(invertedSatConvexHulls, 0.9);
    drawAndShowContours(imgOriginal.size(), circularInvertedSatConvexHulls, "circularInvertedSatConvexHulls");

    // for both value and inverted sat images, get the largest circular convex hull
    // ToDo: assumption is made here that the largest circular contour will be the sun, this is wrong, fix this
    std::vector<cv::Point> valueSun = getLargestContour(circularValueConvexHulls);
    drawAndShowContour(imgOriginal.size(), valueSun, "valueSun");

    std::vector<cv::Point> invertedSatSun = getLargestContour(circularInvertedSatConvexHulls);
    drawAndShowContour(imgOriginal.size(), invertedSatSun, "invertedSatSun");

    // get the center of mass of both "suns"
    cv::Point2f invertedSatSunCenterOfMass = findContourCenterOfMass(invertedSatSun);
    cv::Point2f valueSunCenterOfMass = findContourCenterOfMass(valueSun);

    // if the inverted sat sun center of mass (the smaller sun) is inside the value sun (the larger sun), we've found the sun
    double sunWithinSun = cv::pointPolygonTest(valueSun, invertedSatSunCenterOfMass, false);
    if (sunWithinSun > 0.0) {
        // suppose the smaller of the two overlapping contours is the outline of the sun, then return that smaller sun contour
        // ToDo: this is a bad assumption, fix this
        std::vector<cv::Point> smallerSunContour;
        if (cv::contourArea(valueSun) < cv::contourArea(invertedSatSun)) smallerSunContour = valueSun;
        else smallerSunContour = invertedSatSun;
        return smallerSunContour;
    } else {    // else if a sun was not found, return an empty contour
        std::vector<cv::Point> emptyContour;
        return emptyContour;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::vector<cv::Point> > processImageToConvexHullsLookingForSun(cv::Mat &image, std::string appendName) {

    // first threshold, use a very high threshold since we are looking for the sun (very bright)
    cv::Mat imgThresh;
    cv::threshold(image, imgThresh, 252.0, 255.0, CV_THRESH_BINARY);
    //cv::imshow("imgThresh" + appendName, imgThresh);

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
    //cv::imshow("imgContours" + appendName, imgContours);

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



