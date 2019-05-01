all: main.cpp
	g++ -g main.cpp
	#g++ -Wall -g -c -o digital.o digital.cpp
	#g++ -Wall -g -o main main.o digital.o

clean:
	$(RM) main

#main.o: main.cpp parser.h digital.h

#digital.o: digital.h

