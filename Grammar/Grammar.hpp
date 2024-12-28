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

 private:
  std::unordered_set<char> nonterminals_;
  std::unordered_set<char> alphabet_;
  std::unordered_map<char, std::vector<std::string>> rules_;
  char start_nonterminal_;

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

  void GrammarToNormalForm() {
    bool has_eps_word = HasGrammarEpsWord();

    DeleteNongenerativeAndUnreachableNonterminals();

    ReformatMixedRules();
    DeleteNongenerativeAndUnreachableNonterminals();

    ReformatLongRules();
    DeleteNongenerativeAndUnreachableNonterminals();

    DeleteEpsGenerativeNonterminals();
    DeleteNongenerativeAndUnreachableNonterminals();

    if (has_eps_word) {
      AddEpsWord();
      DeleteNongenerativeAndUnreachableNonterminals();
    }
    
    DeleteChainRules();
    DeleteNongenerativeAndUnreachableNonterminals();
  }

  bool HasGrammarEpsWord() {
    for (const auto& right_part: rules_[start_nonterminal_]) {
      if (right_part.size() == 0) {
        return true;
      }
    }

    return false;
  }

  void DeleteNongenerativeAndUnreachableNonterminals() {
    DeleteNongenerativeNonterminals();
    DeleteUnreachableNonterminals();
  }

  void DeleteNongenerativeNonterminals() {
    std::unordered_set<char> generative_nonterminals;

    for (const auto& [left_part, right_parts]: rules_) {
      for (const auto& right_part: right_parts) {
        bool has_nonterminals = false;
        
        for (int64_t i = 0; i < right_part.size(); ++i) {
          if (nonterminals_.find(right_part[i]) != nonterminals_.end()) {
            has_nonterminals = true;
            break;
          }
        }

        if (!has_nonterminals) {
          generative_nonterminals.insert(left_part);
          break;
        }
      }
    }

    int64_t prev_step_generative_nonterminals_size;

    do {
      prev_step_generative_nonterminals_size = generative_nonterminals.size();

      for (const auto& [left_part, right_parts]: rules_) {
        for (const auto& right_part: right_parts) {
          bool all_nonterminals_generative = true;

          for (int64_t i = 0; i < right_part.size(); ++i) {
            if (nonterminals_.find(right_part[i]) != nonterminals_.end()) {
              if (generative_nonterminals.find(right_part[i]) == generative_nonterminals.end()) {
                all_nonterminals_generative = false;
                break;
              }
            }
          }

          if (all_nonterminals_generative) {
            generative_nonterminals.insert(left_part);
            break;
          }
        }
      }
    } while (generative_nonterminals.size() != prev_step_generative_nonterminals_size);

    for (const auto& [left_part, right_parts]: rules_) {
      if (generative_nonterminals.find(left_part) == generative_nonterminals.end()) {
        rules_.erase(left_part);
      }
    }
  }

  void DeleteUnreachableNonterminals() {
    std::unordered_set<char> reachable_terminals;
    reachable_terminals.insert(start_nonterminal_);

    int64_t prev_reachable_terminals_size;
    do {
      prev_reachable_terminals_size = reachable_terminals.size();

      for (const auto& [left_part, right_parts]: rules_) {
        for (const auto& right_part: right_parts) {
          for (int64_t i = 0; i < right_part.size(); ++i) {
            if (nonterminals_.find(right_part[i]) != nonterminals_.end()) {
              reachable_terminals.insert(right_part[i]);
            }
          }
        }
      }
    } while (reachable_terminals.size() != prev_reachable_terminals_size);
  }

  void ReformatMixedRules() {
    int64_t max_nonterminal_idx = GetMaxNonterminalIdx();

    std::unordered_map<char, char> symbol_to_nonterminal;
    for (auto& [left_part, right_parts]: rules_) {
      for (auto& right_part: right_parts) {
        for (int64_t i = 0; i < right_part.size(); ++i) {
          if (alphabet_.find(right_part[i]) != alphabet_.end()) {
            if (symbol_to_nonterminal.find(right_part[i]) == symbol_to_nonterminal.end()) {
              symbol_to_nonterminal[right_part[i]] = static_cast<char>(max_nonterminal_idx++);
              rules_[symbol_to_nonterminal[right_part[i]]].push_back(std::string(1, right_part[i]));
              nonterminals_.insert(symbol_to_nonterminal[right_part[i]]);
            }
             
            right_part[i] = symbol_to_nonterminal[right_part[i]];
          }
        }
      }
    }
  }

  void ReformatLongRules() {
    int64_t max_nonterminal_idx = GetMaxNonterminalIdx();

    for (auto& [left_part, right_parts]: rules_) {
      std::vector<bool> delete_idx(right_parts.size());
      std::vector<std::string> add_to_left_part;

      int64_t pos = 0;
      for (auto& right_part: right_parts) {
        if (right_part.size() <= 2) {
          ++pos;
          continue;
        }
        
        char cur_nonterminal = left_part;
        for (int64_t i = 0; i < right_part.size() - 2; ++i) {
          nonterminals_.insert(static_cast<char>(max_nonterminal_idx));
          
          if (cur_nonterminal != left_part) {
            rules_[cur_nonterminal].push_back(std::string(1, right_part[i]) + std::string(1, static_cast<char>(max_nonterminal_idx)));
          } else {
            add_to_left_part.push_back(std::string(1, right_part[i]) + std::string(1, static_cast<char>(max_nonterminal_idx)));
          }

          cur_nonterminal = static_cast<char>(max_nonterminal_idx);
          ++max_nonterminal_idx;
        }

        rules_[cur_nonterminal].push_back(std::string(1, right_part[right_part.size() - 2]) + std::string(1, right_part.back()));
        delete_idx[pos++] = true;
      }

      std::vector<std::string> new_right_part;
      for (int64_t i = 0; i < right_parts.size(); ++i) {
        if (!delete_idx[i]) {
          new_right_part.push_back(right_parts[i]);
        }
      }

      rules_[left_part] = std::move(new_right_part);
      for (const auto& right_part: add_to_left_part) {
        rules_[left_part].push_back(right_part);
      }
    }
  }

  void DeleteEpsGenerativeNonterminals() {
    std::unordered_set<char> eps_generative_nonterminals;

    for (const auto& [left_part, right_parts]: rules_) {
      for (const auto& rigth_part: right_parts) {
        if (rigth_part.size() == 0) {
          eps_generative_nonterminals.insert(left_part);
        }
      }
    }

    int64_t prev_eps_generative_nonterminals_size;

    do {
      prev_eps_generative_nonterminals_size = eps_generative_nonterminals.size();

      for (const auto& [left_part, right_parts]: rules_) {
        for (const auto& rigth_part: right_parts) {
          if (rigth_part.size() == 2) {
            if (eps_generative_nonterminals.find(rigth_part[0]) != eps_generative_nonterminals.end() &&
                eps_generative_nonterminals.find(rigth_part[1]) != eps_generative_nonterminals.end()) {
                  eps_generative_nonterminals.insert(left_part);
            }
          }

          if (rigth_part.size() == 1 && eps_generative_nonterminals.find(rigth_part[0]) != eps_generative_nonterminals.end()) {
            eps_generative_nonterminals.insert(left_part);
          }
        }
      }
    } while (eps_generative_nonterminals.size() != prev_eps_generative_nonterminals_size);

    for (auto& [left_part, right_parts]: rules_) {
      std::vector<std::string> new_right_parts;

      for (auto& right_part: right_parts) {
        if (right_part.size() == 0) {
          continue;
        }

        if (right_part.size() == 1) {
          new_right_parts.push_back(right_part);
        }

        if (right_part.size() == 2) {
          bool flag1 = eps_generative_nonterminals.find(right_part[0]) != eps_generative_nonterminals.end();
          bool flag2 = eps_generative_nonterminals.find(right_part[1]) != eps_generative_nonterminals.end();
          
          new_right_parts.push_back(right_part);
          if (flag1 && flag2) { 
            new_right_parts.push_back(std::string(1, right_part[0]));
            new_right_parts.push_back(std::string(1, right_part[1]));
          } else if (flag1 && !flag2) {
            new_right_parts.push_back(std::string(1, right_part[1]));
          } else if (!flag1 && flag2) {
            new_right_parts.push_back(std::string(1, right_part[0]));
          }
        }
      }

      rules_[left_part] = new_right_parts;
    }
  }

  void AddEpsWord() {
    int64_t max_nonterminal_idx = GetMaxNonterminalIdx();
    
    nonterminals_.insert(static_cast<char>(max_nonterminal_idx));
    rules_[static_cast<char>(max_nonterminal_idx)].push_back(std::string(1, start_nonterminal_));
    rules_[static_cast<char>(max_nonterminal_idx)].push_back("");
    start_nonterminal_ = static_cast<char>(max_nonterminal_idx);
  }

  void DeleteChainRules() {
    std::unordered_set<std::pair<char, char>, hash_pair> chain_pairs;
    for (const auto& nonterminal: nonterminals_) {
      chain_pairs.insert({nonterminal, nonterminal});
    }

    int64_t prev_chain_pairs_size;

    do {
      prev_chain_pairs_size = chain_pairs.size();

      for (const auto& chain_pair: chain_pairs) {
        for (const auto& [left_part, right_parts]: rules_) {
          if (chain_pair.second != left_part) {
            continue;
          }

          for (const auto& right_part: right_parts) {
            if (right_part.size() == 1 && nonterminals_.find(right_part[0]) != nonterminals_.end()) {
              chain_pairs.insert({chain_pair.first, right_part[0]});
            }
          }
        }
      }
    } while (chain_pairs.size() != prev_chain_pairs_size);

    for (auto& [left_part, right_parts]: rules_) {
      std::vector<std::string> new_right_parts;

      for (auto& right_part: right_parts) {
        if (right_part.size() == 1 && nonterminals_.find(right_part[0]) != nonterminals_.end()) {
          continue;
        }

        new_right_parts.push_back(right_part);
      }

      rules_[left_part] = new_right_parts;
    }

    for (const auto& chain_pair: chain_pairs) {
      if (chain_pair.first == chain_pair.second) {
        continue;
      }

      for (const auto& right_part: rules_[chain_pair.second]) {
        if (right_part.size() == 1 && nonterminals_.find(right_part[0]) != nonterminals_.end()) {
          continue;
        }

        rules_[chain_pair.first].push_back(right_part);
      }
    }
  }

  struct hash_pair {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        size_t hash1 = std::hash<T1>{}(p.first);
    
        size_t hash2 = std::hash<T2>{}(p.second);

        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
  };
};