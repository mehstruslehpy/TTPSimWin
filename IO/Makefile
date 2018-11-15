all: main

main: stub.cpp IO.o
	g++ -g -Wall -pedantic stub.cpp IO.o -o stub

IO.o: IO.h IO.cpp
	g++ -g -Wall -pedantic IO.cpp -o IO.o -c
