all: main.cpp
	#gcc -g -Wall -o main main.cpp
	g++ -g -std=c++0x main.cpp handlers.cpp parser.cpp digital.cpp

clean:
	$(RM) *.o *.out
