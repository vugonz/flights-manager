#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "header.h"

int add_airport(manager *system, char *id, char *country, char *city)
{
	int i;
	int len = strlen(id);
	
	if(system->nr_airports > MAX_AIRPORTS)
		return 1;

	/* check if all letters are uppercase in id string */
	for(i = 0; i < len; ++i)
		if(!isupper(id[i]))
				return 2;
	
	/* check for airport with same id string */
	for(i = 0; i < system->nr_airports; ++i)
		if(strcmp(system->airports[i].id, id))
			return 3;
	
	strcpy(system->airports[system->nr_airports].id, id);
	strcpy(system->airports[system->nr_airports].country, country);
	strcpy(system->airports[system->nr_airports].city, city);

	return 0;
}

