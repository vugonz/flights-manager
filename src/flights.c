/*  Author: Gonçalo Azevedo 193075
 *  File: flights.c
 */
#include "header.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Adds new flight to global structure if all given parameters are valid
 * If invalid parameters are given, returns an error value to handle function
 */
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

	/* check if max flights limit reached */
	if(system->nr_flights >= MAX_FLIGHTS)
		return -5;
	
	/* check if flight date is a year or less into the future */
	if(!is_valid_date(departure.date, system->date))
		return -6;

	if(!is_valid_duration(duration))
		return -7;
	
	/* check if flight capacity is met in range [10,100] */
	if(nr_passengers < MIN_FLIGHT_CAPACITY || nr_passengers > MAX_FLIGHT_CAPACITY)
		return -8;

	/* create new flight */
	new_flight = create_flight(id, origin, destination, 
			departure, duration, nr_passengers);

	/* insert new flight in system's sorted lists */
	insert_flight(system, new_flight);

	/* increment nr of flights in origin airport */
	++get_airport_by_id(system, origin)->nr_flights;

	/* increment global nr of flights */
	++system->nr_flights;

	return 0;
}

/* 
 * Inserts given flight in system's sorted lists
 */
void insert_flight(manager *system, flight new_flight)
{
	/* list will always be sorted by creation as long as new elements are appended in end */
	system->flights[system->nr_flights] = new_flight;	

	/* insert flight in list sorted by departure schedule */
	insert_sorted_flight(system->sorted_departure_flights, new_flight,
			system->nr_flights, compare_flight_departure);
	/* insert flight in list sorted by arrival schedule */
	insert_sorted_flight(system->sorted_arrival_flights, new_flight,
			system->nr_flights, compare_flight_arrival); 
}

/* 
 * Inserts given flight sorted with cmp_func as comparison method in given flights' list with "size" elements
 */
void insert_sorted_flight(flight *l, flight new_flight, int size, int (*cmp_func)(flight f1, flight f2))
{
	int i;
	
	l[size] = new_flight;

	/* inserts new flight in sorted position using cmp_func as comparison method */
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
		list_airport_flights_by_departure(system->sorted_departure_flights, 
				airport_id, system->nr_flights);
	/* if given command is 'c' */
	} else
		list_airport_flights_by_arrival(system->sorted_arrival_flights, 
				airport_id, system->nr_flights);
	
	return 0;
}

/*
 * Lists flights with origin in airport with given ID sorted by departure schedule 
 */
void list_airport_flights_by_departure(flight *l, char *airport_id, int size)
{
	int i;

	/* print flights in sorted by departure schedule array if they have given airport_id as origin */
	for(i = 0; i < size; ++i)
		if(!strcmp(l[i].origin, airport_id))
			print_flight_in_airport(l[i].id, l[i].destination, l[i].departure);
}

/* 
 * Lists flights with destination in airport with given ID sorted by arrival schedule 
 */
void list_airport_flights_by_arrival(flight *l, char *airport_id, int size)
{
	int i;

	/* print flights in sorted by arrival schedule array if they have given airport_id as destination */
	for(i = 0; i < size; ++i)
		if(!strcmp(l[i].destination, airport_id))
			print_flight_in_airport(l[i].id, l[i].origin, l[i].arrival);
}


/*
 * Returns a flight structure with given arguments as members 
 */
flight create_flight(char *id, char *origin, char *destination,
		schedule departure, time duration, int nr_passengers)
{
	flight new_flight;
	
	strcpy(new_flight.id, id);
	strcpy(new_flight.origin, origin);
	strcpy(new_flight.destination, destination);
	new_flight.nr_passengers = nr_passengers;

	new_flight.departure = departure;
	/* calculate arrival schedule for flight */
	new_flight.arrival = calculate_arrival(departure, duration);

	return new_flight;
}

/* 
 * Returns negative if f1 departs before f2,
 * Returns 0 if f1 and f2 depart at the same schedule
 * Returns positive if f1 departs after f2
 */
int compare_flight_departure(flight f1, flight f2)
{
	return schedule_cmp(f1.departure, f2.departure);
}

/*
 * Returns negative if f1 arrives before f2,
 * Returns 0 if flights arrive at the same schedule
 * Returns positive if f1 arrives after f2
 */
int compare_flight_arrival(flight f1, flight f2)
{
	return schedule_cmp(f1.arrival, f2.arrival);
}

/*
 * Returns 1 if given ID is a valid flight ID
 * Returns 0 if given ID is invalid
 */
int is_valid_flight_id(char *id)
{
	char c1, c2;
	int n;

	/* check if two first characters are upper case and if remaining characters form an integer in ]0, 9999]*/
	if(sscanf(id,"%c%c%d", &c1, &c2, &n) == 3) 
		return isupper(c1) && isupper(c2) && n > 0 && n < FLIGHT_MAX_NUM_ID ? 1 : 0;
	
	return 0;
}

/*
 * Returns 1 if given ID is already in use by a flight for given date
 * Returns 0 if given ID is free for a flight in given date
 */
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

/* 
 * Formatted print of flight structure to be used in 'v' command
 */
void print_flight(flight flight)
{
	printf(PRINT_FLIGHT_STR, flight.id, flight.origin, flight.destination,
			flight.departure.date.day, flight.departure.date.month, flight.departure.date.year,
			flight.departure.time.hour, flight.departure.time.minute);
}

/*
 * Formatted print of flight structure to be used in 'c' and 'p' commands
 */
void print_flight_in_airport(char *id, char *airport, schedule s)
{
	printf(PRINT_FLIGHT_IN_AIRPORT_STR, id, airport, 
			s.date.day, s.date.month, s.date.year,
			s.time.hour, s.time.minute);
}

