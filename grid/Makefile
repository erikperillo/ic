CC = g++
LDEPDIR += `pkg-config opencv --cflags`
LDEPLIB += `pkg-config opencv --libs` 
DEPS = grid.hpp

all: obj

obj: grid.o

blob.o: grid.cpp $(DEPS) 
	$(CC) -c -o grid.o grid.cpp 

test: grid.o test.cpp
	$(CC) -o test test.cpp grid.o $(LDEPDIR) $(LDEPLIB)  

clean: 
	rm -f $(NAME) *.o
