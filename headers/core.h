#ifndef CORE_H
#define CORE_H

#include "../headers/manager.h"
#include "../headers/traitement/pathfinding.h"
#include "../headers/traitement/images.h"
#include "../headers/trajectory.h"


/**
 * @file            core.h
 * @brief           State of State Machine
 * @version         1.0
*/
enum class State { 
    NONE=0, /*!< Default state */
    OK,  /*!< all these well unrolled */
    NO,  /*!< is for pathfinding or trajectory if car can't move */
    ERROR  /*!< there are an error */
}; 


/**
 * @file            core.h
 * @brief           State Machine 
 * @version         1.0
*/
struct _stateMachine
{
    State _stateServer; /*!< State of server */
    State _stateIPC; /*!< Receive or not the message from shared memory */
    State _stateCalibration; /*!< State of calibration */
    State _stateImageBlock; /*!< State of processing Images */
    State _statePathfinding; /*!< State of pathfinding */
    State _stateTrajectory; /*!< State of success trajectory */
};

/**
 * @file            core.h
 * @brief           Operating Mode 
 * @version         1.0
*/
enum class Mode { 
    Stop_Mode=0, /*!< Default Mode */
    Auto_Mode, /*!< Automatic Mode */
    Virtual_Mode, /*!< Virtual Mode */
    Manual_Mode /*!< Manual Mode */
};

class Core{
    public:
        /**
         * @file                core.h
         * @brief               constructor of the class
         * @function            Constructor Class Core
         * @version             1.0    
        */
        Core();

        /**
         * @file                core.h
         * @brief               destruction of the class
         * @function            Desctructor Class Core
         * @version             1.0      
        */
        ~Core();

        /**
         * @file                core.h
         * @brief               start the programme
         * @version             1.0
         *
         * @return true      if started
         * @return false     if not started       
        */
        bool start();

        /**
         * @file                core.h
         * @brief               stop the programme
         * @version             1.0
         *
         * @return true      if stopped
         * @return false     if not stopped       
        */
        bool stop();

        /**
         * @file                core.h
         * @brief               reset the programme
         * @version             1.0
         *
         * @return true      if reseted
         * @return false     if not reseted     
        */
        bool reset();
        

    private:
        struct _stateMachine stateMachine;
        bool changeMode;
        Mode avm_mode; //Auto Virtual Manual mode
        
        Manager *manager;
        ImagesP *process;
        PathFinding *path;
        Trajectory *trajectory;
        Point2f calibration_pt;

        /**
         * @file                core.h
         * @brief               display the abstract matrix with pathinfing, block and car
         * @version             1.0
         *
         * @param Mat& frame    frame to display this matrix 
         *
        */
        void drawDebug(Mat& frame);

        /**
         * @file                core.h
         * @brief               reset the programme
         * @version             1.0
         * @function            call in Constructor
         *
         * @return true      if initialized
         * @return false     if not initialized     
        */
        bool init();

        /**
         * @file                core.h
         * @brief               restart the server
         * @version             1.0
         *
         * @return true      if restarted server
         * @return false     if not restarted     
        */
        bool restartServer();

        /**
         * @file                core.h
         * @brief               change mode automatic manual virtual or stop 
         * @version             1.0
         *
         * @param Mode _mode    choose by enum Mode
        */
        void setMode(Mode _mode); //do


        /**
         * @file                core.h
         * @brief               get Mode selectionned
         * @version             1.0
         *
         * @return Mode      choosen mode  
        */
        Mode getCurrentMode(); //do

        /**
         * @file                core.h
         * @brief               listen shared memory with server
         * @version             1.0
        */
        void listenIPC();

        /**
         * @file                core.h
         * @brief               start automatic or virtual fonctionment
         * @version             1.0
         *
         * @param boolean is_auto   true if auto, false if virtual
        */
        void autoVirtualMode(bool is_auto);

        /**
         * @file                core.h
         * @brief               start manual fonctionment
         * @version             1.0  
        */
        void manualMode();

        /*function*/

        /**
         * @file                core.h
         * @brief               start calibration
         * @version             1.0
         *
         * @return State    OK = success, NO = markers area not found   
        */
        State calibration(); //do

        /**
         * @file                core.h
         * @brief               Find Markers
         * @version             1.0
         *
         * @param boolean is_auto  if true find markers in area, if false markers is given by application
         *
         * @return State    OK = success  
        */
        State blockProcessing(bool is_auto); //do
        
        /**
         * @file                core.h
         * @brief               find pathfinding
         * @version             1.0
         *
         * @return State    OK = have path , NO = have no path   
        */
        State pathfinding(); //do

        /**
         * @file                core.h
         * @brief               start server
         * @version             1.0
         *
         * @return State    OK = started, NO = not stated, ERROR = communication or others  
        */
        State server(); //TODO

        /**
         * @file                core.h
         * @brief               start moving
         * @version             1.0
         *
         * @return State    OK = finished path, NO =  not finished, ERROR = changed mode   
        */
        State followTrajectory(); //TODO /*if change mode, break while and return State::ERROR*/

        /*trajectory function*/
        bool forward(float angle); /*return false if the car is out of path*/
        bool rotation(float angle); /*return true if rotation is success +- 3° */

};

#endif //CORE_H