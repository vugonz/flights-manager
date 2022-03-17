/*  File: proj1.c 
 *	Author: Gonçalo Azevedo ist193075
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

int main()
{
	manager system = {0};
	
	while(command_handler(&system))
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
	
	if(c == '\n') {
		list_airports(system);
	} else {
		list_airports_by_id(system);
	}
}

