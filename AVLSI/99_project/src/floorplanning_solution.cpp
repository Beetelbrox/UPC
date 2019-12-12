#include <iostream>
#include "floorplanning_solution.h"
#include "npe.h"

using namespace std;

Floorplanning_solution::Floorplanning_solution(int size, const Floorplan *fp, int shape_ix): 
    _width (fp->get_shape_width(shape_ix)),
    _height (fp->get_shape_height(shape_ix)),
    _positions(size),
    _shapes(size)
{
    calculate_positions(fp, shape_ix, {0,0} );
}

int Floorplanning_solution::get_width() const { return _width; }
int Floorplanning_solution::get_height() const {return _height; }
int Floorplanning_solution::get_area () const { return _width*_height; }

void Floorplanning_solution::calculate_positions (const Floorplan* fp, int shape_ix, std::pair<int, int> module_pos) {
    const Floorplan *l_child = fp->get_left_child(), *r_child = fp->get_right_child();
    if (l_child == nullptr) {
        _positions[fp->get_id()-1] = module_pos;
        _shapes[fp->get_id()-1] = fp->get_shape(shape_ix);
    } else {
        calculate_positions( l_child, fp->get_subfp_ix(shape_ix).first, module_pos );
        pair<int, int> sm_shape = l_child->get_shape(fp->get_subfp_ix(shape_ix).first);
        calculate_positions(r_child,
            fp->get_subfp_ix(shape_ix).second,
            (fp->get_id() == NPE::H) ?
                make_pair(module_pos.first, module_pos.second + sm_shape.second) :
                make_pair(module_pos.first + sm_shape.first, module_pos.second));
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