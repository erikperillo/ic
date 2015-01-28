CC = g++
SRC_DIR = ../..
LDEPDIR += `pkg-config opencv --cflags` 
LDEPLIB += `pkg-config opencv --libs` -lsfml-audio -lsfml-system
NAME = avideo
OBJS = $(SRC_DIR)/blob/blob.o ../../grid/grid.o ../../ptcol/ptcol.o

all: $(NAME)

obj: avideo.o

$(NAME): avideo.o $(OBJS)
	$(CC) -o avideo avideo.o $(OBJS) $(LDEPDIR) $(LDEPLIB)

avideo.o: avideo.cpp 
	$(CC) -c -o avideo.o avideo.cpp 

$(SRC_DIR)/blob/blob.o:
	cd $(SRC_DIR)/blob/ && make

$(SRC_DIR)/grid/grid.o:
	cd $(SRC_DIR)/grid/ && make

$(SRC_DIR)/ptcol/ptcol.o:
	cd $(SRC_DIR)/ptcol/ && make

clean: 
	rm -f $(NAME) *.o
