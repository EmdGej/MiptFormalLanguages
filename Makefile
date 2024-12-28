all: main
	./a.out	

main: main.o WrongGrammarException.o Grammar.o InputManager.o Check.o
	g++ -std=c++17 main.o WrongGrammarException.o Grammar.o InputManager.o Check.o

main.o: main.cpp
	g++ -std=c++17 -c main.cpp

Check.o: Check/Check.cpp Check/Check.hpp
	g++ -std=c++17 -c Check/Check.cpp

WrongGrammarException.o: WrongGrammarException/WrongGrammarException.cpp WrongGrammarException/WrongGrammarException.hpp
	g++ -std=c++17 -c WrongGrammarException/WrongGrammarException.cpp

Grammar.o: Grammar/Grammar.cpp Grammar/Grammar.hpp
	g++ -std=c++17 -c Grammar/Grammar.cpp

InputManager.o: InputManager/InputManager.cpp InputManager/InputManager.hpp
	g++ -std=c++17 -c InputManager/InputManager.cpp

clear:
	rm *.o
	rm ./a.out