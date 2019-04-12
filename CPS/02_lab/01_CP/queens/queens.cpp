#include <cstdlib>
#include <stdio.h>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

using namespace Gecode;

class Queens : public Space {
protected:
  int n;
  BoolVarArray board;
public:
  // Regular constructor
  Queens(int x) : n{x}, board(*this, n*n, 0, 1) {
    int i, j;
    // ---------------------------------------------------
    // Constraints posting
    //----------------------------------------------------

    // 1 - There are exactly n queens on the board
    rel(*this, sum(board) == n);

    // 2 - At most one queen per row
    // 3 - At most one queen per column
    for(i=0; i<n; i++) {
      rel(*this, sum(board.slice(i*n, 1, n)) <= 1);   // 2
      rel(*this, sum(board.slice(i, n, n)) <= 1);     // 3
    }

    // 4 - At most one queen per descending diagonal
    for(i=0; i < n; ++i) {
      rel(*this, sum(board.slice(i, n+1, n-i)) <= 1);
      if(i > 0) rel(*this, sum(board.slice(i*n, n+1, n-i)) <= 1);
    }

    // 5 - At most one queen per ascending diagonal
    for(i=n-1; i >= 0; --i) {
      rel(*this, sum(board.slice(i, n-1, i+1)) <= 1);
      if(i < n-1) rel(*this, sum(board.slice((i+1)*n-1, n-1, n-i)) <= 1);
    }

    branch(*this, board, BOOL_VAR_NONE(), BOOL_VAL_MAX());
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
        if(board[j*n + i].val()==1){
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
  Queens* q = new Queens(n);
  DFS<Queens> e(q);
  delete q;
  int i=0;
  if (Queens* q = e.next()) {
    delete q;
  }
  return 0;
}
