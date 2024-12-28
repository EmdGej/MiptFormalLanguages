#include <unordered_map>
#include <vector>

#include "../Grammar/Grammar.hpp"

class CheckAlgorithm {
 public:
  CheckAlgorithm() = default;

  void Fit(const Grammar& grammar) { grammar_ = grammar; }

  bool Predict(const std::string& word) const;

 private:
  Grammar grammar_;
};