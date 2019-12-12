#include <iostream>
#include "floorplanning_solution.h"
#include "npe.h"

using namespace std;

Floorplanning_solution::Floorplanning_solution(int size, const Floorplan *fp, int shape_ix): 
    width (fp->get_shape_width(shape_ix)),
    height (fp->get_shape_height(shape_ix)),
    positions(size),
    shapes(size)
{
    calculate_positions(fp, shape_ix, {0,0} );
}

int Floorplanning_solution::get_width() const { return width; }
int Floorplanning_solution::get_height() const {return height; }
int Floorplanning_solution::get_area () const { return width*height; }

void Floorplanning_solution::calculate_positions (const Floorplan* fp, int shape_ix, std::pair<int, int> module_pos) {
    const Floorplan *l_child = fp->get_left_child(), *r_child = fp->get_right_child();
    if (l_child == nullptr) {
        positions[fp->get_id()-1] = module_pos;
        shapes[fp->get_id()-1] = fp->get_shape(shape_ix);
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