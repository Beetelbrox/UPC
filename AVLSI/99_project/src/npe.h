#include <vector>
#include <map>

class NPE {
  std::vector<int> npe;
  std::vector<int> operand_pos;
  std::map<int, int> chain_pos;

  void swap_operands(size_t ix);
  void invert_chain(size_t ix);
  int swap_operand_operator(size_t ix);

public:
  static const int V = -1, H = -2;
  NPE(const std::vector<int> &seq);

  std::vector<int> generate_random_permutation();

  void print_npe();
};
