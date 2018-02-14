#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "manager.h"
#include "traitement/images.h"
#include "traitement/pathfinding.h"

class Trajectory{
    
    public:

        /**
         * @file            trajectory.h
         * @brief           Move Car with a specific path
         * @version         1.0
         * 
         */
        Trajectory(PathFinding *_path, ImagesP *_images_process);
        bool start();
        void stop();
        ~Trajectory();
    private:
        PathFinding *pathfinding;
        ImagesP *process;
        void init();

};

#endif