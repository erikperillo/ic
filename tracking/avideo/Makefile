CC = g++
LDEPDIR += `pkg-config opencv --cflags` 
LDEPLIB += `pkg-config opencv --libs` -lsfml-audio 
NAME = avideo

%.o: %.cpp $(LDEPDIR) $(LDEPLIB) 
	gcc -c -o $@ $< $(LDEPDIR) $(LDEPLIB)  

$(NAME): $(NAME).o 
	gcc -o $@ $^ ../blob/blob.o ../grid/grid.o ../ptcol/ptcol.o $(LDEPDIR) $(LDEPLIB)

clean: 
	rm -f $(NAME) *.o
