#include <unordered_map>
#include <vector>

#include "../Grammar/Grammar.hpp"

class CheckAlgorithm {
 public:
  CheckAlgorithm() = default;

  void Fit(const Grammar& grammar) { grammar_ = grammar; }

  bool Predict(const std::string& word);

 private:
  Grammar grammar_;

  struct Situation {
    char from;
    std::string to;
    int64_t j;

    bool operator==(const Situation& other) const {
      return from == other.from && to == other.to && j == other.j;
    }
  };

  std::vector<std::vector<Situation>> situations_;
  const std::string dot = "#";

  // get str[i]...str[j] -> include j
  std::string GetSubstring(const std::string& str, int64_t i, int64_t j) {
    std::string result = "";
    for (int64_t k = i; k <= j; ++k) {
      result += str[k];
    }

    return result;
  }

  void EarleyScan(int64_t j, const std::string& w) {
    if (j == 0) {
      return;
    }

    for (const auto& situation : situations_[j - 1]) {
      for (int64_t i = 0; i < situation.to.size(); ++i) {
        if (situation.to[i] != dot[0]) {
          continue;
        }

        if (i + 1 < situation.to.size()) {
          if (situation.to[i + 1] == w[j - 1]) {
            Situation new_situation = Situation{
                situation.from,
                GetSubstring(situation.to, 0, i - 1) + situation.to[i + 1] +
                    dot +
                    GetSubstring(situation.to, i + 2, situation.to.size() - 1),
                situation.j};

            bool was = false;
            for (const auto& other_situation : situations_[j]) {
              if (other_situation == new_situation) {
                was = true;
              }
            }

            if (!was) {
              situations_[j].push_back(new_situation);
            }
          }
        }

        break;
      }
    }
  }

  void EarleyPredict(int64_t j, const std::string& w) {
    std::vector<Situation> add_situations;

    for (const auto& situation : situations_[j]) {
      for (int64_t i = 0; i < situation.to.size(); ++i) {
        if (situation.to[i] != dot[0]) {
          continue;
        }

        auto nonterminals = grammar_.GetNonterminals();
        auto rules = grammar_.GetRules();

        if (i + 1 < situation.to.size()) {
          if (nonterminals.find(situation.to[i + 1]) != nonterminals.end()) {
            for (const auto& [left_part, right_parts] : rules) {
              if (left_part != situation.to[i + 1]) {
                continue;
              }

              for (const auto& right_part : right_parts) {
                add_situations.push_back(
                    Situation{situation.to[i + 1], dot + right_part, j});
              }
            }
          }
        }

        break;
      }
    }

    for (const auto& add_situation : add_situations) {
      bool was = false;
      for (const auto& situation : situations_[j]) {
        if (situation == add_situation) {
          was = true;
          break;
        }
      }

      if (!was) situations_[j].push_back(add_situation);
    }
  }

  void EarleyComplete(int64_t j, const std::string& w) {
    std::vector<Situation> add_situations;

    for (const auto& situation : situations_[j]) {
      if (situation.to.back() != dot[0]) {
        continue;
      }

      for (const auto& other_situation : situations_[situation.j]) {
        for (int64_t i = 0; i < other_situation.to.size(); ++i) {
          if (other_situation.to[i] != dot[0]) {
            continue;
          }

          if (i + 1 < other_situation.to.size()) {
            if (other_situation.to[i + 1] == situation.from) {
              add_situations.push_back(
                  Situation{other_situation.from,
                            GetSubstring(other_situation.to, 0, i - 1) +
                                other_situation.to[i + 1] + dot +
                                GetSubstring(other_situation.to, i + 2,
                                             other_situation.to.size() - 1),
                            other_situation.j});
            }
          }

          break;
        }
      }
    }

    for (const auto& add_situation : add_situations) {
      bool was = false;
      for (const auto& situation : situations_[j]) {
        if (situation == add_situation) {
          was = true;
          break;
        }
      }

      if (!was) situations_[j].push_back(add_situation);
    }
  }
};