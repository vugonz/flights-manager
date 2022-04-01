/*  Author: Gonçalo Azevedo 193075
 *  File: schedule.c
 */
#include "header.h"

/*
 * Returns a schedule structure with given arguments as members
 */
schedule create_schedule(time t, date d)
{
	schedule schedule;

	schedule.time = t;
	schedule.date = d;

	return schedule;
}

/*
 * Returns negative if s1 is a schedule set before s2
 * Returns 0 if s1 and s2 are the same schedule
 * Returns positive if s1 is a schedule set after s2
 */
int schedule_cmp(schedule s1, schedule s2)
{
	return same_day(s1.date, s2.date) ?
		time_cmp(s1.time, s2.time) : date_cmp(s1.date, s2.date);
}

/*
 * Returns schedule that is the result of adding t_inc time to given schedule
 */
schedule calculate_arrival(schedule s, time t_inc)
{
	schedule arrival;
	time t = s.time;
	date d = s.date;

	if((t.minute = SUM_MINUTES(t, t_inc)) >= MAX_MINUTES) {
		t.minute -= MAX_MINUTES;
		t.hour++;
	}

	if((t.hour = SUM_HOURS(t, t_inc)) >= MAX_HOURS) { 
		t.hour -= MAX_HOURS;
		d.day++;
	}
	if(d.day > DAYS_IN_MONTH(d.month)) {
		d.day -= DAYS_IN_MONTH(d.month);
		d.month++;
	}
	if(d.month > MAX_MONTHS){
		d.month -= MAX_MONTHS;
		d.year++;
	}
	
	arrival = create_schedule(t, d);

	return arrival;
}
