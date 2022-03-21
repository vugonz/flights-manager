#include "header.h"

schedule create_schedule(time t, date d)
{
	schedule schedule;

	schedule.time = t;
	schedule.date = d;

	return schedule;
}

int compare_flight_schedules(schedule s1, schedule s2)
{
	/* same schedule instant */
	if(!date_compare(s1.date, s2.date) && !time_compare(s1.time, s2.time))
		return 0;
	
	return !date_compare(s1.date, s2.date) ? time_compare(s1.time, s2.time) : date_compare(s1.date, s2.date);
}

void calculate_arrival_schedule(flight *flight, departure, time t_inc)
{
	time t = departure.time;
	date d = departure.date;

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

	flight->arrival = create_schedule(t, d);

	return;
}

void print_schedule(schedule s)
{
	printf(PRINT_SCHEDULE_S, s.date.day, s.date.month, s.date.day, s.time.hour, s.time.minute); 
}
