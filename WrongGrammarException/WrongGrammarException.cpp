#include "WrongGrammarException.hpp"

WrongGrammarException::WrongGrammarException(const std::string& message)
    : message_(message) {}
std::string WrongGrammarException::GetMessage() const { return message_; }