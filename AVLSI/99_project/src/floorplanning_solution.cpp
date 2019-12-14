#include <iostream>
#include "floorplanning_solution.h"
#include "npe.h"

using std::cerr;
using std::endl;
using std::pair;
using std::cout;

Floorplanning_solution::Floorplanning_solution( const Shape_function *sf, size_t size, size_t shape_ix): 
    _width (sf->get_shape(shape_ix).first),
    _height (sf->get_shape(shape_ix).second),
    _positions(size),
    _shapes(size)
{
    calculate_positions(sf, shape_ix, {0,0});
}

int Floorplanning_solution::get_width() const { return _width; }
int Floorplanning_solution::get_height() const {return _height; }
int Floorplanning_solution::get_area () const { return _width*_height; }

void Floorplanning_solution::calculate_positions (const Shape_function* sf, size_t shape_ix, std::pair<int, int> module_pos) {
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

float Floorplanning_solution::cost() const {
    return get_area();
}

void Floorplanning_solution::print() const {
    cout << _positions.size() << endl;
    cout << _width << " " << _height << endl;
    for (size_t i=0; i<_positions.size();++i) {
        cout << _positions[i].first << " " << _positions[i].second << " ";
        cout << _shapes[i].first << " " << _shapes[i].second << endl;
    }
}