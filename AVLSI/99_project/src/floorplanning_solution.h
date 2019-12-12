#ifndef _FP_SOLUTION_GUARD
#define _FP_SOLUTION_GUARD

#include <vector>
#include <utility>
#include "floorplan.h"

class Floorplanning_solution {
    int width, height;
    std::vector<std::pair<int, int>> positions, shapes;

    void calculate_positions (const Floorplan* fp, int shape_ix, std::pair<int, int> module_pos);


public:
    Floorplanning_solution(int size, const Floorplan *fp, int shape_ix);
    float cost() const;
    int get_width() const;
    int get_height() const;
    int get_area () const;
    
};


#endif