#ifndef NLSP_H
#define NLSP_H

#include <ilcplex/ilocplex.h>

class NLSP {
public:
  NLSP(int height, int num_inputs, int* tt);
  ~NLSP();
  void solve();
  int solution_found();
  int get_objective_value ();
  void print_solution ();
  void print_debug ();
private:
  int height, num_inputs, size_tt, max_nodes, max_gates, sol_found;
  int* tt;

  IloEnv env;
  IloModel model;
  IloCplex cplex;
  IloNumVarArray LAYOUT, IS_GATE, CONFIGS;

  IloNumVar is_gate(int i);
  IloNumVar configs(int n, int i);
  IloNumVar layout(int n, int i);

  void gen_tt_row(int* tt_row, const int n);
  void print_node(int n, IloNumArray g, IloNumArray l);
};

#endif
