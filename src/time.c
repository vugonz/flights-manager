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

int is_valid_duration(time duration)
{
	return duration < MAX_FLIGHT_HOUR_DURATION ? 1 : duration == MAX_FLIGHT_DURATION && !duration.time ? 1 : 0; 
}


/* returns time that is a result of adding t1 and t2 */
time add_time(time t1, time t2)
{
	t1.hour = SUM_MINUTES(t1, t2) >= MAX_MINUTES ? SUM_HOURS(t1, t2) + 1 : SUM_HOURS(t1, t2);
	t1.minute = SUM_MINUTES(t1, t2) >= MAX_MINUTES ? SUM_MINUTES(t1, t2) - MAX_MINUTES : SUM_HOURS(t1, t2);

	return t1;
}

/* auxiliary functions */
int same_hour(time t1, time t2)
{
	return !(DIFF_HOURS(t1, t2)) ? 1 : 0;
}
int same_time(time t1, time t2)
{
	return !(DIFF_MINUTES(t1,t2)) && same_hour(t1, t2) ? 1 : 0;
}
