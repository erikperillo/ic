CC = g++
LDEPDIR += `pkg-config opencv --cflags`
LDEPLIB += `pkg-config opencv --libs`
NAME = colclb

%.o: %.cpp $(LDEPDIR) $(LDEPLIB)
	$(CC) -c -o $@ $< $(LDEPDIR) $(LDEPLIB) 

$(NAME): $(NAME).o 
	$(CC) -o $@ $^ $(LDEPDIR) $(LDEPLIB)

clean: 
	rm -f $(NAME) *.o
