/*  Author: Gonçalo Azevedo 193075
 *  File: proj1.c
 */
#include "header.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
	/* initialize global structure */
	manager *system = initialize();
	
	/* keep reading commands until 'q' command is read */
	while(command_handler(system))
		;

	return 0;
}

/*
 * Handle commands from stdin and returns 1 to caller function
 * When 'q' command is read, returns 0
 */
int command_handler(manager *system)
{
	char command = getchar();

	switch(command) {
		case 'q':
			return 0;
		case 'a':
			handle_add_airport(system);
			break;
		case 'l':
			handle_list_airports(system);
			break;
		case 'v':
			handle_v_command(system);
			break;
		case 'p':
			handle_list_flight_by_airport(system, command);
			break;
		case 'c':
			handle_list_flight_by_airport(system, command);
			break;
		case 't':
			handle_forward_date(system);
			break;
		/* ignore unknown commands */
		default:
			break;
	}

	return 1;
}

/*
 * Reads airport structure's members from stdin and, if input is valid, adds a new airport
 * If input is invalid, prints error message
 */
void handle_add_airport(manager *system)
{
	char id[AIRPORT_LENGTH_ID];
	char country[AIRPORT_LENGTH_COUNTRY];
	char city[AIRPORT_LENGTH_CITY];
	int result_value;

	scanf(AIRPORT_MEMBERS_PARSE, id, country, city);

	result_value = add_airport(system, id, country, city);

	if(result_value == -1) {
		printf(ADD_AIRPORT_ERR_1);
	} else if(result_value == -2) {
		printf(ADD_AIRPORT_ERR_2);
	} else if(result_value == -3) {
		printf(ADD_AIRPORT_ERR_3);
	} else { 
		printf(ADD_AIRPORT_SUCCESS, id);
	}
}

/*
 * Handles 'l' command with or without optional arguments
 * If optional arguments are specified, lists specified airports
 * If no optional argument are specified, lists all airports alphabetically sorted
 */
void handle_list_airports(manager *system)
{
	char c = getchar();
	
	if(c == '\n') {
		list_airports(system);
	} else {
		list_airports_by_id(system);
	}
}

/*
 * Handles 'v' command with or without optional arguments
 * If optional arguments are specified, go to handle_add_flight function
 * If no optional arguments are specified, lists all flights sorted by creation
 */
void handle_v_command(manager *system)
{
	char c = getchar();

	if(c == '\n') {
		list_flights(system);
	} else {
		handle_add_flight(system);
	}
}


/*
 * Reads flights structure from stdin and, if input is valid, adds a new flight
 * If input is invalid, prints error message
 */
void handle_add_flight(manager *system)
{
	char id[FLIGHT_LENGTH_ID];
	char origin[AIRPORT_LENGTH_ID], destination[AIRPORT_LENGTH_ID];
	int nr_passengers;
	short day, month, year, hour, minute;
	schedule departure;
	time duration;
	int result_value;
	
	/* parse flight id, origin and destionation airports ids*/
	scanf(FLIGHT_MEMBERS_PARSE, id, origin, destination); 
	
	/* parse flight date and time departure members */
	scanf(DATE_MEMBERS_PARSE, &day, &month, &year);
	scanf(TIME_MEMBERS_PARSE, &hour, &minute);
	/* initialize departure component */
	departure = create_schedule(create_time(hour, minute), create_date(day, month, year));

	/* parse flight duration time members */
	scanf(TIME_MEMBERS_PARSE, &hour, &minute);
	/* initialize duration component */
	duration = create_time(hour, minute);
	
	/* parse flight capacity */
	scanf("%d", &nr_passengers);

	result_value = add_flight(system, id, origin, destination, departure, duration, nr_passengers);
	
	if(result_value == -1) {
		printf(ADD_FLIGHT_ERR_1);
	} else if(result_value == -2) {
		printf(ADD_FLIGHT_ERR_2);
	} else if(result_value == -3 || result_value == -4) {
		/* same error, -3 for origin airport invalid id and -4 for destination airport invalid id */
		printf(ADD_FLIGHT_ERR_3N4, result_value == -3 ? origin : destination);
	} else if(result_value == -5) {
		printf(ADD_FLIGHT_ERR_5);
	} else if(result_value == -6) {
		printf(ADD_FLIGHT_ERR_6);
	} else if(result_value == -7) {
		printf(ADD_FLIGHT_ERR_7);
	} else if(result_value == -8) {
		printf(ADD_FLIGHT_ERR_8);
	}
}

/*
 * Lists flights in given airports.
 * If 'p' command is given as argument, lists flights arriving in airport ID read from stdin
 * If 'c' command is given as argument, lists flights departing from airport ID read from stdin
 */
void handle_list_flight_by_airport(manager *system, char command)
{
	char airport_id[AIRPORT_LENGTH_ID];
	int result_value;
	
	/* read airport id from stdin */
	scanf("%s", airport_id);
	
	result_value = list_flights_by_airport(system, airport_id, command);

	if(result_value == -1)
		printf(LIST_AIRPORTS_ERR, airport_id);
}

/*
 * Reads a date structure from stdin and, if input is valid, forwards system's date to specified date
 * If input is invalid, prints error message
 */
void handle_forward_date(manager *system)
{
	date new_date;
	short day, month, year;
	int result_value;

	scanf(DATE_MEMBERS_PARSE, &day, &month, &year);
	new_date = create_date(day, month, year);
	
	result_value = forward_date(system, new_date);

	if(result_value == -1)
		printf(FORWARD_DATE_ERR);
}

/*
 * Returns pointer to newly initialized global structure
 */
manager *initialize()
{
	manager *system = calloc(1, sizeof *system);

	/* initializes system's date with defined configuration */
	system->date = create_date(DAY_0, MONTH_0, YEAR_0);
	
	return system;
}

/*
 * Forwards system's date if given date is valid
 */
int forward_date(manager *system, date new_date)
{
	if(!is_valid_date(new_date, system->date))
		return -1;

	system->date = new_date;
	
	print_date(new_date);

	return 0;
}
