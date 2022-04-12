/*  Author: Gonçalo Azevedo 93075
 *  File: reservations.c
 */
#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Adds reservation to the system if all arguments are valid
 */
int validate_reservation(manager *system, char *buffer, char **reservation_id,
		int size_id, char *flight_id, date *d, int nr_passengers)
{
	flight *f;

	/* check if reservation id is valid */
	if(size_id < 10 || !is_valid_reservation_id(buffer, size_id))
		return -1;

	/* read reservation id */
	*reservation_id = (char *)malloc((size_id + 1) * sizeof(char));
	sscanf(buffer, "%s", *reservation_id);
	/* if no memory */
	if(reservation_id == NULL)
		terminate_program(system);

	/* check if given flight id exists for given date */
	f = get_flight_by_id_and_date(system, flight_id, d);	
	if(f == NULL)
		return -2;

	/* check if reservation id is already in use */
	if(get_reservation_by_id(system, *reservation_id) != NULL)
		return -3;

	/* check if flight capacity is not exceeded */	
	if(f->nr_passengers + nr_passengers > f->capacity)
		return -4;

	/* check if date is valid */
	if(!is_valid_date(&system->date, d))
		return -5;

	/* check if reservation capacity is valid */
	if(nr_passengers <= 0)
		return -6;

	create_reservation(system, f, *reservation_id, nr_passengers);
	
	return 0;
}

/*
 * Creates a new reservation and adds it to the flight's list
 */
void create_reservation(manager *system, flight *f, char *reservation_id, int nr_passengers)
{
	reservation *new_reservation;	
	
	new_reservation = (reservation*)malloc(sizeof(reservation));
	if(new_reservation == NULL) {
		free(reservation_id);
		terminate_program(system);
	}

	new_reservation->nr_passengers = nr_passengers;
	new_reservation->id = reservation_id;

	add_node(f->reservations, new_reservation); 

	f->nr_passengers += nr_passengers;
	f->nr_reservations++;
	system->nr_reservations++;
}

/* 
 * Lists all reservations in flight with given id in alphabetical order
 */
int list_reservations(manager *system, char *flight_id, date *d)
{
	flight *f = get_flight_by_id_and_date(system, flight_id, d);

	/* check if flight exists */
	if(f == NULL)
		return -1;

	/* check if date is valid */
	if(!is_valid_date(&system->date, d))
		return -2;	

	print_reservations_in_flight(f);

	return 0;
}

void print_reservations_in_flight(flight *f)
{
	f->reservations->head = sort_list(f->reservations->head, f->nr_reservations);

	print_list(f->reservations);
}

/*
 * Returns 0 if reservation with given id was sucessfully removed from the system
 * Removes -1 if reservation with given id doesn't exist
 */
int remove_reservation(manager *system, char *id)
{
	int i = 0;
	int res;

	/* traverse all reservations until reservation with given id is removed */
	while(i < system->nr_flights &&
			(res = remove_node(system->flights[i].reservations, id)) == -1)
		++i;

	if(res != 0) {
		--system->nr_reservations;
		system->flights[i].nr_passengers -= res; 
		--system->flights[i].nr_reservations;
	}

	return res;
}

/*
 * Returns pointer to reservation with given id
 * If reservation with given id doesn't exist, returns NULL pointer
 */
reservation *get_reservation_by_id(manager *system, char *id)
{
	int i, j = 0;
	reservation *node;

	/* iterate over all flights lists and look for matching node in each list until no more reservations */
	for(i = 0; i < system->nr_flights && j != system->nr_reservations; ++i)
		if(system->flights[i].nr_reservations > 0) {
			if((node = find_node_in_list
						(system->flights[i].reservations, id)) != NULL)
				return node;
			j += system->flights[i].nr_reservations;
		}

	return NULL;
}


/* Reads reservation id from buffer
 * If reservation id is invalid returns -1
 * If reservation id is valid returns reservation's length
 */
int is_valid_reservation_id(char *buffer, int size)
{
	int i = 0;

	/* check if reservation's id characters are valid */
	for(i = 0; i < size; ++i)
		if(!((buffer[i] >= '0' && buffer[i] <= '9') ||
					(buffer[i] >= 'A' && buffer[i] <= 'Z')))
			return 0;

	return 1;
}

/* Scans reservation id from given buffer and returns the length of read id */
int get_reservation_id_size(char *buffer)
{
	int i = 0;

	while(buffer[i] != ' ' && buffer[i++] != '\t')
		;

	return i;
}

/* 
 * Reads flight id and date from given buffer to given char and date pointers
 * Sets given buffer pointer to character after read input
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
 * Sets given buffer pointer to next character that is not a whitspace
 */
void ignore_whitespaces(char **buffer)
{
	int i = 0;

	while(*(*buffer + i) == ' ' || *(*buffer + i) == '\t')
		i++;

	*buffer += i;
}
