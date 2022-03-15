#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
/*...........................................................................*/
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
	char id[AIRPORT_ID_LENGTH];
	char country[AIRPORT_COUNTRY_LENGTH];
	char city[AIRPORT_CITY_LENGTH];
	int result_value;

	scanf(AIRPORT_COMPONENTS_PARSE, id, country, city);
	result_value = add_airport(system, id, country, city);

	if(result_value == 1)
		printf(ADD_AIRPORT_ERROR_1);
	else if(result_value == 2)
		printf(ADD_AIRPORT_ERROR_2);
	else if(result_value == 3)
		printf(ADD_AIRPORT_ERROR_3);
	else
		printf(ADD_AIRPORT_SUCCESS, id);
}

