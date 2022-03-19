#include "header.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int add_flight(manager *system, char *id, char *origin, char *destination, 
		date date_departure, time time_departure, time duration, int nr_passengers)
{
	flight new_flight;

	/* max airport limit reached */
	if(system->nr_flights > MAX_FLIGHTS)
		return -1;

	/* check if flight id is valid */
	if(!is_valid_flight_id(id))
		return -2;
	
	/* check if flight id is taken for the day */
	if(is_taken_flight_id(system, id, date_departure))
		return -3;

	if(!exists_airport_id(system, origin))
		return -4;

	if(!exists_airport_id(system, destination))
		return -5;
	
	if(!is_valid_date(system->date, date_departure))
		return -6;

	if(!is_valid_duration(duration))
		return -7;
	
	/* check if plane capacity is met in range [10,100] */
	if(nr_passengers < MIN_FLIGHT_CAPACITY || nr_passengers > MAX_FLIGHT_CAPACITY)
		return -8;
	
	new_flight = create_flight(id, origin, destination, 
			date_departure, time_departure, duration, nr_passengers);

	insert_flight(system, new_flight);
	
	get_airport_by_id(system, origin)->nr_flights++;

	++system->nr_flights;

	return 0;
}

/* inserts flight in list sorted by creation and list sorted by date and time departure */
void insert_flight(manager *system, flight new_flight)
{
	/* list will always be sorted by creation date as long as new elements are appended in end */
	system->flights[system->nr_flights] = new_flight;	

	insert_sorted_flight(system->sorted_departure_flights, new_flight, system->nr_flights, compare_flight_departure);
	insert_sorted_flight(system->sorted_arrival_flights, new_flight, system->nr_flights, compare_flight_arrival); 
}

void insert_sorted_flight(flight *l, flight new_flight, int size, int (*cmp_func)(flight f1, flight f2))
{
	int i;
	
	l[size] = new_flight;

	for(i = size - 1; i >= 0; --i) {
		if(cmp_func(l[i], new_flight) > 0) {
			l[i+1] = new_flight;
			break;
		} else
			l[i+1] = l[i];

		/* if ever reached with i = 0 then new element belongs in the first position */
		if(i == 0)
			l[i] = new_flight;
	}
}

int is_valid_flight_id(char *id)
{
	char c1, c2;
	int n;

	if(sscanf(id, "%c%c%d", &c1, &c2, &n) == 3) 
		return isupper(c1) && isupper(c2) && n >= 0 ? 1 : 0;
	
	return 0;
}


int is_taken_flight_id(manager *system, char *id, date date)
{
	int i;
	
	/* check if any flight has the same id and, if so, check if they have the same date */
	for(i = 0; i < system->nr_flights; ++i)
		if(!strcmp(system->flights[i].id, id) && !date_compare(system->flights[i].date_departure, date))
			return 1;

	return 0;
}

void list_flights(manager *system)
{
	int i;

	for(i = 0; i < system->nr_flights; ++i)
		print_flight(system->flights[i]);
}

void print_flight(flight flight)
{
	printf(PRINT_FLIGHT_STR, flight.id, flight.origin, flight.destination,
			flight.date_departure.day, flight.date_departure.month, flight.date_departure.year,
			flight.time_departure.hour, flight.time_departure.minute);
}

flight create_flight(char *id, char *origin, char *destination,
		date date_departure, time time_departure, time duration, int nr_passengers)
{
	flight new_flight = {0};
	
	strcpy(new_flight.id, id);
	strcpy(new_flight.origin, origin);
	strcpy(new_flight.destination, destination);
	new_flight.nr_passengers = nr_passengers;
	new_flight.date_departure = date_departure;
	new_flight.time_departure = time_departure;
	
	calculate_arrivals(&new_flight, new_flight.date_departure, new_flight.time_departure, duration);

	return new_flight;
}

/* returns negative if f2 departs before f1, 0 if flights depart at the same instant and positive if f2 deprats after f1 */
int compare_flight_departure(flight f1, flight f2)
{
	/* same departure instant */
	return compare_flight_schedules(f1.date_departure, f1.time_departure, f2.date_departure, f2.time_departure);
}

/* returns negative if f2 arrives before f1, 0 if flights arrive at the same instant and positive if f2 arrives after f1 */
int compare_flight_arrival(flight f1, flight f2)
{
	/* same departure instant */
	return compare_flight_schedules(f1.date_arrival, f1.time_arrival, f2.date_arrival, f2.time_arrival);
}

/* returns negative if the instant of date d2 and time t2 are set before the instant of date d1 and time t1 */ 
/* wrapper function */
int compare_flight_schedules(date d1, time t1, date d2, time t2)
{
	/* same departure instant */
	if(!date_compare(d1, d2) && !time_compare(t1, t2))
		return 0;
	
	return !date_compare(d1, d2) ? time_compare(t1, t2) : date_compare(d1, d2);
}

/* adds time to the given time and date instant */
void calculate_arrivals(flight *flight, date d, time t, time t_inc)
{
	if((t.minute = SUM_MINUTES(t, t_inc)) >= MAX_MINUTES) {
		t.minute -= MAX_MINUTES;
		t.hour++;
	}

	if((t.hour = SUM_HOURS(t, t_inc)) >= MAX_HOURS) { 
		t.hour -= MAX_HOURS;
		d.day++;
	}
	if(d.day > DAYS_IN_MONTH(d.month)) {
		d.day -= DAYS_IN_MONTH(d.month);
		d.month++;
	}
	if(d.month > MAX_MONTHS){
		d.month -= MAX_MONTHS;
		d.year++;
	}

	flight->date_arrival = create_date(d.day, d.month, d.year);
	flight->time_arrival = create_time(t.hour, t.minute);

	return;
}

int list_flights_by_airport(manager *system, char *airport_id, char command)
{
	int size = system->nr_flights;

	if(!exists_airport_id(system, airport_id))
		return -1;
	
	if(command == DEPARTURE) {
		list_airport_flights_by_departure(system->sorted_departure_flights, airport_id, size);
	} else
		list_airport_flights_by_arrival(system->sorted_arrival_flights, airport_id, size);
	
	return 0;
}

void list_airport_flights_by_departure(flight *l, char *airport_id, int size)
{
	int i;

	for(i = 0; i < size; ++i)
		if(!strcmp(l[i].origin, airport_id))
			print_flight_in_airport(l[i].id, l[i].destination, l[i].date_departure, l[i].time_departure);
}

void list_airport_flights_by_arrival(flight *l, char *airport_id, int size)
{
	int i;

	for(i = 0; i < size; ++i)
		if(!strcmp(l[i].destination, airport_id))
			print_flight_in_airport(l[i].id, l[i].origin, l[i].date_arrival, l[i].time_arrival);
}

void print_flight_in_airport(char *id, char *airport, date d, time t)
{
	printf(PRINT_FLIGHT_IN_AIRPORT_STR, id, airport, d.day, d.month, d.year, t.hour, t.minute);
}
