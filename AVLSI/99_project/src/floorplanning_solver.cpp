#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <stack>
#include <numeric>

#include "floorplanning_solver.h"
#include "floorplan.h"

using namespace std;

Floorplanning_solver::Floorplanning_solver(const Floorplanning_problem &p): _problem(p) {
  generate_random_npe(_problem.size(), _current_npe, 1);
  pack_npe();
}

int Floorplanning_solver::solve() {
  /*
  for(int i=0; i <1000; ++i) {
    pair<int, int> perturbation = npe->gen_rnd_perturbation();
    //std::cout << "(" << perturbation.first << "," << perturbation.second << ")" << std::endl;
    npe->apply_perturbation(perturbation);
    //npe->print();
  }
  */
  //pack_npe();
  //calculate_floorplan_dimensions(solutions.back().get_npe());
  return 0;
}

//  std::mt19937{std::random_device{}()} For the RNG

void Floorplanning_solver::generate_random_npe(size_t n_operands, NPE& npe, bool shuffle) {
  cerr << "Generating random solution...";
  size_t npe_size = 2*n_operands-1;
  // Allocate the arrays in the heap to avoid stack overflows
  unique_ptr<int[]> npe_seq = make_unique<int[]>(npe_size), operands = make_unique<int[]>(n_operands);
  int *op_it = operands.get(), next_insert;

  // Create a sequence of indices and shuffle it if requested
  iota(op_it, &op_it[n_operands], 1);
  if(shuffle) random_shuffle(op_it, &op_it[n_operands]);

  for(int* npe_it = npe_seq.get(); npe_it != &npe_seq[npe_size]; ++npe_it) {
    if (op_it-operands.get() >= int(n_operands)) next_insert = 1;       // All operands inserted
    else if ( npe_it - npe_seq.get() + 1 >= 2*(op_it-operands.get()) ) next_insert = 0;  // Balloting rule
    else next_insert = rand()%2;

    // 1 insert operator, 0 insert operand
    if(next_insert) *npe_it = *(npe_it-1) < 0 ? !(*(npe_it-1) + 2) - 2 : (rand()%2)-2;
    else *npe_it =  *(op_it++);
  }
  npe = NPE(npe_seq.get(), npe_size);
  cerr << "done." << endl;
}

void calculate_positions (const Floorplan* fp, int shape_ix, pair<int, int> module_pos, vector<pair<int, int>> &positions, vector<pair<int, int>> &shapes) {
  const Floorplan *l_child = fp->get_left_child(), *r_child = fp->get_right_child();
  if (l_child == nullptr) {
    positions[fp->get_id()-1] = module_pos; // Hit a leaf
    shapes[fp->get_id()-1] = fp->get_shape(shape_ix);
  }
  else {

  calculate_positions(l_child,
                      fp->get_subfp_ix(shape_ix).first,
                      module_pos,
                      positions, shapes);

  pair<int, int> sm_shape = l_child->get_shape(fp->get_subfp_ix(shape_ix).first);
  //cerr << sm_shape.first << " " << sm_shape.second << endl;
  calculate_positions(r_child,
                      fp->get_subfp_ix(shape_ix).second,
                      (fp->get_id() == NPE::H) ? make_pair(module_pos.first, module_pos.second + sm_shape.second) : make_pair(module_pos.first + sm_shape.first, module_pos.second),
                      positions, shapes);
  }
}

pair<int, float> Floorplanning_solver::calculate_cost(pair<int, int> fp_dim, const vector<pair<int, int>> &positions, const vector<pair<int, int>> &shapes) {
  return {fp_dim.first*fp_dim.second, 0.0};
}

// The NPE object ensures correctness of the member NPE, so we don't have to check
pair<int, int> Floorplanning_solver::pack_npe() {
  stack<const Floorplan *> fp_stack;
  stack<pair<int, int>> pos_stack;
  vector<Floorplan> intermediate_fps(_problem.size()-1); // Structure to store intermediate floorplans
  const Floorplan *l_child, *r_child;
  Floorplan *fp_ptr = &*intermediate_fps.begin();

  _current_npe.print();
  for(int* npe_it = _current_npe.begin(); npe_it != _current_npe.end(); ++npe_it) {
    if ( *npe_it > 0 ) fp_stack.push(_problem.get_floorplan( *npe_it ) ); // get_floorplan takes an id >= 1
    else {
      r_child = fp_stack.top(); fp_stack.pop(); // Remember that popping from a stack returns items in reverse indtroduction order
      l_child = fp_stack.top(); fp_stack.pop();
      pack_floorplans(*npe_it, l_child, r_child, fp_ptr);
      fp_ptr->print();
      fp_stack.push(fp_ptr++);
    }
  }

  vector<pair<int, int>> positions(_current_npe.n_operands()), shapes(_current_npe.n_operands());
  pair<int, int> shape;
  pair<int, float> cost;
  int best = -1;
  float best_cost=INFINITY;

  cerr << "Positions: " << endl;
  pos_stack.emplace(0,0);
  for(size_t i=0; i < fp_stack.top()->size(); ++i) {
    shape = fp_stack.top()->get_shape(i);
    calculate_positions(fp_stack.top(), i, {0,0}, positions, shapes);
    cost = calculate_cost(shape, positions, shapes);
    if (cost.first < best_cost) {
      best = i;
      best_cost = cost.first;
    }
  }

  cerr << best << " " << best_cost << endl;
  return {0,0};
}



