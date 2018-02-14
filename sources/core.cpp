#include <iostream>
#include "../headers/core.h"


Core::Core(){
    init();
}

Core::~Core(){
    delete path;
    path = nullptr;
    delete process;
    process = nullptr;
    delete trajectory;
    trajectory = nullptr;
    delete manager;
    manager = nullptr;
}

void Core::drawDebug(Mat& frame){
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
    if(manager->path->list_path != nullptr){
        for(int index=0; index < (int)manager->path->list_path->size() - 1; ++index){
            std::pair<int8_t, int8_t> _p = manager->path->list_path->at(index);
            Point point = Point((int)(_p.second*20), (int)(_p.first*20));
            rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(255,255,255));
        }
    }

    for(int index=0; index < (int)manager->block->list_block->size() - 1; ++index){
        std::pair<int8_t, int8_t> _p = manager->block->list_block->at(index);
        Point point = Point((int)(_p.second*20), (int)(_p.first*20));
        rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(0, 0, 255));
    }

    Point point = Point((int)(manager->car->getPosition().second*20), (int)(manager->car->getPosition().first*20));
    rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(0, 255, 0));
    rectangle(frame, point - Point(-4,-4),  point + Point(16,16), Scalar(0, 255, 0));
    rectangle(frame, point - Point(-6,-6),  point + Point(14,14), Scalar(0, 255, 0));

    point = Point((int)(manager->end->getPosition().second*20), (int)(manager->end->getPosition().first*20));
    rectangle(frame, point - Point(-2,-2),  point + Point(18,18), Scalar(255, 0, 0));
    rectangle(frame, point - Point(-4,-4),  point + Point(16,16), Scalar(255, 0, 0));
    rectangle(frame, point - Point(-6,-6),  point + Point(14,14), Scalar(255, 0, 0));

    imshow("TEST", frame);
    if(waitKey(30000) == 0){
        exit(0);
    }
}

bool Core::start(){
    avm_mode = Mode::Auto_Mode;
    while(true){
        //listen to know what mode is select
        //listenIPC();//change mode selected by Stop_Mode if we want stop car or any things as this
        switch (avm_mode){
            case Mode::Stop_Mode:
                break;
            case Mode::Auto_Mode:
                autoVirtualMode(true);
                break;
            case Mode::Virtual_Mode:
                autoVirtualMode(false);
                break;
            case Mode::Manual_Mode:
                manualMode();
                break;
        }
        waitKey(100);
    }
    if(DEBUG){
        Mat frame(480, 480, CV_8UC3, Scalar(0));
        drawDebug(frame);
    }
}

bool Core::init(){
    manager = Manager::getInstance();
    manager->initArea(24);
    stateMachine._stateServer = State::NONE;
    stateMachine._stateIPC = State::NONE;
    stateMachine._stateCalibration = State::NONE;
    stateMachine._stateImageBlock = State::NONE;
    stateMachine._statePathfinding = State::NONE;
    stateMachine._stateTrajectory = State::NONE;
    changeMode=false;
    calibration_pt = Point2f(-1,-1);
    setMode(Mode::Stop_Mode);
}

Mode Core::getCurrentMode(){
    return avm_mode;
}

void Core::setMode(Mode _mode){
    avm_mode = _mode;
    manager->reset();
    stateMachine._stateImageBlock = State::NONE;
    stateMachine._statePathfinding = State::NONE;
    stateMachine._stateTrajectory = State::NONE;

    delete process;
    process= nullptr;
    delete trajectory;
    trajectory=nullptr;
    delete path;
    path = nullptr;
    
}

