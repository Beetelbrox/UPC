#include <iostream>
#include <stdlib.h>


#include "NLSP.h"
ILOSTLBEGIN

NLSP::NLSP(int height, int num_inputs, int* tt):
    height { height },
    num_inputs { num_inputs },
    size_tt { 1 << num_inputs },
    max_nodes { (1 << (height + 1)) - 1 },
    max_gates { (1 << height ) - 1 },
    sol_found { -1 },
    tt { tt },
    env (),
    model ( env ),
    cplex ( model ),
    LAYOUT ( env, max_nodes * (num_inputs+1), 0, 1, ILOBOOL ),
    IS_GATE ( env, max_nodes, 0, 1, ILOBOOL ),
    CONFIGS ( env, max_nodes*size_tt, 0, 1, ILOBOOL)
{

  // CONSTRAINT 1: Exactly 1 input selected per node" << std::endl;
  // CONSTRAINT: Exactly 1 input is selected per node
  for ( int j=1; j <= max_nodes; ++j ) {
    IloExpr expr3(env);
    for(int i=0; i <= num_inputs; ++i) expr3 += layout(j, i);
    model.add(expr3 == 1);
    expr3.end();
  }

  std::cerr << "Adding Constraint 2: A node can be mapped to an input iff its parent is a gate" << std::endl;
  // FRONTIER CONDITION: GATE - LAYOUT
  for(int i=2; i <= max_nodes; ++i ) {
    //model.add( (1 - layout(i, 0)) <= is_gate(i/2) );
    model.add( layout(i, 0) + is_gate(i/2)  >= 1 );
  }

  std::cout << "Adding Constraint 3: A node can be mapped to a gate iff its parent is a gate" << std::endl;
  // GATE PROPAGATION
  for ( int i=2; i <= max_gates; ++i ) {
    model.add( is_gate(i) <= is_gate(i/2) );
  }

  std::cout << "Adding Constraint 3: A node can be mapped to an input iff it is not a gate" << std::endl;
  // FRONTIER CONDITION
  for(int i=2; i <= max_nodes; ++i ) {
    model.add( layout(i, 0) >= is_gate(i) );
  }

  std::cout << "Adding Constraint 4: Leaves can not be mapped to gates" << std::endl;
  for ( int i=max_gates+1; i <= max_nodes; ++i ) {
      model.add( is_gate(i) == 0 );
  }

  std::cout << "Adding Constraint 5: The output of each configuration must be equal to the corresponding \
  truth table entry" << std::endl;
  for ( int i=0; i < size_tt; ++i ) {
    model.add( configs(i, 1) == tt[i] );
  }

  int tt_row[num_inputs+1];
  tt_row[0] = 0;
  std::cout << "Adding Constraint 6" << std::endl;
  // CONSTRAINT: Assigns a value to each node for each configuration
  // HERE WE MIGHT BE ABLE TO CUT THE LAST LINE

  for ( int n=0; n < size_tt; ++n ) {
    gen_tt_row(tt_row, n);    // Convert the line of input to the appropriate format

    for ( int j=1; j <= max_nodes; ++j ) {
      IloExpr expr4(env);
      // Get the value (1 or 0) of the input assigned to the node
      // Note that if it is assigned to 0, the value is 0
      for(int i=0; i <= num_inputs; ++i) expr4 += layout(j, i)*tt_row[i];
      model.add( configs(n, j) <= expr4 + is_gate(j));
      model.add( configs(n, j) >= expr4 );
      expr4.end();
      }
  }

  std::cout << "Adding Constraint 3" << std::endl;
  // CONSTRAINT: Implements NOR behaviour when it is gate, leaves open otherwisela
  // EPLAIN THIS WITH TRUTH table
  for ( int n=0; n < size_tt; ++n ) {
    for ( int i=1; i <= max_gates; ++i ) {
        model.add( configs(n, i) >= is_gate(i) - configs(n, i*2) - configs(n, i*2 + 1) );
        model.add( 2*configs(n, i) <= 2*is_gate(i) - configs(n, i*2) - configs(n, i*2 + 1) + 2*(1 - is_gate(i)) );
    }
  }


  std::cout << "Adding Objective function" << std::endl;
  // Objective function
  IloExpr obj(env);
  for ( int i=0; i < max_nodes; ++i) {
    obj += IS_GATE[i];
  }

  model.add(IloMinimize(env, obj));
  obj.end();
}

// Helper function to allow 1-indexing
IloNumVar NLSP::is_gate(int i) {
  return IS_GATE[i-1];
}

// Helper function to allow 1-indexing at a given configuration
IloNumVar NLSP::configs(int n, int i) {
  if (i > max_nodes) {
    std::cout << "WARNING: Overflowing when accessing configuration " << n <<
    " (" << i << ")";
  }
  return CONFIGS[n*max_nodes + i - 1];
}

IloNumVar NLSP::layout(int n, int i) {
  if (i > num_inputs+1) {
    std::cout << "WARNING: Overflowing when accessing layout " << n <<
    " (" << i << ")";
  }
  return LAYOUT[(n-1)*(num_inputs+1) + i ];
}

void NLSP::gen_tt_row(int* tt_row, const int n) {
  int aux = n;
  for(int j=num_inputs; j > 0; --j) {
    tt_row[j] = aux % 2;
    aux >>= 1;
  }
}

void NLSP::solve() {
  std::cerr << "Solving..." << std::endl;
  sol_found = cplex.solve();
}

int NLSP::solution_found() {
  return sol_found;
}

int NLSP::get_objective_value () {
  return cplex.getObjValue();
}

void NLSP::print_solution () {
  IloNumArray l(env), g(env);
  cplex.getValues(l, LAYOUT);
  cplex.getValues(g, IS_GATE);
  print_node(1, g, l);
}

void NLSP::print_node (int n, IloNumArray g, IloNumArray l) {
  std::cout << n << " ";
  if ( g[n-1] ) { // If it is a gate
    std::cout << -1 << " ";
    std::cout << 2*n << " " << 2*n + 1 << std::endl;
    print_node( 2*n, g, l);
    print_node( 2*n+1, g, l);
  } else {
    for(int i=0; i<=num_inputs; ++i) {
      if ( l[ (n-1)*(num_inputs+1) + i ] ) {
        std::cout << i << " ";
        break;
      }
    }
      std::cout << 0 << " " << 0 << std::endl;
  }
}

void NLSP::print_debug () {
  std::cerr << "Status: " << cplex.getStatus() << std::endl;
  std::cerr << "Objective value: " << cplex.getObjValue() << std::endl;
  IloNumArray v(env), g(env), c(env);
  cplex.getValues(v, LAYOUT);
  cplex.getValues(g, IS_GATE);
  cplex.getValues(c, CONFIGS);
  for (int j=0; j<max_nodes; ++j) {
    if ( j < 10) std::cout << " ";
    std::cerr << j << ": ";
    for (int i=0; i <= num_inputs; ++i) {
      std::cerr  << abs(v[j*(num_inputs+1) + i]) << " ";
    }
    std::cerr <<" - " << abs(g[j]) << std::endl;
  }
  std::cerr << std::endl;
  for (int j=0; j<size_tt; ++j) {
    for (int i=0; i < max_nodes; ++i) {
      std::cerr << abs(c[j*(max_nodes) + i]) << " ";
    }
    std::cerr << std::endl;
  }
}

NLSP::~NLSP() {
  env.end();
}
