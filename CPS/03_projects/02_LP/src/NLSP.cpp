/*------------------------------------------------------------
 - Project: Linear Programming NLSP Solver
 - Subject: Combinatorial Problem Solving
 - Author:  Francisco Javier Jurado Moreno
 - Date:    June 2019
 - File:    NLSP.cpp
*///----------------------------------------------------------

#include <iostream>
#include <stdlib.h>

#include "NLSP.h"
ILOSTLBEGIN

// Class constructor - Initialize the environment, model and related attributes.
// Add constraints and objective function to the model.
NLSP::NLSP(int height, int num_inputs, int* tt):
  depth { depth },
  num_inputs { num_inputs },
  size_tt { 1 << num_inputs },
  max_nodes { (1 << (height + 1)) - 1 },
  max_gates { (1 << height ) - 1 },
  sol_found { -1 },
  tt { tt },
  env (),
  model ( env ),
  cplex ( model ),

//---------------------------------------------------------------------------
// DECISION VARIABLES
//---------------------------------------------------------------------------

// LAYOUT (B, (num_inputs+1) x max_nodes)
// Boolean matrix indicating the mapping to gates, inputs and 0 signals for each
// node. Each row of the matrix corresponds, for each node, to the unary representation
// of the id of the input the node is mapped to. If a node is a gate, it is mapped to 0.
// Note that an extra column is added to account to the 0 signal

// IS_GATE (B, max_nodes)
// Boolean array indicating if a node is mapped to a gate.

// CONFIGS (B, max_nodes x size_tt)
// Boolean matrix indicating the value (1 or 0) at each node for each entry in
// the truth table. Unused node are assigned value 0
  LAYOUT ( env, (num_inputs+1)*max_nodes, 0, 1, ILOBOOL ),
  IS_GATE ( env, max_nodes, 0, 1, ILOBOOL ),
  CONFIGS ( env, max_nodes*size_tt, 0, 1, ILOBOOL )
{
//---------------------------------------------------------------------------
// CONSTRAINTS
//---------------------------------------------------------------------------

// CONSTRAINT 1: Exactly 1 input selected per node.
  for ( int j=1; j <= max_nodes; ++j ) {
    IloExpr expr3(env);
    for(int i=0; i <= num_inputs; ++i) expr3 += layout(j, i);
    model.add(expr3 == 1);
    expr3.end();
  }

// CONSTRAINT 2: Nodes can be mapped to inputs iff their parent is a gate
// FRONTIER CONDITION: GATE - LAYOUT
  for(int i=2; i <= max_nodes; ++i ) {
    model.add( layout(i, 0) + is_gate(i/2)  >= 1 );
  }

// CONSTRAINT 3: Nodes can be mapped to inputs only if they are not gates
// FRONTIER CONDITION
  for(int i=2; i <= max_nodes; ++i ) {
    model.add( layout(i, 0) >= is_gate(i) );
  }

// CONSTRAINT 4: Nodes can be mapped to gates only if their parent is a gate
// GATE PROPAGATION
  for ( int i=2; i <= max_gates; ++i ) {
    model.add( is_gate(i) <= is_gate(i/2) );
  }

// CONSTRAINT 5: Leave nodes can not be mapped to gates
  for ( int i=max_gates+1; i <= max_nodes; ++i ) {
      model.add( is_gate(i) == 0 );
  }

// CONSTRAINT 6: The output of each configuration must be equal to the corresponding \
// truth table entry
  for ( int i=0; i < size_tt; ++i ) {
    model.add( configs(i, 1) == tt[i] );
  }

// CONSTRAINT 7: For each node and configuration, their value must correspond to
// the input they are assigned to, or left unconstrained if they are mapped to
// a gate. This also forces unused nodes to be mapped to 0.
  int tt_row[num_inputs+1]; tt_row[0] = 0;
  for ( int n=0; n < size_tt; ++n ) {
    // Generate the value of the inputs for the given row of the truth table.
    gen_tt_row(tt_row, n);

    for ( int j=1; j <= max_nodes; ++j ) {
      IloExpr expr4(env);
      // Obtain the value (1 or 0) at the input the node is mapped at by performing
      // the scalar product of the mapping and the binary representation of the
      // input at a given row.
      for(int i=0; i <= num_inputs; ++i) expr4 += layout(j, i)*tt_row[i];
      // If a node is mapped to an input with value 1 at a given configuration,
      // it must take value 1 for that configuration. If the value is 0, it
      // must be exaclty 0 if it is not a gate. If it is a gate can take value
      // 0 or 1. (note that gates map to 0 signals in the LAYOUT so they won't
      // ever be forced to be 1 by this constraint)
      model.add( configs(n, j) <= expr4 + is_gate(j));
      model.add( configs(n, j) >= expr4 );
      expr4.end();
      }
  }

// CONSTRAINT 8: Nodes mapped to gates implement the NOR functionality.
  // EPLAIN THIS WITH TRUTH table
  for ( int n=0; n < size_tt; ++n ) {
    for ( int i=1; i <= max_gates; ++i ) {
        // The is-gate checkings act as a switch, if a node is not mapped to
        // a gate they turn off the dependency on the children by opening the
        // constraint bounds.
        model.add( configs(n, i) >= is_gate(i) - configs(n, i*2) - configs(n, i*2 + 1) );
        model.add( 2*configs(n, i) <= 2*is_gate(i) - configs(n, i*2) - configs(n, i*2 + 1)
                + 2*(1 - is_gate(i)) );
    }
  }

//---------------------------------------------------------------------------
// OBJECTIVE FUNCTION
//---------------------------------------------------------------------------

// The objective function tries to minimize the number of gates. As the solver
// is run by trying to find a solution for increasing depth values, it makes sure
// that when a solution is found for a given depth (which will be minimal), it is
// aso found for the smallest circuit
  IloExpr obj(env);
  for ( int i=0; i < max_nodes; ++i) obj += IS_GATE[i];
  model.add(IloMinimize(env, obj));
  obj.end();
}

