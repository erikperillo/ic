CC = g++
LDEPDIR += `pkg-config opencv --cflags`
LDEPLIB += `pkg-config opencv --libs` 
NAME = obj
DEPS = $(NAME).hpp

all: $(NAME)

$(NAME).o: $(NAME).cpp $(NAME).hpp
	$(CC) -c -o $(NAME).o $(NAME).cpp

$(NAME): $(NAME).o
	$(CC) -g -o $(NAME) $(NAME).o ../blob/blob.o ../grid/grid.o $(LDEPDIR) $(LDEPLIB)

clean: 
	rm -f $(NAME) *.o
