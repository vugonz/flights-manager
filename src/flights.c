/*  Author: Gonçalo Azevedo 193075
 *  File: flights.c
 */
#include "header.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Adds new flight to global structure if all given arguments are valid
 * If invalid arguments are given, returns an error value to handle function
 */
int add_flight(manager *system, char *id, char *origin, char *destination,
		date *d, time *t, time *dur, int capacity)
{
	/* check if flight id is valid */
	if(!is_valid_flight_id(id))
		return -1;

	/* check if flight id is taken for the day */
	if(get_flight_by_id_and_date(system, id, d) != NULL)
		return -2;
	
	/* check if airports ids exist */
	if(!exists_airport_id(system, origin))
		return -3;
	if(!exists_airport_id(system, destination))
		return -4;

	/* check if max flights limit reached */
	if(system->nr_flights >= MAX_FLIGHTS)
		return -5;

	/* check if flight date is a year or less into the future */
	if(!is_valid_date(&system->date, d))
		return -6;

	/* check if duration is valid */
	if(!is_valid_duration(dur))
		return -7;

	/* check if flight capacity is smaller than 10 */
	if(capacity < MIN_FLIGHT_CAPACITY)
		return -8;

	/* create new flight */
	create_flight(system, id, origin, destination,
			d, t, dur, capacity);
	
	/* increment nr of flights in origin airport */
	++get_airport_by_id(system, origin)->nr_flights;

	return 0;
}

/*
 * Lists flights in system sorted by creation 
 */
void list_flights(manager *system)
{
	int i;

	for(i = 0; i < system->nr_flights; ++i)
		print_flight(system->flights[i]);
}

/*
 * If 'p' command is given, lists departing flights in airport with given ID
 * If 'c' command is given, lists arriving flights in airport with given ID
 */
int list_flights_by_airport(manager *system, char *airport_id, char command)
{
	if(!exists_airport_id(system, airport_id))
		return -1;

	/* if given command is 'p' */
	if(command == DEPARTURE) {
		list_airport_flights_by_departure(system, airport_id);
	/* if given command is 'c' */
	} else
		list_airport_flights_by_arrival(system, airport_id);

	return 0;
}

/*
 * Lists flights sorted by departure schedule with origin in airport with given ID
 */
void list_airport_flights_by_departure(manager *system, char *airport_id)
{
	int i, n = 0;
	int indexes[MAX_FLIGHTS];


	for(i = 0; i < system->nr_flights; ++i)
		if(!strcmp(system->flights[i].origin, airport_id))
			indexes[n++] = i;

	bubblesort(system, indexes, n, compare_flight_departure);

	for(i = 0; i < n; ++i)
		print_departing_flight(system, indexes[i]);
}

/*
 * Lists flights sorted by arrival schedule with destination in airport with given ID
 */
void list_airport_flights_by_arrival(manager *system, char *airport_id)
{
	int i, n = 0;
	int indexes[MAX_FLIGHTS];


	for(i = 0; i < system->nr_flights; ++i)
		if(!strcmp(system->flights[i].destination, airport_id))
			indexes[n++] = i;

	bubblesort(system, indexes, n, compare_flight_arrival);

	for(i = 0; i < n; ++i)
		print_arriving_flight(system, indexes[i]);
}


/*
 * Adds a flight to the system with given arguments as members
 */
void create_flight(manager *system, char *id, char *origin, char *destination,
		date *d, time *t, time *dur, int capacity)
{
	flight new_flight;

	strcpy(new_flight.id, id);
	strcpy(new_flight.origin, origin);
	strcpy(new_flight.destination, destination);
	new_flight.capacity = capacity;
	new_flight.date = *d;
	new_flight.time = *t;

	/* calculate departure's schedule numeric value */ 
	new_flight.departure_schedule = 
		convert_date_time_to_int(d, t);
	/* calculate arrival schedule numeric value */
	new_flight.arrival_schedule = new_flight.departure_schedule +
		convert_time_to_int(dur);
	
	/* initialize reservations list */
	new_flight.reservations = (list *)malloc(sizeof(list));
	/* check for memory limit exceeded */
	if(new_flight.reservations == NULL)
		terminate_program(system);

	init_list(new_flight.reservations);
	
	new_flight.nr_passengers = 0;
	new_flight.nr_reservations = 0;
	

	system->flights[system->nr_flights] = new_flight;
	system->nr_flights++;
}

/*
 * Returns negative if f1 departs before f2,
 * Returns 0 if f1 and f2 depart at the same schedule
 * Returns positive if f1 departs after f2
 */
int compare_flight_departure(manager *system, int a, int b)
{
	return system->flights[a].departure_schedule > system->flights[b].departure_schedule;
}

/*
 * Returns negative if f1 arrives before f2,
 * Returns 0 if flights arrive at the same schedule
 * Returns positive if f1 arrives after f2
 */
int compare_flight_arrival(manager *system, int a, int b)
{
	return system->flights[a].arrival_schedule > system->flights[b].arrival_schedule;
}

/*
 * Returns 1 if given ID is a valid flight ID
 * Returns 0 if given ID is invalid
 */
int is_valid_flight_id(char *id)
{
	char c1, c2;
	int n;

	/* check if two first characters are upper case and if remaining characters form an integer in range ]0, 10000[ */
	if(sscanf(id,"%c%c%d", &c1, &c2, &n) == 3) 
		return isupper(c1) && isupper(c2) && n > 0 && n < FLIGHT_MAX_NUM_ID ? 1 : 0;

	return 0;
}

/*
 * Returns pointer to flight with given ID for given date
 * Returns NULL pointer if flight doesn't exist
 */
flight *get_flight_by_id_and_date(manager *system, char *id, date *d)
{
	int i;
	int n_date = convert_date_to_int(d);

	/* check if any flight has the same id and, if so, check if they have the same date */
	for(i = 0; i < system->nr_flights; ++i)
		if(!strcmp(system->flights[i].id, id) &&
				convert_date_to_int(&system->flights[i].date) == n_date)
			return &system->flights[i];

	return NULL;
}

/*
 * Formatted print of flight structure to be used in 'v' command
 */
void print_flight(flight flight)
{
	printf(PRINT_FLIGHT_STR, flight.id, flight.origin, flight.destination,
			flight.date.day, flight.date.month, flight.date.year,
			flight.time.hour, flight.time.minute);
}

/*
 * Formatted print of flight structure to be used in 'p' commands
 */
void print_departing_flight(manager *system, int index)
{
	/* print flight's id and destination airport's id */
	printf("%s %s ", system->flights[index].id, system->flights[index].destination);

	/* print arrival date */
	print_date(&system->flights[index].date);
	printf(" ");
	/* print arrival time */
	print_time(&system->flights[index].time);
	printf("\n");
}
/*
 * Formatted print of flight structure to be used in 'c' commands
 */
void print_arriving_flight(manager *system, int index)
{
	time t = convert_int_to_time(system->flights[index].arrival_schedule);
	date d = convert_int_to_date(system->flights[index].arrival_schedule);
	
	/* print flight's id and origin airport's id */
	printf("%s %s ", system->flights[index].id, system->flights[index].origin);

	/* print departure date */
	print_date(&d);
	printf(" ");
	/* print departure time */
	print_time(&t);
	printf("\n");
}