// Class Destructor
NLSP::~NLSP() {
  env.end();
}

void NLSP::solve() {
  std::cerr << "Solving..." << std::endl;
  sol_found = cplex.solve();
}

//---------------------------------------------------------------------------
// HELPER METHODS, SETTERS, GETTERS
//---------------------------------------------------------------------------

int NLSP::solution_found() {
  return sol_found;
}

int NLSP::get_objective_value () {
  return cplex.getObjValue();
}

// Helper method to access IS_GATE with 1-indexing
IloNumVar NLSP::is_gate(int i) {
  return IS_GATE[i-1];
}

// Helper method to access a node in IS_GATE with 1-indexing for a given configuration
IloNumVar NLSP::configs(int n, int i) {
  if (i > max_nodes) {
    // Sanity check
    std::cout << "WARNING: Indexing overlfow when trying to access configuration " << n <<
    " (" << i << ")";
  }
  return CONFIGS[n*max_nodes + i - 1];
}

// Helper method to access an input mapping value in LAYOUT with 1-indexing
// for a given node (nodes are accessed with 1-indexing)
IloNumVar NLSP::layout(int n, int i) {
  if (i > num_inputs+1) {
    // Sanity check
    std::cout << "WARNING: Indexing overlfow when trying to access layout configuration " << n <<
    " (" << i << ")";
  }
  return LAYOUT[(n-1)*(num_inputs+1) + i ];
}

// Helper method returning a zero-padded array of length num_inputs+1 containing
// the binary representation of a given number n
void NLSP::gen_tt_row(int* tt_row, const int n) {
  int aux = n;
  for(int j=num_inputs; j > 0; --j) {
    tt_row[j] = aux % 2;
    aux >>= 1;
  }
}

// Method printing the found solution to STDOUT
void NLSP::print_solution () {
  IloNumArray l(env), g(env);
  cplex.getValues(l, LAYOUT);
  cplex.getValues(g, IS_GATE);
  print_node(1, g, l);
}

// Method recursively printing the solution as requested in the exercise statement.
// Called by print_solution
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
