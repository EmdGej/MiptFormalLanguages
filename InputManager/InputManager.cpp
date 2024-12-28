#include "InputManager.hpp"

InputManager::InputManager(int64_t nonterminals_number, int64_t alphabet_size,
                           int64_t rules_number)
    : nonterminals_number_(nonterminals_number),
      alphabet_size_(alphabet_size),
      rules_number_(rules_number) {}

void InputManager::ReadNonterminals() {
  for (int64_t i = 0; i < nonterminals_number_; ++i) {
    char nonterminal;
    std::cin >> nonterminal;
    nonterminals_.insert(nonterminal);
  }
}

void InputManager::ReadAlphabet() {
  for (int64_t i = 0; i < alphabet_size_; ++i) {
    char symbol;
    std::cin >> symbol;
    alphabet_.insert(symbol);
  }
}

void InputManager::ReadRules() {
  std::string line;
	std::getline(std::cin, line);

  for (int64_t i = 0; i < rules_number_; ++i) {
    std::getline(std::cin, line);

    std::string left_part = "";
    std::string right_part = "";

    int64_t cur_pos = 0;
    while (line[cur_pos] != '-') {
      if (line[cur_pos] == ' ') {
        ++cur_pos;
        continue;
      }

      left_part += line[cur_pos];
      ++cur_pos;
    }

    cur_pos += 2;
    while (cur_pos < line.size()) {
			if (line[cur_pos] == ' ') {
				++cur_pos;
				continue;
			}

      right_part += line[cur_pos];
      ++cur_pos;
    }

    if (left_part.size() == 1) {
      rules_[left_part[0]].push_back(right_part);
    } else {
      throw std::invalid_argument("Grammar is not context free");
    }
  }
}

void InputManager::ReadStartNonterminal() {
	std::cin >> start_nonterminal_;
}