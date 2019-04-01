all: main.cpp
	#gcc -g -Wall -o main main.cpp
	g++ main.cpp

clean:
	$(RM) main
