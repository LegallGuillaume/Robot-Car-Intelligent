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
    void start();
    std::vector<Point> all_block;

private:
    MarkerDetector MDetector;
    std::vector<Marker> Markers;
    Mat markerImg, markerCpy;
    VideoCapture cap;

    void init();
    void markersProcessing(std::vector<Marker> Markers, Point2f centerPt);
    Point getXYMatrix(int16_t x_matrix, int16_t y_matrix);
    Point2f getBlockMatrice(Point2f center, int16_t x_pixel, int16_t y_pixel);
    float getAngle(Marker marker);
    void drawMatrix(Mat& frame, Point center);
    bool willbeBlock(Mat& frame, Point leftTop, int8_t pixelMax);
    void fillRotatedRect(Mat& frame, Point2f center, Size2f size, Scalar color, float angle);
};


#endif