all: main.cpp
	#gcc -g -Wall -o main main.cpp
	g++ -g main.cpp handlers.cpp parser.cpp

clean:
	$(RM) main
