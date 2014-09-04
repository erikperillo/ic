CC = g++
LDEPDIR += `pkg-config opencv --cflags`
LDEPLIB += `pkg-config opencv --libs` 
DEPS = ptcol.hpp

all: obj

obj: ptcol.o

blob.o: ptcol.cpp $(DEPS) 
	$(CC) -c -o ptcol.o ptcol.cpp 

test: ptcol.o test.cpp
	$(CC) -o test test.cpp ptcol.o $(LDEPDIR) $(LDEPLIB)  

clean: 
	rm -f $(NAME) *.o
