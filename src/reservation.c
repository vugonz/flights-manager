#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Adds reservation to the system if all arguments are valid
 */
int validate_reservation(manager *system, char *reservation_id, char *flight_id, date *d, int nr_passengers)
{
	/* check if given flight id exists for given date */
	flight *f = get_flight_by_id_and_date(system, flight_id, *d);	
	if(f == NULL)
		return -2;
	/* check if reservation id is already in use */
	if(2 == 1)
		return -3;
	/* check if flight capacity is not exceeded */	
	if(f->nr_passengers + nr_passengers > f->capacity)
		return -4;
	/* check if date is valid */
	if(!is_valid_date(system->date, *d))
		return -5;
	/* check if reservation capacity is valid */
	if(nr_passengers <= 0)
		return -6;

	add_reservation(system, f, reservation_id, nr_passengers);
	
	return 0;
}

/*
 * Creates a new reservation and adds it to the flight's list
 */
void add_reservation(manager *system, flight *f, char *reservation_id, int nr_passengers)
{
	reservation *new_reservation;	
	
	new_reservation = (reservation*)malloc(sizeof(reservation));
	if(new_reservation == NULL)
		terminate_program(system);

	new_reservation->nr_passengers = nr_passengers;
	new_reservation->id = reservation_id;

	add_reservation_to_list(f->reservations, new_reservation); 

	system->nr_reservations++;
	f->nr_passengers += nr_passengers;
	f->nr_reservations++;
}

void list_reservations(manager *system, char *flight_id, date *d)
{
	flight *f = get_flight_by_id_and_date(system, flight_id, *d);
	reservation *i = f->reservations->head;

	while(i != NULL) {
		printf("%s %d\n", i->id, i->nr_passengers);
		i = i->next;
	}
}
/* Reads reservation id from buffer
 * If reservation id is invalid returns -1
 * If reservation id is valid returns reservation's length
 */
int read_reservation_id(char *buffer)
{
	int i = 0;

	/* check if reservation's id characters are valid */
	while(buffer[i] != ' ' && buffer[i] != '\t') {
		if(!((buffer[i] >= '0' && buffer[i] <= '9') || (buffer[i] >= 'A' && buffer[i] <= 'Z')))
			return -1;
		++i;
	}
	
	/* if reservation id is less than 10 characters long */
	if(i < 10)
		return -1;

	/* return size of reservation id if valid */
	return i;
}

/* 
 * Reads flight id and date from given buffer string and sets given flight_id and date with read input
 * Sets given buffer char* pointing to character after read input
 */
void read_date_and_flight_id(char **buffer, char *flight_id, date *d)
{
	/* ignore trailling whitspaces */
	ignore_whitespaces(buffer);
	
	/* get flight id */
	sscanf(*buffer, "%s", flight_id);
	*buffer += strlen(flight_id);
	
	/* ignore middle whitspaces */
	ignore_whitespaces(buffer);

	/* get date members */
	sscanf(*buffer, DATE_MEMBERS_PARSE, &d->day, &d->month, &d->year);
	*buffer += 10;

	/* ignore trailling whitespaces */
	ignore_whitespaces(buffer);
}

/* 
 * Skips trailing whitspaces in given buffer and updates buffer to next character that is not a whitespace
 */
void ignore_whitespaces(char **buffer)
{
	int i = 0;

	while(*(*buffer + i) == ' ' || *(*buffer + i) == '\t')
		i++;

	*buffer += i;
}
