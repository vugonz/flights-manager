#include "header.h"
#include <stdio.h>

time create_time(short hour, short minute)
{
	time new_time = {0};

	new_time.hour = hour;
	new_time.minute = minute;

	return new_time;
}


int is_valid_duration(time duration)
{
	return duration.hour < 12 ? 1 : !duration.minute ? 1 : 0;
}

