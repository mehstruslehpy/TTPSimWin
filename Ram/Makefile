all: main

main: stub.cpp Ram.o
	g++ -g -Wall -pedantic stub.cpp Ram.o -o stub

RegBank.o: Ram.h Ram.cpp
	g++ -g -Wall -pedantic Ram.cpp -o Ram.o -c
