/*  Author: Gonçalo Azevedo 193075
 *  File: proj1.c
 */
#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	/* initialize global structure */
	manager system = {0};
	initialize(&system);

	/* keep reading commands until 'q' command is read */
	while(command_handler(&system))
		;

	free_all_memory(&system);

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
			handle_flights(system);
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
		case 'r':
			handle_reservations(system);
			break;
		case 'e':
			handle_eliminate(system);
			break;
		/* ignore unknown commands and unhandled '\n' from stdin */
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
void handle_flights(manager *system)
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
	int capacity;
	date d;
	time t;
	time dur;
	int result_value;

	/* parse flight id, origin and destionation airports ids*/
	scanf(FLIGHT_MEMBERS_PARSE, id, origin, destination); 

	/* parse flight date and time departure members */
	scanf(DATE_MEMBERS_PARSE, &d.day, &d.month, &d.year);

	scanf(TIME_MEMBERS_PARSE, &t.hour, &t.minute);
	/* initialize departure component */

	/* parse flight duration time members */
	scanf(TIME_MEMBERS_PARSE, &dur.hour, &dur.minute);
	/* initialize duration component */

	/* parse flight capacity */
	scanf("%d", &capacity);

	result_value = add_flight(system, id, origin, destination,
			&d, &t, &dur, capacity);

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
	date d;
	int result_value;

	scanf(DATE_MEMBERS_PARSE, &d.day, &d.month, &d.year);

	result_value = forward_date(system, &d);

	if(result_value == -1)
		printf(FORWARD_DATE_ERR);
}

/*
 * Handles 'r' command
 */
void handle_reservations(manager *system)
{
	char buffer[MAX_COMMAND_SIZE];
	char *buffer_aux = buffer;
	char flight_id[FLIGHT_LENGTH_ID];
	date d = {0};

	fgets(buffer, MAX_COMMAND_SIZE, stdin);

	read_date_and_flight_id(&buffer_aux, flight_id, &d);
	
	if(*buffer_aux == '\n') {
		list_reservations(system, flight_id, &d);
	} else {
		handle_add_reservation(system, buffer_aux, flight_id, &d);
	}
}

/*
 * Handles add reservation
 */
void handle_add_reservation(manager *system, char *buffer, char *flight_id, date *d)
{
	char *reservation_id = NULL;
	int nr_passengers;
	int result_value;

	/* analyze reservation id from buffer*/
	result_value = evaluate_reservation_id(buffer);

	/* if reservation id is invalid */
	if(result_value == -1) {
		printf(ADD_RESERVATION_ERR1);
		return;
	}

	/* allocate memory for reservation's id */
	reservation_id = (char *)malloc((result_value + 1) * sizeof(char));
	/* if no memory, terminate*/
	if(reservation_id == NULL)
		terminate_program(system);
	
	/* get reservation id and nr_passengers from buffer */
	sscanf(buffer, "%s %d", reservation_id, &nr_passengers);

	/* adds reservation */
	result_value = validate_reservation(system, reservation_id, flight_id, d, nr_passengers);

	if(result_value == -2) {
		printf(ADD_RESERVATION_ERR2, flight_id);
	} else if(result_value == -3) {
		printf(ADD_RESERVATION_ERR3, reservation_id);
	} else if(result_value == -4) {
		printf(ADD_RESERVATION_ERR4);
	} else if(result_value == -5) {
		printf(ADD_RESERVATION_ERR5);
	} else if(result_value == -6) {
		printf(ADD_RESERVATION_ERR6);
	}

	/* if reservation was not added, free the memory of id */
	if(result_value != 0)
		free(reservation_id);
}

void handle_eliminate(manager *system)
{
	char id[MAX_COMMAND_SIZE];
	int size;
	int result_value;

	scanf("%s", id);

	/* evaluate id in buffer and allocate memory */
	size = strlen(id);

	result_value = eliminate(system, id, size);

	if(result_value == -1)
		printf(ELIMINATE_ERR);
}

int eliminate(manager *system, char *id, int size)
{
	return (size >= 10) ? remove_reservation(system, id) :
		remove_flight(system, id);
}


/*
 * Returns pointer to newly initialized global structure
 */
void initialize(manager *system)
{
	/* initializes system's date with defined configuration */
	system->date.day = DAY_0;
	system->date.month = MONTH_0;
	system->date.year = YEAR_0;
}

/*
 * Forwards system's date if given date is valid
 */
int forward_date(manager *system, date *d)
{
	if(!is_valid_date(&system->date, d))
		return -1;

	system->date = *d;

	print_date(d);
	printf("\n");

	return 0;
}

void bubblesort(manager *system, int indexes[], int size, int (*cmp_func) (manager *system, int a, int b)) {
	int i, j, done;
  
	for (i = 0; i < size - 1; ++i){
		done = 1;
		for (j = size - 1; j > i; --j) 
			if ((*cmp_func)(system, indexes[j - 1], indexes[j])) {
				int aux = indexes[j];
				indexes[j] = indexes[j - 1];
				indexes[j - 1] = aux;
				done = 0;
			}
		if(done)
			break;
	}
}

void free_all_memory(manager *system)
{
	int i;
	
	/* destroy all reservation's lists and nodes */
	for(i = 0; i < system->nr_flights; ++i) {
		if(system->flights[i].nr_reservations > 0) {
			destroy_list(system->flights[i].reservations);
		} else {
			free(system->flights[i].reservations);
		}
	}	
}

void terminate_program(manager *system)
{
	printf("No memory\n");

	free_all_memory(system);

	exit(EXIT_SUCCESS);
}
