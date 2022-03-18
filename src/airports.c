/*	File: airports.c Author: Gonçalo Azevedo ist193075 */ 
#include "header.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int add_airport(manager *system, char *id, char *country, char *city)
{
	airport new_airport;
	
	/* max airport limit reached */
	if(system->nr_airports > MAX_AIRPORTS)
		return -1;

	/* check if airport id is valid */
	if(!is_valid_airport_id(id))
		return -2;

	/* check if airport exists */
	if(exists_airport_id(system, id))
		return -3;

	/* create new airport */
	new_airport = create_airport(id, country, city);
	/* insert new airport in sorted order in list */
	insert_airport(system->airports, new_airport, system->nr_airports);

	++system->nr_airports;

	return 0;
}


airport create_airport(char *id, char *country, char *city)
{
	airport new_airport = {0};
	
	strcpy(new_airport.id, id);
	strcpy(new_airport.country, country);
	strcpy(new_airport.city, city);

	return new_airport;
}


int is_valid_airport_id(char *id)
{
	int i;
	int len = strlen(id);

	for(i = 0; i < len; ++i)
		if(!isupper(id[i]))
			return 0;

	return 1;
}


int exists_airport_id(manager *system, char *id)
{
	int i;

	for(i = 0; i < system->nr_airports; ++i)
		if(!strcmp(system->airports[i].id, id))
			return 1;

	return 0;
}


void insert_airport(airport *l, airport new_airport, int size)
{
	int i;
	
	l[size] = new_airport;

	/* insert new element in sorted position by id (insertion sort adaptation) */
	for(i = size - 1; i >= 0; --i) {	
		if(strcmp(new_airport.id, l[i].id) > 0) {
			l[i+1] = new_airport;
			break;
		} else
			l[i+1] = l[i];
	
		/* if ever reached with i = 0 means new element is the first */
		if(i == 0)
			l[0] = new_airport;
	}
	
}


void list_airports(manager *system)
{
	int i;

	for(i = 0; i < system->nr_airports; ++i)
		print_airport(system->airports[i]);
}


void list_airports_by_id(manager *system)
{
	char id[AIRPORT_LENGTH_ID];
	char c;
	
	/* print all airport's by id in stdin until no more ids are available */
	do { 	
		scanf(AIRPORT_IDS_PARSE, id, &c); 
		if(!exists_airport_id(system, id))
			printf(LIST_AIRPORTS_ERR, id);
		else
			print_airport(*get_airport_by_id(system, id));
	} while(c != '\n');
}


void print_airport(airport airport)
{
	printf(AIRPORT_PRINT_STR, airport.id, airport.city, airport.country);
}


airport *get_airport_by_id(manager *system, char *id)
{
	int i;
	
	for(i = 0; i < system->nr_airports; ++i)
		if(!strcmp(system->airports[i].id, id))
			return &system->airports[i];

	return 0;
}
