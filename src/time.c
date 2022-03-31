/*
 *  Author: Gonçalo Azevedo 193075
 *  File: time.c
 */
#include "header.h"
#include <stdio.h>

/*
 * Returns a time structure with given arguments as members
 */
time create_time(short hour, short minute)
{
	time new_time;

	new_time.hour = hour;
	new_time.minute = minute;

	return new_time;
}

/*
 * Returns negative if t1 is a time set before t2
 * Returns 0 if t1 and t2 are the same time
 * Returns positive if t1 is a time set after t2
 */
int time_cmp(time t1, time t2)
{
	return !(DIFF_HOURS(t1, t2)) ? DIFF_MINUTES(t1, t2) : DIFF_HOURS(t1, t2); 
}

/*
 * Returns 1 if duration is valid, that is, if it doesn't exceed the limit of 12 hours
 * Returns 0 if duration is invalid
 */
int is_valid_duration(time duration)
{
	return duration.hour < FLIGHT_MAX_HOUR_DURATION ? 1 :
		duration.hour == FLIGHT_MAX_HOUR_DURATION && !duration.minute ? 1 : 0; 
}
