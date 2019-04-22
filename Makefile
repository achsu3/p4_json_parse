all: main.cpp
	#gcc -g -Wall -o main main.cpp
	g++ -g main.cpp

clean:
	$(RM) main
