#ifndef CORE_H
#define CORE_H

#include "../headers/manager.h"
#include "../headers/traitement/pathfinding.h"
#include "../headers/traitement/images.h"

enum class State { NONE=0, OK, NO, ERROR }; //NO is for pathfinding or trajectory if car can't move

struct _stateMachine
{
    State _stateServer;
    State _stateIPC; //true receive message from IPC
    State _stateCalibration;
    State _stateImageBlock;
    State _statePathfinding;
    State _stateTrajectory;
};

enum class Mode { Stop_Mode=0, Auto_Mode, Virtual_Mode, Manual_Mode };

class Core{
    public:
        Core();
        ~Core();
        bool start();
        bool stop();
        bool reset();
        

    private:
        struct _stateMachine stateMachine;
        bool changeMode;
        Mode avm_mode; //Auto Virtual Manual mode
        
        Manager *manager;
        ImagesP *process;
        PathFinding *path;

        void drawDebug(Mat& frame);
        bool init();
        bool restartServer();
        void setMode(Mode _mode); //do
        Mode getCurrentMode(); //do

        void listenIPC();

        void autoVirtualMode(bool is_auto); //do
        void manualMode();

        /*function*/
        State calibration(); //do
        State blockProcessing(bool is_auto); //do
        State pathfinding(); //do
        State server(); //TODO
        State trajectory(); //TODO /*if change mode, break while and return State::ERROR*/

        /*trajectory function*/
        bool forward(float angle); /*return false if the car is out of path*/
        bool rotation(float angle); /*return true if rotation is success +- 3° */

};

#endif //CORE_H