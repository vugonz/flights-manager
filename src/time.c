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
	return duration < MAX_FLIGHT_HOUR_DURATION ? 1 : duration == MAX_FLIGHT_DURATION && !duration.time ? 1 : 0; 
}


/* adds time to the given time and date instant */
void add_time(date *d, time *t, time t_inc)
{
	t.hour = SUM_MINUTES(t, t_inc) >= MAX_MINUTES ? SUM_HOURS(t, t_inc) + 1 : SUM_HOURS(t, t_inc);
	t.minute = SUM_MINUTES(t, t_inc) >= MAX_MINUTES ? SUM_MINUTES(t, t_inc) - MAX_MINUTES : SUM_MINUTES(t, t_inc);
	
	/* adjust date if addition goes to the next day */
	if(t.hour >= MAX_HOURS) {
		t.hour -= MAX_HOURS;
		d.day = INC(d.day) > DAYS_IN_MONTH(d.month) ? d.day - DAYS_IN_MONTH(d.month) : INC(d.day);
		d.month = INC(d.month) > MAX_MONTHS ? d.month - MAX_MONTHS : d.month;
		d.year = d.month > MAX_MONTHS ? INC(d.year) : d.year;
	}

	return;
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
