/*  Author: Gonçalo Azevedo 193075
 *  File: time.c
 */
#include "header.h"
#include <stdio.h>

time create_time(short hour, short minute)
{
	time new_time;

	new_time.hour = hour;
	new_time.minute = minute;

	return new_time;
}

/* returns negative is t1 is a time set before t2,
 0 if t1 and t2 are the sime time and id
positive if t1 is a time set after t2 */
int time_compare(time t1, time t2)
{
	return same_minute(t1, t2) ? 0 :
		!(DIFF_HOURS(t1, t2)) ? DIFF_MINUTES(t1, t2) : DIFF_HOURS(t1, t2); 
}

/* returns 1 if given duration is valid for a flight and 0 if not */
int is_valid_duration(time duration)
{
	return duration.hour < FLIGHT_MAX_HOUR_DURATION ? 1 :
		duration.hour == FLIGHT_MAX_HOUR_DURATION && !duration.minute ? 1 : 0; 
}

/*
 * auxiliary functions
 */

/* returns 1 if given times are the same minute and hour */
int same_minute(time t1, time t2)
{
	return !(DIFF_HOURS(t1, t2)) && !(DIFF_MINUTES(t1, t2)) ? 1 : 0;
}

