#include "Check.hpp"

bool CheckAlgorithm::Predict(const std::string& word) const {
  if (word == "") {
    for (const auto& rule : grammar_.GetRules()) {
      if (rule.first != grammar_.GetStartNonterminal()) {
        continue;
      }

      for (int64_t i = 0; i < rule.second.size(); ++i) {
        if (rule.second[i] == "") {
          return true;
        }
      }
    }

    return false;
  }

  std::unordered_map<char, std::vector<std::vector<bool>>> dp;

  for (const auto& nonterminal : grammar_.GetNonterminals()) {
    dp[nonterminal] = std::vector<std::vector<bool>>(
        word.size(), std::vector<bool>(word.size(), false));
  }

  for (const auto& rule : grammar_.GetRules()) {
    for (int64_t k = 0; k < rule.second.size(); ++k) {
      for (int64_t i = 0; i < word.size(); ++i) {
        if (rule.second[k].size() == 1 && rule.second[k][0] == word[i]) {
          dp[rule.first][i][i] = true;
        }
      }
    }
  }

  for (int64_t len = 1; len < word.size(); ++len) {
    for (int64_t i = 0; i + len < word.size(); ++i) {
      for (const auto& rule : grammar_.GetRules()) {
        for (int64_t k = 0; k < rule.second.size(); ++k) {
          if (rule.second[k].size() <= 1) {
            continue;
          }

          for (int64_t j = i; j < i + len; ++j) {
            dp[rule.first][i][i + len] =
                dp[rule.first][i][i + len] ||
                dp[rule.second[k][0]][i][j] &&
                    dp[rule.second[k][1]][j + 1][i + len];
          }
        }
      }
    }
  }

  return dp[grammar_.GetStartNonterminal()][0][word.size() - 1];
}