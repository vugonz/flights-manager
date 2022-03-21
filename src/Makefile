CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic -g
OBJECTS = proj1.o airports.o flights.o time.o date.o schedule.o

all:
	make proj1

proj1: $(OBJECTS)
	$(CC) $(CFLAGS) -o proj1 $(OBJECTS)

%(OBJECTS): %.c header.h

clean:
	rm proj1 *.o
