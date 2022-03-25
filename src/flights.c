/*  Author: Gonçalo Azevedo 193075
 *  File: flights.c
 */
#include "header.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int add_flight(manager *system, char *id, char *origin, char *destination, 
		schedule departure, time duration, int nr_passengers)
{
	flight new_flight;

	if(!is_valid_flight_id(id))
		return -1;
	
	/* check if flight id is taken for the day */
	if(is_taken_flight_id(system, id, departure.date))
		return -2;
	
	if(!exists_airport_id(system, origin))
		return -3;
	
	if(!exists_airport_id(system, destination))
		return -4;

	/* if max flights limit reached */
	if(system->nr_flights >= MAX_FLIGHTS)
		return -5;
	
	if(!is_valid_date(departure.date, system->date))
		return -6;

	if(!is_valid_duration(duration))
		return -7;
	
	/* check if plane capacity is met in range [10,100] */
	if(nr_passengers < MIN_FLIGHT_CAPACITY || nr_passengers > MAX_FLIGHT_CAPACITY)
		return -8;

	new_flight = create_flight(id, origin, destination, 
			departure, duration, nr_passengers);

	insert_flight(system, new_flight);

	++get_airport_by_id(system, origin)->nr_flights;

	++system->nr_flights;

	return 0;
}

void insert_flight(manager *system, flight new_flight)
{
	/* list will always be sorted by creation date as long as new elements are appended in end */
	system->flights[system->nr_flights] = new_flight;	

	/* insert flight in list sorted by departure schedule */
	insert_sorted_flight(system->sorted_departure_flights, new_flight,
			system->nr_flights, compare_flight_departure);
	/* insert flight in list sorted by arrival schedule */
	insert_sorted_flight(system->sorted_arrival_flights, new_flight,
			system->nr_flights, compare_flight_arrival); 
}

void insert_sorted_flight(flight *l, flight new_flight, int size, int (*cmp_func)(flight f1, flight f2))
{
	int i;
	
	l[size] = new_flight;

	for(i = size - 1; i >= 0; --i) {
		if(cmp_func(new_flight, l[i]) > 0) {
			l[i+1] = new_flight;
			break;
		} else
			l[i+1] = l[i];

		/* if ever reached with i = 0 then new element belongs in the first position */
		if(i == 0)
			l[i] = new_flight;
	}
}

/* lists flights sorted by creation */
void list_flights(manager *system)
{
	int i;

	for(i = 0; i < system->nr_flights; ++i)
		print_flight(system->flights[i]);
}

/* lists departing flights in airport with given id for 'p' command
 * lists arriving flights in airport with given id for 'c' command */
int list_flights_by_airport(manager *system, char *airport_id, char command)
{
	if(!exists_airport_id(system, airport_id))
		return -1;
	
	if(command == DEPARTURE) {
		list_airport_flights_by_departure(system->sorted_departure_flights, 
				airport_id, system->nr_flights);
	} else
		list_airport_flights_by_arrival(system->sorted_arrival_flights, 
				airport_id, system->nr_flights);
	
	return 0;
}

/* lists flights with origin in given airport sorted by departure schedule */
void list_airport_flights_by_departure(flight *l, char *airport_id, int size)
{
	int i;

	for(i = 0; i < size; ++i)
		if(!strcmp(l[i].origin, airport_id))
			print_flight_in_airport(l[i].id, l[i].destination, l[i].departure);
}

/* lists flights with destination in given airport sorted by arrival schedule */
void list_airport_flights_by_arrival(flight *l, char *airport_id, int size)
{
	int i;

	for(i = 0; i < size; ++i)
		if(!strcmp(l[i].destination, airport_id))
			print_flight_in_airport(l[i].id, l[i].origin, l[i].arrival);
}


/* creates a new flight structure with given members */
flight create_flight(char *id, char *origin, char *destination,
		schedule departure, time duration, int nr_passengers)
{
	flight new_flight;
	
	strcpy(new_flight.id, id);
	strcpy(new_flight.origin, origin);
	strcpy(new_flight.destination, destination);
	new_flight.nr_passengers = nr_passengers;

	new_flight.departure = departure;
	new_flight.arrival = calculate_arrival(departure, duration);

	return new_flight;
}

/* returns negative if f1 departs before f2, 
 * 0 if flights depart at the same schedule
 * and positive if f1 departs after f2 */
int compare_flight_departure(flight f1, flight f2)
{
	/* same departure instant */
	return compare_schedules(f1.departure, f2.departure);
}

/* returns negative if f1 arrives before f2, 
 * 0 if flights arrive at the same schedule 
 * and positive if f1 arrives after f2 */
int compare_flight_arrival(flight f1, flight f2)
{
	/* same departure instant */
	return compare_schedules(f1.arrival, f2.arrival);
}

/* returns 1 if given flight id is valid and 0 if it's not */
int is_valid_flight_id(char *id)
{
	char c1, c2;
	int n;

	if(sscanf(id,"%c%c%d", &c1, &c2, &n) == 3) 
		return isupper(c1) && isupper(c2) && n > 0 && n < FLIGHT_MAX_NUM_ID ? 1 : 0;
	
	return 0;
}

/* returns 1 if given flight id is taken for given date and 0 if it's not */
int is_taken_flight_id(manager *system, char *id, date date)
{
	int i;
	
	/* check if any flight has the same id and, if so, check if they have the same date */
	for(i = 0; i < system->nr_flights; ++i)
		if(!strcmp(system->flights[i].id, id) && 
				same_day(date, system->flights[i].departure.date))
			return 1;

	return 0;
}

/* prints flight in format required by the 'v' command */
void print_flight(flight flight)
{
	printf(PRINT_FLIGHT_STR, flight.id, flight.origin, flight.destination,
			flight.departure.date.day, flight.departure.date.month, flight.departure.date.year,
			flight.departure.time.hour, flight.departure.time.minute);
}

/* prints flight in format required by 'c' and 'p' commands */
void print_flight_in_airport(char *id, char *airport, schedule s)
{
	printf(PRINT_FLIGHT_IN_AIRPORT_STR, id, airport, 
			s.date.day, s.date.month, s.date.year,
			s.time.hour, s.time.minute);
}

