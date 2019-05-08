all: main.cpp
	#gcc -g -Wall -o main main.cpp
	g++ -g main.cpp handlers.cpp parser.cpp digital.cpp

clean:
	$(RM) main

#main.o: main.cpp parser.h digital.h

#digital.o: digital.h
