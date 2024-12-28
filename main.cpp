#include "Check/Check.hpp"
#include "InputManager/InputManager.hpp"

int main() {
  int64_t n, sigma, p;
  std::cin >> n >> sigma >> p;

  InputManager input_manager(n, sigma, p);
  input_manager.ReadNonterminals();
  input_manager.ReadAlphabet();
  input_manager.ReadRules();
  input_manager.ReadStartNonterminal();

  Grammar grammar(input_manager.GetNonterminals(), input_manager.GetAlphabet(),
                  input_manager.GetRules(),
                  input_manager.GetStartNonterminal());

  CheckAlgorithm check;
  check.Fit(grammar);

  int64_t q;
  std::cin >> q;

	std::string word;
	std::getline(std::cin, word);
  for (size_t i = 0; i < q; ++i) {
    std::getline(std::cin, word); 
    std::cout << (check.Predict(word) ? "yes" : "no") << '\n';
  }
}