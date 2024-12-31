SRC = $(wildcard Server/*.c)

OBJ =  $(SRC:Server/.c=.o)
NAME = server


all: $(OBJ)
	gcc -o $(NAME) $(OBJ)
$(OBJ) : $(SRC)
	gcc -c $(SRC) -g3

clean:
	rm -f *.o
	rm -f $(NAME)

re:
	make clean

