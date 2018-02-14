#ifndef PATHFINDING_H
#define PATHFINDING_H


#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "cell.h"
#include "../definition.h"
#include "../manager.h"

class PathFinding{

public:

    /**
     * @file                pathfinding.h
     * @brief               Search the best way to go to Arrival
     * @version             1.0
     * 
     * @param matrix       Matrix do not exceed 127x127 (contains blocks)
     * @param width_scene   Width of Matrix
     * @param height_scene  Height of Matrix
     * @param start         pair<int8_t,int8_t> position of start (x and y)
     * @param end           pair<int8_t,int8_t> position of Arrival (x and y)
     */
    PathFinding(int8_t **matrix, int8_t width_scene, /*matrix 0 to 127*/
        int8_t height_scene, std::pair<int8_t,int8_t> start, std::pair<int8_t,int8_t> end){
            start_coord = start;
            end_coord = end;
            width = width_scene;
            height = height_scene;
            _pathfinding = nullptr;
            for(int8_t i_width = 0; i_width < width_scene; ++i_width){
                for(int8_t i_height= 0; i_height < height_scene; ++i_height){
                    bool isblock = (matrix[i_width][i_height] == BLOC_DEF);
                    all_cells.push_back(new Cell(i_width, i_height, isblock));
                }
            }
            Cell *firstCell = getCell(start.first, start.second);
            firstCell->m_P = 0;
            firstCell->everfind = true;
            current_list.push_back(firstCell);
            findPath();
            list_pathOk = new std::vector<std::pair<int8_t, int8_t>>();
            list_pathOk->reserve(width_scene*(width_scene/2));
        }

    bool everParent(Cell *current, Cell *parent){
        Cell *_current = current;
        if(current->m_parent == nullptr)
            return false;
        uint16_t maxsize = 3;
        while(_current != nullptr){
            maxsize--;
            if(_current == parent){
                return true;
            }else{
                _current = _current->m_parent;
            }
            if(maxsize <= 0)
                break;
        }
        return false;
    }
    /**
     * @file                pathfinding.h
     * @brief               Check if there are a solution
     * @version             1.0
     * 
     * @return bool         true if there are a Way to go to Arrival
     */
    bool hasPossibility(){
        return _pathfinding != nullptr;
    }

    /**
     * @file                pathfinding.h
     * @brief               return Way to go to Arrival
     * @version             1.0
     * 
     * @return std::vector<std::pair<int8_t, int8_t>>*  return vector to pair (position of way, first order Arrival)
     */
    std::vector<std::pair<int8_t, int8_t>>* getPath(){
        list_pathOk->clear();
        Cell *current = _pathfinding;
        while(true){
            if(current == nullptr)
                break;
            if(current->getCoord() != start_coord)
                list_pathOk->push_back(current->getCoord());
            if(current->m_parent != nullptr && !current->m_parent->everfind){
                current = current->m_parent;
                current->everfind = true;
            }else{
                break;
            }
        }
        if(current != nullptr)
            list_pathOk->push_back(current->getCoord());
        if(current->m_parent != nullptr) /*first 1/3block after start block*/
            list_pathOk->push_back(current->m_parent->getCoord());
        return list_pathOk;
    }

    /**
     * @file                pathfinding.h
     * @brief               Destructor to the class Pathfinding
     * @version             1.0 
     * 
     */
    ~PathFinding(){
        free(_pathfinding);
        _pathfinding = nullptr;
        list_pathOk->clear();
        free(list_pathOk);
        list_pathOk = nullptr;
        all_cells.clear();
        current_list.clear();
        
    }
private:
    Cell *_pathfinding;
    std::vector<std::pair<int8_t,int8_t>> *list_pathOk;

