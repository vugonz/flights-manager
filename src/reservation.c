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
	if(get_reservation_by_id(system, reservation_id) != NULL)
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
	if(new_reservation == NULL) {
		free(reservation_id);
		terminate_program(system);
	}

	new_reservation->nr_passengers = nr_passengers;
	new_reservation->id = reservation_id;

	add_reservation_to_list(f->reservations, new_reservation); 

	system->nr_reservations++;
	f->nr_passengers += nr_passengers;
	f->nr_reservations++;
}

/* 
 * Lists all reservations in flight with given id in alphabetical order
 */
void list_reservations(manager *system, char *flight_id, date *d)
{
	flight *f = get_flight_by_id_and_date(system, flight_id, *d);
	reservation *node;

	f->reservations->head = sort_list(f->reservations->head, f->nr_reservations);

	node = f->reservations->head;
	while(node != NULL) {
		printf("%s %d\n", node->id, node->nr_passengers);
		node = node->next;
	}
}

/*
 * Sorts given list of reservations in alphabetical order of reservation's id with mergesort
 */
reservation *sort_list(reservation *head, int size)
{
	int i;
	reservation *left = head;
	reservation *right = head;
	reservation *aux;
	
	/* empty list or single element list are already sorted */
	if(head == NULL || head->next == NULL)
		return head;

	/* split lists in the middle into two */
	for(i = 0; i < size - size/2 - 1; ++i)
		right = right->next;

	aux = right->next;
	right->next = NULL;
	right = aux;

	left = sort_list(left, size - size/2);
	right = sort_list(right, size/2);
	return merge(left, right);
}

reservation *merge(reservation *h1, reservation *h2)
{
	reservation *tail;
	reservation *aux;

	tail = strcmp(h1->id, h2->id) < 0 ? h1 : h2;
	if(tail == h1)
		h1 = h1->next;
	else
		h2 = h2->next;

	aux = tail;

	while(h1 != NULL && h2 != NULL) {
		if(strcmp(h1->id, h2->id) < 0) {
			aux->next = h1;
			h1 = h1->next;
		} else {
			aux->next = h2;
			h2 = h2->next;
		}
		aux = aux->next;
	}
	if(h1 != NULL) {
		while(h1 != NULL) {
			aux->next = h1;
			h1 = h1->next;
			aux = aux->next;
		}
	} else if (h2 != NULL){
		while(h2 != NULL) {
			aux->next = h2;
			h2 = h2->next;
			aux = aux->next;
		}
	}
	return tail;
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
			if((node = find_node_in_list(system->flights[i].reservations, id)) != NULL)
				return node;
			j += system->flights[i].nr_reservations;
		}

	return NULL;
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
