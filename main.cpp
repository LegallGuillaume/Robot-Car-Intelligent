#include <iostream>
#include "headers/manager.h"
#include "headers/traitement/pathfinding.h"
#include "headers/traitement/images.h"
//#include "headers/definition.h"
#include <unistd.h>
//#include <utility>
#include <vector>

#include "headers/manager/serveur.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


/*Unitest move to ---> unitest.cpp*/
/*
void try_server(){
    Serveur *s = Serveur::getInstance();
    s->AcceptAndDispatch();
}*/

void addBlocks(Manager *_m, int8_t x_center, int8_t y_center){
    for(uint8_t x=x_center-1; x<x_center+2; ++x){
        for(uint8_t y=y_center-1; y<y_center+2; ++y){
            _m->bloc->add(x,y);
        }
    }
}

void drawDebug(Mat& frame, std::vector<std::pair<int8_t, int8_t>> *cheminTerminate, Manager *manager){
    int RecX1 = 240-240;
    int RecX2 = 240+240;
    int RecY1 = 240-240;
    int RecY2 = 240+240;
    for(int j=1; j<25;j++){
        RecX2 = RecX2 - 20;
        rectangle(frame, Point(RecX1, RecY1), Point(RecX2, RecY2), cv::Scalar(0, 255, 0));
    }
    RecX2 = 240+240;
    //horizontal
    for(int j=1; j<25;j++){
        RecY2 = RecY2 - 20;
        rectangle(frame, Point(RecX1, RecY1), Point(RecX2, RecY2), cv::Scalar(0, 255, 0));
    }
    if(cheminTerminate != nullptr){
        for(int index=0; index < (int)cheminTerminate->size() - 1; ++index){
            std::pair<int8_t, int8_t> _p = cheminTerminate->at(index);
            Point point = Point((int)(_p.second*20), (int)(_p.first*20));
            rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(255,255,255));
        }
    }

    for(int index=0; index < (int)manager->bloc->list_bloc->size() - 1; ++index){
        std::pair<int8_t, int8_t> _p = manager->bloc->list_bloc->at(index);
        Point point = Point((int)(_p.second*20), (int)(_p.first*20));
        rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(0, 0, 255));
    }

    Point point = Point((int)(manager->car->getPosition().second*20), (int)(manager->car->getPosition().first*20));
    rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(0, 255, 0));
    rectangle(frame, point - Point(-4,-4),  point + Point(16,16), Scalar(0, 255, 0));
    rectangle(frame, point - Point(-6,-6),  point + Point(14,14), Scalar(0, 255, 0));

    point = Point((int)(manager->arrive->getPosition().second*20), (int)(manager->arrive->getPosition().first*20));
    rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(255, 0, 0));
    rectangle(frame, point - Point(-4,-4),  point + Point(16,16), Scalar(255, 0, 0));
    rectangle(frame, point - Point(-6,-6),  point + Point(14,14), Scalar(255, 0, 0));

    imshow("TEST", frame);
    if(waitKey(30000) == 0){
        exit(0);
    }
}

int main(){
    /*Manager * manager = Manager::getInstance();
    manager->initScene(8);
    cv::VideoCapture capture;
    capture.open(1);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 400);
    Calibration *calibre = new Calibration(capture);
    calibre->start();*/
    //try_server();     //-------------Lancer le serveur!-----------------
    Manager *manager = Manager::getInstance();
    manager->initScene(24);
    /*addBlocks(manager, 1, 16);
    addBlocks(manager, 4, 4);
    addBlocks(manager, 4, 10);
    addBlocks(manager, 4, 13);
    addBlocks(manager, 4, 16);
    addBlocks(manager, 10, 7);
    addBlocks(manager, 10, 10);
    addBlocks(manager, 10, 16);
    addBlocks(manager, 11, 22);
    addBlocks(manager, 13, 4);
    addBlocks(manager, 13, 16);
    addBlocks(manager, 16, 4);
    addBlocks(manager, 16, 10);
    addBlocks(manager, 16, 19);
    //addBlocks(manager, 17, 20); //no possible
    addBlocks(manager, 20, 2);
    addBlocks(manager, 19, 13);
    addBlocks(manager, 22, 7);
    addBlocks(manager, 22, 19);*/
    Mat frame(480, 480, CV_8UC3, Scalar(0));
    ImagesP *process = new ImagesP();
    std::cout<<"CALIBRATION ..."<<std::endl;
    process->calibration();
    std::cout<<"LOAD ... "<<process->loadCalib()<<std::endl;
    exit(0);
    process->startBlock();
    exit(0);
    for(int8_t i=0; i<process->all_block.size();++i){
        manager->bloc->add(process->all_block.at(i).x, process->all_block.at(i).y);
    }
    manager->arrive->setPosition(22, 22);
    manager->car->setPosition(22, 10);
    manager->update();
    PathFinding *path = new PathFinding(
        manager->getGeneralTable(), manager->getSceneCarrer(), manager->getSceneCarrer(), 
        manager->car->getPosition(), manager->arrive->getPosition()
    );
    if(!path->hasPossibility()){
        std::cout<<"NO POSSIBILITY"<<std::endl;
        std::cout<<manager->toString()<<std::endl;
        drawDebug(frame, nullptr, manager);
        return -1;
    }
    std::vector<std::pair<int8_t, int8_t>> *cheminTerminate = path->getChemin();
    for(int index=0; index < (int)cheminTerminate->size() - 1; ++index)
        manager->chemin->add(cheminTerminate->at(index).first, cheminTerminate->at(index).second);


    drawDebug(frame, cheminTerminate, manager);
    manager->update();
    std::cout<<manager->toString()<<std::endl;
    //std::cout<<manager->toString()<<std::endl;

    return 0;
}