CC = g++
LDEPDIR += `pkg-config opencv --cflags`
LDEPLIB += `pkg-config opencv --libs` 
NAME = ptcol
OBJS = ptcol.o
DEPS = grid.hpp grid.cpp

all: ptcol.o

%.o: %.cpp $(LDEPDIR) $(LDEPLIB) $(DEPS)
	$(CC) -c -o $@ $< $(LDEPDIR) $(LDEPLIB) 

#$(NAME): $(OBJS)
#	$(CC) -o $@ $^ $(LDEPDIR) $(LDEPLIB) 

clean: 
	rm -f $(NAME) *.o
