#ifndef IMAGES_H
#define IMAGES_H

#include <iostream>
#include <cmath>
#include <vector>
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "../manager.h"
#include "../definition.h"

using namespace cv;
using namespace aruco;

/*image processing*/
class ImagesP{

public:
    ImagesP();
    ~ImagesP();
    bool readyForPath(); /*check posCar != -1, posArrival != -1 and markers != -1*/
    void startBlock();
    std::vector<Point> all_block;
    Point posCar, posArrival;
    Point2f centerPoint;
    float angleCar, angleArrival;
    void saveCalib(std::string str);
    Point2f loadCalib();
    void calibration();
    Point getCarPosition();
    Point getArrivalPosition();
    uint8_t getSizeMarker();
    void trackingCar();

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
     * @param Point2f centerPt      center of frame
     */
    void markersProcessing(Point2f centerPt);

    /**
     * @file                images.h
     * @brief               processing markers from list
     * @function            call in start() (private function)
     * @version             1.0
     *
     * @param int16_t x_matrix      x to 0 from 24
     * @param int16_t y_matrix      y tp 0 from 24
     *
     * @return Point        coord in matrix
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
     
     * @return Point2f      coord in matrix       
     */
    Point2f getBlockMatrix(Point2f center, int16_t x_pixel, int16_t y_pixel);

    /**
     * @file                images.h
     * @brief               have angle from marker
     * @version             1.0
     *
     * @param Marker marker  
     *
     * @return float        angle of marker
     */
    float getAngle(Marker marker);

    /**
     * @file                images.h
     * @brief               draw matrix in debug frame
     * @version             1.0
     *
     * @param Mat& frame    to print debug matrix in this frame
     * @param Point center  center of Area marker
     *
     * @return float        angle of marker
     */
    void drawMatrix(Mat& frame, Point center);

    /**
     * @file                images.h
     * @brief               allow know if there are more or less pixel
     * @version             1.0
     *
     * @param Mat& frame        frame of processing images
     * @param Point leftTop     leftTop of Marker
     * @param int8_t pixelMax   consider as a block if there is this number of pixels in the square
     *
     * @return true             if there are more pixel in the block (matrix)
     * @return false            if there are less pixel in the block (matrix)
     */
    bool willbeBlock(Mat& frame, Point leftTop, int8_t pixelMax);

    /**
     * @file                images.h
     * @brief               this function allow to display block physically into abstract matrix
     * @version             1.0
     *
     * @param Mat& frame        frame of processing images
     * @param Point center      center of Marker
     * @param Size2f size       size of block (default is 150mm squared, like car or another block)
     * @param Scalar color      white color in abstract matrix to recognize the block
     * @param float angle       rotated rectangle from a specific angle
     *
     */
    void fillRotatedRect(Mat& frame, Point2f center, Size2f size, Scalar color, float angle);
};


#endif