CC = g++
LDEPDIR += `pkg-config opencv --cflags`
LDEPLIB += `pkg-config opencv --libs` 
DEPS = blob.hpp colors.hpp

all: obj

obj: blob.o

blob.o: blob.cpp $(DEPS) 
	$(CC) -c -o blob.o blob.cpp 

test: blob.o test.cpp
	$(CC) -o test test.cpp blob.o $(LDEPDIR) $(LDEPLIB)  

clean: 
	rm -f $(NAME) *.o
