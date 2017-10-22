#ifndef CELLULE_PATHFINDING_H
#define CELLULE_PATHFINDING_H

#include <utility>

class Cellule{
public:
    Cellule(int8_t x, int8_t y, bool is_bloc){ /*(uint8_t*uint8_t)*/
        m_coord.first = x;
        m_coord.second = y;
        m_P= 60000;
        m_bloc = is_bloc;
    }
    Cellule *m_parent;
    std::pair<int8_t, int8_t> getCoord(){return m_coord;}
    uint16_t m_P; /*Poid*/
    void setBloc(bool b){m_bloc = b;}
    bool isBloc(){return m_bloc;}
private:
    bool m_bloc;
    std::pair<int8_t, int8_t> m_coord;

};

#endif