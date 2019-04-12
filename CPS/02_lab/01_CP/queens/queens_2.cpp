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
    int i, j, c_i;
    // ---------------------------------------------------
    // Constraints posting
    //----------------------------------------------------

    // 1 - There are exactly n queens on the board
    //rel(*this, sum(board) == n); - NOT NEEDED IN THIS IMPLEMENTATION

    // 2 - Queens must not be assigned to the same row for two different columns
    // 3 - Queens must not be in the same descending diagonal
    // 4 - Queens must not be in the same Ascending diagonal
    for(i=0; i<n; ++i) {
      for(j=0; j<n; ++j) {
        if(i != j) {
          rel(*this, board[j] != board[i]);         // 2
          rel(*this, board[j] != board[i] + (j-i)); // 3
          rel(*this, board[j] != board[i] - (j-i)); // 4
        }
      }
    }

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
