CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic -g
OBJECTS = proj2.o airports.o flights.o schedule.o structures.o reservation.o

all:
	make proj2

proj2: $(OBJECTS)
	$(CC) $(CFLAGS) -o proj2 $(OBJECTS)

%(OBJECTS): %.c header.h

clean:
	rm proj2 *.o
