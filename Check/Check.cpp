#include "Check.hpp"

bool CheckAlgorithm::Predict(const std::string& word) {
  if (!situations_.empty()) situations_.clear();

  situations_.resize(word.size() + 1);
  situations_[0].push_back(
      Situation{grammar_.GetStartNonterminal(),
                dot + std::string(1, grammar_.GetPrevStartNonterminal()), 0});

  for (int64_t j = 0; j <= word.size(); ++j) {
    EarleyScan(j, word);

    int64_t prev_situations_size;
    do {
      prev_situations_size = situations_[j].size();

      EarleyComplete(j, word);
      EarleyPredict(j, word);
    } while (situations_[j].size() != prev_situations_size);
  }

  for (const auto& situation : situations_.back()) {
    if (situation.from == grammar_.GetStartNonterminal() &&
        situation.to == grammar_.GetPrevStartNonterminal() + dot &&
        situation.j == 0) {
      return true;
    }
  }

  return false;
}