#include <vector>
#include <map>

class NPE {
  std::vector<int> npe;
  std::vector<int> operand_positions;
  std::map<int, int> chain_positions;

public:
  static const int V = -1, H = -2;
  NPE(const std::vector<int> &seq);
};
