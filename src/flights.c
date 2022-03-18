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
	if(nr_passengers < MIN_FLIGHT_CAPACITY || nr_passengers > MAX_FLIGHT_CAPACITY)
		return -8;
	
	new_flight = create_flight(id, origin, destination, 
			flight_date, flight_time, duration, nr_passengers);
	
	insert_flight(system->flights, system->sorted_departure_flights, new_flight, system->nr_flights);

	++system->nr_flights;

	return 0;
}

/* inserts flight in list sorted by creation and list sorted by date and time departure */
void insert_flight(flight *l, flight *sorted_l, flight new_flight, int size)
{
	int i;

	/* list will always be sorted by creation date as long as new elements are appended to the end */
	l[size] = new_flight;	
	
	/* insert flight in list sorted by date */
	sorted_l[size] = new_flight;
	
	for(i = size - 1; i >= 0; --i) {
		if(compare_flight_departure(sorted_l[i], new_flight) > 0) {
			sorted_l[i+1] = new_flight;
			break;
		} else
			sorted_l[i+1] = sorted_l[i];

		/* if ever reached with i = 0 then new element belongs in the first position */
		if(i == 0)
			sorted_l[i] = new_flight;
	}
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
		if(!strcmp(system->flights[i].id, id) && same_day(system->flights[i].date_departure, date))
			return 1;

	return 0;
}

void list_all_flights(manager *system)
{
	int i;

	for(i = 0; i < system->nr_flights; ++i)
		print_flight(system->flights[i]);
	
	printf("\nnow sorted \n");

	for(i = 0; i < system->nr_flights; ++i)
		print_flight(system->sorted_departure_flights[i]);
}

void print_flight(flight flight)
{
	printf(PRINT_FLIGHT_STR, flight.id, flight.origin, flight.destination,
			flight.date_departure.day, flight.date_departure.month, flight.date_departure.year,
			flight.time_departure.hour, flight.time_departure.minute);
}

flight create_flight(char *id, char *origin, char *destination,
		date flight_date, time flight_time, time duration, int nr_passengers)
{
	flight new_flight = {0};

	strcpy(new_flight.id, id);
	strcpy(new_flight.origin, origin);
	strcpy(new_flight.destination, destination);
	new_flight.nr_passengers = nr_passengers;
	
	calculate_arrival_time_and_date(&new_flight, duration);

	return new_flight;
}

/* returns negative if f2 departs before f1, 0 if flights depart at the same instant and positive if f2 deprats after f1 */
int compare_flight_departure(flight f1, flight f2)
{
	/* same departure instant */
	if(!date_compare(f1.date_departure, f2.date_departure) && !time_compare(f1.time_departure, f2.time_departure))
		return 0;
	
	return !date_compare(f1.date_departure, f2.date_departure) ? time_compare(f1.time_departure, f2.time_departure) : date_compare(f1.date_departure, f2.date_departure);	
}

void calculate_arrival_time_and_date(flight *new_flight, time duration); 
{
	int result_value;

	new_flight->arrival_time = add_time(new_flight->departure_date, new_flight->departure_time, duration);
	
	return;
}
