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
	
	/* insert in flights list, will be ordered by time of creation if new items appended to the end */
	system->flights[nr->flights] = new_flight;
	/* insert flight in list of flights sorted by departure */
	insert_flight(system->sorted_departure_flights, new_flight, system->nr_flights);
	/* insert flight in list of flights sorted by arrival */
	insert_flight(system->sorted_arrival_flights, new_flight, system->nr_flights);

	++system->nr_flights;

	return 0;
}

/* inserts flight in list sorted by creation and list sorted by date and time departure */
void insert_flight(flight *l, flight new_flight, int size)
{
	int i;

	/* list will always be sorted by creation date as long as new elements are appended to the end */
	l[size] = new_flight;	
	
	for(i = size - 1; i >= 0; --i) {
		if(compare_flight_times(l[i], new_flight) > 0) {
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
int compare_flight_times(time t1, time t2)
{
	/* same departure instant */
	if(!date_compare(t1, t1) && !time_compare(t1, t2))
		return 0;
	
	return !date_compare(t1, t2) ? time_compare(t1, t2) : date_compare(t1, t2);	
}

/* adds time to the given time and date instant */
void calculate_arrivals(flight *flight, date d, time t, time t_inc)
{
	t.hour = SUM_MINUTES(t, t_inc) >= MAX_MINUTES ? INC(SUM_HOURS(t, t_inc)) : SUM_HOURS(t, t_inc);
	t.minute = SUM_MINUTES(t, t_inc) >= MAX_MINUTES ? SUM_MINUTES(t, t_inc) - MAX_MINUTES : SUM_MINUTES(t, t_inc);
	
	/* adjust date if addition goes to the next day */
	if(t.hour >= MAX_HOURS) {
		t.hour -= MAX_HOURS;
		d.day = (INC(d.day) > DAYS_IN_MONTH(d.month)) ? d.day - DAYS_IN_MONTH(d.month) : INC(d.day);
		d.month = (INC(d.month) > MAX_MONTHS) ? d.month - MAX_MONTHS : d.month;
		d.year = (d.month > MAX_MONTHS) ? INC(d.year) : d.year;
	}

	flight->date_arrival = create_date(d.day, d.month, d.year);
	flight->time_arrival = create_time(t.minute, t.hour);

	return;
}
