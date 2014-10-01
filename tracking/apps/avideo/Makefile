CC = g++
LDEPDIR += `pkg-config opencv --cflags` 
LDEPLIB += `pkg-config opencv --libs` -lsfml-audio -lsfml-system -lpthread
NAME = avideo
OBJS = ../../blob/blob.o ../../grid/grid.o ../../ptcol/ptcol.o ../../.cfgen/cfgen.o

all: $(NAME)

obj: avideo.o

$(NAME): avideo.o $(OBJS)
	$(CC) -o avideo avideo.o $(OBJS) $(LDEPDIR) $(LDEPLIB)

avideo.o: avideo.cpp 
	$(CC) -c -o avideo.o avideo.cpp 

../blob/blob.o:
	cd ../blob/ && make

../grid/grid.o:
	cd ../grid/ && make

../ptcol/ptcol.o:
	cd ../ptcol/ && make

clean: 
	rm -f $(NAME) *.o
