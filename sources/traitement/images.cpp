#include "../../headers/traitement/images.h"
#include <fstream>


ImagesP::ImagesP(){
    init();
}

ImagesP::~ImagesP(){
    cap.release();
}

Point ImagesP::getXYMatrix(int16_t x_matrix, int16_t y_matrix){
    return Point((int)(x_matrix/20), (int)(y_matrix/20));
}

float ImagesP::getAngle(Marker marker){
    Point2f p_a = marker[1];
    Point2f p_b = marker[2];

    Point2f p_m(0,-1);
    Point2f p_i((p_a.x+p_b.x)/2, (p_a.y+p_b.y)/2);

    Point2f vec = marker.getCenter() - p_i;

    float scal = vec.x*p_m.x + vec.y*p_m.y;
    float deno = sqrt(pow(vec.x,2) + pow(vec.y,2)) * sqrt(pow(p_m.x,2) + pow(p_m.y,2));

    float r = acos(scal/deno);
    float result = (r*180/M_PI)-90;
    if(p_a.x > p_b.x && p_a.y >= p_b.y){
        result = 0 - result - 180;
    }else if(p_a.x<p_b.x && p_a.y > p_b.y){
        result = 180 - result;
    }
    /*Correction cause by pixel (angle sup 180 and inf to -180*/
    if(result > 180){
        result = 0 - 180 + (sqrt(pow(result,2)) - 180);
    }
    if(result < -180){
        result = 180 - (sqrt(pow(result,2)) - 180);
    }
    return result;

}

bool ImagesP::willbeBlock(Mat& frame, Point leftTop, int8_t pixelMax){
    int16_t counter = 0;
    for(int16_t x=leftTop.x; x<leftTop.x+18;++x){
       for(int16_t y=leftTop.y; y<leftTop.y+18;++y){
           Vec3b intensity = frame.at<Vec3b>(y, x);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            if(blue == 255 && green == 255 && red == 255){
                counter++;
            }
        }
    }
    if(counter >= pixelMax){
        return true;
    }
    return false;
}

Point2f ImagesP::getBlockMatrice(Point2f center, int16_t x_pixel, int16_t y_pixel){
    int16_t x_ref = center.x - 240;
    int16_t y_ref = center.y - 240;
    int16_t x_sta = x_pixel - x_ref;
    int16_t y_sta = y_pixel - y_ref;

    int16_t x = (int)(x_pixel/20)*20;
    int16_t y = (int)(y_pixel/20)*20;
    return Point2f(x+x_ref, y+y_ref);

}

void ImagesP::drawMatrix(Mat& frame, Point center){
    int16_t RecX1 = center.x-240;
    int16_t RecX2 = center.x+240;
    int16_t RecY1 = center.y-240;
    int16_t RecY2 = center.y+240;
    for(int16_t j=1; j<25;j++){
        RecX2 = RecX2 - 20;
        rectangle(frame, Point(RecX1, RecY1), Point(RecX2, RecY2), cv::Scalar(0, 255, 0));
    }
    RecX2 = center.x+240;
    //horizontal
    for(int16_t j=1; j<25;j++){
        RecY2 = RecY2 - 20;
        rectangle(frame, Point(RecX1, RecY1), Point(RecX2, RecY2), cv::Scalar(0, 255, 0));
    }
}

void ImagesP::fillRotatedRect(Mat& frame, Point2f center, Size2f size, Scalar color, float angle){
    RotatedRect rect(center, size, angle);
    cv::Point2f vertices2f[4];
    rect.points(vertices2f);
    cv::Point vertices[4];
    for(int i = 0; i < 4; ++i){
        vertices[i] = vertices2f[i];
    }
    cv::fillConvexPoly(frame,vertices, 4, color);
}

void ImagesP::markersProcessing(std::vector<Marker> Markers, Point2f centerPt){
    for (int16_t i=0;i<Markers.size();i++) {
        int16_t x = (int16_t)Markers[i].getCenter().x;
        int16_t y = (int16_t)Markers[i].getCenter().y;
        if(Markers[i].id == 12){
            drawMatrix(markerCpy, Point(240,240));
        }else{ /*bloc*/
            std::vector<Point> point_marker;
            Point2f new_center = Markers[i].getCenter() - centerPt;
            fillRotatedRect(
                markerCpy, Point2f(240,240) + new_center, Size2f(60, 60), Scalar(255,255,255), getAngle(Markers[i])
            );
            Point2f pts = getBlockMatrice(
                Point2f(240,240), 
                Markers[i].getCenter().x - (MARGE_BLOCK_PROCESSING*sqrt(2)), 
                Markers[i].getCenter().y - (MARGE_BLOCK_PROCESSING*sqrt(2))
            );
            int16_t p_x = pts.x;
            int16_t p_y = pts.y;
            for(int16_t ii=0; ii<6; ++ii){
                for(int16_t jj=0; jj<6;++jj){
                    bool bloc = willbeBlock(markerCpy, Point(p_x+1,p_y+1), 16);
                    if(bloc){
                        point_marker.push_back(getXYMatrix(p_y, p_x)); //inverser
                        rectangle(markerCpy, Point2f(p_x+2,p_y+2), Point2f(p_x+18,p_y+18), Scalar(0, 0, 255));
                    }
                    p_y +=20;
                }
                p_y = pts.y;
                p_x +=20;
            }
            all_block.insert(all_block.end(), point_marker.begin(), point_marker.end());
        }
    }
}

void ImagesP::init(){
    cap = VideoCapture(0);
    markerCpy = Mat(480,480, CV_8UC3, Scalar(0));
    cap >> markerImg;
}

void ImagesP::saveCalib(std::string str){
    ofstream file("ressources/calib_file.txt");
    file << str.c_str();
    file.close();
}

Point2f ImagesP::loadCalib(){
    Point2f centerPt(-1,-1);
    std::string line;
    ifstream file("ressources/calib_file.txt");
    if(file.is_open()){
        uint8_t count = 0;
        while(getline(file, line)){
            if(count == 0){
                centerPt.x = std::stod(line);
                count++;
            }else if(count == 1){
                centerPt.y = std::stod(line);
            }else{
                return centerPt;
            }
        }
        file.close();
    }
    return centerPt;
}

void ImagesP::calibration(){
    while(true){
        if(!cap.read(markerImg))
            break;
        MDetector.detect(markerImg,Markers);
        Point2f centerPt(0,0);
        for(int i=0; i<Markers.size();i++){
            if(Markers[i].id == 12){
                std::cout<<"MARKER ID FOUND ..."<<std::endl;
                centerPt = Markers[i].getCenter();
                std::string save(std::to_string(centerPt.x) +"\n"+ std::to_string(centerPt.y));
                std::cout<<"SAVE IN FILE ==> "<<save<<std::endl;
                saveCalib(save);
                return;
            }
        }
    }
}

void ImagesP::startBlock(){
    all_block = std::vector<Point>(0);
    Point2f centerPt = loadCalib();
    if(centerPt.x > 0){
        markersProcessing(Markers, centerPt);
    }else{
        std::cout<<"ERROR CALIB LOAD"<<std::endl;
    }
    /*DEBUG*/
    imshow("TEST", markerCpy);
    waitKey(30000);
}