    /**
     * @file                pathfinding.h
     * @brief               Find Path
     * @version             1.0
     * 
     * recusive function
     * 
     * +---------------+---------------+---------------+
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * |   0   0   0   |  A2   B1  C1  |   0   0   0   |
     * +---------------+---------------+---------------+
     * |   0   0   A1  |   A   B   C   |   C2   0   0  |
     * |   0   0   D1  |   D   E   F   |   F1   0   0  |
     * |   0   0   G2  |   G   H   I   |   I2   0   0  |
     * +---------------+---------------+---------------+
     * |   0   0   0   |  G1   H1  I1  |   0   0   0   |
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * +---------------+---------------+---------------+
    */
    void findPath(){
        _pathfinding = nullptr;
        if(current_list.size() == 0)
            return;
        std::vector<Cell*> actuel_list = current_list;
        current_list.clear();
        for(uint8_t index=0; index<(uint8_t)actuel_list.size(); ++index){
            Cell* _currentCell = actuel_list.at(index);
            uint16_t newP = _currentCell->m_P+10;
            Cell* down = getCell(_currentCell->getCoord().first + 1, _currentCell->getCoord().second);
            Cell* up = getCell(_currentCell->getCoord().first - 1, _currentCell->getCoord().second);
            Cell* right = getCell(_currentCell->getCoord().first, _currentCell->getCoord().second + 1);
            Cell* left = getCell(_currentCell->getCoord().first, _currentCell->getCoord().second - 1);
            //
            Cell* a1 = getCell(_currentCell->getCoord().first - 1, _currentCell->getCoord().second - 2);
            Cell* d1 = getCell(_currentCell->getCoord().first, _currentCell->getCoord().second - 2);
            Cell* g2 = getCell(_currentCell->getCoord().first + 1, _currentCell->getCoord().second - 2);
            Cell* g1 = getCell(_currentCell->getCoord().first + 2, _currentCell->getCoord().second -1);
            Cell* h1 = getCell(_currentCell->getCoord().first + 2, _currentCell->getCoord().second);
            Cell* i1 = getCell(_currentCell->getCoord().first + 2, _currentCell->getCoord().second + 1);
            Cell* i2 = getCell(_currentCell->getCoord().first + 1, _currentCell->getCoord().second + 2);
            Cell* f1 = getCell(_currentCell->getCoord().first, _currentCell->getCoord().second + 2);
            Cell* c2 = getCell(_currentCell->getCoord().first - 1, _currentCell->getCoord().second + 2);
            Cell* c1 = getCell(_currentCell->getCoord().first - 2, _currentCell->getCoord().second +1);
            Cell* b1 = getCell(_currentCell->getCoord().first - 2, _currentCell->getCoord().second);
            Cell* a2 = getCell(_currentCell->getCoord().first - 2, _currentCell->getCoord().second -1);

            if(g1 != nullptr && h1 != nullptr && i1 != nullptr && down->m_parent != _currentCell){
                if(!g1->isBlock() && !h1->isBlock() && !i1->isBlock()){
                    if(newP < down->m_P && !everParent(_currentCell->m_parent, down)){
                        down->m_parent = _currentCell;
                        down->m_P = newP;
                        current_list.push_back(down);
                    }
                    if(down->getCoord().first == end_coord.first && down->getCoord().second == end_coord.second){
                        _pathfinding = _currentCell;
                        break;
                    }
                }
            }
            if(a2 != nullptr && b1 != nullptr && c1 != nullptr && up->m_parent != _currentCell){
                if(!a2->isBlock() && !b1->isBlock() && !c1->isBlock()){
                    if(newP < up->m_P && !everParent(_currentCell, up)){
                        up->m_parent = _currentCell;
                        up->m_P = newP;
                        current_list.push_back(up);
                    }
                }
                if(up->getCoord().first == end_coord.first && up->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(a1 != nullptr && d1 != nullptr && g2 != nullptr && left->m_parent != _currentCell){
                if(!a1->isBlock() && !d1->isBlock() && !g2->isBlock()){
                    if(newP < left->m_P && !everParent(_currentCell->m_parent, left)){
                        left->m_parent = _currentCell;
                        left->m_P = newP;
                        current_list.push_back(left);
                    }
                }
                if(left->getCoord().first == end_coord.first && left->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(c2 != nullptr && f1 !=nullptr && i2 != nullptr && right->m_parent != _currentCell){
                if(!c2->isBlock() && !f1->isBlock() && !i2->isBlock()){
                    if(newP < right->m_P && !everParent(_currentCell->m_parent, right)){
                        right->m_parent = _currentCell;
                        right->m_P = newP;
                        current_list.push_back(right);
                    }
                }
                if(right->getCoord().first == end_coord.first && right->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
        }
        if(_pathfinding == nullptr){
            findPath();
        }
        return;
    }

    /**   
     * @file                pathfinding.h
     * @brief               Get Cell from position X and position Y
     * @version             1.0
     * 
     * @param x             Postion X into Matrix
     * @param y             Position Y into Matrix
     * @return Cell*     return the Cell choosen
     */
    Cell* getCell(int8_t x, int8_t y){
        if(x < 0 || y < 0)
            return nullptr;
        if(x >= width || y >= height)
            return nullptr;
        uint16_t index = (height*x) + y;
        if(all_cells.size() > index)
            return all_cells.at(index);
        return nullptr;
    }

    /**
     * @file                pathfinding.h
     * @brief               Return true if the Arrival or Start surrounded Blockks
     * @version             1.0
     * 
     * @param x             Position X into Matrix
     * @param y             Position Y into Matrix
     * @param matrix       Matrix contains all blocks
     * @return true         if Arrival or Start is surrounded by block 
     * @return false        if not surrounded by block
     */
    bool surroundedBlocs(int8_t x, int8_t y, int8_t** matrix){
        if(x == 0){
            if(matrix[x+1][y] == 0)
                return false;
        }
        if(y == 0){
            if(matrix[x][y+1] == 0)
                return false;
        }
        if(0 < x && x<width-1){
            if(matrix[x+1][y] == 0 || matrix[x-1][y] == 0)
                return false;
        }
        if(0 < y && y<height-1){
            if(matrix[x][y+1] == 0 || matrix[x][y-1] == 0)
                return false;
        }
        if(x == width-1){
            if(matrix[x-1][y] == 0)
                return false;
        }
        if(y == height-1){
            if(matrix[x][y-1] == 0)
                return false;
        }
        return true;
    }

    std::vector<Cell*> all_cells;
    std::vector<Cell*> current_list;
    int8_t width;
    int8_t height;
    std::pair<int8_t, int8_t> start_coord;
    std::pair<int8_t, int8_t> end_coord;

};

#endif