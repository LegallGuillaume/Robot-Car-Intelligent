#ifndef CELL_H
#define CELL_H

#include <utility>

class Cell{
    
    public:

        /**
         * @file            cell.h
         * @brief           Create cell for pathfinding "header/traitement/pathfinding.h"
         * @version         1.0
         * 
         * @param x         Position x into Matrix
         * @param y         Position y into Matrix
         * @param is_block   true if there is a block(wall or any others)
         */
        Cell(int8_t x, int8_t y, bool is_block){
            m_coord.first = x;
            m_coord.second = y;
            m_P= 60000;
            m_block = is_block;
            is_center=false;
            everfind = false;
        }
        Cell *m_parent;

        /**
         * @file            cell.h
         * @brief           Allows to get position of the cell
         * @version         1.0                              
         * 
         * @return std::pair<int8_t, int8_t>    return position into pair (x and y)
         */
        std::pair<int8_t, int8_t> getCoord(){
            return m_coord;
        }
        uint16_t m_P; /*Poid*/

        /**
         * @file            cell.h
         * @brief           Check if there is a cell block or not
         * @version         1.0                              
         * 
         * @return true     if the cell is block
         * @return false    if the cell is not a block
         */
        bool isBlock(){
            return m_block;
        }
        bool everfind;

        /**
         * @file            cell.h
         * @brief           Destructor class
         * @version         1.0  
         * 
         */
        ~Cell(){
            delete m_parent;
            m_parent = nullptr;
        }

    private:
        bool m_block;
        bool is_center;
        std::pair<int8_t, int8_t> m_coord;

};

#endif