#include "Grammar.hpp"

Grammar::Grammar(
    const std::unordered_set<char>& nonterminals,
    const std::unordered_set<char>& alphabet,
    const std::unordered_map<char, std::vector<std::string>>& rules,
    char start_nonterminal)
    : nonterminals_(nonterminals),
      alphabet_(alphabet),
      rules_(rules),
      start_nonterminal_(start_nonterminal) {
  if (!IsCorrectGrammar() || nonterminals_.find(start_nonterminal_) == nonterminals.end()) {
    throw WrongGrammarException("Grammar is not correct");
  }

  GrammarToNormalForm();
}