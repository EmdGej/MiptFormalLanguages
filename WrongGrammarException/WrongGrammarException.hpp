#include <string>

class WrongGrammarException {
 public:
  WrongGrammarException(const std::string& message);
  std::string GetMessage() const;

 private:
  std::string message_;
};