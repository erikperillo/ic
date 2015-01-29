CC = g++
SRC_DIR = ../..
LDEPDIR += `pkg-config opencv --cflags` 
LDEPLIB += `pkg-config opencv --libs` -lsfml-audio -lsfml-system
NAME = mul_objs
OBJS = $(SRC_DIR)/blob/blob.o ../../grid/grid.o ../../ptcol/ptcol.o ../../obj/obj.o ../../clarg/clarg.o

all: $(NAME)

obj: $(NAME).o

$(NAME): $(NAME).o $(OBJS)
	$(CC) -g -o $(NAME) $(NAME).o $(OBJS) $(LDEPDIR) $(LDEPLIB)

$(NAME).o: $(NAME).cpp 
	$(CC) -c -o $(NAME).o $(NAME).cpp 

$(SRC_DIR)/blob/blob.o:
	cd $(SRC_DIR)/blob/ && make

$(SRC_DIR)/grid/grid.o:
	cd $(SRC_DIR)/grid/ && make

$(SRC_DIR)/ptcol/ptcol.o:
	cd $(SRC_DIR)/ptcol/ && make

$(SRC_DIR)/obj/obj.o:
	cd $(SRC_DIR)/obj/ && make

$(SRC_DIR)/clarg/clarg.o:
	cd $(SRC_DIR)/clarg/ && make clarg.o

clean: 
	rm -f $(NAME) *.o
