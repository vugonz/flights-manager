#include "header.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int add_flight(manager *system, char *id, char *origin, char *destination, 
		date flight_date, time flight_time, time duration, int nr_passengers)
{
	flight new_flight;

	/* max airport limit reached */
	if(system->nr_flights > MAX_FLIGHTS)
		return -1;

	/* check if flight id is valid */
	if(!is_valid_flight_id(id))
		return -2;
	
	/* check if flight id is taken for the day */
	if(is_taken_flight_id(system, id, flight_date))
		return -3;

	if(!exists_airport_id(system, origin))
		return -4;

	if(!exists_airport_id(system, destination))
		return -5;
	
	if(!is_valid_date(system->date, flight_date))
		return -6;

	if(!is_valid_duration(duration))
		return -7;
	
	/* check if plane capacity is met in range [10,100] */
	if(nr_passengers < 10 || nr_passengers > 100)
		return -8;
	
	new_flight = create_flight(id, origin, destination, 
			flight_date, flight_time, duration, nr_passengers);
	
	print_flight(new_flight);

	return 0;
}


int is_valid_flight_id(char *id)
{
	char c1, c2;
	int n;

	if(sscanf(id, "%c%c%d", &c1, &c2, &n) == 3) 
		return isupper(c1) && isupper(c2) && n > 0 ? 1 : 0;
	
	return 0;
}


int is_taken_flight_id(manager *system, char *id, date date)
{
	int i;
	
	/* check if any flight has the same id and, if so, check if they have the same date */
	for(i = 0; i < system->nr_flights; ++i)
		if(!strcmp(system->flights[i].id, id) && same_day(system->flights[i].date, date))
			return 1;

	return 0;
}

void list_all_flights(manager *system)
{
	int i;

	for(i = 0; i < system->nr_flights; ++i)
		print_flight(system->flights[i]);
}

void print_flight(flight flight)
{
	printf(PRINT_FLIGHT_STR, flight.id, flight.origin, flight.destination,
			flight.date.day, flight.date.month, flight.date.day,
			flight.time.hour, flight.time.minute);
}

flight create_flight(char *id, char *origin, char *destination,
		date flight_date, time flight_time, time duration, int nr_passengers)
{
	flight new_flight;

	strcpy(new_flight.id, id);
	strcpy(new_flight.origin, origin);
	strcpy(new_flight.destination, destination);
	new_flight.date = flight_date;
	new_flight.time = flight_time;
	new_flight.duration = duration;
	new_flight.nr_passengers = nr_passengers;

	return new_flight;
}

