/*  Author: Gonçalo Azevedo 193075
 *  File: airports.c
 */
#include "header.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Adds a new airport to global structure if all given arguments are valid
 * If invalid arguments are given, returns an error value to handle function
 */
int add_airport(manager *system, char *id, char *country, char *city)
{
	/* check if airport id is valid */
	if(!is_valid_airport_id(id))
		return -1;

	/* check if max amount of airports is reached */
	if(system->nr_airports >= MAX_AIRPORTS)
		return -2;

	/* check if airport id is duplicate */
	if(exists_airport_id(system, id))
		return -3;

	/* create new airport */
	create_airport(system, id, country, city);

	return 0;
}

/*
 * Returns 1 if given ID is a valid airport id
 * Returns 0 if given ID is invalid
 */
int is_valid_airport_id(char *id)
{
	int i;
	int len = strlen(id);

	/* check if any airport id characters is not uppercase */
	for(i = 0; i < len; ++i)
		if(!isupper(id[i]))
			return 0;

	return 1;
}

/*
 * Returns 1 if given ID is already used by an airport
 * Returns 0 if given ID is not in use
 */
int exists_airport_id(manager *system, char *id)
{
	int i;

	/* checks if an airport already exists with given id */
	for(i = 0; i < system->nr_airports; ++i)
		if(!strcmp(system->airports[i].id, id))
			return 1;

	return 0;
}

/*
 * Recieves indices of airports and returns 1 if airport a is bigger than airport b
 */
int cmp_airports(manager *system, int a, int b)
{
	return (strcmp(system->airports[a].id, system->airports[b].id) > 0);
}

/*
 * Lists all airports in the system in alphabetical order
 */
void list_airports(manager *system)
{
	int i;
	int index_airports[MAX_AIRPORTS];

	for(i = 0; i < system->nr_airports; ++i)
		index_airports[i] = i;

	bubblesort(system, index_airports, system->nr_airports, cmp_airports);

	/* print all airports in the system */
	for(i = 0; i < system->nr_airports; ++i)
		print_airport(system->airports[index_airports[i]]);
}

/*
 * Reads airports' IDs from stdin and prints them until not more IDs are available
 * If ID read doesn't exist, prints an error message and procceds reading the stdin
 */
void list_airports_by_id(manager *system)
{
	char id[AIRPORT_LENGTH_ID];
	char c;
	int res;
	/* print all airport's by ids read in stdin until no more ids are available */
	do {
		res = scanf("%s%c", id, &c);
		/* prints error if airport doesn't exist */
		if(!exists_airport_id(system, id)) {
			printf(LIST_AIRPORTS_ERR, id);
		} else
			print_airport(*get_airport_by_id(system, id));
	} while(res == 2 && c != '\n');
}

/*
 * Returns an airports structure with given arguments as members
 */
void create_airport(manager *system, char *id, char *country, char *city)
{
	airport new_airport;

	strcpy(new_airport.id, id);
	strcpy(new_airport.country, country);
	strcpy(new_airport.city, city);
	new_airport.nr_flights = 0;
	
	system->airports[system->nr_airports] = new_airport;
	system->nr_airports++;
}

/*
 * Returns pointer to airport structure with given ID 
 */
airport *get_airport_by_id(manager *system, char *id)
{
	int i;

	for(i = 0; i < system->nr_airports; ++i)
		if(!strcmp(system->airports[i].id, id))
			return &system->airports[i];

	return NULL;
}

/*
 * Formatted print of given airport structure
 */
void print_airport(airport airport)
{
	printf(AIRPORT_PRINT_STR, airport.id, airport.city, airport.country, airport.nr_flights);
}
