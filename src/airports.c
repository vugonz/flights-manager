/*  Author: Gonçalo Azevedo 193075
 *  File: airports.c
 */
#include "header.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Adds airports to global structure if all given parameters are valid
 * If invalid parameters are given, returns an error value to handle function
 */
int add_airport(manager *system, char *id, char *country, char *city)
{
	airport new_airport;
	
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
	new_airport = create_airport(id, country, city);
	/* insert new airport in sorted order in list */
	insert_airport(system->airports, new_airport, system->nr_airports);

	++system->nr_airports;
	
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
 * Inserts given airport in alphabetically sorted airport's list with "size" elements
 */
void insert_airport(airport *l, airport new_airport, int size)
{
	int i;
	
	l[size] = new_airport;

	/* inserts new element alphabetically sorted in list with insertion sort */
	for(i = size - 1; i >= 0; --i) {	
		if(strcmp(new_airport.id, l[i].id) > 0) {
			l[i+1] = new_airport;
			break;
		} else
			l[i+1] = l[i];

		/* if ever reached with i = 0 then new element belongs in the first position */
		if(i == 0)
			l[i] = new_airport;
	}
}

/*
 * Lists all airports in the system in alphabetical order
 */
void list_airports(manager *system)
{
	int i;
	
	/* print all airports in the system */
	for(i = 0; i < system->nr_airports; ++i)
		print_airport(system->airports[i]);
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
		if(!exists_airport_id(system, id)) {
			printf(LIST_AIRPORTS_ERR, id);
		} else 
			print_airport(*get_airport_by_id(system, id));
	} while(res == 2 && c != '\n');
}

/*
 * Returns an airports structure with given arguments as members
 */
airport create_airport(char *id, char *country, char *city)
{
	airport new_airport;
	
	strcpy(new_airport.id, id);
	strcpy(new_airport.country, country);
	strcpy(new_airport.city, city);
	new_airport.nr_flights = 0;

	return new_airport;
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

	return 0;
}

/*
 * Formatted print of given airport structure
 */
void print_airport(airport airport)
{
	printf(AIRPORT_PRINT_STR, airport.id, airport.city, airport.country, airport.nr_flights);
}
