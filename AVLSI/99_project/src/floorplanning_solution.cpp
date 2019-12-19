/*########################################
 - File: floorplanning_solution.cpp
 - Author: Francisco Javier Jurado Moreno
 - Project: AVLSI Floorplanning project
##########################################*/

#include <iostream>
#include <cmath>
#include "floorplanning_solution.h"
#include "npe.h"

using std::cerr;
using std::endl;
using std::pair;
using std::cout;

Floorplanning_solution::Floorplanning_solution(): _width(__INT_MAX__), _height(__INT_MAX__){};

Floorplanning_solution::Floorplanning_solution( Shape_function *sf, const Floorplanning_problem *p, size_t size, size_t shape_ix): 
    _width (sf->get_shape(shape_ix).first),
    _height (sf->get_shape(shape_ix).second),
    _min_area (sf->get_min_area()),
    _positions(size),
    _shapes(size)
{
    calculate_positions(sf, shape_ix, {0,0});
    calculate_wirelength(p);
}

int Floorplanning_solution::get_width() const { return _width; }
int Floorplanning_solution::get_height() const {return _height; }
int Floorplanning_solution::get_area () const { return _width*_height; }
float Floorplanning_solution::get_wirelength () const { return _wirelength; }

// Calculate the positions by performing an inorder traversal on the slicing tree and propagating the position of the parent to the children.
// It assumes that the left child is placed to the left or above the right child for vertical and horizontal cuts, respectively.
void Floorplanning_solution::calculate_positions (Shape_function* sf, size_t shape_ix, std::pair<int, int> module_pos) {
    if ( sf->get_id() > 0 ) {
        _positions[sf->get_id()-1] = module_pos;
        _shapes[sf->get_id()-1] = sf->get_shape(shape_ix);
    } else if (sf->get_id() == NPE::H || sf->get_id() == NPE::V ) {
        calculate_positions( sf->get_left_child(), sf->get_child_sf_ix(shape_ix).first, module_pos );
        pair<int, int> l_child_shape = sf->get_left_child()->get_shape(sf->get_child_sf_ix(shape_ix).first);
        calculate_positions(
            sf->get_right_child(),
            sf->get_child_sf_ix(shape_ix).second,
            (sf->get_id() == NPE::H) ?
                std::make_pair( module_pos.first, l_child_shape.second + module_pos.second ) :
                std::make_pair( l_child_shape.first + module_pos.first, module_pos.second));
    } else {
        cerr << "Error [Solution]: Malformed Slicing tree node" << endl;
        exit(EXIT_FAILURE);
    }
}

// Calculate wirelength by iterating over the list of connections
void Floorplanning_solution::calculate_wirelength(const Floorplanning_problem *p) {
    pair<int, int> p1, p2;
    float cx_1, cx_2, cy_1, cy_2;
    _wirelength=0;
    for(size_t i=1; i<=_positions.size(); ++i) {
        if (p->module_has_connections(i)) {
            p1 = _positions[i-1];
            cx_1 = p1.first + _shapes[i-1].first/2.0;
            cy_1 = p1.second + _shapes[i-1].second/2.0;
            for (pair<size_t, int> con : *(p->get_module_connections(i))) {
                p2 = _positions[con.first-1];
                cx_2 = p2.first + _shapes[con.first-1].first/2.0;
                cy_2 = p2.second + _shapes[con.first-1].second/2.0;
                _wirelength += std::sqrt(pow(cx_1 - cx_2,2) + pow(cy_1 - cy_2,2))*con.second; //HPWL
            }
        }
    }
}

float Floorplanning_solution::cost() const { return _alpha*get_area() + (1-_alpha)*_wirelength; }

void Floorplanning_solution::print() const {
    cout << _positions.size() << endl;
    cout << _width << " " << _height << endl;
    for (size_t i=0; i<_positions.size();++i) {
        cout << _positions[i].first << " " << _positions[i].second << " ";
        cout << _shapes[i].first << " " << _shapes[i].second << endl;
    }
}