int Floorplanning_solver::pack_floorplans(int op, const Floorplan* fp_1, const Floorplan* fp_2, Floorplan* fp_packed) {
  if(!fp_1->size() || !fp_2->size()) return -1; // If any of the floorplans is empty there's nothing to pack so return (malformed problem)
  // From now on we know that both floorplans have at least size 1
  vector<pair<int, int>> new_sf, subfp_ix;
  size_t ix_1=0, ix_2=0;
  
  if ( op == NPE::H ) {
    while( ix_1 < fp_1->size() || ix_2 < fp_2->size() )
      if ( fp_1->get_shape_width(ix_1) <= fp_2->get_shape_width(ix_2) ) {
        if (fp_1->get_shape_width(ix_1) > fp_2->get_shape_width(0) ){  // If this shape is eliminated by the algorithm, skip
          new_sf.emplace_back(fp_1->get_shape_width(ix_1), fp_1->get_shape_height(ix_1) + fp_2->get_shape_height(ix_2-1));
          subfp_ix.emplace_back(ix_1, ix_2-1);
        }
        ++ix_1;
      } else {
        if (fp_2->get_shape_width(ix_2) >= fp_1->get_shape_width(0) ){
          new_sf.emplace_back(fp_2->get_shape_width(ix_2), fp_1->get_shape_height(ix_1-1) + fp_2->get_shape_height(ix_2));
          subfp_ix.emplace_back(ix_1-1, ix_2);
        }
        ++ix_2;
      }
  } else if ( op == NPE::V ) {
    // Find out which floorplan has the largest h
    while(ix_1 < fp_1->size() && ix_2 < fp_2->size()) {
      if ( fp_1->get_shape_height(ix_1) >= fp_2->get_shape_height(ix_2) ) {
        new_sf.emplace_back(fp_1->get_shape_width(ix_1) + fp_2->get_shape_width(ix_2), fp_1->get_shape_height(ix_1));
        subfp_ix.emplace_back(ix_1, ix_2);
        if ( fp_1->get_shape_height(ix_1) == fp_2->get_shape_height(ix_2) ) ++ix_1;
        ++ix_1;
      } else {
        new_sf.emplace_back(fp_1->get_shape_width(ix_1) + fp_2->get_shape_width(ix_2), fp_2->get_shape_height(ix_2));
        subfp_ix.emplace_back(ix_1, ix_2);
        ++ix_2;
      }
    }
  } else cerr << "This state should not be reached" << endl; // Panic
  *fp_packed = Floorplan(op, new_sf,subfp_ix, fp_1, fp_2);
  return -!fp_packed->size();
  return 0;
}

/*


// If parse=false, it DOES NOT CHECK CORRECTNESS


//TODO do we need an assignment operator?

pair<int, int> NPE::gen_rnd_perturbation() {
  while(true) { // We will exit this loop by returning
    switch(rand()%3){
      case 0: //swap two adjacent operands
        return gen_rnd_operand_swap();
      case 1:
        return gen_rnd_chain_swap();
      case 2:
        pair<int, int> swap = gen_rnd_operand_operator_swap();
        if (swap.first >= 0) return swap;
    }
  }
}

// Assumes correctness of the input
pair<int, int> NPE::gen_rnd_operand_swap() {
  int rand_operand = rand()%(num_operands-1); // Choose a number UAR between the fist and the last
  return {operand_pos[rand_operand], operand_pos[rand_operand+1]};
}

// Returns a pair of <index, -1> to indicate the chain swap
pair<int, int>  NPE::gen_rnd_chain_swap() {
  int rand_chain = rand()%chains.size();
  return {chains[rand_chain].first, -1};
}

// This assumes correctness on the input, as it is a private function. Will panic if it's not.
// ch_ix needs to take a value between 0 and length(chains) - 1. Side cant take either value 0 ir 1,
// only taking value 0 if ch_ix == length(chains) - 1.
pair<int, int> NPE::gen_rnd_operand_operator_swap() {
  // Choose a chain end at random excluding the last one so this is as uniform as possible
  int rnd_choice = rand()%(chains.size()<<1),
      rnd_chain = (rnd_choice)>>1,
      rnd_side = rnd_choice%2,
      rnd_operator_pos = chains[rnd_chain].first + rnd_side*(chains[rnd_chain].second - 1),
      rnd_operand_pos = rnd_operator_pos + (rnd_side<<1) - 1;
  // If the swap breaks the skewness of the tree or if tries to swap with the tree root, panic and jump out
  if((npe[rnd_operator_pos] == npe[rnd_operator_pos + (rnd_side<<2) - 2]) || (rnd_operator_pos == int(length)-1)) return {-1,-1};

  // Check that the perturbation satisfies the balloting rule (only needs to be done if
  // we are pushing operators back
  if (!rnd_side) {
    int op_counter = 1;
    for (vector<pair<int, int>>::iterator it = chains.begin(); it != next(chains.begin(), rnd_chain); ++it ) op_counter += it->second;
    if ( 2*op_counter >= rnd_operand_pos+1 ) return {-1,-1};
  }
  // If the perturbation is valid, swap the values in the vector and return 0
  return {rnd_operator_pos, rnd_operand_pos};
}
*/
