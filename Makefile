all: main.cpp
	g++ -std=c++11 -g main.cpp handlers.cpp parser.cpp digital.cpp

clean:
	$(RM) -rf *.o *.out *.dSYM
