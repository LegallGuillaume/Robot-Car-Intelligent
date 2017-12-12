#ifndef IMAGES_H
#define IMAGES_H

#include <iostream>
#include <cmath>
#include <vector>
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "../definition.h"

using namespace cv;
using namespace aruco;

/*image processing*/
class ImagesP{

public:
    ImagesP();
    ~ImagesP();
    void startBlock();
    std::vector<Point> all_block;
    Point posCar, posArrival;
    void saveCalib(std::string str);
    Point2f loadCalib();
    void calibration();

private:
    MarkerDetector MDetector;
    std::vector<Marker> Markers;
    Mat markerImg, markerCpy;
    VideoCapture cap;

    /**
     * @file                images.h
     * @brief               can initialize all variables
     * @function            call in constructor (private function)
     * @version             1.0
     */
    void init();

    /**
     * @file                images.h
     * @brief               processing markers from list
     * @function            call in start() (private function)
     * @version             1.0
     *
     * @param std::vector<Marker> Markers   vector of Markers
     * @param Point2f centerPt      center of frame
     */
    void markersProcessing(std::vector<Marker> Markers, Point2f centerPt);

    /**
     * @file                images.h
     * @brief               processing markers from list
     * @function            call in start() (private function)
     * @version             1.0
     *
     * @param int16_t x_matrix      x to 0 from 24
     * @param int16_t y_matrix      y tp 0 from 24
     */
    Point getXYMatrix(int16_t x_matrix, int16_t y_matrix);

    /**
     * @file                images.h
     * @brief               processing markers from list
     * @function            call in start() (private function)
     * @version             1.0
     *
     * @param Point2f center        center of frame
     * @param int16_t x_pixel       
     */
    Point2f getBlockMatrice(Point2f center, int16_t x_pixel, int16_t y_pixel);
    float getAngle(Marker marker);
    void drawMatrix(Mat& frame, Point center);
    bool willbeBlock(Mat& frame, Point leftTop, int8_t pixelMax);
    void fillRotatedRect(Mat& frame, Point2f center, Size2f size, Scalar color, float angle);
};


#endif