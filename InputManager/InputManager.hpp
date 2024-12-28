#pragma once

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class InputManager {
 public:
  InputManager(int64_t nonterminals_number, int64_t alphabet_size, int64_t rules_number); 

	void ReadNonterminals();
	void ReadAlphabet();
	void ReadRules();
	void ReadStartNonterminal();

	const std::unordered_set<char>& GetNonterminals() const { return nonterminals_; }
	const std::unordered_set<char>& GetAlphabet() const { return alphabet_; }
	const std::unordered_map<char, std::vector<std::string>>& GetRules() const { return rules_; }
	char GetStartNonterminal() const { return start_nonterminal_; }

 private:
  int64_t nonterminals_number_;
	int64_t alphabet_size_;
	int64_t rules_number_;

	std::unordered_set<char> nonterminals_;
	std::unordered_set<char> alphabet_;
	std::unordered_map<char, std::vector<std::string>> rules_;
	char start_nonterminal_;
};