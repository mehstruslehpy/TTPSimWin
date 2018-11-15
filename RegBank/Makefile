all: main

main: stub.cpp RegBank.o
	g++ -g -Wall -pedantic stub.cpp RegBank.o -o stub

RegBank.o: RegBank.h RegBank.cpp
	g++ -g -Wall -pedantic RegBank.cpp -o RegBank.o -c
