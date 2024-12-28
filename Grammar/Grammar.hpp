#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../WrongGrammarException/WrongGrammarException.hpp"

class Grammar {
 public:
  Grammar() = default;

  Grammar(const std::unordered_set<char>& nonterminals,
          const std::unordered_set<char>& alphabet,
          const std::unordered_map<char, std::vector<std::string>>& rules,
          char start_nonterminal);

  const std::unordered_set<char>& GetNonterminals() const {
    return nonterminals_;
  }
  const std::unordered_set<char>& GetAlphabet() const { return alphabet_; }
  const std::unordered_map<char, std::vector<std::string>>& GetRules() const {
    return rules_;
  }
  char GetStartNonterminal() const { return start_nonterminal_; }
  char GetPrevStartNonterminal() const { return prev_start_nonterminal_; }

 private:
  std::unordered_set<char> nonterminals_;
  std::unordered_set<char> alphabet_;
  std::unordered_map<char, std::vector<std::string>> rules_;
  char start_nonterminal_;
  char prev_start_nonterminal_;

  bool IsCorrectGrammar() const {
    for (const auto& [left_part, right_parts] : rules_) {
      if (nonterminals_.find(left_part) == nonterminals_.end()) {
        return false;
      }

      for (const auto& right_part: right_parts) {
        for (int64_t i = 0; i < right_part.size(); ++i) {
          if (nonterminals_.find(right_part[i]) == nonterminals_.end() &&
              alphabet_.find(right_part[i]) == alphabet_.end()) {
                return false;
          }
        }
      }
    }

    return true;
  }

  int64_t GetMaxNonterminalIdx() {
    int64_t max_nonterminal_idx = -1;
    
    for (const auto& nonterminal: nonterminals_) {
      max_nonterminal_idx = std::max(max_nonterminal_idx,static_cast<int64_t>(nonterminal));
    }

    for (const auto& symbol: alphabet_) {
      max_nonterminal_idx = std::max(max_nonterminal_idx,static_cast<int64_t>(symbol));
    }

    return max_nonterminal_idx + 1;
  }


  void AddNewStart() {
    int64_t max_nonterminal_idx = GetMaxNonterminalIdx();
    
    nonterminals_.insert(static_cast<char>(max_nonterminal_idx));
    rules_[static_cast<char>(max_nonterminal_idx)].push_back(std::string(1, start_nonterminal_));
    start_nonterminal_ = static_cast<char>(max_nonterminal_idx);
  }
};