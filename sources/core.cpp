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
    if(manager->chemin->list_chemin != nullptr){
        for(int index=0; index < (int)manager->chemin->list_chemin->size() - 1; ++index){
            std::pair<int8_t, int8_t> _p = manager->chemin->list_chemin->at(index);
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

bool Core::start(){
    init();
    while(true){
        //listen to know what mode is select
        listenIPC();//change mode selected by Stop_Mode if we want stop car or any things as this
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
        if(stateMachine._statePathfinding == State::OK)
            break;
    }
    if(DEBUG){
        Mat frame(480, 480, CV_8UC3, Scalar(0));
        drawDebug(frame);
    }
}

bool Core::init(){
    manager = Manager::getInstance();
    manager->initScene(24);
    stateMachine._stateServer = State::NONE;
    stateMachine._stateIPC = State::NONE;
    stateMachine._stateCalibration = State::NONE;
    stateMachine._stateImageBlock = State::NONE;
    stateMachine._statePathfinding = State::NONE;
    stateMachine._stateTrajectory = State::NONE;
    changeMode=false;
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
    delete path;
}

void Core::autoVirtualMode(bool is_auto){
    if(stateMachine._stateServer == State::NONE || stateMachine._stateServer == State::NO){
        stateMachine._stateServer = server();
    }else if(stateMachine._stateServer == State::ERROR){
        //TODO ERROR ecrit dans un fichier de log une erreur
    }
/**/else if(stateMachine._stateServer == State::OK){
        if(DEBUG) std::cout<<"SERVER OK"<<std::endl;
        if(stateMachine._stateCalibration == State::NONE){
            calibration();
        }else if(stateMachine._stateCalibration == State::ERROR){
            //TODO NO CENTER OF AREA NO FOUND, turn arround car
            if(DEBUG) std::cout<<"CALIBRATION FAILED (no QRcenter)"<<std::endl;
        }
    /**/else if(stateMachine._stateCalibration == State::OK){
            if(DEBUG) std::cout<<"CALIBRATION OK"<<std::endl;
            if(stateMachine._stateImageBlock == State::NONE){
                blockProcessing(is_auto);
            }else if(stateMachine._stateImageBlock == State::ERROR){
                //TODO ERROR detect block > 3 (Arrival, Car and Area) and 0 case found (Block devide 9)
                if(DEBUG) std::cout<<"IMAGES PROCESSING FAILED (no case found 1 of 9x9)"<<std::endl;
            }
        /**/else if(stateMachine._stateImageBlock == State::OK){
                if(DEBUG) std::cout<<"IMAGES BLOCK OK"<<std::endl;
                if(stateMachine._statePathfinding == State::NONE){
                    pathfinding();
                }else if(stateMachine._statePathfinding == State::NO){
                    //TODO no possibility to found arrival
                    if(DEBUG) std::cout<<"NO POSSIBILITY PATHFINDING"<<std::endl;
                }
            /**/else if(stateMachine._statePathfinding == State::OK){
                    if(DEBUG) std::cout<<"PATHFINDING OK"<<std::endl;
                    if(stateMachine._stateTrajectory == State::NONE){
                        trajectory();
                    }else if(stateMachine._stateTrajectory == State::ERROR){
                        //TODO Car is blocked or some things like this
                        if(DEBUG) std::cout<<"CAR is blocked"<<std::endl;
                    }
                /**/else if(stateMachine._stateTrajectory == State::OK){
                        //TODO Car is on Arrival !!
                        setMode(Mode::Stop_Mode); //wait new 
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
    if(process->loadCalib() != Point2f(-1,-1)){
        stateMachine._stateCalibration = State::OK;
    }else{
        stateMachine._stateCalibration = State::ERROR;
    }
    return State::OK;
}

State Core::blockProcessing(bool is_auto){
    if(is_auto){
        if(DEBUG) std::cout<<"SEARCH BLOCK ... ";
        process->startBlock();
        for(int8_t i=0; i<process->all_block.size();++i){
            manager->bloc->add(process->all_block.at(i).x, process->all_block.at(i).y);
        }
        if(DEBUG) std::cout<<process->all_block.size()<<" FOUNDING"<<std::endl;
        manager->car->setPosition(process->getCarPosition().x, process->getCarPosition().y);
        manager->arrive->setPosition(process->getArrivalPosition().x, process->getArrivalPosition().y);
        manager->update();
        if((int)process->getSizeMarker() >= 2){
            stateMachine._stateImageBlock = State::OK;
        }
        else{
            stateMachine._stateImageBlock = State::ERROR;
        }
    }
    return State::OK;
}

State Core::pathfinding(){
    path = new PathFinding(
        manager->getGeneralTable(), manager->getSceneCarrer(), manager->getSceneCarrer(), 
        manager->car->getPosition(), manager->arrive->getPosition()
    );
    if(!path->hasPossibility()){
        stateMachine._statePathfinding = State::NO;
        return State::NO;
    }
    waitKey(100);
    std::vector<std::pair<int8_t, int8_t>> *cheminTerminate = path->getChemin();
    for(int index=0; index < (int)cheminTerminate->size() - 1; ++index)
        manager->chemin->add(cheminTerminate->at(index).first, cheminTerminate->at(index).second);
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

State Core::trajectory(){
    stateMachine._stateTrajectory = State::OK;



    return State::OK;
}