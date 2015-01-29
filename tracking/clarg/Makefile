all: test

test: test.cpp clarg.o
	g++ -o test test.cpp clarg.o

clarg.o: clarg.cpp clarg.hpp
	g++ -c -o clarg.o clarg.cpp
