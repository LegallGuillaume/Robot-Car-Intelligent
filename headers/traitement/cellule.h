#ifndef CELLULE_H
#define CELLULE_H

#include <utility>

class Cellule{
    
    public:

        /**
         * @file            cellule.h
         * @brief           Create Cellule for pathfinding "header/traitement/pathfinding.h"
         * @version         1.0
         * 
         * @param x         Position x into Matrix
         * @param y         Position y into Matrix
         * @param is_bloc   true if there is a block(wall or any others)
         */
        Cellule(int8_t x, int8_t y, bool is_bloc){
            m_coord.first = x;
            m_coord.second = y;
            m_P= 60000;
            m_bloc = is_bloc;
            is_center=false;
            everfind = false;
        }
        Cellule *m_parent;

        /**
         * @file            cellule.h
         * @brief           Allows to get position of the cellule
         * @version         1.0                              
         * 
         * @return std::pair<int8_t, int8_t>    return position into pair (x and y)
         */
        std::pair<int8_t, int8_t> getCoord(){
            return m_coord;
        }
        uint16_t m_P; /*Poid*/

        /**
         * @file            cellule.h
         * @brief           Check if there is a cellule block or not
         * @version         1.0                              
         * 
         * @return true     if the cellule is block
         * @return false    if the cellule is not a block
         */
        bool isBloc(){
            return m_bloc;
        }
        bool everfind;

        /**
         * @file            cellule.h
         * @brief           Destructor class
         * @version         1.0  
         * 
         */
        ~Cellule(){
            delete m_parent;
            m_parent = nullptr;
        }

    private:
        bool m_bloc;
        bool is_center;
        std::pair<int8_t, int8_t> m_coord;

};

#endif