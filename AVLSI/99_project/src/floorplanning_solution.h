#ifndef _FP_SOLUTION_GUARD
#define _FP_SOLUTION_GUARD

#include <vector>
#include <utility>
#include "shape_function.h"

class Floorplanning_solution {
    int _width, _height;
    std::vector<std::pair<int, int>> _positions, _shapes;

    void calculate_positions (Shape_function* sf, size_t shape_ix, std::pair<int, int> module_pos);

public:
    Floorplanning_solution();
    Floorplanning_solution(Shape_function *sf, size_t size, size_t shape_ix);
    float cost() const;
    int get_width() const;
    int get_height() const;
    int get_area () const;

    void print() const;
    
};


#endif