#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <utility>
#include <vector>

class PathFinding{

public:
    //static std::vector<std::pair<int, int>> find();
    static std::vector<std::pair<int, int>> find();
private:
    static int remplissage(int width, int height, int **tab, int nb_points, int *listX, int *listY,int Bx, int By);
    static int test_case(int width, int height, int **tab, int x, int y);
    static bool arround(int value, int search, int x, int y, int width, int height);
};

#endif