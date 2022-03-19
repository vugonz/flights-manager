#include "header.h"

time create_time(short hour, short minute)
{
	time new_time = {0};

	new_time.hour = hour;
	new_time.minute = minute;

	return new_time;
}

/* returns negative is t2 is a time set before t1, 0 if t1 and t2 are the sime time and positive if t2 is a time set after t1 */
int time_compare(time t1, time t2)
{
	return same_time(t1, t2) ? 0 : same_hour(t1, t2) ? DIFF_MINUTES(t2, t1) : DIFF_HOURS(t2, t1); 
}

/* returns 1 if given duration is valid for a flight and 0 if not */
int is_valid_duration(time duration)
{
	return duration.hour < FLIGHT_MAX_HOUR_DURATION ? 1 : duration.hour == FLIGHT_MAX_HOUR_DURATION && duration.minute == 0? 1 : 0; 
}

/*
 * auxiliary functions
 */

/* returns 1 if given times are the same hour */
int same_hour(time t1, time t2)
{
	return !(DIFF_HOURS(t1, t2)) ? 1 : 0;
}

/* returns 1 if given times are the same minute and hour */
int same_time(time t1, time t2)
{
	return !(DIFF_MINUTES(t1,t2)) && same_hour(t1, t2) ? 1 : 0;
}
