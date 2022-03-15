CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic -g
OBJECTS= proj1.o airports.o

all:
	make proj1

proj1: $(OBJECTS)
	$(CC) $(CFLAGS) -o proj1 $(OBJECTS)

proj1.o: proj1.c header.h

airports.o: airports.c header.h

clean:
	rm proj1 *.o