void Core::autoVirtualMode(bool is_auto){
    if(stateMachine._stateServer == State::NONE || stateMachine._stateServer == State::NO){
        stateMachine._stateServer = server();
    }else if(stateMachine._stateServer == State::ERROR){
        //TODO ERROR ecrit dans un fichier de log une erreur
        debug((char*)"THE SERVER COULD NOT START", true);
    }
/**/else if(stateMachine._stateServer == State::OK){
        debug((char*)"SERVER OK", true);
        if(stateMachine._stateCalibration == State::NONE){
            debug((char*)"CALIBRATION PROCESSING ...", true);
            calibration();
        }else if(stateMachine._stateCalibration == State::ERROR){
            //TODO NO CENTER OF AREA NO FOUND, turn arround car
            debug((char*)"CALIBRATION FAILED TIMEOUT(no QRcenter)", true);
        }
    /**/else if(stateMachine._stateCalibration == State::OK){
            debug((char*)"CALIBRATION OK", true);
            if(stateMachine._stateImageBlock == State::NONE){
                blockProcessing(is_auto);
            }else if(stateMachine._stateImageBlock == State::ERROR){
                //missing markers car & arrival
                debug((char*)"IMAGES PROCESSING FAILED (missing CAR or ARRIVAL)", true);
            }
        /**/else if(stateMachine._stateImageBlock == State::OK){
                debug((char*)"IMAGES BLOCK OK", true);
                if(stateMachine._statePathfinding == State::NONE){
                    pathfinding();
                }else if(stateMachine._statePathfinding == State::NO){
                    //TODO no possibility to found arrival
                    debug((char*)"NO POSSIBILITY PATHFINDING", true);
                }
            /**/else if(stateMachine._statePathfinding == State::OK){
                    debug((char*)"PATHFINDING OK", true);
                    if(stateMachine._stateTrajectory == State::NONE){
                        followTrajectory();
                    }else if(stateMachine._stateTrajectory == State::NO){
                        //
                        debug((char*)"CAR failed, jump pathfinding", true);
                        setMode(Mode::Auto_Mode);
                    }else if(stateMachine._stateTrajectory == State::ERROR){
                        //TODO Car is blocked or some things like this
                        debug((char*)"CAR is blocked", true);
                    }
                /**/else if(stateMachine._stateTrajectory == State::OK){
                        //TODO Car is on Arrival !!
                        setMode(Mode::Stop_Mode); //wait new command
                    }
                }
            }
        }
    }
}

void Core::manualMode(){
    //TODO just listen IPC to know if the mode has change
}


void Core::listenIPC(){
    if(avm_mode != Mode::Auto_Mode)
        setMode(Mode::Auto_Mode);
    return;
}

State Core::server(){
    //TODO do server function after all works. (connect to the server with IPC)
    return State::OK;
}

State Core::calibration(){
    if(process == nullptr)
        process = new ImagesP();
    process->calibration();
    calibration_pt = process->centerPoint;
    if(process->loadCalib() != Point2f(-1,-1)){
        stateMachine._stateCalibration = State::OK;
    }else{
        stateMachine._stateCalibration = State::ERROR;
    }
    return State::OK;
}

State Core::blockProcessing(bool is_auto){
    if(process == nullptr){
        process = new ImagesP();
        if(calibration_pt != Point2f(-1,-1)){
            process->centerPoint = calibration_pt;
        }
    }
    if(is_auto){
        debug((char*)"SEARCH BLOCK ... ", false);
        process->startBlock();
        for(int8_t i=0; i<process->all_block.size();++i){
            manager->block->add(process->all_block.at(i).x, process->all_block.at(i).y);
        }
        if(DEBUG){
             std::cout<<process->all_block.size()<<" FOUNDING"<<std::endl;
             std::cout<<"MARKERS "<<(int)process->getSizeMarker()<<" FOUNDING"<<std::endl;
        }
        manager->car->setPosition(process->getCarPosition().x, process->getCarPosition().y);
        manager->end->setPosition(process->getArrivalPosition().x, process->getArrivalPosition().y);
        manager->update();
        if(process->readyForPath()){
            stateMachine._stateImageBlock = State::OK;
        }
        else{
            stateMachine._stateImageBlock = State::ERROR;
        }
    }
    return State::OK;
}

State Core::pathfinding(){
    debug((char*)"STARTING PATHFINDING ...", false);
    path = new PathFinding(
        manager->getGeneralTable(), manager->getSceneSquare(), manager->getSceneSquare(), 
        manager->car->getPosition(), manager->end->getPosition()
    );
    if(!path->hasPossibility()){
        stateMachine._statePathfinding = State::NO;
        debug((char*)"NOT FOUND", true);
        return State::NO;
    }
    debug((char*)"FOUND", true);
    waitKey(100);
    std::vector<std::pair<int8_t, int8_t>> *pathTerminate = path->getPath();
    for(int index=0; index < (int)pathTerminate->size() - 1; ++index)
        manager->path->add(pathTerminate->at(index).first, pathTerminate->at(index).second);
    manager->update();
    stateMachine._statePathfinding = State::OK;
    return State::OK;
}


bool Core::forward(float angle){
    return false;
}

bool Core::rotation(float angle){
    return false;
}

State Core::followTrajectory(){
    trajectory = new Trajectory(path, process);
    stateMachine._stateTrajectory = (trajectory->start() ? State::OK : State::NO);

    return stateMachine._stateTrajectory;
}