#include <cstdlib>
#include <stdio.h>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

using namespace Gecode;

class Queens : public Space {
protected:
  int n;
  IntVarArray board;
public:
  // Regular constructor
  Queens(int x) : n{x}, board(*this, n, 0, n-1) {
    int i, j;
    IntArgs desc_diag = IntArgs::create(n, 0, -1);
    IntArgs asc_diag = IntArgs::create(n, 0, 1);
    // ---------------------------------------------------
    // Constraints posting
    //----------------------------------------------------

    // 1 - Ecach row has to have a queen in a different position
    distinct(*this, board);
    // 2 - Only one queen per descending diagonal
    distinct(*this, desc_diag, board);
    // 3 - Only one queen per ascending diagonal
    distinct(*this, asc_diag, board);

    branch(*this, board, INT_VAR_NONE(), INT_VAL_MAX());
  }

  // Copy constructor
  Queens(Queens& s) :  Space(s) {
    n = s.n;
    board.update(*this, s.board);
  }

  // Copy funcion
  virtual Space* copy() {
    return new Queens(*this);
  }

  void print(){
    int i, j;
    for(j=0; j<n; ++j){
      for(i=0; i<n; ++i){
        if(i == board[j].val()){
          std::cout << "X";
        }else{
          std::cout << ".";
        }
      }
      std::cout << std::endl;
    }
  }

  int board_size() {
    return n;
  }

};
int main(int argc, char* argv[]) {
  if(argc < 2){
    printf("Bad number of arguments: Required 1, given %d.\n", argc-1);
    return -1;
  }
  int n = atoi(argv[1]);
  if(n < 4){
    std::cout << "Parameter must be an integer greater or equal than 4" << std::endl;
    return -1;
  }
  try {
    Queens* q = new Queens(n);
    DFS<Queens> e(q);
    delete q;
    int i=0;
    if (Queens* q = e.next()) {
      q -> print();
      std::cout << std::endl;
      delete q;
    }
  }catch(Exception e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
