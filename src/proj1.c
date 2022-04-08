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
		case 'r':
			handle_reservations(system);
			break;
		case 'e':
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
	date departure_date;
	time departure_time;
	time duration;
	int result_value;

	/* parse flight id, origin and destionation airports ids*/
	scanf(FLIGHT_MEMBERS_PARSE, id, origin, destination); 

	/* parse flight date and time departure members */
	scanf(DATE_MEMBERS_PARSE, &day, &month, &year);
	departure_date.day = day;
	departure_date.month = month;
	departure_date.year = year;

	scanf(TIME_MEMBERS_PARSE, &hour, &minute);
	/* initialize departure component */
	departure_time.hour = hour;
	departure_time.minute = minute;

	/* parse flight duration time members */
	scanf(TIME_MEMBERS_PARSE, &hour, &minute);
	/* initialize duration component */
	duration.hour = hour;
	duration.minute = minute;

	/* parse flight capacity */
	scanf("%d", &nr_passengers);

	result_value = add_flight(system, id, origin, destination,
			departure_date, departure_time, duration, nr_passengers);

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
	new_date.day = day;
	new_date.month = month;
	new_date.year = year;

	result_value = forward_date(system, new_date);

	if(result_value == -1)
		printf(FORWARD_DATE_ERR);
}

void handle_reservations()
{
	char *buffer;
	char **buffer_aux = &buffer;
	char flight_id[FLIGHT_LENGTH_ID];
	date d = {0};
	
	buffer = (char*)malloc(sizeof(char) * 65000);

	fgets(buffer, MAX_COMMAND_SIZE, stdin);

	read_date_and_flight_id(buffer_aux, flight_id, &d);
	
	if(**buffer_aux == '\n') {
		return;
	} else {
		handle_add_reservation(*buffer_aux, flight_id, &d);
	}
	free(buffer);
}

/* 
 * Reads flight id and date from given buffer string and sets given flight_id and date with read input
 * Returns given buffer char* pointing to character after read input
 */
void read_date_and_flight_id(char **buffer, char *flight_id, date *d)
{
	/* ignore trailling whitspaces */
	*buffer = ignore_whitespaces(buffer);
	
	/* get flight id */
	sscanf(*buffer, "%s", flight_id);
	*buffer += strlen(flight_id);
	
	/* remove trailling whitespaces */
	*buffer = ignore_whitespaces(buffer);

	/* get date members */
	sscanf(*buffer, DATE_MEMBERS_PARSE, &d->day, &d->month, &d->year);
	*buffer += 10;

	/* ignore trailling whitespaces */
	*buffer = ignore_whitespaces(buffer);
}
	
void handle_add_reservation(char *buffer, char *flight_id, date *d)
{
	char *reservation_id = NULL;
	int nr_passengers;

	/* read reservation id */
	read_reservation_id(&buffer, &reservation_id);

	if(reservation_id == NULL)
		return;

	/* read nr_passengers */
	sscanf(buffer, "%d", &nr_passengers);

	printf("%s", reservation_id);
	printf("\n%d", nr_passengers);
	printf("\n%s", flight_id);
	printf("%hd", d->day);
}

void read_reservation_id(char **buffer, char **reservation_id)
{
	int i = 0;

	/* get reservation_id length */
	while(*(*buffer + i) != ' ' && *(*buffer + 1) != '\t')
		++i;

	/* allocate memory for reservation's id */
	*reservation_id = malloc((i + 1) * sizeof(char));
	if(*reservation_id == NULL)
		return;

	/* get reservation id in allocated memory */
	sscanf(*buffer, "%s", *reservation_id);

	*buffer += i;
}

/* 
 * Skips trailing whitspaces in given buffer and updates buffer to next character that is not a whitespace
 */
char* ignore_whitespaces(char **buffer)
{
	int i = 0;

	while(*(*buffer + i) == ' ' || *(*buffer + i) == '\t')
		i++;

	return *buffer += i;
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
int forward_date(manager *system, date new_date)
{
	if(!is_valid_date(system->date, new_date))
		return -1;

	system->date = new_date;

	print_date(new_date);
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
