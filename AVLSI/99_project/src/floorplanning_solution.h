/*########################################
 - File: floorplanning_solution.h
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/
#ifndef _FP_SOLUTION_GUARD
#define _FP_SOLUTION_GUARD

#include <vector>
#include <utility>
#include "shape_function.h"
#include "floorplanning_problem.h"

class Floorplanning_solution {
    int _width, _height, _min_area;
    std::vector<std::pair<int, int>> _positions, _shapes;
    float _alpha=0.8, _wirelength;

    void calculate_positions (Shape_function* sf, size_t shape_ix, std::pair<int, int> module_pos);
    void calculate_wirelength(const Floorplanning_problem *p);

public:
    Floorplanning_solution();
    Floorplanning_solution(Shape_function *sf, const Floorplanning_problem *p, size_t size, size_t shape_ix);
    float cost() const;
    int get_width() const;
    int get_height() const;
    int get_area () const;
    float get_wirelength () const;

    void print() const;
    
};


#endif