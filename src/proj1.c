/*	File: proj1.c 
 *	Author: Gonçalo Azevedo ist193075
 */
#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	manager *system = initialize();
	
	while(command_handler(system))
		;

	return 0;
}

int command_handler(manager *system)
{
	char c = getchar();

	switch(c) {
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
			break;
		case 'c':
			break;
		case 't':
			break;
		default:
			break;
	}
	return 1;
}

void handle_add_airport(manager *system)
{
	char id[AIRPORT_LENGTH_ID];
	char country[AIRPORT_LENGTH_COUNTRY];
	char city[AIRPORT_LENGTH_CITY];
	int result_value;

	scanf(AIRPORT_COMPONENTS_PARSE, id, country, city);

	result_value = add_airport(system, id, country, city);

	if(result_value == -1)
		printf(ADD_AIRPORT_ERR_1);
	else if(result_value == -2)
		printf(ADD_AIRPORT_ERR_2);
	else if(result_value == -3)
		printf(ADD_AIRPORT_ERR_3);
	else
		printf(ADD_AIRPORT_SUCCESS, id);
}

void handle_list_airports(manager *system)
{
	char c = getchar();
	
	if(c == '\n') 
		list_airports(system);
	else 
		list_airports_by_id(system);
}

void handle_v_command(manager *system)
{
	char c = getchar();

	if(c == '\n')
		list_all_flights(system);
	else
		handle_add_flight(system);
}


void handle_add_flight(manager *system)
{
	char id[FLIGHT_LENGTH_ID];
	char origin[AIRPORT_LENGTH_ID], destination[AIRPORT_LENGTH_ID];
	short nr_passengers;
	short day, month, year, hour, minute;
	date flight_date;
	time flight_time;
	time flight_duration;
	int result_value;
	
	/* parse input string and asign buffer structures */
	scanf(FLIGHT_COMPONENTS_PARSE, id, origin, destination); 

	scanf(DATE_COMPONENTS_PARSE, &day, &month, &year);
	flight_date = create_date(day, month, year);

	scanf(TIME_COMPONENTS_PARSE, &hour, &minute);
	flight_time = create_time(hour, minute);

	scanf(TIME_COMPONENTS_PARSE, &hour, &minute);
	flight_duration = create_time(hour, minute);

	scanf(NR_PASSENGERS_PARSE, &nr_passengers);

	result_value = add_flight(system, id, origin, destination, flight_date, flight_time, flight_duration, nr_passengers);
	
	if(result_value == -1)
		printf(ADD_FLIGHT_ERR_1);
	else if(result_value == -2)
		printf(ADD_FLIGHT_ERR_2);
	else if(result_value == -3)
		printf(ADD_FLIGHT_ERR_3);
	else if(result_value == -4 || result_value == -5)
		printf(ADD_FLIGHT_ERR_4N5, result_value == -4 ? origin : destination);
	else if(result_value == -6)
		printf(ADD_FLIGHT_ERR_6);
	else if(result_value == -7)
		printf(ADD_FLIGHT_ERR_7);
	else if(result_value == -8)
		printf(ADD_FLIGHT_ERR_8);
}

manager *initialize()
{
	manager *system = calloc(1, sizeof *system);

	system->date = create_date(1, 1, 2022);
	
	return system;
}
