#include "../headers/trajectory.h"
#include <unistd.h>


Trajectory::Trajectory(PathFinding *_path, ImagesP *_images_process){
   process =_images_process;
   pathfinding = _path;
}

Trajectory::~Trajectory(){}

bool Trajectory::start(){
    std::vector<std::pair<int8_t, int8_t>> *pathTerminate = Manager::getInstance()->path->list_path;
    std::pair<int8_t, int8_t> last_pos = std::make_pair(process->posCar.x, process->posCar.y);
    pathTerminate->insert(pathTerminate->begin(), Manager::getInstance()->end->getPosition());
    for(int i = pathTerminate->size()-2; i>=0; i--){
        if(last_pos.first == pathTerminate->at(i).first && last_pos.second == pathTerminate->at(i).second+1 ){
            std::cout<<"FORWARD LEFT  -> "<<std::endl;
        }
        else if(last_pos.first == pathTerminate->at(i).first && last_pos.second == pathTerminate->at(i).second-1 ){
            std::cout<<"FORWARD RIGHT  -> "<<std::endl;
        }
        else if(last_pos.first == pathTerminate->at(i).first+1 && last_pos.second == pathTerminate->at(i).second ){
            std::cout<<"FORWARD UP  -> "<<std::endl;
        }
        else if(last_pos.first == pathTerminate->at(i).first-1 && last_pos.second == pathTerminate->at(i).second ){
            std::cout<<"FORWARD DOWN  -> "<<std::endl;
        }else{
            std::cout<<"NOP  -> "<<std::endl;
        }
        std::pair<int8_t, int8_t> car_tmp = std::make_pair(process->posCar.x, process->posCar.y);
        while(last_pos == car_tmp){
            car_tmp = std::make_pair(process->posCar.x, process->posCar.y);
            process->trackingCar();
            usleep(70000);
        }
        last_pos = car_tmp;
        if(last_pos == pathTerminate->at(i))
            std::cout<<"    -> OK"<<std::endl;
        else{
            return false;
        }
    }
    
    return true;
}