SRC = $(wildcard server/*.c)

OBJ =  $(SRC:server/.c=.o)
NAME = start_server


all: $(OBJ)
	gcc -o $(NAME) $(OBJ)
$(OBJ) : $(SRC)
	gcc -c $(SRC) -g3

clean:
	rm -f *.o
	rm -f $(NAME)

fclean:
	rm -f *.o
	rm -f $(NAME)

re:		fclean all

